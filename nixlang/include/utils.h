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
	stack.values = realloc(stack.values, \
		(++stack.count) * sizeof(stack.values[0]));\
	stack.values[stack.count - 1] = value; \
} while (0)

#define stack_pop(stack) (stack.values[--stack.count])


#endif

