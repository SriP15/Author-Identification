# Assignment 7: Author Identification

This assignment takes in a text file, and compares it to the text of other authors. It then reveals which text files from other authors are closest to it. The command-line options for this program are listed below.

## How to build the program: 
To build the program, `$ make` or `$ make all`. This builds the identify programs. Optionally, typing `$ make identify` builds the program individually.

## How to run the program:
To clean the program, type `$ make clean`. To run the program, type `$ ./identify`.

## Command-Line Options:
The command-line options for the identify program are the following:

 - h: Prints the usage message, and purpose of the program.
 - n: The path to the noise text file.
 - k: The number of matches to print in the program. 
 - l: The number of noise words to filter out.
 - e: Set distance metric to be Euclidean.
 - m: Set distance metric to be Manhattan.
 - c: Set distance metric to be Cosine.
 - d: The path to the data base of authors.

## Scan-build:
Scan-build revealed an errors --it claims there might be a memory leak in pq.c.

## Notes:
Because my priority queue was implemented using insertion sort, it takes several minutes to run on my virtual machine (even longer if the input text size is larger), but it does eventually output a result.
The performance would be improved if heap sort was used instead.
