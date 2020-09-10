/*******************************************************************************
 * (c) Copyright 2017-2018 Microsemi SoC Products Group. All rights reserved.
 *
 * File : pcie_app.h
 *
 * Description : application and kernel interface
 *
********************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ioctl.h>


#define MPCI_IOC_MAGIC 'm'

#define MPCI_IOCBAR_WRITE    		_IOWR(MPCI_IOC_MAGIC, 0, int)
#define MPCI_IOCBAR_READ 			_IOWR(MPCI_IOC_MAGIC, 1, int)
#define MPCI_IOCCONFIG_WRITE		_IOWR(MPCI_IOC_MAGIC, 2, int)
#define MPCI_IOCCONFIG_READ 		_IOWR(MPCI_IOC_MAGIC, 3, int)
#define MPCI_IOCCONFIG_READ_ALL 	_IOWR(MPCI_IOC_MAGIC, 4, int)
#define MPCI_IOCDEVICE_INFO 		_IOWR(MPCI_IOC_MAGIC, 5, int)
#define MPCI_IOCISR_INFO 			_IOWR(MPCI_IOC_MAGIC, 6, int)


#define MPCI_G5_DMA_CONFIG			_IOWR(MPCI_IOC_MAGIC, 11, int)
#define MPCI_G5_SG_DMA_WRITE		_IOWR(MPCI_IOC_MAGIC, 12, int)
#define MPCI_G5_SG_DMA_READ			_IOWR(MPCI_IOC_MAGIC, 13, int)
#define MPCI_G5_CN_DMA_WRITE		_IOWR(MPCI_IOC_MAGIC, 14, int)
#define MPCI_G5_CN_DMA_READ			_IOWR(MPCI_IOC_MAGIC, 15, int)
#define MPCI_G5_BD_COUNT			_IOWR(MPCI_IOC_MAGIC, 16, int)
#define MPCI_G5_DMA_TRANSFER_LENGTH	_IOWR(MPCI_IOC_MAGIC, 17, int)


#if M_DEBUG
	#define PRINTF printf
#else
	#define PRINTF
#endif


#define LED_GLOW_REG_OFFSET 0x8 /* Led Offset */
#define DIP_SWITCH_STATUS_REGISTER 0x10 /* Dip Switch Offset */

#define DDR_LSRAM_SHIFT_OFFSET 0x8648

#define ON 0
#define OFF 1

#define TRUE 1
#define FALSE 0

#define SUCCESS 0L

#define BAR0 0
#define BAR1 1
#define BAR2 2

#define G5_DMA_WRITE_TH   0x8
#define G5_DMA_READ_TH    0x9
#define G5_FREQ			  0x14

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

enum{
	MENU_CDMA_DDR3_WRITE_OFFSET = 1,
	MENU_CDMA_DDR3_READ_OFFSET,
	MENU_CDMA_DDR3_WRITE_READ_OFFSET,
	MENU_CDMA_DDR4_WRITE_OFFSET,
	MENU_CDMA_DDR4_READ_OFFSET,
	MENU_CDMA_DDR4_WRITE_READ_OFFSET,
	MENU_CDMA_LSRAM_WRITE_OFFSET,
	MENU_CDMA_LSRAM_READ_OFFSET,
	MENU_CDMA_LSRAM_WRITE_READ_OFFSET,
	MENU_SGDMA_DDR3_WRITE_OFFSET,
	MENU_SGDMA_DDR3_READ_OFFSET,
	MENU_SGDMA_DDR3_WRITE_READ_OFFSET,
	MENU_SGDMA_DDR4_WRITE_OFFSET,
	MENU_SGDMA_DDR4_READ_OFFSET,
	MENU_SGDMA_DDR4_WRITE_READ_OFFSET,
	MENU_DMA_THROUGHPUT,
	MENU_DMA_TEST_ALL,
};

enum{
	DMA_BUF_INC = 1,
	DMA_BUF_DEC,
	DMA_BUF_RAND,
	DMA_BUF_ZEROS,
	DMA_BUF_ONES,
	DMA_BUF_AAA,
	DMA_BUF_555
};


enum{
	POLARFIRE_EVAL = 1,
	POLARFIRE_SPL,
};

/* Device type */
enum{
	DEVICE_SF2_FDMA = 1,
	DEVICE_IGLOO2_FDMA,
	DEVICE_SF2_HPDMA,
	DEVICE_IGLOO2_HPDMA,
	DEVICE_SF2_SG,
	DEVICE_RTG4_FDMA,
	DEVICE_G5_PF
};

