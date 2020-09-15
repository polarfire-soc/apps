# Applying patches:

Download  below patches
1. 0001-Added-DTS-support-for-PCIe-UIO-GPIO-Fabric-DMA.patch
2. 0002-Added-AC489-user-apps.patch
3. recipes-microchip-apps.tar 

## Create a directory  “patches“ in “yocto-dev” directory of downloaded Yocto source

```
$ cd yocto-dev/
$ mkdir patches 
```
Copy above downloaded patches and “recipes-microchip-apps.tar” file to patches directory.

Make sure that you should be in “meta-polarfire-soc-yocto-bsp” directory of Yocto source which is downloaded from Github to apply patches:

```
$ cd meta-polarfire-soc-yocto-bsp/
```

## Command to apply patch:

#### Apply dts patch:
```
$ git apply ../patches/0001-Added-DTS-support-for-PCIe-UIO-GPIO-Fabric-DMA.patch
```
#### Apply patch to add user applications:
```
$ git apply ../patches/0002-Added-AC489-user-apps.patch
```
Now “recipes-microchip-apps.tar” file is compressed file, un-compress the file.

```
$ tar -xvf ../patches/recipes-microchip-apps.tar
```
Now you can proceed to building the linux image

```
$ cd ../
$ . ./meta-polarfire-soc-yocto-bsp/polarfire-soc_yocto_setup.sh
```
