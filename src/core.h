#include <iostream>
#include <string>
#include <vector>
#include "input.h"

namespace Project {
	namespace Core {

		using namespace std;

		class var_const {
		private:
			var_const *prioritize_processing(var_const *pc, int current_priority)
			{
				//если приоритет проверяемой операции !БОЛЬШЕ! текущей операции
				if (pc[0].get_priority() > current_priority)
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
				char operation = pointer[0].var_id[0];
				switch (operation) {
				case'+':
					return processing(pointer[0].point_left) + processing(pointer[0].point_right);
				case '*':
					return processing(pointer[0].point_left) * processing(pointer[0].point_right);
				case '/':
					return processing(pointer[0].point_left) / processing(pointer[0].point_right);
					/*case '4':
					return pow(recursion(pointer[0].point_left), recursion(pointer[0].point_right));
					*/
				default:       //для констант, чисел и переменных
					return pointer[0].var;
				}
			}

			string expression_processing(var_const *pointer)
			{
				if ((pointer[0].var_id == "+") || (pointer[0].var_id == "*") || (pointer[0].var_id == "/"))
				{
					return "(" + expression_processing(pointer[0].point_left) + " " + pointer[0].var_id + " " + expression_processing(pointer[0].point_right) + ")";
				}
				else if (pointer[0].var_id == "0")
				{
					return to_string(pointer[0].var);
				}
				else
				{
					return pointer[0].var_id;
				}
			}

