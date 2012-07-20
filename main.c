#include <stdio.h>
#include "linkedlist.h"
#include "http_request.h"

int printstring(void *s)
{
  printf("%s\n", (char *)s);
  return 0;
}

void linked_list_tests() {
  printf("\n---------\nlinked_list_tests()\n");
  list_node* list = list_create((void*) "first");
  list_node* third = list_insert_start(list, (void*) "third");
  list_node* second = list_insert_after(list, (void*) "second");
  list_node* fourth = list_insert_after(list, (void*) "fourht");

  printf("trial 1\n");
  list_foreach(list, printstring);
  printf("trial 2\n");
  list_foreach(third, printstring);
  printf("trial 3\n");
  printf("remove success %d\n", list_remove(third, fourth));
  list_foreach(third, printstring);
  list_free(third);
}

void http_request_tests() {
  printf("\n---------\nhttp_request_tests()\n");
  http_request* request = http_request_create(POST, "http://www.example.com", 45);
  printf("url: %s\n", request->url);
  http_field* latitude_field = http_create_field("latitude", "5321.6802,N");
  http_field* longitude_field = http_create_field("longitude", "00630.3372,W");
  request->fields = list_create(latitude_field);
  list_insert_after(request->fields, longitude_field);
  http_request_send(request);
}

int main(void) {
  linked_list_tests();
  http_request_tests();
}
