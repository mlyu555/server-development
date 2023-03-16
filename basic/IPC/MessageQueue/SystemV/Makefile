.PHONY: all
all: mq1_sender mq1_recver

mq1_sender: mq1_sender.o
	gcc -o mq1_sender mq1_sender.o

mq1_recver: mq1_recver.o
	gcc -o mq1_recver mq1_recver.o

.PHONY: clean
clean:
	rm -rf *.o
	rm mq1_sender
	rm mq1_recver