# Icicle Kit AC489 wic image


## Download Linux Image
[mpfs-dev-cli-icicle-kit-es-AC489_linux_rootfs_image](https://bit.ly/3ln5K4Y) 

Downloaded Linux image is a compressed image, so extract the image. Once extraction is successfull mpfs-dev-cli-icicle-kit-es-AC489_linux.rootfs.wic will be generated. Use this image to format ICICLE eMMC using Win32DiskImager tool as mentioned in AC489 document.

The following versions were used to build this image:
1. Yocto (meta-polarfire-soc-yocto-bsp) - v2020.05
	2. HSS - 0.99.9
	3. U-boot - u-boot_2020.05
	4. Linux - mpfs-linux_5.6.16
    
To rebuild the linux image for newer versions of Yocto,HSS,U-boot and Linux please refer to Application Note AC489
