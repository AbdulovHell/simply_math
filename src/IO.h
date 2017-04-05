#include <iostream>
#include <math.h>

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
			int a = (int)var;
			if (abs(var - a) < 0.000000001)
				return true;
			else
				return false;
		}

		int isntPeriodical(double num) {    //возвращает количество знаков после запятой, -1 если дробь условно бесконечно переодическая
			num = GET_FRAC(num);
			int z = 0, temp = 0;
			while (true) {
				num = num*10.0;
				temp = (int)num;
				if (temp) {
					z++;
					num = GET_FRAC(num);
				}
				else
					break;
				if (z > 30) {
					z = -1;
					break;
				}
			}
			return z;
		}

		enum class var_type : char {
			FRACTIONAL = 1,	//дробное
			INTEGER_ROUND = 2,	//целое с округлением
			INTEGER = 3,	//целая часть
			TOWER = 4
		};
	}
	namespace IO {

		using namespace std;
		using namespace Project::Core;

		wchar_t* err_str(int pos) {	//составляет строку с указателем до неизвестного символа
            wchar_t* str = new wchar_t[(pos + 1) * 2];
			for (int i = 0;i < pos;i++)
				str[i] = ' ';
			str[pos] = 0;
			str[pos - 1] = '^';
			return str;
		}

		wchar_t* wXchar(wchar_t* str)
		{
			unsigned int i, j;
			size_t z = wcslen(str);
			int cnt = 0;
			for (i = 0;i < z;i++)
			{
				if (str[i] == ' ')
				{
					cnt++;
					for (j = i;j < z - 1; ++j)
					{
						str[j] = str[j + 1];
					}
				}
			}
			str[z - cnt] = '\0';
			return str;
		}

		wchar_t* VerifyInput(wchar_t* input) {	//возвращает строку, описывающую ошибку, иначе NULL.
			//wchar_t* EndStr = &input[strlen(input)];
			//wchar_t* cursor = input;
#define BUF_SIZE 300
			int bracket = 0, ravno = 0;
			static wchar_t buf[BUF_SIZE];

			wXchar(input);

			size_t len = wcslen(input);

			for (unsigned int i = 0;i < len;i++) {
				if (input[i] == '(')
					bracket++;
				else if (input[i] == ')')
					bracket--;
				else if (input[i] == '=')
					ravno++;
				else if (input[i] == '@') {
					swprintf(buf, BUF_SIZE, L"\nBad symbol, '%c':\n", input[i]);
					wcscat(buf, input);
					wcscat(buf, L"\n");
					wcscat(buf, err_str(i));
					wcscat(buf, L"\n");
					return buf;
				}

				if (bracket < 0) {
					swprintf(buf, BUF_SIZE, L"\n ')' error.\n");
					return buf;
				}
			}
			if (bracket) {
				swprintf(buf, BUF_SIZE, L"\n ( and ) error.\n");
				return buf;
			}
			if (ravno > 1) {
				swprintf(buf, BUF_SIZE, L"\n'='>1 error.\n");
				return buf;
			}
			/*
			while (cursor < EndStr) {

			}*/
			return NULL;
		}
#define BUF_SIZE2 25
		wstring to_string(double var, var_type type, int decimals) {	//переменная, как представить, количество знаков после запятой(пока от 0 до 9)
			wstring sOut;
			wchar_t buf[BUF_SIZE2];
			int temp;
            double frac;
			wchar_t outFormat[] = L"%.3f";
			memset(buf, 0, 25 * 2);

			switch (type) {
			case var_type::FRACTIONAL:
				outFormat[2] = 48 + decimals;
				swprintf(buf, BUF_SIZE2, outFormat, var);
				sOut = buf;
				break;
			case var_type::INTEGER_ROUND:
				outFormat[2] = 48;
				swprintf(buf, BUF_SIZE2, outFormat, var);
				sOut = buf;
				break;
			case var_type::INTEGER:
				temp = (int)var;
				swprintf(buf, BUF_SIZE2, L"%d", temp);
				sOut = buf;
				break;
            case var_type::TOWER:
                temp = (int)var;	//целая часть
                frac = GET_FRAC(var);   //дробная
                int decs = isntPeriodical(frac);
                if (decs == -1) {
                    swprintf(buf, BUF_SIZE2, L"not implement");
                    break;
                }
                int u = (int)(frac*pow(10, decs));
                int d = (int)pow(10, decs);
                reduce(&u, &d);
                u = u + d*temp;
                swprintf(buf, BUF_SIZE2, L"%d\n", u);
                wchar_t tempbuf[20];
                memset(tempbuf, 0, 20 * 2);
                swprintf(tempbuf, 20, L"---\n");
                wcscat(buf, tempbuf);
                swprintf(tempbuf, 20, L"%d\n", d);
                wcscat(buf, tempbuf);
                sOut=buf;
                break;
			}
			return sOut;
		}

		wchar_t* to_char_string(double var, var_type type, int decimals) {	//переменная, как представить, количество знаков после запятой(пока от 0 до 9)
			static wchar_t buf[BUF_SIZE2];
			int temp;
			wchar_t outFormat[] = L"%.3f";
			double frac;
			memset(buf, 0, 25 * 2);

			switch (type) {
			case var_type::FRACTIONAL:
				outFormat[2] = 48 + decimals;
				swprintf(buf, BUF_SIZE2, outFormat, var);
				break;
			case var_type::INTEGER_ROUND:
				outFormat[2] = 48;
				swprintf(buf, BUF_SIZE2, outFormat, var);
				break;
			case var_type::INTEGER:
				temp = (int)var;
				swprintf(buf, BUF_SIZE2, L"%d", temp);
				break;
			case var_type::TOWER:
				temp = (int)var;	//целая часть
				frac = GET_FRAC(var);   //дробная
				int decs = isntPeriodical(frac);
				if (decs == -1) {
					swprintf(buf, BUF_SIZE2, L"not implement");
					break;
				}
				int u = (int)(frac*pow(10, decs));
				int d = (int)pow(10, decs);
				reduce(&u, &d);
				u = u + d*temp;
				swprintf(buf, BUF_SIZE2, L"%d\n", u);
				wchar_t tempbuf[20];
				memset(tempbuf, 0, 20 * 2);
				swprintf(tempbuf, 20, L"---\n");
				wcscat(buf, tempbuf);
				swprintf(tempbuf, 20, L"%d\n", d);
				wcscat(buf, tempbuf);
				break;
			}
			return buf;
		}
	}
}
