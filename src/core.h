///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include "IO.h"


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
				//TODO: добавить возможность вычисления функций
				const wchar_t* operation = pointer->var_id.c_str();
				switch (*operation) {
				case'+':
					return processing(pointer->point_left) + processing(pointer->point_right);
				case '*':
					return processing(pointer->point_left) * processing(pointer->point_right);
				case '/':
					return processing(pointer->point_left) / processing(pointer->point_right);
				case '^':
					return pow(processing(pointer->point_left), processing(pointer->point_right));

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

			/*Метод возвращает приоритет операции.
			1 - сложение
			2 - умножение
			3 - деление
			4 - возведение в степень
			0 - не операция*/
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
				case '^':
					return 4 + (int)var;
				default:
					return 0;
				}
			}


			/*метод read возвращает строку, содержащую информацию об элементе класса в соответствии с запрошенным аргументом:
			type - тип элемента класса, всё что записано в строке var_id до символа @
			prop - свойство элемента класса, всё что записано после #
			name - имя элемента класса, подстроку в var_id между @ и #, для функций и уравнений - между @ и (
			func - имя элемента класса, подстроку в var_id между @ и #, без исключений для функций
			nvar - имя переменной, на которую указывает элемент класса. Для функций и уравнений (пока без проверки указателя)
			*/
			wstring read(wstring arg)
			{
				wchar_t* id_a = wcsstr(&var_id[0], L"@");
				wchar_t* id_H = wcsstr(&var_id[0], L"#");
				wchar_t* id_f = wcsstr(&var_id[0], L"(");
				wchar_t* id_b = wcsstr(&var_id[0], L")");
				wstring out;

				if (arg == L"type")
				{
					if (id_a != NULL)
					{
						out.assign(var_id, 0, 5);
					}
					else
					{
						if (var_id == L"0")
						{
							out = L"numbr";
						}
						else
						{
							out = L"opert";
						}
					}
				}
				else if (arg == L"prop")
				{
					if (id_H != NULL)
					{
						out.assign(id_H + 1);
					}
					else
					{
						out = L"0";
					}
				}
				else if (arg == L"name")
				{
					if (id_a != NULL)
					{
						if (id_f != NULL)
						{
							if (id_a + 1 == id_f)
							{
								out = L"";
							}
							else
								out.assign(id_a + 1, id_f - 1);
						}
						else
						{
							if (id_a + 1 == id_H)
							{
								out = L"";
							}
							else
								out.assign(id_a + 1, id_H - 1);
						}
					}
					else
					{
						out = var_id;
					}
				}
				else if (arg == L"func")
				{
					if (id_a != NULL)
						if (id_H != NULL)
							if (id_a + 1 == id_H)
								out = L"";
							else
								out.assign(id_a + 1, id_H - 1);
						else
							out.assign(id_a + 1);
					else
						out = var_id;
				}
				else if (arg == L"nvar")
				{
					if ((point_collar != NULL) && (point_collar->read(L"type") == L"varbl"))
						out = point_collar->read(L"name");
					else
						out = L"";
					/* TODO: сделать проверку соответствия имени переменной на которую указывает элемент и имени переменной в названии функции.
					Если не совпадают, заменять имя в названии Н-р: f(x) -> f(y)
					if ((id_f != NULL) && (id_b != NULL))
					{
						out.assign(id_f + 1, id_b - 1);
					}
					else
						out = L"";
						*/
				}
				else
				{
					out = L"fuck up";
				}
				return out;
			}

			/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и коструирующую строку с формальной записью текущего выражения.
			Возвращает строку. ПОКА НЕ РАБОТАЕТ*/
			wstring expresion()
			{
				//TODO: переделать!
				return read(L"name") + L" = " + expression_processing(point_left);
			}

			/*Метод вызывает рекурсивную функцию проверки приоритета операций в текущем дереве операций.
			Возвращает указатель на элемент дерева (операцию), имеющий приоритет меньший или равный в сравнении с текущим элементом. */
			var_const *prioritize(int current_priority)
			{
				return prioritize_processing(point_collar, current_priority);
			}

			/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и выполняющую их.
			Результатом работы метода является запись результата вычислений в double var текущего элемента класса. */
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
			general_var_const = new vector<var_const*>;
			general_var_const->push_back(&pi);
			general_var_const->push_back(&e);
			general_var_const->push_back(new var_const(L"funct@minus#empty", 0));
		}

		var_const* run_vector(wstring _pv)
		{
			size_t temp_size_of_vect = general_var_const->size();
			int count;
			for (count = 0; count < temp_size_of_vect; count++)
			{
				if (_pv == general_var_const->at(count)->read(L"name"))
					return general_var_const->at(count);
			}
			if (count == temp_size_of_vect)
			{
				return NULL;
			}
		}


		var_const* filling_vector(wchar_t* strPtr, wchar_t*endPtr, var_const* current_element, int brakets)
		{
			size_t temp_size_of_vect;
			wchar_t* pDest = strPtr;
			unsigned int count;
			var_const *high_pointer = NULL;
			var_const *low_pointer = NULL;
			var_const *temp_pointer = NULL;
			wchar_t* temp;
			wstring name;

			double num;
			int brakets_counter = 0;
			while (pDest <= endPtr) {

				if (*pDest == '=')
				{
					//если равно стоит в самом начале строки
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						high_pointer = new var_const(L"error@", 1);
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
						/*
						else if (current_element->read(L"type") == L"equat")
						{
							high_pointer = new var_const(L"error@", 8);
							general_var_const->pop_back();
							return high_pointer;							
						}*/
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
						
					}
					pDest++;
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
						current_element->point_left->var_id.replace(current_element->point_left->var_id.find_first_of(L"#") + 1, 5, L"defnd");

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
							high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L"#") + 1, 5, L"defnd");

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
								high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L"#") + 1, 5, L"defnd");
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
						temp_pointer = new var_const(L"exprs@", 0, low_pointer);
						temp_pointer = filling_vector(pDest + 1, temp - 1, temp_pointer, brakets + brakets_counter);
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							if ((temp_pointer->read(L"type") == L"exprs") || (temp_pointer->read(L"type") == L"const"))
							{
								high_pointer = temp_pointer;
								low_pointer = high_pointer;
							}
							else if (temp_pointer->read(L"type") == L"varbl")
							{
								current_element->var_id.replace(0, 5, L"funct");
								current_element->var_id.insert(current_element->var_id.find_first_of('@') + 1, L"(" + temp_pointer->read(L"name") + L")");
								if (temp_pointer->read(L"prop") == L"undfn")
								{
									temp_pointer->var_id.replace(current_element->var_id.find_first_of('#') + 1, 5, L"defnd");
									general_var_const->push_back(new var_const(temp_pointer));
								}
								current_element->point_collar = temp_pointer;
								current_element->point_left = temp_pointer;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else if (temp_pointer->read(L"type") == L"funct")
							{
								current_element->var_id.replace(0, 5, L"funct");
								current_element->var_id.insert(current_element->var_id.find_first_of('@') + 1, L"(" + temp_pointer->read(L"nvar") + L")");
								current_element->point_collar = temp_pointer->point_collar;
								current_element->point_left = temp_pointer;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						else if (low_pointer = high_pointer)
						{
							// вначале строки что-то стоит - минус или буква - потом скобка
						}
						else if (high_pointer->point_right == NULL)
						{
							if ((temp_pointer->read(L"type") == L"exprs") || (temp_pointer->read(L"type") == L"const"))
							{
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
							else if (temp_pointer->read(L"type") == L"varbl")
							{
								if (current_element->read(L"type") == L"funct")
								{
									if (current_element->read(L"nvar") == temp_pointer->read(L"name"))
									{
										high_pointer->point_right = temp_pointer;
										low_pointer = high_pointer->point_right;
									}
									else
									{
										//тут доделать функции/уравнения нескольких переменных
										high_pointer = new var_const(L"error@", -5);
										general_var_const->pop_back();
										return high_pointer;
									}
								}
								else if (current_element->read(L"type") == L"exprs")
								{
									current_element->var_id.replace(0, 5, L"funct");
									current_element->var_id.insert(current_element->var_id.find_first_of('@') + 1, L"(" + temp_pointer->read(L"name") + L")");
									if (temp_pointer->read(L"prop") == L"undfn")
									{
										temp_pointer->var_id.replace(current_element->var_id.find_first_of('#') + 1, 5, L"defnd");
										general_var_const->push_back(new var_const(temp_pointer));
									}
									current_element->point_collar = temp_pointer;
									high_pointer->point_right = current_element->point_collar;
									low_pointer = high_pointer->point_right;;
								}
							}
							else if (temp_pointer->read(L"type") == L"funct")
							{
								if (current_element->read(L"type") == L"funct")
								{
									if (current_element->read(L"nvar") == temp_pointer->read(L"nvar"))
									{
										high_pointer->point_right = temp_pointer;
										low_pointer = high_pointer->point_right;
									}
									else
									{
										//тут доделать функции/уравнения нескольких переменных
										high_pointer = new var_const(L"error@", -5);
										general_var_const->pop_back();
										return high_pointer;
									}
								}
								else if (current_element->read(L"type") == L"exprs")
								{
									current_element->var_id.replace(0, 5, L"funct");
									current_element->var_id.insert(current_element->var_id.find_first_of('@') + 1, L"(" + temp_pointer->read(L"nvar") + L")");
									current_element->point_collar = temp_pointer->point_collar;
									high_pointer->point_left = temp_pointer;
									low_pointer = high_pointer->point_left;
								}
							}
						}
						else
						{
							if (low_pointer->read(L"type") == L"funct")
							{
								if (low_pointer->read(L"name") == L"minus")
								{
									if (low_pointer->point_right == NULL)
									{
										//еще проверки
										low_pointer->point_right = temp_pointer;
									}
									else
									{
										//тут отложу пока - минус перед функцией или конст или переменной и дальше скобка
										high_pointer = new var_const(L"error@", -7);
										general_var_const->pop_back();
										return high_pointer;
									}
								}
								else
								{
									if (temp_pointer->read(L"type") == L"exprs")
									{
										low_pointer->point_right = temp_pointer->point_left;
									}
									else if (temp_pointer->read(L"type") == L"funct")
									{
										if (current_element->point_collar == NULL)
										{
											low_pointer->point_right = temp_pointer;
											low_pointer->point_collar = temp_pointer->point_collar;
											current_element->point_collar = temp_pointer->point_collar;
											current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + temp_pointer->read(L"nvar") + L")");
										}
										else
										{
											if (temp_pointer->read(L"nvar") == current_element->read(L"nvar"))
											{
												low_pointer->point_right = temp_pointer;
											}
											else
											{
												//тут доделать функции/уравнения нескольких переменных
												high_pointer = new var_const(L"error@", -5);
												general_var_const->pop_back();
												return high_pointer;
											}
										}
									}
									else if (temp_pointer->read(L"type") == L"const")
									{
										low_pointer->point_right = temp_pointer;
									}
									else if (temp_pointer->read(L"type") == L"varbl")
									{
										if (current_element->point_collar == NULL)
										{
											low_pointer->point_collar = temp_pointer;
											current_element->point_collar = temp_pointer;
											current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + temp_pointer->read(L"namer") + L")");
										}
										else
										{
											if (temp_pointer->read(L"name") != current_element->read(L"nvar"))
											{
												//тут доделать функции/уравнения нескольких переменных
												high_pointer = new var_const(L"error@", -5);
												general_var_const->pop_back();
												return high_pointer;
											}
										}
									}
								}
							}
							else if (low_pointer->read(L"type") == L"varbl")
							{

							}
							else if (low_pointer->read(L"type") == L"const")
							{

							}
							else if (low_pointer->read(L"type") == L"numbr")
							{
								//возможно этот случай лучше исключить в разделе обработки чисел
							}
							else if (low_pointer->read(L"type") == L"opert")
							{
								high_pointer = new var_const(L"error@", -6);
								general_var_const->pop_back();
								return high_pointer;
							}
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
					temp = wcspbrk(pDest, L"()+-*^/=");
					//если строчка начинается с cимвола
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						if ((temp == NULL) && (brakets == 0))
						{
							high_pointer = new var_const(L"error@", 2);
							general_var_const->pop_back();
							return high_pointer;
						}
						else if (temp == NULL)
						{
							name.assign(pDest, endPtr);
							pDest = endPtr;
						}
						else
						{
							name.assign(pDest, temp);
							pDest = temp;
						}
						high_pointer = run_vector(name);
						if (high_pointer == NULL)
						{
							if (temp == NULL)
							{
								current_element->var_id.replace(0, 6, L"varbl@");
								current_element->var_id += name;
								current_element->var_id += L"#undef";
							}
							else if (*temp == '=')
							{
								current_element->var_id.replace(0, 6, L"varbl@");
								current_element->var_id += name;
								current_element->var_id += L"#undef";
							}
							else if (*temp == '(')
							{
								current_element->var_id.replace(0, 6, L"funct@");
								current_element->var_id += name;
								current_element->var_id += L"#undef";
							}
							else if (*temp == ')')
							{
								high_pointer = new var_const(L"error@", 7);
								general_var_const->pop_back();
								return high_pointer;
							}
							else
							{
								current_element->var_id = L"funct@";
								current_element->var_id += L"(" + name + L")";
								current_element->var_id += L"#undef";
								name.insert(0, L"varbl@");
								name += L"#defnd";
								general_var_const->push_back(new var_const(name, 0));
								current_element->point_collar = new var_const(general_var_const->at(temp_size_of_vect));
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}

						}
						else if (high_pointer->read(L"type") == L"const")
						{
							if (temp == NULL)
							{
								current_element = new var_const(high_pointer);

							}
							else if (*temp == '=')
							{
								current_element = high_pointer;
								current_element->var_id.replace(0, 5, L"varbl");
								general_var_const->pop_back();
							}
							else if (*temp == '(')
							{
								current_element = high_pointer;
								general_var_const->pop_back();
								current_element->point_collar = new var_const();
							}
							else if (*temp == ')')
							{
								high_pointer = new var_const(L"error@", 7);
								general_var_const->pop_back();
								return high_pointer;
							}
							else
							{
								current_element->point_left = new var_const(high_pointer);
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						else if (high_pointer->read(L"type") == L"varbl")
						{
							if (temp == NULL)
							{
								current_element = new var_const(high_pointer);

							}
							else if (*temp == '=')
							{
								current_element = high_pointer;
								general_var_const->pop_back();
							}
							else if (*temp == ')')
							{
								high_pointer = new var_const(L"error@", 7);
								general_var_const->pop_back();
								return high_pointer;
							}
							else if (*temp == '(')
							{

								current_element = high_pointer;
								general_var_const->pop_back();
								current_element->point_collar = new var_const();
							}
							else
							{
								current_element->var_id = L"funct@";
								current_element->var_id += L"(" + name + L")";
								current_element->var_id += L"#undef";
								current_element->point_collar = new var_const(high_pointer);
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						else if (high_pointer->read(L"type") == L"funct")
						{
							if (temp == NULL)
							{
								current_element = new var_const(high_pointer);

							}
							else if (*temp == '=')
							{
								current_element = high_pointer;
								general_var_const->pop_back();
							}
							else if (*temp == ')')
							{
								high_pointer = new var_const(L"error@", 7);
								general_var_const->pop_back();
								return high_pointer;
							}
							else if (*temp == '(')
							{
								current_element->var_id = L"funct@";
								current_element->var_id += L"(" + high_pointer->read(L"nvar") + L")";
								current_element->var_id += L"#undef";
								current_element->point_left = new var_const(high_pointer);
								current_element->point_collar = high_pointer->point_collar;
								high_pointer = current_element->point_left;
								high_pointer->var_id.replace(high_pointer->var_id.find_first_of('#') + 1, 5, L"undef");
								low_pointer = high_pointer;
							}
							else
							{
								current_element->var_id = L"funct@";
								current_element->var_id += L"(" + high_pointer->read(L"nvar") + L")";
								current_element->var_id += L"#undef";
								current_element->point_left = new var_const(high_pointer);
								current_element->point_collar = high_pointer->point_collar;
								high_pointer = current_element->point_left;
								high_pointer->var_id.replace(high_pointer->var_id.find_first_of('#') + 1, 5, L"defnd");
								low_pointer = high_pointer;
							}
						}
						//символ не в начале строки
						else
						{
							if (temp == NULL)
							{
								name.assign(pDest, endPtr);
								pDest = endPtr;
							}
							else
							{
								name.assign(pDest, temp);
								pDest = temp;
							}
							low_pointer = run_vector(name);
							if (low_pointer == NULL)
							{
								if (current_element->read(L"type") == L"funct")
								{
									//тут доделать функции/уравнения нескольких переменных
									high_pointer = new var_const(L"error@", -5);
									general_var_const->pop_back();
									return high_pointer;
								}

								else if (current_element->read(L"type") == L"exprs")
								{
									// выражение в undef функцию 
									current_element->var_id.replace(0, 6, L"funct@");
									current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"undef");
									current_element->var_id.insert(current_element->var_id.find_first_of(L'@') + 1, L"(" + name + L")");
									name.insert(0, L"varbl@");
									name += L"#defnd";
									general_var_const->push_back(new var_const(name, 0));
									current_element->point_collar = new var_const(general_var_const->at(temp_size_of_vect));
									if (high_pointer->point_right->read(L"name") == L"minus")
									{
										high_pointer->point_right->point_right = current_element->point_collar;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}
							}
							else if (low_pointer->read(L"type") == L"varbl")
							{
								if (current_element->read(L"type") == L"funct")
									if (low_pointer->read(L"name") != current_element->read(L"nvar"))
									{
										//тут доделать функции/уравнения нескольких переменных
										high_pointer = new var_const(L"error@", -5);
										general_var_const->pop_back();
										return high_pointer;
									}
									else
									{
										if (high_pointer->point_right->read(L"name") == L"minus")
										{
											high_pointer->point_right->point_right = current_element->point_collar;
											low_pointer = high_pointer->point_right;
										}
										else
										{
											high_pointer->point_right = current_element->point_collar;
											low_pointer = high_pointer->point_right;
										}
									}

								else if (current_element->read(L"type") == L"exprs")
								{
									current_element->var_id.replace(0, 6, L"funct@");
									current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"undef");
									current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + name + L")");
									current_element->point_collar = new var_const(low_pointer);
									if (high_pointer->point_right->read(L"name") == L"minus")
									{
										high_pointer->point_right->point_right = current_element->point_collar;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}
							}
							else if (low_pointer->read(L"type") == L"const")
							{
								if (high_pointer->point_right->read(L"name") == L"minus")
								{
									high_pointer->point_right->point_right = new var_const(low_pointer);
								}
								else
								{
									high_pointer->point_right = new var_const(low_pointer);
								}
								low_pointer = high_pointer->point_right;
							}
							else if (low_pointer->read(L"type") == L"funct")
							{
								if (current_element->read(L"type") == L"funct")
								{
									if (low_pointer->read(L"nvar") == current_element->read(L"nvar"))
									{
										if (high_pointer->point_right->read(L"name") == L"minus")
										{
											high_pointer->point_right->point_right = new var_const(low_pointer);
										}
										else
										{
											high_pointer->point_right = new var_const(low_pointer);
										}
										low_pointer = high_pointer->point_right;
									}
									else
									{
										//тут доделать функции/уравнения нескольких переменных
										high_pointer = new var_const(L"error@", -5);
										general_var_const->pop_back();
										return high_pointer;
									}
								}
								else if (current_element->read(L"type") == L"exprs")
								{
									if (*temp == '(')
									{
										current_element->var_id.replace(0, 6, L"funct@");
										current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"undef");
										if (high_pointer->point_right->read(L"name") == L"minus")
										{
											high_pointer->point_right->point_right = new var_const(low_pointer);
											high_pointer->point_right->point_right->var_id.replace(high_pointer->point_right->point_right->var_id.find_first_of(L'#') + 1, 5, L"defnd");
										}
										else
										{
											high_pointer->point_right = new var_const(low_pointer);
											high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L'#') + 1, 5, L"defnd");
										}
										low_pointer = high_pointer->point_right;
									}
									else
									{
										//тут нужна доп проверка на именные функции. для них всегда надо явно указывать переменные.
										current_element->var_id.replace(0, 6, L"funct@");
										current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"undef");
										current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + low_pointer->read(L"nvar") + L")");
										if (high_pointer->point_right->read(L"name") == L"minus")
										{
											high_pointer->point_right->point_right = new var_const(low_pointer);
											high_pointer->point_right->point_right->var_id.replace(high_pointer->point_right->point_right->var_id.find_first_of(L'#') + 1, 5, L"defnd");
										}
										else
										{
											high_pointer->point_right = new var_const(low_pointer);
											high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L'#') + 1, 5, L"defnd");
										}
										current_element->point_collar = high_pointer->point_right->point_collar;
										low_pointer = high_pointer->point_right;
									}
								}
							}
						}
					}
				}
			}
			return current_element;
		}

		/*Функция выполняет проверку/анализ результата заполнения дерева операций и запросов пользователя.
		Возвращает строку с результатом текущей итерации вычислений.*/
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, var_const* _current_element)
		{
			_current_element = filling_vector(_pDest, _endPtr, _current_element, 0);
			wstring output;
			size_t output_size;
			if (_current_element->read(L"type") == L"error")
			{
				//TODO: ошибки!
				/*сюда пишу возможные коды ошибок и ограничений ввода, которые будут выдаваться при заполнении дерева
				доработать
				-1 - запись вида f(y) - где y - определённая заранее функция. Вложенные/сложные функции не реализованы.
				-2 - запись вида f(x)+2...=..., f+2...=... при неопределённой или определённой заранее f. можно рассматривать как неявное задание
				функции или функциональное уравнение, но пока не хочется
				-3 - запись вида f(2+3*x), f(2*5) - неявное вложение функции или использование константного выражение как аргумента
				-4 - поскольку уравнения недоработаны, эта ошибка выскочит при попытке ввода уравнения
				ошибки
				-5 - функции/уравнение нескольких переменных
				-6 - каким-то образом low_pointer указыват на операцию
				-7 - минус перед функцией

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
			}
			else if (_current_element->read(L"type") == L"exprs")
			{
				_current_element->arithmetic();		
				output = to_string(_current_element->var, var_type::FRACTIONAL, 2);
			}
			else 
			{				
				//else else esle!
			}
			return output;
		}

		/*Функция является точкой входа в вычислительное ядро.
		Возвращает строку с результатом текущей итерации вычислений.*/
		wstring input_to_analize(wchar_t* input)
		{
			wchar_t* error_str = Project::IO::VerifyInput(input);
			if (error_str != NULL)
				return error_str;

			int size_of_vect = general_var_const->size();
			
			wstring temp = L"exprs@#undef";			
			general_var_const->push_back(new var_const(temp, 0));
			
			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer	
			return analized_output(point_start, point_end, general_var_const->at(size_of_vect));

			
		}
	}
}
