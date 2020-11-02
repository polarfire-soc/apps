# System services Softconsole Proj

## Objective:

This SoftConsole project demonstrates the execution of PolarFire SoC system services on the MPFS-ICICLE-Kit using the MSS system service driver. Refer to [AC492: PolarFire SoC FPGA System Services Application Note](http://www.microsemi.com/index.php?option=com_docman&task=doc_download&gid=1245407) for detailed description of this application. The associated Libero project and programming file are available at the following locations:

## Programming file and libero project directory:

|Description | Link |
| --- | --- |
| Libero Project | [Link](https://github.com/polarfire-soc/apps/tree/master/linux_applications/Hardware/Libero_Prj) |
| Programming File | [Link](https://github.com/polarfire-soc/apps/tree/master/linux_applications/Hardware/Programming_Job_File) |

## Setting up the hardware

### Setting up the hardware involves the following steps:

1. Set the Jumpers on MPFS-ICICLE-KIT as per the following table

| Jumper | Setting |
| --- 	| --- 	 |
| J15	| Open   |
| J17	| Open   |
| J24	| Open   |
| J28	| Closed |
| J31	| Open   |
| J34	| 1 and 2 |
| J35	| 1 and 2 |
| J43	| 1 and 2 |
| J45	| 1 and 2 |
| J46	| Closed  |
| J9	| Open    |
| J21	| Open    |

- Connect the Kit to Host PC via J11 (USB-UART) using the USB A to Micro B cable.
- Connect the FlashPro 6 (or FP 4 or 5) programmer to the Kit via Program Header (J23) using USB A to Mini B cable.
- Power-up the board.

## Setting up the Serial Terminal:
1. Open Tera Term on Host PC.
2. Select the COM port which is connected to the following interface:
Silicon Labs Quad CP2108 USB to UART BRIDGE: Interface 0
3. Set Baud rate to “115200”, Set Data to 8-bit, Set Flow control to None.

## Programming the PolarFire SoC Device:

Please follow the steps described in [AC492: PolarFire SoC FPGA System Services Application Note](http://www.microsemi.com/index.php?option=com_docman&task=doc_download&gid=1245407)

## Running the System Service Application:

The system services SoftConsole project must be launched in debug mode for running system services. For more information about steps to running the PolarFire SoC system services, see [AC492: PolarFire SoC FPGA System Services Application Note.](http://www.microsemi.com/index.php?option=com_docman&task=doc_download&gid=1245407) 




