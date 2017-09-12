#include "Request.h"
#include <stdlib.h>

Request *new_request(int id, int length){
	Request *req = (Request *)malloc(sizeof(Request));
	req->id = id;
	req->length = length;
	req->next = NULL;
	return req;
}

void free_request(Request* req){
	free(req);
}
