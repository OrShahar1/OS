
// include headers ------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "error_mgr.h"
#include "Queue.h"


// consts  --------------------------------------------------------------------

static const int EMPTY_QUEUE_VALUE = -1;

// functions declarations  ----------------------------------------------------

node* delete_node_and_return_next(node* current_node);
error_code_t new_node(node** p_p_node, int data);

// functions implementations  -------------------------------------------------

/// InitializeQueue
/// inputs:  p_my_queue
/// outputs: error_code
/// summary: allocates memory for queue structs and initializes it with NULL
/// 
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

/// Top
/// inputs:  my_queue
/// outputs: value (int)
/// summary: returns the value of the first element in queue, 
///			 if queue is empty --> returns EMPTY_QUEUE_VALUE
/// 
int Top(queue* my_queue)
{

	if (Empty(my_queue)) 
		return EMPTY_QUEUE_VALUE;

	return my_queue->queue_head->data;
}

/// Pop
/// inputs:  my_queue
/// outputs: value (int)
/// summary: returns the value of the first element in queue and removes it from the queue,
///			 if queue is empty --> returns EMPTY_QUEUE_VALUE 
/// 
int Pop(queue* my_queue) 
{
	int data;

	if (Empty(my_queue)) 
		return EMPTY_QUEUE_VALUE;

	data = my_queue->queue_head->data;

	my_queue->queue_head = delete_node_and_return_next(my_queue->queue_head);

	return data;
}

/// Push
/// inputs:  my_queue, data
/// outputs: error_code 
/// summary: adds new value to the end of the queue. 
/// 
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

/// Empty
/// inputs:  my_queue
/// outputs: bool 
/// summary: returns true if queue is empty, false otherwise.
/// 
bool Empty(queue* my_queue)
{
	return (my_queue->queue_head == NULL);
}

/// DestroyQueue
/// inputs:  p_my_queue
/// outputs: - 
/// summary: frees all elements in the queue and sets the queue pointer to NULL. 
/// 
void DestroyQueue(queue** p_my_queue)
{
	node* p_queue_node = (*p_my_queue)->queue_head;

	while (p_queue_node != NULL)
		p_queue_node = delete_node_and_return_next(p_queue_node);

	free(*p_my_queue);

	*p_my_queue = NULL;
}

/// new_node
/// inputs:  p_p_node, data
/// outputs: error_code
/// summary: creates new node with value that can be added to the queue.  
/// 
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

/// delete_node_and_return_next
/// inputs:  current_node
/// outputs: node*
/// summary: frees current node and returns the next node (can be NULL).   
/// 
node* delete_node_and_return_next(node* current_node)
{
	node* next_node = current_node->next;
	free(current_node);

	return next_node;

}


