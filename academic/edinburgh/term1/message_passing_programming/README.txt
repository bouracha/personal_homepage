The code will work for any number of processors except in the very rare case where the last
processor would have an endpoint less than its startpoint. They code shall exit with a statement
in this case. For example it shall not work for prime number of cores greater than 17 for the smallest image.


Comiple and Run.

A Makefile is included which uses the compiler flag fastsse for optimisation.

we can compile the program with:
$ make clean && make
This shall also remove any old scripts or other files cluttering the directory.
The program may be run locally for example using:
$ mpiexec -n 2 ./execute edgefiles/edgenew192x128.pgm 0.1

Where 2 is the number of cores used;
edgefiles/edgenew192x128.pgm is the path to the edge file we wish to process;
0.1 is the tolerance. ie program will terminate when the global max pixel change is less than 0.1 (must be less than 1.0)

We have included a script (script.sge) which shall specify to morar to execute the code 100 times
each for cores 1, 2, 4, 8, 16, 32, 64. 

All print statements are commented out in the code so running lots of times as in script.sge is pointless
However if you wish to uncomment the print statements then the job may be submitted to morar using qsub script.sge.
