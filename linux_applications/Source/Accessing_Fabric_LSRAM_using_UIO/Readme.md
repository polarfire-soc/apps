# Accessing Fabric LSRAM using UIO

## Objective:

Using this application, Read's and Write's to memory mapped Fabric LSRAM is performed.

## Description:

In the Libero design, Fabric LSRAM component is interfaced to the MSS using FIC_0 and it is accessible to the processors at 0x61000000.  For more information on Libero design, refer AC489 Application Note.

The pre-built Linux image includes the following customization to access LSRAM from user space.

- A device tree node (uio-generic) is added with LSRAM memory address and size of 64KB in the device tree file.
- UIO framework is enabled in the Linux configuration file (defconfig).
- User application performs LSRAM memory test using uio-dev node (/dev/uio).

## Hardware Requirements:

- ICICLE Kit (MPFS250T-FCVG484EES)
- Host PC - Windows 10 OS

## Pre-Requisite:

Before running this application, please follow AC489 Application Note and ensure the following steps are completed:

1. Setting up the ICICLE kit
2. Programming the provided job file [Programming_Job_File](https://github.com/polarfire-soc/apps/blob/master/linux_applications/Hardware/Programming_Job_File/Programming_file.zip)
3. Programming the provided Linux image [mpfs-dev-cli-icicle-kit-es-AC489_linux.rootfs.wic](https://bit.ly/3ln5K4Y).
4. Booting Linux

## Running the User Application:

This application is part of Linux pre-built image [mpfs-dev-cli-icicle-kit-es-AC489_linux.rootfs.wic](https://bit.ly/3ln5K4Y).

The LSRAM user application (LSRAM_read_write) is available under /microchip-apps directory in rootfs.


```
root@icicle-kit-es:~# cd /microchip-apps/
```

Type the ./LSRAM_read_write command and Press Enter to execute the application.


```
root@icicle-kit-es:/microchip-apps# ./LSRAM_read_write
         # Choose one of  the following options:
         Enter 1 to perform memory test on LSRAM
         Enter 2 to Exit  
```

Enter 1 to perform memory test on LSRAM.
After successful completion of memory test on LSRAM, "LSRAM memory test passed successfully" message is displayed on console.


```
root@icicle-kit-es:/microchip-apps# ./LSRAM_read_write
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

