# Blinking LED's

## Objective: 

Using this application, User LED’s (LED 1, 2, 3 and 4) on the Icicle kit are toggled.
For more information on which GPIO's are used in the Libero design, please refer AC489 Application Note.

## Description:

MSS GPIO_2 is connected to LED's in AC489 Libero design. These LED's are blinked by running Linux user space application.

## Hardware Requirements:

- ICICLE Kit (MPFS250T-FCVG484EES)
- Host PC with Windows 10 OS

## Pre-Requisite:

Before running this application, please follow AC489 Application Note and ensure to complete following:

1. Setting up the ICICLE kit
2. Programming the provided job file [Programming_Job_File](https://github.com/polarfire-soc/apps/blob/master/linux_applications/Hardware/Programming_Job_File/Programming_file.zip)
3. Programming the provided Linux image [mpfs-dev-cli-icicle-kit-es-AC489_linux.rootfs.wic](https://bit.ly/3ln5K4Y).
4. Booting Linux

## Running the Application:

This application is part of Linux pre-built image (.wic).
The led blinky application (./led_blinky) is available under /microchip-apps directory in rootfs

```
root@icicle-kit-es:~# cd /microchip-apps/
```

Type the ./led_blinky command and Press Enter to execute the application.

```
root@icicle-kit-es:/microchip-apps# ./led_blinky                        
        # Choose one of  the following options:
        Enter 1 to blink LEDs 1, 2, 3 and 4
        Press 'Ctrl+c' to Exit
```

Enter 1 to blink LED 1, LED 2, LED 3, and LED 4 on the Icicle Kit

```
root@icicle-kit-es:/microchip-apps# ./led_blinky                      
        # Choose one of  the following options:
        Enter 1 to blink LEDs 1, 2, 3 and 4
        Press 'Ctrl+c' to Exit
1
         LEDs are blinking press 'Ctrl+c' to Exit
```

