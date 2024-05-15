#include "evalute.h"
#include "lexer.h"
#include "libs/stdlib.h"
#include "synt.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* object_type_names[] = {
	[OBJECT_NUMBER] = "OBJECT_NUMBER",
	[OBJECT_FLOAT_NUMBER] = "OBJECT_FLOAT_NUMBER",
	[OBJECT_BOOLEAN] = "OBJECT_BOOLEAN",
	[OBJECT_STRING] = "OBJECT_STRING",
	[OBJECT_PATH] = "OBJECT_PATH",
	[OBJECT_ARRAY] = "OBJECT_ARRAY",
	[OBJECT_SET] = "OBJECT_SET",
	[OBJECT_FUNCTION] = "OBJECT_FUNCTION",
	[OBJECT_FUNCTION_EXTERNAL] = "OBJECT_FUNCTION_EXTERNAL",
	[OBJECT_VARIABLE] = "OBJECT_VARIABLE",
	[OBJECT_NAME] = "OBJECT_NAME",
	[OBJECT_OPERATION] = "OBJECT_OPERATION",
};

char* operation_type_names[] = {
	[OP_EQ] = "OP_EQ",
	[OP_NEQ] = "OP_NEQ",
	[OP_LESS] = "OP_LESS",
	[OP_ELESS] = "OP_ELESS",
	[OP_MORE] = "OP_MORE",
	[OP_EMORE] = "OP_EMORE",
	[OP_ASSIGN] = "OP_ASSIGN",
	[OP_SUM] = "OP_SUM",
	[OP_SUB] = "OP_SUB",
	[OP_MUL] = "OP_MUL",
	[OP_DIV] = "OP_DIV",
	[OP_AND] = "OP_AND",
	[OP_OR] = "OP_OR",
	[OP_XOR] = "OP_XOR",
	[OP_NOT] = "OP_NOT",
	[OP_DOT] = "OP_DOT",
	[OP_CALL] = "OP_CALL",
};


Scope* global_scope;
Scope* current_scope;


void init_def_vars() {
	Object_variable var_true;
	Object_boolean* var_true_value = malloc(sizeof(Object_boolean));
	var_true_value->value = 1;
	var_true.name = "true";
	var_true.value = malloc(sizeof(Object));
	var_true.value->type = OBJECT_BOOLEAN;
	var_true.value->data = var_true_value;

	Object_variable var_false;
	Object_boolean* var_false_value = malloc(sizeof(Object_boolean));
	var_false_value->value = 0;
	var_false.name = "false";
	var_false.value = malloc(sizeof(Object));
	var_false.value->type = OBJECT_BOOLEAN;
	var_false.value->data = var_false_value;


	Object_variable var_stdlib;
	Object_set* var_stdlib_value = malloc(sizeof(Object_set));
	var_stdlib.name = "stdlib";
	var_stdlib_value->count = 1;
	var_stdlib_value->elems = malloc(sizeof(Set_arg) * var_stdlib_value->count);
	var_stdlib.value = malloc(sizeof(Object));
	var_stdlib.value->type = OBJECT_SET;
	var_stdlib.value->data = var_stdlib_value;


	Object_function_external* func_stdlib_atoi = malloc(sizeof(Object_function_external));
	func_stdlib_atoi->body = stdlib_atoi;

	Object* obj_stdlib_atoi = malloc(sizeof(Object));
	obj_stdlib_atoi->type = OBJECT_FUNCTION_EXTERNAL;
	obj_stdlib_atoi->data = func_stdlib_atoi;

	var_stdlib_value->elems[0].name = "atoi";
	var_stdlib_value->elems[0].value = obj_stdlib_atoi;


	global_scope = malloc(sizeof(Scope));


	global_scope->variables_count = 3;

	global_scope->variables = malloc(
	    sizeof(Object_variable) * global_scope->variables_count);

	global_scope->variables[0] = var_true;
	global_scope->variables[1] = var_false;
	global_scope->variables[2] = var_stdlib;

	current_scope = global_scope;
}


