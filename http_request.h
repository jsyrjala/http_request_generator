#ifndef HTTP_REQUEST_H_INCLUDED
#define HTTP_REQUEST_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linkedlist.h"
/* see command AT+HTTPACTION= */

#define HTTP_REQUEST_BUF_SIZE 100

typedef enum http_method_e {
  POST = 1,
  GET = 0
} http_method;

typedef struct http_field_s {
  char name[HTTP_REQUEST_BUF_SIZE];
  char value[HTTP_REQUEST_BUF_SIZE];
} http_field;

typedef struct http_request_s {
  http_method method;
  char cid[2];
  char url[HTTP_REQUEST_BUF_SIZE];
  char user_agent[HTTP_REQUEST_BUF_SIZE];
  char content_type[HTTP_REQUEST_BUF_SIZE];
  int redirect;
  int timeout;

  list_node* fields;
} http_request;
 
http_request* http_request_create(http_method method, char* url, int timeout);

void http_request_destroy(http_request* request);

http_field* http_create_field(char* name, char* value);

int http_request_send(http_request* request);

#endif /* HTTP_REQUEST_H_INCLUDED */
