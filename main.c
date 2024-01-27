#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CHARS 2048
#define MAX_WORDS 128

int runCommand(char** words);

int main(){
    while(1){
        char* cwd = getcwd(NULL, 0);
        printf("MUSH> %s: ", cwd);
        free(cwd);
        char command[MAX_CHARS];
        fgets(command, MAX_CHARS, stdin);
        command[strlen(command) - 1] = '\0';
        char* token = strtok(command, " ");
        char* words[MAX_WORDS];
        int i = 0;

        while (token != NULL && i < MAX_WORDS) {
            words[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        words[i] = NULL; 
        if (strcmp(words[0], "exit") == 0){
            exit(0);
        }else if (strcmp(words[0], "cd") == 0){
            if (chdir(words[1]) == -1){
                perror("chdir failed");
            }
        }else{
            pid_t pid = fork();
            if (pid == 0){
                runCommand(words);
                exit(0);
            } else if (pid > 0) {
                wait(NULL);
            } else {
                printf("fork failed\n");
            exit(1);
            } 
        }
    }
}

#include <errno.h>

int runCommand(char** words){
    
    if (execvp(words[0], words) == -1) {
        perror("execvp failed");
        exit(1);
    }
    return 0;
}