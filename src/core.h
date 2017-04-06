///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include "error.h"
#include "IO.h"

/*сюда пишу возможные коды ошибок и ограничений ввода, которые будут выдаваться при заполнении дерева
доработать
-1 - запись вида f(y) - где y - определённая заранее функция. Вложенные/сложные функции не реализованы.
-2 - запись вида f(x)+2...=..., f+2...=... при неопределённой или определённой заранее f. можно рассматривать как неявное задание
функции или функциональное уравнение, но пока не хочется
-3 - запись вида f(2+3*x), f(2*5) - неявное вложение функции или использование константного выражение как аргумента
-4 - поскольку уравнения недоработаны, эта ошибка выскочит при попытке ввода уравнения
ошибки
1 - первым символом с строке стоит равно
2 - строка символов, не имеющая знака равно и (добаввить!) знака принадлежности// пока убрал из заполнения, эту проверку реализовать отдельно
	до вхождения в заполнение
3 - запись типа (... = - скобка открыта и не закрыта перед равно
4 - попытка получить результат выражения типа x(2), когда x неопределена
5 - попытка получить результат выражения типа x(2), когда x определена как переменная
6 - запись вида f(2xyz...) - переменные/константы не могут начинаться с цифр, если имелось ввиду выражение - ошибка ввода, нет знаков
	операций
7 - запись вида f) - в самом начале строки символы и закрывающая скобка без открывающей, возможно надо как-то проверить заранее
8 - запись вида х+2=(конец строки) при неопределённой или определённой как переменная х. Программа посчитает такую запись
	левой частью уравнения
9 - в начале строки или после скобки стоит знак операции
*/
namespace Project {
	namespace Core {

		using namespace std;
		using namespace Project::IO;

		class var_const {
		private:
			var_const *prioritize_processing(var_const *pc, int current_priority)
			{
				//если приоритет проверяемой операции !БОЛЬШЕ! текущей операции
				if (pc->get_priority() > current_priority)
				{
					//вызываем метод ещё раз для следующей операции
					return prioritize_processing(pc, current_priority);
				}
				//если приоритет проверяемой операции !МЕНЬШЕ! или равен приоритету текущей операции
				else
				{
					//вернуть указатель на проверенную операцию
					return pc;
				}
			}

			double processing(var_const *pointer)
			{
				const wchar_t* operation = pointer->var_id.c_str();
				switch (*operation) {
				case'+':
					return processing(pointer->point_left) + processing(pointer->point_right);
				case '*':
					return processing(pointer->point_left) * processing(pointer->point_right);
				case '/':
					return processing(pointer->point_left) / processing(pointer->point_right);
					/*case '4':
					return pow(recursion(pointer[0].point_left), recursion(pointer[0].point_right));
					*/
				default:       //для констант, чисел и переменных
					return pointer->var;
				}
			}

			wstring expression_processing(var_const *pointer)
			{
				if ((pointer->var_id == L"*") || (pointer->var_id == L"/"))
				{
					return expression_processing(pointer->point_left) + L" " + pointer->var_id + L" " + expression_processing(pointer->point_right);
				}
				else if (pointer->var_id == L"+")
				{
					if ((pointer->point_collar->var_id == L"*") || (pointer->point_collar->var_id == L"/"))
						return L"(" + expression_processing(pointer->point_left) + L" " + pointer->var_id + L" " + expression_processing(pointer->point_right) + L")";
					else
						return expression_processing(pointer->point_left) + L" " + pointer->var_id + L" " + expression_processing(pointer->point_right);
				}
				else if (pointer->var_id == L"0")
				{
					return to_string(pointer->var, var_type::FRACTIONAL, 2);
				}
				else
				{
					return pointer->read(L"name");
				}
			}

