// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <vector>
#include <malloc.h> 

using namespace std;

char* err_str(int pos) {
	char* str = (char*)malloc(pos + 1);
	for (int i = 0;i < pos;i++)
		str[i] = ' ';
	str[pos] = 0;
	str[pos - 1] = '^';
	return str;
}

char* VerifyInput(char* input) {	//возвращает строку, описывающую ошибку, иначе nullptr, 0 кароч.
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
			sprintf(buf,"\nBad symbol, '%c':\n", input[i]);
			strcat(buf,input);
			strcat(buf,"\n");
			strcat(buf,err_str(i));
			strcat(buf,"\n");
			return buf;
		}
	if (left_bracket != right_bracket) {
			sprintf(buf,"\n ( and ) error.\n");
			return buf;
	}
	if (ravno > 1) {
			sprintf(buf,"\n'='>1 error.\n");
			return buf;
	}
	/*
	while (cursor < EndStr) {

	}*/
		return NULL;
}

class var_const {
public:
	var_const()
	{
		var_id = "";
		var = 0;
		point_left = NULL;
		point_right = NULL;
	}

	var_const(string _name, double _num)
	{
		var_id = _name;
		var = _num;
		point_left = NULL;
		point_right = NULL;
	}

	var_const(string _name, double _num, var_const * _pl, var_const *pr) {
		var_id = _name;
		var = _num;
		point_left = _pl;
		point_right = pr;
	}

	void copy(var_const var1)
	{
		var_id = var1.var_id;
		var = var1.var;
		point_left = var1.point_left;
		point_right = var1.point_right;
	}

	double recursion(var_const *pointer)
	{
		char operation = pointer[0].var_id[0];
		switch (operation) {
		case '0':
			return pointer[0].var;
		case'1':
			return recursion(pointer[0].point_left) + recursion(pointer[0].point_right);
		case '2':
			return recursion(pointer[0].point_left) * recursion(pointer[0].point_right);
		case '3':
			return recursion(pointer[0].point_left) / recursion(pointer[0].point_right);
			/*case '4':
			return pow(recursion(pointer[0].point_left), recursion(pointer[0].point_right));
			*/
		}
	}

	void arithmetic()
	{
		var = recursion(point_left);
	}


	string var_id;
	double var;
	var_const *point_left;
	var_const *point_right;
};

