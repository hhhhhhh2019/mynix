#include "lexer.h"
#include "libs.h"
#include "synt.h"
#include "object.h"
#include "evalute.h"
#include "token.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>


unsigned long index_node(Node*, unsigned long);
void node_to_dot(Node*);
void print_node(Node*, int);

void print_object(Object*, int);


int main() {
	char* data =
		// "1 + 2 * 3 / 4";
		// "({ foo, bar, ... }: foo + bar) { foo = 5; bar = 6; }";
		// "(x: x * x + 1) 2";
		// "stdlib.len(\"hello world!\")";
// "{ ... }: let version = \"unstable\"; in {"
// "  repos = {\n"
// "    os = {\n"
// "      url = \"github:mynix/repo_os\";\n"
// "      inherit version;\n"
// "    };\n"
// "    base = {\n"
// "      url = \"github:mynix/repo_base\";"
// "      inherit version;\n"
// "    };\n"
// "    desktop = {\n"
// "      url = \"github:mynix/repo_desktop\";\n"
// "      inherit version;\n"
// "    };\n"
// "  };\n"
// "\n"
// "  config = { base, desktop, ... }: {\n"
// "    network.hostname = \"qwertyuiop\";\n"
// "    system.timezone = \"Europe/Moscow\";\n"
// "    system.default_locale = \"ru_RU.UTF-8\";\n"
// "\n"
// "    users = {\n"
// "       alex = {\n"
// "         home = '/home/alex';\n"
// "         shell = \"zsh\";\n"
// "         modules = ['./users/alex/*'];"
// "       };\n"
// "    };\n"
// "  };\n"
// "}";
	"{ a, b, c }: { import { a, c } './some path/file'; }";

	Lexer_result lexer_result = lexer(data, "file");

	// for (int i = 0; i < lexer_result.tokens_count; i++) {
	// 	printf("%s %lu:%lu %s %s\n",
	// 		lexer_result.tokens[i].filename,
	// 		lexer_result.tokens[i].line,
	// 		lexer_result.tokens[i].column,
	// 		lexer_result.tokens[i].value,
	// 		token_type_names[lexer_result.tokens[i].type]);
	// }

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

	// index_node(syntax_result.root, 0);
	// printf("digraph 1 {\n");
	// node_to_dot(syntax_result.root);
	// printf("}");

	print_node(syntax_result.root, 0);
	
	// return 0;

	Malloc_info eval_malloc_info = {
		.start = NULL,
		.end = NULL,
	};
	malloc_info = &eval_malloc_info;

	Object* object = node_to_object(syntax_result.root);
	print_object(object, 0);

	wfree_all(lexer_result.malloc_info); // we already dont need this


	init_default_variables();
	init_stdlib();

	Object* eval = evalute(object);

	print_object(eval, 0);


	wfree_all(syntax_result.malloc_info);
	wfree_all(eval_malloc_info);
}


void print_node(Node* node, int offset) {
	for (int i = 0; i < offset; i++)
		putc('\t', stdout);

	if (node == NULL) {
		printf("NUL\n");
		return;
	}

	if (strlen(node->token.value) == 0)
		printf("%s ", token_type_names[node->token.type]);
	else
		printf("%s ", node->token.value);
	printf("%s\n", token_type_names[node->next_token]);

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

	if (obj->type == OBJECT_FUNCTION_SET) {
		Object_function_set* func = obj->data;
		printf("%d %d\n", func->args_count, func->allow_other);

		for (int i = 0; i < func->args_count; i++) {
			for (int i = 0; i < offset + 1; i++)
				putc('\t', stdout);
			printf("%s\n", func->args_names[i]);
		}

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

	if (obj->type == OBJECT_SET) {
		Object_set* set = obj->data;
		printf("%d\n", set->count);

		for (int i = 0; i < set->count; i++) {
			Set_arg arg = set->elems[i];
			for (int i = 0; i < offset + 1; i++)
				putc('\t', stdout);
			printf("%s\n", arg.name);
			print_object(arg.value, offset + 2);
		}

		return;
	}
}
