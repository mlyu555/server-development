.PHONY:clean

threadpooltest: threadpool.o
	gcc -o threadpooltest threadpool.c -lpthread

clean:
	rm -rf *.o
	rm threadpooltest