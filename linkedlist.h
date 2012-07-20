#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED
/* source http://en.literateprograms.org/Singly_linked_list_(C) */

#include <stdlib.h>

typedef struct node_s {
  void *data;
  struct node_s *next;	
} list_node;

list_node* list_create(void* data);

list_node* list_insert_start(list_node* list, void* data);

list_node* list_insert_after(list_node* node, void* data);

int list_remove(list_node* list, list_node* node);

int list_foreach(list_node* list, int(*func)(void*));

list_node* list_find(list_node* node, int(*func)(void*, void*), void *data);

void list_free(list_node* list);

#endif /* LINKEDLIST_H_INCLUDED */
