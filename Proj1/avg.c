//
//  main.c
//  Assignment2
//
//  Created by Pruthvi on 2/17/20.
//  Copyright © 2020 Pruthvi. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    double counter = 0;
    double sum = 0;
    double n = 0;
    double avg = 0;
   FILE *fp;
   fp = fopen("/Users/pruthvi/Downloads/numbers.txt", "r");
    if (fp == NULL) {
        printf("cannot open file\n");
        exit(1);
    }
    while(fscanf(fp,"%lf", &n) != EOF){
        sum += n;
        counter++;
        
    }
    if(counter == 0){
        printf("Input file empty\n");
    }
    else{
    avg = sum/counter;
    
    
    
    printf("The average is: %f\n", avg);
    }
    fclose(fp);
    return 0;
}
