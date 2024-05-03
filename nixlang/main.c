#include "lexer.h"
#include "synt.h"
#include "utils.h"


int main() {
	char* data =
		"{\n"
		"  a.b = [\n"
		"    123,\n"
		"    \"qwerty\",\n"
		"    '/path/with space/file'\n"
		"  ];\n"

		"  c = {\n"
		"    a = foo.bar 1 + 2 * 3;\n"
		"    b = f (1 + 2) * 3;\n"
		"  };\n"
		"}\n";
	
	// char* data =
	// 	"{"
	// 	"a = [1,2,3];"
	// 	"b.c = 'path';"
	// 	"}"
	// 	"\n"
	// ;

	Lexer_result lexer_res = lexer(data, "file");

	// for (int i = 0; i < lexer_res.tokens_count; i++) {
	// 	LOG("%lu:%lu %s\n", 
	// 		lexer_res.tokens[i].line,
	// 		lexer_res.tokens[i].column,
	// 		token_type_names[lexer_res.tokens[i].type]);
	// }

	Node* root = synt(lexer_res);
	// print_node(root, 0);
	
	printf("digraph 1 {\n");
	index_node(root, 0);
	node_to_dot(root);
	printf("}\n");
}
