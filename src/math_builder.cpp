#include "math_builder.h"

namespace Project {
	namespace Core {
		using namespace std;
		math_builder::math_builder()
		{
			object = nullptr;
		}

		math_builder::math_builder(wchar_t * strPtr, wchar_t * endPtr, data_list * _up)
		{
			object = new expression();
			data_reference = _up;
		}

		math_builder::~math_builder()
		{
		}
		math_obj * math_builder::build(wchar_t * strPtr, wchar_t * endPtr)
		{
			unsigned int count, comma, iter;
			int count_var;
			math_obj *high_pointer = NULL; //последняя записанная операция
			math_obj *low_pointer = NULL; //последняя записанная константа/переменная/ф-ция/выражение
			math_obj *temp_pointer = NULL;
			math_obj *multiple_var = NULL;
			wchar_t* temp/*, *temp_2*/;
			wstring temp_str, name_str;
			double buf;
			wchar_t* s_iter = strPtr;

			while (s_iter <= endPtr) {

				if (*s_iter >= '0'&& *s_iter <= '9' || *s_iter == '.') {
					// если данное число первое в записи выражения
					if ((high_pointer == NULL) && (low_pointer == NULL)) {
						//создание элемента класса и запись числа
						object->assing_pl(new number(wcstod(s_iter, &s_iter)/*wstr_to_double(s_iter, &s_iter)*/));
						//оба указателя -> на число, тебуется для проверки условия при записи операции
						low_pointer = object->get_pl();
						high_pointer = low_pointer;
					}
					// если данное число не первое в записи (была какая-либо операция)
					else {
						if (high_pointer->get_class_type() != flags::operation) {
							//err
							return nullptr;
						}
						//создание элемента класса и запись числа
						high_pointer->assing_pr(new number(wcstod(s_iter, &s_iter)/*wstr_to_double(s_iter, &s_iter)*/));
						low_pointer = high_pointer->get_pr();
					}
				}
				else if (*s_iter == '+') {
					high_pointer = new addition(high_pointer, low_pointer);					
					s_iter++;
					temp = wcspbrk(s_iter, L")+-*^/=,");
					if (s_iter == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}

			}
			












			return nullptr;
		}
	}
}


