#include "classes.h"

namespace Project {

	namespace Core {
		using namespace std;
		using namespace Project;
		using namespace Project::IO;
		class data_list;
		class math_obj;
		/*Конструктор дерева операций по строке символов типа wchar_t, имеющей начало strPtr и конец ePtr.
		Дополнительный параметр _pc присваивается полю point_collar (для получения дерева операций для функций с заранее обозначенным списком переменных).*/
		math_obj::math_obj(wchar_t* strPtr, wchar_t* endPtr, math_obj* _pc, data_list*data)
		{
			name = L"";
			if (_pc != NULL)
			{
				//если дан список переменных - функция
				type = funct;
				//число переменных
				var = _pc->point_collar->var;
			}
			else
			{
				type = exprs;
				var = 0;
			}
			prop = undef;
			actn = L"";
			point_left = NULL;
			point_right = NULL;
			point_collar = _pc;
			point_up = data;

			math_obj* t_p = NULL;
			//ищем равно в полученной строке
			wchar_t* temp = wcschr(strPtr, L'=');
			//не нашли равно
			if (temp == NULL)
			{
				t_p = build_tree(strPtr, endPtr);
				if (t_p == NULL)
				{
					/*так как всё происходит внутри конструктора в случае ошибки заполнения есть два варианта обработки ошибки:
					- использовать try при вызове конструктора и останавливать его вылетом. Потребуется сразу очищать память от ошибочного дереа операций.
					- всегда предполагать работу конструктора корректной, даже в случае ошибки, но присваивать ему type = err и потом обрабатывать ошибочный экземпляр класса (чистить память и т.д.).
						Можно будет попытаться сделать деструктор, который будет удалять нужное*/
					type = eror;
				}
				//больше ничего тут делать не надо. Если ошибок не было - конструктор выдаст какой-либо элемент
			}
			//нашли равно
			else
			{
				//смотрим на то что слева от равно. При этом проиходит заполнение текущего элемента в данном конструкторе.
				wstring str_left;
				str_left.assign(strPtr, temp);
				math_obj* left = build_tree(&str_left[0], &str_left[str_left.length() - 1]);
				if (left == NULL)
				{
					type = eror;
				}
				else
				{
					//слева находится функция с параметром "запись" и замкнутым списком переменных (заранее обозначенным). Вектор-функции здесь не рассматриваются (и не должны).
					if ((type == funct) && (actn == write) && (point_collar->point_left != NULL))
					{
						//список переменных будет передан в конструктор элемента для строки справа от равно. 					
						t_p = point_collar;
					}
					//смотрим на то что справа от равно. 
					//если строка закнчивается на равно
					if (temp == endPtr)
					{

						if ((actn == write) || ((type == varbl) && (prop == undef)))
						{
							type = eror;
						}
						/*Запрос на действие:
						- type == funct, prop == undef - запрос на "упрощение" выражения для неявно заданной функции.
						- type == funct, в остальных случаях - расчёт чего-либо или вывод выражения для функции, с упрощением.
						- type == cnst, вывод значения константы.
						- type == exprs, расчёт константного выражения и вывод на экран результата
						- type == varbl, prop==defnd - запрос на решение последнего введённого уравнения с данной переменной. Для неопределённых (новых) переменных - ошибка.
						- type == vectr - рассматривается аналогично для поля prop.*/
						else
						{
							actn = solve;
						}
					}
					//после равно что-то есть
					else
					{
						//Вызывается конструктор для нового элемента.
						wstring str_right;
						str_right.assign(temp + 1, endPtr + 1);
						math_obj* right = new math_obj(&str_right[0], &str_right[str_right.length() - 1], t_p, point_up);
						//отсеяли ошибочный элемент
						if (right->type == eror)
						{
							type = eror;
							//попытаться сделать деструктор, который будет удалять нужное
							delete right;
						}
						else if ((right->type == funct) && (right->actn == write))
						{
							type = eror;
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
							delete right;
						}
						//если справа от равно находится константное выражение, функция с конст аргументами или вектор имеющий только константные элементы
						//тогда автоматически знаем, что справа НЕ могут быть функции с параметром write
						else if ((right->type == exprs) || (right->type == cnst) || (right->type == numbr) || ((right->type == funct) && (right->prop == arg_c)) || ((right->type == vectr) && ((right->prop == exprs) || (right->prop == cnst) || (right->prop == arg_c))))
						{
							//если справа от равно находится константное выражение, функция с конст аргументами или вектор имеющий только константные элементы (но не сами константы и конст векторы с именем) или определённая заранее переменная - ошибка
							if ((type == exprs) || (type == numbr) || ((type == funct) && (prop == arg_c)) || ((type == vectr) && ((prop == exprs) || (prop == arg_c))))
							{
								type = eror;
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
								delete left;
							}
							else if ((type == varbl) && (prop == defnd))
							{
								type = eror;
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::USING_VAR_AS_CONST);
								delete left;
							}
							else if ((type == cnst) || ((type == vectr) && (prop == cnst)))
							{
								if (type == vectr)
								{
									//очистить константный вектор. Такой вектор должен сам по себе состоять из чисел или констант.			
									type = cnst;
									var = 0;
								}
								if (right->type == vectr)
								{
									copy(right);
									prop = cnst;
								}
								else
								{
									point_left = right;
									prop = defnd;
								}
								actn = write;
							}
							else if ((type == varbl) && (prop == undef))
							{								
								if (right->type == vectr)
								{
									copy(right);
									prop = cnst;
								}
								else
								{
									type = cnst;
									point_left = right;
									prop = defnd;
								}
								actn = write;
							}
							else if ((type == funct) || ((type == vectr) && (prop == funct)))
							{
								//Уравнение, надо дорабатывать случай с векторами
								t_p = new math_obj(this);
								convert_totaly(L"", equat, unslv, write, var, t_p, right, t_p->get_pc());
							}
						}
						else if (right->type == varbl)
						{
							if (right->prop == undef)
							{
								right->prop = defnd;
								point_up->push_left(new math_obj(right));								
							}
							if ((((type == funct) || (type == vectr) && (prop == funct)) && ((get_prop() == undef) || (get_prop() == arg_v) || (get_prop() == arg_c))) || (type == exprs))
							{
								//Уравнение, надо дорабатывать случай с векторами
								t_p = new math_obj(this);
								convert_totaly(L"", equat, unslv, write, var, t_p, right, t_p->get_pc());
							}
							else if (type == cnst)
							{
								convert_totaly(name, funct, defnd, write, 1, right, NULL, right);
								link_var_list_to_funct();
								close_list();
							}
							else if (type == varbl)
							{
								if (name.compare(right->name) == 0)
								{
									type = eror;
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::USING_VAR_AS_FUNC);
									delete right;
								}
								else
								{
									convert_totaly(name, funct, defnd, write, 1, right, NULL, right);
									link_var_list_to_funct();
									close_list();
								}
							}
							else if (((type == funct) || (type == vectr) && (prop == funct)) && (get_prop() == defnd))
							{
								if (type == vectr)
								{
									//очистить вектор. Такой вектор состоит из сервисных переменных, которые указывают на один и тот же элемент.			
									type = funct;
									var = 0;
								}
								//удалить дерево операций и набор переменных.
								convert_totaly(name, funct, defnd, write, 1, right, NULL, right);
								link_var_list_to_funct();
								close_list();
							}
						}
						else if ((right->type == funct) || (right->type == vectr) && (prop == funct))
						{
							if ((type == funct) && (actn == write))
							{
								if (right->type == vectr)
								{
									right->name = name;
									right->point_left->point_left->name = name;
									right->point_left->point_left->prop = defnd;
									copy(right);
									actn = write;
								}
								else
								{
									prop = defnd;
									point_left = right->point_left;
									link_var_list_to_funct(); //возможно нужно удостовериться в том, что бы эта операция здесь не требовалась
								}
								delete right; //тут чистить дерево операций не надо!								
							}

							else if (((type == funct) || (type == vectr) && (prop == funct)) && ((get_prop() == arg_c) || (get_prop() == undef) || ((get_prop() == arg_v) && (get_pr()->prop == arg_v))) || (type == exprs))
							{
								//Уравнение, надо дорабатывать случай с векторами
								t_p = new math_obj(this);
								convert_totaly(L"", equat, unslv, write, var, t_p, right, t_p->get_pc());
							}
							else if (((type == funct) || (type == vectr) && (prop == funct)) && (get_prop() == defnd))
							{
								if (type == vectr)
								{
									//очистить вектор. Такой вектор состоит из сервисных переменных, которые указывают на один и тот же элемент.			
									type = funct;
									var = 0;
								}
								//удалить дерево операций и набор переменных.
								if (right->type == vectr)
								{
									right->name = name;
									right->point_left->point_left->name = name;
									right->point_left->point_left->prop = defnd;
									copy(right);
									actn = write;
								}
								else
								{
									//TODO:сортировка переменных по алфавиту
									convert_totaly(name, funct, defnd, write, right->var, right->point_left, right->point_right, right->point_collar);
									link_var_list_to_funct();
								}
								if (get_pc()->point_left == NULL)
								{
									reassing_left_pointers(get_pc());
									close_list();
								}
								delete right; //тут чистить дерево операций не надо!
							}
							else if (((type == funct) || (type == vectr) && (prop == funct)) && ((get_prop() == arg_v) && (get_pr()->prop == only_arg_v)))
							{
								//если все переменные в списке аргументов функции справа появляются слева и наоборот.
								if ((right->var_list_compare(this) == 0) && (var_list_compare(right) == 0))
								{
									//удалить дерево операций и список переменных.
									if (right->type == vectr)
									{
										right->name = name;
										right->point_left->point_left->name = name;
										right->point_left->point_left->prop = defnd;
										copy(right);
										actn = write;
									}
									else
									{
										convert_totaly(name, funct, defnd, write, right->var, right->point_left, right->point_right, right->point_collar);
										link_var_list_to_funct();
									}
									if (get_pc()->point_left == NULL)
									{
										reassing_left_pointers(get_pc());
										close_list();
									}
									delete right; //тут чистить дерево операций не надо!
								}
								else
								{
									//Уравнение, надо дорабатывать случай с векторами
									t_p = new math_obj(this);
									convert_totaly(L"", equat, unslv, write, var, t_p, right, t_p->get_pc());
								}
							}
							else if ((type == cnst) || ((type == vectr) && (prop == cnst)) || ((type == varbl) && (prop == undef)))
							{
								if (type == vectr)
								{
									//очистить константный вектор. Такой вектор должен сам по себе состоять из чисел или констант.			
									type = cnst;
									var = 0;
								}
								if (right->type == vectr)
								{
									right->name = name;
									right->point_left->point_left->name = name;
									right->point_left->point_left->prop = defnd;
									copy(right);
									actn = write;
								}
								else
								{
									convert_totaly(name, funct, defnd, write, right->var, right->point_left, right->point_right, right->point_collar);
									link_var_list_to_funct();
								}
								if (get_pc()->point_left == NULL)
								{
									reassing_left_pointers(get_pc());
									close_list();
								}
								delete right; //тут чистить дерево операций не надо!
							}
							else if ((type == varbl) && (prop == defnd))
							{
								if (right->find_by_name(this) != NULL)
								{
									//Уравнение, надо дорабатывать случай с векторами
									t_p = new math_obj(this);
									convert_totaly(L"", equat, unslv, write, var, t_p, right, right->get_pc());
								}
								else
								{
									if (right->type == vectr)
									{
										right->name = name;
										right->point_left->point_left->name = name;
										right->point_left->point_left->prop = defnd;
										copy(right);
										actn = write;
									}
									else
									{
										convert_totaly(name, funct, defnd, write, right->var, right->point_left, right->point_right, right->point_collar);
										link_var_list_to_funct();
									}
									if (get_pc()->point_left == NULL)
									{
										reassing_left_pointers(get_pc());
										close_list();
									}
									delete right; //тут чистить дерево операций не надо!
								}
							}
						}
					}
				}
			}
		}


		/*PRIVATE. Метод строит дерево математических операций для данного математического объекта.
		Данный метод в общем случае может быть рекурсивен.
		Метод возвращает указатель на созданный экземпляр класса math_obj, представляющий собой верхушку (корень) дерева, либо на копию уже созданного экземпляра.
		NULL в случае какой-либо ошибки, см. error.h.*/
		math_obj* math_obj::build_tree(wchar_t* pDest, wchar_t*endPtr)
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
			//math_obj* current_element = this;
			while (pDest <= endPtr)
			{
				if ((*pDest == '1') || (*pDest == '2') || (*pDest == '3') || (*pDest == '4') || (*pDest == '5') || (*pDest == '6') || (*pDest == '7') || (*pDest == '8') || (*pDest == '9') || (*pDest == '0'))
				{
					// если данное число первое в записи выражения
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> константу
						point_left = new math_obj(numbr, real, wcstod(pDest, &pDest), NULL);
						//оба указателя -> на число, тебуется для проверки условия при записи операции
						low_pointer = point_left;
						high_pointer = low_pointer;
					}
					// если данное число не первое в записи (была какая-либо операция)
					else
					{
						//создание элемента класса и запись числа, воротник -> пред операцию 
						high_pointer->point_right = new math_obj(numbr, real, wcstod(pDest, &pDest), NULL);
						low_pointer = high_pointer->point_right;
					}
				}
				else if (*pDest == '+')
				{
					high_pointer = operations(high_pointer, low_pointer, addit);
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
					high_pointer = operations(high_pointer, low_pointer, mltpl);
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
					high_pointer = operations(high_pointer, low_pointer, divis);
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
					high_pointer = operations(high_pointer, low_pointer, power);
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
					//минус - просто отдельная операция. если минус оказался вначале строчки - предполагается вычитание из нуля
					high_pointer = operations(high_pointer, low_pointer, minus);
					if (low_pointer == NULL)
						low_pointer = high_pointer->point_left;
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
						//пустая строка в скобках. Считаестя нулём. 5*() = 0
						//скобка открывается в начале строки
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							//и закрывается в конце
							if ((temp + 1) == NULL)
							{
								//наверх выдаётся ноль
								convert_to(numbr, real, 0, NULL);
							}
							//и закрывается раньше чем конец строки - есть операции
							else
							{
								//ноль добавляется в выражение
								this->point_left = new math_obj(numbr, real, 0, NULL);
								high_pointer = this->point_left;
								low_pointer = high_pointer;
							}
						}
						//скобка открывается не в начале строки
						else
						{
							//создание элемента класса и запись нуля в выражение
							high_pointer->point_right = new math_obj(numbr, real, 0, NULL);
							low_pointer = high_pointer->point_right;
						}
					}
					//в скобках что-то есть и есть запятые
					else if (comma != 0)
					{
						//сразу же считаем содержимое скобок вектором
						temp_str.assign(pDest, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
						temp_pointer = new math_obj(comma + 1, &temp_str[0], &temp_str[temp_str.length() - 1], point_up);						
						if (temp_pointer->type == eror)
						{
							delete temp_pointer;
							return NULL;
						}
						//скобка открывается в начале строки
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							//и закрывается в конце
							if ((temp + 1) == NULL)
							{
								copy(temp_pointer);
								//наверх выдаётся сам созданный вектор, без проверки что в нём
								delete temp_pointer;
							}
							//далее какие-либо операции
							else
							{
								if ((temp_pointer->prop == arg_v) || (temp_pointer->prop == only_arg_v))
								{
									//текущий элемент - выражение
									if (type == exprs)
									{
										//high_pointer - вектор со списком переменных.
										high_pointer = temp_pointer->create_var_list(NULL);
										convert_to(funct, undef, high_pointer->var, high_pointer->point_left);
										delete high_pointer;
									}
									//текущий элемент - функция (если вектор попался в начале строки - функция с заранее заданными переменными)
									else if (type == funct)
									{
										high_pointer = new math_obj(L"", funct, arg_v, L"", temp_pointer->var, NULL, temp_pointer, NULL);
										if (var_list_compare(high_pointer) != 0)
										{
											delete high_pointer;
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											delete high_pointer;
											high_pointer = new math_obj((size_t)0);
											high_pointer->point_left = point_collar;
											high_pointer->prop = servc;
											high_pointer->var = var;
											high_pointer = temp_pointer->create_var_list(high_pointer);
											delete high_pointer;
										}
									}
								}
								//если вектор состоит только из констант - записываем в дерево операций, выражение или функция становятся векторными
								point_left = temp_pointer;
								buf = temp_pointer->var;
								temp_pointer = new math_obj(this);
								low_pointer = new math_obj((int)buf, temp_pointer);
								copy(low_pointer);
								delete low_pointer;
								link_var_list_to_funct();
								high_pointer = point_left;
								low_pointer = high_pointer;
							}
						}
						//скобка открывается не в начале строки
						else
						{
							//Делить на вектор нельзя! Данная проверка не сработает в случае записи 5/(7+(2,4)) - её нужно будеть делать в случае "обычных" скобок
							if (high_pointer->type == divis)
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIVISION);
								return NULL;
							}
							//Проверка на совпадения размерностей векторов
							if ((type == vectr) && (var != temp_pointer->var))
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
								return NULL;
							}
							if ((type == exprs) || ((type == vectr) && (prop == exprs)))
							{
								if ((temp_pointer->prop == arg_v) || (temp_pointer->prop == only_arg_v))
								{
									low_pointer = temp_pointer->create_var_list(NULL);
									convert_to(funct, undef, low_pointer->var, low_pointer->point_left);
									delete low_pointer;
								}
							}
							else if ((type == funct) || ((type == vectr) && (prop == funct)))
							{
								if ((temp_pointer->prop == arg_v) || (temp_pointer->prop == only_arg_v))
								{
									if (get_pc()->point_left != NULL)
									{
										low_pointer = new math_obj(L"", funct, arg_v, L"", temp_pointer->var, NULL, temp_pointer, NULL);
										if (var_list_compare(low_pointer) != 0)
										{
											delete low_pointer;
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											delete low_pointer;
											low_pointer = new math_obj((size_t)0);
											low_pointer->point_left = get_pc();
											low_pointer->prop = servc;
											low_pointer->var = get_var();
											low_pointer = temp_pointer->create_var_list(low_pointer);
											delete low_pointer;
										}
									}
									else
									{
										low_pointer = new math_obj((size_t)0);
										low_pointer->point_left = get_pc();
										low_pointer->prop = servc;
										low_pointer->var = get_var();
										low_pointer = temp_pointer->create_var_list(low_pointer);
										assing_var(low_pointer->var);
										delete low_pointer;
									}
								}
							}
							high_pointer->point_right = temp_pointer;
							low_pointer = high_pointer->point_right;
							if (type != vectr)
							{
								buf = temp_pointer->var;
								temp_pointer = new math_obj(this);
								low_pointer = new math_obj((int)buf, temp_pointer);
								copy(low_pointer);
								delete low_pointer;
							}
							link_var_list_to_funct();
						}
					}
					//если в скобках не было запятых.
					else
					{
						temp_str.assign(pDest + 1, temp);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки НЕ находились в строке.
						temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);
						if (temp_pointer == NULL)
						{
							return temp_pointer;
						}

						//скобка открывается в начале строки
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							//и закрывается в конце
							if ((temp + 1) == NULL)
							{
								copy(temp_pointer);
								delete temp_pointer;
								//return this;
							}
							//после скобки какие-либо операции
							else
							{
								//в начале заполнения текущий элемент не может быть вектором или матрицей
								//текущий элемент - выражение
								if (type == exprs)
								{
									if (temp_pointer->type == varbl)
									{
										if (temp_pointer->prop == undef)
										{
											temp_pointer->type = defnd;
											point_up->push_left(new math_obj(temp_pointer));
										}
										temp_pointer->var = 0;
										convert_to(funct, undef, 1, temp_pointer);
									}
									else if (temp_pointer->type == funct)
									{
										if (temp_pointer->actn == write)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										if (temp_pointer->prop != arg_c)
										{
											low_pointer = temp_pointer->create_var_list(NULL);
											convert_to(funct, undef, low_pointer->var, low_pointer->point_left);
											delete low_pointer;
										}
									}
									/*else if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst) || (temp_pointer->type == numbr))
									{
										point_left = temp_pointer;
									}*/
									else if (temp_pointer->type == vectr)
									{
										if ((temp_pointer->prop == arg_v) || (temp_pointer->prop == only_arg_v) || ((temp_pointer->prop == funct) && (temp_pointer->get_prop() != arg_c)))
										{
											//low_pointer - вектор со списком переменных.
											low_pointer = temp_pointer->create_var_list(NULL);
											convert_to(funct, undef, low_pointer->var, low_pointer->point_left);
											delete low_pointer;

										}
										//тут реализовать доп проверку на скалярное произведение векторов
										//если вектор состоит только из констант - записываем в дерево операций, выражение или функция становятся векторными																		
										buf = temp_pointer->var;
										multiple_var = new math_obj(this);
										low_pointer = new math_obj((int)buf, multiple_var);
										copy(low_pointer);
										delete low_pointer;
									}
									assing_pl(temp_pointer);
									high_pointer = get_pl();
									low_pointer = high_pointer;
									link_var_list_to_funct();
								}
								//текущий элемент - функция (если вектор попался в начале строки - функция с заранее заданными переменными)
								else if (type == funct)
								{
									if (temp_pointer->type == varbl)
									{
										low_pointer = find_by_name(temp_pointer);
										if (low_pointer == NULL)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										delete temp_pointer;
										temp_pointer = low_pointer;
									}
									else if (temp_pointer->type == funct)
									{
										if (temp_pointer->actn == write)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										if (temp_pointer->prop != arg_c)
										{
											if (var_list_compare(temp_pointer) != 0)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											low_pointer = new math_obj((size_t)0);
											low_pointer->point_left = point_collar;
											low_pointer->prop = servc;
											low_pointer->var = var;
											low_pointer = temp_pointer->create_var_list(low_pointer);
											delete low_pointer;
										}
									}
									/*else if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst) || (temp_pointer->type == numbr))
									{
										point_left = temp_pointer;
									}*/
									else if (temp_pointer->type == vectr)
									{

										if ((temp_pointer->prop == arg_v) || (temp_pointer->prop == only_arg_v) || ((temp_pointer->prop == funct) && (temp_pointer->get_prop() != arg_c)))
										{
											count_var = 0;
											if (temp_pointer->prop == funct)
												count_var = var_list_compare(temp_pointer);
											else
											{
												low_pointer = new math_obj(L"", funct, arg_v, L"", temp_pointer->var, NULL, temp_pointer, NULL);
												count_var = var_list_compare(low_pointer);
												delete low_pointer;
											}
											if (count_var != 0)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											else
											{
												low_pointer = new math_obj((size_t)0);
												low_pointer->point_left = point_collar;
												low_pointer->prop = servc;
												low_pointer->var = var;
												low_pointer = temp_pointer->create_var_list(low_pointer);
												delete low_pointer;
											}
										}
										//тут реализовать доп проверку на скалярное произведение векторов
										//если вектор состоит только из констант - записываем в дерево операций, выражение или функция становятся векторными										
										buf = temp_pointer->var;
										multiple_var = new math_obj(this);
										low_pointer = new math_obj((int)buf, multiple_var);
										copy(low_pointer);
										delete low_pointer;
										link_var_list_to_funct();
									}
									assing_pl(temp_pointer);
									high_pointer = get_pl();
									low_pointer = high_pointer;
								}
							}
						}
						//скобка открывается не в начале строки
						else
						{
							if ((type == exprs) || ((type == vectr) && (prop == exprs)))
							{
								if (temp_pointer->type == varbl)
								{
									if (temp_pointer->prop == undef)
									{
										temp_pointer->type = defnd;
										//general_var_const->push_back(new math_obj(temp_pointer));
									}
									temp_pointer->var = 0;
									convert_to(funct, undef, 1, temp_pointer);
								}
								else if (temp_pointer->type == funct)
								{
									if (temp_pointer->actn == write)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									if (temp_pointer->prop != arg_c)
									{
										low_pointer = temp_pointer->create_var_list(NULL);
										convert_to(funct, undef, temp_pointer->var, low_pointer->point_left);
										delete low_pointer;
									}
								}
								/*else if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
								{
								//тут всё равно делать ничего не надо. Закоментил
								//current_element->assing_pl(temp_pointer);
								}*/
								else if (temp_pointer->type == vectr)
								{
									if ((type == vectr) && (var != temp_pointer->var))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
										return NULL;
									}

									if ((temp_pointer->prop == arg_v) || (temp_pointer->prop == only_arg_v) || ((temp_pointer->prop == funct) && (temp_pointer->get_prop() != arg_c)))
									{
										//low_pointer - вектор со списком переменных.
										low_pointer = temp_pointer->create_var_list(NULL);
										convert_to(funct, undef, low_pointer->var, low_pointer->point_left);
										delete low_pointer;
									}
									//если вектор состоит только из констант - записываем в дерево операций, выражение или функция становятся векторными									
									//возможно для temp_pointer->var потребуется буфер
									if (type != vectr)
									{
										buf = temp_pointer->var;
										multiple_var = new math_obj(this);
										low_pointer = new math_obj((int)buf, multiple_var);
										copy(low_pointer);
										delete low_pointer;
									}
								}
								link_var_list_to_funct();
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
							else if ((type == funct) || ((type == vectr) && (prop == funct)))
							{
								if (temp_pointer->type == varbl)
								{
									low_pointer = find_by_name(temp_pointer);
									if (low_pointer == NULL)
									{
										if (get_pc()->point_left != NULL)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->type = defnd;
												//general_var_const->push_back(new math_obj(temp_pointer));
											}
											var_list_push_back(temp_pointer);
										}
									}
									else
									{
										delete temp_pointer;
										temp_pointer = low_pointer;
									}
								}
								else if (temp_pointer->type == funct)
								{
									if (temp_pointer->actn == write)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									if (temp_pointer->prop != arg_c)
									{
										if ((var_list_compare(temp_pointer) != 0) && (get_pc()->point_left != NULL))
										{

											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										//должно сработать в любом случае
										low_pointer = new math_obj((size_t)0);
										low_pointer->point_left = get_pl();
										low_pointer->prop = servc;
										low_pointer->var = get_var();
										low_pointer = temp_pointer->create_var_list(low_pointer);
										delete low_pointer;
									}
								}
								else if (temp_pointer->type == vectr)
								{
									if ((temp_pointer->prop == arg_v) || (temp_pointer->prop == only_arg_v) || ((temp_pointer->prop == funct) && (temp_pointer->get_prop() != arg_c)))
									{
										if ((type == vectr) && (var != temp_pointer->var))
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
											return NULL;
										}
										count_var = 0;
										if (temp_pointer->prop == funct)
											count_var = var_list_compare(temp_pointer);
										else
										{
											low_pointer = new math_obj(L"", funct, arg_v, L"", temp_pointer->var, NULL, temp_pointer, NULL);
											count_var = var_list_compare(low_pointer);
											delete low_pointer;
										}
										if ((count_var != 0) && (get_pc()->point_left != NULL))
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										low_pointer = new math_obj((size_t)0);
										low_pointer->point_left = get_pl();
										low_pointer->prop = servc;
										low_pointer->var = get_var();
										low_pointer = temp_pointer->create_var_list(low_pointer);
										delete low_pointer;
									}
									//если вектор состоит только из констант - записываем в дерево операций, выражение или функция становятся векторными
									if (type != vectr)
									{
										buf = temp_pointer->var;
										multiple_var = new math_obj(this);
										low_pointer = new math_obj((int)buf, multiple_var);
										copy(low_pointer);
										delete low_pointer;
									}
								}
								link_var_list_to_funct();
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
						}
					}

					//сразу переходим к следующей позиции после скобки					
					pDest = temp + 1;
					if ((*pDest == '(') || (*pDest == ','))       //*pDest == '(,'
					{
						//по идее запись вида )( может означать )*(. Потом добавть сюда определение для подобного случая
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
						return NULL;
					}
				}
				else if (*pDest == '[')
				{
					//векторное произведение
				}
				//случай с символами в строке
				else
				{
					temp = wcspbrk(pDest, L"()+-*^/");
					//если строчка начинается с cимвола
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						if (temp == NULL)
						{
							name_str.assign(pDest, endPtr + 1);
							pDest = endPtr + 1;
						}
						else
						{
							name_str.assign(pDest, temp);
							pDest = temp;
						}						
						high_pointer = point_up->find_math_obj(&name_str);
						//если не найден ни один элемент массива мат объектов с таким именем
						if (high_pointer == NULL)
						{
							//считаем найденный элемент переменной
							if (temp == NULL)
							{
								convert_totaly(name_str, varbl, undef, L"", 0, NULL, NULL, NULL);
							}
							//больше случай с равно не рассматривается //else if (*temp == '=')
							//считаем найденный элемент функцией
							else if (*temp == '(')
							{
								temp = brackets_close(temp, endPtr);
								if (temp == NULL)
								{
									return NULL;
								}
								//pDest указывает на открывающую скобку, temp - на закрывающую
								temp_str.assign(pDest, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
								temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], (math_obj*)NULL, point_up);
								if (temp_pointer->type == eror)
								{
									delete temp_pointer;
									return NULL;
								}
								//поскольку функция попалась неизвестная: если после скобки строка не заканчивается - считаем запись ошибкой
								if (temp != endPtr)
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								if ((temp_pointer->type == numbr) && (temp_pointer->var == 0))
								{
									//пустая строка в скобках.  создание функции от неопределённого количества переменных
									//аналогично заданию функции вообще без указания переменных. Указатель на переменные - пустой
									convert_totaly(name_str, funct, undef, write, 0, NULL, NULL, NULL);
									//если получили запись вида f(), где f - неизвестная функция и в скобках ничего нет - передаём ей параметр write. 
									delete temp_pointer; temp_pointer = NULL; //этот ноль больше нам не нужен								
								}
								//если в скобках вектор, то он может состоять только из переменных. Любой другой случай - передача аргументов необъявленной функции
								else if ((temp_pointer->type == vectr) && (temp_pointer->prop == only_arg_v))
								{
									temp_pointer->double_lincked_vector();
									convert_totaly(name_str, funct, undef, write, temp_pointer->var, NULL, NULL, temp_pointer->point_left);
									//если получили запись вида f(), где f - неизвестная функция - передаём ей параметр write.
									delete temp_pointer; temp_pointer = NULL;
									link_var_list_to_funct();
									close_list();
								}
								//в скобках переменная - определяем функцию одного переменного
								else if (temp_pointer->type == varbl)
								{
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										point_up->push_left(new math_obj(temp_pointer));
									}
									temp_pointer->var = 0;
									temp_pointer->point_collar = this;
									convert_totaly(name_str, funct, undef, write, 1, NULL, NULL, temp_pointer);
									close_list();
								}
								else
								{
									//вылезло что угодно другое - ошибка
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								high_pointer = this;
								low_pointer = high_pointer;
								pDest = temp + 1;
							}
							//после буквосочетания идут операции
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								//найденное буквосочетание - переменная, текущий элемент - функция							
								temp_pointer = new math_obj(name, varbl, defnd, 0);
								point_up->push_left(new math_obj(temp_pointer));
								//general_var_const->push_back(new math_obj(name, varbl, defnd, 0));
								//копия переменной с указателем на функцию
								temp_pointer->point_collar = this;
								convert_totaly(L"", funct, undef, L"", 1, temp_pointer, NULL, temp_pointer);
								high_pointer = get_pl();
								low_pointer = high_pointer;
							}
							if (*temp == ')')
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//после буквосочетания идут какие-то другие символы (внезапно)
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}

						}
						//найден элемент массива мат объектов с совпадающим именем - константа
						else if (high_pointer->type == cnst)
						{
							//оттадём наверх копиию константы
							if (temp == NULL)
							{
								copy(high_pointer);
								high_pointer = this;
								low_pointer = high_pointer;
							}
							//тут идея в том, что если уже есть какая-то константа - какие бы ни были аргументы для неё - ответом будет являться константа.
							//однако данная запись может означать переопределение конст в виде функции
							else if (*temp == '(')
							{
								temp = brackets_close(temp, endPtr);
								if (temp == NULL)
								{
									return NULL;
								}
								//если после скобки строка не закнчивается (далее какие-либо операции)
								if (temp != endPtr)
								{
									//константа остаётся константой, содержимое скобок игнорируется, нет смысла вызывать метод заполнения для того что в скобках
									if ((*(temp + 1) == '+') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^') || (*(temp + 1) == '-'))
									{
										point_left = new math_obj(high_pointer);
										high_pointer = point_left;
										low_pointer = high_pointer;
									}
									/*else if ((*(temp + 1) == '('))
									{
									//пока оставлю зто условие. Запись типа с(...)(...) - обычно это означает произведение с()*(...)
									//TODO: временно игнорируется возможность с(...) = с*(...). доработать.
									}*/
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
										return NULL;
									}
								}
								//строка заканчивается скобкой
								else
								{
									//pDest указывает на открывающую скобку, temp - на закрывающую
									temp_str.assign(pDest, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
									temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);
									if (temp_pointer->type == eror)
									{
										delete temp_pointer;
										return NULL;
									}
									if ((temp_pointer->type == numbr) && (temp_pointer->var == 0))
									{
										//пустая строка в скобках.  создание функции от неопределённого количества переменных
										//аналогично заданию функции вообще без указания переменных. Указатель на переменные - пустой
										convert_totaly(name_str, funct, undef, write, 0, NULL, NULL, NULL);
										//если получили запись вида f(), где f - ранее обозначенная константа и в скобках ничего нет - передаём ей параметр write. 
										delete temp_pointer; temp_pointer = NULL; //этот ноль больше нам не нужен
										//здесь намеренно игнорируется возможность с() = с*() = с*0 = 0. 
									}
									//если в скобках вектор, то он может состоять только из переменных. Любой другой случай - константа не поменяется
									else if ((temp_pointer->type == vectr) && (temp_pointer->prop == only_arg_v))
									{
										temp_pointer->double_lincked_vector();
										convert_totaly(name_str, funct, undef, write, temp_pointer->var, NULL, NULL, temp_pointer->point_left);
										//если получили запись вида f(), где f - ранее обозначенная константа - передаём ей параметр write.
										delete temp_pointer; temp_pointer = NULL;
										link_var_list_to_funct();
										close_list();
									}
									//в скобках переменная - определяем функцию одного переменного
									else if (temp_pointer->type == varbl)
									{
										if (temp_pointer->prop == undef)
										{
											temp_pointer->prop = defnd;
											point_up->push_left(new math_obj(temp_pointer));
										}
										temp_pointer->var = 0;
										temp_pointer->point_collar = this;
										convert_totaly(name_str, funct, undef, write, 1, NULL, NULL, temp_pointer);
										close_list();
									}
									else
									{
										//вылезло что угодно другое - this -> копия найденной константы
										//TODO: временно игнорируется возможность с(...) = с*(...). доработать.
										copy(high_pointer);
									}
									high_pointer = this;
									low_pointer = high_pointer;
									pDest = temp + 1;
								}

							}
							else if (*temp == ')')
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//текущий элемент остаётся выражением, найденная константа (её копия) - один из операндов
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								point_left = new math_obj(high_pointer);
								high_pointer = point_left;
								low_pointer = high_pointer;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
						//найден элемент массива мат объектов с совпадающим именем - переменная
						else if (high_pointer->type == varbl)
						{
							if (temp == NULL)
							{
								if ((type == funct) && (point_collar != NULL))
								{
									if ((var != 1) || (point_collar->name.compare(high_pointer->name) != 0))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									point_left = point_collar;
								}
								else
								{
									copy(high_pointer);
									high_pointer = this;
									low_pointer = high_pointer;
								}
							}
							else if (*temp == ')')
							{

								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//такая запись может означать ТОЛЬКО переопределение переменной в качестве функции нового аргумента
							else if (*temp == '(')
							{
								temp = brackets_close(temp, endPtr);
								if (temp == NULL)
								{
									return NULL;
								}
								//если после скобки строка не закнчивается (далее какие-либо операции)
								if (temp != endPtr)
								{
									//если в самом начале строки стоит переменная и далее скобка - считаем что это определение новой функции
									//если после этого ничего нет - ошибка.
									//TODO: временно игнорируется возможность x(...) = x*(...). доработать.	
									//и переместить проверку данного условия ниже. оно сейчас здесь, чтобы не вызывался метод заполнения для содержимого скобок
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								//pDest указывает на открывающую скобку, temp - на закрывающую
								temp_str.assign(pDest, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
								temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);
								if (temp_pointer->type == eror)
								{
									delete temp_pointer;
									return NULL;
								}
								if ((temp_pointer->type == numbr) && (temp_pointer->var == 0))
								{
									//пустая строка в скобках.  создание функции от неопределённого количества переменных
									//аналогично заданию функции вообще без указания переменных. Указатель на переменные - пустой
									convert_totaly(name_str, funct, undef, write, 0, NULL, NULL, NULL);
									//если получили запись вида f(), где f - ранее обозначенная перменная и в скобках ничего нет - передаём ей параметр write. 
									delete temp_pointer; temp_pointer = NULL; //этот ноль больше нам не нужен
									//здесь намеренно игнорируется возможность x() = x*() = x*0 = 0. 
								}
								//если в скобках вектор, то он может состоять только из переменных. Любой другой случай - константа не поменяется
								else if ((temp_pointer->type == vectr) && (temp_pointer->prop == only_arg_v))
								{
									//TODO: проверять чтобы в векторе с переменными не было переменной с тем же именем (и вообще они не должны повторятся)
									temp_pointer->double_lincked_vector();
									convert_totaly(name_str, funct, undef, write, temp_pointer->var, NULL, NULL, temp_pointer->point_left);
									//если получили запись вида f(), где f - ранее обозначенная переменная - передаём ей параметр write.
									delete temp_pointer; temp_pointer = NULL;
									link_var_list_to_funct();
									close_list();
								}
								//в скобках переменная - определяем функцию одного переменного
								else if (temp_pointer->type == varbl)
								{
									//Проверка: запись вида x(x).
									if (temp_pointer->name == name_str)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::USING_VAR_AS_FUNC);
										return NULL;
									}
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										point_up->push_left(new math_obj(temp_pointer));
									}
									temp_pointer->var = 0;
									temp_pointer->point_collar = this;
									convert_totaly(name_str, funct, undef, write, 1, NULL, NULL, temp_pointer);
									close_list();
								}
								else
								{
									//вылезло что угодно другое - ошибка
									//TODO: временно игнорируется возможность x(...) = x*(...). доработать.
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								high_pointer = this;
								low_pointer = high_pointer;
								pDest = temp + 1;
							}
							//текущий элемент становится функцией от найденной переменной
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								//найденное буквосочетание - переменная, текущий элемент - функция							
								temp_pointer = new math_obj(high_pointer);
								temp_pointer->point_collar = this;
								convert_totaly(L"", funct, undef, L"", 1, temp_pointer, NULL, temp_pointer);
								high_pointer = get_pl();
								low_pointer = high_pointer;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
						//найден элемент массива мат объектов с совпадающим именем - функция
						else if (high_pointer->type == funct)
						{
							if (temp == NULL)
							{
								copy(high_pointer);
								high_pointer = this;
								low_pointer = high_pointer;
								//данная функция может быть использована в каком-то уравнении/другой функции в качестве операнда. 
								//может так же означать её переопределение с неизвестным количеством переменных							
							}
							/*в данном случае открытая скобка после названия уже определённой ф-ции (н-р f(x)) может значить:
							--новое определение, в том числе от новых переменных
							--передачу функции аргументов, как константных выражений, так и новых переменных. В этом случае считаем,
								что подразумевается f(t)= - запрос на вывод выражения от t, либо участие f в уравнении с переменной t, за искл. случая с переопределением.
							--передачу функции в качестве аргумента новую функцию g(s) - при этом получившаяся функция f(g(s)) должна всюду зависеть от s, а не от х,
								однако должна состоять из копий оригинальных функций, исключение g(s) - неявно задана - f(5*s+2)*/
							else if (*temp == '(')
							{
								temp = brackets_close(temp, endPtr);
								if (temp == NULL)
								{
									return NULL;
								}
								//pDest указывает на открывающую скобку, temp - на закрывающую
								temp_str.assign(pDest, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
								temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);
								if (temp_pointer->type == eror)
								{
									delete temp_pointer;
									return NULL;
								}
								if ((temp_pointer->type == numbr) && (temp_pointer->var == 0))
								{
									if (temp == endPtr)
									{
										copy(high_pointer);
										high_pointer = this;
										low_pointer = high_pointer;
									}
									else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
									{
										high_pointer = new math_obj(high_pointer);
										//low_pointer - список переменных high_pointer, в том числе внутри других функций
										low_pointer = high_pointer->create_var_list(NULL);
										convert_totaly(L"", funct, undef, L"", low_pointer->var, high_pointer, NULL, low_pointer->point_left);
										delete low_pointer;
										link_var_list_to_funct();
										high_pointer = point_left;
										low_pointer = high_pointer;
									}
									delete temp_pointer;
								}
								else if (temp_pointer->type == varbl)
								{
									if (temp_pointer->prop == undef)
									{										
										temp_pointer->prop = defnd;
										point_up->push_left(new math_obj(temp_pointer));
									}
									if (high_pointer->var == 1)
									{
										if (temp == endPtr)
										{
											copy(high_pointer);
											prop = arg_v;
											low_pointer = new math_obj((size_t)0);
											low_pointer->vector_push_back(temp_pointer);
											low_pointer->prop = only_arg_v;
											point_right = low_pointer;
											high_pointer = this;
											low_pointer = high_pointer;
										}
										else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
										{
											high_pointer = new math_obj(high_pointer);
											high_pointer->prop = arg_v;
											low_pointer = new math_obj((size_t)0);
											low_pointer->vector_push_back(temp_pointer);
											low_pointer->prop = only_arg_v;
											high_pointer->point_right = low_pointer;
											//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
											low_pointer = high_pointer->create_var_list(NULL);
											convert_totaly(L"", funct, undef, L"", low_pointer->var, high_pointer, NULL, low_pointer->point_left);
											delete low_pointer;
											link_var_list_to_funct();
											high_pointer = point_left;
											low_pointer = high_pointer;
										}
									}
									else
									{
										if (temp != endPtr)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										convert_totaly(name_str, funct, undef, write, 1, NULL, NULL, temp_pointer);
										//если получили запись вида f(), где f - ранее обозначенная функция - передаём ей параметр write.
										delete temp_pointer; temp_pointer = NULL;
										link_var_list_to_funct();
										close_list();
										high_pointer = this;
										low_pointer = high_pointer;
									}
								}
								else if ((temp_pointer->type == cnst) || ((temp_pointer->type == funct) && (temp_pointer->prop == arg_c)) || (temp_pointer->type == exprs))
								{
									if (high_pointer->var == 1)
									{
										if (temp == endPtr)
										{
											copy(high_pointer);
											prop = arg_c;
											low_pointer = new math_obj((size_t)0);
											low_pointer->vector_push_back(temp_pointer);
											low_pointer->prop = arg_c;
											point_right = low_pointer;
											high_pointer = this;
											low_pointer = high_pointer;
										}
										else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
										{
											low_pointer = new math_obj((size_t)0);
											low_pointer->vector_push_back(temp_pointer);
											low_pointer->prop = arg_c;
											point_left = new math_obj(high_pointer);
											point_left->prop = arg_c;
											point_left->point_right = low_pointer;
											high_pointer = point_left;
											low_pointer = high_pointer;
										}
									}
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
								}
								else if (temp_pointer->type == funct)
								{
									if (temp_pointer->actn == write)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									if (temp == endPtr)
									{
										copy(high_pointer);
										prop = arg_v;
										low_pointer = new math_obj((size_t)0);
										low_pointer->vector_push_back(temp_pointer);
										low_pointer->prop = arg_v;
										point_right = low_pointer;
										high_pointer = this;
										low_pointer = high_pointer;
									}
									else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
									{
										high_pointer = new math_obj(high_pointer);
										high_pointer->prop = arg_v;
										low_pointer = new math_obj((size_t)0);
										low_pointer->vector_push_back(temp_pointer);
										low_pointer->prop = arg_v;
										high_pointer->point_right = low_pointer;
										//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
										low_pointer = high_pointer->create_var_list(NULL);
										convert_totaly(L"", funct, undef, L"", low_pointer->var, high_pointer, NULL, low_pointer->point_left);
										delete low_pointer;
										link_var_list_to_funct();
										high_pointer = point_left;
										low_pointer = high_pointer;
									}
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
								}
								else if (temp_pointer->type == vectr)
								{
									//число аргументов в скобках совпадает с числом аргументов у определённой заранее функции
									if (high_pointer->var == temp_pointer->var)
									{
										//подаём полученный вектор в качестве аргументов данной функции (даже если он состоит только из переменных
										if ((temp_pointer->prop == arg_v) || (temp_pointer->prop == only_arg_v))
										{
											//если строка заканчивается - отдаём наверх найденную функцию с параметром arg_v
											if (temp == endPtr)
											{
												copy(high_pointer);
												prop = arg_v;
												point_right = temp_pointer;
												high_pointer = this;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												high_pointer = new math_obj(high_pointer);
												high_pointer->point_right = temp_pointer;
												high_pointer->prop = arg_v;
												//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
												low_pointer = high_pointer->create_var_list(NULL);
												convert_totaly(L"", funct, undef, L"", low_pointer->var, high_pointer, NULL, low_pointer->point_left);
												delete low_pointer;
												link_var_list_to_funct();
												high_pointer = point_left;
												low_pointer = high_pointer;
											}
											else
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
												return NULL;
											}
										}
										//все аргументы не переменные
										else if (temp_pointer->prop == arg_c)
										{
											if (temp == endPtr)
											{
												copy(high_pointer);
												prop = arg_c;
												point_right = temp_pointer;
												high_pointer = this;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												point_left = new math_obj(high_pointer);
												point_left->prop = arg_c;
												point_left->point_right = temp_pointer;
												high_pointer = point_left;
												low_pointer = high_pointer;
											}
											else
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
												return NULL;
											}
										}
									}
									/*число переменных функции отличается от количества элементов вектора в скобках*/
									else
									{
										//функция одного переменного трансформируется в вектор-функцию
										if (high_pointer->var == 1)
										{
											buf = temp_pointer->var;
											if ((temp_pointer->prop == arg_v) || (temp_pointer->prop == only_arg_v) || ((temp_pointer->prop == funct) && (temp_pointer->get_prop() != arg_c)))
											{
												//если строка заканчивается - отдаём наверх найденную функцию с параметром arg_v
												if (temp == endPtr)
												{
													copy(high_pointer);
													prop = arg_v;
													point_right = temp_pointer;
													high_pointer = this;
													low_pointer = high_pointer;
												}
												else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													high_pointer = new math_obj(high_pointer);
													high_pointer->point_right = temp_pointer;
													high_pointer->prop = arg_v;
													//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
													low_pointer = high_pointer->create_var_list(NULL);
													temp_pointer = new math_obj((int)buf, high_pointer);
													convert_totaly(L"", funct, undef, L"", low_pointer->var, temp_pointer, NULL, low_pointer->point_left);
													delete low_pointer;
													link_var_list_to_funct();
													high_pointer = point_left;
													low_pointer = high_pointer;
												}
												else
												{
													ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
													return NULL;
												}
											}
											//все аргументы не переменные
											else //if ((temp_pointer->prop == arg_c) || ((temp_pointer->prop == funct) && (temp_pointer->get_prop() == arg_c))|| (temp_pointer->prop == exps) || (temp_pointer->prop == cnst))))
											{
												if (temp == endPtr)
												{
													copy(high_pointer);
													prop = arg_c;
													point_right = temp_pointer;
													high_pointer = this;
													low_pointer = high_pointer;
												}
												else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													high_pointer = new math_obj(high_pointer);
													high_pointer->point_right = temp_pointer;
													high_pointer->prop = arg_c;
													temp_pointer = new math_obj((int)buf, high_pointer);
													point_left = temp_pointer;
													high_pointer = point_left;
													low_pointer = high_pointer;
												}
												else
												{
													ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
													return NULL;
												}
											}
											temp_pointer = new math_obj(this);
											multiple_var = new math_obj((int)buf, temp_pointer);
											copy(multiple_var);
											delete multiple_var;
										}
										//функция нескольких переменных - подразумевается переопределение
										else
										{
											//значит после переопределяемой функции строка должна закончиться. Так же вектор должен состоять только из переменных
											if ((temp != endPtr) || (temp_pointer->prop != only_arg_v))
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											temp_pointer->double_lincked_vector();
											convert_totaly(name_str, funct, undef, write, temp_pointer->var, NULL, NULL, temp_pointer->point_left);
											//если получили запись вида f(), где f - ранее обозначенная перменная - передаём ей параметр write.
											delete temp_pointer; temp_pointer = NULL;
											link_var_list_to_funct();
											close_list();
											high_pointer = this;
											low_pointer = high_pointer;
										}
									}
								}
							}
							else if (*temp == ')')
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								high_pointer = new math_obj(high_pointer);
								//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
								low_pointer = high_pointer->create_var_list(NULL);
								convert_totaly(L"", funct, undef, L"", low_pointer->var, high_pointer, NULL, low_pointer->point_left);
								delete low_pointer;
								link_var_list_to_funct();
								high_pointer = point_left;
								low_pointer = high_pointer;

							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
						else if (high_pointer->type == vectr)
						{
							//попробовать объединить с предыдущими случаями
						}
					}
					//символ находится не в начале строки
					else
					{
						if (temp == NULL)
						{
							name_str.assign(pDest, endPtr + 1);
							pDest = endPtr + 1;
						}
						else
						{
							name_str.assign(pDest, temp);
							pDest = temp;
						}								
						low_pointer = point_up->find_math_obj(&name_str);
						//далее всегда может быть только два варианта - текщуий элемент либо функция(вектор-функция), либо выражение (векторное), причём всегда undef.
						//если не найден ни один элемент массива с таким именем
						if (low_pointer == NULL)
						{
							if ((temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{
								if ((type == funct) || ((type == vectr) && (prop == funct)))
								{
									//если текущий элемент - функция, то очевидно найдена новая переменная в записи выражения. 
									if (get_pc()->point_left != NULL)
									{
										//значит список переменных замкнут => новая переменная - лишняя
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}									
									temp_pointer = new math_obj(name, varbl, defnd, 0);
									point_up->push_left(new math_obj(temp_pointer));
									var_list_push_back(temp_pointer);
									//поскольку символ не в начале строки - значит стоит после какой-либо операции. 
									high_pointer->point_right = temp_pointer;
									low_pointer = high_pointer->point_right;
									temp_pointer = NULL;
								}
								else if ((type == exprs) || ((type == vectr) && (prop == exprs)))
								{									
									temp_pointer = new math_obj(name, varbl, defnd, L"", 0, NULL, NULL, NULL);
									point_up->push_left(new math_obj(temp_pointer));
									temp_pointer->point_collar = this;
									convert_totaly(L"", funct, undef, L"", 1, temp_pointer, NULL, temp_pointer);
									high_pointer->point_right = point_collar;
									low_pointer = high_pointer->point_right;
								}
							}
							else if ((*temp == '(') || (*temp == ')') || (*temp == ','))
							{
								//TODO: временно игнорируется возможность x(...) = x*(...). доработать.
								//ошибка - после переменной стоит открывающаяся скобка. Вообще не совсем бессмысленная запись, возможно нужен другой код ошибки или придумать как такое исключение обрабатывать.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
						}
						//найден элемент массива с совпадающим именем - переменная
						else if (low_pointer->type == varbl)
						{
							if ((temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{
								if ((type == funct) || ((type == vectr) && (prop == funct)))
								{
									temp_pointer = find_by_name(low_pointer);
									if (temp_pointer != NULL)
									{
										high_pointer->point_right = temp_pointer;
										low_pointer = high_pointer->point_right;
									}
									else if (get_pc()->point_left == NULL)
									{
										//список не замкнут - можно добавить больше переменных										
										temp_pointer = new math_obj(low_pointer);
										var_list_push_back(temp_pointer);
										high_pointer->point_right = temp_pointer;
										low_pointer = high_pointer->point_right;
										temp_pointer = NULL;
									}
									else
									{
										//значит список переменных замкнут => новая переменная - лишняя
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
								}
								else if ((type == exprs) || ((type == vectr) && (prop == exprs)))
								{
									temp_pointer = new math_obj(low_pointer);
									convert_totaly(L"", funct, undef, L"", 1, temp_pointer, NULL, temp_pointer);
									link_var_list_to_funct();
									high_pointer->point_right = temp_pointer;
									low_pointer = high_pointer->point_right;
								}
							}
							else if ((*temp == '(') || (*temp == ')') || (*temp == ','))
							{
								//TODO: временно игнорируется возможность x(...) = x*(...). доработать.
								//ошибка - после переменной стоит открывающаяся скобка. Вообще не совсем бессмысленная запись, возможно нужен другой код ошибки или придумать как такое исключение обрабатывать.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
						}
						//найден элемент массива с совпадающим именем - константа - просто записываем её в выражение
						else if (low_pointer->type == cnst)
						{
							if ((temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{
								high_pointer->point_right = new math_obj(low_pointer);
								low_pointer = high_pointer->point_right;
							}
							else if ((*temp == '(') || (*temp == ')') || (*temp == ','))
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
								//TODO: временно игнорируется возможность c(...) = c*(...) доработать.
							}
						}
						//найден элемент массива с совпадающим именем - функция 
						else if (low_pointer->type == funct)
						{
							if ((*temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '='))
							{
								multiple_var = NULL;
							}
							else if (*temp == '(')
							{
								temp = brackets_close(temp, endPtr);
								if (temp == NULL)
								{
									return NULL;
								}
								//pDest указывает на открывающую скобку, temp - на закрывающую
								temp_str.assign(pDest, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
								temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);
								if (temp_pointer->type == eror)
								{
									delete temp_pointer;
									return NULL;
								}
								if ((temp_pointer->type == numbr) && (temp_pointer->var == 0))
								{
									if ((*(temp + 1) == NULL) || (*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
										multiple_var = NULL;
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
										return NULL;
									}
								}
								else if (temp_pointer->type == vectr)
								{
									if ((low_pointer->var == 1) || (temp_pointer->var == low_pointer->var))
									{
										multiple_var = temp_pointer;
									}
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
								}
								else if (temp_pointer->type == varbl)
								{
									if (low_pointer->var != 1)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									if (temp_pointer->prop == undef)
									{										
										temp_pointer->prop == defnd;
										point_up->push_left(new math_obj(temp_pointer));
									}
									multiple_var = new math_obj((size_t)0);
									multiple_var->vector_push_back(temp_pointer);
									multiple_var->prop = only_arg_v;
								}
								else if ((temp_pointer->type == cnst) || ((temp_pointer->type == funct) && (temp_pointer->prop == arg_c)) || (temp_pointer->type == exprs))
								{
									multiple_var = new math_obj((size_t)0);
									multiple_var->vector_push_back(temp_pointer);
									multiple_var->prop = arg_c;
								}
								else if (temp_pointer->type == funct)
								{
									if (temp_pointer->actn == write)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									multiple_var = new math_obj((size_t)0);
									multiple_var->vector_push_back(temp_pointer);
									multiple_var->prop = arg_v;
								}
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
							pDest = temp + 1;
							//если все аргументы полученные от функции либо константы, либо конст. выражения, либо функции с конст аргументами
							if ((multiple_var != NULL) && ((multiple_var->prop == arg_c) || (multiple_var->prop == exprs) || (multiple_var->prop == cnst)))
							{
								//если количество переменных функции совпадает с числом элементов вектора И они не равны 1 И вектор - является векторным выражением - ошибка.
								if ((temp_pointer->var == multiple_var->var) && (temp_pointer->var != 1) && ((multiple_var->prop == exprs) || (multiple_var->prop == cnst)))
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
									return NULL;
								}
								temp_pointer = new math_obj(low_pointer);
								temp_pointer->point_right = multiple_var;
								temp_pointer->prop = arg_c; //указываем что аргументы - константы
								if ((temp_pointer->var == 1) && (multiple_var->var != 1))
								{
									buf = multiple_var->var;
									if (type != vectr)
									{
										low_pointer = new math_obj(this);
										multiple_var = new math_obj((int)buf, low_pointer);
										copy(multiple_var);
										delete multiple_var;
										link_var_list_to_funct();
									}
									else
									{
										if (var != buf)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
											return NULL;
										}
									}
									low_pointer = new math_obj((int)buf, temp_pointer);
									temp_pointer = low_pointer;
								}
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
							else if ((type == funct) || ((type == vectr) && (prop == funct)))
							{

								//копия найденной функции
								temp_pointer = new math_obj(low_pointer);
								//если получены какие-либо аргументы
								if (multiple_var != NULL)
								{
									//если количество переменных функции совпадает с числом элементов вектора И они не равны 1 И вектор - является вектор-функцией - ошибка.
									if ((temp_pointer->var == multiple_var->var) && (temp_pointer->var != 1) && (multiple_var->prop == funct))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									temp_pointer->point_right = multiple_var;
									temp_pointer->prop == arg_v;
									buf = multiple_var->var;
									if ((get_pc()->point_left != NULL) && (var_list_compare(temp_pointer) != 0))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									if ((temp_pointer->var == 1) && (buf != 1))
									{
										if (type != vectr)
										{
											low_pointer = new math_obj(this);
											multiple_var = new math_obj((int)buf, low_pointer);
											copy(multiple_var);
											delete multiple_var;
											link_var_list_to_funct();
										}
										else
										{
											if (var != buf)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
												return NULL;
											}
										}
										low_pointer = new math_obj((size_t)buf, temp_pointer);
										temp_pointer = low_pointer;
									}
								}
								//для функции не получено никаких аргументов
								else
								{
									//если попалась фундаментальная мат функция (синус, корень и т.п.) - всегда требуются аргументы
									if (temp_pointer->prop == fundm)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::FUNDAMENTAL_FUNCT);
										return NULL;
									}
									if ((get_pc()->point_left != NULL) && (var_list_compare(temp_pointer) != 0))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
								}
								low_pointer = new math_obj((size_t)0);
								low_pointer->point_left = get_pc();
								low_pointer->prop = servc;
								low_pointer = temp_pointer->create_var_list(low_pointer);
								delete low_pointer;
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
							else if ((type == exprs) || ((type == vectr) && (prop == exprs)))
							{
								//копия найденной функции
								temp_pointer = new math_obj(low_pointer);
								//если получены какие-либо аргументы
								if (multiple_var != NULL)
								{
									//если количество переменных функции совпадает с числом элементов вектора И они не равны 1 И вектор - является вектор-функцией - ошибка.
									if ((temp_pointer->var == multiple_var->var) && (temp_pointer->var != 1) && (multiple_var->prop == funct))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									temp_pointer->point_right = multiple_var;
									temp_pointer->prop == arg_v;
									buf = multiple_var->var;
									if ((get_pc()->point_left != NULL) && (var_list_compare(temp_pointer) != 0))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									if ((temp_pointer->var == 1) && (buf != 1))
									{
										if (type != vectr)
										{
											low_pointer = new math_obj(this);
											multiple_var = new math_obj((int)buf, low_pointer);
											copy(multiple_var);
											delete multiple_var;
											link_var_list_to_funct();
										}
										else
										{
											if (var != buf)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
												return NULL;
											}
										}
										low_pointer = new math_obj((int)buf, temp_pointer);
										temp_pointer = low_pointer;
									}
								}
								//для функции не получено никаких аргументов
								else
								{
									//если попалась фундаментальная мат функция (синус, корень и т.п.) - всегда требуются аргументы
									if (temp_pointer->prop == fundm)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::FUNDAMENTAL_FUNCT);
										return NULL;
									}
									if ((get_pc()->point_left != NULL) && (var_list_compare(temp_pointer) != 0))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
								}

								low_pointer = temp_pointer->create_var_list(NULL);
								convert_totaly(L"", funct, undef, L"", low_pointer->var, get_pl(), NULL, low_pointer->point_left);
								delete low_pointer;
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
						}
						else if (low_pointer->type == vectr)
						{
							//попробовать объединить с предыдущими случаями
						}
					}
				}
			}
			//верни это!
			return this;
		}

		//Метод замыкает список переменных в кольцо.
		void math_obj::close_list()
		{
			if ((get_pc() != NULL) && (get_type() == funct))
			{
				get_pc()->point_left = vector_back_processing(get_pc());
				get_pc()->point_left->point_right = get_pc();
			}
		}


		wchar_t* math_obj::brackets_close(wchar_t* brackets_open, wchar_t*end)
		{
			wchar_t* temp = brackets_open;
			int count = 1;
			count = 1;
			while (count != 0)
			{
				temp++;
				//если попали в конец строчки не найдя закрывающих скобок
				if (temp == end + 1)
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
			return temp;
		}


		math_obj *math_obj::operations(math_obj *high, math_obj *low, wstring op_type)
		{
			if ((high == NULL) && (low == NULL))
			{
				if (op_type == minus)
				{
					point_left = new math_obj(op_type, 0, new math_obj(L"", numbr, real, 0), NULL, this);
					high = point_left;
				}
				else
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
					return NULL;
				}
			}
			//если это первая операция в выражении
			else if (high == low)
			{
				//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
				point_left = new math_obj(op_type, 0, low, NULL, this);
				//левый рукав текущей вычисляемой константы указывает на созданную операцию
				high = point_left;
			}
			//если была какая-либо операция до этого
			else
			{
				//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
				if (high->get_priority() <= priority(&op_type))
				{
					//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
					high->point_right = new math_obj(op_type, 0, low, NULL, high);
					high = high->point_right;                 //верхний указатель -> на созданную операцию
				}
				//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
				else
				{
					if (point_left->get_priority() >= priority(&op_type))
					{
						//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
						high = new math_obj(op_type, 0, point_left, NULL, this);
						//воротник предыдущей легчайшей операции -> на новую операцию
						point_left->point_collar = high;
						//указываем левым рукавом константы на созданную операцию
						point_left = high;
					}
					else
					{
						high = high->prioritize(priority(&op_type));
						high->point_right->point_collar = new math_obj(op_type, 0, high->point_right, NULL, high);
						high->point_right = high->point_right->point_collar;
						high = high->point_right;
					}

				}
			}
			return high;
		}

		/*Метод возвращает приоритет операции, при условии, что this операция.
		1 - сложение/вычитание
		2 - умножение
		3 - деление
		4 - возведение в степень
		0 - не операция*/
		int math_obj::get_priority()
		{
			return priority(&type);
		}
		/*PRIVATE.Метод возвращает приоритет операции, по указателю на строку с названием типа.
		1 - сложение/вычитание
		2 - умножение
		3 - деление
		4 - возведение в степень
		0 - не операция*/
		int math_obj::priority(wstring *op_type)
		{
			if ((*op_type == addit) || (*op_type == minus))
				return 1;
			else if (*op_type == mltpl)
				return 2;
			else if (*op_type == divis)
				return 3;
			else if (*op_type == power)
				return 4;
			else
				return 0;
		}

		/*Метод вызывает рекурсивную функцию проверки приоритета операций в текущем дереве операций.
		Возвращает указатель на элемент дерева (операцию), имеющий приоритет меньший или равный в сравнении с текущим элементом. */
		math_obj* math_obj::prioritize(int current_priority)
		{
			return prioritize_processing(point_collar, &current_priority);
		}
		/*PRIVATE. Рекурсия для prioritize*/
		math_obj* math_obj::prioritize_processing(math_obj *pc, int *current_priority)
		{
			//если приоритет проверяемой операции !БОЛЬШЕ! текущей операции
			if (pc->get_priority() > *current_priority)
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

		/*Метод собирает список переменных данной функции по спискам переменных функций, участвующих в её записи*/
		math_obj* math_obj::create_var_list(math_obj *var_list)
		{
			math_obj *iter = NULL;
			math_obj *place = NULL;
			math_obj *buf = NULL;
			if (var_list == NULL)
			{
				var_list = new math_obj((size_t)0);
				var_list->prop = servc;
			}
			if (type == funct)
			{
				if (prop == arg_v)
				{
					for (int count = 0; count < (int)var; count++)
					{
						iter = point_right->vector_at(count);
						if (iter->type == varbl)
						{
							place = find_by_name_processing(var_list->point_left, iter);
							if (place == NULL)
							{
								place = new math_obj(iter);
								iter->point_left = place;
								var_list->vector_push_back(place);
							}
							else
							{
								iter->point_left = place;
							}
						}
						else if (iter->type == funct)
						{
							if (iter->prop == arg_v)
							{
								var_list = iter->create_var_list(var_list);
							}
							else if ((iter->prop == undef) || (iter->prop == defnd))
							{
								place = new math_obj((size_t)0);
								var_list_copy_to_vector_processing(iter->point_collar, place);
								iter->prop = arg_v;
								place->prop = only_arg_v;
								iter->point_right = place;
								var_list_copy_to_vector_processing(place->point_left, var_list);
							}
						}
					}
				}
				else if ((prop == undef) || (prop == defnd))
				{
					place = new math_obj((size_t)0);
					var_list_copy_to_vector_processing(point_collar, place);
					prop = arg_v;
					place->prop = only_arg_v;
					point_right = place;
					var_list_copy_to_vector_processing(point_left, var_list);
				}
			}
			else if (type == vectr)
			{
				for (int count = 0; count < (int)var; count++)
				{
					iter = vector_at(count);
					if (iter->type == varbl)
					{
						place = var_list->find_by_name(iter);
						if (place == NULL)
						{
							place = new math_obj(iter);
							vector_assing_at(new math_obj(L" ", varbl, servc, L"", -1, place, NULL, NULL), count);
							var_list->vector_push_back(place);
						}
						else
						{
							vector_assing_at(new math_obj(L" ", varbl, servc, L"", -1, place, NULL, NULL), count);
						}
					}
					else if (iter->type == funct)
					{
						if (buf == iter)
							break;
						if (buf == NULL)
							buf = iter;
						if (iter->prop == arg_v)
						{
							var_list = iter->create_var_list(var_list);
						}
						else if ((iter->prop == undef) || (iter->prop == defnd))
						{
							place = new math_obj((size_t)0);
							iter->var_list_copy_to_vector(place);
							iter->prop = arg_v;
							place->prop = arg_v;
							iter->point_right = place;
							place->var_list_copy_to_vector(var_list);
						}
					}
				}
			}
			return var_list;
		}

		//Метод вызывает рекурсивную функцию установки указателей point_collar для НЕЗАМКНУТОГО списка переменных на текущий экземпляр класса
		void math_obj::link_var_list_to_funct()
		{
			if ((type == vectr) && (var != 0) && (prop == funct) && (point_left->point_left->point_collar != NULL) && (point_left->point_left->type == funct))
			{
				var_list_collar(point_left->point_left->point_collar, point_left->point_left);
			}
			//текущий элемет - что угодно другое
			else
			{
				if ((point_collar != NULL) && (type == funct))
					var_list_collar(point_collar, this);
			}
		}

		/*PRIVATE. Установка указателей point_collar для НЕЗАМКНУТОГО списка переменных на функцию*/
		void math_obj::var_list_collar(math_obj* pointer, math_obj*original)
		{
			pointer->point_collar = original;
			if (pointer->point_right != NULL)
			{
				var_list_collar(pointer->point_right, original);
			}
		}

		/*Метод сравнивает список аргументов (хотя бы один из которых переменная) обхекта pointer (функции, вектора или матрицы) со списком переменных данной функции.
		Метод возвращает:
		0 - список переменных pointer полностью входит в список переменных данной функции.
		любое другое положительное целое - число различий списков функций.
		-1 - ошибка - неверный тип оьъекта
		*/
		int math_obj::var_list_compare(math_obj * pointer)
		{
			if ((type == vectr) && (var != 0) && (prop == funct))
				return point_left->point_left->var_list_compare_processing(pointer);
			else if (type == funct)
			{
				return var_list_compare_processing(pointer);
			}
			else
				return -1;
		}

		/*Метод сравнивает список аргументов (хотя бы один из которых переменная) функции pointer со списком переменных данной функции.
		Метод возвращает:
		0 - список переменных pointer полностью входит в список переменных данной функции.
		любое другое положительное целое - число различий списков функций.
		*/
		int math_obj::var_list_compare_processing(math_obj * pointer)
		{
			int count_var = 0;
			int temp = 0;
			math_obj * place;
			math_obj *buf = NULL;
			//рассматриваются функции с аргументами (определённые и нет)
			if (pointer->get_prop() == arg_v)
			{
				for (int count = 0; count < pointer->get_var(); count++)
				{
					place = pointer->get_pr()->vector_at(count);
					//если среди аргументов попалась переменная
					if (place->type == varbl)
					{
						//если эта переменная не в списке данной функции
						if (find_by_name_processing(get_pc(), place) == NULL)
							count_var++;
					}
					//если функция
					else if (place->type == funct)
					{
						if (buf == place)
							break;
						if (buf == NULL)
							buf = place;
						count_var += var_list_compare_processing(place);
					}
					else
					{
						//если конст или выражение - ничего не делать
					}
				}
			}
			//рассматриваются функции без аргументов
			else
			{
				math_obj * iter = pointer->point_collar;
				do
				{
					if (find_by_name_processing(point_collar, iter) == NULL)
						count_var++;
					iter = iter->point_right;
				} while ((iter != NULL) && (iter->var != 0));
			}
			return count_var;
		}

		/*Метод возвращаяет указатель на элемент списка переменных с данным именем*/
		math_obj* math_obj::find_by_name(math_obj*pointer)
		{
			if (type == vectr)
			{
				if (prop == funct)
				{
					return find_by_name_processing(point_left->point_left->point_collar, pointer);
				}
				else
					return find_by_name_processing(point_left, pointer);
			}
			else if (type == funct)
				return find_by_name_processing(point_collar, pointer);
			else
				return NULL;
		}
		/*PRIVATE. Рекурсия для find_by_name*/
		math_obj* math_obj::find_by_name_processing(math_obj*pointer, math_obj *original)
		{
			if (pointer == NULL)
				return NULL;
			if (original->name.compare(pointer->name) == 0)
				return pointer;
			else if ((pointer->point_right != NULL) && ((pointer->point_right->var != 0) || (pointer->point_right->type != varbl)))
				return find_by_name_processing(pointer->point_right, original);
			else
				return NULL;
		}


		/*Метод добавляет переменную в конец списка переменных данной функции.
		В том числе и для векторных/матричных.
		-1 в случае ошибки.*/
		int math_obj::var_list_push_back(math_obj*pointer)
		{
			math_obj * back;

			if ((type == vectr) && (var != 0) && (prop == funct) && (point_left->point_left->point_collar != NULL))
			{
				back = vector_back_processing(point_left->point_left->point_collar);
			}
			else if ((type == funct) && (point_collar != NULL))
			{
				back = vector_back_processing(point_collar);
			}
			else
				return -1;
			if (back->point_right != NULL)
				return -1;
			back->point_right = pointer;
			pointer->var = get_var();
			pointer->point_collar = this;
			pointer->point_left = NULL;
			pointer->point_right = NULL;
			assing_var(var + 1);
			return 0;
		}


		/*Метод копирует список переменных в вектор. Если this так же вектор, то копируется его содержимое в другой вектор без повторов.
		НЕ ИСПОЛЬЗОВАТЬ ДЛЯ КОПИРОВАНИЯ ВЕКТОРОВ (хотя если вектор не servс прямой ошибки не будет:-).
		-1 при ошибке*/
		int math_obj::var_list_copy_to_vector(math_obj*vector)
		{
			if (type == vectr)
				return var_list_copy_to_vector_processing(point_left, vector);
			else if (type == funct)
				return var_list_copy_to_vector_processing(point_collar, vector);
			else
				return -1;
		}
		/*PRIVATE. Метод рекурсивно копирует список переменных в вектор.
		-1 при ошибке*/
		int math_obj::var_list_copy_to_vector_processing(math_obj* pointer, math_obj* vect)
		{
			if (vect->prop == servc)
			{
				math_obj* temp = find_by_name_processing(vect->point_left, pointer);
				if (temp == NULL)
				{
					vect->vector_push_back(new math_obj(pointer));
					pointer->point_left = vect->vector_back();
				}
				else
				{
					pointer->point_left = temp;
				}
			}
			else
				vect->vector_push_back(new math_obj(pointer));

			if ((pointer->point_right != NULL) && ((pointer->point_right->var != 0) || (pointer->point_right->type != varbl)))
			{
				return var_list_copy_to_vector_processing(pointer->point_right, vect);
			}
			else
				return 0;
		}















		//Конструктор матрицы НИЧЕГО НЕ ДЕЛАЕТ!
		/*math_obj::math_obj(int size_n, int size_m)
		{

		}*/

		//КОНСТРУКТОРЫ, МЕТОДЫ ПРЕОБРАЗОВАНИЯ И ОБРАЩЕНИЯ К ПОЛЯМ ОТТДЕЛЬНЫХ ЭКЗЕМПЛЯРОВ КЛАССА

		//Нулевой конструктор
		math_obj::math_obj()
		{
			name = L"";
			type = L"";
			prop = L"";
			actn = L"";
			var = 0;
			point_left = NULL;
			point_right = NULL;
			point_collar = NULL;
		}
		//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double) 
		math_obj::math_obj(wstring _name, double _num)
		{
			name = _name;
			type = L"";
			prop = L"";
			actn = L"";
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = NULL;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double) 
		math_obj::math_obj(wstring _name, wstring _type, wstring _prop, double _num)
		{
			name = _name;
			type = _type;
			prop = _prop;
			actn = L"";
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = NULL;
		}
		//Конструктор записывает (ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _type, wstring _prop, double _num, math_obj *_pc)
		{
			name = L"";
			type = _type;
			prop = _prop;
			actn = L"";
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = _pc;
		}
		//Конструктор записывает (ТИП объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _type, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
			name = L"";
			type = _type;
			prop = L"";
			actn = L"";
			var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = _pc;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
			name = _name;
			type = _type;
			prop = _prop;
			actn = _actn;
			var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = _pc;			
		}
		//Конструктор копирования
		math_obj::math_obj(math_obj* var1)
		{
			name = var1->name;
			type = var1->type;
			prop = var1->prop;
			actn = var1->actn;
			var = var1->var;
			point_left = var1->point_left;
			point_right = var1->point_right;
			point_collar = var1->point_collar;
		}
		//Деструктор
		math_obj::~math_obj()
		{
			//разобрать возможность использования tree_dust здесь
		}
		//Метод копирования.
		void math_obj::copy(math_obj* ref) {
			name = ref->name;
			type = ref->type;
			prop = ref->prop;
			actn = ref->actn;
			var = ref->var;
			point_left = ref->point_left;
			point_right = ref->point_right;
			point_collar = ref->point_collar;
		}
		//Метод get. ТЕКУЩИЙ ЭЛЕМЕНТ
		math_obj* math_obj::get_this()
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				return point_left->point_left;
			}
			//текущий элемет - что угодно другое
			else
			{
				return this;
			}
		}
		//Метод get. ИМЯ
		wstring math_obj::get_name()
		{			
			return name;
		}
		//Метод assing. ИМЯ
		void math_obj::assing_name(wstring _name)
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				point_left->point_left->name = _name;
				
			}
			name = _name;			
		}
		//Метод get. ТИП
		wstring math_obj::get_type()
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				return point_left->point_left->type;
			}
			//текущий элемет - что угодно другое
			else
			{
				return type;
			}
		}
		//Метод assing. ТИП
		void math_obj::assing_type(wstring _type)
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				point_left->point_left->type = _type;
				prop = _type;
			}
			//текущий элемет - что угодно другое
			else
			{
				type = _type;
			}
		}
		//Метод get. СВОЙСТВО
		wstring math_obj::get_prop()
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				return point_left->point_left->prop;
			}
			//текущий элемет - что угодно другое
			else
			{
				return prop;
			}
		}
		//Метод assing. СВОЙСТВО
		void math_obj::assing_prop(wstring _prop)
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				point_left->point_left->prop = _prop;
			}
			//текущий элемет - что угодно другое
			else
			{
				prop = _prop;
			}
		}
		//Метод get. ДЕЙСТВИЕ
		wstring math_obj::get_actn()
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				return point_left->point_left->actn;
			}
			//текущий элемет - что угодно другое
			else
			{
				return actn;
			}
		}
		//Метод assing. ДЕЙСТВИЕ
		void math_obj::assing_actn(wstring _acnt)
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				point_left->point_left->actn = _acnt;
			}
			//текущий элемет - что угодно другое
			else
			{
				actn = _acnt;
			}
		}
		//Метод get. ЧИСЛО
		double math_obj::get_var()
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				return point_left->point_left->var;
			}
			//текущий элемет - что угодно другое
			else
			{
				return var;
			}
		}
		//Метод assing. ЧИСЛО
		void math_obj::assing_var(double _num)
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				point_left->point_left->var = _num;
			}
			//текущий элемет - что угодно другое
			else
			{
				var = _num;
			}
		}
		//Метод get. УКАЗАТЕЛЬ "левый рукав"
		math_obj* math_obj::get_pl()
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				return point_left->point_left->point_left;
			}
			//текущий элемет - что угодно другое
			else
			{
				return point_left;
			}
		}
		//Метод assing. УКАЗАТЕЛЬ "левый рукав"
		void math_obj::assing_pl(math_obj* pointer)
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				point_left->point_left->point_left = pointer;
			}
			//текущий элемет - что угодно другое
			else
			{
				point_left = pointer;
			}
		}
		//Метод get. УКАЗАТЕЛЬ "правый рукав"
		math_obj* math_obj::get_pr()
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				return point_left->point_left->point_right;
			}
			//текущий элемет - что угодно другое
			else
			{
				return point_right;
			}
		}
		//Метод assing. УКАЗАТЕЛЬ "правый рукав"
		void math_obj::assing_pr(math_obj * pointer)
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				point_left->point_left->point_right = pointer;
			}
			//текущий элемет - что угодно другое
			else
			{
				point_right = pointer;
			}
		}
		//Метод get. УКАЗАТЕЛЬ "воротник"
		math_obj* math_obj::get_pc()
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				return point_left->point_left->point_collar;
			}
			//текущий элемет - что угодно другое
			else
			{
				return point_collar;
			}
		}
		//Метод assing. УКАЗАТЕЛЬ "воротник"
		void math_obj::assing_pc(math_obj * pointer)
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat)))
			{
				point_left->point_left->point_collar = pointer;
			}
			//текущий элемет - что угодно другое
			else
			{
				point_collar = pointer;
			}
		}


		/*Метод преобразует текущий элемент с заданными параметрами (ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник").
		Если текущий элемент - вектор, все элементы которого указывают на один и тот же объект, преобразуется так же объект*/
		void math_obj::convert_to(wstring _type, wstring _prop, double _num, math_obj *_pc)
		{
			//текущий элемент ненулевой вектор И у него есть одно из этих свойств.
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat) || (prop == cnst)))
			{
				point_left->point_left->type = _type;
				point_left->point_left->prop = _prop;
				point_left->point_left->var = _num;
				point_left->point_left->point_collar = _pc;
				prop = _type;
			}
			//текущий элемет - что угодно другое
			else
			{
				type = _type;
				prop = _prop;
				var = _num;
				point_collar = _pc;
			}
		}
		/*Метод ПОЛНОСТЬЮ преобразует this с заданными параметрами (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник").
		Если текущий элемент - вектор, все элементы которого указывают на один и тот же объект, преобразуется так же объект*/
		void math_obj::convert_totaly(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc)
		{
			if ((type == vectr) && (var != 0) && ((prop == funct) || (prop == exprs) || (prop == equat) || (prop == cnst)))
			{
				point_left->point_left->name = _name;
				point_left->point_left->type = _type;
				point_left->point_left->prop = _prop;
				point_left->point_left->actn = _actn;
				point_left->point_left->var = _num;
				point_left->point_left->point_left = _pl;
				point_left->point_left->point_right = _pr;
				point_left->point_left->point_collar = _pc;
				prop = _type;
			}
			else
			{
				name = _name;
				type = _type;
				prop = _prop;
				actn = _actn;
				var = _num;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}
		}












		//ОБЩИЕ МЕТОДЫ ДЛЯ РАБОТЫ С ВЕКТОРАМИ (ОДНОСВЯЗНЫМИ СПИСКАМИ С ДВОЙНЫМ УКАЗАНИЕМ)

		//Конструктор вектора длины size_n
		math_obj::math_obj(size_t size_n)
		{
			name = L"";
			type = vectr;
			prop = L"";
			actn = L"";
			var = size_n;
			point_left = NULL;
			point_right = NULL;
			point_collar = NULL;
			math_obj *temp = vector_create(size_n, NULL, NULL);
		}
		//Конструктор вектора длины size_n по строке символов типа wchar_t, имеющей начало begin и конец end.
		math_obj::math_obj(size_t size_n, wchar_t*begin, wchar_t*end, data_list* data)
		{
			name = L"";
			type = vectr;
			prop = L"";
			actn = L"";
			var = size_n;
			point_left = NULL;
			point_right = NULL;
			point_collar = NULL;
			point_up = data;
			math_obj *temp = vector_create(size_n, begin, end);
		}

		/*PRIVATE. Метод создаёт математический объект - вектор длины size_n по строке символов типа wchar_t, имеющей начало begin и конец end.
		Метод возвращает указатель на созданный экземпляр класса math_obj. Функция вызывает в качестве внутренней функции, а так же рекурсивно, основную функцию заполнения build_tree.
		NULL в случае какой-либо ошибки, см. error.h.*/
		math_obj * math_obj::vector_create(int size_n, wchar_t*begin, wchar_t*end)
		{
			int iter = 0;
			int count_var = 0;
			wstring name;
			wchar_t *temp;
			wchar_t *temp_2;
			math_obj *temp_pointer;
			if ((begin == NULL) || (end == NULL))
			{
				for (int count = 0; count < size_n; count++)
				{
					vector_push_back(new math_obj());
				}
			}
			else
			{
				for (int count = 0; count < size_n; count++)
				{
					temp = wcspbrk(begin + 1, L",)");
					if (*temp == ',')
					{
						temp_2 = wcspbrk(begin + 1, L"(");
						while ((temp_2 != NULL) && (temp_2 < temp))
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
							temp_2 = wcspbrk(temp_2 + 1, L"(");
						}
					}
					name.assign(begin + 1, temp);
					temp_pointer = new math_obj(&name[0], &name[name.length() - 1], NULL, point_up);					
					if (temp_pointer->type == eror)
					{
						delete temp_pointer;
						return NULL;
					}
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
							//general_var_const->push_back(temp_pointer);
						}
						//если в вектор добавляются переменные - они нумеруются по порядку. Это необходимо в случае задания списка переменных, а так же для разрешения возможных неопределённостей при проходе по вектору определённых методов.
						temp_pointer->var = count_var;
					}
					else if (temp_pointer->type == funct)
					{
						if (temp_pointer->actn == write)
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
							return NULL;
						}
						//тут в качестве аргумента получается переменная функция, которая либо  defnd, либо  undef
						if (temp_pointer->prop != arg_c)
						{
							prop = arg_v;
						}
						else //тут вегда подразумевается  defnd функция с конст аргументами										
						{

						}
					}
					vector_push_back(temp_pointer);
					begin = temp;
				}
				if (count_var == size_n)
					prop = only_arg_v;
				else if (prop != arg_v)
				{
					if (count_var == 0)
						prop = arg_c;
					else
						prop = arg_v;
				}
				var = size_n;				
			}
			return this;
		}

		/*Конструктор вектора служебных переменных, указывающих на один и тот же объект obj. Полагается что вектор унаследует имя объекта obj.
		Так же в случае если obj - вектор такой же длины, то служебные переменные будут указывать на элементы первоначального вектора*/
		math_obj::math_obj(size_t size_n, math_obj * obj)
		{
			name = obj->name;
			type = vectr;
			prop = obj->type;
			actn = L"";
			var = size_n;
			point_left = convert_to_vector(size_n, obj);
			point_right = NULL;
			point_collar = NULL;
		}

		/*PRIVATE. Деструктор вектора*/
		int math_obj::vector_destruct(math_obj* pointer)
		{
			int s = 0;
			if (pointer->type == vectr)
			{
				s += vector_destruct(pointer->point_left);
				delete pointer;
			}
			else
			{
				if (pointer->point_right != NULL)
				{
					s += vector_destruct(pointer->point_right);
					if ((pointer->prop == servc) && (pointer->type == varbl))
					{
						s += tree_destruct_processing(pointer->point_left);
						delete pointer;
					}
				}
				else
				{
					s += tree_destruct_processing(pointer);
				}
			}
			return s;
		}
		/*PRIVATE. Метод рекурсивно создаёт связный список указанной длинны size из служебных переменных, указывающих на один и тот же объект.*/
		math_obj* math_obj::convert_to_vector(int size, math_obj*obj)
		{
			math_obj* temp = NULL;
			if (size > 0)
			{
				temp = new math_obj(L"", varbl, servc, L"", -1, obj, NULL, NULL);
				temp->point_right = convert_to_vector(size - 1, obj);
				temp->point_collar = temp->point_right;
			}
			return temp;
		}

		/*Метод вставляет с заменой в вектор элемент на место index, при условии что index существует.
		-1 в случае ошибки*/
		int math_obj::vector_assing_at(math_obj*pointer, int index)
		{
			if (index >= vector_size())
			{
				//ошибка. Такого номера не существует или элемент не вектор. Ничего не делать
				return -1;
			}
			else
			{
				math_obj*place;
				if (index == 0)
				{
					place = point_left;
					if (pointer->type == funct)
					{
						point_left = new math_obj(L"", varbl, servc, L"", -1, pointer, place->point_right, place->point_collar);
					}
					else if ((pointer->type == varbl) || (pointer->type == cnst) || (pointer->type == exprs) || (pointer->type == vectr))
					{
						point_left = pointer;
						point_left->point_right = NULL;
						point_left->point_collar = NULL;
						point_left->point_right = place->point_right;
						point_left->point_collar = place->point_collar;
					}
				}
				else
				{
					math_obj*place_minus_1 = vector_at(index - 1);
					place = place_minus_1->point_right;
					if (pointer->type == funct)
					{
						place_minus_1->point_right = new math_obj(L"", varbl, servc, L"", -1, pointer, place->point_right, place->point_collar);
						place_minus_1->point_collar = place_minus_1->point_right;
					}
					else if ((pointer->type == varbl) || (pointer->type == cnst) || (pointer->type == exprs) || (pointer->type == vectr))
					{
						place_minus_1->point_right = pointer;
						place_minus_1->point_collar = place_minus_1->point_right;
						place_minus_1->point_right->point_right = NULL;
						place_minus_1->point_right->point_collar = NULL;
						place_minus_1->point_right->point_right = place->point_right;
						place_minus_1->point_right->point_collar = place->point_collar;
					}
				}
				if ((place->prop == servc) && (place->type == varbl))
				{
					place->point_left->tree_destruct();
					delete place->point_left;
				}
				delete place;
				return 0;
			}
		}
		/*Метод возвращает указатель на элемент списка с номером index.*/
		math_obj* math_obj::vector_at(int index)
		{
			if ((type != vectr) || (point_left == NULL) || (index >= vector_size()))
			{
				//выход за пределы вектора можно сделать отдельной ошибкой
				return NULL;
			}
			else
			{
				int count=0;
				math_obj*place = vector_at_processing(point_left, &index, &count);
				if ((place->prop == servc) && (place->type == varbl))
				{
					return place->point_left;
				}
				else
				{
					return place;
				}
			}
		}
		/*PRIVATE. Рекурсия для vector_at*/
		math_obj* math_obj::vector_at_processing(math_obj*pointer, int* index, int* count)
		{
			if (*index == *count)
				return pointer;
			else if (pointer->point_right != NULL)
			{
				(*count)++;//увеличение int, а не указателя
				return vector_at_processing(pointer->point_right, index, count);
			}
			else
				//такой случай невозможен, но оставил возврат NULL
				return NULL;
		}
		/*Метод возвращает количество элементов вектора.*/
		int math_obj::vector_size()
		{
			if (type != vectr)
			{
				return -1;
			}
			int size = 0;
			if (point_left != NULL)
			{
				size += vector_size_processing(point_left);
			}
			if ((int)var != size)
				var = size;
			return size;
		}
		/*PRIVATE. Рекурсия для vector_size*/
		int math_obj::vector_size_processing(math_obj*pointer)
		{
			int count = 0;
			if (pointer->point_right == NULL)
			{
				count++;
			}
			else
			{
				count = vector_size_processing(pointer->point_right) + 1;
			}
			return count;
		}
		/*Метод добавляет элемент pointer в конец вектора. Полагается, что pointer - независимая копия какого либо элемента.
		-1 в случае ошибки*/
		int math_obj::vector_push_back(math_obj*pointer)
		{
			//полагаем что pointer - есть копия какого либо элемента
			if (type != vectr)
			{
				//ошибка. Ничего не делать
				return -1;
			}
			else if (point_left == NULL)
			{
				if (pointer->type == funct)
				{
					point_left = new math_obj(L"", varbl, servc, L"", -1, pointer, NULL, NULL);
				}
				else if ((pointer->type == cnst) || (pointer->type == exprs) || (pointer->type == numbr) || (pointer->type == vectr))
				{
					point_left = pointer;
					point_left->point_right = NULL;
					point_left->point_collar = NULL;
				}
				else if (pointer->type == varbl)
				{
					pointer->var = 0;
					point_left = pointer;
					point_left->point_right = NULL;
					point_left->point_collar = NULL;
				}
				var = 1;
			}
			else
			{
				math_obj* place = vector_back_processing(point_left);
				if (pointer->type == funct)
				{
					place->point_right = new math_obj(L"", varbl, servc, L"", -1, pointer, NULL, NULL);
					place->point_collar = place->point_right;
				}
				else if ((pointer->type == cnst) || (pointer->type == exprs) || (pointer->type == numbr) || (pointer->type == vectr))
				{
					place->point_right = pointer;
					place->point_collar = place->point_right;
					place->point_right->point_right = NULL;
					place->point_right->point_collar = NULL;
				}
				else if (pointer->type == varbl)
				{
					place->point_right = pointer;
					place->point_collar = place->point_right;
					pointer->var = var;
					place->point_right->point_right = NULL;
					place->point_right->point_collar = NULL;
				}
				var += 1;
			}
			return 0;
		}
		/*Метод добавляет элемент pointer в начало вектора. Полагается, что pointer - независимая копия какого либо элемента.
		-1 в случае ошибки*/
		int math_obj::vector_push_foward(math_obj*pointer)
		{
			if (type != vectr)
			{
				//ошибка. Ничего не делать 	
				return -1;
			}
			else if (point_left == NULL)
			{
				if (pointer->type == funct)
				{
					point_left = new math_obj(L"", varbl, servc, L"", -1, pointer, NULL, NULL);
				}
				else if ((pointer->type == cnst) || (pointer->type == exprs) || (pointer->type == numbr) || (pointer->type == vectr))
				{
					point_left = pointer;
					point_left->point_right = NULL;
					point_left->point_collar = NULL;
				}
				else if (pointer->type == varbl)
				{
					pointer->var = 0;
					point_left = pointer;
					point_left->point_right = NULL;
					point_left->point_collar = NULL;
				}
				var = 1;
			}
			else
			{
				math_obj* place = point_left;
				if (pointer->type == funct)
				{
					point_left = new math_obj(L"", varbl, servc, L"", -1, pointer, place, place);
				}
				else if ((pointer->type == numbr) || (pointer->type == cnst) || (pointer->type == exprs) || (pointer->type == vectr))
				{
					point_left = pointer;
					point_left->point_right = place;
					point_left->point_collar = place;
				}
				else if (pointer->type == varbl)
				{
					pointer->var = 0;
					point_left = pointer;
					point_left->point_right = place;
					point_left->point_collar = place;
				}
				var += 1;
			}
			return 0;
		}
		/*Метод возвращает указатель на последний элемент вектора*/
		math_obj* math_obj::vector_back()
		{
			if ((point_left == NULL) || (type != vectr))
				return NULL;
			else
				return vector_back_processing(point_left);
		}
		/*PRIVATE. Рекурсия для var_list_back и vector_back*/
		math_obj* math_obj::vector_back_processing(math_obj *pointer)
		{
			if ((pointer->point_right == NULL) || ((pointer->point_right->var == 0) && (pointer->point_right->type == varbl)))
				return pointer;
			else
				return vector_back_processing(pointer->point_right);
		}
		/*Метод преобразует односвязный список в двусвязный.
		Двусвязный список может создаваться только для переменных с именем. Проверять отсутствие других элементов, а так же служебных переменных.*/
		void math_obj::double_lincked_vector()
		{
			if ((point_left != NULL) && (type == vectr))
				reassing_left_pointers(point_left);
		}
		/*PRIVATE. Создание двусвязного списка.*/
		void math_obj::reassing_left_pointers(math_obj* pointer)
		{
			if (pointer->point_right != NULL)
			{
				pointer->point_right->point_left = pointer;
				reassing_left_pointers(pointer->point_right);
			}
		}


		/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и очищающую память.
		Метод должен возвращать число ошибок при использовании delete. Это надо дописать
		*/
		int math_obj::tree_destruct()
		{
			int s = 0;
			if ((type == cnst) || (type == exprs))
			{
				s += tree_destruct_processing(point_left);
				point_left = NULL;
			}
			else if (type == funct)
			{
				if ((prop == undef) || (prop == defnd))
				{
					s += tree_destruct_processing(point_collar);
					point_collar = NULL;
					s += tree_destruct_processing(point_left);
					point_left = NULL;
				}
				else if ((prop == arg_c) || (prop == arg_v))
				{
					s += vector_destruct(point_right->point_collar);
				}
			}
			else if (type == equat)
			{
				//доделать
			}
			else if (type == vectr)
			{
				s += vector_destruct(point_left);
			}
			return s;
		}
		/*PRIVATE. Рекурсия для tree_destruct*/
		int math_obj::tree_destruct_processing(math_obj* pointer)
		{
			if (pointer == NULL) return 0;
			int temp = 0;

			if ((pointer->type == numbr) || (pointer->type == cnst))
			{
				delete pointer;
				pointer = NULL;
				return 0;
			}
			else if ((pointer->type == addit) || (pointer->type == mltpl) || (pointer->type == divis) || (pointer->type == power))
			{
				temp += tree_destruct_processing(pointer->point_left);
				temp += tree_destruct_processing(pointer->point_right);
				delete pointer;
				pointer = NULL;
				return temp;

			}
			else if (pointer->type == exprs)
			{
				temp += tree_destruct_processing(pointer->point_left);
				delete pointer;
				pointer = NULL;
				return temp;
			}
			else if (pointer->type == funct)
			{
				if ((pointer->name == L"minus") || (pointer->name == L"root"))
				{
					temp += vector_destruct(pointer->point_right);
				}
				else
				{
					if (pointer->prop == undef)
					{
						temp += tree_destruct_processing(pointer->point_left);
						temp += tree_destruct_processing(pointer->point_collar);
						delete pointer;
						pointer = NULL;
						return temp;
					}
					else if (pointer->prop == defnd)
					{
						delete pointer;
						pointer = NULL;
						return temp;
					}
					else if ((pointer->prop == arg_c) || (pointer->prop == arg_v))
					{
						temp += vector_destruct(pointer->point_right);
						delete pointer;
						pointer = NULL;
						return temp;
					}
				}
			}
			else if (pointer->type == varbl)
			{
				//удаление связного списка
				if ((pointer->point_right == NULL) || ((pointer->point_right->var == 0) && (pointer->point_right->type == varbl)))
				{
					delete pointer;
					pointer = NULL;
					return temp;
				}
				else
				{
					temp += tree_destruct_processing(pointer->point_right);
					delete pointer;
					pointer = NULL;
					return temp;
				}
			}
			else
				return 0;
		}
			
		/*Метод сортирует незамкнутый список переменных по алфавиту*/
		math_obj* math_obj::sort_list(math_obj * var_list)
		{
			stack <math_obj*> sorting;
			math_obj *  temp;
			while (var_list != NULL)
			{
				sorting.push(var_list);
				sorting.top()->var = 0;
				var_list = var_list->point_right;
				sorting.top()->point_right = NULL;
				if (sorting.size() > 1)
				{
					temp = sorting.top();
					sorting.pop();
					while ((sorting.size() >= 1) && (temp->var == sorting.top()->var))
					{
						temp = merge_lists(temp, sorting.top());
						temp->var += 1;
						sorting.pop();
					}
					sorting.push(temp);
				}
			}
			if (sorting.size() > 1)
			{
				temp = sorting.top();
				sorting.pop();
				while (sorting.size() >= 1)
				{
					temp = merge_lists(temp, sorting.top());
					temp->var += 1;
					sorting.pop();
				}
				temp->var = 0;
				reassing_left_pointers(temp);
				var_list_number(temp);
				return  ;
			}
			else
			{
				return NULL;
			}

		}
		/*PRIVATE. Слияние отсортированных списков*/
		math_obj* math_obj::merge_lists(math_obj* list_a, math_obj* list_b)
		{
			math_obj * merged, *temp;
			if (list_a->name.compare(list_b->name) < 0)
			{
				merged = list_a;
				list_a = list_a->point_right;
			}
			else
			{
				merged = list_b;
				list_b = list_b->point_right;
			}
			temp = merged;
			while ((list_a != NULL) && (list_b != NULL))
			{
				if (list_a->name.compare(list_b->name) < 0)
				{
					temp->point_right = list_a;
					temp = temp->point_right;
					list_a = list_a->point_right;
				}
				else
				{
					temp->point_right = list_b;
					temp = temp->point_right;
					list_b = list_b->point_right;
				}
			}
			if (list_a == NULL)
				temp->point_right = list_b;
			else if (list_b == NULL)
				temp->point_right = list_a;
			return merged;
		}
		/*PRIVATE. Нумерация переменных по порядку.*/
		void  math_obj::var_list_number(math_obj* pointer)
		{
			if (pointer->point_right != NULL)
			{
				pointer->point_right->var = pointer->var + 1;
				reassing_left_pointers(pointer->point_right);
			}
		}
		
		







		//DATA_LIST

		data_list * data_list::back_rec(data_list * pointer)
		{
			if (pointer->right != NULL)
			{
				return back_rec(pointer->right);
			}
			else
				return pointer;
		}

		void data_list::index_plus_one()
		{
			index += 1;
			if (right != NULL)
				right->index_plus_one();
		}

		data_list * data_list::at_rec(int* place, data_list * pointer)
		{
			if (pointer->index == *place)
				return pointer;
			else if (pointer->right != NULL)
				return at_rec(place, pointer->right);
			else
				return nullptr;
		}

		int data_list::size_rec(data_list * pointer, bool* flag)
		{			
			if (*flag) //если 1
			{
				if (pointer->right != NULL)
				{
					return (1 + size_rec(pointer->right, flag));
				}
				else
					return 1;
			}
			else
			{
				if (pointer->left != NULL)
				{
					return (1 + size_rec(pointer->left, flag));
				}
				else
					return 1;
			}			
		}

		data_list::data_list()
		{
			index = 0;
			left = nullptr;
			right = nullptr;
			math = nullptr;
		}

		data_list::data_list(wstring* _in)
		{
			
			in = *_in;
			index = 0;
			left = nullptr;
			right = nullptr;
			math = nullptr;
		}

		data_list::data_list(wstring _in, math_obj * _math)
		{
			in = _in;
			index = -1;
			left = nullptr;
			right = nullptr;
			math = _math;
		}

		int data_list::push_back(data_list * pointer)
		{
			if (index != 0)
			{
				return 0;
			}			
			data_list*place = back();
			place->right = pointer;
			place->right->left = place;
			place->right->index = place->index + 1;
			return 1;		
		}

		int data_list::push_left(math_obj * pointer)
		{
			data_list * zero = begin();
			if (zero == NULL)
				return 0;
			data_list * temp = zero->left;
			zero->left = new data_list(L"",pointer);	
			zero->left->right = zero;
			zero->left->left = temp;
			if (temp!=NULL)
				temp->right = zero->left;
		}

		data_list * data_list::begin()
		{
			if (index == 0)
				return this;
			if ((index > 0) && (left != NULL))
				return left->begin();
			return nullptr;
		}

		int data_list::implace(int place, data_list * pointer)
		{
			data_list* temp = at(place);
			if (temp == NULL)
				return 0;
			data_list*temp_plus_one = temp->right;
			temp->right = pointer;
			temp->right->index = temp->index + 1;
			temp->right->left = temp;
			temp->right->right = temp_plus_one;
			if (temp_plus_one != NULL)
			{
				temp_plus_one->left = pointer;
				temp_plus_one->index_plus_one();
			}
			return 1;
		}

		data_list * data_list::at(int place)
		{
			if (index != 0)
				return nullptr;
			else if ((right != NULL) && (place>0))
				return at_rec(&place, right);
			else if (place == 0)
				return this;
			else
				return NULL;

		}

		data_list * data_list::back()
		{
			if (index != 0)
				return nullptr;
			else if (right != NULL)
			{
				return back_rec(right);
			}
			else
			{
				return this;
			}
		}

		int data_list::compare_in(wstring original)
		{
			return in.compare(original);
		}

		int data_list::compare_out(wstring original)
		{
			return out.compare(original);
		}

		math_obj * data_list::find_math_obj(wstring* name)
		{
			if ((math!=NULL)&&(name->compare(math->name) == 0))
				return math;
			else if (left != NULL)
				return left->find_math_obj(name);
			else
				return nullptr;
		}

		int data_list::run_string()
		{
			//проверка ввода
			if (!Project::IO::VerifyInput(&in[0])) {
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				out = err->GetErrorWStr();
				return 0;
			}
			math = new math_obj(&in[0], &in[in.length() - 1], NULL, this);
			if (math->type == eror)
			{
				delete math;
				//чистить дерево
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				out = err->GetErrorWStr();
				return 0;
			}
			return 1;
		}
		
		int data_list::size_s()
		{
			int out = 0;
			bool flag = true;
			if (index!=0)
				return -1;
			if (right != NULL)
			{
				out += size_rec(right, &flag);
				
			}
			return out;
		}

		int data_list::size_all()
		{
			int out = 0;
			bool flag;
			if (index != 0)
				return -1;
			if (right != NULL)
			{
				flag = true;
				out += size_rec(right, &flag);
			}
			if (left != NULL)
			{
				flag = false;
				out += size_rec(right, &flag);
			}
			return out;
		}

	}
};
