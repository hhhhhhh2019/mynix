#include "lexer.h"
#include "object.h"
#include "synt.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>


unsigned long index_node(Node*, unsigned long);
void node_to_dot(Node*);
void print_node(Node*, int);

void print_object(Object*, int);


int main() {
	char* data =
		"a.b.c = foo.bar(3 + 4) * 5";

	Lexer_result lexer_result = lexer(data, "file");

	for (int i = 0; i < lexer_result.tokens_count; i++) {
		printf("%s %lu:%lu %s %s\n",
			lexer_result.tokens[i].filename,
			lexer_result.tokens[i].line,
			lexer_result.tokens[i].column,
			lexer_result.tokens[i].value,
			token_type_names[lexer_result.tokens[i].type]);
	}

	if (lexer_result.ok == 0) {
		wfree_all(lexer_result.malloc_info);
		return 1;
	}


	Synt_result syntax_result = synt(lexer_result);

	if (syntax_result.ok == 0) {
		wfree_all(lexer_result.malloc_info);
		wfree_all(syntax_result.malloc_info);
		return 1;
	}

	print_node(syntax_result.root, 0);

	Malloc_info eval_malloc_info = {
		.start = NULL,
		.end = NULL,
	};
	malloc_info = &eval_malloc_info;
	Object* object = node_to_object(syntax_result.root);
	
	print_object(object, 0);

	wfree_all(lexer_result.malloc_info);
	wfree_all(syntax_result.malloc_info);
	wfree_all(eval_malloc_info);
}


void print_node(Node* node, int offset) {
	for (int i = 0; i < offset; i++)
		putc('\t', stdout);

	if (node == NULL) {
		printf("NULL\n");
		return;
	}

	if (strlen(node->token.value) == 0)
		printf("%s\n", token_type_names[node->token.type]);
	else
		printf("%s\n", node->token.value);

	// if (token_type_names[node->next_token] != NULL)
	// 	printf("%s\n", token_type_names[node->next_token]);
	// else
	// 	printf("\n");

	for (int i = 0; i < node->childs_count; i++)
		print_node(node->childs[i], offset + 1);
}


void node_to_dot(Node* node) {
	if (strlen(node->token.value) == 0)
		printf("%lu [label=\"%s\"]\n", node->id, token_type_names[node->token.type]);
	else
		printf("%lu [label=\"%s\n%s\"]\n", node->id, token_type_names[node->token.type], node->token.value);

	printf("%lu -> {", node->id);

	for (int i = 0; i < node->childs_count; i++) {
		printf("%lu ", node->childs[i]->id);
	}

	printf("}\n");

	for (int i = 0; i < node->childs_count; i++)
		node_to_dot(node->childs[i]);
}


unsigned long index_node(Node* node, unsigned long id) {
	node->id = id++;

	for (int i = 0; i < node->childs_count; i++)
		id = index_node(node->childs[i], id);

	return id;
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
