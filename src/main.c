#include "../include/headers.h"
// the above line includes all the headers
//LINE 20 -LINE 20 !
// universal variables hehe
//extern volatile int fg_pgid = -1;        // -1 means "no foreground job"
volatile sig_atomic_t sigtstp_received = 0;
void check_bg()
{
	//printf("bg_job_counter=%d\n",bg_job_count);
	for (int i = 0; i < bg_job_count; i++)
	{
		if (bg_jobs[i].active)
		{
			int status;
			pid_t ret = waitpid(bg_jobs[i].pid, &status, WNOHANG);
			//printf("ret=%d\n",ret);
			if (ret > 0)
			{
				if (WIFEXITED(status) || WIFSIGNALED(status))
				{
					//printf("%s with pid %d exited normally\n", bg_jobs[i].cmd, bg_jobs[i].pid);
					bg_jobs[i].active = 0;
					fflush(stdout);
				}
				//this was initially commented
				 else if (WIFSTOPPED(status))
				 {
				 	// strcpy(list[count].state, "Stopped");
				 	printf("%s with pid %d exited abnormally\n", bg_jobs[i].cmd, bg_jobs[i].pid);
				 	fflush(stdout);
				 }
				 //printf("problem idhar hai\n");
				 bg_jobs[i].active = 0;
				//this was intitially commented
			}
			else if (ret == -1)
			{
				if (errno == ECHILD)
				{
					if (kill(bg_jobs[i].pid, 0) == -1 && errno == ESRCH) {
            		// truly gone → safe to mark inactive
            			bg_jobs[i].active = 0;
        			}
				}
			}
		}
	}
}

void activities()
{
	// printf("activities does print!\n");
	check_bg(); // clean finished jobs
	typedef struct
	{
		pid_t pid;
		char cmd[4097];
		char state[20];
	} activity_entry;

	activity_entry list[MAX_BG_JOBS];
	int count = 0;
	//printf("bg_job_count=%d\n", bg_job_count);
	for (int i = 0; i < bg_job_count; i++)
	{
		if (bg_jobs[i].active)
		{
			int status;
			pid_t ret = waitpid(bg_jobs[i].pid, &status, WNOHANG | WUNTRACED);

			if (ret == 0)
			{
				strcpy(list[count].state, "Running");
			}
			else if (ret > 0 && WIFSTOPPED(status))
			{
				strcpy(list[count].state, "Stopped");
			}
			else
			{
				// check if process still exists
				if (kill(bg_jobs[i].pid, 0) == 0)
				{
					strcpy(list[count].state, "Running");
				}
				else
				{
					bg_jobs[i].active = 0; // process is gone
					continue;
				}
			}

			list[count].pid = bg_jobs[i].pid;
			strcpy(list[count].cmd, bg_jobs[i].cmd);
			count++;
		}
	}

	// Sort lexicographically by command name
	//printf("count=%d\n", count);
	for (int i = 0; i < count - 1; i++)
	{
		for (int j = i + 1; j < count; j++)
		{
			if (strcmp(list[i].cmd, list[j].cmd) > 0)
			{
				activity_entry tmp = list[i];
				list[i] = list[j];
				list[j] = tmp;
			}
		}
	}

	//printf("count=%d\n",count);
	for (int i = 0; i < count; i++)
	{
		char *str=(char*)malloc(sizeof(char)*4097);
		strcpy(str,list[i].cmd);
		char* tok=strtok(str," ");
		printf("[%d] : %s - %s\n", list[i].pid, tok , list[i].state);
		//I did not added 1 intentionally 
	}
	fflush(stdout);
	exit(0);
}

void ping(int pid, int signal_number)
{
	int actual_signal = signal_number % 32;
	if (kill(pid, actual_signal) == -1)
	{
		perror("No such process found");
	}
	else
	{
		printf("Sent signal %d to process with pid %d\n", signal_number, pid);
	}
}

void sigint_handler(int sig)
{
	if (fg_pgid > 0)
	{
		kill(-fg_pgid, SIGINT);
		//-fg_pgid is process in that group
	}
}
// LOOK HERE LOOK HERE
void sigtstp_handler(int sig)
{
	 if (fg_pgid > 0)
	 { // take that particular process ID
	 	kill(-fg_pgid, SIGTSTP);
	 	jobs[job_count].job_id = job_count + 1;
	 	jobs[job_count].pgid = fg_pgid;
	 	strcpy(jobs[job_count].command, "fill smthing here\n");
	 	jobs[job_count].running = 0;
	 	job_count++;
	 	printf("\n[%d] Stopped %s\n", job_count, jobs[job_count - 1].command);
	 	fflush(stdout);
	 }
}

