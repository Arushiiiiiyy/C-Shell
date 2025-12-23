#include "../include/headers.h"

static int cmp_strings(const void *a, const void *b) {
    char *s1 = *(char **)a;
    char *s2 = *(char **)b;
    return strcmp(s1, s2); // lexicographic comparison
}

void lexicographic_sort(char* arr[], int n) {
    qsort(arr, n, sizeof(char*), cmp_strings);
}


int check_al(char* tok,int*a_num,int* l_num,int* invalid){
	//int length=strlen(tok);
	int first=0;
	int flag=0;
	//tok++;
	while(*tok!='\0'){
		if(first==0){
			if(*tok=='-'){
				tok++;
				first++;
				continue;
			}
			else{
				(*invalid)++;
				flag=1;
				return 0;
			}
		}
		if(*tok=='a'){
			tok++;
			//printf("in a \n");
			(*a_num)++;
			if(*a_num>0){
				int b=0;
				b=b+1;
			}
			continue;
		}
		else if(*tok=='l'){
			//printf("b\n");
			tok++;
			(*l_num)++;
			if(*l_num>0){
				int a=0;
				a=a+1;
			}
			continue;
		}
		else{
			(*invalid)++;
			if(*invalid>1){
				//printf("value of invalid=%d\n",*invalid);
				return 0;
			}
			return 0;//it is a path change command
		}
		if(flag==1){
			return 0;
		}
	}
	return 1;
}

