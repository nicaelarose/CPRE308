appserver: Bank.o appserver.o
	cc -pthread -o appserver Bank.o appserver.o

Bank: Bank.c
	gcc -c Bank.c

server: appserver.c
	gcc -c appserver.c

clean:
	rm -f Bank.o appserver.o
