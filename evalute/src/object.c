#include "object.h"
#include "node.h"
#include "token.h"
#include "utils.h"
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
	[OBJECT_FUNCTION_SET] = "OBJECT_FUNCTION_SET",
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


static enum Op_type token_to_op[TOKEN_TYPES_COUNT] = {
	[PLUS]        = OP_SUM,
	[MINUS]       = OP_SUB,
	[STAR]        = OP_MUL,
	[SLASH]       = OP_DIV,
	[PIPE]        = OP_OR,
	[AMPERSAND]   = OP_AND,
	[CARET]       = OP_XOR,
	[ASSIGN]      = OP_ASSIGN,
	[EQUALS]      = OP_EQ,
	[NOT_EQUALS]  = OP_NEQ,
	[LESS]        = OP_LESS,
	[LESS_EQUALS] = OP_ELESS,
	[MORE]        = OP_MORE,
	[MORE_EQUALS] = OP_EMORE,
	[EXCLAMATION] = OP_NOT,
	[Call]        = OP_CALL,
};


static Object* operation(Node* node, enum Op_type type) {
	Object* result = wmalloc(sizeof(Object));

	result->type = OBJECT_OPERATION;
	result->data = wmalloc(sizeof(Object_operation));

	Object_operation* data = result->data;

	data->type = type;
	data->count = node->childs_count;
	data->args = wmalloc(sizeof(void*) * data->count);

	for (int i = 0; i < data->count; i++)
		data->args[data->count-i-1] = node_to_object(node->childs[i]);

	return result;
}


