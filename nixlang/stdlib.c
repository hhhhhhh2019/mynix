#include "stdlib.h"
#include "evalute.h"
#include <stdio.h>


Object* stdlib_atoi(Object* arg) {
	if (arg->type != OBJECT_STRING)
		return NULL;

	Object_string* arg_value = arg->data;

	printf("%lu\n", sizeof(Object_number));
	Object_number* res_value = malloc(sizeof(Object_number));
	res_value->value = atoi(arg_value->value);

	Object* result = malloc(sizeof(Object));
	result->type = OBJECT_NUMBER;
	result->data = res_value;

	return result;
	
	return NULL;
}
