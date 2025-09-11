#ifndef REDIRECT_STDIN_H
#define REDIRECT_STDIN_H

void redirect_stdin(char** token,int pipe_no,char* user_input,
                    char*home_dir,char *prev_path,int* hopper,
                    int* failed,int* reveal_func_ret,char* home_path_always,
                    char* always,char** checker);

#endif