Object* node_to_object(Node* node) {
	if (node->token.type == PLUS ||
	    node->token.type == MINUS ||
	    node->token.type == STAR ||
	    node->token.type == SLASH ||
	    node->token.type == PIPE ||
	    node->token.type == AMPERSAND ||
	    node->token.type == CARET ||
	    node->token.type == ASSIGN ||
	    node->token.type == EQUALS ||
	    node->token.type == NOT_EQUALS ||
	    node->token.type == LESS ||
	    node->token.type == LESS_EQUALS ||
	    node->token.type == MORE ||
	    node->token.type == MORE_EQUALS ||
	    node->token.type == EXCLAMATION ||
	    node->token.type == Call) {
		return operation(node, token_to_op[node->token.type]);
	}

	Object* result = wmalloc(sizeof(Object));

	if (node->token.type == DEC_NUMBER) {
		result->type = OBJECT_NUMBER;
		result->data = wmalloc(sizeof(Object_number));

		Object_number* data = result->data;

		data->value = atol(node->token.value);
	}

	else if (node->token.type == FLOAT_NUMBER) {
		result->type = OBJECT_FLOAT_NUMBER;
		result->data = wmalloc(sizeof(Object_float));

		Object_float* data = result->data;

		data->value = atof(node->token.value);
	}

	else if (node->token.type == STRING) {
		result->type = OBJECT_STRING;
		result->data = wmalloc(sizeof(Object_string));

		Object_string* data = result->data;
		data->value = wmalloc(strlen(node->token.value) + 1);
		strcpy(data->value, node->token.value);
	}

	else if (node->token.type == PATH) {
		result->type = OBJECT_PATH;
		result->data = wmalloc(sizeof(Object_path));

		Object_path* data = result->data;
		data->value = wmalloc(strlen(node->token.value) + 1);
		strcpy(data->value, node->token.value);
	}

	else if (node->token.type == Func_decl) {
		if (node->childs[1]->token.type == UNDEFINED) {
			result->type = OBJECT_FUNCTION;
			result->data = wmalloc(sizeof(Object_function));

			Object_function* data = result->data;

			data->argument_name = wmalloc(strlen(node->childs[1]->token.value) + 1);
			strcpy(data->argument_name, node->childs[1]->token.value);
			data->body          = node_to_object(node->childs[0]);
		} else {
			result->type = OBJECT_FUNCTION_SET;
			result->data = wmalloc(sizeof(Object_function_set));

			Object_function_set* data = result->data;

			data->body = node_to_object(node->childs[0]);
			data->allow_other = 0;
			data->args_count = 0;
			data->args_names = wmalloc(0);

			Node* arg = node->childs[1];

			while (1) {
				if (arg->token.type == Fargs) {
					if (arg->childs[1]->token.type == EPSILON)
						data->allow_other = 1;
					else {
						data->args_names = wrealloc(data->args_names, sizeof(char*) * (++data->args_count));
						data->args_names[data->args_count - 1] = arg->childs[1]->token.value;
					}
				} else {
					if (arg->token.type == EPSILON)
						data->allow_other = 1;
					else {
						data->args_names = wrealloc(data->args_names, sizeof(char*) * (++data->args_count));
						data->args_names[data->args_count - 1] = arg->token.value;
					}
					break;
				}

				arg = arg->childs[0];
			}
		}
	}

	else if (node->token.type == Name) {
		if (node->childs_count != 1) {
			result->type = OBJECT_OPERATION;
			result->data = wmalloc(sizeof(Object_operation));

			Object_operation* data = result->data;

			data->type = OP_DOT;
			data->count = 2;
			data->args = wmalloc(sizeof(void*) * data->count);

			data->args[0] = node_to_object(node->childs[0]);
			data->args[1] = node_to_object(node->childs[2]);
		} else {
			result->type = OBJECT_NAME;
			result->data = wmalloc(sizeof(Object_name));

			Object_name* data = result->data;
			data->name = wmalloc(strlen(node->childs[0]->token.value) + 1);
			strcpy(data->name, node->childs[0]->token.value);
		}
	}

	else if (node->token.type == UNDEFINED) {
		result->type = OBJECT_NAME;
		result->data = wmalloc(sizeof(Object_name));

		Object_name* data = result->data;
		data->name = wmalloc(strlen(node->token.value) + 1);
		strcpy(data->name, node->token.value);
	}

	else if (node->token.type == Array) {
		result->type = OBJECT_ARRAY;
		result->data = wmalloc(sizeof(Object_array));

		Object_array* data = result->data;
		data->elems = wmalloc(0);
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

	else if (node->token.type == Set) {
		result->type = OBJECT_SET;
		result->data = wmalloc(sizeof(Object_set));

		Object_set* data = result->data;
		data->count = 0;
		data->elems = wmalloc(0);

		Node* arg = node->childs[0];

		while (1) {
			Stack(char*) name_stack = {
				.count = 0,
				.values = wmalloc(0)
			};

			if (arg->token.type == Sargs) {
				Object* value = node_to_object(arg->childs[1]->childs[0]);

				Node* nname = arg->childs[1]->childs[2];

				while (1) {
					stack_push(name_stack, nname->childs[0]->token.value);

					if (nname->childs_count == 1)
						break;

					nname = nname->childs[2];
				}

				Object_set* curr_set = data;

				for (int i = 0; i < name_stack.count - 1; i++) {
					char* name = stack_pop(name_stack);
					Object* val = set_get(curr_set, name);

					if (val != NULL) {
						if (val->type != OBJECT_SET) {
							printf("Error\n");
							return NULL;
						}

						curr_set = val->data;
						continue;
					}

					val = wmalloc(sizeof(Object));
					val->type = OBJECT_SET;
					val->data = wmalloc(sizeof(Object_set));

					Object_set* sval = val->data;
					sval->count = 0;
					sval->elems = wmalloc(0);

					set_set(curr_set, name, val);

					curr_set = sval;
				}

				set_set(curr_set, stack_pop(name_stack), value);
			} else {
				Object* value = node_to_object(arg->childs[0]);

				Node* nname = arg->childs[2];

				while (1) {
					stack_push(name_stack, nname->childs[0]->token.value);

					if (nname->childs_count == 1)
						break;

					nname = nname->childs[2];
				}

				Object_set* curr_set = data;

				for (int i = 0; i < name_stack.count - 1; i++) {
					char* name = stack_pop(name_stack);
					Object* val = set_get(curr_set, name);

					if (val != NULL) {
						curr_set = val->data;
						continue;
					}

					val = wmalloc(sizeof(Object));
					val->type = OBJECT_SET;
					val->data = wmalloc(sizeof(Object_set));

					Object_set* sval = val->data;
					sval->count = 0;
					sval->elems = wmalloc(0);

					set_set(curr_set, name, val);

					curr_set = sval;
				}

				set_set(curr_set, stack_pop(name_stack), value);

				break;
			}

			arg = arg->childs[0];
		}
	}

	return result;
}


Object* set_get(Object_set* set, char* name) {
	for (int i = 0; i < set->count; i++) {
		if (strcmp(set->elems[i].name, name) == 0)
			return set->elems[i].value;
	}

	return NULL;
}


void set_set(Object_set* set, char* name, Object* value) {
	for (int i = 0; i < set->count; i++) {
		if (strcmp(set->elems[i].name, name) == 0) {
			set->elems[i].value = value;
			return;
		}
	}

	set->elems = wrealloc(set->elems, sizeof(Set_arg) * (++set->count));
	set->elems[set->count - 1].name = name;
	set->elems[set->count - 1].value = value;
}
