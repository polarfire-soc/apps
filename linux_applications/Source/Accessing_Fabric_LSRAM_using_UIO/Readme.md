# Accessing Fabric LSRAM using UIO

## Objective:

Using this application, Read's and Write's to memory mapped Fabric LSRAM is performed.

## Description:

In the Libero design, Fabric LSRAM component is interfaced to the MSS using FIC_0 and it is accessible to the processors at 0x61000000. More information on the Libero design is available on [GitHub](https://github.com/polarfire-soc/icicle-kit-reference-design).

The pre-built Linux image includes the following customization to access LSRAM from user space.

- A device tree node (uio-generic) is added with LSRAM memory address and size of 64KB in the device tree file.
- UIO framework is enabled in the Linux configuration file (defconfig).
- User application performs LSRAM memory test using uio-dev node (/dev/uio).

## Hardware Requirements:

- ICICLE Kit (MPFS250T-FCVG484EES)
- Host PC - Windows 10 OS

## Pre-Requisite:

Ensure to follow the documentation provided on [GitHub](https://github.com/polarfire-soc/polarfire-soc-documentation/blob/master/boards/mpfs-icicle-kit-es/updating-icicle-kit/updating-icicle-kit-design-and-linux.md) and complete the following steps:
1. Programming the ICICLE KIT reference design
2. Writing the Pre-built Linux Image to eMMC or SD
3. Initiating Linux boot


## Running the User Application:


The LSRAM user application (LSRAM_read_write) is available under /opt/microchip/apps directory in rootfs.


```
root@icicle-kit-es:~# cd /opt/microchip/apps/
```

Type the ./LSRAM_read_write command and Press Enter to execute the application.


```
root@icicle-kit-es:/opt/microchip/apps# ./LSRAM_read_write
         # Choose one of  the following options:
         Enter 1 to perform memory test on LSRAM
         Enter 2 to Exit  
```

Enter 1 to perform memory test on LSRAM.
After successful completion of memory test on LSRAM, "LSRAM memory test passed successfully" message is displayed on console.


```
root@icicle-kit-es:/opt/microchip/apps# ./LSRAM_read_write
         # Choose one of  the following options:
         Enter 1 to perform memory test on LSRAM
         Enter 2 to Exit
1
Writing incremental pattern starting from address 0x61000000
Reading data  starting from address 0x61000000
Comparing data

**** LSRAM memory test passed successfully *****
# Choose one of  the following options:
         Enter 1 to perform memory test on LSRAM
         Enter 2 to Exit
```

Enter 2 to exit the application

