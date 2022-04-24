#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "appserver.h"

void * workerFunc();
void push(int reqID);
request pop();

queue * queue;
account * account;
pthread_mutex_t qMutex;
FILE *f;
int id = 1;


int main(int argc, char * argv[])
{
	if(argc != 4)
	{
		printf("Incorrect input\n");
		exit(1);
	}
	
	queue = (queue*) malloc(sizeof(queue));
	queue -> head = NULL;
	queue -> tail = NULL;
	queue -> num_jobs = 0;
	
	pthread_mutex_init(&qMutex, NULL);
	
	int thread_Count = atoi(argv[1]);
	int account_Count = atoi(argv[2]);
	
	char fileName[strlen(argv[3]+1)];
	strcpy(fileName, argv[3]);
	fileName[strlen(argv[3])] = '\0';
	
	outFile = fopen(fileName , "w");
	
	initialize_accounts(account_Count);
	int i;
	for(i=0; i<account_Count; i++)
	{
		pthread_mutex_init(&(account[i].lock), NULL);
		account[i].value= 0;
	}
	
	pthread_t workers[thread_Count];
	int i;
	for(i=0; i<thread_Count; i++){
		pthread_create(&workers[i], NULL, workerFunc, NULL)
	}
	
	while(1){
		printf("> ");
		
		char request[500];
		fgets(request, 500, stdin);
		request[strlen(request)-1] = '\0';
		
		if((strcmp(request, "END"))==0){
			break;
		}
		
		pthread_mutex_lock(&qMutex);
		push(request, id);
		printf("< ID %d\n", id);
		pthread_mutex_unlock(&qMutex);
		id++;
	}
	int i;
	for(i=0; i<thread_Count; i++){
		pthread_join(workers[i], NULL);
	}
	
	free(account);
	free(queue);
	fclose(outFile);
	return 0;
}

void push(int reqID){
	request * new_Request = malloc(sizeof(request));
	new_Request -> request_id = reqID;
	gettimeofday(&(new_Request -> starttime), NULL);
	new_Request -> next = NULL;
	
	if(queue->count >0)[
		queue->tail->next = new_Request;
		queue->tail = new_Request;
		queue->num_jobs = queue->num_jobs + 1;
	}
	
	else{
		queue->head = new_Request;
		queue->tail = new_Request;
		queue->num_jobs = 1; 
	}
	
}


request pop(){
	request *temp;
	request toPop;
	
	if(queue->num_jobs >0){
		toPop.request_id = queue->head->request_id;
		toPop.timeStart = queue->head->starttime;
		toPop.next = NULL;
		
		temp = queue->head;
		queue->head = queue->head->next;
		free(temp);
		
		if(!queue->head){
			queue->tail = =NULL;
		}
		queue->num_jobs = queue->num_jobs -1;
	}
	return toPop;
}

void * workerFunc()
{
	
}


















