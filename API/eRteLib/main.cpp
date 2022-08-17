#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MKSIZE (1024*1024*10)
#define MEMSIZE (1024*1024*1024*1)
int main(int argc, char* argv[])
{

   if( argc != 2 ){
      printf("Usage: LOAD 1 or 2 , 1:CPU, 2:MEM\n");
      return 0;
   }

   int nFlag=atoi(argv[1]);


   if( nFlag == 1 ){
      printf("CPU LOAD: +100%%/1core \n");
   }else{
      printf("MEM LOAD: +1G  \n");
      char* pBuf1=new char[MEMSIZE];
      getc(stdin);
      return 0;
   }

   char* pBuf=new char[MKSIZE];
   while(true){
      if( nFlag == 1 ){
         memset(pBuf,0x00,MKSIZE);
      }else{
         break;
      }
   }
   return 0;
}
