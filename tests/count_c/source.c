#include <stdio.h>

int count_c(char *path) {
	FILE *file = fopen(path, "r");
	if (file == NULL) {
		printf("File %s could not be opened\n", path);
		return 0;
	}
	char c;
	int counter = 0;
	while ((c = fgetc(file)) != EOF) {
		if (c == 'c') {
			++counter;
		}
	}
	fclose(file);
	return counter;
}
