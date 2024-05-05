#include "evalute.h"
#include "lexer.h"
#include "synt.h"

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
	[OBJECT_VARIABLE] = "OBJECT_VARIABLE",
	[OBJECT_VARIABLE_POINTER] = "OBJECT_VARIABLE_POINTER",
	[OBJECT_OPERATION] = "OBJECT_OPERATION",
};

char* operation_type_names[] = {
	[OP_EQ] = "OP_EQ",
	[OP_NEQ] = "OP_NEQ",
	[OP_LESS] = "OP_LESS",
	[OP_ELESS] = "OP_ELESS",
	[OP_MORE] = "OP_MORE",
	[OP_EMORE] = "OP_EMORE",
	[OP_SUM] = "OP_SUM",
	[OP_SUB] = "OP_SUB",
	[OP_MUL] = "OP_MUL",
	[OP_DIV] = "OP_DIV",
	[OP_AND] = "OP_AND",
	[OP_OR] = "OP_OR",
	[OP_XOR] = "OP_XOR",
	[OP_NOT] = "OP_NOT",
	[OP_DOT] = "OP_DOT",
};

Object** variables;
unsigned int variables_count;


void init_def_vars() {
	Object_variable* var_true      = malloc(sizeof(Object_variable));
	Object_boolean* var_true_value = malloc(sizeof(Object_boolean));
	var_true_value->value = 1;
	var_true->name = "true";
	var_true->value = malloc(sizeof(Object));
	var_true->value->type = OBJECT_BOOLEAN;
	var_true->value->data = var_true_value;

	Object_variable* var_false      = malloc(sizeof(Object_variable));
	Object_boolean* var_false_value = malloc(sizeof(Object_boolean));
	var_false_value->value = 0;
	var_false->name = "false";
	var_false->value = malloc(sizeof(Object));
	var_false->value->type = OBJECT_BOOLEAN;
	var_false->value->data = var_false_value;


	variables_count = 2;

	variables = malloc(sizeof(void*) * variables_count);

	variables[0] = malloc(sizeof(Object));
	variables[0]->type = OBJECT_VARIABLE;
	variables[0]->data = var_true;

	variables[1] = malloc(sizeof(Object));
	variables[1]->type = OBJECT_VARIABLE;
	variables[1]->data = var_false;
}


Object* node_to_object(Node* node) {
	Object* object = malloc(sizeof(Object));
	
	if (node->token.type == EQUALS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_EQ;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	if (node->token.type == NOT_EQUALS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_NEQ;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	if (node->token.type == LESS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_LESS;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	if (node->token.type == LESS_EQUALS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_ELESS;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	if (node->token.type == MORE) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_MORE;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	if (node->token.type == MORE_EQUALS) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_EMORE;
		data->count = node->childs_count;
		data->args = malloc(sizeof(void*) * data->count);

		for (int i = 0; i < data->count; i++)
			data->args[i] = node_to_object(node->childs[i]);
	}
	
	if (node->token.type == PLUS) {
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

	else if (node->token.type == DOT) {
		object->type = OBJECT_OPERATION;
		object->data = malloc(sizeof(Object_operation));

		Object_operation* data = object->data;

		data->type = OP_DOT;
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

	else if (node->token.type == Func_decl) {
		object->type = OBJECT_FUNCTION;
		object->data = malloc(sizeof(Object_function));

		Object_function* data = object->data;

		data->argument_name = node->childs[0]->token.value;
		data->body          = node_to_object(node->childs[1]);
	}

	else if (node->token.type == UNDEFINED) {
		object->type = OBJECT_VARIABLE_POINTER;
		object->data = malloc(sizeof(Object_var_pointer));

		Object_var_pointer* data = object->data;

		data->name = node->token.value;
	}

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


static Object* evalute_op(Object_operation* op) {
	if (op->type == OP_EQ)
		return evalute_eq(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_NEQ)
		return evalute_neq(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_LESS)
		return evalute_less(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_ELESS)
		return evalute_eless(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_MORE)
		return evalute_more(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_EMORE)
		return evalute_emore(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_SUM)
		return evalute_sum(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_SUB)
		return evalute_sub(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_MUL)
		return evalute_mul(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_DIV)
		return evalute_div(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_XOR)
		return evalute_xor(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_AND)
		return evalute_and(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_OR)
		return evalute_or(evalute(op->args[0]),
		                   evalute(op->args[1]));
	if (op->type == OP_NOT)
		return evalute_not(evalute(op->args[0]));
	return NULL;
}



Object* evalute_pointer(Object_var_pointer* pointer) {
	for (int i = 0; i < variables_count; i++) {
		Object_variable* var = variables[i]->data;

		if (strcmp(pointer->name, var->name) == 0)
			return var->value;
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

	if (root->type == OBJECT_VARIABLE_POINTER)
		return evalute_pointer(root->data);
}


void print_object(Object* obj, int offset) {
	for (int i = 0; i < offset; i++)
		putc('\t', stdout);

	if (obj == NULL) {
		printf("NULL\n");
		return;
	}

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

	if (obj->type == OBJECT_VARIABLE_POINTER) {
		Object_var_pointer* pointer = obj->data;

		printf("%s\n", pointer->name);

		return;
	}

	if (obj->type == OBJECT_VARIABLE) {
		Object_variable* var = obj->data;

		printf("%s\n", var->name);
		print_object(var->value, offset + 1);

		return;
	}
}
