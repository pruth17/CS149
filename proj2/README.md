You will develop a program avg_many that is given a list of files on the command line. 

You will need to average up all of the floating point numbers in all of the files to produce a single average.

Assume that you have text files with numbers separated by newlines. You want to read the numbers in from all the files. 

Since you want to be flexible on the type and size of numbers your program can handle, you should use double floating point.

If a file appears multiple times on the command line, those numbers will be processed multiple times.

You should use fork() to start a process for each of the files so that we can compute sums and counts in parallel. To communicate between processes you will use pipe(). You need to wait for all the processes to finish using wait().

You must be able to handle files with any number of numbers (including no numbers). You can assume that the files are well formed: they will contain only valid numbers separated by newlines.

