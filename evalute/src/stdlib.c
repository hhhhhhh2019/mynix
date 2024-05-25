#include "libs.h"
#include "evalute.h"
#include "object.h"
#include "utils.h"

#include <string.h>


void init_stdlib() {
	Object* len_obj = wmalloc(sizeof(Object));
	len_obj->type = OBJECT_FUNCTION_EXTERNAL;
	len_obj->data = wmalloc(sizeof(Object_function_external));
	((Object_function_external*)len_obj->data)->body = stdlib_len;


	Object* stdlib = wmalloc(sizeof(Object));
	stdlib->type = OBJECT_SET;
	stdlib->data = wmalloc(sizeof(Object_set));

	Object_set* stdlib_set = stdlib->data;

	stdlib_set->count = 0;
	stdlib_set->elems = wmalloc(0);


	stdlib_set->elems = wrealloc(stdlib_set->elems,
	                             sizeof(Set_arg) * (++stdlib_set->count));

	stdlib_set->elems[stdlib_set->count - 1].name = "len";
	stdlib_set->elems[stdlib_set->count - 1].value = len_obj;



	current_scope->variables = wrealloc(
		current_scope->variables,
		sizeof(Object_variable) * (++current_scope->variables_count));

	current_scope->variables[current_scope->variables_count-1].name = "stdlib";
	current_scope->variables[current_scope->variables_count-1].value = stdlib;
}


Object* stdlib_len(Object* obj) {
	long len = 0;

	if (obj->type == OBJECT_STRING) {
		Object_string* data = obj->data;

		len = strlen(data->value);
	}

	Object* result = wmalloc(sizeof(Object));
	result->type = OBJECT_NUMBER;
	result->data = wmalloc(sizeof(Object_number));
	Object_number* data = result->data;
	data->value = len;

	return result;
}
