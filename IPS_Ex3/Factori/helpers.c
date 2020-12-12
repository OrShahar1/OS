
#include <stdlib.h>
#include "helpers.h"

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


