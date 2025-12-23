#ifndef HEADERS_H
#define HEADERS_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pwd.h>
#include <signal.h>
#include<stdbool.h>
#include<dirent.h>
#include<ctype.h>
#include "check_foreground.h"
#include "prompt_username.h"
#include "input_parser.h"
#include "hop_function.h"
#include "reveal_func.h"
#include "file_redirection.h"
#include "redirect_stdin.h"
#include "split_pipe.h"
#include "log_function.h"
#include "log_add.h"
#include "globals.h"
#include "ping.h"
#include "activities.h"
#include "handle_fg_bg.h"
#endif
