#include <pthread.h>
#include <stdio.h>

void* thread1();
void* thread2();

int main() {
	pthread_t i1;
	pthread_t i2;

	pthread_create(&i1, NULL, (void*)&thread1, NULL);
	pthread_create(&i2, NULL, (void*)&thread2, NULL);

	pthread_join(i1, NULL);
	pthread_join(i2, NULL);

	printf("Hello from main\n");

	return 0;
}

void* thread1() {
	sleep(5);
	printf("Hello from thread1\n");
}

void* thread2() {
	sleep(5);
	printf("Hello from thread2\n");
}
