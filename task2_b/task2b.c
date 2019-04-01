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

void deb_print(char *description, int id, int ret_code){
  system_call(SYS_WRITE, STDERR, " ", 1);
  system_call(SYS_WRITE, STDERR, "Description: ", 13);
  system_call(SYS_WRITE, STDERR, description, strlen(description));
  system_call(SYS_WRITE, STDERR, ", ID: ", 6);
  system_call(SYS_WRITE, STDERR, itoa(id), strlen(itoa(id)));
  system_call(SYS_WRITE, STDERR, ", RC: ", 6); 
  system_call(SYS_WRITE, STDERR, itoa(ret_code), 1); 
  system_call(SYS_WRITE, STDERR, "\n", 1);
}

void exit(){
  system_call(SYS_EXIT, 0x55);
}

int main(int argc, char **argv)
{
	int i, num_of_bytes, count=0, fd, deb_mode=0, retCode;
	char buffer[BUF_SIZE], *prefix="";
  ent *entp=buffer;

  for (i = 1; i < argc; i++){
      if(strcmp(argv[i],"-D")==0)
        deb_mode=1;
      if(strncmp(argv[i], "-p",2)==0)
        prefix=&argv[i][2];
  }
  fd = system_call(SYS_OPEN, ".", 0, 0777);
  if(deb_mode){
    deb_print("SYS_OPEN", SYS_OPEN, fd);
  }

  num_of_bytes = system_call(SYS_GETDENTS, fd, entp, BUF_SIZE);
  if(num_of_bytes==-1)
    exit();
  if(deb_mode){
     deb_print("SYS_GETDENTS", SYS_GETDENTS, num_of_bytes);
  }

  while(count<num_of_bytes){
    if(strncmp(prefix, entp->buf, strlen(prefix))==0){
      system_call(SYS_WRITE, STDOUT, "File name is: ", 14);
      retCode=system_call(SYS_WRITE, STDOUT, entp->buf, strlen(entp->buf));
      system_call(SYS_WRITE, STDOUT, "\n", 1);
    }
    if(deb_mode){
      deb_print("SYS_WRITE", SYS_WRITE, retCode);
    }
    count+=entp->len;
    entp=buffer+count;
  }
  return 0;
}