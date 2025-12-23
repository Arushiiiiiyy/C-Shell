A simple C-shell implementation with different function calls

1) Checks grammar of the input command and takes into account only valid commands.
2) hop - helps in changing the shell's current working directory (cd equivalent).
3) reveal - ls equivalent, views the files inside a directory , -a helps in viewing hidden ones while -l helps
   in viewing in list format.
4) log - logs all the commands (last 15 commands)
5) Input redirection using '<' , output redirection using '>' , '>>' and commnd piping using '|' .
6) Basic sequential and background execution
7) Basic funtionalites like ping , fg ( brings a background or stopped job to the foreground) , bg 
  (resume a stopped background job by sending SIGCONT), Syntax: fg [job_number] and bg [job_number]
8) CTRL-D -> Shell exits with status 0
   CTRL-C -> Sends SIGINT to the current foreground child process group if one exists
   CTRL-Z -> Sends SIGTSTP to the current foreground child process group if one exists,
             move the stopped process to the background process list with status “Stopped”,
             prints:[job_number] Stopped command_name

9) activities - lists all processes spawned by the shell which are still running or are stopped

GRAMMAR USED FOR THE PARSER ->
	shell_cmd  ->  cmd_group ((& | ;) cmd_group)* &?
	cmd_group ->  atomic (\| atomic)*
	atomic -> name (name | input | output)*
	input -> < name | <name
	output -> > name | >name | >> name | >>name
	name -> r"[^|&><;\s]+"
