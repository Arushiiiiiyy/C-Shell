#include "../include/headers.h"

void file_redirection(char** first_arg,int total){
    int rc=fork();
    if(rc==0){
        //child process
        char** argv=(char**)malloc(sizeof(char*)*(total+1));
        for(int i=0;i<total;i++){
            argv[i]=first_arg[i];
        }
        argv[total+1]=NULL;
        execvp(argv[0],argv);
        printf("Command not found!\n");
        //perror("Execvp failed , sorry\n");
    }
    else if(rc>0){
        wait(NULL);
        //wait for child to get over
    }
    else{
        printf("Child allocation failed\n");
    }
    return ;
}
