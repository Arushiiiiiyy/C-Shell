#include "../include/headers.h"


extern char current_working[PATH_MAX]; // used in main.c
extern char psuedo_home[PATH_MAX];     // shell’s home directory

void prompt_username(char* str_final){

    uid_t uid_system=getuid();
    struct passwd* pd=getpwuid(uid_system);
    char* username;
    if(pd){
        username=pd->pw_name;
    }else{
        perror("getpwuid\n");
    }

    char host_name[_SC_HOST_NAME_MAX+1];

    if(gethostname(host_name,sizeof(host_name))==0){
        char host_final[100];
        unsigned long i=0;

        // copy full hostname including ".local"
        for(i=0;i<strlen(host_name);i++){
            host_final[i]=host_name[i];
        }
        host_final[i]='\0';

        strcat(str_final,"<");
        strcat(str_final,username);
        strcat(str_final,"@");
        strcat(str_final,host_final);
        strcat(str_final,":");
        if(getcwd(current_working,sizeof(current_working))!=NULL){

            char final_current[PATH_MAX];
            
            unsigned long a=strlen(psuedo_home);

            if(a>strlen(current_working)){

                strcat(str_final,current_working);
                strcat(str_final,">");
                return;

            }

            unsigned long i=a;
            for(i=a;i<strlen(current_working);i++){
                final_current[i-a]=current_working[i];
            }
            final_current[i-a]='\0';
            if(final_current[0]=='\0'){  
                strcat(str_final,"~>");
            }else{
                strcat(str_final,final_current);
                strcat(str_final,">");
            }

        }else{
            perror("Error getting current directory\n");
        }

    }else{
        perror("getting the host name\n");
    }

}

void default_state(){
    printf("<arushishukla@Arushis-Laptop.local:~> ");
    fflush(stdout);
}
