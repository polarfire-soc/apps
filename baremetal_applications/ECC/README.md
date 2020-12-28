# Single-Bit Error Dectection and Correction for L2 LIM

## Objective 

This application executes the following functions:

1. Initializes the L2 LIM (Loosely Integrated Memory) so that false errors are not created.
2. Injects an error into the L2 LIM to verify that the detection and correction logic is working.
3. Monitors the Single Error count.

## Description

In the Libero design, L2 Cache is configured as L2 LIM with 8 ways starting from way 15. The L2 LIM can be accessed from 0x0800_0000. MSS MMUART_0 is used in the design for serial communication between the user and the application. For more information about the Libero design, see [ICICLE Kit Reference Design](https://github.com/polarfire-soc/icicle-kit-reference-design).


## Requirements

- ICICLE Kit (MPFS250T-FCVG484E)
- SoftConsole v6.5
- Serial Terminal program (PuTTY or TeraTerm)
- Host PC - Windows 10 OS

## Pre-Requisite

Before running the user application, ensure to complete the following steps:

1. Setting up the [jumpers](https://github.com/polarfire-soc/polarfire-soc-documentation/blob/master/boards/mpfs-icicle-kit-es/updating-icicle-kit/updating-icicle-kit-design-and-linux.md) on the ICICLE Kit.
2. Setting up the Serial Terminal:
 - Select the COM port which is connected to the following interface: Silicon Labs Quad CP2108 USB to UART BRIDGE: Interface 0
 - Set Baud rate to “115200”, Set Data to 8-bit, Set Flow control to None.
3. Use FlashPro Express to program the ICICLE Kit with the job file.
4. Download [SoftConsole project](https://bitbucket.microchip.com/projects/FPGA_PFSOC_ES/repos/apps/browse/baremetal_applications?at=refs%2Fheads%2Fdevelop_12_6_deliverables).

## Running the Application

After the device is programmed, power cycle the board. To run the application, follow these steps:

1. Launch SoftConsole v6.5, open the ECC project.
2. Build the SoftConsole project in release mode. This application is built using ITIM as stack and eNVM as code section. 
3. Select Run > External Tools > PolarFire SoC program non secure boot mode 1 option to program the eNVM with the application and execute it.

The following options are displayed on the Serial Terminal.
``` 
   The L2 cache controller supports ECC for Single-Error Correction 

   Whenever a correctable error is detected

   the cache controller immediately repairs the corrupted bit and writes it back to
   L2-LIM.

   This application shows L2-LIM ECC functionality for single bit error

   Type 0 To show this menu

   Type 1 To initialize L2 LIM Initialization

   Type 2 To print ECC fix count register value

   Type 3 To inject single bit error

   Type 4 To access L2 LIM memory

```

4. On the serial terminal, type 1 to initialize the L2 LIM memory. L2 LIM is initialized with an incremental pattern.
5. Type 2 to get the initial ECC Fix_count register value. The initial ECC Fix_count register content will be a non-zero value.
6. Type 3 to inject single bit error using Error injection register.
7. Type 4 to perform L2 LIM Memory access
8. Type 2 to get the updated ECC Fix count register value. The value should be incremented by 1. 

This indicates that the single-bit error is detected and corrected.












