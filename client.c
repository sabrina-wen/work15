#include "headers.h"
//add gate to headers later*********************************************
//for changing semaphore; gate(ENTER) or gate(LEAVE)
#define ENTER 1
#define LEAVE 0
struct sembuf ctlVal;
//SEMVAL = 0 (occupied) ; SEMVAL = 1 (unoccupied)
int gate( int action ) {
  int sem_id = semget(SEMKEY, 0, 0);
  ctlVal.sem_num = 0;
  if (action) {
    ctlVal.sem_op = 1; //leave
  } else {
    ctlVal.sem_op = -1; //enter
  }
  int check = semop(sem_id, &ctlVal, 1);
  return check;
}
int main(){
  int fd= open("story.txt",O_APPEND|O_RDWR);//writer can read,write, and add
  //struct sembuf sops;
  int shm_id;
  int * length;
  char * story;
  char newline[256];
  gate(ENTER);

  //USER GETS TO READ:
  printf("Last line: \n ");
  shm_id=shmget(MEMKEY,sizeof(int),0);//WHY THIS LINE GIVING ME A SEGFAULT
  printf("19");
  length=shmat(shm_id,0,0);//attaches mem to var
  story= (char*) calloc(1,*length+1);
  lseek(fd,*length * -1 ,SEEK_END);//sets the current position in open file-- backwards by length from the end.
  read(fd,story,*length);
  printf("%s\n",story);

  //USER GETS TO WRITE
  printf("Would you like to add?:\n");
  fgets(newline,sizeof(newline),stdin);//gets user input
  gate(LEAVE);
  return 0;
}