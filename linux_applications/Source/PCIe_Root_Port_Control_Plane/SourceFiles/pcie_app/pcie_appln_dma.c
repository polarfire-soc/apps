
#include "pcie_appln.h"
#include <math.h>

//extern u32 *Global_data;
extern globle_dev_info GMpci_dev;
extern int fd;
extern int mpci_bar_read(u32 bar,u32 offset);
extern int mpci_bar_write(u32 bar,u32 offset,u32 data);

u32 volatile *Global_data;// = (u32 *) malloc(4*1024*1024);

int mpci_dma_bd_count(config_bar* bd_ct)
{

	config_bar lbd_ct = {0,0,0};

	if (ioctl(fd, MPCI_G5_BD_COUNT,&lbd_ct) == -1)
	{
		printf("mpci_dma_bd_count failed.\n");
		return FALSE;
	}
	else
	{
		PRINTF("mpci_dma_bd_count success \n");

	}

	bd_ct->offset = lbd_ct.offset;
	bd_ct->val = lbd_ct.val;
	return TRUE;
}
int mpci_G5_CN_Dma_Write(u32 offset,u32 size,u32 type)
{
	u32  j = 0,k =0;
	u32 heap_size = 0, dummy = 0;
	u32 *data = (u32 *) malloc(sizeof(u32)*size);
	G5_DmaRegisters ldma_config = {0,0,0,0,0,0};

	if(data == NULL)
	{
		printf("mpci_dma_write --> faild to allocate data \n");
		return FALSE;

	}

	heap_size = sizeof(u32)*size;
	dummy = heap_size/4;
	k = dummy;
//	printf("mpci_dma_write %d --> size = %d\n",__LINE__,heap_size);
	switch(type)
	{

	case DMA_BUF_INC://increment
		for(j = 0; j < dummy;j++ )
		{
			data[j] = j+1;

		}
		break;
	case DMA_BUF_DEC://decrement
		for(j = 0; j < dummy;j++)
		{
			data[j] = k;
			k--;
		}
		break;
	case DMA_BUF_RAND://random

		for(j = 0; j < dummy;j++ )
		{
			data[j] = rand();

		}
		break;
	case DMA_BUF_ZEROS://fill zero's

		for(j = 0; j < dummy;j++ )
		{
			data[j] = 0;

		}
		break;
	case DMA_BUF_ONES://fill ones's
		for(j = 0; j < dummy;j++ )
		{
			data[j] = 0x11111111;

		}
		break;
	case DMA_BUF_AAA:

		for(j = 0; j < dummy;j++ )
		{
			data[j] = 0xAAAAAAAA;

		}
		break;
	case DMA_BUF_555:

		for(j = 0; j < dummy;j++ )
		{
			data[j] = 0x55555555;

		}
		break;
	}

	for(j = 0; j < dummy;j++ )
	{
		Global_data[j] = data[j] ;

	}


	ldma_config.Dma_Destaddr32 = offset;
	ldma_config.Dma_length = heap_size;


	if (ioctl(fd, MPCI_G5_DMA_CONFIG,&ldma_config) == -1)
	{
		printf("mpci_G5_CN_Dma_Write config failed.\n");
		return FALSE;

	}
	else
	{
		PRINTF("mpci_G5_CN_Dma_Write config success \n");
	}

	if (ioctl(fd, MPCI_G5_CN_DMA_WRITE,data) == -1)
	{
		printf("mpci_G5_CN_Dma_Write failed.\n");
		return FALSE;
	}
	else
	{
		PRINTF("mpci_G5_CN_Dma_Write success \n");

	}


	free(data);

    return TRUE;


}

