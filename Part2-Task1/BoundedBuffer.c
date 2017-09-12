#include "BoundedBuffer.h"
#include "Request.h"
#include <stdlib.h>

BoundedBuffer *new_bounded_buff(int size){
	BoundedBuffer *buffer = (BoundedBuffer *)malloc(sizeof(BoundedBuffer));
	buffer->bufferMaxSize = size;
	buffer->head = NULL;
	buffer->elements = 0;
	return buffer;
}

Request *remove_request(BoundedBuffer *buff){
	Request *req = NULL;
	if(buff->elements > 0){
		req = buff->head;
		buff->head = req->next;
		(buff->elements)--;
		req->next= NULL;
	}
	if(buff->elements == 0){
		buff->tail = NULL;
	}
	return req;
}

void add_request(BoundedBuffer *buff, Request *req){
	if(buff->elements < buff->bufferMaxSize){
		if(buff->elements==0){
			buff->head = req;
			buff->tail = req;
			buff->elements=1;
		}
		else{
			buff->tail->next = req;
			buff->tail = req;
			(buff->elements) += 1;
		}
	}
}

int buffer_full(BoundedBuffer *buff){
	if(buff->elements == buff->bufferMaxSize){
		return 1;
	}
	return 0;
}

int buffer_empty(BoundedBuffer *buff){
	if(buff->elements == 0){
		return 1;
	}
	return 0;
}
