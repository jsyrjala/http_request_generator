#include "linkedlist.h"

list_node* list_create(void* data) 
{
  list_node* node;
  node = malloc(sizeof(list_node));
  if(!node) {
    //  fail
    return NULL;
  }
  node->data=data;
  node->next=NULL;
  return node;
}

list_node* list_insert_start(list_node* list, void* data) {
  list_node* new_node;
  new_node = list_create(data);
  new_node->next = list;
  return new_node;
}

/**
 * returns the newly created element, not the first element.
 */
list_node* list_insert_after(list_node* node, void* data) 
{
  list_node* new_node;
  new_node = list_create(data);
  new_node->next = node->next;
  node->next = new_node;
  return new_node;
}

void list_free(list_node* list) 
{
  if(!list) {
    return;
  }
  while(list->next) {
    list_node* tmp = list->next;
    free(list);
    list = tmp;
  }
  free(list);
}

int list_remove(list_node* list, list_node* node)
{
  while(list->next && list->next!=node) {
    list=list->next;
  }
  if(list->next) {
    list->next=node->next;
    free(node);
    return 0;		
  } else {
    return -1;
  }
}

int list_foreach(list_node* node, int(*func)(void*))
{
  while(node) {
    if(func(node->data) != 0) {
      return -1;
    }
    node = node->next;
  }
  return 0;
}

list_node* list_find(list_node* node, int(*func)(void*, void*), void *data) {
  while(node) {
    if(func(node->data, data) > 0) {
      return node;
    }
  }
  return NULL;
}