		public:
			var_const()
			{
				var_id = "";
				var = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(string _name, double _num)
			{
				var_id = _name;
				var = _num;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(string _name, double _num, var_const *_pc)
			{
				var_id = _name;
				var = _num;
				point_left = NULL;
				point_right = NULL;
				point_collar = _pc;
			}

			var_const(string _name, double _num, var_const * _pl, var_const *_pr) {
				var_id = _name;
				var = _num;
				point_left = _pl;
				point_right = _pr;
				point_collar = NULL;
			}

			var_const(string _name, double _num, var_const * _pl, var_const *_pr, var_const *_pc) {
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


			string expresion()
			{
				return var_id + " = " + expression_processing(point_left);
			}

			var_const *prioritize(int current_priority)
			{
				return prioritize_processing(point_collar, current_priority);
			}

			void arithmetic()
			{
				var = processing(point_left);
			}


			string var_id;
			double var;
			var_const *point_left;		//левый рукав
			var_const *point_right;		//правый рукав
			var_const *point_collar;	//воротник
		};

		var_const pi = var_const("pi", 3.1415926535897932384626433832);
		var_const e = var_const("e", 2.71828182846);

		vector<var_const>* input_var_const;

		void Init() {
			input_var_const = new vector<var_const>;
			input_var_const->push_back(pi);
			input_var_const->push_back(e);
		}

		char* input_to_analize(char* input)
		{
			char* error_str = Project::Input::VerifyInput(input);
			if (error_str != NULL)
				return error_str;

			int input_size = strlen(input);

			char* pDest = input;	//start pointer
			char* endPtr = input + strlen(input) - 1;	//end pointer
			char *temp = NULL;

			var_const *high_pointer = NULL;
			var_const *low_pointer = NULL;

			char* varnameDest = strstr(input, "=");

			char *varname = (char*)malloc(varnameDest - input + 1);
			memcpy(varname, input, varnameDest - input);
			varname[varnameDest - input] = 0;

			int current_size_of_vect = input_var_const->size(); //равен 2, значит является индексом третьего элемента
			input_var_const->push_back(var_const(varname, 0));
			input_var_const->reserve(input_size * 2 + current_size_of_vect);

			int temp_size_of_vect;
			pDest = varnameDest + 1;
			var_const *k = &input_var_const->at(current_size_of_vect);

			int brakets_counter = 0;
			while (pDest <= endPtr) {

				if (*pDest == '+')
				{
					//операция сложения имеет id "1"	
					temp_size_of_vect = input_var_const->size();
					//если это первая операция в выражении
					if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						input_var_const->push_back(var_const("+", brakets_counter, low_pointer, NULL, &input_var_const->at(current_size_of_vect)));
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
						high_pointer = &input_var_const->at(temp_size_of_vect);// верхний указатель -- на созданную операцию
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer[0].get_priority() <= (brakets_counter + 1))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							input_var_const->push_back(var_const("+", brakets_counter, low_pointer, NULL, high_pointer));
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //правый рукав предыдущей -> на созданную
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ! или равен приоритету текущей (получена операция с наименьшим приоритетом)
							if (input_var_const->at(current_size_of_vect).point_left[0].get_priority() >= (brakets_counter + 1))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								input_var_const->push_back(var_const("+", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL, &input_var_const->at(current_size_of_vect)));
								//воротник предыдущей легчайшей операции -> на новую операцию
								input_var_const->at(current_size_of_vect).point_left[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//указываем левым рукавом константы на созданную операцию
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
							}

							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
							//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей

							else
							{
								//получаем указатель на первую операцию в текущей ветке, которая имеет приоритет меньше или равный приоритету текущей 
								high_pointer = high_pointer[0].prioritize(brakets_counter + 1);
								//записываем операцию, левый рукав -> правый рукав найденной операции. Воротник -> на саму найденную операцию
								input_var_const->push_back(var_const("+", brakets_counter, high_pointer[0].point_right, NULL, high_pointer));
								//воротник правого рукава (!) найденной операции -> созданную операцию
								high_pointer[0].point_right[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//правый рукав найденной операции -> созданную операцию
								high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
							}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //верхний указатель на созданную операцию

						}
					}
					pDest++;
				}
				else if (*pDest == '*')
				{
					//операция сложения имеет id "1"	
					temp_size_of_vect = input_var_const->size();
					//если это первая операция в выражении
					if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						input_var_const->push_back(var_const("*", brakets_counter, low_pointer, NULL, &input_var_const->at(current_size_of_vect)));
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
						high_pointer = &input_var_const->at(temp_size_of_vect);// верхний указатель -- на созданную операцию
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer[0].get_priority() <= (brakets_counter + 2))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							input_var_const->push_back(var_const("*", brakets_counter, low_pointer, NULL, high_pointer));
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //правый рукав предыдущей -> на созданную
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ!  или равен приоритету текущей (получена операция с наименьшим приоритетом)
							if (input_var_const->at(current_size_of_vect).point_left[0].get_priority() >= (brakets_counter + 2))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								input_var_const->push_back(var_const("*", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL, &input_var_const->at(current_size_of_vect)));
								//воротник предыдущей легчайшей операции -> на новую операцию
								input_var_const->at(current_size_of_vect).point_left[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//указываем левым рукавом константы на созданную операцию
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


							}

							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
							//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей
							else
							{
								//получаем указатель на первую операцию в текущей ветке, которая имеет приоритет меньше или равный приоритету текущей 
								high_pointer = high_pointer[0].prioritize(brakets_counter + 2);
								//записываем операцию, левый рукав -> правый рукав найденной операции. Воротник -> на саму найденную операцию
								input_var_const->push_back(var_const("*", brakets_counter, high_pointer[0].point_right, NULL, high_pointer));
								//воротник правого рукава (!) найденной операции -> созданную операцию
								high_pointer[0].point_right[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//правый рукав найденной операции -> созданную операцию
								high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
							}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //верхний указатель на созданную операцию

						}
					}
					pDest++;
				}
				else if (*pDest == '/')
				{
					//операция сложения имеет id "1"	
					temp_size_of_vect = input_var_const->size();
					//если это первая операция в выражении
					if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						input_var_const->push_back(var_const("/", brakets_counter, low_pointer, NULL, &input_var_const->at(current_size_of_vect)));
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
						high_pointer = &input_var_const->at(temp_size_of_vect);// верхний указатель -- на созданную операцию
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer[0].get_priority() <= (brakets_counter + 3))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							input_var_const->push_back(var_const("/", brakets_counter, low_pointer, NULL, high_pointer));
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //правый рукав предыдущей -> на созданную
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ!  или равен приоритету текущей (получена операция с наименьшим приоритетом)
							if (input_var_const->at(current_size_of_vect).point_left[0].get_priority() >= (brakets_counter + 3))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								input_var_const->push_back(var_const("/", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL, &input_var_const->at(current_size_of_vect)));
								//воротник предыдущей легчайшей операции -> на новую операцию
								input_var_const->at(current_size_of_vect).point_left[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//указываем левым рукавом константы на созданную операцию
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


							}

							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
							//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей
							else
							{
								//получаем указатель на первую операцию в текущей ветке, которая имеет приоритет меньше или равный приоритету текущей 
								high_pointer = high_pointer[0].prioritize(brakets_counter + 3);
								//записываем операцию, левый рукав -> правый рукав найденной операции. Воротник -> на саму найденную операцию
								input_var_const->push_back(var_const("/", brakets_counter, high_pointer[0].point_right, NULL, high_pointer));
								//воротник правого рукава (!) найденной операции -> созданную операцию
								high_pointer[0].point_right[0].point_collar = &input_var_const->at(temp_size_of_vect);
								//правый рукав найденной операции -> созданную операцию
								high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
							}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //верхний указатель на созданную операцию

						}
					}
					pDest++;
				}
				else if (*pDest == '-')
				{
					//операция минус понимается как сложение с отрицательным числом, id "1"
					temp_size_of_vect = input_var_const->size();
					//если выражение начинается с минуса (случай когда после минуса идёт число, минус перед скобко не реализован)
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> на конст
						input_var_const->push_back(var_const("0", strtod(pDest, &pDest), &input_var_const->at(current_size_of_vect)));
						//оба указателя -> на число, тебуется для проверки условия при записи операции
						high_pointer = &input_var_const->at(temp_size_of_vect);
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//левый рукав вычисляемой константы -> созданную структуру с числом.
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
					}
					//если были другие числа/операция перед минусом
					else
					{
						//если это первая операция в выражении (сложение)
						if (high_pointer == low_pointer)
						{
							//сначала записываем операцию, левый рукав -> на предыдущее число, воротник на конст
							input_var_const->push_back(var_const("+", brakets_counter, low_pointer, NULL, &input_var_const->at(current_size_of_vect)));
							input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//левый рукав текущей вычисляемой константы указывает на созданную операцию
							high_pointer = &input_var_const->at(temp_size_of_vect);// верхний указатель -- на созданную операцию
							temp_size_of_vect = input_var_const->size(); //счётчик прохода по массиву увеличивается ещё раз для записи самого числа
							 //создание элемента класса и запись числа, воротник -> на пред операцию
							input_var_const->push_back(var_const("0", strtod(pDest, &pDest), high_pointer));
							//нижний указатель -> на созданное число
							low_pointer = &input_var_const->at(temp_size_of_vect);
							//Правый рукав предшествующей операции на созданное число
							high_pointer[0].point_right = low_pointer;
						}
						//если ранее были другие операции
						else
						{
							//если операция вычитания стоит в записи после операции, стоящей перед скобкой (т.е. вычитание стоит в скобках)
							if (brakets_counter > high_pointer[0].var)
								//если предыдущая операция заполнена до конца (правый рукав на что-то указывает)
								if (high_pointer[0].point_right != NULL)
								{
									//сначала записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
									input_var_const->push_back(var_const("+", brakets_counter, low_pointer, NULL, high_pointer));
									//заполняем предыдущую операцию до конца, правый рукав на новую операцию сложения
									high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
									high_pointer = &input_var_const->at(temp_size_of_vect);// верхний указатель -- на созданную операцию
									temp_size_of_vect = input_var_const->size(); //счётчик прохода по массиву увеличивается ещё раз для записи самого числа
									 //создание элемента класса и запись числа, воротник на созданную операцию
									input_var_const->push_back(var_const("0", strtod(pDest, &pDest), high_pointer));
									//нижний указатель -> на созданное число
									low_pointer = &input_var_const->at(temp_size_of_vect);
									//Правый рукав предшествующей операции на созданное число
									high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
								}
							//если предыдущая операция не заполнена до конца (правый рукав ни на что не указывает). 
							//Равносильно ситуации н-р: 7*(-2+5), т.е. знак минус стоит сразу после скобки
								else
								{
									//создание элемента класса и запись числа, воротник на пред операцию
									input_var_const->push_back(var_const("0", strtod(pDest, &pDest), high_pointer));
									//нижний указатель -> на созданное число
									low_pointer = &input_var_const->at(temp_size_of_vect);
									//Правый рукав предшествующей операции на созданное число. 
									high_pointer[0].point_right = low_pointer;
								}
							//если операция вычитания стоит в записи после операции, стоящей внутри одних и тех же скобок (или вообще без них)
							else
							{
								//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
								if (high_pointer[0].get_priority() <= (brakets_counter + 1))
								{
									//записываем операцию, левый рукав -> на предыдущее число, воротник на пред операцию
									input_var_const->push_back(var_const("+", brakets_counter, low_pointer, NULL, high_pointer));
									high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //правый рукав предыдущей -> на созданную
									high_pointer = &input_var_const->at(temp_size_of_vect);                 //верхний указатель -> на созданную операцию
								}
								//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
								else
								{
									//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ!  или равен приоритету текущей
									if (input_var_const->at(current_size_of_vect).point_left[0].get_priority() >= (brakets_counter + 1))
									{
										//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
										input_var_const->push_back(var_const("+", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL, &input_var_const->at(current_size_of_vect)));
										//воротник предыдущей легчайшей операции -> на новую операцию
										input_var_const->at(current_size_of_vect).point_left[0].point_collar = &input_var_const->at(temp_size_of_vect);
										//указываем левым рукавом константы на созданную операцию
										input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


									}

									//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
									//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей

									else
									{
										//получаем указатель на первую операцию в текущей ветке, которая имеет приоритет меньше или равный приоритету текущей 
										high_pointer = high_pointer[0].prioritize(brakets_counter + 1);
										//записываем операцию, левый рукав -> правый рукав найденной операции. Воротник -> на саму найденную операцию
										input_var_const->push_back(var_const("+", brakets_counter, high_pointer[0].point_right, NULL, high_pointer));
										//воротник правого рукава (!) найденной операции -> созданную операцию
										high_pointer[0].point_right[0].point_collar = &input_var_const->at(temp_size_of_vect);
										//правый рукав найденной операции -> созданную операцию
										high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
									}
									high_pointer = &input_var_const->at(temp_size_of_vect);  //верхний указатель на созданную операцию			
								}
								temp_size_of_vect = input_var_const->size(); //счётчик прохода по массиву увеличивается ещё раз для записи самого числа
								//создание элемента класса и запись числа, воротник -> пред операцию
								input_var_const->push_back(var_const("0", strtod(pDest, &pDest), high_pointer));
								//нижний указатель -> на созданное число
								low_pointer = &input_var_const->at(temp_size_of_vect);
								//Правый рукав предшествующей операции на созданное число
								high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);
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
					temp_size_of_vect = input_var_const->size();

					// если данное число первое (и возможно единственное) в записи выражения
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> константу
						input_var_const->push_back(var_const("0", strtod(pDest, &pDest), &input_var_const->at(current_size_of_vect)));
						//оба указателя -> на число, тебуется для проверки условия при записи операции
						high_pointer = &input_var_const->at(temp_size_of_vect);
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//левый рукав вычисляемой константы -> созданную структуру с числом.
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
					}
					// если данное число не первое в записи (была какая-либо операция)
					else
					{
						//создание элемента класса и запись числа, воротник -> пред операцию 
						input_var_const->push_back(var_const("0", strtod(pDest, &pDest), high_pointer));
						//нижний указатель -> на созданное число
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//Правый рукав предшествующей операции на созданное число
						high_pointer[0].point_right = low_pointer;
					}
				}
			}

			input_var_const->at(current_size_of_vect).arithmetic();
			//cout << input_var_const->at(current_size_of_vect).var_id << " = " << input_var_const->at(current_size_of_vect).var << endl;
			//тут выводится изначальное выражение
			//TODO: не.надо.так.делать.оно не будет работать в гуи. доформируй вывод в output
			//cout << input_var_const->at(current_size_of_vect).expresion()<<endl;
			int output_size = input_var_const->at(current_size_of_vect).var_id.size() + std::to_string(input_var_const->at(current_size_of_vect).var).size() + 50;
			char* output = (char*)malloc(output_size);
			for (int i = 0;i < output_size;i++)
				output[i] = 0;

			strcpy(output, input_var_const->at(current_size_of_vect).var_id.c_str());
			strcat(output, " = ");
			strcat(output, std::to_string(input_var_const->at(current_size_of_vect).var).c_str());

			return output;
		}

	}
}