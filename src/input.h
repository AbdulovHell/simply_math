#include <iostream>

namespace Project {
	namespace Input {

		using namespace std;

		char* err_str(int pos) {	//составляет строку с указателем до неизвестного символа
			char* str = (char*)malloc(pos + 1);
			for (int i = 0;i < pos;i++)
				str[i] = ' ';
			str[pos] = 0;
			str[pos - 1] = '^';
			return str;
		}

		char* VerifyInput(char* input) {	//возвращает строку, описывающую ошибку, иначе NULL.
			//char* EndStr = &input[strlen(input)];
			//char* cursor = input;
			int len = strlen(input);
			int left_bracket = 0, right_bracket = 0, ravno = 0;
			char buf[300];

			for (int i = 0;i < len;i++)
				if (input[i] == '(')
					left_bracket++;
				else if (input[i] == ')')
					right_bracket++;
				else if (input[i] == '=')
					ravno++;
				else if ((input[i] >= 0x2A && input[i] <= 0x39) || (input[i] >= 0x41 && input[i] <= 0x5A) || (input[i] >= 0x61 && input[i] <= 0x7A)) {}
				else {
					sprintf(buf, "\nBad symbol, '%c':\n", input[i]);
					strcat(buf, input);
					strcat(buf, "\n");
					strcat(buf, err_str(i));
					strcat(buf, "\n");
					return buf;
				}
				if (left_bracket != right_bracket) {
					sprintf(buf, "\n ( and ) error.\n");
					return buf;
				}
				if (ravno > 1) {
					sprintf(buf, "\n'='>1 error.\n");
					return buf;
				}
				/*
				while (cursor < EndStr) {

				}*/
				return NULL;
		}
	}
}