enum {
	SF2 = 1,
	IGLOO,
	RTG4,
	POLARFIRE,
};

typedef struct {

	u32 bar;
	u32 offset;
	u32 val;

}config_bar;

struct isr_count{
	u32 isr1;
	u32 isr2;
	u32 isr3;
	u32 isr4;
	u32 isr_type_act;
};

struct config_info{
	int offset;
	u16 val_16;
	u32 val_32;
};


/** Structure used in IOCTL to set the normal dma registers */
typedef struct{
	u16 DmaSize;
	u16 DmaDirStart;
	u16 DmaCh0Status;
	u16 DmaCh1Status;
	u16 DmaClkCount;
	u16 DmaAddWrite;
	u16 DmaAddRead;
 }DmaRegisters;
/** Structure used in IOCTL to set the Scatter/gather dma registers */
typedef struct{

	u16 DmaSgReadWrite;
	u16 DmaSgStart;
	u16 DmaSgBDCh0;
	u16 DmaSgBDCh1;
	u16 DmaSgSRCH0;
	u16 DmaSgSRCH1;

 }DmaSgRegisters;
/** Structure used in IOCTL to set the LED  registers */
typedef struct{
	u32 Led1offset;
	u32 Led2offset;
	u32 Led3offset;
	u32 Led4offset;
	u32 Led5offset;
	u32 Led6offset;
	u32 Led7offset;
	u32 Led8offset;
	u32 LedGlowoffset;
 }LedRegisters;

 typedef struct{
	u32 Gpio9offset;
	u32 Gpio10offset;
	u32 Gpio11offset;
	u32 Gpio12offset;
	u32 DipSwitchStatus;
}DipSwitchRegisters;

typedef struct{
	u32 Devicetype;
	u32 Dmasupport;
	u32 ControlPlan;
	u32 Dma;
}devtype;

typedef struct{
	unsigned int       Dma_Srcaddr32;
	unsigned int       Dma_Srcaddr64;
	unsigned int       Dma_Destaddr32;
	unsigned int       Dma_Destaddr64;
	unsigned int       Dma_length;
	unsigned int       Dma_Status;
}G5_DmaRegisters;


struct device_info{
	int device_status;
	int device_type;
	int demo_type;
	int support_width;
	int n_width;
	int support_speed;
	int n_speed;
	int num_bar;
	u32 bar0_add;
	int bar0_size;
	u32 bar1_add;
	int bar1_size;
	u32 bar2_add;
	int bar2_size;
	u32 bar3_add;
	int bar3_size;
	u32 bar4_add;
	int bar4_size;
	u32 bar5_add;
	int bar5_size;
	u32 bar6_add;
	int bar6_size;
};

struct dma_operation{
	u32 throughput_tx;//pc to ddr
	u32 throughput_rx;//ddr to pc
	u32 bd_tx;
	u32 bd_rx;

};


typedef struct{

	u32 Status;
	DmaSgRegisters DmasgR;
	DmaRegisters   DmaR;
	LedRegisters   LedR;
	DipSwitchRegisters DipSR;
	devtype Devtype;
	int Led_status ;
	int DipS_status;
	int Dma_status ;
	int dmatpye ;

}mpci;


typedef struct{
	DmaSgRegisters DmasgR;
	DmaRegisters   DmaR;
	LedRegisters   LedR;
	DipSwitchRegisters DipSR;
	devtype Devtype;
	int Led_status ;
	int DipS_status;
	int Dma_status ;
	int dmatpye ;
	int Barw_status;
	int BAR_LSRAM_status;
	int BAR_DDR3_status;
	int BAR_DDR4_status;
	int lab_w_status,lab_r_status;
}globle_dev_info;


struct pci_config{
	u16  VendorID;
	u16  DeviceID;
	u16  Command;
	u16  Status;
	u32  RevisionID;
	u8   Clase_prog;
	u16  Class_device;
	u8   CacheLineSize;
	u8   LatencyTimer;
	u8   HeaderType;
	u8   BIST;
	u32  BaseAddresses[6];
	u32  CIS;
	u16  Sub_VID;
	u16  Sub_DID;
	u32  ROMBaseAddress;
	u8   IRQ_Line;
	u8   IRQ_Pin;
	u8   Min_Gnt;
	u8   Max_Lat;


struct _PCI_MSI_MESSAGE_CONTROL{
    u16  MSIEnable:1;
    u16  MultipleMessageCapable:3;
    u16  MultipleMessageEnable:3;
    u16  CapableOf64Bits:1;
    u16  PerVectorMaskCapable:1;
    u16  Reserved:7;
  } MessageControl;
};
