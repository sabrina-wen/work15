#include "headers.h"

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
      int fd = open("story.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
    }
  }

  else if (argc == 2 && strcmp(argv[1], "-v") == 0) {
    char story[1000];
    int fd = open("story.txt", O_RDWR);
    read(fd, &story, sizeof(char) * 1000);
    printf("story contents:\n%s",story);
    close(fd);
  }

  else if (argc == 2 && strcmp(argv[1], "-r") == 0) {
    // removing semaphore
    int sem_id = semget(SEMKEY, 0, 0777);
    semctl(sem_id, 0, IPC_RMID);
    printf("semaphore removed: %d\n", sem_id);

    // removing mem
    int mem_id = shmget(MEMKEY, sizeof(int), IPC_EXCL);
    shmctl(mem_id, IPC_RMID, NULL);
    printf("shared mem removed: %d\n", mem_id);

    //does removing the story just meaning erasing its contents?
    // need to wait until semaphore reaches 0
    char story[1000];
    int fd = open("story.txt", O_RDWR);
    read(fd, &story, sizeof(char) * 1000);
    printf("story contents:\n%s\n",story);
  }

  else {
    printf("args invalid\n");
  }
  return 0;
}
