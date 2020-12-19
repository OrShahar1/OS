
// include headers ------------------------------------------------------------

#include <stdlib.h>

#include "helpers.h"

// functions declarations  ----------------------------------------------------

/// print_queue
/// inputs:  my_queue
/// outputs: -
/// summary: Auxiliary function for printing the queue
///
void print_queue(queue* my_queue)
{
	if (Empty(my_queue)) {
		printf("[]\n");
		return;
	}
	node* current_node = my_queue->queue_head;
	printf("[%d", current_node->data);
	current_node = current_node->next;

	while (current_node != NULL) {
		printf(", %d", current_node->data);
		current_node = current_node->next;
	}
	printf("]\n");
}


