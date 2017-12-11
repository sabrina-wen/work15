#include "headers.h"

union semun initVal;
struct sembuf ctlVal;

//functions to view and control shm value
//===========================================================

//get shm val

//set new shm val
void setshm( int size ) {
  int mem_id = shmget(MEMKEY, 0, 0);
  //attach it to a pointer
  int * shm_val = (int *) shmat(mem_id, 0, 0);
  *shm_val = size;
  //detach it
  shmdt(shm_val);
}

//===========================================================

int main(int argc, char * argv[]) {
  if (argc == 2 && strcmp(argv[1], "-c") == 0) {
    // creates semaphore, note the permissions
    int sem_id = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0777);
    if (sem_id == -1) {
      printf("semaphore already exists\n");
    } else {
      printf("sempahore created: %d\n", sem_id);

      //setting sem value
      initVal.val = 1;
      semctl(sem_id, 0, SETVAL, initVal);

      // creating shared mem
      int mem_id = shmget(MEMKEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0777);
      printf("mem id: %d\n", mem_id);
      setshm(0);

      // create file
      int fd = open("story.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
    }
  }

  else if (argc == 2 && strcmp(argv[1], "-v") == 0) {
    // getting actual size of story file
    struct stat storyfile;
    stat("story.txt", &storyfile);
    char * story = calloc(1, storyfile.st_size + 1);
    //char story[] = calloc(1000, sizeof(char));
    int fd = open("story.txt", O_RDWR);
    read(fd, story, storyfile.st_size);
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
    struct stat storyfile;
    stat("story.txt", &storyfile);
    char * story = calloc(1, storyfile.st_size + 1);
    int fd = open("story.txt", O_RDWR);
    read(fd, story, storyfile.st_size);
    printf("story contents:\n%s\n",story);
    printf("\n");
    printf("story file has been removed\n");
    char * remove_contents[] = {"rm", "story.txt", NULL};
    execvp(remove_contents[0], remove_contents);
  }

  else {
    printf("args invalid\n");
  }
  return 0;
}
