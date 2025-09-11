#include "../include/headers.h"
//this file checks if the foreground process is 
//not occupied by someone else so that prompt 
//can be printed :)

int check_foreground(int flag){
	pid_t frgrnd_pid=tcgetpgrp(STDIN_FILENO);
	pid_t process_id=getpid();
	flag=0;
	if(frgrnd_pid!=getpgid(process_id)){
		printf("Oops, not a foreground process!\n");
		flag=1;
		exit(1);
	}
	
	//printf("Hey , don't worry , the program reached here\n");
	return flag;
}
