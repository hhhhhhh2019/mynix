#include "utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


Malloc_info* malloc_info = NULL;


void* wmalloc(size_t size) {
	assert(malloc_info != NULL);

	void* result = malloc(size);

	Malloc_node* node = malloc(sizeof(Malloc_node));
	node->next = NULL;
	node->address = result;

	if (malloc_info->start == NULL) {
		node->prev = NULL;
		node->next = NULL;
		malloc_info->start = node;
		malloc_info->end = node;
		return result;
	}

	node->prev = malloc_info->end;
	malloc_info->end->next = node;
	malloc_info->end = node;

	return result;
}


void* wzmalloc(size_t size) {
	void* result = wmalloc(size);
	memset(result, 0, size);
	return result;
}


void* wrealloc(void* address, size_t size) {
	Malloc_node* node = malloc_info->start;

	while (node != NULL) {
		if (node->address == address)
			break;
		node = node->next;
	}

	assert(node != NULL);

	node->address = realloc(node->address, size);

	return node->address;
}


void wfree(void* address) {
	Malloc_node* node = malloc_info->start;

	printf("---------\n");

	while (node != NULL) {
		printf("%p %p\n", address, node->address);
		if (node->address == address)
			break;
		node = node->next;
	}

	printf("%p\n\n", node);

	assert(node != NULL);

	free(node->address);

	if (node->prev != NULL)
		node->prev->next = node->next;
	else {
		malloc_info->start = NULL;
		malloc_info->end = NULL;
	}

	free(node);
}


void wfree_all(Malloc_info minfo) {
	Malloc_node* node = minfo.start;

	while (node != NULL) {
		free(node->address);
		Malloc_node* next = node->next;
		free(node);
		node = next;
	}
}