int mpci_G5_CN_Dma_Read(u32 offset,u32 size,u32 **p,u32 *buf_offset)
{

	u32  j = 0, k = 0;
	u32 heap_size = 0;
	u32 test = 0;
	u32 *data2 = (u32 *) malloc(size*sizeof(u32));
	G5_DmaRegisters ldma_config = {0,0,0,0,0,0};

	heap_size = sizeof(u32)*size;

	if((data2 == NULL))
	{
		printf("mpci_dma_sg_read_ioctl --> faild to allocate data \n");
		return FALSE;

	}

	for(j = 0; j < size;j++ )
	{
		data2[j] = 0;

	}


	ldma_config.Dma_Srcaddr32 = offset;
	ldma_config.Dma_length = heap_size;


	if (ioctl(fd, MPCI_G5_DMA_CONFIG,&ldma_config) == -1)
	{
		printf("mpci_G5_CN_Dma_Read config failed.\n");
		return FALSE;
	}
	else
	{
		PRINTF("mpci_G5_CN_Dma_Read config success \n");

	}

	if (ioctl(fd, MPCI_G5_CN_DMA_READ,data2) == -1)
	{
		printf("mpci_G5_CN_Dma_Read failed.\n");
		return FALSE;

	}
	else
	{
		PRINTF("mpci_G5_CN_Dma_Read success \n");
	}

	test = heap_size / 4;
#if M_DEBUG
		for(j = 0; j < size;j++ )
		{
			PRINTF("0x%x\t",data2[j]);
		}

	for(j = 0; j < test;j++ )
	{
		if(data2[j] != Global_data[j])
		{
			for(k = 0; k < test;k++ )
			{
				Global_data[k] = data2[k] ;

			}
			PRINTF("mpci_G5_CN_Dma_Read failed -->%d \n",j);
			free(data2);
			*p = (u32 *) (Global_data);
			*buf_offset = j;
			return 2;
		}
	}
#endif
	*p = (u32 *) (Global_data);
	*buf_offset = 0;
//	PRINTF("mpci_dma_read %d\n",status);
	free(data2);

   return TRUE;


}


int mpci_G5_SG_Dma_Write(u32 offset,u32 size,u32 type)
{

	int heap_size = 0, k = 0;
	u32 j = 0;
	u32 dummy = 0;
	G5_DmaRegisters ldma_config = {0,0,0,0,0,0};

	heap_size = sizeof(u32)*size;

	u32 *data = (u32 *) malloc(sizeof(u32)*size);

	ldma_config.Dma_Destaddr32 = offset;
	ldma_config.Dma_length = heap_size;

	if(data == NULL)
	{
		printf("mpci_write memory allocation failed-->\n");
		return FALSE;
	}

	dummy = heap_size/4;
	PRINTF("mpci_write type %d\n",type);
	k = dummy;

	switch(type)
	{

	case DMA_BUF_INC://increment
		for(j = 0; j < dummy;j++ )
		{
			data[j] = j+1;

		}
		break;
	case DMA_BUF_DEC://decrement
		for(j = 0; j < dummy;j++)
		{
			data[j] = k;
			k--;
		}
		break;
	case DMA_BUF_RAND://random

		for(j = 0; j < dummy;j++ )
		{
			data[j] = rand();

		}
		break;
	case DMA_BUF_ZEROS://fill zero's

		for(j = 0; j < dummy;j++ )
		{
			data[j] = 0;

		}
		break;
	case DMA_BUF_ONES://fill ones's
		for(j = 0; j < dummy;j++ )
		{
			data[j] = 0x11111111;

		}
		break;
	case DMA_BUF_AAA:

		for(j = 0; j < dummy;j++ )
		{
			data[j] = 0xAAAAAAAA;

		}
		break;
	case DMA_BUF_555:

		for(j = 0; j < dummy;j++ )
		{
			data[j] = 0x55555555;

		}
		break;
	}

	for(j = 0; j < dummy;j++ )
	{
		Global_data[j] = data[j] ;

	}
	if (ioctl(fd, MPCI_G5_DMA_CONFIG,&ldma_config) == -1)
	{
		printf("mpci_G5_SG_Dma_Write config failed.\n");
		return FALSE;

	}
	else
	{
		PRINTF("mpci_G5_SG_Dma_Write config success \n");
	}

	if (ioctl(fd, MPCI_G5_SG_DMA_WRITE,data) == -1)
	{
		printf("mpci_G5_SG_Dma_Write failed.\n");
		return FALSE;
	}
	else
	{
		PRINTF("mpci_G5_SG_Dma_Write success \n");

	}


	free(data);
	return TRUE;
}


