# MicroPython

## Objective

The scope of the project is to build MicroPython for PolarFire SoC RISC-V architecture and execute the MicroPython on the ICICLE Kit.�

## Description

The following figure shows the block diagram of the design.

![](./images/running_micropython.jpg)

This design uses a bootloader application which runs on E51 core to copy the MicroPython application from Host PC to LPDDR4 using ymodem. Then, the bootloader switches the execution to MicroPython. The MicroPython is executed by U54 core from LPDDR4. The MSS UART and MSS GPIO are integrated into MicroPython Source code to show basic functionality with Python commands. The Command line interface (CLI) of MicroPython will be shown on serial terminal program.

## Requirements

- ICICLE Kit (MPFS250T_ES-FCVG484E)
- SoftConsole v6.5
- Serial Terminal program (PuTTY or TeraTerm)
- Host PC - Windows 10 OS

## Pre-Requisite

Before running the user application, ensure to complete the following steps:
1. Setting up the [jumpers](https://github.com/polarfire-soc/polarfire-soc-documentation/blob/master/boards/mpfs-icicle-kit-es/updating-icicle-kit/updating-icicle-kit-design-and-linux.md) on the ICICLE Kit.
2. Setting up the Serial Terminal: 
    - Select the COM port which is connected to the following interface:  Silicon Labs Quad CP2108 USB to UART BRIDGE: Interface 0.
    - Set Baud rate to �115200�, Set Data to 8-bit, Set Flow control to None.
3. Use FlashPro Express to program the ICICLE Kit with the [PolarFire SoC ICICLE Kit Reference Design job file](https://github.com/polarfire-soc/polarfire-soc-documentation/blob/master/boards/mpfs-icicle-kit-es/updating-icicle-kit/updating-icicle-kit-design-and-linux.md).�
4. Download the provided Bootloader application [softconsole_project.7z](https://bitbucket.microchip.com/projects/FPGA_PFSOC_ES/repos/apps/browse/baremetal_applications/MicroPython/softconsole_project.7z?at=refs%2Fheads%2Fdevelop_12_6_deliverables).
5. Download the [micropython.bin](https://bitbucket.microchip.com/projects/FPGA_PFSOC_ES/repos/apps/browse/baremetal_applications/MicroPython/micropython.bin?at=develop_12_6_deliverables), this is the cross-compiled binary image which can be used to run the demo of MicroPython. If customization is required for adding more peripherals, then the MicroPython source with required changes needs to be cross-compiled as described in Cross-Compiling MicroPython for PolarFire SoC ICICLE Kit (RISC-V Architechture) section below.
 
## Running the Application�

After the device is programmed, power cycle the board. Build and launch the SoftConsole project in Debug mode. In Debug mode, the bootloader application runs from loosely integrated memory (LIM). The bootloader application prints the menu on the Tera Term program through the UART interface, as shown in following figure. This program waits to load the micropython.bin to LPDDR4 over ymodem.

Note: This SoftConsole project can also be built in release mode and run from eNVM. Select Run > External Tools > PolarFire SoC program non secure boot mode 1 option to program the eNVM with the application and execute it.�

![](./images/micropython_printscreen_1.png)

To run the demo, perform the following steps:

1. On Tera Term, select File > Transfer > YMODEM > Send to transfer micropython.bin as shown in the following figure. The Tera Term: YMODEM Send Dialog appears.

![](./images/micropython_printscreen_2.png)

2. Browse the micropython.bin and click Open as shown in the following figure.

![](./images/micropython_printscreen_3.png)

3. The file is transferred as shown in the following figure. It takes around 30 sec.

![](./images/micropython_printscreen_4.png)

4. After transferring micropython.bin, bootloader switches the execution to the MicroPython. U54 starts executing MicroPython from LPDDR4 and the MicroPython command prompt appears on Tera Term as shown in the following figure.

![](./images/micropython_printscreen_5.png)

5. Using the MicroPython, you can turn ON and OFF the LEDs. Following are the commands to turn ON and OFF the LEDs.�

- from machine import Pin
- led = Pin((''GPIO2_0'', 16), Pin.OUT)
- led.value(1) 
- led.value(0) 

![](./images/micropython_printscreen_6.png)

6. �Using the Micropython, you can perform mathematical operations as shown in the following figure.

![](./images/micropython_printscreen_7.png)

This concludes running the MicroPython application on ICICLE kit. The SOftConsole Debug session can be terminated.

## Cross-Compiling MicroPython for PolarFire SoC ICICLE Kit (RISC-V Architecture)

Ensure to install prebuilt toolchain for RISC-V or SoftConsole v6.5 on the Linux host machine. Download the [icicle_kit_micropython_source.zip](https://bit.ly/3seuYGV).

To cross compile the MicroPython, follow:

1. Export the toolchain path.
```
   export PATH=$PATH:/path to Microchip/SoftConsole-v6.5/riscv-unknown-elf-gcc/bin
```  
   For example, 
``` 
   export PATH=$PATH:</home/microchip/Microchip/SoftConsole-v6.5/riscv-unknown-elf-gcc/bin
```

2. Build the MicroPython.

   Extract the file from icicle_kit_micropython_source.zip.
```
   cd icicle_kit_micropython_source/micropython_release_1_9_3
```
   cd path to micropython folder/micropython_release_softfloat/ports/icicle_miv.

   For example, 
```
   cd micropython_release_1_9_3/micropython_release_softfloat/ports/icicle_miv

   make clean

   make CROSS=2 V=1
```   
3. Generate the hex file.
```
   cd build

   riscv64-unknown-elf-objcopy -O ihex micropython.elf firmware.hex --change-section-lma *-0x80000000
```   
4. Generate the bin file.
```
   cd build

   riscv64-unknown-elf-objcopy -O binary -j .isr_vector -j .text -j .data� micropython.elf micropython.bin
```   
On successful compilation of source code, the output file (micropython.bin) is generated.


