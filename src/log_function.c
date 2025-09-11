//LLM Generated 
#include "../include/headers.h"


//#define LOG_FILE "shell_log.txt"
#define MAX_LOG 15

// Add a shell command to the log
void log_add(char *shell_cmd,char* always,char** checker) {
    if (strncmp(shell_cmd, "log", 3) == 0) return;
    if (strstr(shell_cmd, "log") != NULL) return;

    FILE *fp = fopen(always, "r");
    char *lines[MAX_LOG];
    int count = 0;
    char line_buf[4097];

    // Read existing lines
    if (fp) {
        while (fgets(line_buf, sizeof(line_buf), fp) != NULL && count < MAX_LOG) {
            line_buf[strcspn(line_buf, "\n")] = 0;
            lines[count++] = strdup(line_buf);
        }
        fclose(fp);
    }

    // Prepare the new command (clean newline)
    char new_cmd[4097];
    strncpy(new_cmd, shell_cmd, sizeof(new_cmd)-1);
    new_cmd[sizeof(new_cmd)-1] = '\0';
    new_cmd[strcspn(new_cmd, "\n")] = 0;

    // Skip duplicate (if last is same as new)
    if (count > 0 && strcmp(lines[count-1], new_cmd) == 0) {
        for (int i = 0; i < count; i++) free(lines[i]);
        return;
    }

    // Shift if full
    if (count == MAX_LOG) {
        free(lines[0]);
        for (int i = 1; i < MAX_LOG; i++) lines[i-1] = lines[i];
        count--;
    }

    // Add new command
    lines[count++] = strdup(new_cmd);

    // Write back full log
    fp = fopen(always, "w");
    if (!fp) {
        perror("fopen");
        for (int i = 0; i < count; i++) free(lines[i]);
            return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s\n", lines[i]);
        free(lines[i]);
    }
    fclose(fp);
}


// Execute a command from log by its index (1-indexed, newest->oldest)
void log_execute(char *index_str, char *user_input, char *home_dir, 
                 char *prev_path,
                 int *hopper, int *failed, 
                 int *reveal_func_ret, char *home_path_always,
                 char* always,char** checker) {
    FILE *fp = fopen(always, "r");
    if (!fp) return;

    char *lines[MAX_LOG];
    int count = 0;
    char buffer[4097];

    while (fgets(buffer, sizeof(buffer), fp) != NULL && count < MAX_LOG) {
        buffer[strcspn(buffer, "\n")] = 0;
        lines[count++] = strdup(buffer);
    }
    printf("%s=buffer\n",buffer);
    fclose(fp);

    int idx = atoi(index_str);
    if (idx < 1 || idx > count) {
        printf("Invalid index\n");
    } else {
        // newest → oldest mapping
        char *cmd = lines[count - idx];
        char* cmd_copy=strdup(cmd);
        char* argv[50];
        int argc=0;
        char* tok=strtok(cmd_copy," ");
        while (tok && argc < 50) {
            argv[argc++] = tok;
            tok = strtok(NULL, " ");
        }
        argv[argc] = NULL;
        if (argc > 0 && strcmp(argv[0], "hop") == 0) {
            hop_function(cmd, home_dir, prev_path, hopper, failed,
            reveal_func_ret, home_path_always,checker);
        }
        //else if (argc > 0 && strcmp(argv[0], "reveal") == 0) {
          //  reveal_func(cmd, home_dir, prev_path, hopper, failed, reveal_func_ret, home_path_always);
        //}
        else{
        char *send_pipe = strdup(cmd);
        //printf("%s=send_pipe\n",send_pipe);
        counter=1;
        split_pipe(send_pipe,cmd,home_dir,prev_path,
                    hopper,failed,reveal_func_ret,
                    home_path_always,always,checker);
        free(send_pipe);
        }
        free(cmd_copy);
    }

    for (int i = 0; i < count; i++) free(lines[i]);
}

// The main log function, called from redirect_stdin
void log_function(char *stage_cmd_log, int idx, char *user_input, char *home_dir,
                 char *prev_path, int *hopper, int *failed, int *reveal_func_ret,
                 char *home_path_always,char* always,char** checker) {

    // Parse stage_cmd_log into tokens
    char *tokens[50];
    int num_tokens = 0;
    char *tmp = strdup(stage_cmd_log);
    char *tok = strtok(tmp, " ");
    while (tok != NULL && num_tokens < 50) {
        tokens[num_tokens++] = tok;
        tok = strtok(NULL, " ");
    }

    if (num_tokens == 1) {
        // log -> print history
        FILE *fp = fopen(always, "r");
        if (!fp) return;
        char buffer[4097];
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }
        fclose(fp);
    } 
    else if (num_tokens == 2 && strcmp(tokens[1], "purge") == 0) {
        fclose(fopen(always, "w")); // clear file
    } 
    else if (num_tokens == 3 && strcmp(tokens[1], "execute") == 0) {
        //printf("sent in execute=%s\n",tokens[2]);
        log_execute(tokens[2], user_input, home_dir,
                    prev_path, hopper, failed, reveal_func_ret,
                    home_path_always,always,checker);
    } 
    else {
        printf("Invalid log syntax\n");
    }

    free(tmp);
}

//LLM Generated code 