int mpci_G5_SG_Dma_Read(u32 offset,u32 size,u32 **p,u32 *buf_offset)
{

	u32  j = 0,k=0;
	u32 heap_size = 0;
	int *data2 = (int *) malloc(size*sizeof(int));
	u32 test = 0;
	G5_DmaRegisters ldma_config = {0,0,0,0,0,0};


	if((data2 == NULL))
	{
		printf("mpci_dma_sg_read_ioctl --> faild to allocate data \n");
		return FALSE;

	}

	heap_size = sizeof(int)*size;
	test = heap_size / 4;

	for(j = 0; j < size;j++ )
	{
		data2[j] = 0;

	}

	ldma_config.Dma_Srcaddr32 = offset;
	ldma_config.Dma_length = heap_size;



	if (ioctl(fd, MPCI_G5_DMA_CONFIG,&ldma_config) == -1)
	{
		printf("mpci_G5_SG_Dma_Read config failed.\n");
		return FALSE;

	}
	else
	{
		PRINTF("mpci_G5_SG_Dma_Read config success \n");
	}

	if (ioctl(fd, MPCI_G5_SG_DMA_READ,data2) == -1)
	{
		printf("mpci_G5_SG_Dma_Read failed.\n");
		return FALSE;
	}
	else
	{
		PRINTF("mpci_G5_SG_Dma_Read success \n");

	}

#if M_DEBUG
	for(j = 0; j < test;j++ )
	{

		PRINTF("%d\t",data2[j]);

	}

	usleep(50);
	for(j = 0; j < test;j++ )
	{
		if(data2[j] != Global_data[j])
		{

			for(k = 0; k < test;k++ )
			{
				Global_data[k] = data2[k] ;

			}
			PRINTF("mpci_dma_sg_read failed -->%d \n",j);
			free(data2);
			*p = (u32 *) (Global_data);
			*buf_offset = j;
			return 2;
		}
	}
#endif
	*p = (u32 *) (Global_data);
	*buf_offset = 0;
	free(data2);
    return TRUE;

}

int MenuDma(struct dma_operation *dma_op,int type,int tx_size,int rx_size,u32** buf,u32 *pattern)
{
	int option = 0;
	u32  bar = 0, offset = 0,size = 1024,size1 = 1024,buf_offset = 0;
	int ret;
	u32 offset1 = 0;
	u32 TR = 0,TW = 0,Th_Wput = 0,Th_Rput = 0,FDMA_HZ = 0;
	u32 *p = &bar;
	config_bar bd_ct = {0,0,0},wr = {0,0,0};

	PRINTF("Devtype.Dma --> %d\n",type);

	dma_op->bd_tx = 0;
	dma_op->bd_rx = 0;
	dma_op->throughput_tx = 0;
	dma_op->throughput_rx = 0;

	option = type;
		switch (option)
		{
		case MENU_CDMA_DDR3_READ_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
				size = rx_size/4;
				offset = 0x20000000;
				ret = mpci_G5_CN_Dma_Read(offset,size,&p,&buf_offset);
				if(ret == 0)
				{
					printf("Read failed \n");
					return FALSE;
				}
				else
				{
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

					PRINTF("FDMA_HZ = %d\n",FDMA_HZ);
					PRINTF("TR = %d\n",TR);
					PRINTF("rx_size = %d\n",rx_size);
					if(TR)
					{
						Th_Rput = (4 * size * FDMA_HZ) / TR;
						PRINTF("Read throughput = %u MBPS \n",Th_Rput);
					}

					dma_op->bd_tx = wr.offset;
					dma_op->bd_rx = wr.val;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;
				}
			}


		break;
		case MENU_CDMA_DDR3_WRITE_OFFSET:

			 if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
				size = tx_size/4;
				offset = 0x20000000;
				ret = mpci_G5_CN_Dma_Write(offset,size,*pattern);
				if(ret == 0)
				{
					printf("Write failed \n");
					return FALSE;
				}
				else
				{
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

					PRINTF("FDMA_HZ = %d\n",FDMA_HZ);
					PRINTF("TW = %d\n",TW);
					PRINTF("tx_size = %d\n",tx_size);

					if(TW)
					{
						 Th_Wput = (4 * size * FDMA_HZ) / TW;
						 PRINTF("Write throughput = %u MBPS \n",Th_Wput);
					}


					dma_op->bd_tx = wr.offset;
					dma_op->bd_rx = wr.val;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;
				}
			}
		break;
		case MENU_CDMA_DDR3_WRITE_READ_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{

				size = tx_size/4;
				offset = 0x20000000;
				ret = mpci_G5_CN_Dma_Write(offset,size,*pattern);
				if(ret == 0)
				{
					printf("Write failed \n");
					return FALSE;
				}

				TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
				FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

				if(TW)
				{
					Th_Wput = (4 * size * FDMA_HZ) / TW;
					PRINTF("Write throughput = %u MBPS \n",Th_Wput);
				}


				size1 = rx_size/4;
				offset = 0x20000000;
				ret = mpci_G5_CN_Dma_Read(offset,size1,&p,&buf_offset);

				//*buf = (u32 *) p;
				*buf = (u32 *) (Global_data);
				*pattern = buf_offset;

				TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
				TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);


				if(TR)
				{
					Th_Rput = (4 * size1 * FDMA_HZ) / TR;
					PRINTF("Read throughput = %u MBPS \n",Th_Rput);
				}

				dma_op->bd_tx = wr.offset;
				dma_op->bd_rx = wr.val;
				dma_op->throughput_tx = Th_Wput;
				dma_op->throughput_rx =  Th_Rput;

				if(ret == 0)
				{
					printf("Read failed \n");
					return FALSE;
				}
				else if(ret == 2)
				{

					printf("failed at location %d",*p);
					return 2;
				}
				else
				{
					PRINTF("read successful\n" );
				}
			}
		break;
		case MENU_CDMA_DDR4_READ_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
				size = rx_size/4;
				offset = 0x40000000;
				ret = mpci_G5_CN_Dma_Read(offset,size,&p,&buf_offset);
				if(ret == 0)
				{
					printf("Read failed \n");
					return FALSE;
				}
				else
				{
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);


					if(TR)
					{
						Th_Rput = (4 * size * FDMA_HZ) / TR;
						PRINTF("Read throughput = %u MBPS \n",Th_Rput);
					}

					dma_op->bd_tx = wr.offset;
					dma_op->bd_rx = wr.val;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;
				}
			}


		break;
		case MENU_CDMA_DDR4_WRITE_OFFSET:

			 if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
				size = tx_size/4;
				offset = 0x40000000;
				ret = mpci_G5_CN_Dma_Write(offset,size,*pattern);
				if(ret == 0)
				{
					printf("Write failed \n");
					return FALSE;
				}
				else
				{
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

					if(TW)
					{
						 Th_Wput = (4 * size * FDMA_HZ) / TW;
						 PRINTF("Write throughput = %u MBPS \n",Th_Wput);
					}


					dma_op->bd_tx = wr.offset;
					dma_op->bd_rx = wr.val;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;
				}
			}
		break;
		case MENU_CDMA_DDR4_WRITE_READ_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
