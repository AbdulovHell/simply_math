///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///

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
					return to_string(pointer->var,var_type::FRACTIONAL,2);
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

			void copy(var_const var1)
			{
				var_id = var1.var_id;
				var = var1.var;
				point_left = var1.point_left;
				point_right = var1.point_right;
			}

			int get_priority()
			{
				char operation = var_id[0];
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
				wchar_t* out = (wchar_t*)malloc(5*sizeof(wchar_t));
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
							out = (wchar_t*)malloc((temp - (id + 1))*2);
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
			double var_im;
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
		}

		
		var_const* filling_vector(wchar_t* pDest, wchar_t* endPtr, var_const* current_elment)
		{			
			int temp_size_of_vect;
			int count;
			var_const *high_pointer = NULL;
			var_const *low_pointer = NULL;
			wchar_t* p_var;
			wchar_t* temp;
			int brakets_counter = 0;
			while (pDest <= endPtr) {

				if (*pDest == '+')
				{									
					//если это первая операция в выражении
					if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_elment->point_left = new var_const(L"+", brakets_counter, low_pointer, NULL, current_elment);						
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_elment->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + 1))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"+", brakets_counter, low_pointer, NULL, high_pointer);								
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ! или равен приоритету текущей (получена операция с наименьшим приоритетом)
							if (current_elment->point_left->get_priority() >= (brakets_counter + 1))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new var_const(L"+", brakets_counter, current_elment->point_left, NULL, current_elment);								
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_elment->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
								current_elment->point_left = high_pointer;
							}

							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
							//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей

							else
							{
								//получаем указатель на первую операцию в текущей ветке, которая имеет приоритет меньше или равный приоритету текущей 
								high_pointer = high_pointer->prioritize(brakets_counter + 1);
								//записываем операцию, левый рукав -> правый рукав найденной операции. Воротник -> на саму найденную операцию
								//воротник правого рукава (!) найденной операции -> созданную операцию
								high_pointer->point_right->point_collar = new var_const(L"+", brakets_counter, high_pointer->point_right, NULL, high_pointer);								
								//правый рукав найденной операции -> созданную операцию
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}
						}
					}
					pDest++;
				}
				else if (*pDest == '*')
				{										
					//если это первая операция в выражении
					if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_elment->point_left = new var_const(L"*", brakets_counter, low_pointer, NULL, current_elment);						
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_elment->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + 2))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"*", brakets_counter, low_pointer, NULL, high_pointer);							
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ! или равен приоритету текущей (получена операция с наименьшим приоритетом)
							if (current_elment->point_left->get_priority() >= (brakets_counter + 2))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new var_const(L"*", brakets_counter, current_elment->point_left, NULL, current_elment);
								
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_elment->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
								current_elment->point_left = high_pointer;
							}

							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
							//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей

							else
							{
								//получаем указатель на первую операцию в текущей ветке, которая имеет приоритет меньше или равный приоритету текущей 
								high_pointer = high_pointer->prioritize(brakets_counter + 2);
								//записываем операцию, левый рукав -> правый рукав найденной операции. Воротник -> на саму найденную операцию
								//воротник правого рукава (!) найденной операции -> созданную операцию
								high_pointer->point_right->point_collar = new var_const(L"*", brakets_counter, high_pointer->point_right, NULL, high_pointer);								
								//правый рукав найденной операции -> созданную операцию
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}
						}
					}					
					pDest++;
				}
				else if (*pDest == '/')
				{					
					//если это первая операция в выражении
					if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_elment->point_left = new var_const(L"/", brakets_counter, low_pointer, NULL, current_elment);						
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_elment->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + 3))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"/", brakets_counter, low_pointer, NULL, high_pointer);							
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ! или равен приоритету текущей (получена операция с наименьшим приоритетом)
							if (current_elment->point_left->get_priority() >= (brakets_counter + 3))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new var_const(L"/", brakets_counter, current_elment->point_left, NULL, current_elment);								
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_elment->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
								current_elment->point_left = high_pointer;
							}

							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
							//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей

							else
							{
								//получаем указатель на первую операцию в текущей ветке, которая имеет приоритет меньше или равный приоритету текущей 
								high_pointer = high_pointer->prioritize(brakets_counter + 3);
								//записываем операцию, левый рукав -> правый рукав найденной операции. Воротник -> на саму найденную операцию
								//воротник правого рукава (!) найденной операции -> созданную операцию
								high_pointer->point_right->point_collar = new var_const(L"/", brakets_counter, high_pointer->point_right, NULL, high_pointer);								
								//правый рукав найденной операции -> созданную операцию
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}
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
						current_elment->point_left = new var_const(L"0", wcstod(pDest, &pDest), current_elment);						
						//оба указателя -> на число, тебуется для проверки условия при записи операции
						low_pointer = current_elment->point_left;
						high_pointer = low_pointer;
					}
					//если были другие числа/операции перед минусом
					else
					{
						//если это первая операция в выражении (сложение)
						if (high_pointer == low_pointer)
						{
							//сначала записываем операцию, левый рукав -> на предыдущее число, воротник на конст
							current_elment->point_left = new var_const(L"+", brakets_counter, low_pointer, NULL, current_elment);
							high_pointer = current_elment->point_left;							
							high_pointer->point_right = new var_const(L"0", wcstod(pDest, &pDest), high_pointer);							
							low_pointer = high_pointer->point_right;
						}
						//если ранее были другие операции
						else
						{
							//если операция вычитания стоит в записи после операции, стоящей перед скобкой (т.е. вычитание стоит в скобках)
							if (brakets_counter > high_pointer->var)
							{
								//если предыдущая операция заполнена до конца (правый рукав на что-то указывает) н-р 7*(5-2)
								if (high_pointer->point_right != NULL)
								{
									//сначала записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
									high_pointer->point_right = new var_const(L"+", brakets_counter, low_pointer, NULL, high_pointer);
									high_pointer = high_pointer->point_right;
									
									 //создание элемента класса и запись числа, воротник на созданную операцию
									 //Правый рукав предшествующей операции на созданное число
									high_pointer->point_right = new var_const(L"0", wcstod(pDest, &pDest), high_pointer);
									//нижний указатель -> на созданное число
									low_pointer = high_pointer->point_right;	
								}
							//если предыдущая операция не заполнена до конца (правый рукав ни на что не указывает). 
							//Равносильно ситуации н-р: 7*(-2+5), т.е. знак минус стоит сразу после скобки
								else
								{
									//создание элемента класса и запись числа, воротник на пред операцию
									//Правый рукав предшествующей операции на созданное число. 
									high_pointer->point_right = new var_const(L"0", wcstod(pDest, &pDest), high_pointer);									
									//нижний указатель -> на созданное число
									low_pointer = high_pointer->point_right;	
								}
							}
							//если операция вычитания стоит в записи после операции, стоящей внутри одних и тех же скобок (или вообще без них)
							else
							{
								//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
								if (high_pointer->get_priority() <= (brakets_counter + 1))
								{
									//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
									high_pointer->point_right = new var_const(L"+", brakets_counter, low_pointer, NULL, high_pointer);									
									high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
								}
								//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
								else
								{
									//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ! или равен приоритету текущей (получена операция с наименьшим приоритетом)
									if (current_elment->point_left->get_priority() >= (brakets_counter + 1))
									{
										//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
										high_pointer = new var_const(L"+", brakets_counter, current_elment->point_left, NULL, current_elment);										
										//воротник предыдущей легчайшей операции -> на новую операцию
										current_elment->point_left->point_collar = high_pointer;
										//указываем левым рукавом константы на созданную операцию
										current_elment->point_left = high_pointer;
									}

									//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
									//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей

									else
									{
										//получаем указатель на первую операцию в текущей ветке, которая имеет приоритет меньше или равный приоритету текущей 
										high_pointer = high_pointer->prioritize(brakets_counter + 1);
										//записываем операцию, левый рукав -> правый рукав найденной операции. Воротник -> на саму найденную операцию
										//воротник правого рукава (!) найденной операции -> созданную операцию
										high_pointer->point_right->point_collar = new var_const(L"+", brakets_counter, high_pointer->point_right, NULL, high_pointer);										
										//правый рукав найденной операции -> созданную операцию
										high_pointer->point_right = high_pointer->point_right->point_collar;
										high_pointer = high_pointer->point_right;
									}
								}
								
								 //создание элемента класса и запись числа, воротник -> пред операцию
								 //Правый рукав предшествующей операции на созданное число
								high_pointer->point_right = new var_const(L"0", wcstod(pDest, &pDest), high_pointer);								
								//нижний указатель -> на созданное число
								low_pointer = high_pointer->point_right;
							}
						}
					}
				}
				else if (*pDest == '(')
				{                  //левая скобка увеличивает счётчик скобок на число, равное самому высокому id функции
					brakets_counter += 3;
					pDest++;
				}
				else if (*pDest == ')')
				{				//правая скобка меньшает счётчик скобок на число, равное самому высокому id функции
					brakets_counter -= 3;
					pDest++;
				}
				else if ((*pDest == '1') || (*pDest == '2') || (*pDest == '3') || (*pDest == '4') || (*pDest == '5') || (*pDest == '6') || (*pDest == '7') || (*pDest == '8') || (*pDest == '9') || (*pDest == '0'))
				{                   //любое число имеет id "0"					
					// если данное число первое (и возможно единственное) в записи выражения
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> константу
						current_elment->point_left = new var_const(L"0", wcstod(pDest, &pDest), current_elment);						
						//оба указателя -> на число, тебуется для проверки условия при записи операции
						low_pointer = current_elment->point_left;
						high_pointer = low_pointer;							
					}
					// если данное число не первое в записи (была какая-либо операция)
					else
					{
						//создание элемента класса и запись числа, воротник -> пред операцию 
						high_pointer->point_right = new var_const(L"0", wcstod(pDest, &pDest), high_pointer);
						low_pointer = high_pointer->point_right ;
					}
				}
				else
				{
					temp_size_of_vect = general_var_const->size();
					temp = wcspbrk(pDest, L")+-*/^");
					if (temp != NULL)
					{
						p_var = (wchar_t*)malloc((temp - pDest + 1)*2);
						wcsncpy(p_var, pDest, temp - pDest);
						p_var[temp - pDest] = 0;
						temp = NULL;
					}
					else
					{
						p_var = (wchar_t*)malloc((endPtr - pDest + 1)*2);
						wcscpy(p_var, pDest);						
					}
					//проходим по вектору, ищем переменную/конст/функц с таким именем
					for (count = 0; count < temp_size_of_vect; count++)
					{
						//проверяем имя, второе условие - на случай записи t=y+2, при этом у уже определён ранее y(x)=5*x
						if ((p_var == general_var_const->at(count)->read(L"name")) || (p_var == general_var_const->at(count)->read(L"func")))
						{
							//если найдена константа
							if (general_var_const->at(count)->read(L"type") == L"const")
							{
								if ((high_pointer == NULL) && (low_pointer == NULL))
								{
									//оба указателя -> на конст или перем из массива, тебуется для проверки условия при записи операции
									high_pointer = general_var_const->at(count);
									low_pointer = general_var_const->at(count);
									//левый рукав вычисляемой константы -> созданную структуру с числом.
									current_elment->point_left = general_var_const->at(count);
								}
								else
								{
									//нижний указатель -> на конст или перем из массива
									low_pointer = general_var_const->at(count);
									//Правый рукав предшествующей операции на конст или перем из массива
									high_pointer->point_right = low_pointer;
								}
								break;  //не имеет смысла считать дальше
							}
							//если найдена переменная
							else if (general_var_const->at(count)->read(L"type") == L"varbl")
							{
								//и она соответствует переменной на которую указывает функция
								if (current_elment->point_right == general_var_const->at(count))
								{
									if ((high_pointer == NULL) && (low_pointer == NULL))
									{
										//оба указателя -> на конст или перем из массива, тебуется для проверки условия при записи операции
										high_pointer = general_var_const->at(count);
										low_pointer = general_var_const->at(count);
										//левый рукав вычисляемой константы -> созданную структуру с числом.
										current_elment->point_left = general_var_const->at(count);
									}
									else
									{
										//нижний указатель -> на конст или перем из массива
										low_pointer = general_var_const->at(count);
										//Правый рукав предшествующей операции на конст или перем из массива
										high_pointer->point_right = low_pointer;
									}
									break;  //не имеет смысла считать дальше
								}
								//и она не соответствует переменной на которую указывает функция
								else
								{
									//ошибка, мы ничего не знаем о функциях нескольких аргументов и тп
								}

							}
							//если найдена функция	
							else if (general_var_const->at(count)->read(L"type") == L"funct")
							{
								
								break;//пока ошибка. в данном случая это вложение одной функции в другую, необходимо проверять соответствие переменных в обеих
							}
						}
					}
					//если в массиве нет ничего с таким именем, найдена новая переменная
					if (count == temp_size_of_vect)
					{
						temp = (wchar_t*)malloc(6 * sizeof(wchar_t) + wcslen(p_var));
						wcscpy(temp, L"varbl@");
						wcscat(temp, p_var);
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							general_var_const->push_back(new var_const(temp, 0, current_elment));							
							//оба указателя -> на конст или перем из массива, тебуется для проверки условия при записи операции
							high_pointer = general_var_const->at(temp_size_of_vect);
							low_pointer = general_var_const->at(temp_size_of_vect);
							//левый рукав вычисляемой константы -> созданную структуру с числом.
							current_elment->point_left = general_var_const->at(temp_size_of_vect);
						}
						else
						{
							general_var_const->push_back(new var_const(temp, 0, current_elment));							
							//нижний указатель -> на конст или перем из массива
							low_pointer = general_var_const->at(temp_size_of_vect);
							//Правый рукав предшествующей операции на конст или перем из массива
							high_pointer->point_right = low_pointer;
						}
						//free(temp);
						if (current_elment->read(L"type") == L"const")
						{
							temp = (wchar_t*)malloc(7 * sizeof(wchar_t) + wcslen(p_var) + wcslen(current_elment->read(L"name").c_str()));
							wcscpy(temp, L"funct@");
							wcscat(temp, current_elment->read(L"name").c_str());
							wcscat(temp, L"(");
							wcscat(temp, p_var);
							wcscat(temp, L")");
							current_elment->var_id = temp;
							current_elment->point_right = low_pointer;
							//free(temp);
						}
						else if(current_elment->read(L"type") == L"equat")//пока оставлю это здесь для будущей реализации уравнений
						{
							
						}
						else
						{
							//случай, когда мы попали на функцию, которая указывает на другую, уже заданную переменную,
							//н-р ввод f(x)=t+1, пока ошибка ввода - мы не умеем функции нескольких переменных
						}

					}
					pDest+=wcslen(p_var);
					free(p_var);
					free(temp);
				}
			}
			return current_elment;
		}

		wchar_t* analized_output(wchar_t* _pDest, wchar_t* _endPtr, var_const* _current_elment)
		{
			_current_elment = filling_vector(_pDest, _endPtr, _current_elment);
			wstring expr;
			int output_size;
			if (_current_elment->read(L"type") == L"const")
			{
				_current_elment->arithmetic();
				expr = _current_elment->expresion();
				output_size = wcslen(_current_elment->var_id.c_str()) + wcslen(to_string(_current_elment->var, var_type::FRACTIONAL,2).c_str()) + 10 + wcslen(expr.c_str());
				wchar_t* output = (wchar_t*)malloc(output_size*2);
				for (int i = 0; i < output_size; i++)
					output[i] = 0;
				wcscpy(output, expr.c_str());
				wcscat(output,L"\n");
				wcscat(output, _current_elment->read(L"name").c_str());
				wcscat(output, L" = ");
				wcscat(output, to_string(_current_elment->var, var_type::FRACTIONAL, 2).c_str());
				return output;
				
			}	
			else if (_current_elment->read(L"type") == L"funct")
			{				
				expr = _current_elment->expresion();
				output_size = wcslen(_current_elment->var_id.c_str()) + wcslen(to_string(_current_elment->var, var_type::FRACTIONAL, 2).c_str()) + 10 + wcslen(expr.c_str());
				wchar_t* output = (wchar_t*)malloc(output_size*2);
				for (int i = 0; i < output_size; i++)
					output[i] = 0;
				wcscpy(output, expr.c_str());
				wcscat(output, L"\n");
				//strcat(output, _current_elment->read("name").c_str());
				//strcat(output, " = ");
				//strcat(output, to_string(_current_elment->var, var_type::FRACTIONAL,2).c_str());
				return output;
			}
			
			
			//потому что иногда я хочу видеть эту строку сразу
			//cout << input_var_const->at(current_size_of_vect).expresion() << endl;
			
		}

		wchar_t* input_to_analize(wchar_t* input)
		{
			wchar_t* error_str = Project::IO::VerifyInput(input);
			if (error_str != NULL)
				return error_str;

			int input_size = wcslen(input);
			int size_of_vect;
			int count;
			int  brackets_left = 0;
			int brackets_right = 0;
			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer			
			
			wchar_t* equal_right = wcsstr(input, L"="); // равно и справа от равно
			wchar_t* temp;
			wchar_t* equal_left = (wchar_t*)malloc((equal_right - input) * 2 + 2); //слева от равно
			memcpy(equal_left, input, (equal_right - input)*2);
			equal_left[(equal_right - input)] = 0;
			vector<var_const*>* k = general_var_const;
			//если справа после равно ничего нет
			if (equal_right == point_end)
			{

			}
			//если справа что-то есть
			//если слева есть операции
			else if (wcspbrk(equal_left, L"+*/^") != NULL)
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
		}
	}
}
