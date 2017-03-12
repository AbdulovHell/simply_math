#include <iostream>

namespace Project {
	namespace Core {

		const int simple_nums[] = { 2,3,5,7,11,13,17,19,23 };	//size 9

		void reduce(int* u, int* d) {
			for (int i = 0; i < 9;i++)
				if (*u%simple_nums[i] == 0 && *d%simple_nums[i] == 0) {
					*u /= simple_nums[i];
					*d /= simple_nums[i];
					reduce(u, d);
					return;
				}
		}

		bool isInteger(double var) {
			int a = var;
			if (abs(var - a) < 0.000000001)
				return true;
			else
				return false;
		}

		int isntPeriodical(double num) {
			num = GET_FRAC(num);
			int z = 0;
			while (true) {
				
				if (z > 30) {
					z = 0;
					break;
				}
			}
		}

		enum class var_type {
			FRACTIONAL,	//дробное
			INTEGER_ROUND,	//целое с округлением
			INTEGER,	//целая часть
			TOWER
		};
	}
	namespace IO {

		using namespace std;
		using namespace Project::Core;

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

		string to_string(double var, var_type type, int decimals) {	//переменная, как представить, количество знаков после запятой(пока от 0 до 9)
			string sOut;
			char buf[25];
			int temp;
			char outFormat[] = "%.3f";
			memset(buf, 0, 25);

			switch (type) {
			case var_type::FRACTIONAL:
				outFormat[2] = 48 + decimals;
				sprintf(buf, outFormat, var);
				sOut = buf;
				break;
			case var_type::INTEGER_ROUND:
				outFormat[2] = 48;
				sprintf(buf, outFormat, var);
				sOut = buf;
				break;
			case var_type::INTEGER:
				temp = var;
				sprintf(buf, "%d", temp);
				sOut = buf;
				break;
			}
			return sOut;
		}

		char* to_char_string(double var, var_type type, int decimals) {	//переменная, как представить, количество знаков после запятой(пока от 0 до 9)
			char buf[25];
			int temp;
			char outFormat[] = "%.3f";
			double frac;
			memset(buf, 0, 25);

			switch (type) {
			case var_type::FRACTIONAL:
				outFormat[2] = 48 + decimals;
				sprintf(buf, outFormat, var);
				break;
			case var_type::INTEGER_ROUND:
				outFormat[2] = 48;
				sprintf(buf, outFormat, var);
				break;
			case var_type::INTEGER:
				temp = var;
				sprintf(buf, "%d", temp);
				break;
			case var_type::TOWER:
				temp = var;	//целая часть
				frac = GET_FRAC(var);

				break;
			}
			return buf;
		}
	}
}