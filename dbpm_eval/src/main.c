#include <getopt.h>
#include <bits/getopt_core.h>
#include <bits/getopt_ext.h>

#include <stdio.h>


void print_help(char*);


static struct option longopts[] = {
	{"preproc", no_argument, 0, 0},
	{"help", no_argument, 0, 0},
};


int main(int argc, char** argv) {
	int c;
	int longind;

	while ((c = getopt_long(argc, argv, "-ph", longopts, &longind)) != -1) {
		if (c == 0) {
			printf("long option: %s\n", longopts[longind].name);

			if (longind == 1) { // help
				print_help(argv[0]);
				return 0;
			}
		}
		
		else if (c == 1) {
			printf("regular argument: %s\n", optarg);
		}

		else if (c == 'h') {
			print_help(argv[0]);
			return 0;
		}

		else {
			printf("short option: %c\n", c);
		}
	}
}


void print_help(char* name) {
	printf("Usage: cat <file> | %s -p|command\n", name);
	printf("\nParameters:\n"
		"	-h  --help     print program usage\n"
		"	-p  --preproc  preprocess config file\n"
	);

	printf("\nExamples:\n"
		"	echo '{a.b = 2; a.c = 3;}' | %s -p\n"
		"		result: \"{a = {b = 2; c = 3;}}\n"
		"	echo '{a = {import \"some file\"}; b = 3;}' | %s -p\n"
		"		result: {a = <data from some file>; b = 3}\n",
		name, name
	);
}
