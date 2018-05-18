#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "line_parser.h"
#include "limits.h"
#define TRUE 1
#define FALSE 0

int status;

int execute(cmd_line* line){
    
    if(execvp(line -> arguments[0], line -> arguments) == -1){
        _exit(-1);
    }
    return 0;
  
}


int main(int argc, char* argv[]){
    
    char buff [PATH_MAX];
    char* _getcwd;
    char* line;
    while (TRUE){
        _getcwd = getcwd(buff, (size_t)PATH_MAX);
        if(_getcwd !=NULL)
            printf("%s:",_getcwd);
        
        char in_buff [2048];
        line = fgets(in_buff,2048,stdin);
        
        if(strcmp(line,"\n")==0)
            continue;
        
        cmd_line* _cmd_line = parse_cmd_lines(line);
        
        if(strcmp(_cmd_line -> arguments[0],"quit")==0){
            free_cmd_lines(_cmd_line);
            return 0;
        }
        
        pid_t child_pid = fork();
        
        if(_cmd_line -> blocking == 1){
            if(child_pid == 0) execute(_cmd_line);
            else waitpid(child_pid, &status, 0);
        }
        else if(child_pid==0) execute(_cmd_line);
        
        free_cmd_lines(_cmd_line);
    }
    
    return 0;
}