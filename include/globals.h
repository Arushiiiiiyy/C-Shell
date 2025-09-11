#ifndef GLOBALS_H
#define GLOBALS_H
//Gpt did it 
typedef struct Job{
    int job_id;
    pid_t pgid;
    char command[4097];         //store the command here 
    int running;
}Job;

#define MAX_BG_JOBS 100
#define MAX_JOBS 100
char current_working[PATH_MAX];
char psuedo_home[PATH_MAX];
extern Job jobs[100];
extern int job_count;
extern pid_t fg_pgid;


typedef struct bg_job {
    int job_number;
    pid_t pid;
    pid_t pgid;
    char cmd[4097];
    int active;  // 1 = running, 0 = stopped
} bg_job;

extern bg_job bg_jobs[MAX_BG_JOBS];
extern int bg_job_count;

extern int counter;
extern char final_path[4097];
//GPT did it 
#endif