void install_signal_handlers()
{
	struct sigaction sa_int = {0};
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) < 0) {
        perror("sigaction SIGINT");
        // continue but note it
    }
	struct sigaction sa_tstp = {0};
    sa_tstp.sa_handler = sigtstp_handler;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = 0;

    if (sigaction(SIGTSTP, &sa_tstp, NULL) < 0) {
        perror("sigaction SIGTSTP");
    }

	signal(SIGINT, sigint_handler);
	signal(SIGTSTP, sigtstp_handler);
}

int main()
{
	getcwd(psuedo_home, sizeof(psuedo_home));
	int counter = 0;
	char path_init[4097] = {0}; // change dimensions accordingly
	char *user_copy = (char *)malloc(sizeof(char) * 4097);
	getcwd(path_init, sizeof(path_init));
	char home_path_always[4097] = {0};
	getcwd(home_path_always, sizeof(home_path_always));
	// LLM Generated code
	//int len = strlen(path_init);
	//if (len > 1 && path_init[len - 1] == '/')
	//{
	//	path_init[len - 1] = '\0';
	//}
	char home_dir[5000];
	char *slash_last = strrchr(path_init, '/');
	if (slash_last != NULL && *(slash_last + 1) != '\0')
	{ // check out how it works
		strcpy(home_dir, slash_last + 1);
		// so our directory is in home_dir now
	}
	// printf("%s is the home dir\n",home_dir);
	char *prev_path = (char *)malloc(sizeof(char) * 4097);
	int hopper = 0;
	int fdyr = open("shell_log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fdyr == -1)
	{
		perror("open");
		return 1;
	}
	char *always_log_address = (char *)malloc(sizeof(char) * 4097);
	char *always = (char *)malloc(sizeof(char) * 4097);
	getcwd(always_log_address, 4097);
	snprintf(always, 4097, "%s/shell_log.txt", always_log_address);
	// prompt_username(len, home_dir);
	install_signal_handlers();
	signal(SIGTTOU, SIG_IGN); // ignore background terminal output stops
	signal(SIGTTIN, SIG_IGN);
	struct sigaction sa = {0};
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);
	while (1)
	{

		check_bg();
		fflush(stdout);
		// init_prompt_display();
		// the below code checks if the process is foreground or not
		getcwd(path_init, sizeof(path_init));
		//int lent = strlen(path_init);
		//if (lent > 1 && path_init[lent - 1] == '/')
		//{
		//	path_init[lent - 1] = '\0';
		//}
		int flag_ch = 0;

		if (flag_ch == 1)
		{
			printf("Sorry , not a foreground process");
			return 0;
		}
		//int len = 1000;
		//char *checkery = (char *)malloc(sizeof(char) * 4097);
		getcwd(current_working, sizeof(current_working));

        // Call your prompt function
        char prompt[4097] = {0};
        prompt_username(prompt);
        printf("%s ", prompt);
        fflush(stdout);
		//prompt_username(checkery);
		// check_bg();

		char *checker = (char *)malloc(sizeof(char) * 4097);
		checker[0] = '0';
		// THE ABOVE PART IS GOING TO PRINT THE PROMPT, NOW FOR THE SECOND PART
		// WHERE THE USER CAN GIVE INPUT
		char *user_input = (char *)malloc(sizeof(char) * 4097); // for ;
		char *input_copy = (char *)malloc(sizeof(char) * 100000);
		int ret = scanf("%[^\n]", user_input);
		getchar();
		if (ret == EOF)
		{
			// Ctrl-D pressed (EOF)
			printf("logout\n");
			for (int i = 0; i < job_count; i++)
			{
				kill(-jobs[i].pgid, SIGKILL);
			}
			exit(0);
		}
		char *str = (char *)malloc(sizeof(str) * 4097);
		//to trim whitespaces
		strcpy(str, user_input);
		while (*str && isspace((unsigned char)*str))
		{
			str++;
		}

		// If all spaces
		if (*str == 0)
			continue;

		// Trim trailing whitespace
		char *end = str + strlen(str) - 1;
		while (end > str && isspace((unsigned char)*end))
		{
			*end = '\0';
			end--;
		}
		//trimmed
		//char *trimmed_input = trim_spaces(user_input);
		if (strlen(str) == 0)
			continue;
		char *argv[4096];
		int argc = 0;
		char *tok = strtok(str, " \t\n");
		if (!tok)
			continue;
		argv[argc++] = tok;
		while ((tok = strtok(NULL, " \t\n")) != NULL)
		{
			argv[argc++] = tok;
		}

		// 5. Handle fg/bg commands first
		if (strcmp(argv[0], "fg") == 0 || strcmp(argv[0], "bg") == 0)
		{
			handle_fg_bg(argv, argc);
			// After fg/bg, check if any background processes finished
			//check_bg();
			continue;
		}
		strcpy(input_copy, user_input);
		// getchar();
		//int length = strlen(user_input);
		strcpy(user_copy, user_input);
		if (!input_parser(user_input, strlen(user_input))) {
    		continue; // skip processing invalid input
		}
		while (*user_input == ' ' && *user_input != '\0')
		{
			user_input++;
		}
		int failed = 0;
		int reveal_func_ret = 0;
		int a = 0;
		if (a == 0)
		{
			// tokenise based on ;
			char *semi_token[4097];
			char *token = strtok(user_copy, ";");
			int semi_tot = 0;
			while (token != NULL)
			{
				while (*token == ' ')
				{
					token++;
				}
				semi_token[semi_tot++] = token;
				token = strtok(NULL, ";");
			} // broken into tokens
			//printf("%s=token[0] and %s=token[1]\n",semi_token[0],semi_token[1]);
			for (int i = 0; i < semi_tot; i++)
			{ // check it out
				// char* and_split[4097];//it is gonna split by &
				char *cmd = semi_token[i];
				while (*cmd == ' ')
					cmd++;
				char *amp_rest;
				char *amp_part = strtok_r(cmd, "&", &amp_rest);

				// char* rest=cmd;
				int is_background = 0;
				while (amp_part != NULL)
				{
					while (*amp_part == ' ')
						amp_part++;
					char *end = amp_part + strlen(amp_part) - 1;
					while (end > amp_part && *end == ' ')
					{
						*end = '\0';
						end--;
					}

					is_background = 0;
					if (amp_rest != NULL)
					{
						is_background = 1;
					}
					// so we won't keep hop in background , ever right ?
					if (strncmp(amp_part, "log", 3) != 0)
					{
						log_add(amp_part, always, &checker);
					}
					/*(if(strncmp(amp_part, "cd", 2) == 0){
						hop_function(amp_part, home_dir, prev_path, &hopper,
									 &failed, &reveal_func_ret, home_path_always, &checker);
						// log_add(amp_part);
						amp_part = strtok_r(NULL, "&", &amp_rest);
						// log_add(amp_part);
						continue;
					}*/
					if (strncmp(amp_part, "hop", 3) == 0 || strncmp(amp_part, "cd", 2) == 0)
					{
						hop_function(amp_part, home_dir, prev_path, &hopper,
									 &failed, &reveal_func_ret, home_path_always, &checker);
						// log_add(amp_part);
						amp_part = strtok_r(NULL, "&", &amp_rest);
						// log_add(amp_part);
						continue;
					}
					int rc = fork();
					if (rc == 0)
					{
						if (is_background == 1)
						{
							setpgid(0, 0);
							// now no one can touch this process
						}
						else
						{
							setpgid(0, 0);
							// tcsetpgrp(STDIN_FILENO,getpid());
						}
						char *send_pipe = (char *)malloc(sizeof(char) * 4097);
						strcpy(send_pipe, amp_part);
						// log_add(amp_part);
						// printf("go to split pipe\n");
						split_pipe(send_pipe, user_input, home_dir,
								   prev_path, &hopper, &failed,
								   &reveal_func_ret, home_path_always, always, &checker);
						// printf("go to split pipe\n");
						// prompt_username(len, home_dir);
						exit(0);
						// child process se exit ho rha hai , parent abhi bhi chal ra hai
					}
					else
					{
						// setpgid(0,0);
						if (setpgid(rc, rc) < 0)
						{
						}
						if (is_background)
						{
							static int job_number = 1;
							
							printf("[%d] %d\n", job_number, rc);
							bg_jobs[bg_job_count].job_number = job_number;
							
							bg_jobs[bg_job_count].pid = rc;
							
							bg_jobs[bg_job_count].pgid = rc;
							
							strcpy(bg_jobs[bg_job_count].cmd, amp_part);
							bg_jobs[bg_job_count].active = 1;
							bg_job_count++;
							job_number++;
						}
						else
						{
							fg_pgid = rc;				 // <--- mark foreground process group
							setpgid(rc, rc);			 // put child in its own group
							tcsetpgrp(STDIN_FILENO, rc); // give terminal control
							int status;
							waitpid(rc, &status, WUNTRACED);
							tcsetpgrp(STDIN_FILENO, getpgrp()); // return terminal to shell
							fg_pgid = -1;
							// waitpid(rc, NULL, 0);
						}
						if (counter == 1)
						{
							chdir(final_path);
							counter = 0;
						}
						if (checker[0] != '0')
						{
							chdir(checker);
						}
					}
					// prompt_username(len, home_dir);
					amp_part = strtok_r(NULL, "&", &amp_rest);
				}
				// prompt_username(len, home_dir);
			}
		}
		// return 0;
	}
	return 0;
}