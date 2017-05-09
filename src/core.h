///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
//#include <thread>
//#include <mutex>
#include "classes.h"
#include "error.h"
#include "IO.h"
#include "filters.h"


namespace Project {
	namespace Core {

		using namespace std;
		using namespace Project;
		using namespace Project::IO;

		//типы обЪектов	(type)	
		wstring cnst = L"const";
		wstring funct = L"funct";
		wstring varbl = L"varbl";
		wstring equat = L"equat";
		wstring exprs = L"exprs";
		wstring numbr = L"numbr";
		wstring addit = L"oper+";
		wstring mltpl = L"oper*";
		wstring divis = L"oper/";
		wstring power = L"oper^";
		//свойства обЪектов (prop)		
		wstring defnd = L"defnd";//общее свойство
		wstring undef = L"undef";//общее свойство

		wstring arg_c = L"arg_c"; //функция с константными аргументами
		wstring arg_v = L"arg_v"; //функция с переменными аргументами 
		wstring empty = L"empty"; //"пустая" функция. Свойство именных неинициализированных функций

		wstring fundm = L"fundm";//Фундаментальная константа

		wstring unslv = L"unslv";//свойство уравнений и функций от конст выражений
		wstring solvd = L"solvd";//свойство уравнений и функций от конст выражений

		wstring real = L"real_";//свойства чисел
		wstring cmplx = L"cmplx";//свойства чисел

		wstring servc = L"servc";//свойство служебных объектов
		//действия над объектами (actn)
		wstring solve = L"solve";
		wstring write = L"write";
		wstring read = L"read_";
		//wstring arg = L"argum";

		//структура хранения данных
		vector<math_obj*>* general_var_const;

		math_obj pi = math_obj(L"pi", cnst, fundm, L"0", 3.1415926535897932384626433832);
		math_obj e = math_obj(L"e", cnst, fundm, L"0", 2.7182818284590452353602874713527);
		math_obj i = math_obj(L"i", cnst, fundm, L"0", 0);
		//служебная переменная для заполнения "пустых" функций, просто висит тут, без добавления в массив
		math_obj temporary_variable = math_obj(L"temporary_variable", varbl, servc, L"0", 0);






