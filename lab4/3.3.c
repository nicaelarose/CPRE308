#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
void exception();
int main()  {
	signal(SIGFPE, exception);
	int a = 4;
	a = a/0;
}

void exception() {
	printf("Caught a SIGFPE.\n");
	exit(0);
}
