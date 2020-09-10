/*******************************************************************************
 * (c) Copyright 2017-2018 Microsemi SoC Products Group. All rights reserved.
 * File : pcie_appln_ctrlpln.c
 *
 * Description : Linux User Space Application to access Polarfire
 * 	PCIe End Point
 *
********************************************************************************/


#include "pcie_appln.h"
#include <math.h>

#define igr(x) {__typeof__(x) __attribute__((unused)) d=(x);} 
extern int MenuDma(struct dma_operation *dma_op,int type,int tx_size,int rx_size,u32** buf,u32 *pattern);
extern u32 *Global_data;
globle_dev_info GMpci_dev = {0};
int fd = 0;

int clean_stdin()
{
    while(getchar() !='\n');
    return 1;

}

int mpci_bar_read(u32 bar,u32 offset)
{
	config_bar lc_bar;

	lc_bar.bar = bar;
	lc_bar.offset = offset;
	lc_bar.val = 0 ;

	if (ioctl(fd, MPCI_IOCBAR_READ,&lc_bar) == -1){

		PRINTF("error while reading\n");
		return FALSE;

	}
	else {
		PRINTF("mpci_bar_read success\n");

	}
	return lc_bar.val;
}

int mpci_bar_write(u32 bar,u32 offset,u32 data)
{
	config_bar lc_bar;
	lc_bar.bar = bar;
	lc_bar.offset = offset;
	lc_bar.val = data ;

	if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
	{
		PRINTF("error while writing \n");
		return FALSE;
	}
	else
	{
		PRINTF("mpci bar write success\n");
	}
	return TRUE;
}
int main(int argc, char *argv[]) {

  int i = 0,option = 0,loop=0;
  int SW1,SW2,SW3,SW4,led_test,isr_test,bar_RW,buf,data,offset,input,ret;
  int tx_size = 0, rx_size = 0;
  u32 status = 1,pattern =1;
  u32 *buf_test = &status;
  config_bar lc_bar;
  struct isr_count local_isr;
  struct config_info local_info;
  struct device_info mdev;
  struct pci_config local_config;
  struct dma_operation local_dma;

	printf("welcome to PCI Demo\n");

	printf("---------------------------------------------\n");

	fd = open("/dev/MS_PCI_DEV", O_RDWR);

	if( fd < 0 ) {
		printf("PCI Device[/dev/MS_PCI_DEV] not opened [%d] ::: Device Driver Not Inserted\n",fd);
		return -1;
	}
	else {
		//printf("PCI Device [/dev/MS_PCI_DEV] opened\n");
	}

	Global_data = (u32 *) malloc(4*1024*1024);

	GMpci_dev.Led_status = TRUE;
	GMpci_dev.DipS_status = TRUE;
	GMpci_dev.Dma_status = TRUE;
	GMpci_dev.Barw_status = TRUE;
	GMpci_dev.lab_r_status = FALSE;
	GMpci_dev.lab_w_status = FALSE;
	GMpci_dev.BAR_DDR3_status = TRUE;
	GMpci_dev.BAR_DDR4_status = TRUE;
	GMpci_dev.BAR_LSRAM_status = TRUE;

	GMpci_dev.Devtype.ControlPlan   = TRUE;
	GMpci_dev.Devtype.Devicetype    = POLARFIRE;
	GMpci_dev.Devtype.Dmasupport    = TRUE;
	GMpci_dev.Devtype.Dma           = DEVICE_G5_PF;

	while(1)
	{                   
               
		printf("1.Read EP Device Info  \n");
		printf("2.Blink EP LEDs  \n");
		printf("3.Display EP Switch Status \n");
	//	printf("4.Receive EP Interrupts  \n");
		printf("4.Read/Write to EP bar Space\n");
		printf("5.Display EP Config Space \n");
	        

		if (!(scanf("%d",&input)))
		{
			clean_stdin();
//			printf("please  enter numeric values only");

		}
		switch(input)
		{
		case 1:
			printf("---------------------------------------------\n");
			{

				//mdev = (struct device_info *)malloc(sizeof(struct device_info));

				if (ioctl(fd, MPCI_IOCDEVICE_INFO,&mdev) == -1)
				{
					printf("error while reading\n");
				}
				else
				{

					printf("demo_type = %s\n","PolarFire PCIe Demo");

					if(mdev.device_status)
					printf("device_status = Microsemi Device Detected\n");
					else
					printf("device_status = Microsemi Device Not Detected\n");

					switch(mdev.device_type)
					{
					case POLARFIRE_EVAL:
					printf("device_type = PolarFire Evaluation kit\n");
					break;
					case POLARFIRE_SPL:
					printf("device_type = PolarFire splash kit\n");
					break;
					default:
					printf("device_type = Unknown \n");

					}
					printf("number of BARs enabled    = %d\n",mdev.num_bar);
					printf("bar0_add      = 0x%x\n",mdev.bar0_add);
					printf("bar0_size     = %x\n",mdev.bar0_size);
					printf("bar2_add      = 0x%x\n",mdev.bar2_add);
					printf("bar2_size     = %x\n",mdev.bar2_size);
				}
				printf("\n");

			}
		break;
		case 2:
			printf("---------------------------------------------\n");

		//	printf("Enter Led data\n");
#if 0
			if (!(scanf("%d",&led_test)))
			{
				clean_stdin();
				printf("please  enter numeric values only");

			}
#endif
			led_test = 255;loop=1;
			while(loop <= 10)
			{
			//	led_test = 255;
			lc_bar.bar = 2;
			lc_bar.offset = LED_GLOW_REG_OFFSET/4;
			lc_bar.val = led_test ;
			if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
			{
				printf("error while reading\n");
			}
			else
			{
				printf("Blinking  LED's success for Loop:%d out of 10 iterations\n",loop);

			}
			sleep(1);
			loop++;
			if(!led_test)
				led_test =255;
			else
				led_test = 0;


			
			}

		printf("\n");
		break;
		case 3:
			// dipswitch
			printf("---------------------------------------------\n");

				lc_bar.bar = 2;
				lc_bar.offset = DIP_SWITCH_STATUS_REGISTER/4;
				lc_bar.val = 0 ;
				if (ioctl(fd, MPCI_IOCBAR_READ,&lc_bar) == -1){

					printf("error while reading\n");
				}
				else {
					ret = lc_bar.val;
					SW1 = (ret & 0x1)?OFF:ON;
					SW2 = (ret >> 1 & 0x1)?OFF:ON;
					SW3 = (ret >> 2 & 0x1)?OFF:ON;
					SW4 = (ret >> 3 & 0x1)?OFF:ON;

					//printf("SW1 = %d\nSW2 = %d\nSW3 = %d\nSW4 = %d\n",SW1,SW2,SW3,SW4);

					if(SW1 == OFF)
					{
							printf("\nDIP SWITCH1 : OFF\n");
					}else
					{
							printf("\nDIP SWITCH1 : ON\n");
					}

					if(SW2 == OFF)
					{
							printf("DIP SWITCH2 : OFF\n");
					}else
					{
							printf("DIP SWITCH2 : ON\n");
					}

					if(SW3 == OFF)
					{
							printf("DIP SWITCH3 : OFF\n");
					}else
					{
							printf("DIP SWITCH3 : ON\n");
					}

					if(SW4 == OFF)
					{
							printf("DIP SWITCH4 : OFF");
					}else
					{
							printf("DIP SWITCH4 : ON");
					}
					printf("\n");
				}
		printf("\n");
		break;
	  #	if 0
		case 4:
			printf("---------------------------------------------\n");
			printf("1. for ISR count, 2. for ISR clean\n");
			if (!(scanf("%d",&isr_test)))
			{
				clean_stdin();
				printf("please  enter numeric values only");

			}

			local_isr.isr_type_act = isr_test;

			if (ioctl(fd, MPCI_IOCISR_INFO,&local_isr) == -1)
			{
				printf("error while reading\n");
			}
			else {
				printf("counter 1 = %u\n",local_isr.isr1);
				printf("counter 2 = %u\n",local_isr.isr2);
				printf("counter 3 = %u\n",local_isr.isr3);
				printf("counter 4 = %u\n",local_isr.isr4);
			}
		printf("\n");
		break;
#endif
		case 4:
			printf("---------------------------------------------\n");

		//	printf("provide type 1.DDR-3, 2.LSRAM,3.DDR-4\n");
			printf("provide type 1.DDR-3 \n");
			scanf("%i",&buf);

			switch(buf)
			{
			case 1:
				//DDR3
				lc_bar.bar = 0;
				lc_bar.offset = DDR_LSRAM_SHIFT_OFFSET/4;
				lc_bar.val = 0x20000000;

				if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
				{
					printf("error while reading\n");
					continue;
				}


				printf("1. Read from  bar space\n");
				printf("2. Write to bar space\n");

				if (!(scanf("%d",&bar_RW)))
				{
					clean_stdin();
			//		printf("please  enter numeric values only");

				}
				switch(bar_RW)
				{
				case 1:
						printf("Enter the offset  \n");
						scanf("%i",&offset);

							lc_bar.bar = 2;
							lc_bar.offset = offset/4;

							if (ioctl(fd, MPCI_IOCBAR_READ,&lc_bar) == -1)
							{
								printf("error while reading\n");
							}
							else
							{
								printf("read value = 0x%x\n",lc_bar.val);
							}

				break;

				case 2:
						printf("Enter the offset  \n");
						scanf("%i",&offset);

						printf("Provide the data to write \n");
						scanf("%i",&data);

						lc_bar.bar = 2;
						lc_bar.offset = offset/4;
						lc_bar.val = data;
						if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
						{
							printf("error while reading\n");
						}
						else
						{
							printf("Write successful \n");
						}
				break;
				}

				lc_bar.bar = 0;
				lc_bar.offset = DDR_LSRAM_SHIFT_OFFSET/4;
				lc_bar.val = 0x10000000;

				if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
				{
					printf("error while reading\n");
					continue;
				}

			break;

			case 2:
				//LSRAM
				lc_bar.bar = 0;
				lc_bar.offset = DDR_LSRAM_SHIFT_OFFSET/4;
				lc_bar.val = 0x30000000;

				if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
				{
					printf("error while reading\n");
					continue;
				}


				printf("1. Read from  bar space\n");
				printf("2. Write to bar space\n");

				if (!(scanf("%d",&bar_RW)))
				{
					clean_stdin();
					printf("please  enter numeric values only");

				}
				switch(bar_RW)
				{
				case 1:
						printf("Enter the offset  \n");
						scanf("%i",&offset);

						lc_bar.bar = 2;
						lc_bar.offset = offset/4;

						if (ioctl(fd, MPCI_IOCBAR_READ,&lc_bar) == -1)
						{
							printf("error while reading\n");
						}
						else
						{
							printf("read value = 0x%x\n",lc_bar.val);
						}

				break;

				case 2:
						printf("Enter the offset  \n");
						scanf("%i",&offset);

						printf("Provide the data to write \n");
						scanf("%i",&data);

						lc_bar.bar = 2;
						lc_bar.offset = offset/4;
						lc_bar.val = data;
						if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
						{
							printf("error while reading\n");
						}
						else
						{
							printf("Write successful \n");
						}
				break;
				}
				lc_bar.bar = 0;
				lc_bar.offset = DDR_LSRAM_SHIFT_OFFSET/4;
				lc_bar.val = 0x10000000;

				if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
				{
					printf("error while reading\n");
					continue;
				}
			break;
			case 3:
				//DDR4
				lc_bar.bar = 0;
				lc_bar.offset = DDR_LSRAM_SHIFT_OFFSET/4;
				lc_bar.val = 0x40000000;

				if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
				{
					printf("error while reading\n");
					continue;
				}


				printf("1. Read from  bar space\n");
				printf("2. Write to bar space\n");

				if (!(scanf("%d",&bar_RW)))
				{
					clean_stdin();
					printf("please  enter numeric values only");

				}
				switch(bar_RW)
				{
				case 1:
						printf("Enter the offset  \n");
						scanf("%i",&offset);

						lc_bar.bar = 2;
						lc_bar.offset = offset/4;

						if (ioctl(fd, MPCI_IOCBAR_READ,&lc_bar) == -1)
						{
							printf("error while reading\n");
						}
						else
						{
							printf("read value = 0x%x\n",lc_bar.val);
						}

				break;

				case 2:
						printf("Enter the offset  \n");
						scanf("%i",&offset);

						printf("Provide the data to write \n");
						scanf("%i",&data);

						lc_bar.bar = 2;
						lc_bar.offset = offset/4;
						lc_bar.val = data;
						if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
						{
							printf("error while reading\n");
						}
						else
						{
							printf("Write successful \n");
						}
				break;
				}
				lc_bar.bar = 0;
				lc_bar.offset = DDR_LSRAM_SHIFT_OFFSET/4;
				lc_bar.val = 0x10000000;

				if (ioctl(fd, MPCI_IOCBAR_WRITE,&lc_bar) == -1)
				{
					printf("error while reading\n");
					continue;
				}
			break;
			}
		printf("\n");
		break;
		case 5:

			printf("---------------------------------------------\n");

			printf("1.Read from PCIe ConfigSpace offset \n");
			printf("2.Read PCIe ConfigSpace \n");
			scanf("%i",&buf);

			switch(buf)
			{
			case 1:

				printf("Enter the offset  \n");
				scanf("%i",&offset);
				local_info.offset = offset;
				local_info.val_32 = 0;
				local_info.val_16 = 1;
				if (ioctl(fd, MPCI_IOCCONFIG_READ,&local_info) == -1)
				{
					printf("error while reading\n");
					continue;
				}

				printf("read value = 0x%x\n",local_info.val_32);
				printf("read value = 0x%x\n",local_info.val_16);


				break;
			case 2:


					if (ioctl(fd, MPCI_IOCCONFIG_READ_ALL,&local_config) == -1)
					{
						printf("error while reading\n");
						continue;
					}
					printf("Vendor ID: 0x%x \n", local_config.VendorID);
					printf("Device ID: 0x%x \n", local_config.DeviceID);
					printf("Cmd Reg: 0x%x \n", local_config.Command);
					printf("Stat Reg: 0x%x \n", local_config.Status);
					printf("Revision ID: 0x%x \n", local_config.RevisionID);
					printf("Class Prog: 0x%x \n", local_config.Clase_prog);
					printf("Device Class: 0x%x \n", local_config.Class_device);
					printf("Cache Line Size: 0x%x \n", local_config.CacheLineSize);
					printf("Latency Timer: 0x%x \n", local_config.LatencyTimer);
					printf("Header Type: 0x%x \n", local_config.HeaderType);
					printf("BIST: 0x%x\n", local_config.BIST);
					for(i=0; i<=5; i++)
					{
						printf("BAR%d: Addr:0x%x \n", i, local_config.BaseAddresses[i]);
					}
					printf("CardBus CIS Pointer: 0x%x \n", local_config.CIS);
					printf("Subsystem Vendor ID: 0x%x \n", local_config.Sub_VID);
					printf("Subsystem Device ID: 0x%x \n", local_config.Sub_DID);
					printf("Expansion ROM Base Address: 0x%x \n", local_config.ROMBaseAddress);
					printf("IRQ Line: 0x%x \n", local_config.IRQ_Line);
					printf("IRQ Pin: 0x%x \n", local_config.IRQ_Pin);
					printf("Min Gnt: 0x%x \n", local_config.Min_Gnt);
					printf("Max Lat: 0x%x \n", local_config.Max_Lat);

					printf("MSIEnable: 0x%x \n", local_config.MessageControl.MSIEnable);

					ret = pow(2,local_config.MessageControl.MultipleMessageCapable);
					printf("MultipleMessageCapable: 0x%x \n",ret);
					ret = pow(2,local_config.MessageControl.MultipleMessageEnable);
					printf("MultipleMessageEnable: 0x%x \n",ret);
					printf("CapableOf64Bits: 0x%x \n", local_config.MessageControl.CapableOf64Bits);
					printf("PerVectorMaskCapable: 0x%x \n", local_config.MessageControl.PerVectorMaskCapable);




				break;

			}
			printf("\n");
			break;
#if 0 //HALTED to support DMA		
		case 7:

			printf("---------------------------------------------\n");
			printf("\n");
			printf("Read/write to memeory using DMA\n");
			printf("--------------------------------------------\n");
			printf("%d. G5 Continuous Dma Write to DDR3\n",MENU_CDMA_DDR3_WRITE_OFFSET);
			printf("%d. G5 Continuous Dma Read from DDR3\n",MENU_CDMA_DDR3_READ_OFFSET);
			printf("%d. G5 Continuous Dma write/read DDR3\n",MENU_CDMA_DDR3_WRITE_READ_OFFSET);
			printf("%d. G5 Continuous Dma Write to DDR4\n",MENU_CDMA_DDR4_WRITE_OFFSET);
			printf("%d. G5 Continuous Dma Read from DDR4\n",MENU_CDMA_DDR4_READ_OFFSET);
			printf("%d. G5 Continuous Dma write/read DDR4\n",MENU_CDMA_DDR4_WRITE_READ_OFFSET);
			printf("%d. G5 Continuous Dma Write to LSRAM\n",MENU_CDMA_LSRAM_WRITE_OFFSET);
			printf("%d. G5 Continuous Dma Read from LSRAM\n",MENU_CDMA_LSRAM_READ_OFFSET);
			printf("%d. G5 Continuous Dma write/read LSRAM\n",MENU_CDMA_LSRAM_WRITE_READ_OFFSET);
			printf("%d. G5 SG Dma Write to DDR3\n",MENU_SGDMA_DDR3_WRITE_OFFSET);
			printf("%d. G5 SG Dma Read from DDR3\n",MENU_SGDMA_DDR3_READ_OFFSET);
			printf("%d. G5 SG Dma write/read DDR3\n",MENU_SGDMA_DDR3_WRITE_READ_OFFSET);
			printf("%d. G5 SG Dma Write to DDR4\n",MENU_SGDMA_DDR4_WRITE_OFFSET);
			printf("%d. G5 SG Dma Read from DDR4\n",MENU_SGDMA_DDR4_READ_OFFSET);
			printf("%d. G5 SG Dma write/read DDR4\n",MENU_SGDMA_DDR4_WRITE_READ_OFFSET);

#if 1
			if (!(scanf("%i",&option))){
				clean_stdin();
				printf("please  enter numeric values only");
				continue;
			}
#endif
#if 1
		  while(1){
         printf("Tx size \n");
		     scanf("%d",&tx_size);
         if((tx_size > (1024 *1024)) || (tx_size < 4))
         {
           printf("Please enter size between 4Bytes to 1MBytes \n");
         }
         else{
           break;
         }
      }
      while(1){
  			printf("Rx size \n");
  			scanf("%d",&rx_size);
        if((rx_size > (1024 *1024)) || (rx_size < 4)){
          printf("Please enter size between 4Bytes to 1MBytes \n");
        }
        else{
          break;
        }
      }

			printf("Enter the data pattern 1.inc 2. dec 3.rand 4.zero's 5.one's 6. AAA 7. 555   \n");
			scanf("%i",&pattern);
#endif
			switch (option)
			{
			case MENU_CDMA_DDR3_WRITE_OFFSET:
        MenuDma(&local_dma,MENU_CDMA_DDR3_WRITE_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Write throughput = %d MBPS\n",local_dma.throughput_tx);
        break;
			case MENU_CDMA_DDR3_READ_OFFSET:
				MenuDma(&local_dma,MENU_CDMA_DDR3_READ_OFFSET,tx_size,rx_size,&buf_test,&pattern);
				printf("Read throughput  = %d MBPS\n",local_dma.throughput_rx);
				break;
			case MENU_CDMA_DDR3_WRITE_READ_OFFSET:
        MenuDma(&local_dma,MENU_CDMA_DDR3_WRITE_READ_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Write throughput = %d MBPS\n",local_dma.throughput_tx);
        printf("Read throughput = %d MBPS\n",local_dma.throughput_rx);
				break;
			case MENU_CDMA_DDR4_WRITE_OFFSET:
        MenuDma(&local_dma,MENU_CDMA_DDR4_WRITE_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Write throughput = %d MBPS\n",local_dma.throughput_tx);
				break;
			case MENU_CDMA_DDR4_READ_OFFSET:
        MenuDma(&local_dma,MENU_CDMA_DDR4_READ_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Read throughput  = %d MBPS\n",local_dma.throughput_rx);
				break;
			case MENU_CDMA_DDR4_WRITE_READ_OFFSET:
        MenuDma(&local_dma,MENU_CDMA_DDR4_WRITE_READ_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Write throughput = %d MBPS\n",local_dma.throughput_tx);
        printf("Read throughput  = %d MBPS\n",local_dma.throughput_rx);
				break;
			case MENU_CDMA_LSRAM_WRITE_OFFSET:
        MenuDma(&local_dma,MENU_CDMA_LSRAM_WRITE_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Write throughput = %d MBPS\n",local_dma.throughput_tx);
				break;
			case MENU_CDMA_LSRAM_READ_OFFSET:
        MenuDma(&local_dma,MENU_CDMA_LSRAM_READ_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Read throughput  = %d MBPS\n",local_dma.throughput_rx);
				break;
			case MENU_CDMA_LSRAM_WRITE_READ_OFFSET:
        MenuDma(&local_dma,MENU_CDMA_LSRAM_WRITE_READ_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Write throughput = %d MBPS\n",local_dma.throughput_tx);
        printf("Read throughput  = %d MBPS\n",local_dma.throughput_rx);
				break;
			case MENU_SGDMA_DDR3_WRITE_OFFSET:
        MenuDma(&local_dma,MENU_SGDMA_DDR3_WRITE_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Write throughput = %d MBPS\n",local_dma.throughput_tx);
        printf("Write Buffer Descriptor count = %d\n",local_dma.bd_tx);
				break;
			case MENU_SGDMA_DDR3_READ_OFFSET:
        MenuDma(&local_dma,MENU_SGDMA_DDR3_READ_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Read throughput  = %d MBPS\n",local_dma.throughput_rx);
        printf("Read Buffer Descriptor count = %d\n",local_dma.bd_rx);
				break;
			case MENU_SGDMA_DDR3_WRITE_READ_OFFSET:
        MenuDma(&local_dma,MENU_SGDMA_DDR3_WRITE_READ_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Write throughput = %d MBPS\n",local_dma.throughput_tx);
        printf("Read throughput  = %d MBPS\n",local_dma.throughput_rx);
        printf("Write Buffer Descriptor count = %d\n",local_dma.bd_tx);
        printf("Read Buffer Descriptor count = %d\n",local_dma.bd_rx);
				break;
			case MENU_SGDMA_DDR4_WRITE_OFFSET:
        MenuDma(&local_dma,MENU_SGDMA_DDR4_WRITE_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Write throughput = %d MBPS\n",local_dma.throughput_tx);
        printf("Write Buffer Descriptor count = %d\n",local_dma.bd_tx);
				break;
			case MENU_SGDMA_DDR4_READ_OFFSET:
        MenuDma(&local_dma,MENU_SGDMA_DDR4_READ_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Read throughput  = %d MBPS\n",local_dma.throughput_rx);
        printf("Read Buffer Descriptor count = %d\n",local_dma.bd_rx);
				break;
			case MENU_SGDMA_DDR4_WRITE_READ_OFFSET:
        MenuDma(&local_dma,MENU_SGDMA_DDR4_WRITE_READ_OFFSET,tx_size,rx_size,&buf_test,&pattern);
        printf("Write throughput = %d MBPS\n",local_dma.throughput_tx);
        printf("Read throughput  = %d MBPS\n",local_dma.throughput_rx);
        printf("Write Buffer Descriptor count = %d\n",local_dma.bd_tx);
        printf("Read Buffer Descriptor count = %d\n",local_dma.bd_rx);
				break;

			   }
#endif
  		}

	}
 
	close(fd);
    printf("PCI Device [/dev/MS_PCI_DEV] closed\n");

	exit(0);
}
