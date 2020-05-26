#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "CommandNode.c"

int main(int argc, char **argv) {
  
    int pid;
    ssize_t read;
    size_t len = 0;
    char *line = NULL;
    int i, j, ctr;
    int index = 0;
    FILE *File = fopen(argv[1], "r");
    
  
    CommandNode *headCommand = NULL; // initializing the linked list
    
    while ((read = getline(&line, &len, File)) != -1) {
        // reading the contents of the file
        j = 0;
        ctr = 0;
        char newString[20][20];
        for (i = 0; i <= (strlen(line)); i++) {
            if (line[i] == ' ' || line[i] == '\0') {
                newString[ctr][j] = '\0';
                ctr++; //for next word
                j = 0; //for next word, init index to 0
            }
            else {
                newString[ctr][j] = line[i];
                j++;
            }
        }
        if (headCommand == NULL) {
            // storing the value in the node
            headCommand = (CommandNode *)malloc(sizeof(CommandNode));
            CreateCommandNode(headCommand, newString, index, NULL);
        }
        else {
            // if already the first node is initialized
            CommandNode *nextCommand1 = (CommandNode*)malloc(sizeof(CommandNode));
            CreateCommandNode(nextCommand1, newString, index, NULL);
            InsertCommandAfter(headCommand, nextCommand1);
        }
        index++;
    }

    struct timespec start, finish;
    
    clock_t t;
    t = clock();

    CommandNode *temp = headCommand; // pointer points at the first node
    while (temp != NULL) {
        pid = fork();
        clock_gettime(CLOCK_MONOTONIC, &start);
        temp->starttime = start.tv_sec;
        if (pid < 0) {
            printf("ERROR!\n");
        }
        else if (pid == 0) {
            // executing the command line in the child process
            char cmd1[20];
            char cmd2[20];
            
            strcpy(cmd1, temp->command[0]);
            strcpy(cmd2, temp->command[1]);
           
            if (strcmp(cmd1, "pwd") == 0) {
                // executing the command
                execlp(cmd1, cmd1, (char *)0);
            }
            else {
                 // executing the command
                execlp(cmd1, cmd1, cmd2, (char *)0);
            }
            temp = temp->nextCommandPtr;
        }
        temp = temp->nextCommandPtr;
    }

    
    /** wait for anything to complete, compute elapsed time, possibly refork **/
    int status;
    temp = headCommand;
    while ((pid = wait(&status)) >= 0){
        if (pid > 0) {
            char buffer[4];
            char outPath[100] = "./";
            char errPath[100] = "./";

            sprintf(buffer, "%d", temp->index);
            // .out files
            strcat(outPath, buffer);
            strcat(outPath, ".out");

            // .err files
            strcat(errPath, buffer);
            strcat(errPath, ".err");

            int outFile = open(outPath, O_WRONLY | O_APPEND | O_CREAT);
            int errFile = open(errPath, O_WRONLY | O_APPEND | O_CREAT);

            clock_gettime(CLOCK_MONOTONIC, &finish);
            int finishTime = finish.tv_sec; // get the finished time.
            int startTime = temp->starttime;
            // fprintf(stderr, "Process with PID %d terminated.\n", pid);
            char cmd1[20];
            char cmd2[20];
            
            strcpy(cmd1, temp->command[0]);
            strcpy(cmd2, temp->command[1]);

            if (finishTime - startTime > 2) {
                pid = fork();
                clock_gettime(CLOCK_MONOTONIC, &start);
                temp->starttime = start.tv_sec;
                if (pid == 0) {
                    char cmd1[20];
                    char cmd2[20];
                    strcpy(cmd1, temp->command[0]);
                    strcpy(cmd2, temp->command[1]);
                   
                    if (strcmp(cmd1, "pwd") == 0) {
                        execlp(cmd1, cmd1, (char *)0);
                    }
                    else {
                        execlp(cmd1, cmd1, cmd2, (char *)0);
                    }
                }
                else {
                    temp->active = false;
                }
                dup2(errFile, 2);
                if (WIFEXITED(status)) {
                    printf("Child %d terminated, exit code: %d\n",
                           pid, WEXITSTATUS(status));
                }
                else if (WIFSIGNALED(status)) {
                    printf("Child %d terminated abnormally, signal number: %d\n", pid, WTERMSIG(status));
                }
            }
            else {
                dup2(errFile, 2);
                printf("Exited with exitcode = 0\n");
                printf("Spawning too fast\n");
            }
            dup2(outFile, 1);
            printf("Starting command %d: child: %d, Parent: %d\n", temp->index, getpid(), getppid());
            printf("Finished time at %d. Runtime duration %d\n", finishTime, finishTime - startTime);
            temp = temp->nextCommandPtr;

            if (temp == NULL) {
                exit(0);
            }
        }
    }
    fclose(File);
    return 0;
}
