#include "lexer.h"
#include "synt.h"
#include "evalute.h"
#include "utils.h"


int main() {
	// char* data =
	// 	"{\n"
	// 	"  a.b = [\n"
	// 	"    123,\n"
	// 	"    \"qwerty\",\n"
	// 	"    '/path/with space/file'\n"
	// 	"  ];\n"
	//
	// 	"  c = {\n"
	// 	"    a = foo.bar 1 + 2 * 3;\n"
	// 	"    b = f (1 + 2) * 3;\n"
	// 	"  };\n"
	// 	"}\n";
	
	// char* data =
	// 	"{"
	// 	"a = [1,2,3];"
	// 	"b.c = 'path';"
	// 	"}"
	// 	"\n"
	// ;
	

	// char* data =
	// 	"input: {"
	// 		"system.host = \"nixhost\";"
	// 		"system.arch = \"myarch\";"
	// 		"system.packages = ["
	// 			"input.pkgs.htop,"
	// 			"input.pkgs.git,"
	// 			"input.pkgs.zsh,"
	// 		"];"
	// 		"system.users = {"
	// 			"alex = {"
	// 				"groups = [ wheel users alex ];"
	// 				"home = '/home/alex';"
	// 				"shell = input.pkgs.zsh;"
	// 				"modules = ["
	// 					"'./users/alex/alex.nix'"
	// 				"];"
	// 			"};"
	// 		"};"
	// 	"}"
	// 	"\n";
	
	char* data = "-1 < 1\n";

	Lexer_result lexer_res = lexer(data, "file");

	// for (int i = 0; i < lexer_res.tokens_count; i++) {
	// 	LOG("%lu:%lu %s\n",
	// 		lexer_res.tokens[i].line,
	// 		lexer_res.tokens[i].column,
	// 		token_type_names[lexer_res.tokens[i].type]);
	// }

	Node* root = synt(lexer_res);
	// print_node(root, 0);
	
	// printf("digraph 1 {\n");
	// index_node(root, 0);
	// node_to_dot(root);
	// printf("}\n");
	
	Object* oroot = node_to_object(root);
	print_object(oroot, 0);

	init_def_vars();

	Object* eval = evalute(oroot);

	print_object(eval, 0);
}