Object* node_to_object(Node* node) {
	Object* object = malloc(sizeof(Object));

	if (node->token.type == ASSIGN) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_ASSIGN;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}

	else if (node->token.type == EQUALS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_EQ;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	else if (node->token.type == NOT_EQUALS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_NEQ;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	else if (node->token.type == LESS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_LESS;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	else if (node->token.type == LESS_EQUALS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_ELESS;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	else if (node->token.type == MORE) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_MORE;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	else if (node->token.type == MORE_EQUALS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_EMORE;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	else if (node->token.type == ASSIGN) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_ASSIGN;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	else if (node->token.type == PLUS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_SUM;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}

	else if (node->token.type == MINUS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_SUB;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}

	else if (node->token.type == STAR) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_MUL;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}

	else if (node->token.type == SLASH) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_DIV;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}

	else if (node->token.type == PIPE) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_OR;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}

	else if (node->token.type == AMPERSAND) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_AND;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}

	else if (node->token.type == CARET) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_XOR;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}

	else if (node->token.type == EXCLAMATION) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_NOT;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}

	else if (node->token.type == Call) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_CALL;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}

	else if (node->token.type == DEC_NUMBER) {
		object->type = OBJECT_NUMBER;
		object->data = malloc(sizeof(Object_number));

		Object_number* data = object->data;

		data->value = atol(node->token.value);
	}

	else if (node->token.type == FLOAT_NUMBER) {
		object->type = OBJECT_FLOAT_NUMBER;
		object->data = malloc(sizeof(Object_float));

		Object_float* data = object->data;

		data->value = atof(node->token.value);
	}

	else if (node->token.type == STRING) {
		object->type = OBJECT_STRING;
		object->data = malloc(sizeof(Object_string));

		Object_string* data = object->data;

		data->value = node->token.value;
	}

	else if (node->token.type == PATH) {
		object->type = OBJECT_PATH;
		object->data = malloc(sizeof(Object_path));

		Object_path* data = object->data;

		data->value = node->token.value;
	}

	else if (node->token.type == Func_decl) {
		object->type = OBJECT_FUNCTION;
		object->data = malloc(sizeof(Object_function));

		Object_function* data = object->data;

		data->argument_name = node->childs[0]->token.value;
		data->body          = node_to_object(node->childs[1]);
	}

	// else if (node->token.type == UNDEFINED) {
	// 	object->type = OBJECT_VARIABLE_POINTER;
	// 	object->data = malloc(sizeof(Object_var_pointer));
	//
	// 	Object_var_pointer* data = object->data;
	//
	// 	data->name = node->token.value;
	// }

	else if (node->token.type == Name) {
		if (node->childs_count != 1) {
			object->type = OBJECT_OPERATION;
			object->data = malloc(sizeof(Object_operation));

			Object_operation* data = object->data;

			data->type = OP_DOT;
			data->count = 2;
			data->args = malloc(sizeof(void*) * data->count);

			data->args[0] = node_to_object(node->childs[0]);
			data->args[1] = node_to_object(node->childs[2]);
		} else {
			object->type = OBJECT_NAME;
			object->data = malloc(sizeof(Object_name));

			Object_name* data = object->data;

			data->name = node->childs[0]->token.value;
		}
	}

	else if (node->token.type == UNDEFINED) {
		object->type = OBJECT_NAME;
		object->data = malloc(sizeof(Object_name));

		Object_name* data = object->data;

		data->name = node->token.value;
	}

	else if (node->token.type == Array) {
		object->type = OBJECT_ARRAY;
		object->data = malloc(sizeof(Object_array));

		Object_array* data = object->data;
		data->elems = malloc(0);
		data->count = 0;

		Node* arg = node->childs[0];

		while (1) {
			if (arg->token.type == Aarg) {
				data->elems = realloc(data->elems, sizeof(Object*) * (++data->count));
				data->elems[data->count-1] = node_to_object(arg->childs[1]);
				arg = arg->childs[0];
			} else {
				data->elems = realloc(data->elems, sizeof(Object*) * (++data->count));
				data->elems[data->count-1] = node_to_object(arg);
				break;
			}
		}
	}

	// else if (node->token.type == Set) {
	// 	object->type = OBJECT_SET;
	// 	object->data = malloc(sizeof(Object_array));
	//
	// 	Object_set* data = object->data;
	// 	data->elems = malloc(0);
	// 	data->count = 0;
	//
	// 	Node* arg = node->childs[0];
	//
	// 	while (1) {
	// 		if (arg->token.type == Sargs) {
	// 			
	// 		} else {
	// 			
	// 		}
	// 	}
	// }
	
	else return NULL;

	return object;
};


