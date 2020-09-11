# PCIe Control Plane Linux Application

## Objective
In this Linux Application, PolarFire SoC Device (ICICLE Kit) acts as the Root Port (RP) to access the PolarFire Evaluation Kit as the End Point (EP). 

The following Control Plane functions can be performed:

1. Fetching EP Device information.
2. Blinking LED's on the EP.
3. Receiving DIP switch status from EP.
4. Reading and Writing to the EP BAR space.
5. Fetching EP Config space information.

## Description
In the Root Port libero design, PolarFire SoC PCIe Controller is configured as Root Port for Gen2 x4 lane with a data rate of 5 Gbps. Refer to AC489 Application Note for more information on Root Port Libero design.

In the End Point libero design, PolarFire PCIe Controller is configured as End Point with the same parameters as Root Port. In the End Point design, the onboard LEDs and DIP Switches are enabled so that they can be controlled through from PCIe Root Port via PCIe link.

PolarFire SoC PCIe Root Port is configured as a PCIe bridge to establish a PCIe link with the PolarFire FPGA PCIe End Point (EP).

The pre-built Linux image includes the following customization to access the EP from the user application.

- A device tree node is added for RP in the device tree file. - (add a note about the address region for PCIe node)
- A driver for RP controller is added to establish PCIe link with the EP .
- The PCIe RP controller is enabled in the Linux configuration file (defconfig).
- PCIe EP driver is integrated.
- User application performs control plane functions on EP.

## Hardware Requirements:

- Root Port HW - ICICLE Kit (MPFS250T-FCVG484EES)
- End Point HW - PolarFire Evaluation Kit (MPF300-EVAL-KIT-ES)
- Host PC - Windows 10 OS

## Pre-Requisite:

Connect the EP (PolarFire Eval Kit) to the RP (ICICLE Kit) PCIe Slot.

![](./images/rp_endpoint.png)

1. Switch ON the power supply for ICICLE Kit and PolarFire Eval Kit.
2. Program the EP Kit with the provided job file (PCIe_EP_EVALKit.job) using Flash pro express software by selecting the allotted port.
3. Program the ICICLE KIT with the provided job file (ICICLE_RP.job) using external flash pro 6 hardware by selecting the allotted port with Flash pro express software.
4. Program the provided Linux pre-built image [mpfs-dev-cli-icicle-kit-es-AC489_linux.rootfs.wic](ftp://ftp.actel.com/outgoing/AC489/mpfs-dev-cli-icicle-kit-es-AC489_linux.rootfs.wic.gz).
5. Boot Linux
6. Ensure both RP and EP are detected by executing the lspci command as shown below.

```
root@icicle-kit-es:~# lspci
PCI bridge: Actel Device 1556
RAM memory: Actel Device 1557
```
7. Ensure EP driver is inserted by executing the lsmod command as shown below

```
root@icicle-kit-es:~# lsmod
Module Size Used by
mpci 38567 0
```

## Running the user application to test the endpoint device 

Go to the apps folder

```
root@icicle-kit-es:# cd /microchip-apps/MS_PCI_APP
```

execute the ./pcie_app command 

the welcome menu is displayed, as shown in the following figure.

```
root@icicle-kit-es:/microchip-apps/MS_PCI_APP# ./pcie_app
welcome to PCI Demo
---------------------------------------------
1.Read EP Device Info
2.Blink EP LEDs
3.Display EP Switch Status
4.Read/Write to EP bar Space
5.Display EP Config Space
```

## 1. Device info

Enter 1 to get EP device information 

```
root@icicle-kit-es:/microchip-apps/MS_PCI_APP# ./pcie_app
welcome to PCI Demo
---------------------------------------------
1.Read EP Device Info
2.Blink EP LEDs
3.Display EP Switch Status
4.Read/Write to EP bar Space
5.Display EP Config Space
1
---------------------------------------------
demo_type = PolarFire PCIe Demo
device_status = Microsemi Device Detected
device_type = PolarFire Evaluation kit
number of BARs enabled = 2
bar0_add = 0x7830000c
bar0_size = 10000
bar2_add = 0x7820000c
bar2_size = 100000

```

## 2. Blink LED's 

Enter 2 to blink EP led's

```
1.Read EP Device Info
2.Blink EP LEDs
3.Display EP Switch Status
4.Read/Write to EP bar Space
5.Display EP Config Space
2
---------------------------------------------
Blinking LED's success for Loop:1 out of 10 iterations
Blinking LED's success for Loop:2 out of 10 iterations
Blinking LED's success for Loop:3 out of 10 iterations
Blinking LED's success for Loop:4 out of 10 iterations
Blinking LED's success for Loop:5 out of 10 iterations
Blinking LED's success for Loop:6 out of 10 iterations
Blinking LED's success for Loop:7 out of 10 iterations
Blinking LED's success for Loop:8 out of 10 iterations
Blinking LED's success for Loop:9 out of 10 iterations
Blinking LED's success for Loop:10 out of 10 iterations
```

## 3. Dip switch status

Enter 3 to get EP DIP Switch status
```
1.Read EP Device Info
2.Blink EP LEDs
3.Display EP Switch Status
4.Read/Write to EP bar Space
5.Display EP Config Space
3
---------------------------------------------
DIP SWITCH1 : ON
DIP SWITCH2 : OFF
DIP SWITCH3 : ON
DIP SWITCH4 : ON

```

## 4. Reading and writing to the EP bars space.

Enter 4 to read and write to the bar address space of EP

```
1.Read EP Device Info
2.Blink EP LEDs
3.Display EP Switch Status
4.Read/Write to EP bar Space
5.Display EP Config Space
4
---------------------------------------------
provide type 1.DDR-3
1
1. Read from bar space
2. Write to bar space
2
Enter the offset
0x10
Provide the data to write
0xa0
Write successful

1.Read EP Device Info
2.Blink EP LEDs
3.Display EP Switch Status
4.Read/Write to EP bar Space
5.Display EP Config Space
4
---------------------------------------------
provide type 1.DDR-3
1
1. Read from bar space
2. Write to bar space
1
Enter the offset
0x10
read value = 0xa0
```

## 5. Display EP Config space.

Enter 5 then enter 2 to read from PCIE Config space

```
1.Read EP Device Info
2.Blink EP LEDs
3.Display EP Switch Status
4.Read/Write to EP bar Space
5.Display EP Config Space
5
---------------------------------------------
1.Read from PCIe ConfigSpace offset
2.Read PCIe ConfigSpace
2
Vendor ID: 0x11aa
Device ID: 0x1557
Cmd Reg: 0x406
Stat Reg: 0x10
Revision ID: 0x5000000
Class Prog: 0x0
Device Class: 0x500
Cache Line Size: 0x0
Latency Timer: 0x0
Header Type: 0x0
BIST: 0x0
BAR0: Addr:0x7830000c
BAR1: Addr:0x0
BAR2: Addr:0x7820000c
BAR3: Addr:0x0
BAR4: Addr:0x0
BAR5: Addr:0x0
CardBus CIS Pointer: 0x0
Subsystem Vendor ID: 0x0
Subsystem Device ID: 0x0
Expansion ROM Base Address: 0x0
IRQ Line: 0x18
IRQ Pin: 0x4
Min Gnt: 0x0
Max Lat: 0x0
MSIEnable: 0x1
MultipleMessageCapable: 0x4
MultipleMessageEnable: 0x1
CapableOf64Bits: 0x1
PerVectorMaskCapable: 0x0

```

Press Ctl-C to exit the Linux application.

