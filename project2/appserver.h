#include "Bank.h"

struct account{
	pthread_mutex_t lock;
	int value;
};

struct transfer{

	int acc_id;
	int amount;
};

struct request{
	struct * request next;
	int request_id;
	int check_acc_id;
	struct transfer * transfers;
	int num_transfers;
	struct timeval starttime, endtime;
};

struct queue {
	struct request * head, * tail;
	int num_jobs;
};