		//Инициализация массива констант, переменных, функций и уравнений
		void Init() {
			ProjectError::Init();
			math_obj* temp;
			general_var_const = new vector<math_obj*>;
			general_var_const->push_back(&pi);
			general_var_const->push_back(&e);
			general_var_const->push_back(&i);
			//минус
			{
				general_var_const->push_back(new math_obj(L"minus", funct, empty, L"", 1, new math_obj(&temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
				temp->point_left = new math_obj(L"", mltpl, L"", L"", 0, new math_obj(L"0", -1), temp->point_collar, temp);
			}

			//корень (пока квадратный)
			{
				general_var_const->push_back(new math_obj(L"root", funct, empty, L"", 0, new math_obj(&temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
			}
			//модуль (корень из квадрата числа)
			{
				general_var_const->push_back(new math_obj(L"abs", funct, empty, L"", 0, new math_obj(&temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new math_obj(temp);
				general_var_const->back()->point_left->point_collar->point_collar = general_var_const->back()->point_left;
				general_var_const->back()->point_left->point_right = new math_obj(L"", power, L"", L"", 0, general_var_const->back()->point_collar, new math_obj(L"0", 2), NULL);
			}
			//знак
			{
				temp = general_var_const->back();
				general_var_const->push_back(new math_obj(L"sgn", funct, empty, L"", 0, new math_obj(&temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new math_obj(L"", divis, L"", L"", 0, general_var_const->back()->point_collar, new math_obj(temp), general_var_const->back());
				general_var_const->back()->point_left->point_right->point_right = general_var_const->back()->point_collar;
				general_var_const->back()->point_left->point_right->point_collar = general_var_const->back()->point_collar;
			}

		}

		/*void testfunc(mutex &mut) {
			mut.lock();
			MessageBoxA(0,"testfunc","tThread",0);
		}*/

		//функция проходит по вектору general_var_const и ищет объект с нужным именем
		math_obj* run_vector(wstring _pv)
		{
			size_t temp_size_of_vect = general_var_const->size();
			int count;
			for (count = 0; count < temp_size_of_vect; count++)
			{
				try {
					int iPv = _pv.length();
					wstring wstr = general_var_const->at(count)->name;	//тут вылет
					int iVect = wstr.length();
					if (iPv == iVect)
						if (_pv.compare(general_var_const->at(count)->name) == 0)
							return general_var_const->at(count);
				}
				catch (exception e) {	//выдает string too long кароч

				}
			}
			if (count == temp_size_of_vect)
			{
				return NULL;
			}
		}



		math_obj* filling_vector(wchar_t* strPtr, wchar_t*ePtr, math_obj* c_e, int brakets)
		{
			wchar_t* pDest = strPtr;
			wchar_t* endPtr = ePtr;
			unsigned int count, comma, count_var;
			math_obj *high_pointer = NULL; //последняя записанная операция
			math_obj *low_pointer = NULL; //последняя записанная константа/переменная/ф-ция/выражение
			math_obj *temp_pointer = NULL;
			math_obj *multiple_var = NULL;
			wchar_t* temp;
			wstring name;
			math_obj* current_element = c_e;
			double num;
			int brakets_counter = 0;
			while (pDest <= endPtr)
			{

				if (*pDest == '=')
				{
					//если равно стоит в самом начале строки
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::EQUALY_FIRST);
						return NULL;
					}
					//равно в конце строки, запрос на действие
					else if (pDest == endPtr)
					{
						//просто выдаём наверх текущий заполненный элемент с параметром solve, а там уже пытаемся его расчитать и вывести результат или сообщить ошибку
						if (current_element->actn != write)
							current_element->actn = solve;
						else
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
							return NULL;
						}
					}
					else
					{
						if (current_element->type == cnst)
						{
							temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer = NULL)
								return temp_pointer;
							if (temp_pointer->type == funct)
							{
								current_element->define_funct(temp_pointer);
								current_element->type = funct;
								current_element->prop = defnd;
								current_element->actn = write;
								delete temp_pointer;
							}
							else if (temp_pointer->type == varbl)
							{
								if (temp_pointer->prop == undef)
								{
									temp_pointer->prop = defnd;
									general_var_const->push_back(temp_pointer);
								}
								current_element->point_collar = temp_pointer;
								current_element->point_collar->point_collar = current_element;
								current_element->point_collar->var = 0;
								current_element->point_collar->point_left = current_element->point_collar;
								current_element->point_collar->point_right = current_element->point_collar;
								current_element->point_left = current_element->point_collar;
								current_element->var = 1;
								current_element->type = funct;
								current_element->prop = defnd;
								current_element->actn = write;
							}
							else if (temp_pointer->type == exprs)
							{
								//тут любопытно получилось. Если запись вида с=с+1, где с была записана как константа, программа сначала посчитает со старым значением с выражение, а потом - запишет вместо c.
								current_element->point_left = temp_pointer->point_left;
								current_element->prop = defnd;
								current_element->actn = write;
								delete temp_pointer;
							}
							else if (temp_pointer->type == cnst)
							{
								current_element->var = temp_pointer->var;
								current_element->type = defnd;
								current_element->actn = write;
								delete temp_pointer;
							}
						}
						else if (current_element->type == varbl)
						{
							temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer = NULL)
								return temp_pointer;
							//тут происходит переопределение переменых в качестве функций. Могут возникнуть проблемы, т.к. появятся два разных по типу объекта с одним именем.
							if (temp_pointer->type == funct)
							{
								low_pointer = temp_pointer->find_varbl(current_element);
								if (low_pointer == NULL)
								{
									//если переменная слева от равно не появляется справа - переопределение переменной в функцию. н-р : y=2*x+1
									current_element->define_funct(temp_pointer);
									current_element->type = funct;
									current_element->prop = defnd;
									current_element->actn = write;
									delete temp_pointer;
								}
								else
								{
									delete current_element;
									//если переменная слева от равно появляется так же справа - уравнение. н-р : x=2*x+1
									current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, low_pointer, temp_pointer, temp_pointer->point_collar);
								}
							}
							else if (temp_pointer->type == varbl)
							{
								if (temp_pointer->name == current_element->name)
								{
									//вид выаражения х=х, где х - переменная. Не знаю что это может значить и кто подобное напишет
								}
								else
								{
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(temp_pointer);
									}
									current_element->point_collar = temp_pointer;
									current_element->point_collar->point_collar = current_element;
									current_element->point_collar->var = 0;
									current_element->point_collar->point_left = current_element->point_collar;
									current_element->point_collar->point_right = current_element->point_collar;
									current_element->point_left = current_element->point_collar;
									current_element->var = 1;
									current_element->type = funct;
									current_element->prop = defnd;
									current_element->actn = write;
								}
							}
							else if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
							{
								/*вообще подобное переопределение переменной в константу возможно только если эта переменная не используется ни в одной функции/уравнении.
								Поскольку на данном этапе объявление переменных как отдельных объектов (не в составе функций) невозможно - данная запись будет считаться ошибкой.
								*/
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::VARBL_BLOCKED);
								return NULL;
							}

						}
						else if (current_element->type == funct)
						{
							if (current_element->prop == defnd)
							{
								temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
								if (temp_pointer = NULL)
									return temp_pointer;
								//случай записи f=... где f - заранее определённая функция.
								//полагаем, что если после равно находится функция (неважно от каких переменных) - переопределение f новым выражением (с новыми переменными)
								//если после равно константа или выражение - уравнение вида f(x) = const
								if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
								{
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, high_pointer->var, high_pointer, temp_pointer, high_pointer->point_collar);
								}
								else if (temp_pointer->type == varbl)
								{
									temp_pointer->var = 0;
									temp_pointer->point_left = temp_pointer;
									temp_pointer->point_right = temp_pointer;
									temp_pointer->point_collar = current_element;
									current_element->point_collar = temp_pointer;
									current_element->point_left = current_element->point_collar;
									current_element->point_right = NULL;
									current_element->var = 1;
								}
								else if (temp_pointer->type == funct)
								{
									if (temp_pointer->prop == arg_c)
									{
										high_pointer = current_element;
										current_element = new math_obj(L"", equat, unslv, write, high_pointer->var, high_pointer, temp_pointer, high_pointer->point_collar);
									}
									else
									{
										current_element->define_funct(temp_pointer);
										current_element->actn = write;
									}
								}
							}
							else if (current_element->prop == undef)
							{
								if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
								{

								}
								else if (temp_pointer->type == varbl)
								{

								}
								else if (temp_pointer->type == funct)
								{

								}
							}
							else if (current_element->prop == arg_c)
							{
								if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
								{

								}
								else if (temp_pointer->type == varbl)
								{

								}
								else if (temp_pointer->type == funct)
								{

								}
							}
							else if (current_element->prop == arg_v)
							{
								if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
								{

								}
								else if (temp_pointer->type == varbl)
								{

								}
								else if (temp_pointer->type == funct)
								{

								}
							}
















						}
						else if (current_element->type == exprs)
						{
							//слева - конст выражение - справа функция => уравнение
							if (temp_pointer->type == funct)
							{
								current_element->type = equat;
								current_element->point_right = temp_pointer;
								//копия переменной с указателем на функцию
								current_element->point_collar = new math_obj(temp_pointer->point_collar);
								current_element->point_collar->point_collar = current_element;
								current_element->prop = unslv;
							}
							else if (temp_pointer->type == varbl)
							{
								current_element->type = equat;
								current_element->point_right = temp_pointer;
								//тут уже должна вылезать копия переменной
								current_element->point_collar = temp_pointer;
								current_element->point_collar->point_collar = current_element;
								current_element->prop = unslv;
							}
							else if ((temp_pointer->type == cnst) || (temp_pointer->type == exprs))
							{
								//запись вида 2+3=7. Пока не знаю как реагировать на такое
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
								return NULL;
							}
						}

					}
					pDest = endPtr + 1;
				}
				else if (*pDest == '+')
				{
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;

					}
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new math_obj(L"+", addit, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_element->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 1);
								high_pointer->point_right->point_collar = new math_obj(L"+", addit, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=,");
					if (pDest == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*pDest == '*')
				{
					//если это первая операция в выражении
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new math_obj(L"*", mltpl, brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 2))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new math_obj(L"*", mltpl, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 2))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new math_obj(L"*", mltpl, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_element->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 2);
								high_pointer->point_right->point_collar = new math_obj(L"*", mltpl, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=,");
					if (pDest == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*pDest == '/')
				{
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new math_obj(L"/", divis, brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 3))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new math_obj(L"/", divis, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 3))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new math_obj(L"/", divis, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_element->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 3);
								high_pointer->point_right->point_collar = new math_obj(L"/", divis, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=,");
					if (pDest == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*pDest == '^')
				{
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{

						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new math_obj(L"^", power, brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 4))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new math_obj(L"^", power, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 4))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new math_obj(L"^", power, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_element->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 4);
								high_pointer->point_right->point_collar = new math_obj(L"^", power, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=,");
					if (pDest == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*pDest == '-')
				{
					//операция минус понимается как сложение с отрицательным числом					
					//если выражение начинается с минуса (случай когда после минуса идёт число, минус перед скобкой не реализован)
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> константу
						current_element->point_left = new math_obj(general_var_const->at(2));
						current_element->point_left->type = defnd;

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
							current_element->point_left = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, current_element);
							high_pointer = current_element->point_left;
							high_pointer->point_right = new math_obj(general_var_const->at(3));

							high_pointer->point_right->type = defnd;

							low_pointer = high_pointer->point_right;
						}
						//если ранее были другие операции
						else
						{
							//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
							if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
							{
								//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
								high_pointer->point_right = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, high_pointer);
								high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
								high_pointer->point_right = new math_obj(general_var_const->at(2));
								high_pointer->point_right->type = defnd;
								low_pointer = high_pointer->point_right;
							}

							else
							{
								if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
								{
									//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
									high_pointer = new math_obj(L"+", addit, brakets_counter + brakets, current_element->point_left, NULL, current_element);
									//воротник предыдущей легчайшей операции -> на новую операцию
									current_element->point_left->point_collar = high_pointer;
									//указываем левым рукавом константы на созданную операцию
									current_element->point_left = high_pointer;
									high_pointer->point_right = new math_obj(general_var_const->at(2));
									high_pointer->point_right->type = defnd;
									low_pointer = high_pointer->point_right;
								}
								else
								{
									high_pointer = high_pointer->prioritize(brakets_counter + brakets + 1);
									high_pointer->point_right->point_collar = new math_obj(L"+", addit, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
									high_pointer->point_right = high_pointer->point_right->point_collar;
									high_pointer = high_pointer->point_right;
									high_pointer->point_right = new math_obj(general_var_const->at(2));
									high_pointer->point_right->type = defnd;
									low_pointer = high_pointer->point_right;
								}

							}
						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=,");
					if (pDest == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
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
						if (temp == endPtr + 1)
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
							return NULL;
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
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
							return NULL;
						}
					}
					if (pDest + 1 == temp)
					{
						//пустая строка в скобках. вроде не ошибка, но можно и запретить). Вообще в контекте построения дерева это означает запись нуля. Поэтому пока запрещено.
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::EMTY_BRACKETS);
						return NULL;
					}
					else
					{
						name.assign(pDest + 1, temp);
						temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
						if (temp_pointer == NULL)
						{
							return temp_pointer;
						}
						//скобка в самом начале строки
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							if (temp_pointer->type == cnst)
							{
								current_element->point_left = temp_pointer;
								high_pointer = temp_pointer;
								low_pointer = high_pointer;
							}
							else if (temp_pointer->type == exprs)
							{
								//может быть не будет работать правильно
								current_element->point_left = temp_pointer->point_left;
								high_pointer = current_element->point_left;
								low_pointer = current_element->point_left;
								delete temp_pointer;
								temp_pointer = NULL;
							}
							else if (temp_pointer->type == varbl)
							{
								if (current_element->type == exprs)
								{
									current_element->type = funct;
									current_element->var = 1;
									if (temp_pointer->prop == undef)
									{
										temp_pointer->type = defnd;
										general_var_const->push_back(new math_obj(temp_pointer));
									}
									current_element->point_collar = temp_pointer;
									current_element->point_left = current_element->point_collar;
									high_pointer = current_element->point_left;
									low_pointer = high_pointer;
								}
								else if (current_element->type == funct)
								{
									if (current_element->point_collar->point_right != NULL)
									{
										multiple_var = current_element->find_varbl(temp_pointer);
										if (multiple_var == NULL)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											current_element->point_left = multiple_var;
										}
									}
									//так как скобка стоит в самом начале строки, если текущий элемент - функция, её список переменных всегда замкнут
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
										return NULL;
									}
								}
							}
							else if (temp_pointer->type == funct)
							{
								if (temp_pointer->prop == arg_c)
								{
									current_element->point_left = temp_pointer;
									high_pointer = temp_pointer;
									low_pointer = high_pointer;
								}
								else if (current_element->type == exprs)
								{
									current_element->type = funct;
									current_element->prop = undef;
									current_element->var = 1;
									current_element->point_collar = new math_obj(&temporary_variable);
									current_element->point_right = temp_pointer;
									current_element->point_collar->var = 0; //просто для достоверности)
									current_element->point_collar->point_collar = current_element;
									current_element->point_left = current_element->point_collar;
									high_pointer = current_element->point_left;
									low_pointer = high_pointer;
								}
								else if (current_element->type == funct)
								{
									if (current_element->point_collar->point_right != NULL)
									{
										if (current_element->var_list_compare(temp_pointer) != 0)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											current_element->point_left = temp_pointer;
										}
									}
									//так как скобка стоит в самом начале строки, если текущий элемент - функция, её список переменных всегда замкнут
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
										return NULL;
									}
								}
							}
						}

						// вначале строки стоит минус
						else if (low_pointer == high_pointer)
						{
							//перед скобкой стоит минус
							if (high_pointer->name == L"minus")
							{
								if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
								{
									high_pointer->point_right = temp_pointer;
								}
								else if (temp_pointer->type == varbl)
								{
									if (current_element->type == exprs)
									{
										current_element->type = funct;
										current_element->var = 1;
										if (temp_pointer->prop == undef)
										{
											temp_pointer->prop = defnd;
											general_var_const->push_back(new math_obj(temp_pointer));
										}
										current_element->point_collar = temp_pointer;
										high_pointer->point_right = current_element->point_collar;
									}
									else if (current_element->type == funct)
									{
										if (current_element->point_collar->point_right != NULL)
										{
											multiple_var = current_element->find_varbl(temp_pointer);
											if (multiple_var == NULL)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											else
											{
												high_pointer->point_right = multiple_var;
											}
										}
										//так как скобка стоит в самом начале строки, если текущий элемент - функция, её список переменных всегда замкнут
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
											return NULL;
										}
									}
								}
								else if (temp_pointer->type == funct)
								{
									if (temp_pointer->prop == arg_c)
									{
										high_pointer->point_right = temp_pointer;
									}
									else if (current_element->type == exprs)
									{
										current_element->type = funct;
										current_element->prop = undef;
										current_element->var = 1;
										current_element->point_collar = new math_obj(&temporary_variable);
										current_element->point_right = temp_pointer;
										current_element->point_collar->var = 0; //просто для достоверности)
										current_element->point_collar->point_collar = current_element;
										high_pointer->point_right = current_element->point_collar;
									}
									else if (current_element->type == funct)
									{
										if (current_element->point_collar->point_right != NULL)
										{
											if (current_element->var_list_compare(temp_pointer) != 0)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											else
											{
												high_pointer->point_right = temp_pointer;
											}
										}
										//так как скобка стоит в самом начале строки, если текущий элемент - функция, её список переменных всегда замкнут
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
											return NULL;
										}
									}
								}
							}
							//перед скобкой что-то кроме минуса. Перед скобкой может стоять какое-либо число, н-р 5(...) = 5*(...). Пока ошибка. 
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
								return NULL;
							}

						}
						//если скобка стоит не в начале строки - подразумевается наличие какой-либо операции перед ней
						else
						{
							if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
							{
								if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
								{
									high_pointer->point_right->point_right = temp_pointer;
								}
								else
								{
									high_pointer->point_right = temp_pointer;
								}
								low_pointer = high_pointer->point_right;
							}
							else if (temp_pointer->type == varbl)
							{
								if (current_element->type == funct)
								{
									//список переменных замкнут
									multiple_var = current_element->find_varbl(temp_pointer);
									if (current_element->point_collar->point_right != NULL)
									{

										if (multiple_var == NULL)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											//стоит ли перед переменной минус
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
											{
												high_pointer->point_right->point_right = multiple_var;
											}
											else
											{
												high_pointer->point_right = multiple_var;
											}
											low_pointer = high_pointer->point_right;
										}
									}
									//не замкнут
									else
									{
										if (multiple_var == NULL)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->type = defnd;
												general_var_const->push_back(new math_obj(temp_pointer));
											}
											multiple_var = current_element->var_list_back();
											multiple_var->point_left = temp_pointer;
											multiple_var->point_left->point_right = multiple_var;
											multiple_var->point_left->point_collar = current_element;
											multiple_var->point_left->var = current_element->var;
											current_element->var += 1;
											//поскольку символ не в начале строки - значит стоит после какой-либо операции. 
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//стоит после минуса
											{
												high_pointer->point_right->point_right = multiple_var->point_left;
											}
											else
											{
												high_pointer->point_right = multiple_var->point_left;
											}
											low_pointer = high_pointer->point_right;
										}
										else
										{
											//стоит ли перед переменной минус
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
											{
												high_pointer->point_right->point_right = multiple_var;
											}
											else
											{
												high_pointer->point_right = multiple_var;
											}
											low_pointer = high_pointer->point_right;
										}
									}
								}
								else if (current_element->type == exprs)
								{
									current_element->type = funct;
									current_element->var = 1;
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(new math_obj(temp_pointer));
									}
									current_element->point_collar = temp_pointer;
									//учитывается минус перед переменной
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
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
							else if (temp_pointer->type == funct)
							{
								if (temp_pointer->prop == arg_c)
								{
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = temp_pointer;
									}
									else
									{
										high_pointer->point_right = temp_pointer;
									}
									low_pointer = high_pointer->point_right;
								}
								else if (current_element->type == exprs)
								{
									current_element->type = funct;
									current_element->prop = undef;
									current_element->var = 1;
									current_element->point_collar = new math_obj(&temporary_variable);
									current_element->point_right = temp_pointer;
									current_element->point_collar->var = 0; //просто для достоверности)
									current_element->point_collar->point_collar = current_element;

									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = current_element->point_collar;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}
								else if (current_element->type == funct)
								{
									if (current_element->point_collar->point_right != NULL)
									{
										if (current_element->var_list_compare(temp_pointer) != 0)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
											{
												high_pointer->point_right->point_right = temp_pointer;
											}
											else
											{
												high_pointer->point_right = temp_pointer;
											}
											low_pointer = high_pointer->point_right;
										}
									}
									else
									{
										current_element->arg_copy_plus_1();
										current_element->point_right[(int)current_element->var - 1] = temp_pointer;
										temp_pointer = NULL;
										temp_pointer = current_element->var_list_back();
										temp_pointer->point_left = new math_obj(&temporary_variable);
										temp_pointer->point_left->point_right = temp_pointer;
										temp_pointer->point_left->var = current_element->var - 1;

										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
										{
											high_pointer->point_right->point_right = temp_pointer->point_left;
										}
										else
										{
											high_pointer->point_right = temp_pointer->point_left;
										}
										low_pointer = high_pointer->point_right;
									}
								}
							}
						}
					}
					//сразу переходим к следующей позиции после скобки
					brakets_counter -= 4;
					pDest = temp + 1;
					if (*pDest == '(,')
					{
						//по идее запись вида )( может означать )*(. Потом добавть сюда определение для подобного случая
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
						return NULL;
					}
				}
				else if (*pDest == '|')
				{
					//ну как бы модуль. маленькая проблема - в случае со скобками открывающая ( и закрывающая ) - разные символы. 
					//В случае с модулем | - эта палочка - один и тот же символ. Модуль доступен через вызов abs().
				}
				else if ((*pDest == '1') || (*pDest == '2') || (*pDest == '3') || (*pDest == '4') || (*pDest == '5') || (*pDest == '6') || (*pDest == '7') || (*pDest == '8') || (*pDest == '9') || (*pDest == '0'))
				{
					// если данное число первое в записи выражения
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> константу
						current_element->point_left = new math_obj(L"", numbr, real, wcstod(pDest, &pDest), current_element);
						//оба указателя -> на число, тебуется для проверки условия при записи операции
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					//перед числом в начале строки стоит минус
					else if (high_pointer == low_pointer)
					{
						if (high_pointer->name == L"minus")
						{
							high_pointer->point_right = new math_obj(L"", numbr, real, wcstod(pDest, &pDest), low_pointer);
						}
						else
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
							return NULL;
						}
					}
					// если данное число не первое в записи (была какая-либо операция)
					else
					{
						//создание элемента класса и запись числа, воротник -> пред операцию 						
						if ((high_pointer->point_right != NULL) && (low_pointer->name == L"minus"))
						{
							low_pointer->point_right = new math_obj(L"", numbr, real, wcstod(pDest, &pDest), low_pointer);
						}
						else
						{
							high_pointer->point_right = new math_obj(L"", numbr, real, wcstod(pDest, &pDest), high_pointer);
							low_pointer = high_pointer->point_right;
						}
					}
				}

				//случай с символами в строке
				else
				{
					temp = wcspbrk(pDest, L"()+-*^/=");
					//если строчка начинается с cимвола
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						if ((temp == NULL) && (brakets == 0))
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::EQUALY_MISSING);
							return NULL;
						}
						else if (temp == NULL)
						{
							name.assign(pDest, endPtr + 1);
							pDest = endPtr + 1;
						}
						else
						{
							name.assign(pDest, temp);
							pDest = temp;
						}
						high_pointer = run_vector(name);
						//если не найден ни один элемент массива с таким именем
						if (high_pointer == NULL)
						{
							//считаем найденный элемент переменной
							if (temp == NULL)
							{
								current_element->type = varbl;
								current_element->name = name;
								current_element->prop = undef;
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//считаем найденный элемент конст
							else if (*temp == '=')
							{
								current_element->type = cnst;
								current_element->name = name;
								current_element->prop = undef;
								current_element->actn = write;
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//считаем найденный элемент функцией
							else if (*temp == '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
								while (count != 0)
								{
									temp++;
									//если попали в конец строчки не найдя закрывающих скобок
									if (temp == endPtr + 1)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
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
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										//считаем запятые. количество запятых в скобках +1 = число различных переменных
										comma++;
									}
								}
								if (*(temp + 1) != '=')
								{
									//если в самом начале строки стоит новое буквосочетание и дальше скобка - это всегда определение новой функции
									//если после этого нет равно - ошибка
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								//далее ВСЕГДА рассматриваем случай с равно после скобки
								if (pDest + 1 == temp)
								{
									//пустая строка в скобках.  создание функции от неопределённого количества переменных
									//аналогично заданию функции вообще без указания переменных. Указатель на переменные - пустой
									current_element->type = funct;
									current_element->name = name;
									current_element->prop = undef; //считаем новую функцию undef, но при этом мы знаем что у неё есть имя
									current_element->actn = write;  //на заполнение
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								else
								{
									//запоминаем число переменных
									current_element->var = comma + 1;
									current_element->type = funct;
									current_element->name = name;
									current_element->prop = undef; //считаем новую функцию undef, но при этом мы знаем что у неё есть имя
									current_element->actn = write;  //на заполнение
									for (count = 0; count < comma + 1; count++)
									{
										temp = wcspbrk(pDest + 1, L",)");
										name.assign(pDest + 1, temp);
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new math_obj(temp_pointer));
											}
											temp_pointer->point_collar = current_element;
											temp_pointer->var = count; //тут происходит запись номера переменной в массиве внутрь самой переменной. подразумевается что переменные НЕ имеют установленного значения => можно использовать var как счётчик

											if (count == 0)
												//сразу же получаем уазатель на первую переменную в списке
												current_element->point_collar = temp_pointer;
											else
											{
												//создание двунаправленного списка переменных, где на нулевой элемент указывает воротник функции.
												temp_pointer->point_right = current_element->var_list_back();
												temp_pointer->point_right->point_left = temp_pointer;
											}
											if (count == comma)
											{
												//создание замкнутого в кольцо двунаправленного списка переменных. Нулевой элемент = последнему.
												temp_pointer->point_left = current_element->point_collar;
												current_element->point_collar->point_right = temp_pointer;
											}
											//temp_pointer в данном случае удалять нельзя. Можно занулить, но удалять нельзя
										}
										else
										{
											//поскольку функция ещё неопределённая передавать ей любые аргументы будет неверно
											//TODO:чистить мусор перед уходом
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										pDest = temp;
									}
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								brakets_counter -= 4;
								pDest = temp + 1;
							}
							else if (*temp == ')')
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//считаем найденный элемент переменной, а текущее выражение - функцией
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->type = funct;
								current_element->prop = undef;
								//запоминаем число переменных
								current_element->var = 1;
								general_var_const->push_back(new math_obj(name, varbl, defnd, 0));
								//копия переменной с указателем на функцию
								current_element->point_collar = new math_obj(general_var_const->back());
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
						//найден элемент массива с совпадающим именем - константа
						else if (high_pointer->type == cnst)
						{
							//копируем константу
							if ((temp == NULL) || (*temp == '='))
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//тут идея в том, что если уже есть какая-то константа - какие бы ни были аргументы для неё - ответом будет являться константа.
							//однако данная запись может означать переопределение конст в виде функции
							else if (*temp == '(')
							{
								//тогда сразу лучше прояснить что именно тут происходит,
								//заглядываем в скобки и смотрим что там
								//current_element->copy(high_pointer);
								brakets_counter += 4;
								count = 1;
								comma = 0;
								while (count != 0)
								{
									temp++;
									//если попали в конец строчки не найдя закрывающих скобок
									if (temp == endPtr + 1)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
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
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										//считаем запятые. количество запятых в скобках +1 = число различных переменных
										comma++;
									}
								}
								if (*(temp + 1) != '=') //два варианта - стоит равно после скобки или нет
								{
									//если его нет, то данная запись в любом случае равносильна просто вызову константы, какие бы аргументы ей не передавались
									//копируем константу
									if (*(temp + 1) == NULL) // если мы вышли в конец строки - то ноль - т.е. после скобки отсутствуют символы
									{
										current_element->copy(high_pointer);
										high_pointer = current_element;
										low_pointer = high_pointer;
									}
									else if ((*(temp + 1) == '+') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^') || (*(temp + 1) == '-'))
									{
										current_element->point_left = new math_obj(high_pointer);
										current_element->point_left->point_collar = current_element;
										high_pointer = current_element->point_left;
										low_pointer = high_pointer;
									}
									/*else if ((*(temp + 1) == '('))
									{
										//пока оставлю зто условие. Запись типа с(...)(...) - обычно это означает произведение с()*(...)
									}*/
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
										return NULL;
									}
								}
								//далее ВСЕГДА рассматриваем случай с равно после скобки
								else if (pDest + 1 == temp)
								{
									// тут либо вывод значения, либо новое определение c()=
									current_element->copy(high_pointer);
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								//тут надо только проверить, чтобы аргументы в скобках были переменными
								else
								{
									current_element->copy(high_pointer);
									for (count = 0; count < comma + 1; count++)
									{
										temp = wcspbrk(pDest + 1, L",)");
										name.assign(pDest + 1, temp);
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new math_obj(temp_pointer));
											}
											temp_pointer->point_collar = current_element;
											temp_pointer->var = count; //тут происходит запись номера переменной в массиве внутрь самой переменной. подразумевается что переменные НЕ имеют установленного значения => можно использовать var как счётчик

											if (count == 0)
												//сразу же получаем уазатель на первую переменную в списке
												current_element->point_collar = temp_pointer;
											else
											{
												//создание двунаправленного списка переменных, где на нулевой элемент указывает воротник функции.
												temp_pointer->point_right = current_element->var_list_back();
												temp_pointer->point_right->point_left = temp_pointer;
											}
											if (count == comma)
											{
												//создание замкнутого в кольцо двунаправленного списка переменных. Нулевой элемент = последнему.
												temp_pointer->point_left = current_element->point_collar;
												current_element->point_collar->point_right = temp_pointer;
											}
											//temp_pointer в данном случае удалять нельзя. Можно занулить, но удалять нельзя
										}
										else
										{
											//TODO:в случае этой ветки чистить список уже заполненных переменных
											break;
										}
										pDest = temp;
									}

									if ((count == comma + 1) && (high_pointer->prop != fundm))//за искл фундаментальных констант
									{
										//запоминаем число переменных
										current_element->var = comma + 1;
										current_element->type = funct;
										current_element->prop = undef; //считаем новую функцию undef, но при этом мы знаем что у неё есть имя
										current_element->actn = write; //на запись										
										low_pointer = high_pointer;
									}
									else
									{
										// тут либо вывод значения, либо новое определение, игнорируя аргументы
										//тут ещё может означать умножение с(...) = с*(...). К примеру запрос на расчёт с*(...)= 
										//однако в данном случае это умножение на вектор - что не реализовано, возможно стоит как-то отдельно делать	
										high_pointer = current_element;
										low_pointer = high_pointer;
									}
								}
								brakets_counter -= 4;
								pDest = temp + 1;
							}
							else if (*temp == ')')
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//текущий элемент остаётся выражением, найденная константа (её копия) - один из операндов
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->point_left = new math_obj(high_pointer);
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
						//найден элемент массива с совпадающим именем - переменная
						else if (high_pointer->type == varbl)
						{
							//копируем переменную
							if ((temp == NULL) || (*temp == '='))
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;

							}
							else if (*temp == ')')
							{

								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//такая запись может означать ТОЛЬКО переопределение переменной в качестве функции нового аргумента
							else if (*temp == '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
								while (count != 0)
								{
									temp++;
									//если попали в конец строчки не найдя закрывающих скобок
									if (temp == endPtr + 1)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
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
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										//считаем запятые. количество запятых в скобках +1 = число различных переменных
										comma++;
									}
								}
								if (*(temp + 1) != '=')
								{
									//если в самом начале строки стоит переменная и далее скобка - считаем что это определение новой функции
									//если после этого нет равно - ошибка
									//так же - может быть умножение вида x(...) что значит x*(...)
									//однако не стоит забывать о неявном определении функций. Может она тут через уравнение задаётся
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								//далее ВСЕГДА рассматриваем случай с равно после скобки
								else if (pDest + 1 == temp)
								{
									//пустая строка в скобках. получается, наверное создание функции от неопределённого количества переменных
									//аналогично заданию функции вообще без указания переменных. Указатель на переменные - пустой
									current_element->copy(high_pointer);
									current_element->type = funct;
									current_element->prop = undef; //считаем новую функцию undef, но при этом мы знаем что у неё есть имя		
									current_element->actn = write; //на запись
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								else
								{
									current_element->copy(high_pointer);
									for (count = 0; count < comma + 1; count++)
									{
										temp = wcspbrk(pDest + 1, L",)");
										name.assign(pDest + 1, temp);
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new math_obj(temp_pointer));
											}
											temp_pointer->point_collar = current_element;
											temp_pointer->var = count; //тут происходит запись номера переменной в массиве внутрь самой переменной. подразумевается что переменные НЕ имеют установленного значения => можно использовать var как счётчик

											if (count == 0)
												//сразу же получаем уазатель на первую переменную в списке
												current_element->point_collar = temp_pointer;
											else
											{
												//создание двунаправленного списка переменных, где на нулевой элемент указывает воротник функции.
												temp_pointer->point_right = current_element->var_list_back();
												temp_pointer->point_right->point_left = temp_pointer;
											}
											if (count == comma)
											{
												//создание замкнутого в кольцо двунаправленного списка переменных. Нулевой элемент = последнему.
												temp_pointer->point_left = current_element->point_collar;
												current_element->point_collar->point_right = temp_pointer;
											}
											//temp_pointer в данном случае удалять нельзя. Можно занулить, но удалять нельзя
										}
										else
										{
											//поскольку функция ещё неопределённая передавать ей любые аргументы будет неверно
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										pDest = temp;
									}
									//запоминаем число переменных
									current_element->var = comma + 1;
									current_element->type = funct;
									current_element->prop = undef; //считаем новую функцию undef, но при этом мы знаем что у неё есть имя
									current_element->actn = write; //на запись	
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								brakets_counter -= 4;
								pDest = temp + 1;
							}
							//текущий элемент => функция, аргументом является копия переменной
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->type = funct;
								current_element->prop = undef;
								//запоминаем число переменных
								current_element->var = 1;
								//копия переменной с указателем на функцию
								current_element->point_collar = new math_obj(high_pointer);
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
						//найден элемент массива с совпадающим именем - функция
						else if (high_pointer->type == funct)
						{
							//копируем функцию
							if ((temp == NULL) || (*temp == '='))
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;
								//данная функция может быть использована в каком-то уравнении/другой функции в качестве операнда. 
								//может так же означать её переопределение с неизвестным количеством переменных
							}
							//в данном случае открытая скобка после названия уже определённой ф-ции (н-р f(x)) может значить:
							//--новое определение, в том числе от новых переменных
							//--передачу функции аргументов, как константных выражений, так и новых переменных. В этом случае считаем, 
							//что подразумевается f(t)= - запрос на вывод выражения от t, либо участие f в уравнении с переменной t, за искл. случая с переопределением.
							//--передачу функции в качестве аргумента новую функцию g(s) - при этом получившаяся функция f(g(s)) должна всюду зависеть от s, а не от х,
							//однако должна состоять из копий оригинальных функций, исключение g(s) - неявно задана - f(5*s+2)
							else if (*temp == '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
								while (count != 0)
								{
									temp++;
									//если попали в конец строчки не найдя закрывающих скобок
									if (temp == endPtr + 1)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
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
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										//считаем запятые. количество запятых в скобках +1 = число различных переменных
										comma++;
									}
								}
								if (pDest + 1 == temp)
								{
									//пустая строка в скобках. получается, либо определение функции от неопределённого количества переменных или просто использование функции в выражении
									//аналогично заданию функции вообще без указания переменных. Указатель на переменные - тот что был раньше
									if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
									{
										current_element->copy(high_pointer);
										high_pointer = current_element;
										low_pointer = high_pointer;
									}
									//случай, аналогичный последнему условию в данной ветке 
									else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
									{
										current_element->type = funct;
										current_element->prop = arg_v;
										current_element->var = 1; // одна переменная
										current_element->point_right = new math_obj(high_pointer); //найденная функция - аргумент текущей
										//копия служебной переменной с указателем на функцию
										current_element->point_collar = new math_obj(&temporary_variable);
										current_element->point_collar->var = 0; //просто для достоверности)
										current_element->point_collar->point_collar = current_element;
										current_element->point_left = current_element->point_collar;

										high_pointer = current_element->point_left;
										low_pointer = high_pointer;
									}
								}
								//число аргументов в скобках совпадает с числом аргументов у определённой заранее функции
								else if (high_pointer->var == comma + 1)
								{
									//нет запятых 
									if (comma == 0)
									{
										name.assign(pDest + 1, temp);
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if ((temp_pointer->type == cnst) || (temp_pointer->type == exprs))
										{
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												current_element->point_right = temp_pointer;
												current_element->prop = arg_c; //указываем что в функцию передали аргумент
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->point_left = new math_obj(high_pointer);
												current_element->point_left->point_right = temp_pointer;
												current_element->point_left->prop = arg_c;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
										else if (temp_pointer->type == funct)
										{
											if (temp_pointer->prop == arg_c)
											{
												if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
												{
													current_element->copy(high_pointer);
													current_element->point_right = temp_pointer;
													current_element->prop = arg_c; //указываем что в функцию передали аргумент
													high_pointer = current_element;
													low_pointer = high_pointer;
												}
												else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													current_element->point_left = new math_obj(high_pointer);
													current_element->point_left->point_right = temp_pointer;
													current_element->point_left->prop = arg_c;
													high_pointer = current_element->point_left;
													low_pointer = high_pointer;
												}
											}
											else
											{
												if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
												{
													current_element->copy(high_pointer);
													current_element->point_right = temp_pointer;
													current_element->prop = arg_v; //указываем что в функцию передали аргумент
													high_pointer = current_element;
													low_pointer = high_pointer;
												}
												else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													current_element->type = funct;
													current_element->prop = arg_v;
													current_element->var = 1; // одна переменная
													current_element->point_right = new math_obj(high_pointer); //найденная функция - аргумент текущей
																												//копия служебной переменной с указателем на функцию
													current_element->point_collar = new math_obj(&temporary_variable);
													current_element->point_collar->var = 0; //просто для достоверности)
													current_element->point_collar->point_collar = current_element;
													current_element->point_left = current_element->point_collar;
													current_element->point_right->point_right = temp_pointer;
													current_element->point_right->prop = arg_v;
													high_pointer = current_element->point_left;
													low_pointer = high_pointer;
												}
											}
										}
										else if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new math_obj(temp_pointer));
											}
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												current_element->point_right = temp_pointer;
												current_element->prop = arg_v; //указываем что в функцию передали аргумент
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->type = funct;
												current_element->prop = arg_v;
												current_element->var = 1; // одна переменная
												current_element->point_right = new math_obj(high_pointer); //найденная функция - аргумент текущей
																											//копия служебной переменной с указателем на функцию
												current_element->point_collar = new math_obj(&temporary_variable);
												current_element->point_collar->var = 0; //просто для достоверности)
												current_element->point_collar->point_collar = current_element;
												current_element->point_left = current_element->point_collar;
												current_element->point_right->point_right = temp_pointer;
												current_element->point_right->prop = arg_v;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
									}
									//если в скобках стоят запятые - определение функции нескольких переменных
									else
									{
										multiple_var = new math_obj[comma + 1];
										count_var = 0;
										for (count = 0; count < comma + 1; count++)
										{
											temp = wcspbrk(pDest + 1, L",)");
											name.assign(pDest + 1, temp);
											temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
											if (temp_pointer == NULL)
												return temp_pointer;
											//заполняем массив аргументов
											if (temp_pointer->type == varbl)
											{
												count_var++;
												if (temp_pointer->prop == undef)
												{
													temp_pointer->prop = defnd;
													general_var_const->push_back(new math_obj(temp_pointer));
												}
											}
											else if (temp_pointer->type == funct)
											{
												if (temp_pointer->prop != arg_c)
												{
													count_var++;
												}
											}
											multiple_var[count].copy(temp_pointer);
											pDest = temp;
										}
										if (count_var != 0)//если среди аргументов хотя бы одна переменная
										{
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												current_element->point_right = multiple_var;
												current_element->prop = arg_v; //указываем что в функцию передали аргумент
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->type = funct;
												current_element->prop = arg_v;
												current_element->var = 1; // одна переменная
												current_element->point_right = new math_obj(high_pointer); //найденная функция - аргумент текущей
																											//копия служебной переменной с указателем на функцию
												current_element->point_collar = new math_obj(&temporary_variable);
												current_element->point_collar->var = 0; //просто для достоверности)
												current_element->point_collar->point_collar = current_element;
												current_element->point_left = current_element->point_collar;
												current_element->point_right->point_right = multiple_var;
												current_element->point_right->prop = arg_v;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
										else // все аргументы не переменные
										{
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												current_element->point_right = multiple_var;//в качестве аргумента - массив объектов
												current_element->prop = arg_c; //указываем что в функцию передали аргумент
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->point_left = new math_obj(high_pointer);
												current_element->point_left->point_right = multiple_var;
												current_element->point_left->prop = arg_c;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
									}
								}
								//число переменных - различное 
								else
								{
									if (*(temp + 1) != '=')
									{
										//тут ошибка: ранее была f(x,y), записали f(k)+... - количество переменных в вызываемой функции не совпадает
										//TODO:здесь надо будет написать исключения для функций нескольких аргументов, которые могут пониматься как функция одного аргумента
										//пример root(5) - квадратный корень из 5, root (5,2) - тоже самое, root (5,3) - кубический корень из 5
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;

									}
									//но если далее стоит равно - всё нормально,  переопределение. 
									//TODO:При этом необходимо очищать предыдущее дерево операций, только не здесь, а далее при переходе через равно, т.к. это ещё может быть ошибка ввода.	
									//так же, в случае ошибки ввода - работать всё-таки с копией функции, т.к. дерево опреций ещё на месте, а вот переменные уже пропали)

									else
									{
										current_element->copy(high_pointer);
										for (count = 0; count < comma + 1; count++)
										{
											temp = wcspbrk(pDest + 1, L",)");
											name.assign(pDest + 1, temp);
											temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
											if (temp_pointer == NULL)
												return temp_pointer;
											if (temp_pointer->type == varbl)
											{
												if (temp_pointer->prop == undef)
												{
													temp_pointer->prop = defnd;
													general_var_const->push_back(new math_obj(temp_pointer));
												}
												temp_pointer->point_collar = current_element;
												temp_pointer->var = count; //тут происходит запись номера переменной в массиве внутрь самой переменной. подразумевается что переменные НЕ имеют установленного значения => можно использовать var как счётчик

												if (count == 0)
													//сразу же получаем уазатель на первую переменную в списке
													current_element->point_collar = temp_pointer;
												else
												{
													//создание двунаправленного списка переменных, где на нулевой элемент указывает воротник функции.
													temp_pointer->point_right = current_element->var_list_back();
													temp_pointer->point_right->point_left = temp_pointer;
												}
												if (count == comma)
												{
													//создание замкнутого в кольцо двунаправленного списка переменных. Нулевой элемент = последнему.
													temp_pointer->point_left = current_element->point_collar;
													current_element->point_collar->point_right = temp_pointer;
												}
												//temp_pointer в данном случае удалять нельзя. Можно занулить, но удалять нельзя
											}
											else
											{
												//поскольку функция ещё неопределённая передавать ей любые аргументы будет неверно, т.к. количество аргументов не совпадает
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
												return NULL;
											}
											pDest = temp;
										}
										//запоминаем число переменных
										current_element->var = comma + 1;
										current_element->prop = undef; //считаем новую функцию undef, но при этом мы знаем что у неё есть имя
										current_element->actn = write;  //на заполнение		
										high_pointer = current_element;
										low_pointer = high_pointer;
									}

								}
								brakets_counter -= 4;
								pDest = temp + 1;
							}
							else if (*temp == ')')
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//текущее выражение => функция, копия найденной функции - аргумент выражения.
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->type = funct;
								current_element->prop = arg_v;
								current_element->point_right = new math_obj(high_pointer); //полагаем новую неопределённую функцию, которая зависит от какой-либо переменной,
																							//имеющей аргумент в виде другой (определённой) функции. При этом если окажется, что
																							//в выражении участвуют несколько функций (f+g), то функция, котрая их объединяет
																							//будет считаться функцией нескольких переменных, 
																							//даже если все функции в её выражении зависят от одной и той же переменной.
								//копия служебной переменной с указателем на функцию, которая иемеет номер ноль
								current_element->point_collar = new math_obj(&temporary_variable);
								current_element->point_collar->var = 0; //просто для достоверности)
								current_element->point_collar->point_collar = current_element;
								current_element->point_left = current_element->point_collar;//т.о. каждая функция будет иметь одну и туже структуру и пониматься всегда, 
																							//как функция зависящая от переменных, которой в качетве аргументов что-то передаётся
								current_element->var = 1; //количество переменных								
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
					}
					//символ находится не в начале строки
					else
					{
						if (temp == NULL)
						{
							name.assign(pDest, endPtr + 1);
							pDest = endPtr + 1;
						}
						else
						{
							name.assign(pDest, temp);
							pDest = temp;
						}
						low_pointer = run_vector(name);

						//далее всегда может быть только два варианта - текщуий элемент либо функция, либо выражение, причём всегда undef.
						//если не найден ни один элемент массива с таким именем
						if (low_pointer == NULL)
						{
							if (*temp == '(),')
							{
								//ошибка - после переменной стоит открывающаяся скобка. Вообще не совсем бессмысленная запись, возможно нужен другой код ошибки или придумать как такое исключение обрабатывать.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//если текущий элемент - функция, то очевидно найдена новая переменная в записи выражения. 


							if (current_element->type == funct)
							{
								if (current_element->point_collar->point_right != NULL)
								{
									//значит список переменных замкнут => новая переменная - лишняя
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
									return NULL;
								}
								//список не замкнут - можно добавить больше переменных
								else
								{
									general_var_const->push_back(new math_obj(name, varbl, defnd, 0));
									temp_pointer = current_element->var_list_back();
									temp_pointer->point_left = new math_obj(general_var_const->back());
									temp_pointer->point_left->point_right = temp_pointer;
									temp_pointer->point_left->point_collar = current_element;
									temp_pointer->point_left->var = current_element->var;
									current_element->var += 1;
									//поскольку символ не в начале строки - значит стоит после какой-либо операции. 
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//стоит после минуса
									{
										high_pointer->point_right->point_right = temp_pointer->point_left;
									}
									else
									{
										high_pointer->point_right = temp_pointer->point_left;
									}
									low_pointer = high_pointer->point_right;
									temp_pointer = NULL;
								}
							}
							//если выражение
							else if (current_element->type == exprs)
							{
								// выражение в undef функцию 
								current_element->type = funct;
								current_element->prop = undef;
								general_var_const->push_back(new math_obj(name, varbl, defnd, 0));
								//копия переменной с указателем на функцию
								current_element->point_collar = new math_obj(general_var_const->back());
								current_element->point_collar->point_collar = current_element;
								current_element->var = 1;//одна переменная
								//тут проблема может возникнуть - я не помню какая))
								if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
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
						//найден элемент массива с совпадающим именем - переменная
						else if (low_pointer->type == varbl)
						{
							if (*temp == '(),')
							{
								//ошибка - после переменной стоит открывающаяся скобка. Вообще не совсем бессмысленная запись, возможно нужен другой код ошибки или придумать как такое исключение обрабатывать.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							if (current_element->type == funct)
							{
								temp_pointer = current_element->find_varbl(low_pointer);
								if (current_element->point_collar->point_right != NULL)
								{
									if (temp_pointer == NULL)
									{
										//значит список переменных замкнут => новая переменная - лишняя
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									else
									{
										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
										{
											high_pointer->point_right->point_right = temp_pointer;//указатель на нужное место в списке переменных
										}
										else
										{
											high_pointer->point_right = temp_pointer;
										}
										low_pointer = high_pointer->point_right;
									}
								}
								//список не замкнут - можно добавить больше переменных
								else if (temp_pointer == NULL) //если нет совпадений имён переменных - найдена новая переменная
								{
									temp_pointer = current_element->var_list_back();
									temp_pointer->point_left = new math_obj(low_pointer);
									temp_pointer->point_left->point_right = temp_pointer;
									temp_pointer->point_left->point_collar = current_element;
									temp_pointer->point_left->var = current_element->var;
									current_element->var += 1;
									//поскольку символ не в начале строки - значит стоит после какой-либо операции. 
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//стоит после минуса
									{
										high_pointer->point_right->point_right = temp_pointer->point_left;
									}
									else
									{
										high_pointer->point_right = temp_pointer->point_left;
									}
									low_pointer = high_pointer->point_right;
								}
								else //при совпадении просто записываем переменную в выражение
								{
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = temp_pointer;//указатель на нужное место в списке переменных
									}
									else
									{
										high_pointer->point_right = temp_pointer;
									}
									low_pointer = high_pointer->point_right;
								}
							}
							else if (current_element->type == exprs)
							{
								current_element->type = funct;
								current_element->prop = undef;
								//копия переменной с указателем на функцию
								current_element->point_collar = new math_obj(low_pointer);
								current_element->point_collar->point_collar = current_element;
								current_element->var = 1;//одна переменная
								if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
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
						//найден элемент массива с совпадающим именем - константа - просто записываем её в выражение
						else if (low_pointer->type == cnst)
						{
							if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
							{
								high_pointer->point_right->point_right = new math_obj(low_pointer);
							}
							else
							{
								high_pointer->point_right = new math_obj(low_pointer);
							}
							low_pointer = high_pointer->point_right;
						}
						//найден элемент массива с совпадающим именем - функция 
						else if (low_pointer->type == funct)
						{
							if (*temp = '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
								while (count != 0)
								{
									temp++;
									//если попали в конец строчки не найдя закрывающих скобок
									if (temp == endPtr + 1)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
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
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										//считаем запятые. количество запятых в скобках +1 = число различных переменных
										comma++;
									}
								}
								//если количество аргументов не совпадает с количеством переменных
								if (comma + 1 != low_pointer->var)
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
									return NULL;
								}
								multiple_var = new math_obj[comma + 1];
								count_var = 0;
								for (count = 0; count < comma + 1; count++)
								{
									temp = wcspbrk(pDest + 1, L",)");
									name.assign(pDest + 1, temp);
									temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, 0, current_element), brakets + brakets_counter);
									if (temp_pointer == NULL)
										return temp_pointer;
									//заполняем массив аргументов
									if (temp_pointer->type == varbl)
									{
										count_var++;
										if (temp_pointer->prop == undef)
										{
											temp_pointer->prop = defnd;
											general_var_const->push_back(temp_pointer);
										}
										multiple_var[count].copy(temp_pointer);
									}
									else if ((temp_pointer->type == cnst) || (temp_pointer->type == exprs))
									{
										multiple_var[count].copy(temp_pointer);
									}
									else if (temp_pointer->type == funct)
									{
										//тут в качестве аргумента получается переменная функция, которая либо defnd, либо undef
										if (temp_pointer->prop != arg_c)
										{
											count_var++;
											if ((temp_pointer->prop == defnd) || (temp_pointer->prop == undef))
												multiple_var[count].copy(temp_pointer);
											else if (temp_pointer->prop == arg_v)
											{
												multiple_var[count].copy(temp_pointer);
												temp_pointer->point_right = NULL;//зануляем массив аргументов. Без очистки!
											}
										}
										else //тут вегда подразумевается defnd функция с конст аргументами										
											multiple_var[count].copy(temp_pointer);
									}
									pDest = temp;
								}
							}
							else if ((*temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{
								count_var = -1;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
							if (current_element->type == funct)
							{
								//если все аргументы полученные от функции либо константы, либо конст. выражения, либо функции с конст аргументами
								if (count_var == 0)
								{
									temp_pointer = new math_obj(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_c;//указываем что аргументы - константы
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = temp_pointer;
									}
									else
									{
										high_pointer->point_right = temp_pointer;
									}
									low_pointer = high_pointer->point_right;
								}
								//в качестве аргументов есть переменные
								else
								{
									temp_pointer = new math_obj(low_pointer);
									if (count_var != -1)
									{
										temp_pointer->point_right = multiple_var;
										multiple_var = NULL;
										temp_pointer->prop = arg_v;//указываем что среди аргументов есть переменные
									}
									//если список переменных не замкнут
									if (current_element->point_collar->point_right == NULL)
									{
										current_element->arg_copy_plus_1();
										current_element->point_right[(int)current_element->var - 1] = temp_pointer;
										temp_pointer = NULL;
										temp_pointer = current_element->var_list_back();
										temp_pointer->point_left = new math_obj(&temporary_variable);
										temp_pointer->point_left->point_right = temp_pointer;
										temp_pointer->point_left->var = current_element->var - 1;
									}
									//замкнут
									else
									{
										//если среди переменных аргументов найденной функции есть хоть один не входящий в замкнутый список
										if (current_element->var_list_compare(temp_pointer) != 0)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										//все переменные найденной функции входят в список переменных текущего элемента
										else
										{
											//копия найденной функции добавляется в выражение
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
											{
												high_pointer->point_right->point_right = temp_pointer;
											}
											else
											{
												high_pointer->point_right = temp_pointer;
											}
											low_pointer = high_pointer->point_right;
										}
									}
								}
							}
							else if (current_element->type == exprs)
							{
								//текущий элемент остаётся выражением.
								if (count_var == 0)
								{
									temp_pointer = new math_obj(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_c;//указываем что аргументы - константы

									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = temp_pointer;
									}
									else
									{
										high_pointer->point_right = temp_pointer;
									}
									low_pointer = high_pointer->point_right;
								}
								//текущий элемент становится функцией.
								else
								{
									temp_pointer = new math_obj(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_v;//указываем что среди аргументов есть переменные
									current_element->type = funct;
									current_element->prop = arg_v;
									current_element->point_right = temp_pointer; //полагаем новую неопределённую функцию, которая зависит от какой-либо переменной,
																								//имеющей аргумент в виде другой (определённой) функции. При этом если окажется, что
																								//в выражении участвуют несколько функций (f+g), то функция, котрая их объединяет
																								//будет считаться функцией нескольких переменных, 
																								//даже если все функции в её выражении зависят от одной и той же переменной.
																								//копия служебной переменной с указателем на функцию, которая иемеет номер ноль
									current_element->point_collar = new math_obj(&temporary_variable);
									current_element->point_collar->var = 0; //просто для достоверности)
									current_element->point_collar->point_collar = current_element;
									current_element->point_left = current_element->point_collar;//т.о. каждая функция будет иметь одну и туже структуру и пониматься всегда, 
																								//как функция зависящая от переменных, которой в качетве аргументов что-то передаётся
									current_element->var = 1; //количество переменных
									low_pointer = high_pointer->point_right;
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
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, math_obj* _current_element)
		{
			math_obj* CE = filling_vector(_pDest, _endPtr, _current_element, 0);
			wstring output;
			size_t output_size;
			if (CE == NULL)
			{
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				return err->GetErrorWStr();
			}
			else if (CE->type == exprs)
			{
				CE->arithmetic();
				output = to_string(CE->var, var_type::FRACTIONAL, 2);
				CE->tree_destruct();
				delete CE;
			}
			else if (CE->type == equat)
			{
				// ну делать тут как бы ничего не надо. Пользователь вбил уравнение и оно просто записалось. Когда научимся делать парралельные потоки вычислений можно на этом
				//моменте решать это уравнение с низким приоритетом, чтобы когда пользователь запросил ответ - вычисления занимали меньше времени - для него.
				//mutex* mut=new mutex();
				//thread tTest(testfunc);	//сразу запускается и пашет
				//thread* ptTest;	//для глобальной видимости потока, можно глобально хранить на него указатель
				//ptTest = new thread(testfunc);	//инициализировать, и он сразу запуститься
				//mut->unlock();
			}
			else if (CE->type == funct)
			{
				if (CE->actn == solve)//тут ещё условие
				{
					if (CE->point_right == NULL)
					{
						output = CE->expresion(1);
					}
					else
					{
						CE->arithmetic();
					}
				}
				else if (CE->prop == undef)
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
					ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
					ProjectError::GetProjectLastError(err);
					return err->GetErrorWStr();
				}
				else if (CE->prop == defnd)
				{
					//опять же, видимой реакции от программы быть не должно. Забили функцию - записали. Возможно в отдельный поток отдать разложение сложной функции 
					//на элементарные. Можно и не в отдельный, врядли там будет высокая сложность вычислений
				}
			}
			else if (CE->type == cnst)
			{
				if (CE->actn == solve)
				{
					//ответ 
					output = to_string(CE->var, var_type::FRACTIONAL, 2);

				}
				else if (CE->prop == undef)
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
					ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
					ProjectError::GetProjectLastError(err);
					return err->GetErrorWStr();
				}
				else if (CE->prop == defnd)
				{
					//заполнили - посчитали
					CE->arithmetic();
					CE->tree_destruct();
				}
			}
			else if (CE->type == varbl)
			{
				CE->type = defnd;
				//один из вариантов запроса на решение уравнения (скорее всего - последнего записанного)
				ProjectError::SetProjectLastError(ProjectError::ErrorCode::IS_EQUATION);
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				return err->GetErrorWStr();
			}
			else
			{
				//else else esle!!!
			}
			return output;
		}

		/*Функция является точкой входа в вычислительное ядро.
		Возвращает строку с результатом текущей итерации вычислений.*/
		wstring input_to_analize(wchar_t* input)
		{
			if (!Project::IO::VerifyInput(input)) {
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				return err->GetErrorWStr();
			}

			size_t size_of_vect = general_var_const->size();


			//general_var_const->push_back(new var_const(L"", exprs, undef, write, 0));

			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer	
			return analized_output(point_start, point_end, new math_obj(L"", exprs, undef, write, 0));
		}
	}
}
