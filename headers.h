#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/uio.h>
#define SEMKEY 46
#define MEMKEY 121

//union semun {
  //int              val;    /* Value for SETVAL */
  //struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  //unsigned short  *array;  /* Array for GETALL, SETALL */
  //struct seminfo  *__buf;  /* Buffer for IPC_INFO
	//		      (Linux-specific) */
//};
int gate( int action );
