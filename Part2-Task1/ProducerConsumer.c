#include "BoundedBuffer.h"
#include "Request.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;	//mutex lock to provide mutual exclusion for the buffer
pthread_cond_t fullBuffer = PTHREAD_COND_INITIALIZER;  //the producer waits on this condition variable if the buffer is full
pthread_cond_t emptyBuffer = PTHREAD_COND_INITIALIZER; //consumers wait on this condition variable if the buffer is empty
int producerMaxRandSleep;	//max seconds for the producer to sleep between generating requests
int maxRequestLength;	//max amount of time for a request to take to process
BoundedBuffer *circQ;	//buffer for requests

typedef struct thread_data_t thread_data_t;
struct thread_data_t{
  int tid;
};

void *producer(){
	static int requestCounter = 1;	//for identifying requests
	Request *req;
	while(1){
		pthread_mutex_lock(&lock); 

		while(buffer_full(circQ)){
			pthread_cond_wait(&fullBuffer, &lock); //if full wait
		}
		req = new_request(requestCounter++, (rand()%maxRequestLength)+1); //generate request
		add_request(circQ, req);
		printf("Producer: produced request ID %d, length %d seconds\n",req->id,req->length);
		int sleepFor = rand()%(producerMaxRandSleep+1);
		printf("Producer: sleeping for %d seconds\n", sleepFor);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&emptyBuffer); //for those waiting for requests to process
		sleep(sleepFor); //sleep for random interval
	}
	pthread_exit(NULL);
}

void *consumer(void *arg){
	thread_data_t *dat = ((thread_data_t *)arg);
	Request *req;
	while(1){
		pthread_mutex_lock(&lock);

		while(buffer_empty(circQ)){
			pthread_cond_wait(&emptyBuffer, &lock);
		}
		req = remove_request(circQ);
		pthread_mutex_unlock(&lock);
		pthread_cond_signal(&fullBuffer);
		printf("Consumer %d: assigned request ID %d, processing request for the next %d seconds \n", dat->tid, req->id, req->length);
		sleep(req->length);
		printf("Consumer %d: completed request ID %d \n", dat->tid, req->id);

		free_request(req);

	}
	pthread_exit(NULL);
}


int main(int argc, char *argv[]){
	int numConsumers;
	int err = 0;
	time_t t;
	srand((unsigned) time(&t));//seed random number generator

	if(argc < 4){
		fprintf(stderr, "Too few arguments\n");
		return 1;
	}
	producerMaxRandSleep = atoi(argv[1]);
	numConsumers = atoi(argv[2]);
	maxRequestLength = atoi(argv[3]);
	//printf("producer will sleep no longer than %d, there will be %d consumers, and the max request length will be %d.\n", producerMaxRandSleep, numConsumers, maxRequestLength);

	circQ = new_bounded_buff(BUFFER_SIZE); //initialize the request buffer
	//pthread_mutex_init(&lock, NULL); //initialize the lock mutex
	//pthread_cond_init(&fullBuffer, NULL); //initialize full buffer condition variable
	//pthread_cond_init(&emptyBuffer, NULL); //initialize full buffer condition variable


	pthread_t thrp; 
	//pthread_t *thrc = (pthread_t *)malloc(sizeof(pthread_t)*(numConsumers)); //dynamicly created array of threads
	pthread_t thrc[numConsumers];

	pthread_mutex_lock(&lock);

	/*create consumers*/
	thread_data_t *tdata = (thread_data_t *)malloc(sizeof(thread_data_t)*numConsumers);
	for(int j=0; j<numConsumers; j++){
		tdata[j].tid = j+1;
		if((err = pthread_create(&thrc[j], NULL, consumer, &tdata[j]))){
			fprintf(stderr, "error: pthread_create %d\n", err);
			return 1;
		}
		//printf("consumer %d created\n", (j+1));
	}

	/*create producer*/
	if((err = pthread_create(&thrp, NULL, producer, NULL))){
		fprintf(stderr, "error: pthread_create %d\n", err);
		return 1;
	}

	pthread_mutex_unlock(&lock);

	/*collect prducer and consumers*/
	pthread_join(thrp, NULL);
	for (int i = 0; i < numConsumers; ++i) {//i<= to include the producer
    pthread_join(thrc[i], NULL);
  }
	return 0;
}
