.PHONY: all
all: producer customer

producer: producer.o
	gcc -o producer producer.o -lrt

customer: customer.o
	gcc -o customer customer.o -lrt

.PHONY: clean
clean:
	rm -rf *.o
	rm producer
	rm customer