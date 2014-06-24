       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
 #include <stdio.h>
#include "diskio.h"
#include "ff.h"
int sImageFileHandle=0;
int main(int argc, char **argv)
{
  if(argc<2) 
  {
     printf("make_fatfs <image> [files to add]\n");
     return -1;
  }
  sImageFile=argv[1];  
  FATFS fat;

  int size=512*128;
  f_mount(&fat,"",1);
  FRESULT res=f_mkfs("0",1 , 2048);
  f_setlabel ("BOOT");
  printf("#Create dosfs of %d byes rsult %d\n", size, res);
  int i=0;
  for(i=2;i<argc; i++)
  {
  char *file;
  {
  char *a=argv[i];
  file=a;
  while(*a!=0)
  {
    if(*a=='/')
     file=a+1;
    a++;
  }

  }
  FIL f;
  FILE *fin=fopen(argv[i], "rb");
  if(fin==0)
  {
    printf("Could not open %s\n", argv[i]);
    return -1;
  }
  printf("Writing %s\n", file);
  res = f_open (&f, file, FA_CREATE_ALWAYS |FA_WRITE);
  printf("#Open file %d\n", res);
  UINT w;
  char buf[4096];
  int r=0;
  do{
   r=fread(buf,1,sizeof(buf),fin);
   if(r>0){
      res = f_write(&f, buf, r, &w);
      if(res!=0){
        printf("Write error %d\n", res);
        return -1;
      }
       if(w!=r){
        printf("Write error wrote only %d of %d\n", w,r);
        return -1;
       }
    }
  }while(r>0);
  res= f_close(&f);
  fclose(fin);
  printf("#File %s closed %d\n", file , res);
  }
  
   
  if(sImageFileHandle!=0)
    close(sImageFileHandle);
}

