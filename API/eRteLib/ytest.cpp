#include <string.h>
#include <stdio.h>


int main( int argc, char* argv[])
{
#if 0
   FILE* fp;

   if( (fp=fopen("./aaa","w")) == NULL ){
      printf("fopen fail!!!\n");
      return 0;
   }
   char* pbuf="MemTotal:       16325888 kB\n"\
              "MemFree:         8952100 kB\n"\
              "Buffers:          551140 kB\n"\
              "Cached:          3154256 kB\n"\
              "SwapCached:            0 kB\n";
   char* pbuf2="MemTotal:       16325888 kB\n"\
              "MemFree:         8952100 kB\n"\
              "Buffers:          551140 kB\n"\
              "Buffers:          551140 kB\n"\
              "Buffers:          551140 kB\n"\
              "Buffers:          551140 kB\n"\
              "Buffers:          551140 kB\n"\
              "Cached:          3154256 kB\n"\
              "SwapCached:            0 kB\n";
   //fwrite(pbuf2,strlen(pbuf2),1,fp);
   fclose(fp);

   //renmae

   //if( argc != 1 ){
   //}
#endif
//   printf("AAAAAAAAAAAAA\n");
   rename("aaa_org","aaa");
   return 0;
}
