/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "stdio.h"
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

/* Definitions of physical drive number for each media */
#define ATA		0
const int SECTOR_SIZE=512;
const int SECTOR_COUNT=16483*2;

static int sImageFileHandle=0;
char *sImageFile;

DWORD get_fattime (void)
{
   return 0;
}


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :
          if(sImageFileHandle!=0) {
            printf("Close handle %d\n", sImageFileHandle);
            close(sImageFileHandle);
          }
          sImageFileHandle=open(sImageFile, O_RDWR | O_CREAT ,  00666 );
         lseek(sImageFileHandle, SECTOR_SIZE*SECTOR_COUNT-4,SEEK_SET);
         int a=0;
                int w=write(sImageFileHandle,&a,sizeof(a));
          if(sImageFileHandle!=0)
          {
             printf("Opened %s %d\n",sImageFile, sImageFileHandle);
             return  RES_OK;
          }else{
              printf("Error open %s\n",sImageFile);
              return STA_NODISK;
          }
   	  return 0;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :

		return 0;

	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA : 
           {
		// translate the arguments here
                printf("Read %d at Secotr %d\n", (int) sector, count);
                lseek(sImageFileHandle, SECTOR_SIZE*sector,SEEK_SET);
                int r=read(sImageFileHandle,buff,count*SECTOR_SIZE);
                if(r!=count*SECTOR_SIZE)
                {
                   printf("Read error expected %d got %d\n", count*SECTOR_SIZE,r);
	           return RES_PARERR;
                }
		return 0;
           }

	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA : 
            {
            
                printf("Write %d sectors at Sector %d \n", count*SECTOR_SIZE, (int) sector);
                lseek(sImageFileHandle, SECTOR_SIZE*sector,SEEK_SET);
                int toWrite=count*SECTOR_SIZE;
                int w=write(sImageFileHandle,buff,toWrite);
                if(w!=toWrite) 
	        {
                   printf("Write error expected %d got %d Handle %d\n", toWrite, w, sImageFileHandle);
	           return RES_PARERR;
                }

		return 0;
            }  
	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
        switch(cmd) 
        {
         case GET_SECTOR_SIZE: 
           {
              *(WORD*)buff = SECTOR_SIZE;
              return 0;
           }
         case  GET_SECTOR_COUNT:
           {

              *(DWORD*)buff = SECTOR_COUNT;
              return 0;
           }
         case  GET_BLOCK_SIZE:
           {
              *(DWORD*)buff = 1 ;
              return 0;
           }
         case  CTRL_ERASE_SECTOR:
           {
              return 0;
            }
         case CTRL_SYNC:  
              sync();
              return 0;
        }
         printf("Tried IOCTL and ignored\n");
	return 0;
}
#endif