static Object* evalute_eq(Object* left, Object* right) {
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a == b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		double a = ((Object_float*)  left->data)->value;
		long   b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a == b;

		return result;
	}

	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		long   a = ((Object_number*)left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a == b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a == b;

		return result;
	}

	return NULL;
}


static Object* evalute_neq(Object* left, Object* right) {
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a != b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		double a = ((Object_float*)  left->data)->value;
		long   b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a != b;

		return result;
	}

	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		long   a = ((Object_number*)left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a != b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a != b;

		return result;
	}

	return NULL;
}


static Object* evalute_more(Object* left, Object* right) {
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a > b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		double a = ((Object_float*)  left->data)->value;
		long   b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a > b;

		return result;
	}

	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		long   a = ((Object_number*)left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a > b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a > b;

		return result;
	}

	return NULL;
}


static Object* evalute_emore(Object* left, Object* right) {
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a >= b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		double a = ((Object_float*)  left->data)->value;
		long   b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a >= b;

		return result;
	}

	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		long   a = ((Object_number*)left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a >= b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a >= b;

		return result;
	}

	return NULL;
}


static Object* evalute_less(Object* left, Object* right) {
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a < b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		double a = ((Object_float*)  left->data)->value;
		long   b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a < b;

		return result;
	}

	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		long   a = ((Object_number*)left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a < b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a < b;

		return result;
	}

	return NULL;
}


static Object* evalute_eless(Object* left, Object* right) {
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a <= b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		double a = ((Object_float*)  left->data)->value;
		long   b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a <= b;

		return result;
	}

	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		long   a = ((Object_number*)left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a <= b;

		return result;
	}

	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_boolean*)result->data)->value = a <= b;

		return result;
	}

	return NULL;
}


static Object* evalute_sum(Object* left, Object* right) {
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_NUMBER;
		result->data = malloc(sizeof(Object_number));
		((Object_number*)result->data)->value = a + b;

		return result;
	}
	
	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a + b;

		return result;
	}
	
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_number*)left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a + b;

		return result;
	}
	
	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		double a = ((Object_float*)  left->data)->value;
		double b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a + b;

		return result;
	}

	return NULL;
}


static Object* evalute_sub(Object* left, Object* right) {
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_NUMBER;
		result->data = malloc(sizeof(Object_number));
		((Object_number*)result->data)->value = a - b;

		return result;
	}
	
	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a - b;

		return result;
	}
	
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_number*)left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a - b;

		return result;
	}
	
	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		double a = ((Object_float*)  left->data)->value;
		double b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a - b;

		return result;
	}

	return NULL;
}


static Object* evalute_mul(Object* left, Object* right) {
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_NUMBER;
		result->data = malloc(sizeof(Object_number));
		((Object_number*)result->data)->value = a * b;

		return result;
	}
	
	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a * b;

		return result;
	}
	
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_number*)left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a * b;

		return result;
	}
	
	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		double a = ((Object_float*)  left->data)->value;
		double b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a * b;

		return result;
	}

	return NULL;
}


