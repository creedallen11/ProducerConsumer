#ifndef __BOUNDEDBUFFER_H__
#define __BOUNDEDBUFFER_H__
#include "Request.h"

typedef struct BoundedBuffer BoundedBuffer;

struct BoundedBuffer{
	int bufferMaxSize;
	int elements;
	Request *head;
	Request *tail;
};

BoundedBuffer *new_bounded_buff(int size);
Request *remove_request(BoundedBuffer *buff);
void add_request(BoundedBuffer *buff, Request * req);
int buffer_full(BoundedBuffer *buff);
int buffer_empty(BoundedBuffer *buff);


#endif
