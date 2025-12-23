#include "../include/headers.h"
//LLM Generated Code
extern bg_job bg_jobs[100];
extern int bg_job_count;
extern pid_t fg_pgid;

// Bring a job to the foreground
void fg_command(int job_number) {
    if (bg_job_count == 0){
        printf("No jobs available\n");
        return;
    }

    int idx = -1;
    if (job_number == -1) {
        idx = bg_job_count - 1;  // most recent job
    } else {
        for (int i = 0; i < bg_job_count; i++) {
            if (bg_jobs[i].job_number == job_number && bg_jobs[i].active) {
                idx = i;
                break;
            }
        }
        if (idx == -1) {
            printf("No such job\n");
            return;
        }
    }

    bg_job *job = &bg_jobs[idx];
    printf("%s\n", job->cmd);

    // Mark foreground job
    fg_pgid = job->pid;

    // Resume job
    if (kill(job->pid, SIGCONT) < 0) {
        printf("No such job\n");
        return;
    }

    // Give terminal control to the foreground job
    tcsetpgrp(STDIN_FILENO, job->pid);

    int status;
    if (waitpid(job->pid, &status, WUNTRACED) < 0) {
        perror("waitpid failed");
    }

    if (WIFSTOPPED(status)) {
        //job->active = 0;
        printf("[%d] Stopped %s\n", job->job_number, job->cmd);
    } else {
        // Remove finished job
        for (int i = idx; i < bg_job_count - 1; i++) {
            bg_jobs[i] = bg_jobs[i + 1];
        }
        bg_job_count--;
    }

    // Restore terminal to shell
    tcsetpgrp(STDIN_FILENO, getpgrp());
    fg_pgid = -1;
}

// Resume a stopped job in the background
void bg_command(int job_number) {
    if (bg_job_count == 0) {
        printf("No jobs available\n");
        return;
    }

    int idx = -1;
    if (job_number == -1) {
        idx = bg_job_count - 1;
    } else {
        for (int i = 0; i < bg_job_count; i++) {
            if (bg_jobs[i].job_number == job_number) {
                idx = i;
                break;
            }
        }
        if (idx == -1) {
            printf("No such job\n");
            return;
        }
    }

    bg_job *job = &bg_jobs[idx];

    if (job->active) {
        printf("Job already running\n");
        return;
    }

    if (kill(job->pid, SIGCONT) < 0) {
        printf("No such job\n");
        return;
    }

    job->active = 1;
    printf("[%d] %s &\n", job->job_number, job->cmd);
}

// Handle fg/bg shell input
void handle_fg_bg(char **argv, int argc) {
    if (strcmp(argv[0], "fg") == 0) {
        if (argc == 1)
            fg_command(-1);
        else
            fg_command(atoi(argv[1]));
    } else if (strcmp(argv[0], "bg") == 0) {
        if (argc == 1)
            bg_command(-1);
        else
            bg_command(atoi(argv[1]));
    }
}
//LLM Generated Code