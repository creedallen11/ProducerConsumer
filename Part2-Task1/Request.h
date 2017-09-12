#ifndef __REQUEST_H__
#define __REQUEST_H__

typedef struct Request Request;

struct Request{
	int id;
	int length;
	Request *next;
};

Request *new_request(int id, int length);

void free_request(Request* req);

#endif
