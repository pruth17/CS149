A utility that is missing from linux: avg.  

To create the avg binary, you'll be creating a single source file, avg.c, which will involve writing C code to implement it.

Sometimes you have a file full of numbers and you want to average them all up. 


For this assignment assume that you have a text file with numbers separated by newlines. You want to read the numbers in and add them up ; then divide by the line count to get the average. 

Since we want to be flexible on the type and size of numbers we can handle, we will use double floating point.

You must be able to handle a file with any number of numbers (including no numbers). You can assume that the files are well formed: it will contain only valid numbers separated by newlines.

For example, numbers.txt contains a list of numbers.

prompt> ./avg numbers.txt
which averages the numbers to 10.5/5=2.1 and will output

2.100000
(trailing zeros are ok.)

The "./" before the avg above is a UNIX thing; it just tells the system which directory to find avg in (in this case, in the "." (dot) directory, which means the current working directory).

 

To compile this program outside of cLion, you can also do the following:

prompt> gcc -o avg avg.c -Wall -Werror
prompt> 
This will make a single executable binary called avg, which you can then run as above.

 

