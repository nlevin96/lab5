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

int main(int argc, char* argv[]){
    int pipefd[2];
    pipe(pipefd);
    pid_t child1_pid;
    pid_t child2_pid;
    child1_pid = fork();
    if(child1_pid == 0){
        fclose(stdout);
        dup(pipefd[1]);
        close(pipefd[1]);
        cmd_line* _cmd_line = parse_cmd_lines("ls -l");
        if(execvp(_cmd_line -> arguments[0], _cmd_line -> arguments) == -1){
            _exit(-1);
        }
        free_cmd_lines(_cmd_line);
    }
    else {
        waitpid(child1_pid, &status, 0);
        close(pipefd[1]);
        child2_pid = fork();
        if(child2_pid == 0){
            fclose(stdin);
            dup(pipefd[0]);
            close(pipefd[0]);
            cmd_line* _cmd_line = parse_cmd_lines("tail -n 2");
            if(execvp(_cmd_line -> arguments[0], _cmd_line -> arguments) == -1){
                _exit(-1);
            }
            free_cmd_lines(_cmd_line);
        }
        else {
            waitpid(child2_pid, &status, 0);
            close(pipefd[0]);
        }
    }
    
    
    return 0;
}