#if 1
				size = tx_size/4;
				offset = 0x40000000;
				ret = mpci_G5_CN_Dma_Write(offset,size,*pattern);
				if(ret == 0)
				{
					printf("Write failed \n");
					return FALSE;
				}

				TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
				FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

				if(TW)
				{
					Th_Wput = (4 * size * FDMA_HZ) / TW;
					PRINTF("Write throughput = %u MBPS \n",Th_Wput);
				}


				size = rx_size/4;
				offset = 0x40000000;
				ret = mpci_G5_CN_Dma_Read(offset,size1,&p,&buf_offset);

				//*buf = (u32 *) p;
				*buf = (u32 *) (Global_data);
				*pattern = buf_offset;

				TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
				TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);


				if(TR)
				{
					Th_Rput = (4 * size1 * FDMA_HZ) / TR;
					PRINTF("Read throughput = %u MBPS \n",Th_Rput);
				}

				dma_op->bd_tx = wr.offset;
				dma_op->bd_rx = wr.val;
				dma_op->throughput_tx = Th_Wput;
				dma_op->throughput_rx =  Th_Rput;

				if(ret == 0)
				{
					printf("Read failed \n");
					return FALSE;
				}
				else if(ret == 2)
				{

					printf("failed at location %d",*p);
					return 2;
				}
				else
				{
					PRINTF("read successful\n" );
				}