		public:
			var_const()
			{
				var_id = L"";
				var = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(wstring _name, double _num)
			{
				var_id = _name;
				var = _num;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(wstring _name, double _num, var_const *_pc)
			{
				var_id = _name;
				var = _num;
				point_left = NULL;
				point_right = NULL;
				point_collar = _pc;
			}

			var_const(wstring _name, double _num, var_const * _pl, var_const *_pr) {
				var_id = _name;
				var = _num;
				point_left = _pl;
				point_right = _pr;
				point_collar = NULL;
			}

			var_const(wstring _name, double _num, var_const * _pl, var_const *_pr, var_const *_pc) {
				var_id = _name;
				var = _num;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}

			var_const(var_const* var1)
			{
				var_id = var1->var_id;
				var = var1->var;
				point_left = var1->point_left;
				point_right = var1->point_right;
				point_collar = var1->point_collar;
			}

			int get_priority()
			{
				wchar_t operation = var_id[0];
				switch (operation)
				{
				case '+':
					return 1 + (int)var;
				case '*':
					return 2 + (int)var;
				case '/':
					return 3 + (int)var;
				default:
					return 0;
				}
			}

			wstring read(wstring arg)
			{
				wchar_t* id = wcsstr(&var_id[0], L"@");
				wchar_t* out = (wchar_t*)malloc(5 * sizeof(wchar_t));
				wchar_t* temp = NULL;
				if (id != NULL)
				{
					if (arg == L"type")
					{
						wcsncpy(out, var_id.c_str(), 5);
						out[5] = 0;
						return out;
					}
					else if (arg == L"name")
					{
						return id + 1;
					}
					else if (arg == L"func")
					{
						wcsncpy(out, var_id.c_str(), 5);
						out[5] = 0;
						if (out == L"funct")
						{
							free(out);
							temp = wcsstr(&var_id[0], L"(");
							out = (wchar_t*)malloc((temp - (id + 1)) * 2);
							wcsncpy(out, id + 1, temp - (id + 1));
							return out;
						}
						else //if ((out == "const")||(out == "varbl"))
						{
							return id + 1;
						}
					}
					else
					{
						return L"fuck up";
					}
				}
				else
				{
					return var_id;
				}
			}

			wstring expresion()
			{
				return read(L"name") + L" = " + expression_processing(point_left);
			}

			var_const *prioritize(int current_priority)
			{
				return prioritize_processing(point_collar, current_priority);
			}

			void arithmetic()
			{
				var = processing(point_left);
			}


			enum class variable_type {
				INTEGER,
				FRACTIONAL,
				IMAGINARY,
				REAL,
				RE_IM,
				EXP	//5E+10 5*10^10
			};

			wstring var_id;
			double var;
			//double var_im;  не зачем. разумнее пользоваться Эйлером
			int exp;
			var_const *point_left;		//левый рукав
			var_const *point_right;		//правый рукав
			var_const *point_collar;	//воротник
		};

		var_const pi = var_const(L"const@pi", 3.1415926535897932384626433832);
		var_const e = var_const(L"const@e", 2.7182818284590452353602874713527);

		vector<var_const*>* general_var_const;

		void Init() {
            Project::ProjectError::Init();
			general_var_const = new vector<var_const*>;
			general_var_const->push_back(&pi);
			general_var_const->push_back(&e);
			general_var_const->push_back(new var_const(L"funct@minus#unspec", 0));
		}

		var_const* run_vector(wstring _pv)
		{
			size_t temp_size_of_vect = general_var_const->size();
			int count;
			for (count = 0; count < temp_size_of_vect; count++)
			{
				if (_pv == general_var_const->at(count)->read(L"func"))
					return general_var_const->at(count);
			}
			if (count == temp_size_of_vect)
			{
				return NULL;
			}
		}

		/*var_const* analize_symbol(wchar_t* strPtr, wchar_t* endPtr, var_const* current_element)
		{

		}*/

		//рекурсия!!!
		var_const* filling_vector(wchar_t* strPtr, wchar_t*endPtr, var_const* current_element, int brakets)
		{
			size_t temp_size_of_vect;
			wchar_t* pDest = strPtr;
			unsigned int count;
			var_const *high_pointer = NULL;
			var_const *low_pointer = NULL;
			wstring p_var;
			wchar_t* temp;
			wchar_t* p_temp;
			wstring name;

			double num;
			int brakets_counter = 0;
			while (pDest <= endPtr) {

				if (*pDest == '=')
				{
					//если равно стоит в самом начале строки
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
						wcscpy(temp, L"error@");
						high_pointer = new var_const(temp, 1);
						general_var_const->pop_back();
						return high_pointer;
					}
					//равно в конце строки, запрос на действие
					else if (pDest == endPtr)
					{
						if (current_element->read(L"type") == L"const")
						{
							current_element->var_id += L"#read";
						}
						else if (current_element->read(L"type") == L"funct")
						{
							current_element->var_id += L"#read";
						}
						else if (current_element->read(L"type") == L"equat")
						{
							temp = NULL;
							temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
							wcscpy(temp, L"error@");
							high_pointer = NULL;
							high_pointer = new var_const(temp, 8);
							general_var_const->pop_back();
							return high_pointer;
						}
						else if (current_element->read(L"type") == L"exprs")
						{
							//не важно в принципе. если текущая строка всё ещё считается константным выражением его просто нужно посчтитать
						}
					}
					else
					{
						if (current_element->read(L"type") == L"const")
						{
							current_element->var_id += L"#writ";
							high_pointer = NULL;
							low_pointer = NULL;
						}
						else if (current_element->read(L"type") == L"funct")
						{
							current_element->var_id += L"#writ";
							high_pointer = NULL;
							low_pointer = NULL;
						}
						else if (current_element->read(L"type") == L"equat")
						{
							temp = NULL;
							temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
							wcscpy(temp, L"error@");
							high_pointer = NULL;
							high_pointer = new var_const(temp, -4);
							general_var_const->pop_back();
							return high_pointer;
						}
						else if (current_element->read(L"type") == L"exprs")
						{
							temp = NULL;
							temp = (wchar_t*)malloc(6 * sizeof(wchar_t));
							wcscpy(temp, L"error@");
							high_pointer = NULL;
							high_pointer = new var_const(temp, -4);
							general_var_const->pop_back();
							return high_pointer;
						}
						pDest++;
					}
				}
				else if (*pDest == '+')
				{
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						high_pointer = new var_const(L"error@", 9);
						general_var_const->pop_back();
						return high_pointer;
					}
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new var_const(L"+", brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + 1))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"+", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
							high_pointer = new var_const(L"+", brakets_counter + brakets, current_element->point_left, NULL, current_element);
							//воротник предыдущей легчайшей операции -> на новую операцию
							current_element->point_left->point_collar = high_pointer;
							//указываем левым рукавом константы на созданную операцию
							current_element->point_left = high_pointer;

						}
					}
					pDest++;
				}
				else if (*pDest == '*')
				{
					//если это первая операция в выражении
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						high_pointer = new var_const(L"error@", 9);
						general_var_const->pop_back();
						return high_pointer;
					}
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new var_const(L"*", brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + 2))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"*", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
							high_pointer = new var_const(L"*", brakets_counter + brakets, current_element->point_left, NULL, current_element);
							//воротник предыдущей легчайшей операции -> на новую операцию
							current_element->point_left->point_collar = high_pointer;
							//указываем левым рукавом константы на созданную операцию
							current_element->point_left = high_pointer;

						}
					}
					pDest++;
				}
				else if (*pDest == '/')
				{
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						high_pointer = new var_const(L"error@", 9);
						general_var_const->pop_back();
						return high_pointer;
					}
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new var_const(L"/", brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + 3))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"/", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
							high_pointer = new var_const(L"/", brakets_counter + brakets, current_element->point_left, NULL, current_element);
							//воротник предыдущей легчайшей операции -> на новую операцию
							current_element->point_left->point_collar = high_pointer;
							//указываем левым рукавом константы на созданную операцию
							current_element->point_left = high_pointer;

						}
					}
					pDest++;
				}
				else if (*pDest == '^')
				{
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						high_pointer = new var_const(L"error@", 9);
						general_var_const->pop_back();
						return high_pointer;
					}
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new var_const(L"^", brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + 4))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"^", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
							high_pointer = new var_const(L"^", brakets_counter + brakets, current_element->point_left, NULL, current_element);
							//воротник предыдущей легчайшей операции -> на новую операцию
							current_element->point_left->point_collar = high_pointer;
							//указываем левым рукавом константы на созданную операцию
							current_element->point_left = high_pointer;

						}
					}
					pDest++;
				}
				else if (*pDest == '-')
				{
					//операция минус понимается как сложение с отрицательным числом					
					//если выражение начинается с минуса (случай когда после минуса идёт число, минус перед скобкой не реализован)
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> константу
						current_element->point_left = new var_const(general_var_const->at(2));
						current_element->point_left->var_id.replace(current_element->point_left->var_id.find_first_of(L"#") + 1, 5, L"specf");

						//оба указателя -> на число, тебуется для проверки условия при записи операции
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					//если были другие числа/операции перед минусом
					else
					{
						//если это первая операция в выражении (сложение)
						if (high_pointer == low_pointer)
						{
							//сначала записываем операцию, левый рукав -> на предыдущее число, воротник на конст
							current_element->point_left = new var_const(L"+", brakets_counter, low_pointer, NULL, current_element);
							high_pointer = current_element->point_left;
							high_pointer->point_right = new var_const(general_var_const->at(2));
							high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L"#") + 1, 5, L"specf");

							low_pointer = high_pointer->point_right;
						}
						//если ранее были другие операции
						else
						{
							//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
							if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
							{
								//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
								high_pointer->point_right = new var_const(L"+", brakets_counter, low_pointer, NULL, high_pointer);
								high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
								high_pointer->point_right = new var_const(general_var_const->at(2));
								high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L"#") + 1, 5, L"specf");
								low_pointer = high_pointer->point_right;
							}

							else
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new var_const(L"+", brakets_counter, current_element->point_left, NULL, current_element);
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_element->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
								current_element->point_left = high_pointer;
								high_pointer->point_right = new var_const(general_var_const->at(2));
								high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L"#") + 1, 5, L"specf");
								low_pointer = high_pointer->point_right;
							}
						}
					}
					pDest++;
				}
				else if (*pDest == '(')
				{
					//левая скобка увеличивает счётчик скобок на число, равное самому высокому приоритету функции
					brakets_counter += 4;
					temp = pDest;
					count = 1;
					while (count != 0)
					{
						temp++;
						//если попали в конец строчки не найдя закрывающих скобок
						if (temp == endPtr)
						{
							high_pointer = new var_const(L"error@", 2);
							general_var_const->pop_back();
							return high_pointer;
						}
						else if (*temp == '(')
						{
							count++;
						}
						else if (*temp == ')')
						{
							count--;
						}
						else if (*temp == '=') //открытая скобка и дальше равно
						{
							high_pointer = new var_const(L"error@", 3);
							general_var_const->pop_back();
							return high_pointer;
						}
					}
					if (pDest + 1 == temp)
					{
						//пустая строка в скобках. вроде не ошибка, но можно и запретить)
					}
					else
					{
						low_pointer = new var_const(L"exprs@", 0, high_pointer);
						//на выходе - указатель на дерево, соответствующее выражению в скобках.
						low_pointer = filling_vector(pDest + 1, temp - 1, low_pointer, brakets + brakets_counter);
						if (low_pointer->read(L"type") == L"exprs")
						{

						}
						else if (low_pointer->read(L"type") == L"funct")
						{

						}
						else if (low_pointer->read(L"type") == L"const")
						{

						}
						else if (low_pointer->read(L"type") == L"varbl")
						{

						}

						if (high_pointer->read(L"name") == L"minus")
						{
							high_pointer->point_left = low_pointer;
							low_pointer = high_pointer;
						}
						else if (high_pointer->point_right->read(L"name") == L"minus")
						{
							high_pointer->point_right->point_right = low_pointer;
							low_pointer = high_pointer->point_right;
						}
						else
						{
							high_pointer->point_right = low_pointer;
						}
					}
					//сразу переходим к следующей позиции после скобки
					brakets_counter -= 4;
					pDest = temp + 1;
				}
				
				else if ((*pDest == '1') || (*pDest == '2') || (*pDest == '3') || (*pDest == '4') || (*pDest == '5') || (*pDest == '6') || (*pDest == '7') || (*pDest == '8') || (*pDest == '9') || (*pDest == '0'))
				{                   //любое число имеет id "0"					
					// если данное число первое в записи выражения
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> константу
						current_element->point_left = new var_const(L"0", wcstod(pDest, &pDest), current_element);
						//оба указателя -> на число, тебуется для проверки условия при записи операции
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					// если данное число не первое в записи (была какая-либо операция)
					else
					{
						//создание элемента класса и запись числа, воротник -> пред операцию 						
						if (low_pointer->read(L"name") == L"minus")
						{
							low_pointer->point_right = new var_const(L"0", wcstod(pDest, &pDest), low_pointer);
						}
						else
						{
							high_pointer->point_right = new var_const(L"0", wcstod(pDest, &pDest), high_pointer);
							low_pointer = high_pointer->point_right;
						}
					}
				}
				else
				{
					temp_size_of_vect = general_var_const->size();
					//если строчка начинается с имвола
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						temp = wcspbrk(pDest, L"()+-*^/=");
						if (temp == NULL)
						{
							high_pointer = new var_const(L"error@", 2);
							general_var_const->pop_back();
							return high_pointer;
						}
						else
						{
							name.copy(pDest, temp - pDest);
							high_pointer = run_vector(name);
						}
						if (high_pointer == NULL)
						{
							if (*temp = '=')
							{
								current_element->var_id.replace(0, 6, L"const@");
								current_element->var_id += name;
							}
							else if (*temp = '(')
							{
								current_element->var_id.replace(0, 6, L"funct@");
								current_element->var_id += name;
							}
							else if (*temp = ')')
							{
								high_pointer = new var_const(L"error@", 7);
								general_var_const->pop_back();
								return high_pointer;
							}
							else
							{
								current_element->var_id = L"equat@";
								current_element->var_id += name;
								current_element->var_id += L"#";
								name.insert(0, L"varbl@");
								name += L"#defnd";
								general_var_const->push_back(new var_const(name, 0));
								current_element->point_collar = new var_const(general_var_const->at(temp_size_of_vect));
								current_element->point_collar = current_element->point_left;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;								
							}

						}
						else
						{
							//если найдена переменная/конст/функция в массиве
						}
					}
					else
					{

					}
				}
			}
			return current_element;
		}

		wchar_t* analized_output(wchar_t* _pDest, wchar_t* _endPtr, var_const* _current_element)
		{
			_current_element = filling_vector(_pDest, _endPtr, _current_element);
			wstring expr;
			size_t output_size;
			if (_current_element->read(L"type") == L"const")
			{
				_current_element->arithmetic();
				expr = _current_element->expresion();
				output_size = wcslen(_current_element->var_id.c_str()) + wcslen(to_string(_current_element->var, var_type::FRACTIONAL, 2).c_str()) + 10 + wcslen(expr.c_str());
				wchar_t* output = (wchar_t*)malloc(output_size * 2);
				for (unsigned int i = 0; i < output_size; i++)
					output[i] = 0;
				wcscpy(output, expr.c_str());
				wcscat(output, L"\n");
				wcscat(output, _current_element->read(L"name").c_str());
				wcscat(output, L" = ");
				wcscat(output, to_string(_current_element->var, var_type::FRACTIONAL, 2).c_str());
				return output;

			}
			else if (_current_element->read(L"type") == L"funct")
			{
				expr = _current_element->expresion();
				output_size = wcslen(_current_element->var_id.c_str()) + wcslen(to_string(_current_element->var, var_type::FRACTIONAL, 2).c_str()) + 10 + wcslen(expr.c_str());
				wchar_t* output = (wchar_t*)malloc(output_size * 2);
				for (unsigned int i = 0; i < output_size; i++)
					output[i] = 0;
				wcscpy(output, expr.c_str());
				wcscat(output, L"\n");
				//strcat(output, _current_element->read("name").c_str());
				//strcat(output, " = ");
				//strcat(output, to_string(_current_element->var, var_type::FRACTIONAL,2).c_str());
				return output;
			}
		}

		wchar_t* input_to_analize(wchar_t* input)
		{
			wchar_t* error_str = Project::IO::VerifyInput(input);
			if (error_str != NULL)
				return error_str;

			int size_of_vect = general_var_const->size();

			vector<var_const*>* k = general_var_const;  //проверка для отладки
			wchar_t* temp = (wchar_t*)malloc(sizeof(wchar_t) * 6);
			wcscpy(temp, L"exprs@");
			general_var_const->push_back(new var_const(temp, 0));
			//free(temp);
			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer	
			return analized_output(point_start, point_end, general_var_const->at(size_of_vect));

			/*
			size_t input_size = wcslen(input);
			size_t size_of_vect;
			unsigned int count;
			int  brackets_left = 0;
			int brackets_right = 0;
			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer

			wchar_t* equal_right = wcsstr(input, L"="); // равно и справа от равно
			wchar_t* temp;
			wchar_t* equal_left = (wchar_t*)malloc((equal_right - input) * 2 + 2); //слева от равно
			memcpy(equal_left, input, (equal_right - input)*2);
			equal_left[(equal_right - input)] = 0;


			//если справа после равно ничего нет
			if (equal_right == point_end)
			{

			}
			//если справа что-то есть
			//если слева есть операции
			else if (wcspbrk(equal_left, L"+*^/") != NULL)
			{

			}
			//если слева есть минус
			else if (wcsstr(equal_left, L"-") != NULL)
			{

			}
			//нет операций, только скобки
			else if (wcsstr(equal_left, L"(") != NULL)
			{
				if (wcsstr(equal_left, L")") == NULL)
				{
													 //error
				}
				else
				{
					temp = equal_left;
					while (temp <= equal_right)
					{
						if (wcsstr(temp, L"(") != NULL)
						{
							brackets_left++;
							temp = wcsstr(temp, L"(") + 1;
						}
					}
					temp = equal_left;
					while(temp <= equal_right)
					{
						if (wcsstr(temp, L")") != NULL)
						{
							brackets_right++;
							temp = wcsstr(temp, L")") + 1;
						}
					}
					if ((brackets_left > 1)||(brackets_right > 1)||(brackets_right != brackets_left))
					{
						//error
					}
					//слева от равно нет операций, но есть одна правая и одна левая скобка и нет цифр
					else if (wcspbrk(equal_left, L"0123456789") == NULL)
					{
						//записываем переменную - всё, что стоит в скобках
						//необходимо будет добавить условие, проверяющее существует ли уже данная переменная в массиве
						//если существует - ничего не записывать - только указать новую функцию на неё
						size_of_vect = general_var_const->size();
						temp = (wchar_t*)malloc((wcsstr(temp, L")") - wcsstr(temp, L"(") + 7)*2);
						wcscpy(temp, L"varbl@");
						wcscat(temp, wcsstr(equal_left, L"(") + 1);
						temp[wcsstr(temp, L")") - temp] = 0;
						general_var_const->push_back(new var_const(temp, 0));
						free(temp);
						//записываем функцию, указываем правым рукавом  на созданную переменную
						size_of_vect = general_var_const->size();
						temp = (wchar_t*)malloc((equal_right - input + 7)*2);
						wcscpy(temp, L"funct@");
						wcscat(temp, equal_left);
						general_var_const->push_back(new var_const(temp, 0));

						//general_var_const->reserve(input_size * 2 + size_of_vect);
						free(temp);
						general_var_const->at(size_of_vect)->point_right = general_var_const->at(size_of_vect - 1);
						general_var_const->at(size_of_vect - 1)->point_collar = general_var_const->at(size_of_vect);
						point_start = equal_right + 1;

						return analized_output(point_start, point_end, general_var_const->at(size_of_vect));

					}
					//слева от равно нет операций, но есть одна правая и одна левая скобка и цифры
					else
					{
						//тут надо разобраться с последовательностью условий
						//эта ветка для вычисления значения функции в точке (запись вида f(3.5)), когда функция f уже определена
						//по идее нужно это проверять только когда после равно ничего нет, т.е. просто считать и выводить результат
						//но это может быть присвоение, например f(3.5) = t (впрочем можно запретить такой синтаксис)
					}
				}
			}
			else if (wcspbrk(equal_left, L"0123456789") != NULL)
			{

			}
			//слева от равно стоят только буквы
			else
			{
				size_of_vect = general_var_const->size();
				for (count = 0; count < size_of_vect; count++)
				{
					if (equal_left == general_var_const->at(count)->read(L"func"))
					{
						if (general_var_const->at(count)->read(L"type") == L"const")
						{
							point_start = equal_right + 1;
							return analized_output(point_start, point_end, general_var_const->at(count));
						}
						else if (general_var_const->at(count)->read(L"type") == L"varbl")
						{
							temp = (wchar_t*)malloc((equal_right - input + 7)*2);
							wcscpy(temp, L"const@");
							wcscat(temp, equal_left);
							general_var_const->at(count)->var_id = temp;
							point_start = equal_right + 1;
							free(temp);
							return analized_output(point_start, point_end, general_var_const->at(count));
						}
						else if (general_var_const->at(count)->read(L"type") == L"funct")
						{
							point_start = equal_right + 1;
							return analized_output(point_start, point_end, general_var_const->at(count));
						}
						else
						{
														return (wchar_t*)"fuck up";//кто знает что ещё тут может быть
						}
					}
				}
				if (count == size_of_vect)
				{
					temp = (wchar_t*)malloc((equal_right - input + 7)*2);
					wcscpy(temp, L"const@");
					wcscat(temp, equal_left);
					general_var_const->push_back(new var_const(temp, 0));
					//general_var_const->reserve(input_size * 2 + size_of_vect);
					free(temp);
					point_start = equal_right + 1;
					return analized_output(point_start, point_end, general_var_const->at(size_of_vect));
				}
			}
			*/
		}
	}
}
