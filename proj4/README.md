One of the main problems in memory management with C is memory leaks. Memory leaks in an Operating System cause serious runtime problems and they can make a system unstable.  You can read some info on problems caused by memory leaks here (Links to an external site.).

Implement a program mem_tracer.c that reads all lines from an input file, in a similar as way you did for assignment 3. However, read all lines into an array of type char ** . For example, for the input file provided as an instance for assignment 3 your char **array will store:

array[0] = "sleep 5"

array[1] = "ls -latr"

.....

array[5] = "wc /etc/passwd"
Initialize the array memory with malloc to an initial size. You should use realloc to expand your array if the initial size turns out not to be big enough for the lines in the input file. Your implementation should work with an input file of up to a few hundred lines.

Store each line in a linked list where a node contains the line as a string (char *) and index number (you probably already did something similar for assignment3). Additionally, implement a recursive function PrintNodes that prints out the content of all nodes in the linked list; printing just the line and index number are sufficient. Call PrintNodes to ensure the linked list has the correct content.

Implement tracing of memory consumption. Use a stack holding the function names and print the memory consumed whenever memory is allocated, reallocated or freed. Your tracing should print messages describing the memory management and usage in each function. Specifically, use this template code to print for each malloc, free and realloc the memory consumed and in which function and line the memory consumption happened.  Wherever there is a "TODO" string in the comments it explains what you should implement in that function code.

Your allocation and deallocation for the char ** array and your linked list should make use of your memory tracing in order to print messages describing the memory management and usage. This is an example of similar memory tracing output from the template code provided.

Your parent process should redirect the stdout to a log file named "memtrace.out". You can use dup2 for the purpose of redirecting stdout (fd1) to the memtrace.out file. Your dup2() call will replace the file descriptor STDOUT_FILENO with the opened file for memtrace.out.