#endif
					//*buf = (u32 *) p;
					//*p = ;
					*buf = (u32 *) (Global_data);
					*pattern = buf_offset;
				//	return 2;
			}

		break;
		case MENU_CDMA_LSRAM_READ_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
				size = rx_size/4;
				offset = 0x30000000;
				ret = mpci_G5_CN_Dma_Read(offset,size,&p,&buf_offset);
				if(ret == 0)
				{
					printf("Read failed \n");
					return FALSE;
				}
				else
				{
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

					if(TR)
					{
					Th_Rput = (4 * size * FDMA_HZ) / TR;
					PRINTF("Read throughput = %u MBPS \n",Th_Rput);
					}

					dma_op->bd_tx = wr.offset;
					dma_op->bd_rx = wr.val;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;
				}

			}



		break;
		case MENU_CDMA_LSRAM_WRITE_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
				size = tx_size/4;
				offset = 0x30000000;
				ret = mpci_G5_CN_Dma_Write(offset,size,*pattern);
				if(ret == 0)
				{
					printf("Write failed \n");
					return FALSE;
				}
				else
				{
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					//TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

					if(TW)
					{
						Th_Wput = (4 * size * FDMA_HZ) / TW;
						PRINTF("Write throughput = %u MBPS \n",Th_Wput);
					}


					dma_op->bd_tx = wr.offset;
					dma_op->bd_rx = wr.val;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;
				}
			}
		break;
		case MENU_CDMA_LSRAM_WRITE_READ_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{

				size = tx_size/4;
				offset = 0x30000000;
				ret = mpci_G5_CN_Dma_Write(offset,size,*pattern);
				if(ret == 0)
				{
					printf("Write failed \n");
					return FALSE;
				}

					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					//TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);


				if(TW)
				{
					Th_Wput = (4 * size * FDMA_HZ) / TW;
					PRINTF("Write throughput = %u MBPS \n",Th_Wput);
				}
				size1 = rx_size/4;
				offset = 0x30000000;
				ret = mpci_G5_CN_Dma_Read(offset,size1,&p,&buf_offset);

				//*buf = (u32 *) p;
				*buf = (u32 *) (Global_data);
				*pattern = buf_offset;

				TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
				TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);


				if(TR)
				{
					Th_Rput = (4 * size1 * FDMA_HZ) / TR;
					PRINTF("Read throughput = %u MBPS \n",Th_Rput);
				}

				dma_op->bd_tx = wr.offset;
				dma_op->bd_rx = wr.val;
				dma_op->throughput_tx = Th_Wput;
				dma_op->throughput_rx =  Th_Rput;

				if(ret == 0)
				{
					printf("Read failed \n");
					return FALSE;
				}
				else if(ret == 2)
				{

					printf("failed at location %d",*p);
					return 2;
				}
				else
				{
					PRINTF("read successful\n" );
				}

			}
			break;
		case MENU_SGDMA_DDR3_READ_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
				size = rx_size/4;
				offset = 0x20000000;
				ret = mpci_G5_SG_Dma_Read(offset,size,&p,&buf_offset);
				if(ret == 0)
				{
					printf("Read failed \n");
					return FALSE;
				}

				else
				{
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

					ret = mpci_dma_bd_count(&bd_ct);

					if(TR)
					{
						Th_Rput = (4 * size * FDMA_HZ) / TR;
						PRINTF("Read throughput = %u MBPS \n",Th_Rput);
					}

					dma_op->bd_tx = bd_ct.offset;
					dma_op->bd_rx = bd_ct.val;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;
				}
			}


		break;
		case MENU_SGDMA_DDR3_WRITE_OFFSET:

			 if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
				size = tx_size/4;
				offset = 0x20000000;
				offset1 = *pattern;
				ret = mpci_G5_SG_Dma_Write(offset,size, offset1);
				if(ret == 0)
				{
					printf("Write failed \n");
					return FALSE;
				}
				else
				{
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);
					ret = mpci_dma_bd_count(&bd_ct);
					PRINTF("FDMA_HZ = %d\n",FDMA_HZ);
					PRINTF("TW = %d\n",TW);
					PRINTF("tx_size = %d\n",tx_size);


					if(TW)
					{
						 Th_Wput = (4 * size * FDMA_HZ) / TW;
						 PRINTF("Write throughput = %u MBPS \n",Th_Wput);
					}


					dma_op->bd_tx = bd_ct.offset;
					dma_op->bd_rx = bd_ct.val;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;
				}
			}
		break;
		case MENU_SGDMA_DDR3_WRITE_READ_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{

				size = tx_size/4;
				offset = 0x20000000;
				offset1 = *pattern;
				ret = mpci_G5_SG_Dma_Write(offset,size,offset1);
				if(ret == 0)
				{
					printf("Write failed \n");
					return FALSE;
				}

				TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
				FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

				if(TW)
				{
					Th_Wput = (4 * size * FDMA_HZ) / TW;
					PRINTF("Write throughput = %u MBPS \n",Th_Wput);
				}

				usleep(10);

				size1 = rx_size/4;
				offset = 0x20000000;
				ret = mpci_G5_SG_Dma_Read(offset,size1,&p,&buf_offset);

				//*buf = (u32 *) p;
				*buf = (u32 *) (Global_data);
				*pattern = buf_offset;

				TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
				TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);

				ret = mpci_dma_bd_count(&bd_ct);
				if(TR)
				{
					Th_Rput = (4 * size1 * FDMA_HZ) / TR;
					PRINTF("Read throughput = %u MBPS \n",Th_Rput);
				}

				dma_op->bd_tx = bd_ct.offset;
				dma_op->bd_rx = bd_ct.val;
				dma_op->throughput_tx = Th_Wput;
				dma_op->throughput_rx =  Th_Rput;

				if(ret == 0)
				{
					printf("Read failed \n");
					return FALSE;
				}
				else if(ret == 2)
				{

					printf("failed at location %d",*p);
					return 2;
				}
				else
				{
					PRINTF("read successful\n" );
				}

			}

		break;
		case MENU_SGDMA_DDR4_READ_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
				size = rx_size/4;
				offset = 0x40000000;
				ret = mpci_G5_SG_Dma_Read(offset,size,&p,&buf_offset);
				if(ret == 0)
				{
					printf("Read failed \n");
					return FALSE;
				}
				else
				{
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

					ret = mpci_dma_bd_count(&bd_ct);
					if(TR)
					{
						Th_Rput = (4 * size * FDMA_HZ) / TR;
						PRINTF("Read throughput = %u MBPS \n",Th_Rput);
					}

					dma_op->bd_tx = bd_ct.offset;
					dma_op->bd_rx = bd_ct.val;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;
				}
			}


		break;
		case MENU_SGDMA_DDR4_WRITE_OFFSET:

			 if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{
				size = tx_size/4;
				offset = 0x40000000;
				offset1 = *pattern;
				ret = mpci_G5_SG_Dma_Write(offset,size,offset1);
				if(ret == 0)
				{
					printf("Write failed \n");
					return FALSE;
				}
				else
				{
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);
						ret = mpci_dma_bd_count(&bd_ct);
					if(TW)
					{
						 Th_Wput = (4 * size * FDMA_HZ) / TW;
						 PRINTF("Write throughput = %u MBPS \n",Th_Wput);
					}


					dma_op->bd_tx = bd_ct.offset;
					dma_op->bd_rx = bd_ct.val;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;
				}
			}
		break;
		case MENU_SGDMA_DDR4_WRITE_READ_OFFSET:

			if(GMpci_dev.Devtype.Dma == DEVICE_G5_PF )
			{

				size = tx_size/4;
				offset = 0x40000000;
				offset1 = *pattern;
				ret = mpci_G5_SG_Dma_Write(offset,size,offset1);
				if(ret == 0)
				{
					printf("Write failed \n");
					return FALSE;
				}

				TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
				FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);

				if(TW)
				{
					Th_Wput = (4 * size * FDMA_HZ) / TW;
					PRINTF("Write throughput = %u MBPS \n",Th_Wput);
				}


				size1 = rx_size/4;
				offset = 0x40000000;
				ret = mpci_G5_SG_Dma_Read(offset,size1,&p,&buf_offset);

				//*buf = (u32 *) p;
				*buf = (u32 *) (Global_data);
				*pattern = buf_offset;

				TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
				TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
				ret = mpci_dma_bd_count(&bd_ct);

				if(TR)
				{
					Th_Rput = (4 * size1 * FDMA_HZ) / TR;
					PRINTF("Read throughput = %u MBPS \n",Th_Rput);
				}

				dma_op->bd_tx = bd_ct.offset;
				dma_op->bd_rx = bd_ct.val;
				dma_op->throughput_tx = Th_Wput;
				dma_op->throughput_rx =  Th_Rput;

				if(ret == 0)
				{
					printf("Read failed \n");
					return FALSE;
				}
				else if(ret == 2)
				{

					printf("failed at location %d",*p);
					return 2;
				}
				else
				{
					PRINTF("read successful\n" );
				}

			}

		break;
		case MENU_DMA_THROUGHPUT:
					TW = mpci_bar_read(BAR2,G5_DMA_WRITE_TH);
					TR = mpci_bar_read(BAR2,G5_DMA_READ_TH);
					FDMA_HZ = mpci_bar_read(BAR2,G5_FREQ);
					TW = TW & 0xffff;
					TR = TR & 0xffff;
					if(TW)
					{
					Th_Wput = (tx_size * FDMA_HZ) / TW;
					PRINTF("Write throughput = %u MBPS \n",Th_Wput);
					}

					if(TR)
					{
					Th_Rput = (rx_size * FDMA_HZ) / TR;
					PRINTF("Read throughput = %u MBPS \n",Th_Rput);
					}

					dma_op->bd_tx = wr.offset;
					dma_op->bd_rx = wr.val;;
					dma_op->throughput_tx = Th_Wput;
					dma_op->throughput_rx =  Th_Rput;

		break;

	}

	return TRUE;


}
