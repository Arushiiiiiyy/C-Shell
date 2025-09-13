#include "../include/headers.h"

#include "../include/headers.h"

void hop_function(char* user_input,char*home_dir,char *prev_path,
				  int* hopper,int* failed,int* reveal_func_ret,
				  char* home_path_always,char** checker){
	char* pointer_space=strchr(user_input,' ');
	if(user_input[3]=='\0'){
		return ;
	}
	char curr_work[4096];
	char* prevy=(char*)malloc(sizeof(char)*4097);
	strcpy(prevy,getcwd(curr_work,sizeof(curr_work)));
	while(pointer_space!=NULL && *pointer_space!='\0'){
		if(pointer_space==NULL){
			return ;//there was no space
		}
		while(*pointer_space==' ' && *pointer_space!='\0'){
			pointer_space++;
		}//this guy removes spaces 
		if(*pointer_space=='\0'){
			continue;
		}
		//new 3 lines
		if(strncmp("; ",pointer_space,2)==0||strncmp("< ",pointer_space,2)==0 ||strncmp("> ",pointer_space,2)==0 || strncmp(">> ",pointer_space,2)==0){
			break;
		}
		else if(strncmp("..",pointer_space,2)==0){
			(*hopper)++;
			if(getcwd(curr_work,sizeof(curr_work))==NULL){
				printf("sorry can't get the files\n");
				return ;
			}
			if(*reveal_func_ret!=-1){//didn't go to reveal
				strcpy(prevy,getcwd(curr_work,sizeof(curr_work)));
			}
			chdir("..");
			/*if(chdir("..")!=0){
				printf("Sorry , directory wasn't able to change\n");
			}*/
			if(getcwd(curr_work,sizeof(curr_work))!=NULL){
				//printf("%s=current path in ..\n",curr_work);
			}	
			pointer_space=pointer_space+2;
		}
		else if(strncmp(".",pointer_space,1)==0 && pointer_space+1!=NULL && strncmp(" ",pointer_space+1,1)==0){
				pointer_space++;
				//return;//do nothing
		}
		else if(strncmp("~",pointer_space,1)==0){
			//printf(".. ~ - case , ~ part seg fault\n");
			//printf("Change cwd to the shell's home directory\n");
			(*hopper)++;
			char path_init[4097]={0};
			getcwd(path_init,sizeof(path_init));//current path
			int len=strlen(path_init);
			if (len>1 && path_init[len-1]=='/'){
			        path_init[len-1]='\0';
			}
			//char home_dir_change[4097];
			chdir(home_path_always);
			//finding the home directory
			//char* slash_last=strrchr(path_init,'/');
			//if(slash_last!=NULL && *(slash_last+1)!='\0'){//check out how it works
			//	strcpy(home_dir_change,slash_last+1);
			//}
			if(*reveal_func_ret!=-1){//didn't go in reveal 
				strcpy(prevy,path_init);
			}
			pointer_space++;
		}
		else if(strncmp("-",pointer_space,1)==0){
			//printf(".. ~ - case , - part seg fault\n");
			if(*hopper==0){
				//printf("hopper fault\n");
				return ;
			}
			//printf("%s=prev_path in -\n",prev_path);
			//lol how to go to previous directory
			char new_path[4097];
			getcwd(new_path,sizeof(new_path));
			//printf("%s=prev_path\n",prev_path);
			chdir(prev_path);
			if(*reveal_func_ret!=-1){//didn't go to reveal
				strcpy(prevy,new_path);
			}
			pointer_space++;
		}
		//else if(strncmp("/",pointer_space,1)==0){
		//	pointer_space++;
		//}
		else{
			(*hopper)++;
			//you may get relative or exact file location
			//int rel_const=check_path(pointer_space);
			//printf("c'man\n");
			char path_initi[4097]={0};
			//printf("%s=path inti\n",path_initi);
			getcwd(path_initi,sizeof(path_initi));//intial path
			char *token = strtok(pointer_space, " ");  
			int length=strlen(token);
			if (chdir(token)==-1){
			    printf("No such directory!\n");
				*failed=1;
			    return ;
			}
			chdir(token);//changed the directory
			if(*reveal_func_ret!=-1){
				strcpy(prevy,path_initi);
			}
			pointer_space+=length;
			//printf("What's your problem?\n");
		}
		if(*reveal_func_ret!=-1){//didn't go to reveal
			strcpy(prev_path,prevy);
			//printf("%s=prev path\n",prev_path);//correct
		}
		if(counter==1){
			getcwd(final_path,4097);
		}
		getcwd(*checker,4097);
		//printf("%s=prev_path bahar aane par\n",prev_path);
	}
	return ;
}

