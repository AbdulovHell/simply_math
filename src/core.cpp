#include "core.h"

namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;
		using namespace Project::IO;

		//структура хранения данных
		vector<math_obj*>* general_var_const;
		math_obj* temporary_variable;

		void ClearGeneral() {
			general_var_const->clear();
			delete general_var_const;
		}
		//Инициализация массива констант, переменных, функций и уравнений
		void Init() {
			ProjectError::Init();
			math_obj* temp;
			general_var_const = new vector<math_obj*>;
			general_var_const->push_back(new math_obj(L"pi", cnst, fundm, 3.1415926535897932384626433832));
			general_var_const->push_back(new math_obj(L"e", cnst, fundm, 2.7182818284590452353602874713527));
			general_var_const->push_back(new math_obj(L"i", cnst, fundm, 0));
			temporary_variable = new math_obj(L"temporary_variable", varbl, servc, L"", 0);
			//минус
			{
				temporary_variable->prop = L"minus";
				general_var_const->push_back(new math_obj(L"minus", funct, servc, L"", 1, new math_obj(temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
				temp->point_left = new math_obj(L"", mltpl, L"", L"", 0, new math_obj(L"0", numbr, real, -1), temp->point_collar, temp);
				temporary_variable->prop = servc;
			}

			//корень 
			{
				temp = new math_obj((int)0);
				temporary_variable->prop = L"root_";
				temp->vector_push_back(new math_obj(temporary_variable));
				temporary_variable->var += 1;
				temp->vector_push_back(new math_obj(temporary_variable));
				temporary_variable->var = 0;
				temporary_variable->prop = servc;
				temp->double_lincked_vector();
				temp->vector_at(0)->point_left = temp->vector_at(1);
				temp->vector_at(1)->point_right = temp->vector_at(0);
				general_var_const->push_back(new math_obj(L"root", funct, servc, L"", 2, temp->point_collar));
				delete temp;
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
				temp->point_collar->point_right->point_collar = temp;
				temp->point_left = new math_obj(L"^", power, 0, temp->point_collar, new math_obj(L"/", divis, 0, new math_obj(L"0", numbr, real, 1), temp->point_collar->point_right, temp->point_left), temp);
				temp->point_left->point_right->point_collar = temp->point_left;
			}
			//модуль (корень из квадрата числа)
			{
				general_var_const->push_back(new math_obj(L"abs", funct, servc, L"", 0, new math_obj(temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new math_obj(temp);
				general_var_const->back()->point_left->point_collar->point_collar = general_var_const->back()->point_left;
				general_var_const->back()->point_left->point_right = new math_obj(L"", power, L"", L"", 0, general_var_const->back()->point_collar, new math_obj(L"0", 2), NULL);
			}
			//знак
			{
				temp = general_var_const->back();
				general_var_const->push_back(new math_obj(L"sgn", funct, servc, L"", 0, new math_obj(temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new math_obj(L"", divis, L"", L"", 0, general_var_const->back()->point_collar, new math_obj(temp), general_var_const->back());
				general_var_const->back()->point_left->point_right->point_right = general_var_const->back()->point_collar;
				general_var_const->back()->point_left->point_right->point_collar = general_var_const->back()->point_collar;
			}

		}

		//функция проходит по вектору general_var_const и ищет объект с нужным именем
		math_obj* run_vector(wstring _pv)
		{
			size_t temp_size_of_vect = general_var_const->size();
			size_t count;
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

		/*Функция создаёт математический объект - вектор длины size- по строке символов типа wchar_t, имеющей начало begin и конец end.
		Функция возвращает указатель на созданный экземпляр класса math_obj. Функция вызывает в качестве внутренней функции, а так же рекурсивно, основную функцию заполнения build_tree.
		NULL в случае какой-либо ошибки, см. error.h.*/
		math_obj *make_vector(int size, wchar_t*begin, wchar_t*end)
		{						
			int iter = 0;
			int count_var = 0;
			wstring name;
			wchar_t *temp;
			wchar_t *temp_2;
			math_obj *out = new math_obj((int)0);
			math_obj *temp_pointer;


			for (int count = 0; count < size; count++)
			{
				temp = wcspbrk(begin + 1, L",)");
				if (*temp == ',')
				{
					temp_2 = wcspbrk(begin + 1, L"(");
					if ((temp_2 != NULL) && (temp_2 < temp))
					{
						iter = 1;
						while (iter != 0)
						{
							temp_2++;
							if (*temp_2 == '(')
							{
								iter++;
							}
							else if (*temp_2 == ')')
							{
								iter--;
							}
						}
						temp = wcspbrk(temp_2 + 1, L",)");
					}
				}
				name.assign(begin + 1, temp);
				temp_pointer = build_tree(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, 0), 0);
				if (temp_pointer == NULL)
					return temp_pointer;
				//заполняем массив аргументов
				if (temp_pointer->type == vectr)
				{
					//пока ошибка. Надо расмотреть векторы с векторами внутри. Скорее всего в этом случае необходимо требовать чтобы все элементы вектора были векторами равной длины, возможно переопределение вектора в матрицу.
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::EQUALY_MISSING);
					return NULL;
				}
				else if (temp_pointer->type == varbl)
				{
					count_var++;
					if (temp_pointer->prop == undef)
					{
						temp_pointer->prop = defnd;
						general_var_const->push_back(temp_pointer);
					}
					//если в вектор добавляются переменные - они нумеруются по порядку. Это необходимо в случае задания списка переменных, а так же для разрешения возможных неопределённостей при проходе по вектору определённых методов.
					temp_pointer->var = count_var;					
				}
				else if (temp_pointer->type == funct)
				{
					//тут в качестве аргумента получается переменная функция, которая либо  defnd, либо  undef
					if (temp_pointer->prop != arg_c)
					{
						out->prop = arg_v;
					}
					else //тут вегда подразумевается  defnd функция с конст аргументами										
					{

					}
				}
				out->vector_push_back(temp_pointer);
				begin = temp;
			}
			if (count_var == size)
				out->prop = only_arg_v;
			else if (out->prop != arg_v)
			{
				if (count_var == 0)
					out->prop = arg_c;
				else
					out->prop = arg_v;
			}
			out->var = size;
			return out;
		}

		/*Функция строит математические объекты из других мат объектов в виде структуры типа дерева по строке символов типа wchar_t, имеющей начало strPtr и конец ePtr.
		Функция возвращает указатель на созданный экземпляр класса math_obj, представляющий собой верхушку (корень) дерева, либо на копию уже созданного экземпляра.
		NULL в случае какой-либо ошибки, см. error.h. Типы мат объектов описаны в classes.h*/
		math_obj* build_tree(wchar_t* strPtr, wchar_t*ePtr, math_obj* c_e, int brakets)
		{
			wchar_t* pDest = strPtr;
			wchar_t* endPtr = ePtr;
			unsigned int count, comma, iter;
			int64_t count_var;
			math_obj *high_pointer = NULL; //последняя записанная операция
			math_obj *low_pointer = NULL; //последняя записанная константа/переменная/ф-ция/выражение
			math_obj *temp_pointer = NULL;
			math_obj *multiple_var = NULL;
			wchar_t* temp, *temp_2;
			wstring name;
			math_obj* current_element = c_e;
			//double num;
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
						//просто выдаём наверх текущий заполненный элемент с параметром  solve, а там уже пытаемся его расчитать и вывести результат или сообщить ошибку
						if (current_element->actn != write)
							current_element->actn = solve;
						else
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
							return NULL;
						}
						pDest = endPtr + 1;
					}
					else
					{
						if (current_element->type == cnst)
						{
							temp_pointer = build_tree(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer == NULL)
								return temp_pointer;
							if (temp_pointer->type == funct)
							{
								if (temp_pointer->prop == arg_c)
								{
									current_element->point_left = temp_pointer;
									current_element->prop = defnd;
									current_element->actn = write;
								}
								else
								{
									current_element->funct_define(temp_pointer);
									current_element->type = funct;
									current_element->prop = defnd;
									current_element->actn = write;
									delete temp_pointer;
								}
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
							pDest = endPtr + 1;
						}
						else if (current_element->type == varbl)
						{
							temp_pointer = build_tree(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer == NULL)
								return temp_pointer;
							//тут происходит переопределение переменых в качестве функций. Могут возникнуть проблемы, т.к. появятся два разных по типу объекта с одним именем.
							if (temp_pointer->type == funct)
							{
								low_pointer = temp_pointer->find_by_name(current_element);
								if (low_pointer == NULL)
								{
									//если переменная слева от равно не появляется справа - переопределение переменной в функцию. н-р : y=2*x+1
									current_element->funct_define(temp_pointer);
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
							pDest = endPtr + 1;
						}
						else if (current_element->type == funct)
						{
							if (current_element->prop == defnd)
							{
								temp_pointer = build_tree(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
								if (temp_pointer == NULL)
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
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(temp_pointer);
									}
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
										current_element->funct_define(temp_pointer);
										current_element->actn = write;
									}
								}
								pDest = endPtr + 1;
							}
							else if (current_element->prop == undef)
							{
								if (current_element->actn == write)
								{
									//сразу создаём функцию с указателем на список переменных данной функции
									temp_pointer = build_tree(pDest + 1, endPtr, new math_obj(L"", funct, undef, current_element->var, current_element->point_collar), brakets + brakets_counter);
									if (temp_pointer == NULL)
										return temp_pointer;
									//если вылезло что угодно кроме функции - ошибка
									if (temp_pointer->type != funct)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
										return NULL;
									}
									else
									{
										//забираем указатель сразу на дерево операций
										current_element->point_left = temp_pointer->point_left;
										current_element->prop = defnd;
									}
								}
								else
								{
									/*При записи уравнений надо составлять общий список переменных в левой и правой частях. Поскольку это менее критично, чем при записи функций,
									возможно данное действие оставить на последующий анализ уравнения
									*/
									temp_pointer = build_tree(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
									if (temp_pointer == NULL)
										return temp_pointer;
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, high_pointer->var, high_pointer, temp_pointer, high_pointer->point_collar);
								}
								pDest = endPtr + 1;
							}
							else if (current_element->prop == arg_c)
							{
								temp_pointer = build_tree(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
								if (temp_pointer == NULL)
									return temp_pointer;
								if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
									return NULL;
								}
								else if (temp_pointer->type == varbl)
								{
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(temp_pointer);
									}
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, 1, high_pointer, temp_pointer, temp_pointer);
								}
								else if (temp_pointer->type == funct)
								{
									if (temp_pointer->prop == arg_c)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
										return NULL;
									}
									else
									{
										high_pointer = current_element;
										current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, high_pointer, temp_pointer, temp_pointer->point_collar);
									}
								}
								pDest = endPtr + 1;
							}
							else if (current_element->prop == arg_v)
							{
								count_var = 0;
								if ((int)current_element->var != current_element->point_right->vector_size())
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
									return NULL;
								}
								for (count = 0; count < current_element->var; count++)
								{
									if (current_element->point_right->vector_at(count)->type == varbl)
										count_var++;
								}
								if ((current_element->name.size() != 0) && (count_var == current_element->var))
								{
									current_element->point_right->double_lincked_vector();
									current_element->point_collar = current_element->point_right->point_collar;
									delete current_element->point_right;
									current_element->point_right = NULL;
									current_element->point_collar->point_left = current_element->var_list_back();
									current_element->point_collar->point_left->point_right = current_element->point_collar;
									current_element->prop = undef;
									current_element->actn = write;
								}
								else
								{
									temp_pointer = build_tree(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
									if (temp_pointer == NULL)
										return temp_pointer;
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, high_pointer, temp_pointer, temp_pointer->point_collar);
									pDest = endPtr + 1;
								}
							}
						}
						else if (current_element->type == exprs)
						{
							temp_pointer = build_tree(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer == NULL)
								return temp_pointer;
							//слева - конст выражение - справа функция => уравнение
							if (temp_pointer->type == funct)
							{
								if (temp_pointer->prop == arg_c)
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
									return NULL;
								}
								else
								{
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, high_pointer, temp_pointer, temp_pointer->point_collar);
								}
							}
							else if (temp_pointer->type == varbl)
							{
								if (temp_pointer->prop == undef)
								{
									temp_pointer->prop = defnd;
									general_var_const->push_back(temp_pointer);
								}
								high_pointer = current_element;
								current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, high_pointer, temp_pointer, temp_pointer);
							}
							else if ((temp_pointer->type == cnst) || (temp_pointer->type == exprs))
							{
								//запись вида 2+3=7. Пока не знаю как реагировать на такое
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
								return NULL;
							}
							pDest = endPtr + 1;
						}
					}
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
						current_element->point_left = new math_obj(general_var_const->at(3));
						current_element->point_left->prop = arg_c;

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

							high_pointer->point_right->prop = arg_c;

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
								high_pointer->point_right = new math_obj(general_var_const->at(3));
								high_pointer->point_right->prop = arg_c;
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
									high_pointer->point_right = new math_obj(general_var_const->at(3));
									high_pointer->point_right->prop = arg_c;
									low_pointer = high_pointer->point_right;
								}
								else
								{
									high_pointer = high_pointer->prioritize(brakets_counter + brakets + 1);
									high_pointer->point_right->point_collar = new math_obj(L"+", addit, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
									high_pointer->point_right = high_pointer->point_right->point_collar;
									high_pointer = high_pointer->point_right;
									high_pointer->point_right = new math_obj(general_var_const->at(3));
									high_pointer->point_right->prop = arg_c;
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
							if (count == 1)
								comma++;
						}
					}

					if (pDest + 1 == temp)
					{
						//пустая строка в скобках. вроде не ошибка, но можно и запретить). Вообще в контекте построения дерева это означает запись нуля. Поэтому пока запрещено.
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::EMTY_BRACKETS);
						return NULL;
					}
					else if (comma != 0)//в скобках записан вектор
					{
						name.assign(pDest + 1, temp);
						temp_pointer = make_vector(comma + 1, &name[0], &name[name.length() - 1]);
						if (temp_pointer == NULL)
						{
							return temp_pointer;
						}
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							if ((*(temp + 1) == NULL)|| (*(temp + 1) == '='))
							{
								delete current_element;
								current_element = temp_pointer;
							}
							else if ((*(temp + 1) == '+') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '-') || (*(temp + 1) == '^'))
							{
								if (current_element->type == exprs)
								{	
									if (temp_pointer->prop == arg_c)
									{
										current_element->prop == vectr;
										current_element->point_left = temp_pointer;
										high_pointer = temp_pointer;
										low_pointer = high_pointer;
									}
									else if (temp_pointer->prop == arg_v)
									{

									}
									else if (temp_pointer->prop == only_arg_v)
									{

									}
								}
							}
							else
							{

							}
						}
						else if (low_pointer == high_pointer)
						{

						}
						else
						{

						}
					}
					else
					{
						name.assign(pDest + 1, temp);
						temp_pointer = build_tree(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
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
									if (current_element->point_collar->point_left != NULL)
									{
										multiple_var = current_element->find_by_name(temp_pointer);
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
									current_element->funct_arg_push_back(temp_pointer);
									current_element->point_collar = new math_obj(temporary_variable);
									current_element->type = funct;
									current_element->prop = undef;
									current_element->var = 1;
									current_element->point_collar->var = 0; //просто для достоверности)
									current_element->point_collar->point_collar = current_element;
									current_element->point_left = current_element->point_collar;
									high_pointer = current_element->point_left;
									low_pointer = high_pointer;
								}
								else if (current_element->type == funct)
								{
									if (current_element->point_collar->point_left != NULL)
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
									high_pointer->point_right = new math_obj((int)0);
									high_pointer->point_right->vector_push_back(temp_pointer);
									high_pointer->prop = arg_c;
									high_pointer->point_right->prop = arg_c;
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
										temp_pointer = new math_obj(current_element->point_collar);
										temp_pointer->var = -1;
										temp_pointer->point_collar = current_element->point_collar;
										high_pointer->point_right = new math_obj((int)0);
										high_pointer->point_right->vector_push_back(temp_pointer);
										high_pointer->prop = arg_v;
										high_pointer->point_right->prop = arg_v;
									}
									else if (current_element->type == funct)
									{
										if (current_element->point_collar->point_left != NULL)
										{
											multiple_var = current_element->find_by_name(temp_pointer);
											if (multiple_var == NULL)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											else
											{
												temp_pointer = new math_obj(multiple_var);
												temp_pointer->var = -1;
												temp_pointer->point_collar = multiple_var;
												high_pointer->point_right = new math_obj((int)0);
												high_pointer->point_right->vector_push_back(temp_pointer);
												high_pointer->prop = arg_v;
												high_pointer->point_right->prop = arg_v;
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
										high_pointer->point_right = new math_obj((int)0);
										high_pointer->point_right->vector_push_back(temp_pointer);
										high_pointer->prop = arg_c;
										high_pointer->point_right->prop = arg_c;
									}
									else if (current_element->type == exprs)
									{
										current_element->funct_arg_push_back(temp_pointer);
										current_element->point_collar = new math_obj(temporary_variable);
										current_element->type = funct;
										current_element->prop = undef;
										current_element->var = 1;
										current_element->point_collar->var = 0; //просто для достоверности)
										current_element->point_collar->point_collar = current_element;

										temp_pointer = new math_obj(current_element->point_collar);
										temp_pointer->var = -1;
										temp_pointer->point_collar = current_element->point_collar;
										temp_pointer->prop = L"";
										high_pointer->point_right = new math_obj((int)0);
										high_pointer->point_right->vector_push_back(temp_pointer);
										high_pointer->prop = arg_v;
										high_pointer->point_right->prop = arg_v;
									}
									else if (current_element->type == funct)
									{
										if (current_element->point_collar->point_left != NULL)
										{
											if (current_element->var_list_compare(temp_pointer) != 0)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											else
											{
												high_pointer->point_right = new math_obj((int)0);
												high_pointer->point_right->vector_push_back(temp_pointer);
												high_pointer->prop = arg_v;
												high_pointer->point_right->prop = arg_v;
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
									high_pointer->point_right->point_right = new math_obj((int)0);
									high_pointer->point_right->point_right->vector_push_back(temp_pointer);
									high_pointer->point_right->prop = arg_c;
									high_pointer->point_right->point_right->prop = arg_c;
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
									multiple_var = current_element->find_by_name(temp_pointer);
									if (current_element->point_collar->point_left != NULL)
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
												temp_pointer = new math_obj(multiple_var);
												temp_pointer->var = -1;
												temp_pointer->point_collar = multiple_var;
												high_pointer->point_right->point_right = new math_obj((int)0);
												high_pointer->point_right->point_right->vector_push_back(temp_pointer);
												high_pointer->point_right->prop = arg_v;
												high_pointer->point_right->point_right->prop = arg_v;
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
											multiple_var->point_right = temp_pointer;
											multiple_var->point_right->point_left = multiple_var;
											multiple_var->point_right->point_collar = current_element;
											multiple_var->point_right->var = current_element->var;
											current_element->var += 1;
											//поскольку символ не в начале строки - значит стоит после какой-либо операции. 
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//стоит после минуса
											{
												multiple_var = new math_obj(temp_pointer);
												multiple_var->point_collar = temp_pointer;
												high_pointer->point_right->point_right = new math_obj((int)0);
												high_pointer->point_right->point_right->vector_push_back(multiple_var);
												high_pointer->point_right->prop = arg_v;
												high_pointer->point_right->point_right->prop = arg_v;
											}
											else
											{
												high_pointer->point_right = multiple_var->point_right;
											}
											low_pointer = high_pointer->point_right;
										}
										else
										{
											//стоит ли перед переменной минус
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
											{
												temp_pointer = new math_obj(multiple_var);
												temp_pointer->var = -1;
												temp_pointer->point_collar = multiple_var;
												high_pointer->point_right->point_right = new math_obj((int)0);
												high_pointer->point_right->point_right->vector_push_back(temp_pointer);
												high_pointer->point_right->prop = arg_v;
												high_pointer->point_right->point_right->prop = arg_v;
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
										temp_pointer = new math_obj(current_element->point_collar);
										temp_pointer->var = -1;
										temp_pointer->point_collar = current_element->point_collar;
										high_pointer->point_right->point_right = new math_obj((int)0);
										high_pointer->point_right->point_right->vector_push_back(temp_pointer);
										high_pointer->point_right->prop = arg_v;
										high_pointer->point_right->point_right->prop = arg_v;
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
										high_pointer->point_right->point_right = new math_obj((int)0);
										high_pointer->point_right->point_right->vector_push_back(temp_pointer);
										high_pointer->point_right->prop = arg_c;
										high_pointer->point_right->point_right->prop = arg_c;
									}
									else
									{
										high_pointer->point_right = temp_pointer;
									}
									low_pointer = high_pointer->point_right;
								}
								else if (current_element->type == exprs)
								{
									current_element->point_collar = new math_obj(temporary_variable);
									current_element->funct_arg_push_back(temp_pointer);
									current_element->type = funct;
									current_element->prop = undef;
									current_element->var = 1;
									current_element->point_collar->var = 0; //просто для достоверности)
									current_element->point_collar->point_collar = current_element;

									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										temp_pointer = new math_obj(current_element->point_collar);
										temp_pointer->var = -1;
										temp_pointer->prop = L"";
										temp_pointer->point_collar = current_element->point_collar;
										high_pointer->point_right->point_right = new math_obj((int)0);
										high_pointer->point_right->point_right->vector_push_back(temp_pointer);
										high_pointer->point_right->prop = arg_v;
										high_pointer->point_right->point_right->prop = arg_v;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}
								else if (current_element->type == funct)
								{
									if (current_element->point_collar->point_left != NULL)
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
												high_pointer->point_right->point_right = new math_obj((int)0);
												high_pointer->point_right->point_right->vector_push_back(temp_pointer);
												high_pointer->point_right->prop = arg_v;
												high_pointer->point_right->point_right->prop = arg_v;
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
										current_element->funct_arg_push_back(temp_pointer);
										temp_pointer = NULL;
										temp_pointer = current_element->var_list_back();
										temp_pointer->point_right = new math_obj(temporary_variable);
										temp_pointer->point_right->point_left = temp_pointer;
										temp_pointer->point_right->var = current_element->var;
										current_element->var += 1;
										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
										{
											multiple_var = new math_obj(temp_pointer->point_right);
											multiple_var->prop = L"";
											multiple_var->point_collar = temp_pointer->point_right;
											high_pointer->point_right->point_right = new math_obj((int)0);
											high_pointer->point_right->point_right->vector_push_back(multiple_var);
											high_pointer->point_right->prop = arg_v;
											high_pointer->point_right->point_right->prop = arg_v;
										}
										else
										{
											high_pointer->point_right = temp_pointer->point_right;
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
					if ((*pDest == '(') || (*pDest == ','))       //*pDest == '(,'
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
							high_pointer->point_right = new math_obj((int)0);
							high_pointer->point_right->vector_push_back(new math_obj(L"", numbr, real, wcstod(pDest, &pDest), low_pointer));
							high_pointer->prop = arg_c;
							high_pointer->point_right->prop = arg_c;
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
							low_pointer->point_right = new math_obj((int)0);
							low_pointer->point_right->vector_push_back(new math_obj(L"", numbr, real, wcstod(pDest, &pDest), low_pointer));
							low_pointer->prop = arg_c;
							low_pointer->point_right->prop = arg_c;
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
										if (count == 1)
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
									current_element->prop = undef; //считаем новую функцию  undef, но при этом мы знаем что у неё есть имя
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
									current_element->prop = undef; //считаем новую функцию  undef, но при этом мы знаем что у неё есть имя
									current_element->actn = write;  //на заполнение
									temp_pointer = make_vector(comma + 1, pDest, temp);
									if (temp_pointer == NULL)
										return temp_pointer;
									if (temp_pointer->prop != only_arg_v)
									{
										temp_pointer->tree_destruct();
										delete temp_pointer;
										delete current_element;
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									temp_pointer->double_lincked_vector();
									current_element->point_collar = temp_pointer->point_left;
									delete temp_pointer; temp_pointer = NULL;
									current_element->link_varbls_to_funct();
									current_element->close_list();

									high_pointer = current_element;
									low_pointer = high_pointer;
								}
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
										if (count == 1)
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

									temp_pointer = make_vector(comma + 1, pDest, temp);
									if (temp_pointer == NULL)
										return temp_pointer;
									if (temp_pointer->prop != only_arg_v)
									{
										// тут либо вывод значения, либо новое определение, игнорируя аргументы
										//тут ещё может означать умножение с(...) = с*(...). К примеру запрос на расчёт с*(...)= 
										//однако в данном случае это умножение на вектор - что не реализовано, возможно стоит как-то отдельно делать	
										temp_pointer->tree_destruct();
										delete temp_pointer;
										high_pointer = current_element;
										low_pointer = high_pointer;
									}
									else if (high_pointer->prop != fundm)
									{
										temp_pointer->double_lincked_vector();
										current_element->point_collar = temp_pointer->point_left;
										delete temp_pointer; temp_pointer = NULL;
										current_element->link_varbls_to_funct();
										current_element->close_list();

										current_element->var = comma + 1;
										current_element->type = funct;
										current_element->prop = undef; //считаем новую функцию  undef, но при этом мы знаем что у неё есть имя
										current_element->actn = write; //на запись										
										low_pointer = high_pointer;
									}
									else
									{
										//предупреждение при переопределении фундаментальной константы.
									}
								}
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
										if (count == 1)
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
									current_element->prop = undef; //считаем новую функцию  undef, но при этом мы знаем что у неё есть имя		
									current_element->actn = write; //на запись
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								else
								{
									current_element->copy(high_pointer);
									temp_pointer = make_vector(comma + 1, pDest, temp);
									if (temp_pointer == NULL)
										return temp_pointer;
									if (temp_pointer->prop != only_arg_v)
									{
										temp_pointer->tree_destruct();
										delete temp_pointer;
										delete current_element;
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									temp_pointer->double_lincked_vector();
									current_element->point_collar = temp_pointer->point_left;
									delete temp_pointer; temp_pointer = NULL;
									current_element->link_varbls_to_funct();
									current_element->close_list();

									//запоминаем число переменных
									current_element->var = comma + 1;
									current_element->type = funct;
									current_element->prop = undef; //считаем новую функцию  undef, но при этом мы знаем что у неё есть имя
									current_element->actn = write; //на запись	
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								pDest = temp + 1;
							}
							//текущий элемент => функция, аргументом является копия переменной
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								if (current_element->type == exprs)
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
								else if (current_element->type == funct)
								{
									//случай, когда текущее выражение - функция, которой заранее дан список переменных
									temp_pointer = current_element->find_by_name(high_pointer);
									if (temp_pointer == NULL)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									current_element->point_left = temp_pointer;
									high_pointer = current_element->point_left;
									low_pointer = high_pointer;
								}
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
										if (count == 1)
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

										current_element->var = 1; // одна переменная
										current_element->funct_arg_push_back(new math_obj(high_pointer)); //найденная функция - аргумент текущей
										//копия служебной переменной с указателем на функцию
										current_element->point_collar = new math_obj(temporary_variable);
										current_element->type = funct;
										current_element->prop = arg_v;
										current_element->point_collar->var = 0; //просто для достоверности)
										current_element->point_collar->point_collar = current_element;
										current_element->point_left = current_element->point_collar;

										high_pointer = current_element->point_left;
										low_pointer = high_pointer;
									}
								}
								
								else
								{

									multiple_var = make_vector(comma + 1, pDest, temp);
									if (multiple_var == NULL)
										return multiple_var;
									//число аргументов в скобках совпадает с числом аргументов у определённой заранее функции
									if (high_pointer->var == comma + 1)
									{


										if ((multiple_var->prop == arg_v) || (multiple_var->prop == only_arg_v))//если среди аргументов хотя бы одна переменная
										{
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												multiple_var->prop = arg_v;
												current_element->point_right = multiple_var;
												current_element->prop = arg_v; //указываем что в функцию передали аргумент
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->var = 1; // одна переменная
												temp_pointer = new math_obj(high_pointer);
												multiple_var->prop = arg_v;
												temp_pointer->point_right = multiple_var;
												temp_pointer->prop = arg_v;
												current_element->funct_arg_push_back(temp_pointer); //найденная функция - аргумент текущей
																											//копия служебной переменной с указателем на функцию
												current_element->point_collar = new math_obj(temporary_variable);
												current_element->type = funct;
												current_element->prop = arg_v;
												current_element->point_collar->var = 0; //просто для достоверности)
												current_element->point_collar->point_collar = current_element;
												current_element->point_left = current_element->point_collar;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
										else // все аргументы не переменные
										{
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												multiple_var->prop = arg_c;
												current_element->point_right = multiple_var;
												current_element->prop = arg_c; //указываем что в функцию передали аргумент
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->point_left = new math_obj(high_pointer);
												multiple_var->prop = arg_c;
												current_element->point_left->point_right = multiple_var;
												current_element->point_left->prop = arg_c;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}

									}
									//число переменных - различное 
									else
									{
										if ((*(temp + 1) == NULL)||(*(temp + 1) == '+')|| (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '-') || (*(temp + 1) == '^'))
										{
											//тут ошибка: ранее была f(x,y), записали f(k)+... - количество переменных в вызываемой функции не совпадает
											//TODO:здесь надо будет написать исключения для функций нескольких аргументов, которые могут пониматься как функция одного аргумента
											//пример root(5) - квадратный корень из 5, root (5,2) - тоже самое, root (5,3) - кубический корень из 5

											if (high_pointer->var != 1)
											{
												multiple_var->tree_destruct();
												delete temp_pointer;
												delete current_element;
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											else
											{
												//вариант использования векторной функции (функции от вектора)
												if ((multiple_var->prop == arg_v) || (multiple_var->prop == only_arg_v))
												{
													current_element->var = 1; // одна переменная
													temp_pointer = new math_obj(high_pointer);
													multiple_var->prop = arg_v;
													temp_pointer->point_right = multiple_var;
													temp_pointer->prop = vectr_arg_v;
													current_element->funct_arg_push_back(temp_pointer); //найденная функция - аргумент текущей
																										//копия служебной переменной с указателем на функцию
													current_element->point_collar = new math_obj(temporary_variable);
													current_element->type = funct;
													current_element->prop = vectr_arg_v;
													current_element->point_collar->var = 0; //просто для достоверности)
													current_element->point_collar->point_collar = current_element;
													current_element->point_left = current_element->point_collar;
													high_pointer = current_element->point_left;
													low_pointer = high_pointer;
												}
												else
												{
													current_element->point_left = new math_obj(high_pointer);
													multiple_var->prop = arg_c;
													current_element->point_left->point_right = multiple_var;
													current_element->point_left->prop = vectr_arg_v;
													high_pointer = current_element->point_left;
													low_pointer = high_pointer;
												}
											}

										}
										//но если далее стоит равно - всё нормально,  переопределение. 									

										else if(*(temp + 1) == '=')
										{
											current_element->copy(high_pointer);
											if (multiple_var->prop != only_arg_v)
											{
												multiple_var->tree_destruct();
												delete temp_pointer;
												delete current_element;
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
												return NULL;
											}
											temp_pointer->double_lincked_vector();
											current_element->point_collar = temp_pointer->point_left;
											delete temp_pointer; temp_pointer = NULL;
											current_element->link_varbls_to_funct();
											current_element->close_list();
											//запоминаем число переменных
											current_element->var = comma + 1;
											current_element->prop = undef; //считаем новую функцию  undef, но при этом мы знаем что у неё есть имя
											current_element->actn = write;  //на заполнение		
											high_pointer = current_element;
											low_pointer = high_pointer;
										}
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
											return NULL;
										}

									}
								}
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
								current_element->var = 1; // одна переменная
								temp_pointer = new math_obj(high_pointer);
								temp_pointer->prop = arg_v;
								current_element->funct_arg_push_back(temp_pointer); //полагаем новую неопределённую функцию, которая зависит от какой-либо переменной,
																							//имеющей аргумент в виде другой (определённой) функции. При этом если окажется, что
																							//в выражении участвуют несколько функций (f+g), то функция, котрая их объединяет
																							//будет считаться функцией нескольких переменных, 
																							//даже если все функции в её выражении зависят от одной и той же переменной.
								//копия служебной переменной с указателем на функцию, которая иемеет номер ноль
								current_element->point_collar = new math_obj(temporary_variable);
								current_element->type = funct;
								current_element->prop = arg_v;
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

						//далее всегда может быть только два варианта - текщуий элемент либо функция, либо выражение, причём всегда  undef.
						//если не найден ни один элемент массива с таким именем
						if (low_pointer == NULL)
						{
							if ((temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '='))
							{
								if (current_element->type == funct)
								{
									//если текущий элемент - функция, то очевидно найдена новая переменная в записи выражения. 
									if (current_element->point_collar->point_left != NULL)
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
										temp_pointer->point_right = new math_obj(general_var_const->back());
										temp_pointer->point_right->point_left = temp_pointer;
										temp_pointer->point_right->point_collar = current_element;
										temp_pointer->point_right->var = current_element->var;
										current_element->var += 1;
										//поскольку символ не в начале строки - значит стоит после какой-либо операции. 
										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//стоит после минуса
										{
											high_pointer->point_right->point_right = new math_obj((int)0);
											high_pointer->point_right->point_right->vector_push_back(new math_obj(temp_pointer->point_right));
											high_pointer->point_right->point_right->vector_back()->point_collar = temp_pointer->point_right;
											high_pointer->point_right->prop = arg_v;
											high_pointer->point_right->point_right->prop = arg_v;
										}
										else
										{
											high_pointer->point_right = temp_pointer->point_right;
										}
										low_pointer = high_pointer->point_right;
										temp_pointer = NULL;
									}
								}
								//если выражение
								else if (current_element->type == exprs)
								{
									// выражение в  undef функцию 
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
										high_pointer->point_right->point_right = new math_obj((int)0);
										high_pointer->point_right->point_right->vector_push_back(new math_obj(current_element->point_collar));
										high_pointer->point_right->point_right->vector_back()->point_collar = current_element->point_collar;
										high_pointer->point_right->prop = arg_v;
										high_pointer->point_right->point_right->prop = arg_v;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}
							}
							else if ((*temp == '(') || (*temp == ')') || (*temp == ','))
							{
								//ошибка - после переменной стоит открывающаяся скобка. Вообще не совсем бессмысленная запись, возможно нужен другой код ошибки или придумать как такое исключение обрабатывать.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
						}
						//найден элемент массива с совпадающим именем - переменная
						else if (low_pointer->type == varbl)
						{
							if ((temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '='))
							{
								if (current_element->type == funct)
								{
									temp_pointer = current_element->find_by_name(low_pointer);
									if (current_element->point_collar->point_left != NULL)
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
												high_pointer->point_right->point_right = new math_obj((int)0);
												multiple_var = new math_obj(temp_pointer);
												multiple_var->var = -1; // на всякий случай
												high_pointer->point_right->point_right->vector_push_back(multiple_var);
												multiple_var = NULL;
												high_pointer->point_right->point_right->vector_back()->point_collar = temp_pointer;
												high_pointer->point_right->prop = arg_v;
												high_pointer->point_right->point_right->prop = arg_v;
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
										temp_pointer->point_right = new math_obj(low_pointer);
										temp_pointer->point_right->point_left = temp_pointer;
										temp_pointer->point_right->point_collar = current_element;
										temp_pointer->point_right->var = current_element->var;
										current_element->var += 1;
										//поскольку символ не в начале строки - значит стоит после какой-либо операции. 
										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//стоит после минуса
										{
											high_pointer->point_right->point_right = new math_obj((int)0);
											high_pointer->point_right->point_right->vector_push_back(new math_obj(temp_pointer->point_right));
											high_pointer->point_right->point_right->vector_back()->point_collar = temp_pointer->point_right;
											high_pointer->point_right->prop = arg_v;
											high_pointer->point_right->point_right->prop = arg_v;
										}
										else
										{
											high_pointer->point_right = temp_pointer->point_right;
										}
										low_pointer = high_pointer->point_right;
									}
									else //при совпадении просто записываем переменную в выражение
									{
										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
										{
											high_pointer->point_right->point_right = new math_obj((int)0);
											multiple_var = new math_obj(temp_pointer);
											multiple_var->var = -1; // на всякий случай
											high_pointer->point_right->point_right->vector_push_back(multiple_var);
											multiple_var = NULL;
											high_pointer->point_right->point_right->vector_back()->point_collar = temp_pointer;
											high_pointer->point_right->prop = arg_v;
											high_pointer->point_right->point_right->prop = arg_v;
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
										high_pointer->point_right->point_right = new math_obj((int)0);
										high_pointer->point_right->point_right->vector_push_back(new math_obj(current_element->point_collar));
										high_pointer->point_right->point_right->vector_back()->point_collar = current_element->point_collar;
										high_pointer->point_right->prop = arg_v;
										high_pointer->point_right->point_right->prop = arg_v;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}
							}
							else if ((*temp == '(') || (*temp == ')') || (*temp == ','))
							{
								//ошибка - после переменной стоит открывающаяся скобка. Вообще не совсем бессмысленная запись, возможно нужен другой код ошибки или придумать как такое исключение обрабатывать.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}

						}
						//найден элемент массива с совпадающим именем - константа - просто записываем её в выражение
						else if (low_pointer->type == cnst)
						{
							if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
							{
								high_pointer->point_right->point_right = new math_obj((int)0);
								high_pointer->point_right->point_right->vector_push_back(new math_obj(low_pointer));
								high_pointer->point_right->prop = arg_c;
								high_pointer->point_right->point_right->prop = arg_c;
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
							if ((*temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^')|| (*temp == '='))
							{
								multiple_var = NULL;
							}
							else if (*temp == '(')
							{								
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
										if (count == 1)
											comma++;
									}
								}
								if (pDest+1 == temp)
									if ((*(temp+1) == NULL) || (*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^') || (*(temp + 1) == '='))
										multiple_var = NULL;
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
										return NULL;
									}
								else
								{
									//если количество аргументов не совпадает с количеством переменных
									if (comma + 1 != low_pointer->var)
									{
										if (low_pointer->var == 1)
										{
											multiple_var = make_vector(comma + 1, pDest, temp);
											if (multiple_var == NULL)
												return multiple_var;
										}
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
									}
									else
									{
										multiple_var = make_vector(comma + 1, pDest, temp);
										if (multiple_var == NULL)
											return multiple_var;
									}
								}
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
							if (current_element->type == funct)
							{
								//если все аргументы полученные от функции либо константы, либо конст. выражения, либо функции с конст аргументами
								if ((multiple_var != NULL)&&(multiple_var->prop == arg_c))
								{
									temp_pointer = new math_obj(low_pointer);									
									temp_pointer->point_right = multiple_var;
									if ((temp_pointer->var == 1) && (multiple_var->vector_size() != 1))
										temp_pointer->prop = vectr_arg_c;
									else
										temp_pointer->prop = arg_c;//указываем что аргументы - константы
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = new math_obj((int)0);
										high_pointer->point_right->point_right->vector_push_back(temp_pointer);
										if (temp_pointer->prop == vectr_arg_c)
											high_pointer->point_right->prop = vectr_arg_c;
										else
											high_pointer->point_right->prop = arg_c;										
										high_pointer->point_right->point_right->prop = arg_c;
									}
									else
									{
										high_pointer->point_right = temp_pointer;
									}
									low_pointer = high_pointer->point_right;

									if (current_element->prop == undef)									
										current_element->prop = vectr;		//тут может создаваться неопределённость.							
									else if (current_element->prop == arg_v)									
										current_element->prop = vectr_arg_v;
								}
								//в качестве аргументов есть переменные
								else
								{
									temp_pointer = new math_obj(low_pointer);
									if (multiple_var!=NULL)
									{
										multiple_var->prop = arg_v;
										temp_pointer->point_right = multiple_var;										
										if ((temp_pointer->var == 1) && (multiple_var->vector_size() != 1))
											temp_pointer->prop = vectr_arg_v;
										else
											temp_pointer->prop = arg_v;
										multiple_var = NULL;
									//указываем что среди аргументов есть переменные
									}
									//если список переменных не замкнут
									if (current_element->point_collar->point_left == NULL)
									{

										current_element->funct_arg_push_back(temp_pointer);
										if (temp_pointer->prop == vectr_arg_v)
											current_element->prop = vectr_arg_v;
										else if (current_element->prop == undef)
											current_element->prop = arg_v;
										temp_pointer = NULL;
										temp_pointer = current_element->var_list_back();
										temp_pointer->point_right = new math_obj(temporary_variable);
										temp_pointer->point_right->point_left = temp_pointer;
										temp_pointer->point_right->var = current_element->var;
										current_element->var += 1;

										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
										{
											high_pointer->point_right->point_right = new math_obj((int)0);
											multiple_var = new math_obj(temp_pointer->point_right);
											multiple_var->point_collar = temp_pointer->point_right;
											multiple_var->prop = L"";
											high_pointer->point_right->point_right->vector_push_back(multiple_var);
											high_pointer->point_right->prop = arg_v;
											high_pointer->point_right->point_right->prop = arg_v;
										}
										else
										{
											high_pointer->point_right = temp_pointer->point_right;
										}
										low_pointer = high_pointer->point_right;

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
												high_pointer->point_right->point_right = new math_obj((int)0);
												high_pointer->point_right->point_right->vector_push_back(temp_pointer);
												if (temp_pointer->prop == vectr_arg_v)
												{
													high_pointer->point_right->prop = vectr_arg_v;
													current_element->prop = vectr;
												}
												else
												{
													high_pointer->point_right->prop = arg_v;													
												}
												high_pointer->point_right->point_right->prop = arg_v;
											}
											else
											{
												if (temp_pointer->prop == vectr_arg_v)
													current_element->prop = vectr;												
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
								if ((multiple_var != NULL) && (multiple_var->prop == arg_c))
								{
									temp_pointer = new math_obj(low_pointer);									
									temp_pointer->point_right = multiple_var;									
									if ((temp_pointer->var == 1) && (multiple_var->vector_size() != 1))
									{
										temp_pointer->prop = vectr_arg_c;
										current_element->prop = vectr;
									}
									else									
										temp_pointer->prop = arg_c;//указываем что аргументы - константы									
									
									multiple_var = NULL;
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = new math_obj((int)0);
										high_pointer->point_right->point_right->vector_push_back(temp_pointer);
										if (temp_pointer->prop == vectr_arg_c)
											high_pointer->point_right->prop = vectr_arg_c;
										else
											high_pointer->point_right->prop = arg_c;
										high_pointer->point_right->point_right->prop = arg_c;
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
									if (multiple_var != NULL)
									{
										multiple_var->prop = arg_v;
										temp_pointer->point_right = multiple_var;										
										if ((temp_pointer->var == 1) && (multiple_var->vector_size() != 1))
											temp_pointer->prop = vectr_arg_v;
										else
											temp_pointer->prop = arg_v;
										multiple_var = NULL;
									}															
									current_element->funct_arg_push_back(temp_pointer); //полагаем новую неопределённую функцию, которая зависит от какой-либо переменной,
																								//имеющей аргумент в виде другой (определённой) функции. При этом если окажется, что
																								//в выражении участвуют несколько функций (f+g), то функция, котрая их объединяет
																								//будет считаться функцией нескольких переменных, 
																								//даже если все функции в её выражении зависят от одной и той же переменной.
																								//копия служебной переменной с указателем на функцию, которая иемеет номер ноль
									current_element->type = funct;
									if (temp_pointer->prop == vectr_arg_v)
										current_element->prop = vectr_arg_v;
									else
										current_element->prop = arg_v;									
									current_element->point_collar = new math_obj(temporary_variable);
									current_element->point_collar->var = 0; //просто для достоверности)
									current_element->point_collar->point_collar = current_element;

									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = new math_obj((int)0);
										multiple_var = new math_obj(current_element->point_collar);
										multiple_var->point_collar = current_element->point_collar;
										multiple_var->prop = L"";
										high_pointer->point_right->point_right->vector_push_back(multiple_var);
										high_pointer->point_right->prop = arg_v;
										high_pointer->point_right->point_right->prop = arg_v;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
									current_element->var = 1; //количество переменных

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
			math_obj* CE = build_tree(_pDest, _endPtr, _current_element, 0);
			wstring output;
			math_obj* temp;
			//size_t output_size;
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
				CE->actn = solve;
				general_var_const->push_back(CE);
			}
			else if (CE->type == funct)
			{
				if (CE->actn == solve)
				{
					if (CE->prop == arg_c)
					{
						CE->arithmetic();
						output = to_string(CE->var, var_type::FRACTIONAL, 2);
						CE->tree_destruct();
						delete CE;
					}
					else
					{
						output = CE->expresion(1);
					}
				}
				else if (CE->actn == write)
				{
					//опять же, видимой реакции от программы быть не должно. Забили функцию - записали. Возможно в отдельный поток отдать разложение сложной функции 
					//на элементарные. Можно и не в отдельный, врядли там будет высокая сложность вычислений

					if (CE->name.size() != 0)
					{
						CE->actn = L"";
						temp = run_vector(CE->name);
						if (temp == NULL)
						{
							general_var_const->push_back(CE);
						}
						else
						{
							temp->tree_destruct();
							for (size_t count = 0; count < general_var_const->size(); count++)
							{
								if (temp == general_var_const->at(count))
								{
									general_var_const->at(count) = CE;
									delete temp;
									break;
								}
							}
						}
					}
					else
					{
						//ошибка
					}
				}
				else
				{
					//сюда попасть не должны. На всякий случай оставил удаление
					CE->tree_destruct();
					delete CE;
				}
			}
			else if (CE->type == cnst)
			{
				if (CE->actn == solve)
				{
					//ответ 
					output = to_string(CE->var, var_type::FRACTIONAL, 5);

				}
				else if (CE->actn == write)
				{
					if (CE->prop == undef)
					{
						CE->tree_destruct();
						delete CE;
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
						ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
						ProjectError::GetProjectLastError(err);
						return err->GetErrorWStr();
					}
					else if (CE->prop == defnd)
					{
						CE->arithmetic();
						CE->tree_destruct();
						CE->actn = L"";
						temp = run_vector(CE->name);
						if (temp == NULL)
						{
							general_var_const->push_back(CE);
						}
						else
						{
							temp->tree_destruct();
							for (size_t count = 0; count < general_var_const->size(); count++)
							{
								if (temp == general_var_const->at(count))
								{
									general_var_const->at(count) = CE;
									delete temp;
									break;
								}
							}
						}
					}
				}
			}
			else if (CE->type == varbl)
			{
				delete CE;
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

			//size_t size_of_vect = general_var_const->size();


			general_var_const->back();

			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer	
			return analized_output(point_start, point_end, new math_obj(L"", exprs, undef, 0));
		}
	}
}
