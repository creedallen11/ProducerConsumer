#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct q_node q_node;
typedef struct q q;

struct q_node {
  char *name;
  int arrivalTime;
  int runTime;
  int cyclesExecuted;
  int totalWaitTime;
  q_node *next;
};

struct q {
  q_node *head;
  q_node *tail;
};

void q_enqueue(q* q, q_node* node){
  if(q->head == NULL){
    q->head = node;
    q->tail = node;
  }
  else{
    q->tail->next = node;
    q->tail = node;
  }
  node->next = NULL;
}

q_node *q_dequeue(q* q){
  q_node *node = q->head;
  if(q->head == q->tail){
    q->head = NULL;
    q->tail = NULL;
  }
  else{
    q->head = node->next;
    node->next = NULL;
  }
  return node;
}

q *new_q(){
  q *newQueue = (q *)malloc(sizeof(q));
  newQueue->head = NULL;
  newQueue->tail = NULL;
  return newQueue;
}

q_node *new_node(char *name, int arrivalTime, int runTime){
  q_node *newNode = (q_node *)malloc(sizeof(q_node));
  newNode->name = name;
  newNode->arrivalTime = arrivalTime;
  newNode->runTime = runTime;
  newNode->totalWaitTime = 0;
  newNode->cyclesExecuted = 0;
  return newNode;
}

int main(int argc, char *argv[]){
  int t = 0; /*time*/
  int waitTime = 0;
  int processes = 0;
  q *readyQ = new_q(); /*ready q*/
  q *arrivalQ = new_q(); /*processes not yet arrived*/
  q *doneQ = new_q(); /*terminated processes*/
  q_node *temp;
  int simTime; /*how long to run simulation*/
  int maxLength;
  int jobStart, jobTime; /*Variables for reading in jobs from files*/
  jobStart = 0;
  jobTime = 0;

  /*Attempt to open trace file!*/
  if(argc != 2){
    fprintf(stderr, "Invalid # of arguments.\n");
    return 1;
  }
  FILE *trace;
  trace = fopen(argv[1], "r");
  if (trace == NULL) {
    fprintf(stderr, "Can't open trace file %s.\n", argv[1]);
    exit(1);
  }

  /*read in data from trace*/
  fscanf(trace, "%d", &processes);
  fscanf(trace, "%d", &simTime);
  fscanf(trace, "%d", &maxLength); /*we don't need to use max job length*/
  //printf("%d\n", processes);
  //printf("%d\n", simTime);
  while (fscanf(trace, "%d %d\n", &jobStart, &jobTime) == 2){
    q_enqueue(arrivalQ, new_node("JOB", jobStart, jobTime));
    //printf("%d %d\n", jobStart, jobTime);
  }
  fclose(trace);

  while(1){
    if(arrivalQ->head != NULL){
      if(t >= arrivalQ->head->arrivalTime){
        q_enqueue(readyQ, q_dequeue(arrivalQ));
        //printf("Job %s has arrived. Running Time:%d.\n",readyQ->tail->name ,readyQ->tail->runTime);
      }
    }

    if(readyQ->head != NULL){
      temp = q_dequeue(readyQ);

      //printf("Time slice #%d. The current job is %s. It has executed %d cycles.\n", t, temp->name, temp->cyclesExecuted);

      (temp->cyclesExecuted)++;
      if(temp->runTime == temp->cyclesExecuted){
        /*processes completed*/
        temp->totalWaitTime = t+1-(temp->cyclesExecuted)-(temp->arrivalTime);
        //printf("Job %s terminated. Total wait time:%d.\n", temp->name, temp->totalWaitTime);
        q_enqueue(doneQ, temp);
      }
      else{
        q_enqueue(readyQ, temp);
      }
    }

    if(readyQ->head == NULL && arrivalQ->head == NULL) break;
    if(t == simTime) break;
    t++;
    //printf("time is %d\n", t);
  }

  while(doneQ->head != NULL){
    temp = q_dequeue(doneQ);
    waitTime += temp->totalWaitTime;
    //printf("wait time %d\n", waitTime);
    /*processes++; known value*/
    //printf("Job %s waiting time:%d\n", temp->name, temp->totalWaitTime);
  }
  //printf("Average waiting time:%d\n", waitTime/processes);
  printf("RR %d-%d-%d: %d\n", processes, simTime, maxLength, (int) ceil((long)(waitTime)/(float)processes));

  return 0;
}
