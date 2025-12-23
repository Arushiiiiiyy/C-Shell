#include "../include/headers.h"

void split_pipe(char* user_copy,
                char* user_input,char* home_dir,char* prev_path,
                int* hopper,int* failed,int* reveal_func_ret,
                char* home_path_always,char* always,char** checker)
{
    char *pipe_tokens[4097];
    int pipe_tok = 0;
    char *token = strtok(user_copy, "|");
    // printf("initial breaking=%s tok\n",token);
    // pipe_tokens[0]=token;
    while (token != NULL){
        while(*token==' '){
            token++;
        }
        pipe_tokens[pipe_tok++] = token;
        // printf("pipe_token[%d]=%s\n",pipe_tok-1,token);
        token = strtok(NULL, "|");
    }
    pipe_tokens[pipe_tok] = NULL;
    // now direct it to redirect_stdin
    // send the entire pipe_tokens here
    //printf("in redirect stdin , pipe tokens sent is=%s and %s\n",pipe_tokens[0],pipe_tokens[1]);
    redirect_stdin(pipe_tokens, pipe_tok, 
                    user_input, home_dir, prev_path, 
                    hopper, failed, reveal_func_ret, home_path_always,
                    always,checker);
    // char* cmd=(char*)malloc(sizeof(char)*4097);
    // char* first_arg=(char*)malloc(sizeof(char)*4097);
    /*char* tokens[4096];
    //now break into tokens
    char* token1;
    token1=strtok(user_cpy," ");//it was changing your userinput
    tokens[0]=token1;
    int ie=1;
    int fleg=0;
    while(token1!=NULL){
        token1=strtok(NULL," ");
        tokens[ie]=token1;
        ie++;
    }
    //printf("userinput=%s\n",user_input);
    if(strchr(user_input,'<')!=NULL && strchr(user_input,'>')==NULL && strstr(user_input,">>")==NULL){
        fleg=1;
        redirect_stdin(user_input,fleg);
    }
    else if(strchr(user_input,'>')!=NULL || strstr(user_input,">>")!=NULL ){
        //pehle ye karwana hai
        fleg=2;
        redirect_stdin(user_input,fleg);
    }
    else if(strchr(user_input,'|')!=NULL){

    }
    else{
        //for now , keep the if here , consider that all the
        //other cases are stripped off and no random f input is given
        printf("ab bhi ?\n");
        file_redirection(tokens,ie);
        //file_redirection(cmd,first_arg);
    }
    */
}