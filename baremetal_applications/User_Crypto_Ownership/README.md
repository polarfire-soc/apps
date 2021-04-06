# Dynamic Switching of User Cryptoprocessor Ownership

## Objective
The scope of the reference design is to perform cryptographic operations such as encryption, decryption, dynamic switching of User Cryptoprocessor ownership from microcontroller subsystem (MSS) to fabric and vice versa using the User Cryptoprocessor as a coprocessor to a host general purpose processor.

Note: Microchip provides an Athena TeraFire Cryptographic Applications Library (CAL) to access User Cryptoprocessor functions. CAL driver requires a license. Email [FPGA_marketing@microchip.com](FPGA_marketing@microchip.com) to request the design files. This design is targeted for PolarFire SoC Icicle kit.

## Description
This reference design shows how to perform switching of the User Cryptoprocessor from PolarFire SoC MSS to Fabric and vice versa. The User Cryptoprocessor can be accessed from MSS or Fabric. The User Cryptoprocessor is a hard block in PolarFire SoC FPGAs and its maximum operating frequency is 200 MHz. It is accessible to MSS, or a soft processor in the FPGA fabric through the AHBLite slave interface for control and primary data input and output. The User Cryptoprocessor has built-in DMA to offload the main processor from performing data transfers between the User Cryptoprocessor and the user memory. The DMA functionality is accessible from fabric through an AMBA AHB-Lite master interface. 

