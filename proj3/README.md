When a UNIX-like system starts up, it runs init.

Nowadays this is a program called systemd on UNIX-like systems. 

On Mac the similar system manager is called launchd. It runs under PID of 1 and is an ancestor of all other processes. You can see the process with command "ps aux".

If a process is left as an orphan (its parent dies), it gets reassigned as a child of PID 1.

These service programs (init, systemd or launchd) are running in the background; on UNIX-like OSs these are commonly referred to as daemons and generally have names ending with the letter “d.” These programs ensure that things start up properly and stay running. 

If a process crashes systemd (or launchd) can detect this and start it back up.

You will develop a proc_manager program that reads in the commands to execute from an input file one-by-one. You will read one command and its parameters per each line. 

Then the proc_manager program will start up the process and "babysit" it.

You will run each command in a child process using exec (or one of its variants) and the parent process will wait for each exec to complete. For each child process there will be log messages written to output and error files, which are named after its index in the sequence of commands read in. For example, process #1 will have logs written to 1.out and 1.err. Upon start, the string "Starting command INDEX: child PID pid of parent PPID" will be logged to the corresponding output file 1.out. You can retrieve PID and PPID through the return value of fork() or getpid() or getppid().

You can use dup2() to make file handle 1 (stdout) go to a file X.out and handle 2 (stderr) go to a file X.err. Note: new executions of a command should append to the previous output and error files, such as 1.out, rather than overwrite them.

Timer for each process: you can include the timer.h library and use timer to record the start time of spawning each child process in the parent. Upon finish of an exec (either a successful finish or termination via a signal), the process should record the finish runtime, and it should write to the output file the string "Finished at FIN, runtime duration DUR" where FIN is the finish time and DUR is the duration of the execution.

Each time a process finishes, the message "Exited with exitcode = X" should be written to the error file of the process. X is the process exit code. If the process was killed with a signal, then "Killed with signal S" should be written to the error file. S is the signal number that killed the process. This information is gathered using the status parameter of the wait() system call. If the program cannot be started (exec fails), use perror("name of command") to get the error message and command name and write them to the command's error file. Processes that encounter an invalid command (exec fails) should have an exit code of 2. Remember the exit code of 0 indicates success. Exit codes other than 0 indicate some failure, including a termination via a kill signal.

 

Here is an example run:

$ ./proc_manager cmdfile
$ cat 1.out
Starting command 1: child 2353 pid of parent 2234
Finished at 90, runtime duration 1

$ cat 1.err
Exited with exitcode = 0
spawning too fast

$ cat 2.out
Starting command 2: child 2363 pid of parent 2234
Finished at 100, runtime duration 10
Starting command 2: child 2387 pid of parent 2234
Finished at 110, runtime duration 10

$ cat 2.err
Exited with exitcode = 0
Killed with signal 15
spawning too fast

$ cat 3.out
Starting command 3: child 2377 pid of parent 2234
Finished at 95, runtime duration 1

$ cat 3.err
Exited with exitcode = 0
spawning too fast
...
...


Where cmdfile contains:
prompt> cat cmdfile
sleep 5
ls -latr
sleep 3
pwd
sleep 1
wc /etc/passwd


The parent will re-start the executable if the command took more than two seconds to complete. Therefore, a process restarts as long as its last execution took more than 2 seconds. If a process completed within less than 2 seconds after starting, proc_manager will not restart it and will print a message "spawning too fast" to the error file for the process. If it was terminated by a signal right after it started, or it had some other failure that caused it to exit immediately, it won't restart, since it exited immediately.

proc_manager runs until there are no more processes running. In this example the ls and wc commands won't get restarted because they finish right after they are started. The sleep runs for 3 seconds, so proc_manager will keep restarting it, unless we do "pkill sleep" in another terminal. If we do pkill fast enough, proc_manager will detect the quick duration and exit and not restart it. If any process has no child process then wait() returns immediately "-1", thus you can continue until wait() returns -1.
