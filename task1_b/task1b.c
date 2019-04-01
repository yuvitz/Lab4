#include "util.h"

#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEAK 19

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDRW 2
#define O_CREAT 64

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

int main(int argc, char **argv)
{
	int i;
	int retCode;
	int deb_mode=0, read=STDIN, write=STDOUT;
	char buff[1], *inFile, *outFile;

	for (i = 1; i < argc; i++){
  		if(strcmp(argv[i], "-D")==0)
    		deb_mode=1;
    	if(strncmp(argv[i], "-i",2)==0){
    		inFile=&argv[i][2];
    		read=system_call(SYS_OPEN, inFile, O_RDONLY, 0777);
    	}
    	if(strncmp(argv[i], "-o",2)==0){
   			outFile=&argv[i][2];
    		write=system_call(SYS_OPEN, outFile, O_RDRW | O_CREAT, 0777);
      }  
  }
  while((retCode =  system_call(SYS_READ, read, buff, 1))!=0){
   	if(buff[0]!='\n'){
   		if(deb_mode){
        deb_print("SYS_READ", SYS_READ, retCode);
      }

   		if ((buff[0]>='A') & (buff[0]<='Z')){
   			buff[0]=buff[0]+32;
      }
   		
 			retCode=system_call(SYS_WRITE, write, buff,1);

     	if(deb_mode){
        deb_print("SYS_WRITE",SYS_WRITE, retCode);
      }
    }
  }
  system_call(SYS_WRITE, write, "\n",1);
  if(write!=STDOUT){
    retCode=system_call(SYS_CLOSE, write);
    if(deb_mode){
        deb_print("SYS_CLOSE", SYS_CLOSE, retCode);
    }
  }
  if(read!=STDIN){
    retCode=system_call(SYS_CLOSE, read);
    if(deb_mode){
        deb_print("SYS_CLOSE", SYS_CLOSE, retCode);
    }
  }
  return 0;
}