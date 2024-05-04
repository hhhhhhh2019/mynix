#include "evalute.h"
#include "lexer.h"
#include "synt.h"

#include <stdio.h>
#include <stdlib.h>


char* object_type_names[] = {
	[OBJECT_NUMBER] = "OBJECT_NUMBER",
	[OBJECT_FLOAT_NUMBER] = "OBJECT_FLOAT_NUMBER",
	[OBJECT_STRING] = "OBJECT_STRING",
	[OBJECT_PATH] = "OBJECT_PATH",
	[OBJECT_ARRAY] = "OBJECT_ARRAY",
	[OBJECT_SET] = "OBJECT_SET",
	[OBJECT_FUNCTION] = "OBJECT_FUNCTION",
	[OBJECT_VARIABLE] = "OBJECT_VARIABLE",
	[OBJECT_OPERATION] = "OBJECT_OPERATION",
};

char* operation_type_names[] = {
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



Object* node_to_object(Node* node) {
	Object* object = malloc(sizeof(Object));
	
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

	return object;
};


Object* evalute(Object* root) {
	
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

	if (obj->type == OBJECT_OPERATION) {
		Object_operation* op = obj->data;

		printf("%s\n", operation_type_names[op->type]);

		for (int i = 0; i < op->count; i++)
			print_object(op->args[i], offset + 1);

		return;
	}
}