int input_to_analize(vector<var_const>* input_var_const)  //погоди. как передаётся указатель  на вектор в такой записи? это ясно
{
	

	//"возвращает указатель на первый элемент в векторе" так помимо того, что ты не получаешь сам вектор, так еще и утечки в памяти делаешь т.е. data() не используем? 
	//в данном случае это не то, что надо, я сделал до этого, как должно быть. супер, тогда как правильно получать достув к элементам вектора
	
	string sInput;
	cin >> sInput;


	char* input = (char*)malloc(sInput.size());
	strcpy(input, sInput.c_str());
	
	///новый вариант, пробелы кушает, память динамическая, но пока free не работает и надо как-то учесть, что стирать то нельзя)
	///вот если бы переделать уже с графическим интерфейсом...
	char* error_str = VerifyInput(input);
	if (error_str!=NULL)
		return -1;
	///
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
	input_var_const->reserve(input_size*2);

	cout << "size = " << input_var_const->size() << "   capacity = " << input_var_const->capacity() << endl;
	
	int temp_size_of_vect;
	pDest = varnameDest + 1;
	var_const *k = &input_var_const->at(current_size_of_vect);	//выкинь уже этот 4ewnfjk
	//current_size_of_vect нужно сохранить после всех присвиваний))


	int brakets_counter = 0;
	while (pDest <= endPtr) {

		if (*pDest == '+')
		{
			//операция сложения имеет id "1"	
			temp_size_of_vect = input_var_const->size();
			//если это первая операция в выражении
			if (high_pointer == low_pointer)
			{
				input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //записываем операцию, левый рукав -> на предыдущее число
				input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//левый рукав текущей вычисляемой константы указывает на созданную операцию
				high_pointer = &input_var_const->at(temp_size_of_vect);// верхний указатель -- на созданную операцию
			}
			//если была какая-либо операция до этого
			else
			{
				//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
				if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
				{
					input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //записываем операцию, левый рукав -> на предыдущее число
					high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //правый рукав предыдущей -> на созданную
					high_pointer = &input_var_const->at(temp_size_of_vect);                 //верхний указатель -> на созданную операцию
				}
				//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
				else
				{
					//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //записываем операцию, левый рукав -> на предыдущую операцию

					//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ! приоритета текущей
					if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
					{
						//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию
						input_var_const->push_back(var_const("1", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
						//указываем левым рукавом констаннты на созданную операцию
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


					}

					//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
					//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей
					else
					{
						input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//тут всё неправильно
					}
					high_pointer = &input_var_const->at(temp_size_of_vect);  //верхний указатель на созданную операцию

				}
			}
			pDest++;
		}
		else if (*pDest == '*')
		{                     //операция умножения имеет id "2"		
			temp_size_of_vect = input_var_const->size();
			//если это первая операция в выражении
			if (high_pointer == low_pointer)
			{
				input_var_const->push_back(var_const("2", brakets_counter, low_pointer, NULL));  //записываем операцию, левый рукав -> на предыдущее число
				input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//левый рукав текущей вычисляемой константы указывает на созданную операцию
				high_pointer = &input_var_const->at(temp_size_of_vect);// верхний указатель -- на созданную операцию
			}
			//если была какая-либо операция до этого
			else
			{
				//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
				if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
				{
					input_var_const->push_back(var_const("2", brakets_counter, low_pointer, NULL));  //записываем операцию, левый рукав -> на предыдущее число
					high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //правый рукав предыдущей -> на созданную
					high_pointer = &input_var_const->at(temp_size_of_vect);                 //верхний указатель -> на созданную операцию
				}
				//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
				else
				{
					//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //записываем операцию, левый рукав -> на предыдущую операцию

					//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ! приоритета текущей
					if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
					{
						//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию
						input_var_const->push_back(var_const("2", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
						//указываем левым рукавом констаннты на созданную операцию
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


					}

					//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
					//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей
					else
					{
						input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//тут всё неправильно
					}
					high_pointer = &input_var_const->at(temp_size_of_vect);  //верхний указатель на созданную операцию

				}
			}
			pDest++;
		}
		else if (*pDest == '/')
		{                     //операция деления имеет id "3"
			temp_size_of_vect = input_var_const->size();
			//если это первая операция в выражении
			if (high_pointer == low_pointer)
			{
				input_var_const->push_back(var_const("3", brakets_counter, low_pointer, NULL));  //записываем операцию, левый рукав -> на предыдущее число
				input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//левый рукав текущей вычисляемой константы указывает на созданную операцию
				high_pointer = &input_var_const->at(temp_size_of_vect);// верхний указатель -- на созданную операцию
			}
			//если была какая-либо операция до этого
			else
			{
				//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
				if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
				{
					input_var_const->push_back(var_const("3", brakets_counter, low_pointer, NULL));  //записываем операцию, левый рукав -> на предыдущее число
					high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //правый рукав предыдущей -> на созданную
					high_pointer = &input_var_const->at(temp_size_of_vect);                 //верхний указатель -> на созданную операцию
				}
				//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
				else
				{
					//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //записываем операцию, левый рукав -> на предыдущую операцию

					//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ! приоритета текущей
					if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
					{
						//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию
						input_var_const->push_back(var_const("3", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
						//указываем левым рукавом констаннты на созданную операцию
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


					}

					//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
					//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей
					else
					{
						input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//тут всё неправильно
					}
					high_pointer = &input_var_const->at(temp_size_of_vect);  //верхний указатель -> на созданную операцию

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
				input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //создание элемента класса и запись числа
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
					input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //сначала записываем операцию, левый рукав -> на предыдущее число
					input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//левый рукав текущей вычисляемой константы указывает на созданную операцию
					high_pointer = &input_var_const->at(temp_size_of_vect);// верхний указатель -- на созданную операцию
					temp_size_of_vect = input_var_const->size(); //счётчик прохода по массиву увеличивается ещё раз для записи самого числа
					input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //создание элемента класса и запись числа
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
							input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));//сначала записываем операцию, левый рукав -> на предыдущее число
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);//заполняем предыдущую операцию до конца
							high_pointer = &input_var_const->at(temp_size_of_vect);// верхний указатель -- на созданную операцию
							temp_size_of_vect = input_var_const->size(); //счётчик прохода по массиву увеличивается ещё раз для записи самого числа
							input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //создание элемента класса и запись числа
							//нижний указатель -> на созданное число
							low_pointer = &input_var_const->at(temp_size_of_vect);
							//Правый рукав предшествующей операции на созданное число
				high_pointer[0].point_right = low_pointer;
			}
					//если предыдущая операция не заполнена до конца (правый рукав ни на что не указывает). 
					//Равносильно ситуации н-р: 7*(-2+5), т.е. знак минус стоит сразу после скобки
			else
			{
							input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //создание элемента класса и запись числа
							low_pointer = &input_var_const->at(temp_size_of_vect);//нижний указатель -> на созданное число
							//Правый рукав предшествующей операции на созданное число. В данном случае может быть бессмысленное действие
				high_pointer[0].point_right = low_pointer;
			}
					//если операция вычитания стоит в записи после операции, стоящей внутри одних и тех же скобок (или вообще без них)
			else
			{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
			{
							input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //записываем операцию, левый рукав -> на предыдущее число
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //правый рукав предыдущей -> на созданную
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //верхний указатель -> на созданную операцию
			}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
			else
			{
							//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //записываем операцию, левый рукав -> на предыдущую операцию

							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !БОЛЬШЕ! приоритета текущей
							if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
			{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию
								input_var_const->push_back(var_const("1", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
								//указываем левым рукавом констаннты на созданную операцию
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


			}

							//если приоритет операции с наименьшим приоритетом (на которую указывает левый рукав констанны)  !МЕНЬШЕ! приоритета текущей, 
							//т.е. операция приоритено ГДЕ-ТО между последней и наилегчайшей
			else
			{
								input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//тут всё неправильно
			}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //верхний указатель на созданную операцию			
			}
						temp_size_of_vect = input_var_const->size(); //счётчик прохода по массиву увеличивается ещё раз для записи самого числа
						input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //создание элемента класса и запись числа
						//нижний указатель -> на созданное число
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//Правый рукав предшествующей операции на созданное число
				high_pointer[0].point_right = low_pointer;
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
			temp_size_of_vect = input_var_const->size();	//3? когда размер уже 4
			input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //создание элемента класса и запись числа
			// если данное число первое (и возможно единственное) в записи выражения
			if ((high_pointer == NULL) && (low_pointer == NULL))
						{
				//оба указателя -> на число, тебуется для проверки условия при записи операции
				high_pointer = &input_var_const->at(temp_size_of_vect);
				low_pointer = &input_var_const->at(temp_size_of_vect);
				//левый рукав вычисляемой константы -> созданную структуру с числом.
				input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
						}
			// если данное число не первое в записи (была какая-либо операция)
						else
						{
				//нижний указатель -> на созданное число
				low_pointer = &input_var_const->at(temp_size_of_vect);
				//Правый рукав предшествующей операции на созданное число
						high_pointer[0].point_right = low_pointer;
					}
		}
	}

	input_var_const->at(current_size_of_vect).arithmetic();
	cout << input_var_const->at(current_size_of_vect).var_id << " = " << input_var_const->at(current_size_of_vect).var << endl;

	return 0;
}




int main()
{
	//верни все как было, или переделай под новый вариант. почитай документацию по vector
	//vector<var_const> general_var_const(1);
	//general_var_const.reserve(30);

	//cout << "Size: " << general_var_const.capacity() << "  " << sizeof(general_var_const) << endl;

	var_const pi = var_const("pi", 3.1415926535897932384626433832);
	var_const e = var_const("e", 2.71828182846);

	//general_var_const[0].copy(pi);
	//general_var_const[1].copy(e);

	//general_var_const.push_back(pi);		
	//general_var_const.push_back(e);
	vector<var_const> general_var_const = { pi,e };	//гешефт
													//размер впринципе не нужен тут
	int check = input_to_analize(&general_var_const); //что надо: передавать указатель на какой-то массив данных типа var_const, так, что бы массив можно было динамически обрабатывать в пределах каждой подпрограммы, куда попадаем. вектор он или нет - значения не имееет, я думал что можно будет по указателю выполнять с ним операции и методы из описания вектора. память там выделять, стирать и добавлять .ща

	getchar();	//eat endl
	getchar();	//press any key to exit

	return 0;


}

