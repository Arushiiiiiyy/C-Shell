#ifndef SPLIT_PIPE_H
#define SPLIT_PIPE_H

void split_pipe(char* user_copy,char* user_input,
                char* home_dir,char* prev_path, int* hopper,
                int* failed,int* reveal_func_ret,
                char* home_path_always,char* always,char** checker);

#endif
