#include "evalute.h"
#include "object.h"
#include "utils.h"
#include <string.h>


// i dont know how make it simpler


Scope* current_scope = NULL;


void init_default_variables() {
	current_scope = wmalloc(sizeof(Scope));
	current_scope->parent = NULL;
	current_scope->variables_count = 2;
	current_scope->variables = wmalloc(
	    current_scope->variables_count * sizeof(Object_variable));


	// true

	Object_boolean* val_true = wmalloc(sizeof(Object_boolean));
	val_true->value = 1;

	Object* object_true = wmalloc(sizeof(Object));
	object_true->type = OBJECT_BOOLEAN;
	object_true->data = val_true;


	// false

	Object_boolean* val_false = wmalloc(sizeof(Object_boolean));
	val_false->value = 0;

	Object* object_false = wmalloc(sizeof(Object));
	object_false->type = OBJECT_BOOLEAN;
	object_false->data = val_false;


	current_scope->variables[0].name = "true";
	current_scope->variables[0].value = object_true;

	current_scope->variables[1].name = "false";
	current_scope->variables[1].value = object_false;
}



static Object* evalute_op_sum(Object* left, Object* right) {
	if (left == NULL || right == NULL)
		return NULL;

	if (left->type == OBJECT_NUMBER && right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_NUMBER;
		result->data = wmalloc(sizeof(Object_number));
		((Object_number*)result->data)->value = a + b;

		return result;
	}

	if (left->type == OBJECT_FLOAT_NUMBER && right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a + b;

		return result;
	}

	if (left->type == OBJECT_NUMBER && right->type == OBJECT_FLOAT_NUMBER) {
		long   a =((Object_number*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));
		((Object_float*)result->data)->value = (double)a + b;

		return result;
	}

	if (left->type == OBJECT_FLOAT_NUMBER && right->type == OBJECT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		long   b =((Object_number*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a + (double)b;

		return result;
	}

	if (left->type == OBJECT_STRING && right->type == OBJECT_STRING) {
		char* a = ((Object_string*) left->data)->value;
		char* b = ((Object_string*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_STRING;
		result->data = wmalloc(sizeof(Object_string));
		((Object_string*)result->data)->value =
		    wmalloc(strlen(a) + strlen(b) + 1);
		strcpy(((Object_string*)result->data)->value, a);
		strcat(((Object_string*)result->data)->value, b);

		return result;
	}

	if (left->type == OBJECT_PATH && right->type == OBJECT_STRING) {
		char* a = ((Object_path*)   left->data)->value;
		char* b = ((Object_string*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_PATH;
		result->data = wmalloc(sizeof(Object_path));
		((Object_path*)result->data)->value =
		    wmalloc(strlen(a) + strlen(b) + 1);
		strcpy(((Object_path*)result->data)->value, a);
		strcat(((Object_path*)result->data)->value, b);

		return result;
	}

	if (left->type == OBJECT_STRING && right->type == OBJECT_PATH) {
		char* a = ((Object_string*)left->data)->value;
		char* b = ((Object_path*) right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_PATH;
		result->data = wmalloc(sizeof(Object_path));
		((Object_path*)result->data)->value =
		    wmalloc(strlen(a) + strlen(b) + 1);
		strcpy(((Object_path*)result->data)->value, a);
		strcat(((Object_path*)result->data)->value, b);

		return result;
	}

	if (left->type == OBJECT_PATH && right->type == OBJECT_PATH) {
		char* a = ((Object_path*) left->data)->value;
		char* b = ((Object_path*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_PATH;
		result->data = wmalloc(sizeof(Object_path));
		((Object_path*)result->data)->value =
		    wmalloc(strlen(a) + strlen(b) + 1);
		strcpy(((Object_path*)result->data)->value, a);
		strcat(((Object_path*)result->data)->value, b);

		return result;
	}

	return NULL;
}


static Object* evalute_op_sub(Object* left, Object* right) {
	if (left == NULL || right == NULL)
		return NULL;

	if (left->type == OBJECT_NUMBER && right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_NUMBER;
		result->data = wmalloc(sizeof(Object_number));
		((Object_number*)result->data)->value = a - b;

		return result;
	}

	if (left->type == OBJECT_FLOAT_NUMBER && right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a - b;

		return result;
	}

	if (left->type == OBJECT_NUMBER && right->type == OBJECT_FLOAT_NUMBER) {
		long   a =((Object_number*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));
		((Object_float*)result->data)->value = (double)a - b;

		return result;
	}

	if (left->type == OBJECT_FLOAT_NUMBER && right->type == OBJECT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		long   b =((Object_number*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a - (double)b;

		return result;
	}

	return NULL;
}


static Object* evalute_op_mul(Object* left, Object* right) {
	if (left == NULL || right == NULL)
		return NULL;

	if (left->type == OBJECT_NUMBER && right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_NUMBER;
		result->data = wmalloc(sizeof(Object_number));
		((Object_number*)result->data)->value = a * b;

		return result;
	}

	if (left->type == OBJECT_FLOAT_NUMBER && right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a * b;

		return result;
	}

	if (left->type == OBJECT_NUMBER && right->type == OBJECT_FLOAT_NUMBER) {
		long   a =((Object_number*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_NUMBER;
		result->data = wmalloc(sizeof(Object_number));
		((Object_number*)result->data)->value = (long)(a * b);

		return result;
	}

	if (left->type == OBJECT_FLOAT_NUMBER && right->type == OBJECT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		long   b =((Object_number*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a * (double)b;

		return result;
	}

	return NULL;
}


static Object* evalute_op_div(Object* left, Object* right) {
	if (left == NULL || right == NULL)
		return NULL;

	if (left->type == OBJECT_NUMBER && right->type == OBJECT_NUMBER) {
		long a = ((Object_number*) left->data)->value;
		long b = ((Object_number*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_NUMBER;
		result->data = wmalloc(sizeof(Object_number));
		((Object_number*)result->data)->value = a / b;

		return result;
	}

	if (left->type == OBJECT_FLOAT_NUMBER && right->type == OBJECT_FLOAT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a / b;

		return result;
	}

	if (left->type == OBJECT_NUMBER && right->type == OBJECT_FLOAT_NUMBER) {
		long   a =((Object_number*) left->data)->value;
		double b = ((Object_float*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_NUMBER;
		result->data = wmalloc(sizeof(Object_number));
		((Object_number*)result->data)->value = (long)(a / b);

		return result;
	}

	if (left->type == OBJECT_FLOAT_NUMBER && right->type == OBJECT_NUMBER) {
		double a = ((Object_float*) left->data)->value;
		long   b =((Object_number*)right->data)->value;

		Object* result = wmalloc(sizeof(Object));
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));
		((Object_float*)result->data)->value = a / (double)b;

		return result;
	}

	return NULL;
}


static Object* evalute_op(Object* object) {
	Object_operation* op = object->data;

	if (op->type == OP_SUM)
		return evalute_op_sum(evalute(op->args[0]), evalute(op->args[1]));
	if (op->type == OP_SUB)
		return evalute_op_sub(evalute(op->args[0]), evalute(op->args[1]));
	if (op->type == OP_MUL)
		return evalute_op_mul(evalute(op->args[0]), evalute(op->args[1]));
	if (op->type == OP_DIV)
		return evalute_op_div(evalute(op->args[0]), evalute(op->args[1]));

	return NULL;
}


Object* evalute(Object* object) {
	if (object->type == OBJECT_NUMBER ||
	    object->type == OBJECT_FLOAT_NUMBER ||
	    object->type == OBJECT_STRING ||
	    object->type == OBJECT_PATH) {
		return object;
	}

	if (object->type == OBJECT_OPERATION)
		return evalute_op(object);
}
