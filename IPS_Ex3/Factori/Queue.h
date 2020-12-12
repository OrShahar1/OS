#ifndef QUEUE_H 
#define QUEUE_H

#include <stdbool.h>
#include "error_mgr.h"

typedef struct _node {
	int data;
	struct _node* next;
}node;

typedef struct _queue {
	node* queue_head;
} queue;

error_code_t InitializeQueue(queue** p_my_queue);
error_code_t Push(queue* my_queue, int data);
int Top(queue* my_queue);
int Pop(queue* my_queue);
bool Empty(queue* my_queue);
void DestroyQueue(queue** p_my_queue);


#endif
