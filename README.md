scc
===

C Compiler hacking - starting with Small-C by Ron Cain

First I used the version from here:

http://www.physics.rutgers.edu/~vitchev/smallc-i386.html

Evguenily Vitchev add for and do statements while outputing 32bit GAS 
assembler suitable for the GNU toolchain.  This lets you compile 
32 bit programs. 

From EV website:

    Build a stage 1 compiler:
    gcc -o smc386c1 smc386c.c
    Using the stage 1 compiler build a stage 2 compiler:
    ./smc386c1
    Output filename? smc386c2.s
    Input filename? smc386c.c
    Input filename? <enter>
    There were 0 errors in compilation.

    You must add -m32 so gcc compiles for 32bit
    gcc -m32 -o smc386c2 smc386c2.s
    In order to make sure everything went properly, go to stage 3:
    ./smc386c2
    Output filename? smc386c3.s
    Input filename? smc386c.c
    Input filename? <enter>

    diff smc386c2.s smc386c3.s
    If diff doesn't produce output, this means the bootstrap was successful, and you can use the stage 2 compiler smc386c2.


This is the first copy of Small-C that I found that ouputs a usable 
ASM file for modern Linux systems.  Of note however that the original Small-C
I could find, did compile successfully, I just had to rename inline function
to inline2 as inline is used by gcc.  It output asm for the 8080 which 
of course you cannot use without an emulator but it's pretty amazing that
something 30 years old compiled that easily.


