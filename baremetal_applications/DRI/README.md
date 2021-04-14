# Dynamic Reconfiguration of Hard IPs

## Objective:

This application demonstrates how to reconfigure PolarFire SoC FPGA Hard IP blocks such as Clock Conditioning Circuits (CCC) and Transceivers without reprogramming the FPGA.

## Description

In the reference design, the south-west CCC IP is configured to generate four fabric output clock frequencies of 100 MHz (PLL OUT0), 75 MHz (PLL OUT1), 50 MHz (PLL OUT2), and 25 MHz (PLL OUT3) respectively. These PLL output clocks are generated from a reference clock of 125 MHz. The CCC IP includes a Dynamic Reconfiguration Interface (DRI), which is enabled for dynamic reconfiguration without reprogramming the device. The PF_DRI block in the fabric is used to interface the CCC's DRI bus with MSS FIC3 interface. One of processors in the MSS can dynamically modify the CCC configuration register values through FIC3 interface.

The following figure shows the top-level block diagram of the design.

![](./images/PFSOC_CCC_DRI.png)

Per [PolarFire SoC Register map](http://www.microsemi.com/index.php?option=com_docman&task=doc_download&gid=1244581), the address offset for the south-west CCC registers starts at 0x08400000. Since these CCC registers are accessed through FIC3 path the base address for the CCC registers becomes 0x4840 0000 (0x4000 0000 + 0x0840 0000).

Refer to the bit definitions of the south-west CCC register documented in the PolarFire SoC Register map.

