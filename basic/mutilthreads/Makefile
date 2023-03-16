a.out: thread_sync.o
	gcc -o a.out thread_sync.o -lpthread -D -REENTRANT
thread-sync.o: thread_sync.c
	gcc -c thread_sync.c

clean:
	rm -rf *.o *.out