#include "../include/headers.h"

int counter = 0;
char final_path[4097] = {0};
Job jobs[MAX_JOBS];
int job_count = 0;
pid_t fg_pgid = -1;
bg_job bg_jobs[MAX_BG_JOBS];
int bg_job_count = 0;
extern char current_working[PATH_MAX];
extern char psuedo_home[PATH_MAX];