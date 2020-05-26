//
//  avg_many.c
//  assignment2
//
//  Created by Pruthvi on 2/26/20.
//  Copyright © 2020 Pruthvi. All rights reserved.
// @author Pruthvi Punwar, Bhavana Mahendrakar

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 A struct containing number of numbers and total
 */

typedef struct S {
   int n;
   double total;
} node;

/*
 calculateAvg function calculates the average of the numbers
 in the files
 */

void calculateAvg(int n, int fd1[][2], char **argv){
   if(n == 0)
       return;
  
   int pid = fork();
   if(pid == 0){
       // pid is zero, so it is a child process
       calculateAvg(n - 1, fd1, argv); // recursive call to the other files
       // variable declarations
       double total = 0, num;
       int counter = 0;
       int res;
        FILE *f = fopen(argv[n], "r"); // opening the file
       // reading in the from the files till its end
       while((res = fscanf(f, "%lf", &num)) != EOF) {
           total += num; // adding the numbers in the file to total
           counter++; // incrementing the counter
       }
       node r;
       // assigining values to the node
       r.total = total;
       r.n = counter;
       write(fd1[n][1], &r, sizeof(r));
      
       exit(0); // exit status of the child
   } else {
       // parent waiting for child process to complete
       wait(NULL);
   }
  
}

/*
 Main function of the program
 */
int main(int argc, char **argv){
   int num_files = argc - 1;
   int fd1[argc][2], i;
  
   for(i = 1; i <= num_files; i++){
       pipe(fd1[i]); // communicating between processes using pipe
   }
  
   calculateAvg(num_files, fd1, argv); // function call
  
   double total = 0;
   int counter = 0;
   for(i = 1; i <= num_files; i++){
       node r;
       read(fd1[i][0], &r, sizeof(r));
       total += r.total;
       counter += r.n;
   }
  if(total == 0 && counter == 0)
      printf("No numbers in the file\n");
    else
      printf("%f\n", total/counter);
  
    return 0;
}
