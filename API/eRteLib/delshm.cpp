#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

void help(char* pName);
int DestroyShm(const char *pszPath,int& nShmId);

int main(int argc, char* argv[])
{
   if( argc != 2 ){
      help(argv[0]);
      return 0;
   }
   char* pName=argv[1];
   int nShmId=0;
   int nRet= DestroyShm(pName,nShmId);
   if( nRet == 1 ){
      printf("SUCCESS: delete shared memory(%s), key(%d)\n",pName,nShmId);
   }
   return 0; 
}

void help(char* pName)
{

   printf("USAGE: %s pathname\n",pName);
   printf(" pathname(M): using ftok func, file full path\n");
}

int DestroyShm(const char *pszPath, int& nShmId)
{
   key_t key;

   if ((key = ftok(pszPath, 'x')) < 0) {
      printf("ERROR: fail in ftok. not found file[%s]\n",pszPath);
      return -1;
   }

   if ((nShmId = shmget(key, 0, 0666)) < 0) {
      if (errno == ENOENT) {
         printf("not found shared memory. not created yet.. (%s)\n",pszPath);
         return 1;
      }
      printf("ERROR: fail in shmget. file[%s]\n",pszPath);
      return -1;
   }
   if (shmctl(nShmId, IPC_RMID, 0) < 0) {
      printf("ERROR: fail in shmctl(REMOVE). file[%s]\n",pszPath);
      return -1;
   }
   return 1;
}
