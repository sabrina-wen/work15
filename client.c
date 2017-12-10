#include "headers.h"
#define ENTER 1
#define LEAVE 0

union semun initVal;
struct sembuf ctlVal;


//function to control sem value; how many can still access
//===========================================================

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

//functions to view and control shm value
//===========================================================

//get shm val
int getshm() {
  int mem_id = shmget(MEMKEY, 0, 0);
  //attach it to a pointer; obtain info
  int * shm_val = (int *) shmat(mem_id, 0, SHM_RDONLY);
  int size = *shm_val;
  //detach it 
  shmdt(shm_val);
  return size;
}

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
  
int main(){

  gate(ENTER);
  int fd= open("story.txt", O_APPEND|O_RDWR);//writer can read,write, and add

  //struct sembuf sops;
  int  len;
  char * story;
  char newline[256];

  //USER GETS TO READ:
  printf("Last lines: \n");
  len = getshm();
  printf("%d\n", len);
  story = (char*) calloc(1,len+1);


  //sets the current position in open file-- backwards by length from the end.
  if (len) {
    lseek(fd,len* -1, SEEK_END);
    read(fd, story, len);
    printf("%s\n", story);
  } else {
    printf("No last lines.\n");
  }
   
  //USER GETS TO WRITE
  printf("Would you like to add?:\n");
  fgets(newline,sizeof(newline),stdin);//gets user input
  printf("%d\n", sizeof(newline));
  lseek(fd, 0, SEEK_END);
  write(fd, newline, strlen(newline));
  setshm(strlen(newline));
  len=getshm();
  /* if (len) {
    lseek(fd,len* -1, SEEK_END);
    read(fd, story, len);
    printf("%s\n", story);
  } else {
    printf("No last lines.\n");
    }*/
  //close(fd);
  // gate(LEAVE);
  return 0;
}
