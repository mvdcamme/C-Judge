#include <stdio.h>

int do_calculation(char operator, int number1, int number2) {
	int result;
	switch(operator) {
	case '+':
		result = number1 + number2;
		break;
	case '-':
		result = number1 - number2;
		break;
	case 'X':
		result = number1 * number2;
		break;
	case '/':
		result = number1 / number2;
		break;
	}
	return result;
}

int main(int argc, char *argv[]) {
	if (argc == 4) {
		int number1 = atoi(argv[1]);
		char operator = argv[2][0];
		int number2 = atoi(argv[3]);
		int result = do_calculation(operator, number1, number2);
//			printf("Result is %i\n", result);
		return result;
	}
}
