#include "lexer.h"
#include "synt.h"

#include <stdio.h>
#include <string.h>


unsigned long index_node(Node*, unsigned long);
void node_to_dot(Node*);
void print_node(Node*, int);


int main() {
	char* data =
		"1 + 2 +";

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

	wfree_all(lexer_result.malloc_info);
	wfree_all(syntax_result.malloc_info);
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
