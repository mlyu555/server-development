gcc -c get.c
gcc -c sum.c
gcc -c val.c
gcc -c main.c
gcc -o main main.o val.o sum.o get.o
