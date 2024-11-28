At this point, the project should meet all of the basic requirements.

Through two functions, we can emulate SJF and FCFS scheduling. When they run,
processes (and page tables within them generate address translations from virtual
to physical at the same time). The debug output shows how the scheduling works, and
how the memory is being translated from virtual to physical.

I switched to C++ halfway through to make use of the standard library. Still,
a lot of the program is written in a C style. 

To run the program, type "make clean" to remove any old source files, then "make" to
generate an executable. "./main" will execute the program.