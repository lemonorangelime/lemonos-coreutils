#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int lexer_measure(char * line) {
	int i = 0;
	char chr = *line;
	while (chr) {
		chr = *line++;
		switch (chr) {
			default:
				break;
			case '\0':
			case ' ':
				i++;
				break;
		}
	}
	return i;
}

char ** lexer_parse(char * line, int * array_size) {
	char * item = line;
	int size = 0;

	*array_size = lexer_measure(line);
	char ** array = malloc(*array_size * 4);
	char chr = *line;
	int i = 0;
	while (chr) {
		chr = *line;
		switch (chr) {
			default:
				line++;
				break;
			case '\0':
			case ' ':
				size = line++ - item;

				char * string = malloc(size + 1);
				memcpy(string, item, size);
				string[size] = '\0';
				array[i++] = string;

				item = line;
				break;
		}
	}
	return array;
}
