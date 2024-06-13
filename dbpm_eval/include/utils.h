#ifndef UTILS_H
#define UTILS_H


#include <stdio.h>


#define STR_HELP(x) #x
#define STR(x) STR_HELP(x)


#ifdef DEBUG
#define LOG(...) do { \
	printf("\033[38;5;4mInfo:\033[0m "); \
	printf(__VA_ARGS__); \
} while(0)
#else
#define LOG(...)
#endif

#define ERROR(...) do { \
	printf("\033[38;5;1mError:\033[0m "); \
	printf(__VA_ARGS__); \
} while(0)


#define Stack(type) struct { \
	unsigned int count; \
	type* values;\
}

#define stack_push(stack, value) do { \
	stack.values = wrealloc(stack.values, \
		(++stack.count) * sizeof(stack.values[0])); \
	stack.values[stack.count - 1] = value; \
} while (0)

#define stack_pop(stack) (stack.values[--stack.count])


#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)


struct Malloc_node;
typedef struct Malloc_node {
	struct Malloc_node* prev;
	struct Malloc_node* next;
	void* address;
} Malloc_node;

typedef struct {
	Malloc_node* start;
	Malloc_node* end;
} Malloc_info;

extern Malloc_info* malloc_info;

void* wmalloc(size_t);
void* wzmalloc(size_t);
void* wrealloc(void*, size_t);
void wfree(void*);
void wfree_all(Malloc_info);


#endif
