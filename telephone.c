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
#define SEMKEY 46
#define MEMKEY 121

int main(int argc, char * argv[]) {
  if (argc == 2 && strcmp(argv[1], "-c") == 0) {
    //printf("hi\n");
    
    // creates semaphore, note the permissions
    int sem_id = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0777);
    if (sem_id == -1) {
      printf("semaphore already exists\n");
    } else {
      printf("sempahore created: %d\n", sem_id);
      /** semctl(sem_id, 0, SETVAL, atoi(argv[2]));
      if ((semctl(sem_id, 0, GETVAL)) == -1) {
        printf("Semctl error: %s\n", strerror(errno));
      } else {
        printf("value set: %d\n", semctl(sem_id, 0, GETVAL));
	} **/

      // creating shared mem
      int mem_id = shmget(MEMKEY, sizeof(int), IPC_CREAT | IPC_EXCL);
      printf("mem id: %d\n", mem_id);

      // create file
      int fd = open("story.txt", O_CREAT | O_RDWR | O_APPEND, 0644);
    }
  } else if (argc == 2 && strcmp(argv[1], "-v") == 0) {
    int sem_id = semget(SEMKEY, 0, 0777);
    if ((semctl(sem_id, 0, GETVAL)) == -1) {
      printf("Semctl error: %s\n", strerror(errno));
    } else {
      printf("value set: %d\n", semctl(sem_id, 0, GETVAL));
    }
  } else if (argc == 2 && strcmp(argv[1], "-r") == 0) {
    int sem_id = semget(SEMKEY, 0, 0777);
    semctl(sem_id, 0, IPC_RMID);
    printf("semaphore removed: %d\n", sem_id);
  } else {
    printf("args invalid\n");
  }
  return 0;
}
