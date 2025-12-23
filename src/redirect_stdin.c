
#include "../include/headers.h"

// Function prototypes for your existing code

// Function prototypes for your existing code

void redirect_stdin(char **token, int pipe_no, char *user_input,
                    char *home_dir, char *prev_path,
                    int *hopper, int *failed,
                    int *reveal_func_ret, char *home_path_always,
                    char *always, char **checker)
{

    /*printf("n %d\n",pipe_no);
    for(int ni=0;ni<pipe_no;ni++){
        printf("n %s\n",token[ni]);
    }*/

    // first one must be a command ?
    // printf("user input in redirect=%s\n",user_input);
    char *user_cpy = (char *)malloc(sizeof(char) * 4097);
    strcpy(user_cpy, user_input);
    int pipe_fd[pipe_no - 1][2]; // each row is a pipe's read write
    int pid_store[pipe_no];
    for (int i = 0; i < pipe_no - 1; i++)
    {
        if (pipe(pipe_fd[i]) < 0)
        {
            printf("Pipe failed lol\n");
            exit(1);
        }
    }
    for (int i = 0; i < pipe_no; i++)
    {   
        //if(pipe_no==1){
        //    break;
        //}
        pid_store[i] = fork();
        // printf("user input in redirect after fork=%s\n",user_input);
        if (pid_store[i] == 0)
        {
            if (i > 0)
            {
                dup2(pipe_fd[i - 1][0], STDIN_FILENO);
            }
            if (i < pipe_no - 1)
            {
                // printf("yha tak pahucha with pid %d\n",getpid());
                if (dup2(pipe_fd[i][1], STDOUT_FILENO) < 0)
                {
                    perror("dup2 failed on stdin");
                    exit(1);
                }
                // printf("2 yha tak pahucha with pid  %d\n",getpid());
                // fprintf(stderr,"pipe_fd[%d][1]=%d\n",i,pipe_fd[i][1]);
            }
            for (int j = 0; j < pipe_no - 1; j++)
            {
                close(pipe_fd[j][0]);
                close(pipe_fd[j][1]);
            }
            char *argv[4097];
            int var1_in = -1;
            int var2_out = -1;
            int idx = 0;

            char *pipe_command = token[i]; // user_copy
            while (*pipe_command == ' ')
            {
                pipe_command++;
            }
            char *end = pipe_command + strlen(pipe_command) - 1;
            while (end > pipe_command && *end == ' ')
            {
                *end = '\0';
                end--;
            }
            char *tok = strtok(pipe_command, " ");
            // printf("token 0=%s\n",tok);
            // printf("%s=tok\n",tok);
            // printf("user input in redirect before while=%s\n",user_input);
            while (tok != NULL)
            {
                if (strcmp(tok, "<") == 0)
                {
                    tok = strtok(NULL, " ");
                    var1_in = open(tok, O_RDONLY);
                    if (var1_in < 0)
                    {
                        printf("No such file or directory!\n");
                        exit(1);
                    }
                    dup2(var1_in, STDIN_FILENO);
                    close(var1_in);
                    // var1_in will contain your
                    // file to be written in
                }
                else if (strcmp(tok, ">") == 0)
                {
                    tok = strtok(NULL, " ");
                    var2_out = open(tok, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    // this will also wipe the file
                    if (var2_out < 0)
                    {
                        printf("No such file or directory!\n");
                    }
                    dup2(var2_out, STDOUT_FILENO);
                    close(var2_out);
                }
                // should've thought of this
                else if (strcmp(">>", tok) == 0)
                {
                    tok = strtok(NULL, " ");
                    var2_out = open(tok, O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (var2_out < 0)
                    {
                        printf("No such file or directory!\n");
                    }
                    dup2(var2_out, STDOUT_FILENO);
                    close(var2_out);
                    // remember to close
                }
                else
                {
                    // printf("%s=tok\n",tok);
                    argv[idx++] = tok;
                }
                tok = strtok(NULL, " ");
            }
            argv[idx] = NULL;
            if (strcmp(argv[0], "hop") == 0)
            {
                // only pass those tokens which are useful
                //printf("user_input=%s in hop\n", user_input);
                hop_function(user_input, home_dir, prev_path, hopper,
                             failed, reveal_func_ret, home_path_always, checker);
                exit(0);
            }
            else if (strcmp(argv[0], "reveal") == 0)
            {
                // printf("argv[0]=%s\n",argv[0]);
                char stage_cmd[4097] = {0};
                stage_cmd[0] = '\0';
                for (int j = 0; j < idx; j++)
                {
                    strcat(stage_cmd, argv[j]);
                    // printf("something\n");
                    if (j != idx - 1)
                        strcat(stage_cmd, " ");
                }
                // only pass those tokens which are useful
                // printf("stage cmd passed=%s\n",stage_cmd);
                reveal_func(stage_cmd, home_dir, prev_path, hopper, failed,
                            reveal_func_ret, home_path_always, checker);
                fflush(stdout);
                exit(0);
            }
            else if (strcmp(argv[0], "log") == 0)
            {
                // Pass all arguments to log_command
                //printf("argv[0]=%s\n", argv[0]);
                char stage_cmd_log[4097] = {0};
                stage_cmd_log[0] = '\0';
                for (int j = 0; j < idx; j++)
                {
                    strcat(stage_cmd_log, argv[j]);
                    // printf("something\n");
                    if (j != idx - 1)
                        strcat(stage_cmd_log, " ");
                }
                log_function(stage_cmd_log, idx, user_input,
                             home_dir, prev_path, hopper, failed,
                             reveal_func_ret, home_path_always, always, checker);
                exit(0);
            }
            else if (strcmp(argv[0], "activities") == 0)
            {
                activities();
                //printf("reached activities in redirect_stdin\n");
                exit(0);
            }
            else if (strcmp(argv[0], "ping") == 0)
            {
                if (idx < 3)
                {
                    printf("Usage: ping <pid> <signal_number>\n");
                    exit(1);
                }
                pid_t pid = atoi(argv[1]);
                char *endptr;
                long sig_num = strtol(argv[2], &endptr, 10);
                if (*endptr != '\0')
                {
                    printf("Invalid syntax!\n");
                    exit(1);
                }
                int actual_signal = sig_num % 32;
                if (kill(pid, actual_signal) == -1)
                {
                    printf("No such process found\n");
                }
                else
                {
                    printf("Sent signal %ld to process with pid %d\n", sig_num, pid);
                }
                exit(0);
            }
            else if (strcmp(argv[0], "fg") == 0 || strcmp(argv[0], "bg") == 0) {
                //handle_fg_bg(argv, idx);
                return;
                exit(0);
            }
            else
            {
                argv[idx] = NULL;
                execvp(argv[0], argv);
                printf("Command not found!\n");
                exit(1);
            }
            // perror("Execvp failed, sorry");
            // exit(1);
        }

        // else{
        //    printf("Sorry , child not creatable\n");
        //}
        // for (int i=0;i<pipe_no-1;i++){
        //    close(pipe_fd[i][0]);
        //    close(pipe_fd[i][1]);
        //}
        // wait for all children

        // return ;
    }

    for (int i = 0; i < pipe_no - 1; i++)
    {
        close(pipe_fd[i][0]);
        close(pipe_fd[i][1]);
        // remember to close pipes
    }
    // what is wrong with pipe not closing ?

    for (int i = 0; i < pipe_no; i++)
    {
        waitpid(pid_store[i], NULL, 0);
        char *tok = strtok(user_cpy, " ");
        if (strcmp("hop", tok) == 0)
        {
            hop_function(user_input, home_dir, prev_path,
                         hopper, failed, reveal_func_ret, home_path_always, checker);
        }
    }
    // LLM Usage done/inspiration
}