static Object* evalute_div(Object* left, Object* right) {
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_NUMBER;
		result->data = malloc(sizeof(Object_number));
		((Object_number*)result->data)->value = a / b;

		return result;
	}
	
	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a / b;

		return result;
	}
	
	if (left->type  == OBJECT_NUMBER &&
	    right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_number*)left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a / b;

		return result;
	}
	
	if (left->type  == OBJECT_FLOAT_NUMBER &&
	    right->type == OBJECT_NUMBER) {
		double a = ((Object_float*)  left->data)->value;
		double b = ((Object_number*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = malloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a / b;

		return result;
	}

	return NULL;
}


static Object* evalute_and(Object* left, Object* right) {
	if (left->type  == OBJECT_BOOLEAN &&
	    right->type == OBJECT_BOOLEAN) {
		char a = ((Object_boolean*) left->data)->value;
		char b = ((Object_boolean*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_number*)result->data)->value = a && b;

		return result;
	}

	return NULL;
}


static Object* evalute_or(Object* left, Object* right) {
	if (left->type  == OBJECT_BOOLEAN &&
	    right->type == OBJECT_BOOLEAN) {
		char a = ((Object_boolean*) left->data)->value;
		char b = ((Object_boolean*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_number*)result->data)->value = a || b;

		return result;
	}

	return NULL;
}


static Object* evalute_xor(Object* left, Object* right) {
	if (left->type  == OBJECT_BOOLEAN &&
	    right->type == OBJECT_BOOLEAN) {
		char a = ((Object_boolean*) left->data)->value;
		char b = ((Object_boolean*)right->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_number*)result->data)->value = a ^ b;

		return result;
	}

	return NULL;
}


static Object* evalute_not(Object* arg) {
	if (arg->type  == OBJECT_BOOLEAN) {
		char a = ((Object_boolean*)arg->data)->value;

		Object* result = malloc(sizeof(Object));
		result->type = OBJECT_BOOLEAN;
		result->data = malloc(sizeof(Object_boolean));
		((Object_number*)result->data)->value = !a;

		return result;
	}

	return NULL;
}


static Object* evalute_dot(Object* left, Object* right) {
	if (left->type != OBJECT_SET || right->type != OBJECT_NAME)
		return NULL;

	Object_set* set = left->data;
	Object_name* name = right->data;

	for (int i = 0; i < set->count; i++) {
		if (strcmp(set->elems[i].name, name->name) == 0)
			return set->elems[i].value;
	}

	return NULL;
}


static Object* evalute_call(Object* left, Object* right) {
	if (left->type == OBJECT_FUNCTION) {
		Object_function* func = left->data;

		Scope* new_scope = malloc(sizeof(Scope));
		new_scope->parent = current_scope;
		current_scope = new_scope;

		current_scope->variables_count = 1;
		current_scope->variables = malloc(sizeof(Object_variable) * 1);
		current_scope->variables[0].name = func->argument_name;
		current_scope->variables[0].value = right;

		Object* result = evalute(func->body);

		current_scope = current_scope->parent;

		free(new_scope->variables);
		free(new_scope);

		return result;
	}
	if (left->type == OBJECT_FUNCTION_EXTERNAL) {
		Object_function_external* func = left->data;

		return (*func->body)(right);
	}

	return NULL;
}