void reveal_func(char* user_input,char* home_dir,
				char* prev_path,int* hopper,int*failed,
				int* reveal_func_ret,char* home_path_always,char** checker){
	//printf("usen\n");
	char* token[4096];
	int index=0;
	*reveal_func_ret=-1;
	//the thing is that first one may not be al 
	char* al_start=strchr(user_input,' ');
	char* lexic_sort[4097];			//stores the pointers to sort
	if(al_start==NULL){
		//printf("current directory me hai\n");
		DIR* dir=opendir(".");
			if(!dir){
			   printf("Command not found!\n");
			   return ;
			}
			struct dirent* entry;
			index=0;
			while ((entry=readdir(dir))!=NULL) {
        		if (entry->d_name[0] == '.')
            		continue;
				lexic_sort[index++]=strdup(entry->d_name);
        		//printf("%s ", entry->d_name);
    		}
			lexicographic_sort(lexic_sort,index);
			for(int i = 0; i < index; i++){
    			//if(l_num > 0) 
        		//	printf("%s\n", lexic_sort[i]);
    			//else
        		printf("%s ", lexic_sort[i]);
    			free(lexic_sort[i]); 
			}
			//if(l_num == 0) printf("\n");
			printf("\n");
		return ;
	}
	while(*al_start!='\0' && *al_start==' '){
		al_start++;
	}
	if(al_start==NULL || *al_start=='\n' || *al_start=='\0'){
		DIR* dir=opendir(".");
			if(!dir){
			   printf("Command not found!\n");
			   return ;
			}
			index=0;
			struct dirent* entry;
			while ((entry=readdir(dir))!=NULL) {
        		if (entry->d_name[0] == '.')
            		continue;
				lexic_sort[index++]=strdup(entry->d_name);
        		//printf("%s ", entry->d_name);
    		}
			lexicographic_sort(lexic_sort,index);
			for(int i = 0; i < index; i++){
    			//if(l_num > 0) 
        		//	printf("%s\n", lexic_sort[i]);
    			//else
        		printf("%s ", lexic_sort[i]);
    			free(lexic_sort[i]); 
			}
			//if(l_num == 0) printf("\n");
			printf("\n");
			//printf("\n");
		return ;
	}
	//printf("al_start=%s\n",al_start);
	//while(*al_start!='\0' && *al_start==' '){
	//	al_start++;
	//}
	char* tok=strtok(al_start," ");
	
	//printf("first token=%s\n",tok);
	char* tok_copy=tok;
	//int al_supremacy=0;
	int a_num=0;
	int l_num=0;
	//int min_sig=0;
	//int iter=0;
	int minus=0;
	int hop_func=0;
	int v_u_c=0;
	//checking the first token if it is al or path :0
	while(*tok_copy!='\0'){
		if(*tok_copy=='-'){
			tok_copy++;
			minus++;
			continue;
		}
		if(*tok_copy=='a'){
			a_num++;
			tok_copy++;
		}
		else if(*tok_copy=='l'){
			l_num++;
			tok_copy++;
		}
		else{
			hop_func=1;//it means it is a path
			v_u_c++;
			break;
		}
	}
	//this is to check if first one is l/a or path ?
	//printf("anum=%d and lnum=%d\n",a_num,l_num);
	token[0]=tok;
	int i=1,first_path=0;
	//int v_u_c=0;
	int dir_exist=0;
	int invalid=0;
	int wrapper=0;
	//char* tokcopy=(char*)malloc(sizeof(char)*4097);
	//printf("outside while function\n");
	while(tok!=NULL){
		//printf("inside while tok function\n");
	    //printf("Token: '%s'\n", token);
		if(tok[0] == '>' || tok[0] == '|' || tok[0] == ';' || tok[0] == '&'){
        	break;
    	}
	    tok=strtok(NULL," \t");    // continue splitting
		//printf("below while tok function\n");
		if (tok==NULL){
        	break;
    	}
		wrapper=1;
		char* tokcopy=tok;
		//char* last_ptr=tok;
	    token[i]=tok;
		int zerone=check_al(tokcopy,&a_num,&l_num,&invalid);
		if(zerone==1){
			continue;
		}
		else{
			if(v_u_c==0){
				first_path=i;
			}
			v_u_c++;
			dir_exist=1;
			//break;
		}
	    i++;
	}
	//printf("%d=invalide,%d=vuc\n",invalid,v_u_c);
	if(invalid>1 /*|| v_u_c>1*/){
		printf("reveal: Invalid Syntax!\n");
		return ;
	}
	//printf("Just checked dir exists or not\n");
	char* path_concat=(char*)malloc(sizeof(char)*4097);
	path_concat[0]='\0';
	//only 1 is the path
	//printf("dir=%d and hop_func=%d\n",dir_exist,hop_func);
	if((dir_exist==1 || hop_func==1 )&& wrapper==0){
		//printf("first_path=%d\n",first_path);
		strcat(path_concat, token[first_path]);
	}
	else{
		strcat(path_concat,token[first_path]);
	}
	char* actual_path=(char*)malloc(sizeof(char)*4097);
	char* path_concat_final=(char*)malloc(sizeof(char)*4097);
	path_concat_final[0]='\0';
	strcat(path_concat_final,"hop ");
	if(dir_exist==1 || hop_func==1){
		getcwd(actual_path,4097);
		if(actual_path==NULL){
			printf("Sorry , can't find path\n");
		}
		strcat(path_concat_final,path_concat);//check this out once
		if(hop_func==1 || dir_exist==1){
			hop_function(path_concat_final,home_dir,prev_path,hopper,
						failed,reveal_func_ret,home_path_always,checker);
			//change this
			if(*failed==1){
				printf("reveal: Invalid Syntax!\n");
				return;
			}
			fflush(stdout);
		}
		//hop_to that function
		if(a_num>0 && l_num==0){
			DIR* dir=opendir(".");
			if(!dir){
			   printf("Command not found!\n");
			   return ;
			}
			struct dirent * entry;
			index=0;
			while((entry=readdir(dir))!=NULL){
				lexic_sort[index++]=strdup(entry->d_name);
				//printf("%s ",entry->d_name);
			}
			//printf("\n");
			closedir(dir);
			
		}
		else if(a_num==0 && l_num>0){
			DIR* dir=opendir(".");
			if(!dir){
			   printf("Command not found!\n");
			   return ;
			}
			struct dirent* entry;
			index=0;
			while ((entry=readdir(dir))!=NULL) {
        		if (entry->d_name[0] == '.')
            		continue;
				lexic_sort[index++]=strdup(entry->d_name);
        		//printf("%s\n", entry->d_name);
    		}
			closedir(dir);
		}
		else if(a_num>0 && l_num>0){
			DIR* dir=opendir(".");
			if(!dir){
			   printf("Command not found!\n");
			   return ;
			}
			struct dirent * entry;
			index=0;
			while((entry=readdir(dir))!=NULL){
				lexic_sort[index++]=strdup(entry->d_name);
				//printf("%s\n",entry->d_name);
			}
			closedir(dir);
		}
		else{
			//if(token[i-1]=='~'){

			//}
			DIR* dir=opendir(".");
			if(!dir){
			   printf("Command not found!\n");
			   return ;
			}
			struct dirent* entry;
			index=0;
			while ((entry=readdir(dir))!=NULL) {
        		if (entry->d_name[0] == '.')
            		continue;
				lexic_sort[index++]=strdup(entry->d_name);
        		//printf("%s ", entry->d_name);
    		}
			//printf("\n");
			closedir(dir);
		}
		if(chdir(actual_path)!=0){
        	perror("chdir failed");
        	return ;
    	}
	}
	
	else{
		//getcwd(actual_path,4097);
		if(actual_path==NULL){
			printf("Sorry , can't find path\n");
		}
		//strcat(path_concat_final,path_concat);//check this out once
		//if(hop_func==1 || dir_exist==1){
		//	hop_function(path_concat_final,home_dir,prev_path,hopper);
		//}
		//hop_to that function
		if(a_num>0 && l_num==0){
			DIR* dir=opendir(".");
			if(!dir){
			   printf("Command not found!\n");
			   return ;
			}
			struct dirent * entry;
			index=0;
			while((entry=readdir(dir))!=NULL){
				lexic_sort[index++]=strdup(entry->d_name);
				//printf("%s ",entry->d_name);
			}
			printf("\n");
			closedir(dir);
			
		}
		else if(a_num==0 && l_num>0){
			DIR* dir=opendir(".");
			if(!dir){
			   printf("Command not found!\n");
			   return ;
			}
			struct dirent* entry;
			index=0;
			while ((entry=readdir(dir))!=NULL) {
        		if (entry->d_name[0] == '.')
            		continue;
				lexic_sort[index++]=strdup(entry->d_name);
        		//printf("%s\n", entry->d_name);
    		}
			closedir(dir);
		}
		else if(a_num>0 && l_num>0){
			DIR* dir=opendir(".");
			if(!dir){
			   printf("Command not found!\n");
			   return ;
			}
			struct dirent * entry;
			index=0;
			while((entry=readdir(dir))!=NULL){
				lexic_sort[index++]=strdup(entry->d_name);
				//printf("%s\n",entry->d_name);
			}
			closedir(dir);
		}
		else if(a_num==0 && l_num==0 && minus==1){
			char* v=(char*)malloc(sizeof(char)*1000);
			strcpy(v,"hop -");
			hop_function(v,home_dir,prev_path,
				  hopper,failed,reveal_func_ret,
				   home_path_always,checker);
			struct dirent * entry;
			index=0;
			DIR* dir=opendir(".");
			while((entry=readdir(dir))!=NULL){
				lexic_sort[index++]=strdup(entry->d_name);
				//printf("%s\n",entry->d_name);
			}
			hop_function(v,home_dir,prev_path,
				  hopper,failed,reveal_func_ret,
				   home_path_always,checker);
			closedir(dir);
		}
		else {
			DIR* dir=opendir(".");
			if(!dir){
			   printf("Command not found!\n");
			   return ;
			}
			struct dirent* entry;
			index=0;
			while ((entry=readdir(dir))!=NULL) {
        		if (entry->d_name[0] == '.')
            		continue;
				lexic_sort[index++]=strdup(entry->d_name);
        		//printf("%s ", entry->d_name);
    		}
			//printf("\n");
			//printf("reveal: Invalid Syntax!\n");
		}
		//if(chdir(actual_path)!=0){
        //	perror("chdir failed");
        //    return ;
    	//}
		
	}
	lexicographic_sort(lexic_sort,index);
	for(int i = 0; i < index; i++){
    	if(l_num > 0) 
        	printf("%s\n", lexic_sort[i]);
    	else
        	printf("%s ", lexic_sort[i]);
    	free(lexic_sort[i]); 
	}
	fflush(stdout);
	if(l_num == 0) printf("\n");
	/*if(chdir(actual_path)!=0){
        	perror("chdir failed");
        	return ;
    }*/
}