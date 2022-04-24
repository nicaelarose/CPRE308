/* t2.c
   synchronize threads through mutex and conditional variable 
   To compile use: gcc -o t2 t2.c -lpthread 
*/ 

#include <stdio.h>
#include <pthread.h>

void* 	hello();    // define two routines called by threads    
void* 	world();         	
void*	again();    // added a routine to be called by threads
 
/* global variable shared by threads */
pthread_mutex_t 	mutex;  		// mutex
pthread_cond_t 		done_hello; 	// conditional variable
int 			done = 0;      	// testing variable

int main (int argc, char *argv[]){
    pthread_t 	tid_hello, // thread id  
    		tid_world, 
		tid_again; //added thread id for again

    /*  initialization on mutex and cond variable  */ 
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&done_hello, NULL); 
    
    pthread_create(&tid_hello, NULL, (void*)&hello, NULL); //thread creation
    pthread_create(&tid_world, NULL, (void*)&world, NULL); //thread creation 
    pthread_create(&tid_again, NULL, (void*)&again, NULL); //added again thread creation

    /* main waits for the two threads to finish */
    pthread_join(tid_hello, NULL);  
    pthread_join(tid_world, NULL);
    pthread_join(tid_again, NULL); //added wait for again

    printf("\n");
    return 0;
}

void* hello() {
    pthread_mutex_lock(&mutex);
    printf("Hello ");
    fflush(stdout); 	// flush buffer to allow instant print out
    done = 1;
    pthread_cond_signal(&done_hello);	// signal world() thread
    pthread_mutex_unlock(&mutex);	// unlocks mutex to allow world to print
    return ;
}


void* world() {
    pthread_mutex_lock(&mutex);

    /* world thread waits until done == 1. */
    while(done == 0 || done == 2) //added another done == 2 
	pthread_cond_wait(&done_hello, &mutex);

    printf("World "); //added a space after world
    fflush(stdout);
    done = 2; //added done = 2
    pthread_cond_signal(&done_hello); //added signal word() thread
    pthread_mutex_unlock(&mutex); // unlocks mutex

    return ;
}

//added again()
void* again() {
	pthread_mutex_lock(&mutex);

	/*world thread waits until done == 2. */
	while(done == 0 || done == 1)
		pthread_cond_wait(&done_hello, &mutex);

	printf("Again!"); //print again
	fflush(stdout);
	pthread_mutex_unlock(&mutex); 
}
