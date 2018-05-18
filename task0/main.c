#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "line_parser.h"
#include "limits.h"


int execute(cmd_line* line){
    if(execvp(line -> arguments[0], line -> arguments) == -1){
        perror("error");
        return -1;
    }
    return 0;
}



int main(int argc, char* argv[]){
    char buff [PATH_MAX];
    char* _getcwd;
    while (1){
        _getcwd = getcwd(buff, (size_t)PATH_MAX);
        if(_getcwd !=NULL)
            printf("%s:\n",_getcwd);
        
        char str [2048];
        char* line = fgets(str,2048,stdin);
        
        cmd_line* _cmd_line = parse_cmd_lines(line);
        
        if(strcmp(_cmd_line -> arguments[0],"quit")==0){
            free_cmd_lines(_cmd_line);
            return 0;
        }
        
        if(execute(_cmd_line) == -1)
            return -1;
        free_cmd_lines(_cmd_line);
    }
    
    return 0;
}