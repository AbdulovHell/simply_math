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
				if ((pointer[0].var_id == "*") || (pointer[0].var_id == "/"))
				{
					return expression_processing(pointer[0].point_left) + " " + pointer[0].var_id + " " + expression_processing(pointer[0].point_right);
				}
				else if (pointer[0].var_id == "+")
				{
					if ((pointer[0].point_collar[0].var_id == "*") || (pointer[0].point_collar[0].var_id == "/"))
						return "(" + expression_processing(pointer[0].point_left) + " " + pointer[0].var_id + " " + expression_processing(pointer[0].point_right) + ")";
					else
						return expression_processing(pointer[0].point_left) + " " + pointer[0].var_id + " " + expression_processing(pointer[0].point_right);
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
			
			string read(string arg)
			{
				char* id = strstr(&var_id[0], "@");
				char* temp = (char*)malloc(5*sizeof(char));
				if (id != NULL)
				{
					if (arg == "type")
					{
						strncpy(temp, &var_id[0], 5);
						temp[5] = 0;
						return temp;
					}
					else if (arg == "name")
					{
						return id + 1;
					}
				}
				else
				{
					return var_id;
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

		var_const pi = var_const("const@pi", 3.1415926535897932384626433832);
		var_const e = var_const("const@e", 2.71828182846);

		vector<var_const>* general_var_const;

		void Init() {
			general_var_const = new vector<var_const>;
			general_var_const->push_back(pi);
			general_var_const->push_back(e);
		}


		char* filling_vector(char* pDest, char* endPtr, vector<var_const> *input_var_const,int current_size_of_vect)
		{
			int temp_size_of_vect;
			int count;
			var_const *high_pointer = NULL;
			var_const *low_pointer = NULL;
			char* p_var;
			char* temp;
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
				else
				{
					temp_size_of_vect = input_var_const->size();
					temp = strpbrk(pDest, ")+-*/^");
					if (temp != NULL)
					{
						p_var = (char*)malloc(temp - pDest + 1);
						strncpy(p_var, pDest, temp - pDest);
						p_var[temp - pDest] = 0;
					}
					else
					{
						p_var = (char*)malloc(endPtr - pDest + 1);
						strcpy(p_var, pDest);						
					}
					//проходим по вектору, ищем переменную/конст/функц с таким именем
					for (count = 0; count < temp_size_of_vect; count++)
					{						
							if (input_var_const->at(count).read("type") == "const")
							{
								//если найдена константа
								if (p_var == input_var_const->at(count).read("name"))
								{
									if ((high_pointer == NULL) && (low_pointer == NULL))
									{
										//оба указателя -> на конст или перем из массива, тебуется для проверки условия при записи операции
										high_pointer = &input_var_const->at(count);
										low_pointer = &input_var_const->at(count);
										//левый рукав вычисляемой константы -> созданную структуру с числом.
										input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(count);
									}
									else
									{
										//нижний указатель -> на конст или перем из массива
										low_pointer = &input_var_const->at(count);
										//Правый рукав предшествующей операции на конст или перем из массива
										high_pointer[0].point_right = low_pointer;
									}
									break;  //не имеет смысла считать дальше
								}
							}
							//если найдена переменная
							else if (input_var_const->at(count).read("type") == "varbl")
							{												
								if (p_var == input_var_const->at(count).read("name"))
									//и она соответствует переменной на которую указывает функция
									if (input_var_const->at(current_size_of_vect).point_right == &input_var_const->at(count))
									{
										if ((high_pointer == NULL) && (low_pointer == NULL))
										{
											//оба указателя -> на конст или перем из массива, тебуется для проверки условия при записи операции
											high_pointer = &input_var_const->at(count);
											low_pointer = &input_var_const->at(count);
											//левый рукав вычисляемой константы -> созданную структуру с числом.
											input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(count);
										}
										else
										{
											//нижний указатель -> на конст или перем из массива
											low_pointer = &input_var_const->at(count);
											//Правый рукав предшествующей операции на конст или перем из массива
											high_pointer[0].point_right = low_pointer;
										}
										break;  //не имеет смысла считать дальше
									}
									//и она не соответствует переменной на которую указывает функция
									else
									{
										//ошибка, мы ничего не знаем о функциях нескольких аргументов и тп
									}
								
							}
							else if (input_var_const->at(count).read("type") == "funct")
							{
								//если найдена функция
								if (p_var == input_var_const->at(count).read("name"))
								{
									//пока ошибка. в данном случая это вложение одной функции в другую, необходимо проверять соответствие переменных в обеих
								}
							}
					}
					
					
					pDest+=strlen(p_var);
				}
			}
			general_var_const->at(current_size_of_vect).arithmetic();
			int output_size = general_var_const->at(current_size_of_vect).var_id.size() + std::to_string(general_var_const->at(current_size_of_vect).var).size() + 50;
			char* output = (char*)malloc(output_size);
			for (int i = 0; i < output_size; i++)
				output[i] = 0;
			//потому что иногда я хочу видеть эту строку сразу
			cout << input_var_const->at(current_size_of_vect).expresion()<<endl;
			strcpy(output, general_var_const->at(current_size_of_vect).var_id.c_str());
			strcat(output, " = ");
			strcat(output, std::to_string(general_var_const->at(current_size_of_vect).var).c_str());

			return output;
		}

		char* input_to_analize(char* input)
		{
			char* error_str = Project::Input::VerifyInput(input);
			if (error_str != NULL)
				return error_str;

			int input_size = strlen(input);
			int size_of_vect;
			int count;
			int  brackets_left = 0;
			int brackets_right = 0;
			char* point_start = input;	//start pointer
			char* point_end = input + strlen(input) - 1;	//end pointer			
								
			char* equal_right = strstr(input, "="); // равно и справа от равно
			char* temp;
			char *equal_left = (char*)malloc(equal_right - input + 1); //слева от равно
			memcpy(equal_left, input, equal_right - input);
			equal_left[equal_right - input] = 0;

			//если справа после равно ничего нет
			if (equal_right == point_end)
			{

			}
			//если справа что-то есть
			//если слева есть операции
			else if (strpbrk(equal_left, "+*/^") != NULL)
			{
			
			}
			//если слева есть минус
			else if (strstr(equal_left, "-") != NULL)
			{

			}
			//нет операцийскобки
			else if (strstr(equal_left, "(") != NULL)
			{
				if (strstr(equal_left, ")") == NULL)
				{
					                                 //error
				}
				else 
				{
					temp = equal_left;
					while (temp <= equal_right)
					{
						if (strstr(temp, "(") != NULL)
						{
							brackets_left++;
							temp = strstr(temp, "(") + 1;
						}
					}
					temp = equal_left;
					while(temp <= equal_right)
					{
						if (strstr(temp, ")") != NULL)
						{
							brackets_right++;
							temp = strstr(temp, ")") + 1;
						}							
					}
					if ((brackets_left > 1)||(brackets_right > 1)||(brackets_right != brackets_left))
					{
						//error
					}
					//слева от равно нет операций, но есть одна правая и одна левая скобка и нет цифр
					else if (strpbrk(equal_left, "0123456789") == NULL)
					{
						//записываем переменную - всё, что стоит в скобках
						//необходимо будет добавить условие, проверяющее существует ли уже данная переменная в массиве
						//если существует - ничего не записывать - только указать нувую функцию на неё
						size_of_vect = general_var_const->size();
						temp = (char*)malloc(strstr(temp, ")") - strstr(temp, "(") + 7);
						strcpy(temp, "varbl@");
						strcat(temp, strstr(equal_left, "(") + 1);
						temp[strstr(temp, ")") - temp] = 0;
						general_var_const->push_back(var_const(temp, 0));
						free(temp);
						//записываем функцию, указываем правым рукавом  на созданную переменную						
						size_of_vect = general_var_const->size();
						temp = (char*)malloc(equal_right - input + 7);
						strcpy(temp, "funct@");
						strcat(temp, equal_left);
						general_var_const->push_back(var_const(temp, 0));
						general_var_const->reserve(input_size * 2 + size_of_vect);
						free(temp);
						general_var_const->at(size_of_vect).point_right = &general_var_const->at(size_of_vect - 1);
						general_var_const->at(size_of_vect - 1).point_collar = &general_var_const->at(size_of_vect);
						point_start = equal_right + 1;

						return filling_vector(point_start, point_end, general_var_const, size_of_vect);

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
			else if (strpbrk(equal_left, "0123456789") != NULL)
			{

			}
			//слева от равно стоят только буквы 
			else
			{
				size_of_vect = general_var_const->size();
				temp = (char*)malloc(equal_right - input + 7);
				strcpy(temp, "const@");
				strcat(temp, equal_left);
				general_var_const->push_back(var_const(temp, 0));
				general_var_const->reserve(input_size * 2 + size_of_vect);
				free(temp);
				point_start = equal_right + 1;

				return filling_vector(point_start, point_end, general_var_const, size_of_vect);
			}
		}
	}
}