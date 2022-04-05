/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2017        */
/*                                                                       */
/*   Portions COPYRIGHT 2017 STMicroelectronics                          */
/*   Portions Copyright (C) 2017, ChaN, all right reserved               */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various existing      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "diskio.h"
#include "ff_gen_drv.h"
#include "spi_flash.h"
#include "ringBuffer.h"
#include "app_debug.h"
#include <string.h>
#if defined ( __GNUC__ )
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI_FLASH               0
#define FLASH_PAGE_SIZE			256
#define FLASH_SECTOR_SIZE       4096
#define FLASH_SECTOR_COUNT		2048
#define FLASH_BLOCK_SIZE		65536
/* Private variables ---------------------------------------------------------*/
extern Disk_drvTypeDef  disk;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
//  DSTATUS stat;

//  stat = disk.drv[pdrv]->disk_status(disk.lun[pdrv]);
//  return stat;
        switch (pdrv)
    {
        case SPI_FLASH:
        DEBUG_INFO ("DISK STATUS IS OK\r\n");
        return RES_OK;
    }
  return STA_NOINIT;
}

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
  DSTATUS stat = RES_OK;

  if(disk.is_initialized[pdrv] == 0)
  {
    disk.is_initialized[pdrv] = 1;
//    stat = disk.drv[pdrv]->disk_initialize(disk.lun[pdrv]);
  }
//  return stat;
      if (W25qxx_Init ())
    {
        stat = RES_OK;
        DEBUG_INFO ("diask init ok \r\n");
    }
    else
    {
    stat = STA_NOINIT;
    }
//    return Stat;
  return stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	        /* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
//  DRESULT res;

//  res = disk.drv[pdrv]->disk_read(disk.lun[pdrv], buff, sector, count);
//  return res;
        int i;
	for (i=0; i<count; i++)
	{
		W25qxx_ReadSector (buff, sector, FLASH_SECTOR_SIZE);
//        DEBUG_INFO ("read data at sector add: %d\r\n", sector);
//        DEBUG_INFO ("%s", buff);
		sector ++;
		buff += FLASH_SECTOR_SIZE;
	}

	return RES_OK;
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT disk_write (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count        	/* Number of sectors to write */
)
{
//  DRESULT res;

//  res = disk.drv[pdrv]->disk_write(disk.lun[pdrv], buff, sector, count);
//  return res;
    int i;
    uint8_t recvbuff = 0;
//    uint8_t recBuff = 0;
    uint8_t revpagebuff[256];
	for(i=0;i<count;i++)
	{	
        W25qxx_EraseSector (sector);
        for (int j =0; j < FLASH_SECTOR_SIZE; j++)
        {
            W25qxx_ReadByte (&recvbuff, sector*FLASH_SECTOR_SIZE +j);
            if (recvbuff != 0xFF)
            {
                DEBUG_INFO ("error in erase sector %d\r\n", sector);
                while(1);
            }
                
        }
//        W25qxx_ReadPage (revpagebuff, sector*FLASH_SECTOR_SIZE , 0,256);
		W25qxx_WriteSector ((uint8_t *)buff, sector, FLASH_SECTOR_SIZE);
//        for (int k =0; k < FLASH_SECTOR_SIZE; k++)
//        {
//            W25qxx_ReadByte (&recBuff, sector*FLASH_SECTOR_SIZE +k);
//            if (buff[k] != recBuff)
//            {
//                DEBUG_INFO ("error in write sector %d \r\n", sector);
//            }
//        }
        for (int y = 0; y < 15; y++)
        {
            W25qxx_ReadPage (revpagebuff, sector*FLASH_SECTOR_SIZE/256 + y, 0,256);      
            if (memcmp (buff + y *256, revpagebuff, 256))
            {
                DEBUG_INFO ("error in write sector %d \r\n", sector);
                while(1);
            }
        }
		sector ++;
		buff += FLASH_SECTOR_SIZE;
	}

    return RES_OK;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  DRESULT res;

//  res = disk.drv[pdrv]->disk_ioctl(disk.lun[pdrv], cmd, buff);
//  return res;
    //  res = disk.drv[pdrv]->disk_ioctl(disk.lun[pdrv], cmd, buff);
//  return res;
    	switch(cmd)
		{
			case CTRL_SYNC :
                res = RES_OK;
				break;

			//????
	//		case CTRL_ERASE_SECTOR:
	//			nFrom = *((DWORD*)buff);
	//			nTo = *(((DWORD*)buff)+1);
	//			for(i=nFrom;i<=nTo;i++)
	//				W25X_Erase_Sector(i);
	//
	//			break;

			case GET_BLOCK_SIZE:
				*(DWORD*)buff = FLASH_BLOCK_SIZE;
                DEBUG_INFO ("get block size \r\n");
            res = RES_OK;
			//	buf[1] = (u8)(FLASH_BLOCK_SIZE & 0xFF);
			//	buf[0] = (u8)(FLASH_BLOCK_SIZE >> 8);
				break;


			case GET_SECTOR_SIZE:
				*(DWORD*)buff = FLASH_SECTOR_SIZE;
                DEBUG_INFO ("get sector size \r\n");
            res = RES_OK;
			//	buf[0] = (u8)(FLASH_SECTOR_SIZE & 0xFF);
			//	buf[1] = (u8)(FLASH_SECTOR_SIZE >> 8);
				break;

			case GET_SECTOR_COUNT:
				*(DWORD*)buff = FLASH_SECTOR_COUNT;
                DEBUG_INFO ("get sector count \r\n");
            res = RES_OK;
			//	buf[0] = (u8)(FLASH_SECTOR_COUNT & 0xFF);
			//	buf[1] = (u8)(FLASH_SECTOR_COUNT >> 8);
				break;

			default:
				res = RES_PARERR;
				break;
		}
    return res;
}
#endif /* _USE_IOCTL == 1 */

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
__weak DWORD get_fattime (void)
{
  return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

