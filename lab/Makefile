
output: lab1.o message.o
	gcc -o output lab1.o message.o

lab1.o: lab1.c
	gcc -c lab1.c

message.o: message.c message.h
	gcc -c message.c

clean:
	rm output lab1.o message.o
