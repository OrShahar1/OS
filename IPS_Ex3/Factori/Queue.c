
//// include headers --------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_mgr.h"
#include "Queue.h"

// structs  -------------------------------------------------------------------



//// consts  ----------------------------------------------

static const int EMPTY_QUEUE_VALUE = -1;

//// functions declarations  ----------------------------------------------

node* delete_node_and_return_next(node* current_node);
error_code_t new_node(node ** p_node, int data);


error_code_t InitializeQueue(queue **p_my_queue)
{
	error_code_t status = SUCCESS_CODE;

	queue *my_queue = (queue*)malloc(sizeof(queue));
	status = check_mem_alloc(my_queue, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		return status; 

	my_queue->queue_head = NULL;

	*p_my_queue = my_queue;
	return status;
}

int Top(queue* my_queue)
{

	if (Empty(my_queue)) 
		return EMPTY_QUEUE_VALUE;

	return my_queue->queue_head->data;
}

int Pop(queue* my_queue) 
{
	int data;

	if (Empty(my_queue)) 
		return EMPTY_QUEUE_VALUE;

	data = my_queue->queue_head->data;

	my_queue->queue_head = delete_node_and_return_next(my_queue->queue_head);

	return data;
}

error_code_t Push(queue* my_queue, int data)
{	
	error_code_t status = SUCCESS_CODE;
	node *p_end, *p_new_node;

	status = new_node(&p_new_node, data);

	if (status != SUCCESS_CODE)
		return status; 

	if (Empty(my_queue))
	{
		my_queue->queue_head = p_new_node;
		return status; 
	}

	p_end = my_queue->queue_head;

	while (p_end->next != NULL)
		p_end = p_end->next;

	p_end->next = p_new_node;

	return status;
}

bool Empty(queue* my_queue)
{
	return (my_queue->queue_head == NULL);
}

void DestroyQueue(queue** p_my_queue)
{
	node* p_queue_node = (*p_my_queue)->queue_head;

	while (p_queue_node != NULL)
		p_queue_node = delete_node_and_return_next(p_queue_node);

	free(*p_my_queue);

	*p_my_queue = NULL;
}

error_code_t new_node(node** p_p_node, int data)
{
	error_code_t status;
	node* p_new_node;

	p_new_node = (node*)malloc(sizeof(node));
	status = check_mem_alloc(p_new_node, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		return status;

	p_new_node->next = NULL;
	p_new_node->data = data;

	*p_p_node = p_new_node;

	return status;
}

node* delete_node_and_return_next(node* current_node)
{
	node* next_node = current_node->next;
	free(current_node);

	return next_node;

}