static Object* evalute_op(Object_operation* op) {
	if (op->type == OP_EQ)
		return evalute_eq(evalute(op->args[1]),
		                  evalute(op->args[0]));
	if (op->type == OP_NEQ)
		return evalute_neq(evalute(op->args[1]),
		                   evalute(op->args[0]));
	if (op->type == OP_LESS)
		return evalute_less(evalute(op->args[1]),
		                    evalute(op->args[0]));
	if (op->type == OP_ELESS)
		return evalute_eless(evalute(op->args[1]),
		                    evalute(op->args[0]));
	if (op->type == OP_MORE)
		return evalute_more(evalute(op->args[1]),
		                    evalute(op->args[0]));
	if (op->type == OP_EMORE)
		return evalute_emore(evalute(op->args[1]),
		                    evalute(op->args[0]));
	if (op->type == OP_SUM)
		return evalute_sum(evalute(op->args[1]),
		                   evalute(op->args[0]));
	if (op->type == OP_SUB)
		return evalute_sub(evalute(op->args[1]),
		                   evalute(op->args[0]));
	if (op->type == OP_MUL)
		return evalute_mul(evalute(op->args[1]),
		                   evalute(op->args[0]));
	if (op->type == OP_DIV)
		return evalute_div(evalute(op->args[1]),
		                   evalute(op->args[0]));
	if (op->type == OP_XOR)
		return evalute_xor(evalute(op->args[1]),
		                   evalute(op->args[0]));
	if (op->type == OP_AND)
		return evalute_and(evalute(op->args[1]),
		                   evalute(op->args[0]));
	if (op->type == OP_OR)
		return evalute_or(evalute(op->args[1]),
		                   evalute(op->args[0]));
	if (op->type == OP_NOT)
		return evalute_not(evalute(op->args[0]));
	if (op->type == OP_DOT)
		return evalute_dot(evalute(op->args[1]),
		                   op->args[0]);
	if (op->type == OP_CALL)
		return evalute_call(evalute(op->args[1]),
		                    evalute(op->args[0]));
	return NULL;
}



Object* evalute_pointer(Object_name* pointer) {
	for (int i = 0; i < current_scope->variables_count; i++) {
		if (strcmp(pointer->name, current_scope->variables[i].name) == 0)
			return current_scope->variables[i].value;
	}

	printf("Variable %s not found!\n", pointer->name);

	return NULL;
}


Object* evalute(Object* root) {
	if (root->type == OBJECT_NUMBER ||
	    root->type == OBJECT_FLOAT_NUMBER ||
	    root->type == OBJECT_STRING ||
	    root->type == OBJECT_PATH) {
		return root;
	}

	if (root->type == OBJECT_OPERATION)
		return evalute_op(root->data);

	if (root->type == OBJECT_NAME)
		return evalute_pointer(root->data);
}


void print_object(Object* obj, int offset) {
	for (int i = 0; i < offset; i++)
		putc('\t', stdout);

	if (obj == NULL) {
		printf("NULL\n");
		return;
	}

	// printf("\n%d\n", obj->type);
	printf("%s ", object_type_names[obj->type]);

	if (obj->type == OBJECT_NUMBER) {
		printf("%li\n", ((Object_number*)obj->data)->value);
		return;
	}

	if (obj->type == OBJECT_FLOAT_NUMBER) {
		printf("%f\n", ((Object_float*)obj->data)->value);
		return;
	}

	if (obj->type == OBJECT_STRING) {
		printf("%s\n", ((Object_string*)obj->data)->value);
		return;
	}

	if (obj->type == OBJECT_PATH) {
		printf("%s\n", ((Object_path*)obj->data)->value);
		return;
	}

	if (obj->type == OBJECT_BOOLEAN) {
		printf("%d\n", ((Object_boolean*)obj->data)->value);
		return;
	}

	if (obj->type == OBJECT_OPERATION) {
		Object_operation* op = obj->data;

		printf("%s\n", operation_type_names[op->type]);

		for (int i = 0; i < op->count; i++)
			print_object(op->args[i], offset + 1);

		return;
	}

	if (obj->type == OBJECT_FUNCTION) {
		Object_function* func = obj->data;

		printf("%s\n", func->argument_name);
		print_object(func->body, offset + 1);

		return;
	}

	if (obj->type == OBJECT_NAME) {
		Object_name* pointer = obj->data;

		printf("%s\n", pointer->name);

		return;
	}

	if (obj->type == OBJECT_VARIABLE) {
		Object_variable* var = obj->data;

		printf("%s\n", var->name);
		print_object(var->value, offset + 1);

		return;
	}

	if (obj->type == OBJECT_ARRAY) {
		Object_array* arr = obj->data;
		printf("%d\n", arr->count);

		for (int i = 0; i < arr->count; i++)
			print_object(arr->elems[i], offset + 1);

		return;
	}
}
