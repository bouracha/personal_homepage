Greater than 19 MPI process are necessary for the program to run.
16, one for each cellactor. 1 for the clockactor. 1 for the master actor.
And one as the master process.

The number of initial squirrels may be edited at the top of the masteractor.c code.
The maximum population for the environment may also be edited here. For runs
on Morar the maximum population is set to 45 when running on 64 cores as this does
not allow us to run out of MPI processes in our process pool.

The parameters is currently set to 34 squirrels (30 healthy, 4 alive max_pop = 45).
A sample sumission script "submit.sge" is also provided which submits the problem
to morar on 64 cores with: $qsub submit.sge

