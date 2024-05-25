#include "evalute.h"
#include "object.h"
#include "utils.h"
#include <string.h>


// i dont know how make it simpler


Scope* current_scope = NULL;


Object* get_from_scope(Scope* scope, char* name) {
	if (scope == NULL)
		return NULL;

	for (int i = 0; i  < scope->variables_count; i++) {
		if (strcmp(scope->variables[i].name, name) == 0)
			return scope->variables[i].value;
	}

	return get_from_scope(scope->parent, name);
}


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


static Object* evalute_name(Object* name) {
	Object_name* data = name->data;

	return get_from_scope(current_scope, data->name);
}


static Object* evalute_op_dot(Object* left, Object* right) {
	char* name = ((Object_name*)right->data)->name;

	if (left->type == OBJECT_SET) {
		Object_set* set = left->data;

		for (int i = 0; i < set->count; i++) {
			if (strcmp(set->elems[i].name, name) == 0)
				return set->elems[i].value;
		}

		return NULL;
	}

	return NULL;
}


static Object* evalute_op_call(Object* func, Object* arg) {
	if (func->type == OBJECT_FUNCTION) {
		Object_function* data = func->data;

		Scope scope = {
			.parent = current_scope,
			.variables_count = 1,
			.variables = wmalloc(sizeof(Object_variable) * 1)
		};

		scope.variables[0].name = data->argument_name;
		scope.variables[0].value = arg;

		current_scope = &scope;

		Object* result = evalute(data->body);

		current_scope = current_scope->parent;

		wfree(scope.variables);

		return result;
	}

	else if (func->type == OBJECT_FUNCTION_EXTERNAL) {
		Object_function_external* data = func->data;

		return data->body(arg);
	}

	else if (func->type == OBJECT_FUNCTION_SET) {
		if (arg->type != OBJECT_SET)
			return NULL;

		Object_function_set* data = func->data;
		Object_set* args = arg->data;

		Scope scope = {
			.parent = current_scope,
			.variables_count = data->args_count,
			.variables = wmalloc(sizeof(Object_variable) * data->args_count)
		};

		for (int i = 0; i < args->count; i++) {
			char found = 0;

			for (int j = 0; j < data->args_count; j++) {
				if (strcmp(args->elems[i].name, data->args_names[j]) != 0)
					continue;

				found = 1;
				break;
			}

			if (found == 0) {
				if (data->allow_other == 1)
					continue;

				return NULL;
			}

			// scope.variables = wrealloc(scope.variables, sizeof(Object_variable) * (++scope.variables_count));
			scope.variables[i].name = args->elems[i].name;
			scope.variables[i].value = args->elems[i].value;
		}

		current_scope = &scope;

		Object* result = evalute(data->body);

		current_scope = current_scope->parent;

		wfree(scope.variables);

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
	// if (op->type == OP_EQ)
	// 	return evalute_op_eq(evalute(op->args[0]), evalute(op->args[1]));
	// if (op->type == OP_NEQ)
	// 	return evalute_op_neq(evalute(op->args[0]), evalute(op->args[1]));
	// if (op->type == OP_LESS)
	// 	return evalute_op_less(evalute(op->args[0]), evalute(op->args[1]));
	// if (op->type == OP_ELESS)
	// 	return evalute_op_eless(evalute(op->args[0]), evalute(op->args[1]));
	// if (op->type == OP_MORE)
	// 	return evalute_op_more(evalute(op->args[0]), evalute(op->args[1]));
	// if (op->type == OP_EMORE)
	// 	return evalute_op_emore(evalute(op->args[0]), evalute(op->args[1]));
	// if (op->type == OP_ASSIGN)
	// 	return evalute_op_assign(evalute(op->args[0]), evalute(op->args[1]));
	// if (op->type == OP_AND)
	// 	return evalute_op_and(evalute(op->args[0]), evalute(op->args[1]));
	// if (op->type == OP_OR)
	// 	return evalute_op_or(evalute(op->args[0]), evalute(op->args[1]));
	// if (op->type == OP_XOR)
	// 	return evalute_op_xor(evalute(op->args[0]), evalute(op->args[1]));
	// if (op->type == OP_NOT)
	// 	return evalute_op_not(evalute(op->args[0]));
	if (op->type == OP_DOT)
		return evalute_op_dot(evalute(op->args[1]), op->args[0]);
	if (op->type == OP_CALL)
		return evalute_op_call(evalute(op->args[0]), evalute(op->args[1]));

	return NULL;
}


Object* evalute(Object* object) {
	if (object->type == OBJECT_NUMBER ||
	    object->type == OBJECT_FLOAT_NUMBER ||
	    object->type == OBJECT_STRING ||
	    object->type == OBJECT_PATH ||
	    object->type == OBJECT_ARRAY ||
	    object->type == OBJECT_SET ||
	    object->type == OBJECT_FUNCTION ||
	    object->type == OBJECT_FUNCTION_EXTERNAL ||
	    object->type == OBJECT_FUNCTION_SET) {
		return object;
	}

	if (object->type == OBJECT_NAME)
		return evalute_name(object);

	if (object->type == OBJECT_OPERATION)
		return evalute_op(object);

	return NULL;
}
