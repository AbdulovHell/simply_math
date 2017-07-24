#include "classes.h"

namespace Project {

	namespace Core {
		using namespace std;
		using namespace Project;
		using namespace Project::IO;
		class data_list;
		class math_obj;

		//MATH_OBJ

		/*Конструктор дерева операций по строке символов типа wchar_t, имеющей начало strPtr и конец ePtr.
		Дополнительный параметр _pc присваивается полю point_collar (для получения дерева операций для функций с заранее обозначенным списком переменных).*/
		math_obj::math_obj(wchar_t* strPtr, wchar_t* endPtr, math_obj* _pc, data_list*data)
		{
			name = L"";
			if (_pc != NULL)
			{
				//если дан список переменных - функция
				type = flags::funct;
				//число переменных
				var = _pc->point_collar->var;
			}
			else
			{
				type = flags::exprs;
				var = 0;
			}
			prop = flags::undef;
			actn = flags::nthng;
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
					prop = flags::error;
				}
				//больше ничего тут делать не надо. Если ошибок не было - конструктор выдаст какой-либо элемент
			}
			//нашли равно
			else
			{
				//смотрим на то что слева от равно. При этом проиходит заполнение текущего элемента в данном конструкторе.
				wstring str_left;
				str_left.assign(strPtr, temp);
				double buf;
				math_obj* left = build_tree(&str_left[0], &str_left[str_left.length() - 1]);
				if (left == NULL)
				{
					prop = flags::error;
				}
				else
				{
					//слева находится функция с параметром "запись" и замкнутым списком переменных (заранее обозначенным). Вектор-функции здесь не рассматриваются (и не должны).
					if ((type == flags::funct) && (actn == flags::write) && (point_collar->point_left != NULL))
					{
						//список переменных будет передан в конструктор элемента для строки справа от равно. 					
						t_p = point_collar;
					}
					//смотрим на то что справа от равно. 
					//если строка закнчивается на равно
					if (temp == endPtr)
					{
						if ((actn == flags::write) || ((type == flags::varbl) && (prop == flags::undef)))
						{
							prop = flags::error;
						}
						/*Запрос на действие:
						- type == funct, prop == undef - запрос на "упрощение" выражения для неявно заданной функции.
						- type == funct, в остальных случаях - расчёт чего-либо или вывод выражения для функции, с упрощением.
						- type == flags::cnst, вывод значения константы.
						- type == exprs, расчёт константного выражения и вывод на экран результата
						- type == varbl, prop==flags::defnd - запрос на решение последнего введённого уравнения с данной переменной. Для неопределённых (новых) переменных - ошибка.
						- type == vectr - рассматривается аналогично для поля prop.*/
						else
						{
							//если справа находится функция с конст. аргументами или именная константа - создаётся выражение с указателем на неё. (чтобы в списке данных наименования не повторялись)
							if (((get_type() == flags::funct) && (get_prop() == flags::arg_c)) || (get_type() == flags::cnst))
							{
								t_p = new math_obj(this);
								convert_totaly(L"", flags::exprs, flags::undef, flags::solve, 0, t_p, NULL, NULL);
								if (t_p->type == flags::vectr)
								{
									buf = t_p->var;
									t_p = new math_obj(this);
									t_p = new math_obj((int)buf, t_p);
									copy(t_p);
									delete t_p;
								}
							}
							actn = flags::solve;
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
						if (right->prop == flags::error)
						{
							prop = flags::error;
							//попытаться сделать деструктор, который будет удалять нужное
							delete right;
						}
						else if ((right->type == flags::funct) && (right->actn == flags::write))
						{
							prop = flags::error;
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
							delete right;
						}
						//если справа от равно находится константное выражение, функция с конст аргументами или вектор имеющий только константные элементы
						//тогда автоматически знаем, что справа НЕ могут быть функции с параметром flags::write
						else if ((right->get_type() == flags::exprs) || (right->get_type() == flags::cnst) || (right->get_type() == flags::numbr) || ((right->get_type() == flags::funct) && (right->get_prop() == flags::arg_c)) || ((right->type == flags::vectr) && (right->prop == flags::arg_c)))
						{
							//если справа от равно находится константное выражение, функция с конст аргументами или вектор имеющий только константные элементы (но не сами константы и конст векторы с именем) или определённая заранее переменная - ошибка
							if ((get_type() == flags::exprs) || (get_type() == flags::numbr) || ((get_type() == flags::funct) && (get_prop() == flags::arg_c)) || ((type == flags::vectr) && (prop == flags::arg_c)))
							{
								prop = flags::error;
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
								delete left;
							}
							//слева от равно заранее определённая переменная. поскольку переменные задаются неявно и используются в функциях, их нельзя переопределять в качестве констант.
							else if ((type == flags::varbl) && (prop == flags::defnd))
							{
								prop = flags::error;
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::USING_VAR_AS_CONST);
								delete left;
							}
							//вектор с параметром cnst и arg_c - это один и тот же объект, однако flags::cnst вектор имеет имя, т.е. его можно как-то переопределить.
							else if (get_type() == flags::cnst)
							{
								if (type == flags::vectr)
								{
									if (unvectorize() == NULL)
									{
										prop = flags::error;
										delete right;
									}
								}
								if (right->type == flags::vectr)
								{
									copy(right);
									delete right;
									prop = flags::cnst;
								}
								else
								{
									point_left = right;
									prop = flags::defnd;
								}
								actn = flags::write;
							}
							//слева от равно - неизвестный ранее набор символов - определение новой константы
							else if ((type == flags::varbl) && (prop == flags::undef))
							{
								if (right->type == flags::vectr)
								{
									copy(right);
									delete right;
									prop = flags::cnst;
								}
								else
								{
									type = flags::cnst;
									point_left = right;
									prop = flags::defnd;
								}
								actn = flags::write;
							}
							else if (get_type() == flags::funct)
							{
								//Уравнение
								//если справа и слева веторные объекты и их размерность не совпадает - ошибка
								if ((type == flags::vectr) && (right->type == flags::vectr) && (var != right->var))
								{
									prop = flags::error;
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
									delete right;
								}
								t_p = new math_obj(this);
								convert_totaly(L"", flags::equat, flags::unslv, flags::write, get_var(), t_p, right, t_p->get_pc());
								//тут немного сложнее. переменные, используемые в уравнении - могут быть записаны среди аргументов функции. Надо отдельно формировать список аргументов.
								//возможно имеет смысл создание уравнения вынести в отдельный метод.
								//TODO: собирать список переменных для данного уравнения
								if (t_p->type == flags::vectr)
								{
									buf = t_p->var;
									t_p = new math_obj(this);
									right = new math_obj((int)buf, t_p);
									copy(right);
									delete right;
								}
								else if (right->type == flags::vectr)
								{
									buf = right->var;
									t_p = new math_obj(this);
									right = new math_obj((int)buf, t_p);
									copy(right);
									delete right;
								}
								actn = flags::write;
							}
						}
						//справа - переменная, как заранее определённая, так и новое буквосочетание
						else if (right->type == flags::varbl)
						{
							if (right->prop == flags::undef)
							{
								right->prop = flags::defnd;
								point_up->push_left(new math_obj(right));
							}
							if (((get_type() == flags::funct) && ((get_prop() == flags::undef) || (get_prop() == flags::arg_v) || (get_prop() == flags::arg_c))) || (get_type() == flags::exprs))
							{
								//Уравнение
								t_p = new math_obj(this);
								convert_totaly(L"", flags::equat, flags::unslv, flags::write, get_var(), t_p, right, t_p->get_pc());
								//TODO: собирать список переменных для данного уравнения
								if (t_p->type == flags::vectr)
								{
									buf = t_p->var;
									t_p = new math_obj(this);
									right = new math_obj((int)buf, t_p);
									copy(right);
									delete right;
								}
							}
							else if (get_type() == flags::cnst)
							{
								if (type == flags::vectr)
								{
									if (unvectorize() == NULL)
									{
										prop = flags::error;
										delete right;
									}
								}
								//определение простейшей функции
								convert_totaly(name, flags::funct, flags::defnd, flags::write, 1, right, NULL, right);
								link_var_list_to_funct();
								close_list();
							}
							else if (type == flags::varbl)
							{
								//если справа и слева стоит одно и тоже буквосочетание
								if (name.compare(right->name) == 0)
								{
									prop = flags::error;
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::USING_VAR_AS_FUNC);
									delete right;
								}
								else
								{
									if (prop == flags::undef)
									{
										prop = flags::defnd;
										point_up->push_left(new math_obj(this));
									}
									//определение простейшей функции
									convert_totaly(name, flags::funct, flags::defnd, flags::write, 1, right, NULL, right);
									link_var_list_to_funct();
									close_list();
								}
							}
							else if ((get_type() == flags::funct) && (get_prop() == flags::defnd))
							{
								if (type == flags::vectr)
								{
									if (unvectorize() == NULL)
									{
										prop = flags::error;
										delete right;
									}
								}
								convert_totaly(name, flags::funct, flags::defnd, flags::write, 1, right, NULL, right);
								link_var_list_to_funct();
								close_list();
							}
							actn = flags::write;
						}
						//справа функция. Случай с фуннкциями, имеющими конст. аргуметнты уже рассмотрен
						else if (right->get_type() == flags::funct) //flags::defnd, flags::undef, flags::arg_v
						{
							if ((type == flags::funct) && (actn == flags::write))
							{
								if (right->type == flags::vectr)
								{
									right->assing_name(name);
									right->assing_prop(flags::defnd);
									copy(right);
									actn = flags::write;
								}
								else
								{
									prop = flags::defnd;
									point_left = right->point_left;
								}
								delete right; //тут чистить дерево операций не надо!								
							}

							else if ((get_type() == flags::funct) && ((get_prop() == flags::arg_c) || (get_prop() == flags::undef) || ((get_prop() == flags::arg_v) && (get_pr()->prop == flags::arg_v))) || (get_type() == flags::exprs))
							{
								//Уравнение
								//если справа и слева веторные объекты и их размерность не совпадает - ошибка
								if ((type == flags::vectr) && (right->type == flags::vectr) && (var != right->var))
								{
									prop = flags::error;
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
									delete right;
								}
								t_p = new math_obj(this);
								convert_totaly(L"", flags::equat, flags::unslv, flags::write, var, t_p, right, t_p->get_pc());
								//TODO: собирать список переменных для данного уравнения
								if ((right->type == flags::vectr) && (type != flags::vectr))
								{
									buf = right->var;
									t_p = new math_obj(this);
									right = new math_obj((int)buf, t_p);
									copy(right);
									delete right;
								}
								actn = flags::write;
							}
							//слева определённая заранее функция - переопределяем заново
							else if ((get_type() == flags::funct) && (get_prop() == flags::defnd))
							{
								if (type == flags::vectr)
								{
									if (unvectorize() == NULL)
									{
										prop = flags::error;
										delete right;
									}
								}
								if (right->get_prop() == flags::undef)
								{
									if (right->get_var() > 1)
										t_p = sort_list(right->get_pc());//отсортированный по алфавиту и пронумерованный двусвязный список, НЕ ВЕКТОР
									else
										t_p = right->get_pc(); //незачем вызывать сортировку для одного элемента
									if (right->type == flags::vectr)
									{
										right->assing_name(name);
										right->assing_prop(flags::defnd);
										copy(right);
										assing_pc(t_p);
									}
									else
									{
										convert_totaly(name, flags::funct, flags::defnd, flags::write, right->var, right->point_left, right->point_right, t_p);
									}
									delete right; //тут чистить дерево операций не надо!
								}
								else //flags::defnd, flags::arg_v
								{
									left = right->create_var_list(NULL);//список переменных функции, ВЕКТОР
									if (left->var > 1)
										t_p = sort_list(left->point_left);//отсортированный по алфавиту и пронумерованный двусвязный список, НЕ ВЕКТОР
									else
										t_p = left->point_left;
									convert_totaly(name, flags::funct, flags::defnd, flags::write, left->var, right, NULL, t_p);
									if (right->type == flags::vectr)
									{
										buf = right->var;
										t_p = new math_obj(this);
										right = new math_obj((int)buf, t_p);
										copy(right);
										delete right;
									}
									delete left;
								}
								//тут всегда будет необходимо устанавливать указатели переменных на созданную функцию.
								link_var_list_to_funct();
								close_list();
								actn = flags::write;
							}
							//слева запись н-р f(x,y), но f была ранее определена
							else if ((get_type() == flags::funct) && ((get_prop() == flags::arg_v) && (get_pr()->prop == flags::only_arg_v)))
							{
								//важен только случай, при котором все переменные участвующие в выражении справа, появляются среди аргументов функции слева.
								//фальшивая функция
								t_p = new math_obj(L"", flags::funct, flags::undef, flags::nthng, get_pr()->var, NULL, NULL, get_pr()->point_left);
								if (t_p->var_list_compare(right) == 0)
								{
									delete t_p;
									if (type == flags::vectr)
									{
										if (unvectorize() == NULL)
										{
											prop = flags::error;
											delete right;
										}
									}
									t_p = point_right;
									t_p->prop = flags::servc;
									left = right->create_var_list(t_p);
									if (left->var > 1)
										t_p = sort_list(left->point_left);//отсортированный по алфавиту и пронумерованный двусвязный список, НЕ ВЕКТОР
									else
										t_p = left->point_left;
									convert_totaly(name, flags::funct, flags::defnd, flags::write, left->var, right, NULL, t_p);
									if (right->type == flags::vectr)
									{
										buf = right->var;
										t_p = new math_obj(this);
										right = new math_obj((int)buf, t_p);
										copy(right);
										delete right;
									}
									delete left;
									//тут всегда будет необходимо устанавливать указатели переменных на созданную функцию.
									link_var_list_to_funct();
									close_list();
								}
								//если справа нашлась переменная не входящая в список аргументов левой функции - уравнение
								else
								{
									delete t_p;
									//Уравнение
									//если справа и слева веторные объекты и их размерность не совпадает - ошибка
									if ((type == flags::vectr) && (right->type == flags::vectr) && (var != right->var))
									{
										prop = flags::error;
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
										delete right;
									}
									t_p = new math_obj(this);
									convert_totaly(L"", flags::equat, flags::unslv, flags::write, var, t_p, right, t_p->get_pc());
									//TODO: собирать список переменных для данного уравнения
									if ((right->type == flags::vectr) && (type != flags::vectr))
									{
										buf = right->var;
										t_p = new math_obj(this);
										right = new math_obj((int)buf, t_p);
										copy(right);
										delete right;
									}
								}
								actn = flags::write;
							}
							//слева именная константа (вектор) или новое буквосочетание
							else if ((get_type() == flags::cnst) || ((type == flags::varbl) && (prop == flags::undef)))
							{
								if (type == flags::vectr)
								{
									if (unvectorize() == NULL)
									{
										prop = flags::error;
										delete right;
									}
								}
								if (right->get_prop() == flags::undef)
								{
									if (right->get_var() > 1)
										t_p = sort_list(right->get_pc());//отсортированный по алфавиту и пронумерованный двусвязный список, НЕ ВЕКТОР
									else
										t_p = right->get_pc(); //незачем вызывать сортировку для одного элемента
									if (right->type == flags::vectr)
									{
										right->assing_name(name);
										right->assing_prop(flags::defnd);
										copy(right);
										assing_pc(t_p);
									}
									else
									{
										convert_totaly(name, flags::funct, flags::defnd, flags::write, right->var, right->point_left, right->point_right, t_p);
									}
									delete right; //тут чистить дерево операций не надо!
								}
								else //flags::defnd, flags::arg_v
								{
									left = right->create_var_list(NULL);//список переменных функции, ВЕКТОР
									if (left->var > 1)
										t_p = sort_list(left->point_left);//отсортированный по алфавиту и пронумерованный двусвязный список, НЕ ВЕКТОР
									else
										t_p = left->point_left;
									convert_totaly(name, flags::funct, flags::defnd, flags::write, left->var, right, NULL, t_p);
									if (right->type == flags::vectr)
									{
										buf = right->var;
										t_p = new math_obj(this);
										right = new math_obj((int)buf, t_p);
										copy(right);
										delete right;
									}
									delete left;
								}
								//тут всегда будет необходимо устанавливать указатели переменных на созданную функцию.
								link_var_list_to_funct();

								t_p = new math_obj((size_t)0);
								t_p->var = var;
								t_p->point_left = get_pc();
								t_p->prop = flags::only_arg_v;
								wstring vars = t_p->vector_to_string();
								delete t_p;

								close_list();

								size_t place = point_up->in.find_first_of(L"=");
								point_up->in.insert(place, vars);

								actn = flags::write;
							}
							//слева уже используемая ранее переменная
							else if ((type == flags::varbl) && (prop == flags::defnd))
							{
								//если в выражении справа эта переменная есть
								if (right->find_by_name(this) != NULL)
								{
									//Уравнение
									t_p = new math_obj(this);
									convert_totaly(L"", flags::equat, flags::unslv, flags::write, var, t_p, right, right->get_pc());
									if (right->type == flags::vectr)
									{
										buf = t_p->var;
										t_p = new math_obj(this);
										right = new math_obj((int)buf, t_p);
										copy(right);
										delete right;
									}
								}
								//если в выражении справа этой переменной нет - переопределение переменной как функции
								else
								{
									if (right->get_prop() == flags::undef)
									{
										if (right->get_var() > 1)
											t_p = sort_list(right->get_pc());//отсортированный по алфавиту и пронумерованный двусвязный список, НЕ ВЕКТОР
										else
											t_p = right->get_pc(); //незачем вызывать сортировку для одного элемента
										if (right->type == flags::vectr)
										{
											right->assing_name(name);
											right->assing_prop(flags::defnd);
											copy(right);
											assing_pc(t_p);
											actn = flags::write;
										}
										else
										{
											convert_totaly(name, flags::funct, flags::defnd, flags::write, right->var, right->point_left, right->point_right, t_p);
										}
										delete right; //тут чистить дерево операций не надо!
									}
									else //flags::defnd, flags::arg_v
									{
										left = right->create_var_list(NULL);//список переменных функции, ВЕКТОР
										if (left->var > 1)
											t_p = sort_list(left->point_left);//отсортированный по алфавиту и пронумерованный двусвязный список, НЕ ВЕКТОР
										else
											t_p = left->point_left;
										convert_totaly(name, flags::funct, flags::defnd, flags::write, left->var, right, NULL, t_p);
										if (right->type == flags::vectr)
										{
											buf = right->var;
											t_p = new math_obj(this);
											right = new math_obj((int)buf, t_p);
											copy(right);
											delete right;
										}
										delete left;
									}
									//тут всегда будет необходимо устанавливать указатели переменных на созданную функцию.
									link_var_list_to_funct();
									t_p = new math_obj((size_t)0);
									t_p->var = var;
									t_p->point_left = get_pc();
									t_p->prop = flags::only_arg_v;
									wstring vars = t_p->vector_to_string();
									delete t_p;

									close_list();
									size_t place = point_up->in.find_first_of(L"=");
									point_up->in.insert(place, vars);

								}
								actn = flags::write;
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
		math_obj* math_obj::build_tree(wchar_t* strPtr, wchar_t*endPtr)
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
			//math_obj* current_element = this;
			while (s_iter <= endPtr)
			{
				if ((*s_iter == '1') || (*s_iter == '2') || (*s_iter == '3') || (*s_iter == '4') || (*s_iter == '5') || (*s_iter == '6') || (*s_iter == '7') || (*s_iter == '8') || (*s_iter == '9') || (*s_iter == '0') || (*s_iter == '.'))
				{
					// если данное число первое в записи выражения
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> константу
						point_left = new math_obj(flags::numbr, flags::real, wstr_to_double(s_iter, &s_iter), NULL);
						//оба указателя -> на число, тебуется для проверки условия при записи операции
						low_pointer = point_left;
						high_pointer = low_pointer;
					}
					// если данное число не первое в записи (была какая-либо операция)
					else
					{
						//создание элемента класса и запись числа, воротник -> пред операцию 
						high_pointer->point_right = new math_obj(flags::numbr, flags::real, wstr_to_double(s_iter, &s_iter), NULL);
						low_pointer = high_pointer->point_right;
					}
				}
				else if (*s_iter == '+')
				{
					high_pointer = operations(high_pointer, low_pointer, flags::addit);
					//low_pointer = high_pointer->point_left;
					s_iter++;
					temp = wcspbrk(s_iter, L")+-*^/=,");
					if (s_iter == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*s_iter == '*')
				{
					high_pointer = operations(high_pointer, low_pointer, flags::mltpl);
					//low_pointer = high_pointer->point_left;
					s_iter++;
					temp = wcspbrk(s_iter, L")+-*^/=,");
					if (s_iter == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*s_iter == '/')
				{
					high_pointer = operations(high_pointer, low_pointer, flags::divis);
					//low_pointer = high_pointer->point_left;
					s_iter++;
					temp = wcspbrk(s_iter, L")+-*^/=,");
					if (s_iter == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*s_iter == '^')
				{
					high_pointer = operations(high_pointer, low_pointer, flags::power);
					//low_pointer = high_pointer->point_left;
					s_iter++;
					temp = wcspbrk(s_iter, L")+-*^/=,");
					if (s_iter == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*s_iter == '-')
				{
					//минус - просто отдельная операция. если минус оказался вначале строчки - предполагается вычитание из нуля
					high_pointer = operations(high_pointer, low_pointer, flags::minus);
					if (low_pointer == NULL)
						low_pointer = high_pointer->point_left;
					s_iter++;
					temp = wcspbrk(s_iter, L")+-*^/=,");
					if (s_iter == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*s_iter == '(')
				{
					temp = s_iter;
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
					if (s_iter + 1 == temp)
					{
						//пустая строка в скобках. Считаестя нулём. 5*() = 0
						//скобка открывается в начале строки
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							//и закрывается в конце
							if ((temp + 1) == NULL)
							{
								//наверх выдаётся ноль
								convert_to(flags::numbr, flags::real, 0, NULL);
							}
							//и закрывается раньше чем конец строки - есть операции
							else
							{
								//ноль добавляется в выражение
								this->point_left = new math_obj(flags::numbr, flags::real, 0, NULL);
								high_pointer = this->point_left;
								low_pointer = high_pointer;
							}
						}
						//скобка открывается не в начале строки
						else
						{
							//создание элемента класса и запись нуля в выражение
							high_pointer->point_right = new math_obj(flags::numbr, flags::real, 0, NULL);
							low_pointer = high_pointer->point_right;
						}
					}
					//в скобках что-то есть и есть запятые
					else if (comma != 0)
					{
						//сразу же считаем содержимое скобок вектором
						temp_str.assign(s_iter, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
						temp_pointer = new math_obj(comma + 1, &temp_str[0], &temp_str[temp_str.length() - 1], point_up);
						if (temp_pointer->prop == flags::error)
						{
							temp_pointer->vector_destruct();
							delete temp_pointer;
							return NULL;
						}
						//скобка открывается в начале строки
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							//и закрывается в конце
							if (*(temp + 1) == NULL)
							{
								copy(temp_pointer);
								//наверх выдаётся сам созданный вектор, без проверки что в нём
								delete temp_pointer;
							}
							//далее какие-либо операции
							else
							{
								if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v))
								{
									//текущий элемент - выражение
									if (type == flags::exprs)
									{
										//high_pointer - вектор со списком переменных.
										high_pointer = temp_pointer->create_var_list(NULL);
										convert_to(flags::funct, flags::undef, high_pointer->var, high_pointer->point_left);
										delete high_pointer;
									}
									//текущий элемент - функция (если вектор попался в начале строки - функция с заранее заданными переменными)
									else if (type == flags::funct)
									{
										high_pointer = new math_obj(L"", flags::funct, flags::arg_v, flags::nthng, temp_pointer->var, NULL, temp_pointer, NULL);
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
											high_pointer->prop = flags::servc;
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
							//Делить на вектор нельзя! Возводить в векторную степень нельзя!! Данная проверка не сработает в случае записи 5/(7+(2,4)) - её нужно будеть делать в случае "обычных" скобок
							if ((high_pointer->type == flags::divis) || (high_pointer->type == flags::power))
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIVISION);
								return NULL;
							}
							//Проверка на совпадения размерностей векторов
							if ((type == flags::vectr) && (var != temp_pointer->var))
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
								return NULL;
							}
							if (get_type() == flags::exprs)
							{
								if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v))
								{
									low_pointer = temp_pointer->create_var_list(NULL);
									convert_to(flags::funct, flags::undef, low_pointer->var, low_pointer->point_left);
									delete low_pointer;
								}
							}
							else if (get_type() == flags::funct)
							{
								if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v))
								{
									if (get_pc()->point_left != NULL)
									{
										low_pointer = new math_obj(L"", flags::funct, flags::arg_v, flags::nthng, temp_pointer->var, NULL, temp_pointer, NULL);
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
											low_pointer->prop = flags::servc;
											low_pointer->var = get_var();
											low_pointer = temp_pointer->create_var_list(low_pointer);
											delete low_pointer;
										}
									}
									else
									{
										low_pointer = new math_obj((size_t)0);
										low_pointer->point_left = get_pc();
										low_pointer->prop = flags::servc;
										low_pointer->var = get_var();
										low_pointer = temp_pointer->create_var_list(low_pointer);
										assing_var(low_pointer->var);
										delete low_pointer;//здесь не нужно снова передавать указатель point_collar в функцию. вроде бы
									}
								}
							}
							high_pointer->point_right = temp_pointer;
							low_pointer = high_pointer->point_right;
							if (type != flags::vectr)
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
					//если в скобках не было запятых. Точнее не было запятых на "первом уровне" скобок
					else
					{
						temp_str.assign(s_iter + 1, temp);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки НЕ находились в строке.
						temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);
						if (temp_pointer->prop == flags::error)
						{
							delete temp_pointer;
							return NULL;
						}
						//скобка открывается в начале строки
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							//и закрывается в конце
							if (*(temp + 1) == NULL)
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
								if (type == flags::exprs)
								{
									if (temp_pointer->type == flags::varbl)
									{
										if (temp_pointer->prop == flags::undef)
										{
											temp_pointer->type = flags::defnd;
											point_up->push_left(new math_obj(temp_pointer));
										}
										temp_pointer->var = 0;
										convert_to(flags::funct, flags::undef, 1, temp_pointer);
									}
									else if (temp_pointer->type == flags::funct)
									{
										if (temp_pointer->actn == flags::write)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										if (temp_pointer->prop != flags::arg_c)
										{
											low_pointer = temp_pointer->create_var_list(NULL);
											convert_to(flags::funct, flags::undef, low_pointer->var, low_pointer->point_left);
											delete low_pointer;
										}
									}
									else if (temp_pointer->type == flags::vectr)
									{

										if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v) || ((temp_pointer->prop == flags::funct) && (temp_pointer->get_prop() != flags::arg_c)))
										{
											//low_pointer - вектор со списком переменных.
											low_pointer = temp_pointer->create_var_list(NULL);
											convert_to(flags::funct, flags::undef, low_pointer->var, low_pointer->point_left);
											delete low_pointer;

										}
										//если вектор состоит только из констант - записываем в дерево операций, выражение или функция становятся векторными	
										/*если в скобках векторное выражение или функция И самая верхняя операция для которых - умножение И правый и левый операнды для умножения - векторы (проверка на совпадение их размерностей должна быть проведена раньше)
										- получено скалярное произведение векторов. Ответ для которого - скаляр!*/
										if (((temp_pointer->prop == flags::exprs) || (temp_pointer->prop == flags::funct)) && (get_pl()->type == flags::mltpl) && (get_pl()->point_left->type == flags::vectr) && (get_pl()->point_right->type == flags::vectr))
										{
											//тогда результат такого произведения не может быть векторным
											temp_pointer->unvectorize();
										}
										//если скалярное произведение не получено - этот элемент становится векторным
										else
										{
											buf = temp_pointer->var;
											multiple_var = new math_obj(this);
											low_pointer = new math_obj((int)buf, multiple_var);
											copy(low_pointer);
											delete low_pointer;
										}
									}
									assing_pl(temp_pointer);
									high_pointer = get_pl();
									low_pointer = high_pointer;
									link_var_list_to_funct();
								}
								//текущий элемент - функция (если вектор попался в начале строки - функция с заранее заданными переменными)
								else if (type == flags::funct)
								{
									if (temp_pointer->type == flags::varbl)
									{
										low_pointer = find_by_name(temp_pointer);
										delete temp_pointer;
										if (low_pointer == NULL)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										temp_pointer = low_pointer;
									}
									else if (temp_pointer->type == flags::funct)
									{
										if (temp_pointer->actn == flags::write)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										if (temp_pointer->prop != flags::arg_c)
										{
											if (var_list_compare(temp_pointer) != 0)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											low_pointer = new math_obj((size_t)0);
											low_pointer->point_left = point_collar;
											low_pointer->prop = flags::servc;
											low_pointer->var = var;
											low_pointer = temp_pointer->create_var_list(low_pointer);
											delete low_pointer;
										}
									}
									else if (temp_pointer->type == flags::vectr)
									{

										if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v) || ((temp_pointer->prop == flags::funct) && (temp_pointer->get_prop() != flags::arg_c)))
										{
											count_var = 0;
											if (temp_pointer->prop == flags::funct)
												count_var = var_list_compare(temp_pointer);
											else
											{
												low_pointer = new math_obj(L"", flags::funct, flags::arg_v, flags::nthng, temp_pointer->var, NULL, temp_pointer, NULL);
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
												low_pointer->prop = flags::servc;
												low_pointer->var = var;
												low_pointer = temp_pointer->create_var_list(low_pointer);
												delete low_pointer;
											}
										}
										//если вектор состоит только из констант - записываем в дерево операций, выражение или функция становятся векторными	
										/*если в скобках векторное выражение или функция И самая верхняя операция для которых - умножение И правый и левый операнды для умножения - векторы (проверка на совпадение их размерностей должна быть проведена раньше)
										- получено скалярное произведение векторов. Ответ для которого - скаляр!*/
										if (((temp_pointer->prop == flags::exprs) || (temp_pointer->prop == flags::funct)) && (get_pl()->type == flags::mltpl) && (get_pl()->point_left->type == flags::vectr) && (get_pl()->point_right->type == flags::vectr))
										{
											//тогда результат такого произведения не может быть векторным
											temp_pointer->unvectorize();
										}
										//если скалярное произведение не получено - этот элемент становится векторным
										else
										{
											buf = temp_pointer->var;
											multiple_var = new math_obj(this);
											low_pointer = new math_obj((int)buf, multiple_var);
											copy(low_pointer);
											delete low_pointer;
										}
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
							if (get_type() == flags::exprs)
							{
								if (temp_pointer->type == flags::varbl)
								{
									if (temp_pointer->prop == flags::undef)
									{
										temp_pointer->type = flags::defnd;
										point_up->push_left(new math_obj(temp_pointer));
									}
									temp_pointer->var = 0;
									convert_to(flags::funct, flags::undef, 1, temp_pointer);
								}
								else if (temp_pointer->type == flags::funct)
								{
									if (temp_pointer->actn == flags::write)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									if (temp_pointer->prop != flags::arg_c)
									{
										low_pointer = temp_pointer->create_var_list(NULL);
										convert_to(flags::funct, flags::undef, temp_pointer->var, low_pointer->point_left);
										delete low_pointer;
									}
								}
								else if (temp_pointer->type == flags::vectr)
								{
									if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v) || ((temp_pointer->prop == flags::funct) && (temp_pointer->get_prop() != flags::arg_c)))
									{
										//low_pointer - вектор со списком переменных.
										low_pointer = temp_pointer->create_var_list(NULL);
										convert_to(flags::funct, flags::undef, low_pointer->var, low_pointer->point_left);
										delete low_pointer;
									}
									//если вектор состоит только из констант - записываем в дерево операций, выражение или функция становятся векторными	
									/*если в скобках векторное выражение или функция И самая верхняя операция для которых - умножение И правый и левый операнды для умножения - векторы (проверка на совпадение их размерностей должна быть проведена раньше)
									- получено скалярное произведение векторов. Ответ для которого - скаляр!*/
									if (((temp_pointer->prop == flags::exprs) || (temp_pointer->prop == flags::funct)) && (get_pl()->type == flags::mltpl) && (get_pl()->point_left->type == flags::vectr) && (get_pl()->point_right->type == flags::vectr))
									{
										//тогда результат такого произведения не может быть векторным
										temp_pointer->unvectorize();
									}
									else
									{
										//если из скобок получен вектор - необходима проверка на размерность
										if ((type == flags::vectr) && (var != temp_pointer->var))
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
											return NULL;
										}
										//Делить на вектор нельзя! Возводить в векторную степень нельзя!!Проверка последней записанной операции
										if ((high_pointer->type == flags::divis) || (high_pointer->type == flags::power))
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIVISION);
											return NULL;
										}
										if (type != flags::vectr)
										{
											buf = temp_pointer->var;
											multiple_var = new math_obj(this);
											low_pointer = new math_obj((int)buf, multiple_var);
											copy(low_pointer);
											delete low_pointer;
										}
									}
								}
								link_var_list_to_funct(); //нужная вещь
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
							else if (get_type() == flags::funct)
							{
								if (temp_pointer->type == flags::varbl)
								{
									low_pointer = find_by_name(temp_pointer);
									if (low_pointer == NULL)
									{
										if (get_pc()->point_left != NULL)
										{
											delete temp_pointer;
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											if (temp_pointer->prop == flags::undef)
											{
												temp_pointer->type = flags::defnd;
												point_up->push_left(new math_obj(temp_pointer));
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
								else if (temp_pointer->type == flags::funct)
								{
									if (temp_pointer->actn == flags::write)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									if (temp_pointer->prop != flags::arg_c)
									{
										if ((var_list_compare(temp_pointer) != 0) && (get_pc()->point_left != NULL))
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										//должно сработать в любом случае
										low_pointer = new math_obj((size_t)0);
										low_pointer->point_left = get_pc();
										low_pointer->prop = flags::servc;
										low_pointer->var = get_var();
										low_pointer = temp_pointer->create_var_list(low_pointer);
										delete low_pointer;
									}
								}
								else if (temp_pointer->type == flags::vectr)
								{
									if ((type == flags::vectr) && (var != temp_pointer->var))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
										return NULL;
									}
									if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v) || ((temp_pointer->prop == flags::funct) && (temp_pointer->get_prop() != flags::arg_c)))
									{
										count_var = 0;
										if (temp_pointer->prop == flags::funct)
											count_var = var_list_compare(temp_pointer);
										else
										{
											low_pointer = new math_obj(L"", flags::funct, flags::arg_v, flags::nthng, temp_pointer->var, NULL, temp_pointer, NULL);
											count_var = var_list_compare(low_pointer);
											delete low_pointer;
										}
										if ((count_var != 0) && (get_pc()->point_left != NULL))
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										low_pointer = new math_obj((size_t)0);
										low_pointer->point_left = get_pc();
										low_pointer->prop = flags::servc;
										low_pointer->var = get_var();
										low_pointer = temp_pointer->create_var_list(low_pointer);
										delete low_pointer;
									}
									//если вектор состоит только из констант - записываем в дерево операций, выражение или функция становятся векторными
									/*если в скобках векторное выражение или функция И самая верхняя операция для которых - умножение И правый и левый операнды для умножения - векторы (проверка на совпадение их размерностей должна быть проведена раньше)
									- получено скалярное произведение векторов. Ответ для которого - скаляр!*/
									if (((temp_pointer->prop == flags::exprs) || (temp_pointer->prop == flags::funct)) && (get_pl()->type == flags::mltpl) && (get_pl()->point_left->type == flags::vectr) && (get_pl()->point_right->type == flags::vectr))
									{
										//тогда результат такого произведения не может быть векторным
										temp_pointer->unvectorize();
									}
									//если скалярное произведение не получено - этот элемент становится векторным (если ещё не был)
									else
									{
										//если из скобок получен вектор - необходима проверка на размерность
										if ((type == flags::vectr) && (var != temp_pointer->var))
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIMENSIONS);
											return NULL;
										}
										//Делить на вектор нельзя! Возводить в векторную степень нельзя!!Проверка последней записанной операции
										if ((high_pointer->type == flags::divis) || (high_pointer->type == flags::power))
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTOR_DIVISION);
											return NULL;
										}
										if (type != flags::vectr)
										{
											buf = temp_pointer->var;
											multiple_var = new math_obj(this);
											low_pointer = new math_obj((int)buf, multiple_var);
											copy(low_pointer);
											delete low_pointer;
										}
									}
								}
								// если этот элемент функция с незамкнутым спископ переменных - настроить указатели для переменных
								if (get_pc()->point_left == NULL)
									link_var_list_to_funct();
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
						}
					}
					//сразу переходим к следующей позиции после скобки					
					s_iter = temp + 1;
					if ((*s_iter == '(') || (*s_iter == ','))
					{
						//по идее запись вида )( может означать )*(. Потом добавть сюда определение для подобного случая
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
						return NULL;
					}
				}
				else if (*s_iter == '[')
				{
					//векторное произведение
				}
				//случай с символами в строке
				else
				{
					temp = wcspbrk(s_iter, L"()+-*^/");
					//если строчка начинается с cимвола
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						if (temp == NULL)
						{
							name_str.assign(s_iter, endPtr + 1);
							s_iter = endPtr + 1;
						}
						else
						{
							name_str.assign(s_iter, temp);
							s_iter = temp;
						}
						high_pointer = point_up->left->find_math_obj(&name_str);
						//если не найден ни один элемент массива мат объектов с таким именем
						if (high_pointer == NULL)
						{
							//считаем найденный элемент переменной
							if (temp == NULL)
							{
								convert_totaly(name_str, flags::varbl, flags::undef, flags::nthng, 0, NULL, NULL, NULL);
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
								//s_iter указывает на открывающую скобку, temp - на закрывающую
								temp_str.assign(s_iter, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
								temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], (math_obj*)NULL, point_up);
								if (temp_pointer->prop == flags::error)
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
								if ((temp_pointer->type == flags::numbr) && (temp_pointer->var == 0))
								{
									//пустая строка в скобках.  создание функции от неопределённого количества переменных
									//аналогично заданию функции вообще без указания переменных. Указатель на переменные - пустой
									convert_totaly(name_str, flags::funct, flags::undef, flags::write, 0, NULL, NULL, NULL);
									//если получили запись вида f(), где f - неизвестная функция и в скобках ничего нет - передаём ей параметр flags::write. 
									delete temp_pointer; temp_pointer = NULL; //этот ноль больше нам не нужен								
								}
								//если в скобках вектор, то он может состоять только из переменных. Любой другой случай - передача аргументов необъявленной функции
								else if ((temp_pointer->type == flags::vectr) && (temp_pointer->prop == flags::only_arg_v))
								{
									temp_pointer->double_lincked_vector();
									convert_totaly(name_str, flags::funct, flags::undef, flags::write, temp_pointer->var, NULL, NULL, temp_pointer->point_left);
									//если получили запись вида f(), где f - неизвестная функция - передаём ей параметр flags::write.
									delete temp_pointer; temp_pointer = NULL;
									link_var_list_to_funct();
									close_list();
								}
								//в скобках переменная - определяем функцию одного переменного
								else if (temp_pointer->type == flags::varbl)
								{
									if (temp_pointer->prop == flags::undef)
									{
										temp_pointer->prop = flags::defnd;
										point_up->push_left(new math_obj(temp_pointer));
									}
									temp_pointer->var = 0;
									temp_pointer->point_collar = this;
									convert_totaly(name_str, flags::funct, flags::undef, flags::write, 1, NULL, NULL, temp_pointer);
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
								s_iter = temp + 1;
							}
							//после буквосочетания идут операции
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								//найденное буквосочетание - переменная, текущий элемент - функция							
								temp_pointer = new math_obj(name_str, flags::varbl, flags::defnd, 0);
								point_up->push_left(new math_obj(temp_pointer));
								//копия переменной с указателем на функцию
								temp_pointer->point_collar = this;
								convert_totaly(L"", flags::funct, flags::undef, flags::nthng, 1, temp_pointer, NULL, temp_pointer);
								high_pointer = get_pl();
								low_pointer = high_pointer;
							}
							else if (*temp == ')')
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
						else if (high_pointer->type == flags::cnst)
						{
							//оттадём наверх копиию константы
							if (temp == NULL)
							{
								copy(high_pointer);
								actn = flags::nthng;
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
									//s_iter указывает на открывающую скобку, temp - на закрывающую
									temp_str.assign(s_iter, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
									temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);
									if (temp_pointer->prop == flags::error)
									{
										delete temp_pointer;
										return NULL;
									}
									if ((temp_pointer->type == flags::numbr) && (temp_pointer->var == 0))
									{
										//пустая строка в скобках.  создание функции от неопределённого количества переменных
										//аналогично заданию функции вообще без указания переменных. Указатель на переменные - пустой
										convert_totaly(name_str, flags::funct, flags::undef, flags::write, 0, NULL, NULL, NULL);
										//если получили запись вида f(), где f - ранее обозначенная константа и в скобках ничего нет - передаём ей параметр flags::write. 
										delete temp_pointer; temp_pointer = NULL; //этот ноль больше нам не нужен
										//здесь намеренно игнорируется возможность с() = с*() = с*0 = 0. 
									}
									//если в скобках вектор, то он может состоять только из переменных. Любой другой случай - константа не поменяется
									else if ((temp_pointer->type == flags::vectr) && (temp_pointer->prop == flags::only_arg_v))
									{
										temp_pointer->double_lincked_vector();
										convert_totaly(name_str, flags::funct, flags::undef, flags::write, temp_pointer->var, NULL, NULL, temp_pointer->point_left);
										//если получили запись вида f(), где f - ранее обозначенная константа - передаём ей параметр flags::write.
										delete temp_pointer; temp_pointer = NULL;
										link_var_list_to_funct();
										close_list();
									}
									//в скобках переменная - определяем функцию одного переменного
									else if (temp_pointer->type == flags::varbl)
									{
										if (temp_pointer->prop == flags::undef)
										{
											temp_pointer->prop = flags::defnd;
											point_up->push_left(new math_obj(temp_pointer));
										}
										temp_pointer->var = 0;
										temp_pointer->point_collar = this;
										convert_totaly(name_str, flags::funct, flags::undef, flags::write, 1, NULL, NULL, temp_pointer);
										close_list();
									}
									else
									{
										//вылезло что угодно другое - this -> копия найденной константы
										//TODO: временно игнорируется возможность с(...) = с*(...). доработать.
										copy(high_pointer);
										actn = flags::nthng;
									}
									high_pointer = this;
									low_pointer = high_pointer;
									s_iter = temp + 1;
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
								point_left->actn = flags::nthng;
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
						else if (high_pointer->type == flags::varbl)
						{
							if (temp == NULL)
							{
								//случай для простейшей функции
								if ((type == flags::funct) && (point_collar != NULL))
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
									actn = flags::nthng;
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
								//s_iter указывает на открывающую скобку, temp - на закрывающую
								temp_str.assign(s_iter, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
								temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);
								if (temp_pointer->prop == flags::error)
								{
									delete temp_pointer;
									return NULL;
								}
								if ((temp_pointer->type == flags::numbr) && (temp_pointer->var == 0))
								{
									//пустая строка в скобках.  создание функции от неопределённого количества переменных
									//аналогично заданию функции вообще без указания переменных. Указатель на переменные - пустой
									convert_totaly(name_str, flags::funct, flags::undef, flags::write, 0, NULL, NULL, NULL);
									//если получили запись вида f(), где f - ранее обозначенная перменная и в скобках ничего нет - передаём ей параметр flags::write. 
									delete temp_pointer; temp_pointer = NULL; //этот ноль больше нам не нужен
									//здесь намеренно игнорируется возможность x() = x*() = x*0 = 0. 
								}
								//если в скобках вектор, то он может состоять только из переменных. Любой другой случай - константа не поменяется
								else if ((temp_pointer->type == flags::vectr) && (temp_pointer->prop == flags::only_arg_v))
								{
									//TODO: проверять чтобы в векторе с переменными не было переменной с тем же именем (и вообще они не должны повторятся)
									temp_pointer->double_lincked_vector();
									convert_totaly(name_str, flags::funct, flags::undef, flags::write, temp_pointer->var, NULL, NULL, temp_pointer->point_left);
									//если получили запись вида f(), где f - ранее обозначенная переменная - передаём ей параметр flags::write.
									delete temp_pointer; temp_pointer = NULL;
									link_var_list_to_funct();
									close_list();
								}
								//в скобках переменная - определяем функцию одного переменного
								else if (temp_pointer->type == flags::varbl)
								{
									//Проверка: запись вида x(x).
									if (temp_pointer->name == name_str)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::USING_VAR_AS_FUNC);
										return NULL;
									}
									if (temp_pointer->prop == flags::undef)
									{
										temp_pointer->prop = flags::defnd;
										point_up->push_left(new math_obj(temp_pointer));
									}
									temp_pointer->var = 0;
									temp_pointer->point_collar = this;
									convert_totaly(name_str, flags::funct, flags::undef, flags::write, 1, NULL, NULL, temp_pointer);
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
								s_iter = temp + 1;
							}
							//текущий элемент становится функцией от найденной переменной
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								if ((type == flags::funct) && (point_collar != NULL))
								{
									temp_pointer = find_by_name(high_pointer);
									if (temp_pointer == NULL)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									assing_pl(temp_pointer);
									high_pointer = get_pl();
									low_pointer = high_pointer;
								}
								else
								{
									//найденное буквосочетание - переменная, текущий элемент - функция							
									temp_pointer = new math_obj(high_pointer);
									temp_pointer->point_collar = this;
									convert_totaly(L"", flags::funct, flags::undef, flags::nthng, 1, temp_pointer, NULL, temp_pointer);
									high_pointer = get_pl();
									low_pointer = high_pointer;
								}
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
						//найден элемент массива мат объектов с совпадающим именем - функция
						else if (high_pointer->type == flags::funct)
						{
							//выделяется два случая
							//текущий элемент - выражение
							if (get_type() == flags::exprs)
							{
								if (temp == NULL)
								{
									copy(high_pointer);
									actn = flags::nthng;
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
									//s_iter указывает на открывающую скобку, temp - на закрывающую
									temp_str.assign(s_iter, temp + 1);
									temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);

									if (temp_pointer->prop == flags::error)
									{
										delete temp_pointer;
										return NULL;
									}
									if ((temp_pointer->type == flags::numbr) && (temp_pointer->var == 0))
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
											high_pointer->actn = flags::nthng;
											//low_pointer - список переменных high_pointer, в том числе внутри других функций
											low_pointer = high_pointer->create_var_list(NULL);
											convert_totaly(L"", flags::funct, flags::undef, flags::nthng, low_pointer->var, high_pointer, NULL, low_pointer->point_left);
											delete low_pointer;
											link_var_list_to_funct();
											high_pointer = point_left;
											low_pointer = high_pointer;
										}
										delete temp_pointer;
									}
									else if (temp_pointer->type == flags::varbl)
									{
										if (temp_pointer->prop == flags::undef)
										{
											temp_pointer->prop = flags::defnd;
											point_up->push_left(new math_obj(temp_pointer));
										}
										if (high_pointer->var == 1)
										{
											if (temp == endPtr)
											{
												copy(high_pointer);
												actn = flags::nthng;
												prop = flags::arg_v;
												low_pointer = new math_obj((size_t)0);
												low_pointer->vector_push_back(temp_pointer);
												low_pointer->prop = flags::only_arg_v;
												point_right = low_pointer;
												high_pointer = this;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												high_pointer = new math_obj(high_pointer);
												high_pointer->actn = flags::nthng;
												high_pointer->prop = flags::arg_v;
												low_pointer = new math_obj((size_t)0);
												low_pointer->vector_push_back(temp_pointer);
												low_pointer->prop = flags::only_arg_v;
												high_pointer->point_right = low_pointer;
												//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
												low_pointer = high_pointer->create_var_list(NULL);
												convert_totaly(L"", flags::funct, flags::undef, flags::nthng, low_pointer->var, high_pointer, NULL, low_pointer->point_left);
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
										else
										{
											if (temp != endPtr)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											convert_totaly(name_str, flags::funct, flags::undef, flags::write, 1, NULL, NULL, temp_pointer);
											//если получили запись вида f(), где f - ранее обозначенная функция - передаём ей параметр flags::write.
											delete temp_pointer; temp_pointer = NULL;
											link_var_list_to_funct();
											close_list();
											high_pointer = this;
											low_pointer = high_pointer;
										}
									}
									else if ((temp_pointer->type == flags::cnst) || ((temp_pointer->type == flags::funct) && (temp_pointer->prop == flags::arg_c)) || (temp_pointer->type == flags::exprs))
									{
										if (high_pointer->var == 1)
										{
											if (temp == endPtr)
											{
												copy(high_pointer);
												actn = flags::nthng;
												prop = flags::arg_c;
												low_pointer = new math_obj((size_t)0);
												low_pointer->vector_push_back(temp_pointer);
												low_pointer->prop = flags::arg_c;
												point_right = low_pointer;
												high_pointer = this;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												low_pointer = new math_obj((size_t)0);
												low_pointer->vector_push_back(temp_pointer);
												low_pointer->prop = flags::arg_c;
												point_left = new math_obj(high_pointer);
												point_left->prop = flags::arg_c;
												point_left->actn = flags::nthng;
												point_left->point_right = low_pointer;
												high_pointer = point_left;
												low_pointer = high_pointer;
											}
											else
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
												return NULL;
											}
										}
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
									}
									else if (temp_pointer->type == flags::funct)
									{
										if (temp_pointer->actn == flags::write)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										if (high_pointer->var == 1)
										{
											if (temp == endPtr)
											{
												copy(high_pointer);
												actn = flags::nthng;
												prop = flags::arg_v;
												low_pointer = new math_obj((size_t)0);
												low_pointer->vector_push_back(temp_pointer);
												low_pointer->prop = flags::arg_v;
												point_right = low_pointer;
												high_pointer = this;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												high_pointer = new math_obj(high_pointer);
												high_pointer->prop = flags::arg_v;
												low_pointer = new math_obj((size_t)0);
												low_pointer->vector_push_back(temp_pointer);
												low_pointer->prop = flags::arg_v;
												high_pointer->point_right = low_pointer;
												//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
												low_pointer = high_pointer->create_var_list(NULL);
												convert_totaly(L"", flags::funct, flags::undef, flags::nthng, low_pointer->var, high_pointer, NULL, low_pointer->point_left);
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
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
									}
									else if (temp_pointer->type == flags::vectr)
									{
										//число аргументов в скобках совпадает с числом аргументов у определённой заранее функции
										if (high_pointer->var == temp_pointer->var)
										{
											//подаём полученный вектор в качестве аргументов данной функции (даже если он состоит только из переменных
											if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v))
											{
												//если строка заканчивается - отдаём наверх найденную функцию с параметром flags::arg_v
												if (temp == endPtr)
												{
													copy(high_pointer);
													actn = flags::nthng;
													prop = flags::arg_v;
													point_right = temp_pointer;
													high_pointer = this;
													low_pointer = high_pointer;
												}
												else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													high_pointer = new math_obj(high_pointer);
													high_pointer->actn = flags::nthng;
													high_pointer->point_right = temp_pointer;
													high_pointer->prop = flags::arg_v;
													//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
													low_pointer = high_pointer->create_var_list(NULL);
													convert_totaly(L"", flags::funct, flags::undef, flags::nthng, low_pointer->var, high_pointer, NULL, low_pointer->point_left);
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
											else if (temp_pointer->prop == flags::arg_c)
											{
												if (temp == endPtr)
												{
													copy(high_pointer);
													actn = flags::nthng;
													prop = flags::arg_c;
													point_right = temp_pointer;
													high_pointer = this;
													low_pointer = high_pointer;
												}
												else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													point_left = new math_obj(high_pointer);
													point_left->actn = flags::nthng;
													point_left->prop = flags::arg_c;
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
												if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v) || ((temp_pointer->prop == flags::funct) && (temp_pointer->get_prop() != flags::arg_c)))
												{
													//если строка заканчивается - отдаём наверх найденную функцию с параметром flags::arg_v
													if (temp == endPtr)
													{
														copy(high_pointer);
														actn = flags::nthng;
														prop = flags::arg_v;
														point_right = temp_pointer;
														high_pointer = this;
														low_pointer = high_pointer;
													}
													else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
													{
														high_pointer = new math_obj(high_pointer);
														high_pointer->actn = flags::nthng;
														high_pointer->point_right = temp_pointer;
														high_pointer->prop = flags::arg_v;
														//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
														low_pointer = high_pointer->create_var_list(NULL);
														temp_pointer = new math_obj((int)buf, high_pointer);
														convert_totaly(L"", flags::funct, flags::undef, flags::nthng, low_pointer->var, temp_pointer, NULL, low_pointer->point_left);
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
												else //if ((temp_pointer->prop == flags::arg_c) || ((temp_pointer->prop == flags::funct) && (temp_pointer->get_prop() == flags::arg_c))|| (temp_pointer->prop == exps) || (temp_pointer->prop == flags::cnst))))
												{
													if (temp == endPtr)
													{
														copy(high_pointer);
														actn = flags::nthng;
														prop = flags::arg_c;
														point_right = temp_pointer;
														high_pointer = this;
														low_pointer = high_pointer;
													}
													else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
													{
														high_pointer = new math_obj(high_pointer);
														high_pointer->actn = flags::nthng;
														high_pointer->point_right = temp_pointer;
														high_pointer->prop = flags::arg_c;
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
												if ((temp != endPtr) || (temp_pointer->prop != flags::only_arg_v))
												{
													ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
													return NULL;
												}
												temp_pointer->double_lincked_vector();
												convert_totaly(name_str, flags::funct, flags::undef, flags::write, temp_pointer->var, NULL, NULL, temp_pointer->point_left);
												//если получили запись вида f(), где f - ранее обозначенная перменная - передаём ей параметр flags::write.
												delete temp_pointer; temp_pointer = NULL;
												link_var_list_to_funct();
												close_list();
												high_pointer = this;
												low_pointer = high_pointer;
											}
										}
									}
									s_iter = temp + 1;
								}
								else if (*temp == ')')
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
									return NULL;
								}
								else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
								{
									high_pointer = new math_obj(high_pointer);
									high_pointer->actn = flags::nthng;
									//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
									low_pointer = high_pointer->create_var_list(NULL);
									convert_totaly(L"", flags::funct, flags::undef, flags::nthng, low_pointer->var, high_pointer, NULL, low_pointer->point_left);
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
							//текущий элемент функция (только с замкнутым списком переменных)
							else if ((get_type() == flags::funct) && (get_pc()->point_left != NULL))
							{
								if ((temp == NULL) || (*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
								{
									if (var_list_compare(high_pointer) != 0)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									high_pointer = new math_obj(high_pointer);
									high_pointer->actn = flags::nthng;
									low_pointer = new math_obj((size_t)0);
									low_pointer->prop = flags::servc;
									low_pointer->point_left = get_pc();
									low_pointer->var = var;
									low_pointer = high_pointer->create_var_list(low_pointer);
									convert_totaly(L"", flags::funct, flags::undef, flags::nthng, var, high_pointer, NULL, low_pointer->point_left);
									delete low_pointer;
									high_pointer = get_pl();
									low_pointer = high_pointer;
								}
								else if (*temp == '(')
								{
									temp = brackets_close(temp, endPtr);
									if (temp == NULL)
									{
										return NULL;
									}
									//s_iter указывает на открывающую скобку, temp - на закрывающую
									temp_str.assign(s_iter, temp + 1);
									temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);
									if (temp_pointer->prop == flags::error)
									{
										delete temp_pointer;
										return NULL;
									}
									if ((temp != endPtr) && (*(temp + 1) != '+') && (*(temp + 1) != '-') && (*(temp + 1) != '*') && (*(temp + 1) != '/') && (*(temp + 1) != '^'))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
										return NULL;
									}
									if ((temp_pointer->type == flags::numbr) && (temp_pointer->var == 0))
									{
										if (var_list_compare(high_pointer) != 0)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										high_pointer = new math_obj(high_pointer);
										high_pointer->actn = flags::nthng;
										low_pointer = new math_obj((size_t)0);
										low_pointer->prop = flags::servc;
										low_pointer->point_left = get_pc();
										low_pointer->var = var;
										low_pointer = high_pointer->create_var_list(low_pointer);
										assing_pl(high_pointer);//поскольку список переменных уже задан, снова его назначать не нужно
										delete low_pointer;
										high_pointer = get_pl();
										low_pointer = high_pointer;
										delete temp_pointer;
									}
									else if (temp_pointer->type == flags::varbl)
									{
										//на данном шаге неопределённая переменная будет ошибкой
										if (temp_pointer->prop == flags::undef)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										low_pointer = find_by_name(temp_pointer);
										if (low_pointer == NULL)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										if (high_pointer->var == 1)
										{
											high_pointer = new math_obj(high_pointer);
											high_pointer->actn = flags::nthng;
											high_pointer->prop = flags::arg_v;
											low_pointer = new math_obj((size_t)0);
											low_pointer->vector_push_back(temp_pointer);
											low_pointer->prop = flags::only_arg_v;
											high_pointer->point_right = low_pointer;
											//
											low_pointer = new math_obj((size_t)0);
											low_pointer->prop = flags::servc;
											low_pointer->point_left = get_pc();
											low_pointer->var = var;
											low_pointer = high_pointer->create_var_list(low_pointer);
											assing_pl(high_pointer);
											delete low_pointer;
											high_pointer = get_pl();
											low_pointer = high_pointer;
										}
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
									}
									else if (temp_pointer->type == flags::funct)
									{
										if (temp_pointer->actn == flags::write)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										if (high_pointer->var == 1)
										{
											if (var_list_compare(temp_pointer) != 0)//сравнивать необходимо переменные только для функции в скобках
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											high_pointer = new math_obj(high_pointer);
											high_pointer->actn = flags::nthng;
											high_pointer->prop = flags::arg_v;
											low_pointer = new math_obj((size_t)0);
											low_pointer->vector_push_back(temp_pointer);
											low_pointer->prop = flags::arg_v;
											high_pointer->point_right = low_pointer;
											//хз, может в таком виде не работать
											low_pointer = new math_obj((size_t)0);
											low_pointer->prop = flags::servc;
											low_pointer->point_left = get_pc();
											low_pointer->var = var;
											low_pointer = high_pointer->create_var_list(low_pointer);
											//
											assing_pl(high_pointer);
											delete low_pointer;
											high_pointer = get_pl();
											low_pointer = high_pointer;
										}
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
									}
									else if ((temp_pointer->type == flags::cnst) || ((temp_pointer->type == flags::funct) && (temp_pointer->prop == flags::arg_c)) || (temp_pointer->type == flags::exprs))
									{
										if (high_pointer->var == 1)
										{
											low_pointer = new math_obj((size_t)0);
											low_pointer->vector_push_back(temp_pointer);
											low_pointer->prop = flags::arg_c;
											assing_pl(new math_obj(high_pointer));
											high_pointer = get_pl();
											high_pointer->prop = flags::arg_c;
											high_pointer->actn = flags::nthng;
											high_pointer->point_right = low_pointer;
											low_pointer = high_pointer;
										}
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
									}
									else if (temp_pointer->type == flags::vectr)
									{
										//число аргументов в скобках совпадает с числом аргументов у определённой заранее функции
										if (high_pointer->var == temp_pointer->var)
										{
											if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v))
											{
												if ((temp == endPtr) || (*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													high_pointer = new math_obj(high_pointer);
													high_pointer->actn = flags::nthng;
													high_pointer->point_right = temp_pointer;
													high_pointer->prop = flags::arg_v;
													if (var_list_compare(high_pointer) != 0)
													{
														ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
														return NULL;
													}
													low_pointer = new math_obj((size_t)0);
													low_pointer->prop = flags::servc;
													low_pointer->point_left = get_pc();
													low_pointer->var = var;
													low_pointer = high_pointer->create_var_list(low_pointer);
													assing_pl(high_pointer);
													delete low_pointer;
													high_pointer = get_pl();
													low_pointer = high_pointer;
												}
												else
												{
													ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
													return NULL;
												}
											}
											else if (temp_pointer->prop == flags::arg_c)
											{
												if ((temp == endPtr) || (*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													assing_pl(new math_obj(high_pointer));
													high_pointer = get_pl();
													high_pointer->actn = flags::nthng;
													high_pointer->prop = flags::arg_c;
													high_pointer->point_right = temp_pointer;
													low_pointer = high_pointer;
												}
												else
												{
													ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
													return NULL;
												}
											}
										}
										else
										{
											if (high_pointer->var == 1)
											{
												buf = temp_pointer->var;
												if ((temp_pointer->prop == flags::arg_v) || (temp_pointer->prop == flags::only_arg_v) || ((temp_pointer->prop == flags::funct) && (temp_pointer->get_prop() != flags::arg_c)))
												{
													high_pointer = new math_obj(high_pointer);
													high_pointer->actn = flags::nthng;
													high_pointer->point_right = temp_pointer;
													high_pointer->prop = flags::arg_v;
													if (var_list_compare(high_pointer) != 0)
													{
														ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
														return NULL;
													}
													//low_pointer - список переменных, переданных в high_pointer, в том числе внутри других функций
													low_pointer = new math_obj((size_t)0);
													low_pointer->prop = flags::servc;
													low_pointer->point_left = get_pc();
													low_pointer->var = var;
													low_pointer = high_pointer->create_var_list(low_pointer);
													temp_pointer = new math_obj((int)buf, high_pointer);
													assing_pl(temp_pointer);
													delete low_pointer;													
													high_pointer = get_pl();
													low_pointer = high_pointer;
												}
												else//не переменные аргументы
												{
													high_pointer = new math_obj(high_pointer);
													high_pointer->actn = flags::nthng;
													high_pointer->point_right = temp_pointer;
													high_pointer->prop = flags::arg_c;
													temp_pointer = new math_obj((int)buf, high_pointer);
													assing_pl( temp_pointer);
													high_pointer = get_pl();
													low_pointer = high_pointer;
												}
												temp_pointer = new math_obj(this);
												multiple_var = new math_obj((int)buf, temp_pointer);
												copy(multiple_var);
												delete multiple_var;
											}
											else
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
										}
									}
									s_iter = temp + 1;
								}
								else if (*temp == ')')
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
									return NULL;
								}
								else
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
									return NULL;
								}
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
								return NULL;
							}
						}
						else if (high_pointer->type == flags::vectr)
						{
							//попробовать объединить с предыдущими случаями
						}
					}
					//символ находится не в начале строки
					else
					{
						if (temp == NULL)
						{
							name_str.assign(s_iter, endPtr + 1);
							s_iter = endPtr + 1;
						}
						else
						{
							name_str.assign(s_iter, temp);
							s_iter = temp;
						}
						low_pointer = point_up->left->find_math_obj(&name_str);
						//далее всегда может быть только два варианта - текщуий элемент либо функция(вектор-функция), либо выражение (векторное), причём всегда flags::undef.
						//если не найден ни один элемент массива с таким именем
						if (low_pointer == NULL)
						{
							if ((temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{
								if (get_type() == flags::funct)
								{
									//если текущий элемент - функция, то очевидно найдена новая переменная в записи выражения. 
									if (get_pc()->point_left != NULL)
									{
										//значит список переменных замкнут => новая переменная - лишняя
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									temp_pointer = new math_obj(name_str, flags::varbl, flags::defnd, 0);
									point_up->push_left(new math_obj(temp_pointer));
									var_list_push_back(temp_pointer);
									//поскольку символ не в начале строки - значит стоит после какой-либо операции. 
									high_pointer->point_right = temp_pointer;
									low_pointer = high_pointer->point_right;
									temp_pointer = NULL;
								}
								else if (get_type() == flags::exprs)
								{
									temp_pointer = new math_obj(name_str, flags::varbl, flags::defnd, flags::nthng, 0, NULL, NULL, NULL);
									point_up->push_left(new math_obj(temp_pointer));
									temp_pointer->point_collar = get_this();
									convert_to(flags::funct, flags::undef, 1, temp_pointer);
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
						else if (low_pointer->type == flags::varbl)
						{
							if ((temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{
								if (get_type() == flags::funct)
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
								else if ((type == flags::exprs) || ((type == flags::vectr) && (prop == flags::exprs)))
								{
									temp_pointer = new math_obj(low_pointer);
									convert_to(flags::funct, flags::undef, 1, temp_pointer);
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
						else if (low_pointer->type == flags::cnst)
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
						else if (low_pointer->type == flags::funct)
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
								//s_iter указывает на открывающую скобку, temp - на закрывающую
								temp_str.assign(s_iter, temp + 1);//проверить как формируется строка temp_str. здесь необходимо, чтобы сами скобки тоже были в строке.
								temp_pointer = new math_obj(&temp_str[0], &temp_str[temp_str.length() - 1], NULL, point_up);
								if (temp_pointer->prop == flags::error)
								{
									delete temp_pointer;
									return NULL;
								}
								if ((temp_pointer->type == flags::numbr) && (temp_pointer->var == 0))
								{
									if ((*(temp + 1) == NULL) || (*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
										multiple_var = NULL;
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
										return NULL;
									}
								}
								else if (temp_pointer->type == flags::vectr)
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
								else if (temp_pointer->type == flags::varbl)
								{
									if (low_pointer->var != 1)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									if (temp_pointer->prop == flags::undef)
									{
										temp_pointer->prop = flags::defnd;
										point_up->push_left(new math_obj(temp_pointer));
									}
									multiple_var = new math_obj((size_t)0);
									multiple_var->vector_push_back(temp_pointer);
									multiple_var->prop = flags::only_arg_v;
								}
								else if ((temp_pointer->type == flags::cnst) || ((temp_pointer->type == flags::funct) && (temp_pointer->prop == flags::arg_c)) || (temp_pointer->type == flags::exprs))
								{
									multiple_var = new math_obj((size_t)0);
									multiple_var->vector_push_back(temp_pointer);
									multiple_var->prop = flags::arg_c;
								}
								else if (temp_pointer->type == flags::funct)
								{
									if (temp_pointer->actn == flags::write)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									multiple_var = new math_obj((size_t)0);
									multiple_var->vector_push_back(temp_pointer);
									multiple_var->prop = flags::arg_v;
								}
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
							s_iter = temp + 1;
							//если все аргументы полученные от функции либо константы, либо конст. выражения, либо функции с конст аргументами
							if ((multiple_var != NULL) && ((multiple_var->prop == flags::arg_c) || (multiple_var->prop == flags::exprs) || (multiple_var->prop == flags::cnst)))
							{
								//если количество переменных функции совпадает с числом элементов вектора И они не равны 1 И вектор - является векторным выражением - ошибка.
								if ((temp_pointer->var == multiple_var->var) && (temp_pointer->var != 1) && ((multiple_var->prop == flags::exprs) || (multiple_var->prop == flags::cnst)))
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
									return NULL;
								}
								temp_pointer = new math_obj(low_pointer);
								temp_pointer->point_right = multiple_var;
								temp_pointer->prop = flags::arg_c; //указываем что аргументы - константы
								if ((temp_pointer->var == 1) && (multiple_var->var != 1))
								{
									buf = multiple_var->var;
									if (type != flags::vectr)
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
							else if ((type == flags::funct) || ((type == flags::vectr) && (prop == flags::funct)))
							{

								//копия найденной функции
								temp_pointer = new math_obj(low_pointer);
								//если получены какие-либо аргументы
								if (multiple_var != NULL)
								{
									//если количество переменных функции совпадает с числом элементов вектора И они не равны 1 И вектор - является вектор-функцией - ошибка.
									if ((temp_pointer->var == multiple_var->var) && (temp_pointer->var != 1) && (multiple_var->prop == flags::funct))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									temp_pointer->point_right = multiple_var;
									temp_pointer->prop = flags::arg_v;
									buf = multiple_var->var;
									if ((get_pc()->point_left != NULL) && (var_list_compare(temp_pointer) != 0))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									if ((temp_pointer->var == 1) && (buf != 1))
									{
										if (type != flags::vectr)
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
									if (temp_pointer->prop == flags::fundm)
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
								low_pointer->prop = flags::servc;
								low_pointer = temp_pointer->create_var_list(low_pointer);
								delete low_pointer;
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
							else if ((type == flags::exprs) || ((type == flags::vectr) && (prop == flags::exprs)))
							{
								//копия найденной функции
								temp_pointer = new math_obj(low_pointer);
								//если получены какие-либо аргументы
								if (multiple_var != NULL)
								{
									//если количество переменных функции совпадает с числом элементов вектора И они не равны 1 И вектор - является вектор-функцией - ошибка.
									if ((temp_pointer->var == multiple_var->var) && (temp_pointer->var != 1) && (multiple_var->prop == flags::funct))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									temp_pointer->point_right = multiple_var;
									temp_pointer->prop = flags::arg_v;
									buf = multiple_var->var;
									if ((get_pc()->point_left != NULL) && (var_list_compare(temp_pointer) != 0))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									if ((temp_pointer->var == 1) && (buf != 1))
									{
										if (type != flags::vectr)
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
									if (temp_pointer->prop == flags::fundm)
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
								convert_totaly(L"", flags::funct, flags::undef, flags::nthng, low_pointer->var, get_pl(), NULL, low_pointer->point_left);
								delete low_pointer;
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
						}
						else if (low_pointer->type == flags::vectr)
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
			if ((get_pc() != NULL) && (get_type() == flags::funct))
			{
				get_pc()->point_left = vector_back_processing(get_pc());
				get_pc()->point_left->point_right = get_pc();
			}
		}

		/*Метод ищет в заданной строке начиная с места brackets_open соответствующую закрывающую скобку.
		Метод возвращает указатель на нужное место в строке.
		NULL в случае ошибки.*/
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

		/*PRIVATE.Метод создаёт мат. объект - мат. операцию, в соответствии с op_type, и записывает на нужное место в дереве high, с учётом low.
		Метод возвращает указатель на созданную операцию.*/
		math_obj *math_obj::operations(math_obj *high, math_obj *low, flags op_type)
		{
			if ((high == NULL) && (low == NULL))
			{
				if (op_type == flags::minus)
				{
					point_left = new math_obj(op_type, 0, new math_obj(L"", flags::numbr, flags::real, 0), NULL, this);
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
		int math_obj::priority(flags *op_type)
		{
			if ((*op_type == flags::addit) || (*op_type == flags::minus))
				return 1;
			else if (*op_type == flags::mltpl)
				return 2;
			else if (*op_type == flags::divis)
				return 3;
			else if (*op_type == flags::power)
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

		bool math_obj::math_simplify()
		{
			math_obj* res;
			if ((type == flags::exprs) || (type == flags::cnst))
			{
				if (point_left == NULL)//если нечего вычислять считаем, что результат уже есть
					return true;
				res = point_left->math_simplify_processing(NULL);
				if ((res == NULL) || (res->type != flags::numbr))
					return false;
				if (res == point_left)
				{
					prop = res->prop;//константа или выражение принимает свойство посчитанного числа 
					actn = flags::nthng;
					var = res->var;
					return true;
				}
				else return false;

			}
			else if (type == flags::funct)
			{
				if (point_left == NULL)//если нечего вычислять считаем, что результат уже есть
					return true;

				vector<math_obj*>* last = new vector<math_obj*>;
				last->push_back(this);
				res = point_left->math_simplify_processing(last);
				if ((res == NULL) || (res->type != flags::numbr))
					return false;
				if (prop == flags::arg_c)
				{
					point_left = res;
				}
				else // функция не имеет константных аргументов
				{

				}
				actn = flags::nthng;
				last->pop_back();
				delete last;
				return true;

			}
			else if (type == flags::equat)
			{
				return false;
				//пока ничего
			}
			else if (type == flags::vectr)
			{
				return false;
				//res = point_left->math_simplify_processing();
			}
			else if (type == flags::matrx)
			{
				return false;
				//res = point_left->math_simplify_processing();
			}
			return false;
		}

		double math_obj::wstr_to_double(wchar_t * str, wchar_t ** remap)
		{
			//числа без знака			
			if (str == NULL) {
				//error
				//throw (-1);
				return 0.0;
			}
			wchar_t * check = wcspbrk(str, L".0123456789");//TODO:Проверить будет ли работать для str==NULL 
			if ((check == NULL)||(check !=str))	{
				//error - подумать над реализацией.
				//throw (-1);
				return 0.0;
			}
			wchar_t* decimal = wcschr(check, L'.');
			
			//wchar_t* iter;
			double buf = 0.0;
			while (check){
				if (check == decimal) { buf += 0; check++; }
				else if ((decimal == NULL)||(check < decimal)){
                    if(*check>='0' && *check<='9'){//лучше указывать сами символы, так как их код может отличатся в зависимости от кодировки
                         buf = buf * 10 + (*check -'0');
                         check++;
                    }else{
                        *remap = check;
                        check = NULL;
                    }                   
				}
				else if (check > decimal) {
                    if (*check >= '0' && *check <= '9') {
                         buf = buf + (*check-'0') * pow(10, (int)(decimal - check));
                         check++;
                    }else{
                        *remap = check;
                        check = NULL;
                    }                   
				}				
			}
			return buf;
		}

		math_obj * math_obj::math_simplify_processing(vector<math_obj*>* last_funct)
		{
			switch (type)
			{
			case Project::Core::flags::error:
			{
				ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTORS_LIMITED);
				return NULL;
				break;
			}
			case Project::Core::flags::nthng:
			{
				ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTORS_LIMITED);
				return NULL;
				break;
			}
			case Project::Core::flags::matrx:
			{
				ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTORS_LIMITED);
				return NULL;
				break;
			}
			case Project::Core::flags::vectr:
			{
				ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTORS_LIMITED);
				return NULL;
				break;
			}
			case Project::Core::flags::exprs://для выражений предполагается отсутствие каких-либо переменных, всегда константные результаты вычислений
			{
				if (point_left == NULL)
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
					return NULL;
				}
				else if (point_left->type == flags::numbr)
				{
					var = point_left->var;
					prop = point_left->prop;
					type = flags::numbr;
					delete point_left; point_left = NULL;
					return this;
				}
				math_obj*out = point_left->math_simplify_processing(NULL);
				if ((point_left == out) && ((out->type == flags::numbr) || (out->type == flags::cnst)))
				{
					var = point_left->var;
					prop = point_left->prop;
					type = flags::numbr;
					delete point_left; point_left = NULL;
					return this;
				}
				else return NULL;
				break;
			}
			case Project::Core::flags::equat:
			{
				ProjectError::SetProjectLastError(ProjectError::ErrorCode::VECTORS_LIMITED);
				//пока оставлю это условие здесь, хотя в рекурсии оно не должно попадаться
				return NULL;
				break;
			}
			case Project::Core::flags::funct:
			{
				if (point_left == NULL)
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
					return NULL;
				}
				math_obj* out = NULL;
				if (last_funct == NULL)
				{
					last_funct = new vector<math_obj*>;
				}
				last_funct->push_back(this);//добавляется новый указатель к списку функций в дереве операций
				out = point_left->math_simplify_processing(last_funct);
				if (out == NULL)
					return NULL;
				if ((prop == flags::arg_c) && (out->type == flags::numbr))
				{
					type = flags::numbr;
					prop = out->prop;
					var = out->var;
					point_left = NULL;
					point_collar = NULL;
					//point_right->vector_destruct();//очистка аргументов функции
					//delete point_right;
					point_right = NULL;
					delete out;
				}
				//undef, arg_v, defnd		

				last_funct->pop_back();//последний указатель удаляется
				if (last_funct->size() == 0)
					delete last_funct;
				if ((prop == flags::arg_v) || (prop == flags::undef) || (prop == flags::defnd))
					return out;
				return this;
				break;
			}
			case Project::Core::flags::varbl:
			{
				if (last_funct == NULL)//переменная за пределами функции
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
					return NULL;
				}
				math_obj* out = NULL;
				math_obj* last = last_funct->back();//указатель на последнюю функцию
				math_obj* temp_var = this;//указатель на переменную в списке переменных
				math_obj* iter = NULL;//указатель на соответствующий аргумент				
				int size = last_funct->size() - 1;
				//пока в векторе есть функции и они аргументированы
				while ((size >= 0) && ((last->prop == flags::arg_c) || (last->prop == flags::arg_v)))
				{
					iter = last->point_right->vector_at((int)temp_var->var);
					if (iter == NULL)//если среди аргументов функции нет нужного номера
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
						return NULL;
					}
					if (iter->type == flags::varbl)//переменная среди аргументов функции должна указывать на переменную более верхней функции
					{
						if (iter->point_left == NULL)//|| (iter->point_left->point_collar->name.compare(last->name) != 0))
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
							return NULL;
						}
						if (size != 0)
						{
							size--;
							last = last_funct->at(size);
							temp_var = iter->point_left;
						}
						else//size == 0
						{
							size--;
							//значит переменная участвует в выражении для undef или defnd функций
						}
					}
					else //if ((iter->type == flags::funct)|| (iter->type == flags::exprs)|| (iter->type == flags::cnst)|| (iter->type == flags::numbr))
					{
						out = iter->math_simplify_processing(last_funct);
						size = -1;
					}
				}
				if (out == NULL)//если не найден аргумент для подстановки (переменная указывает на функцию без аргументов)
				{
					return new math_obj(flags::numbr, flags::real, 0, NULL);
				}
				//для выражений, констант, чисел и функций с конст аргументами
				else if ((out == iter) || (out->type == flags::numbr))
				{
					//out берётся из вектора аргументов. Чтобы не использовать сам аргуемнт в качестве буфера - лучше его скопировать
					return new math_obj(out);
				}
				//для функций с переменными аргументами и без аргументов
				else if (out != iter)
				{
					return new math_obj(flags::numbr, flags::real, 0, NULL);
				}

				break;
			}
			case Project::Core::flags::cnst://для константы её значение/значения будут находится по указателю point_left
			{
				//TODO: для типов чисел отличных от real должна быть доп. проверка. так же рассмотреть константу i	
				math_obj* out;
				if (last_funct != NULL)//если идёт проход по дереву операций для какой-либо функции - его менять каким-либо образом нельзя
				{
					if ((point_left == NULL) && (prop == flags::fundm))
						return this;
					else if (point_left == NULL)
					{
						var = 0;
						return this;
					}
					else if (point_left->type == flags::numbr)//если константа уже посчитана
					{
						var = point_left->var;
						prop = point_left->prop;
						return this;
					}
					out = point_left->math_simplify_processing(last_funct);
					if ((point_left == out) && (out->type == flags::numbr))
					{
						var = out->var;
						prop = out->prop;
						return this;
					}
					else return NULL;
				}
				else//если идёт проход по дереву операций для константного выражения
				{
					if (point_left == NULL)//конст превращается в число
					{
						type = flags::numbr;
						if (prop != flags::fundm)
						{
							var = 0;
						}
						prop = flags::real;
						return this;
					}
					else if (point_left->type == flags::numbr)
					{
						var = point_left->var;
						prop = point_left->prop;
						type = flags::numbr;
						return this;
					}
					out = point_left->math_simplify_processing(last_funct);
					if ((point_left == out) && (out->type == flags::numbr))
					{
						var = out->var;
						prop = out->prop;
						type = flags::numbr;
						return this;
					}
					else return NULL;
				}
				break;
			}
			case Project::Core::flags::numbr:
			{
				return this;
				break;
			}

			case Project::Core::flags::addit:
			{
				if ((point_left == NULL) || (point_right == NULL))
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
					return NULL;
				}
				math_obj* left = point_left->math_simplify_processing(last_funct);
				math_obj* right = point_right->math_simplify_processing(last_funct);
				if ((left == NULL) || (right == NULL))
					return NULL;
				if (last_funct != NULL)
				{
					if ((point_left == left) && (point_right == right)) //если при вычислении справа и слева находятся константные элементы
					{
						var = left->var + right->var;//простое сложение
						prop = left->prop;//много проверок на сложение двух чисел
						type = flags::numbr;
						//далее дерево операций упрощается
						delete point_left; point_left = NULL;
						delete point_right; point_right = NULL;
						return this;
					}
					else if ((point_left != left) && (point_right != right))
					{
						left->var += right->var;//простое сложение						
						delete right;
						return left;
					}
					else if (point_left != left)
					{
						left->var += right->var;//простое сложение						
						return left;
					}
					else if (point_right != right)
					{
						right->var += left->var;//простое сложение						
						return right;
					}
				}
				else
				{
					if ((point_left != left) || (point_right != right))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
						return NULL;
					}
					var = left->var + right->var;//простое сложение
					prop = left->prop;//много проверок на сложение двух чисел
					type = flags::numbr;
					//далее дерево операций упрощается
					delete point_left; point_left = NULL;
					delete point_right; point_right = NULL;
					return this;
				}
				break;
			}
			case Project::Core::flags::minus:
			{
				if ((point_left == NULL) || (point_right == NULL))
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
					return NULL;
				}
				math_obj* left = point_left->math_simplify_processing(last_funct);
				math_obj* right = point_right->math_simplify_processing(last_funct);
				if ((left == NULL) || (right == NULL))
					return NULL;
				if (last_funct != NULL)
				{
					if ((point_left == left) && (point_right == right)) //если при вычислении справа и слева находятся константные элементы
					{
						var = left->var - right->var;//простое вычитание
						prop = left->prop;//много проверок на сложение двух чисел
						type = flags::numbr;
						//далее дерево операций упрощается
						delete point_left; point_left = NULL;
						delete point_right; point_right = NULL;
						return this;
					}
					else if ((point_left != left) && (point_right != right))
					{
						left->var -= right->var;//простое вычитание
						delete right;
						return left;
					}
					else if (point_left != left)
					{
						left->var -= right->var;//простое вычитание						
						return left;
					}
					else if (point_right != right)
					{
						right->var = left->var - right->var;//простое вычитание						
						return right;
					}
				}
				else
				{
					if ((point_left != left) || (point_right != right))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
						return NULL;
					}
					var = left->var - right->var;//простое вычитание
					prop = left->prop;//много проверок на сложение двух чисел
					type = flags::numbr;
					//далее дерево операций упрощается
					delete point_left; point_left = NULL;
					delete point_right; point_right = NULL;
					return this;
				}
				break;
			}
			case Project::Core::flags::mltpl:
			{
				if ((point_left == NULL) || (point_right == NULL))
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
					return NULL;
				}
				math_obj* left = point_left->math_simplify_processing(last_funct);
				math_obj* right = point_right->math_simplify_processing(last_funct);
				if ((left == NULL) || (right == NULL))
					return NULL;
				if (last_funct != NULL)
				{
					if ((point_left == left) && (point_right == right)) //если при вычислении справа и слева находятся константные элементы
					{
						var = left->var * right->var;//простое умножение
						prop = left->prop;//много проверок на умножение двух чисел
						type = flags::numbr;
						//далее дерево операций упрощается
						delete point_left; point_left = NULL;
						delete point_right; point_right = NULL;
						return this;
					}
					else if ((point_left != left) && (point_right != right))
					{
						left->var *= right->var;//простое умножение
						delete right;
						return left;
					}
					else if (point_left != left)
					{
						left->var *= right->var;//простое умножение						
						return left;
					}
					else if (point_right != right)
					{
						right->var = left->var * right->var;//простое умножение						
						return right;
					}
				}
				else
				{
					if ((point_left != left) || (point_right != right))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
						return NULL;
					}
					var = left->var * right->var;//простое умножение
					prop = left->prop;//много проверок на умножение двух чисел
					type = flags::numbr;
					//далее дерево операций упрощается
					delete point_left; point_left = NULL;
					delete point_right; point_right = NULL;
					return this;
				}
				break;
			}
			case Project::Core::flags::divis:
			{
				if ((point_left == NULL) || (point_right == NULL))
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
					return NULL;
				}
				math_obj* left = point_left->math_simplify_processing(last_funct);
				math_obj* right = point_right->math_simplify_processing(last_funct);
				if ((left == NULL) || (right == NULL))
					return NULL;
				if (last_funct != NULL)
				{
					if ((point_left == left) && (point_right == right)) //если при вычислении справа и слева находятся константные элементы
					{
						var = left->var / right->var;//простое деление
						prop = left->prop;//много проверок на деление двух чисел
						type = flags::numbr;
						//далее дерево операций упрощается
						delete point_left; point_left = NULL;
						delete point_right; point_right = NULL;
						return this;
					}
					else if ((point_left != left) && (point_right != right))
					{
						left->var /= right->var;//простое деление
						delete right;
						return left;
					}
					else if (point_left != left)
					{
						left->var /= right->var;//простое деление						
						return left;
					}
					else if (point_right != right)
					{
						right->var = left->var / right->var;//простое деление						
						return right;
					}
				}
				else
				{
					if ((point_left != left) || (point_right != right))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
						return NULL;
					}
					var = left->var / right->var;//простое деление
					prop = left->prop;//много проверок на деление двух чисел
					type = flags::numbr;
					//далее дерево операций упрощается
					delete point_left; point_left = NULL;
					delete point_right; point_right = NULL;
					return this;
				}
				break;
			}
			case Project::Core::flags::power:
			{
				if ((point_left == NULL) || (point_right == NULL))
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
					return NULL;
				}
				math_obj* left = point_left->math_simplify_processing(last_funct);
				math_obj* right = point_right->math_simplify_processing(last_funct);
				if ((left == NULL) || (right == NULL))
					return NULL;
				if (last_funct != NULL)
				{
					if ((point_left == left) && (point_right == right)) //если при вычислении справа и слева находятся константные элементы
					{
						var = pow(left->var, right->var);//простое возведение в степень
						prop = left->prop;//много проверок на возведение в степень двух чисел
						type = flags::numbr;
						//далее дерево операций упрощается
						delete point_left; point_left = NULL;
						delete point_right; point_right = NULL;
						return this;
					}
					else if ((point_left != left) && (point_right != right))
					{
						left->var = pow(left->var, right->var);//простое возведение в степень
						delete right;
						return left;
					}
					else if (point_left != left)
					{
						left->var = pow(left->var, right->var);//простое возведение в степень						
						return left;
					}
					else if (point_right != right)
					{
						right->var = pow(left->var, right->var);//простое возведение в степень						
						return right;
					}
				}
				else
				{
					if ((point_left != left) || (point_right != right))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
						return NULL;
					}
					var = pow(left->var, right->var);//простое возведение в степень
					prop = left->prop;//много проверок на возведение в степень двух чисел
					type = flags::numbr;
					//далее дерево операций упрощается
					delete point_left; point_left = NULL;
					delete point_right; point_right = NULL;
					return this;
				}
				break;
			}
			default:
			{
				ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
				return NULL;
				break;
			}

			}
			return nullptr;
		}

		wstring math_obj::expression()
		{
			return wstring();
		}

		wstring math_obj::expression_processing()
		{
			return wstring();
		}

		wstring math_obj::vector_to_string()
		{
			if (type != flags::vectr)
				return wstring();
			//TODO:переделать в рекурсию. Это временное решение
			math_obj* iter;
			wstring out = L"(";
			for (int count = 0; count < var; count++)
			{
				iter = vector_at(count);
				if (iter->type == flags::varbl)
				{
					out += iter->name;
				}
				if (count < var - 1)
					out += L",";
			}
			out += L")";
			return out;
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
				var_list->prop = flags::servc;
			}
			if (type == flags::funct)
			{
				if (prop == flags::arg_v)
				{
					for (int count = 0; count < (int)var; count++)
					{
						iter = point_right->vector_at(count);
						if (iter->type == flags::varbl)
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
						else if (iter->type == flags::funct)
						{
							if (iter->prop == flags::arg_v)
							{
								var_list = iter->create_var_list(var_list);
							}
							else if ((iter->prop == flags::undef) || (iter->prop == flags::defnd))
							{
								place = new math_obj((size_t)0);
								var_list_copy_to_vector_processing(iter->point_collar, place);
								iter->prop = flags::arg_v;
								if (iter->prop == flags::undef)
								{
									reassing_left_pointers(iter->point_collar);
									iter->close_list();
								}
								place->prop = flags::only_arg_v;
								iter->point_right = place;
								var_list_copy_to_vector_processing(place->point_left, var_list);

							}
						}
					}
				}
				else if ((prop == flags::undef) || (prop == flags::defnd))
				{
					place = new math_obj((size_t)0);
					var_list_copy_to_vector_processing(point_collar, place);
					if (prop == flags::undef)
					{
						reassing_left_pointers(point_collar);
						close_list();
					}
					prop = flags::arg_v;
					place->prop = flags::only_arg_v;
					point_right = place;
					var_list_copy_to_vector_processing(point_right->point_left, var_list);
				}
			}
			else if (type == flags::vectr)
			{
				for (int count = 0; count < (int)var; count++)
				{
					iter = vector_at(count);
					if (iter->type == flags::varbl)
					{
						place = var_list->find_by_name(iter);
						if (place == NULL)
						{
							place = new math_obj(iter);
							vector_assing_at(new math_obj(L" ", flags::varbl, flags::servc, flags::nthng, -1, place, NULL, NULL), count);
							var_list->vector_push_back(place);
						}
						else
						{
							vector_assing_at(new math_obj(L" ", flags::varbl, flags::servc, flags::nthng, -1, place, NULL, NULL), count);
						}
					}
					else if (iter->type == flags::funct)
					{
						if (buf == iter)
							break;
						if (buf == NULL)
							buf = iter;
						if (iter->prop == flags::arg_v)
						{
							var_list = iter->create_var_list(var_list);
						}
						else if ((iter->prop == flags::undef) || (iter->prop == flags::defnd))
						{
							place = new math_obj((size_t)0);
							iter->var_list_copy_to_vector(place);
							if (iter->prop == flags::undef)
							{
								reassing_left_pointers(iter->point_collar);
								iter->close_list();
							}
							iter->prop = flags::arg_v;
							place->prop = flags::only_arg_v;
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
			if ((type == flags::vectr) && (var != 0) && (prop == flags::funct) && (point_left->point_left->point_collar != NULL) && (point_left->point_left->type == flags::funct))
			{
				var_list_collar(point_left->point_left->point_collar, point_left->point_left);
			}
			//текущий элемет - что угодно другое
			else
			{
				if ((point_collar != NULL) && (type == flags::funct))
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

		/*Метод сравнивает список аргументов (хотя бы один из которых переменная) объекта pointer (функции, вектора или матрицы) со списком переменных данной функции.
		Метод возвращает:
		0 - список переменных pointer полностью входит в список переменных данной функции.
		любое другое положительное целое - число различий списков функций.
		-1 - ошибка - неверный тип оьъекта
		*/
		int math_obj::var_list_compare(math_obj * pointer)
		{
			if ((type == flags::vectr) && (var != 0) && (prop == flags::funct))
				return point_left->point_left->var_list_compare_processing(pointer);
			else if (type == flags::funct)
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
			//TODO:дописать возможность проверки для функций типа flags::arg_v. хотя можено и не дописывать
			int count_var = 0;
			int temp = 0;
			math_obj * place;
			math_obj *buf = NULL;
			//рассматриваются функции с аргументами (определённые и нет)
			if (pointer->get_prop() == flags::arg_v)
			{
				for (int count = 0; count < pointer->get_var(); count++)
				{
					place = pointer->get_pr()->vector_at(count);
					//если среди аргументов попалась переменная
					if (place->type == flags::varbl)
					{
						//если эта переменная не в списке данной функции
						if (find_by_name_processing(get_pc(), place) == NULL)
							count_var++;
					}
					//если функция
					else if (place->type == flags::funct)
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
			if (pointer == NULL)
				return NULL;
			if (type == flags::vectr)
			{
				if (prop == flags::funct)
				{
					return find_by_name_processing(point_left->point_left->point_collar, pointer);
				}
				else
					return find_by_name_processing(point_left, pointer);
			}
			else if (type == flags::funct)
				return find_by_name_processing(point_collar, pointer);
			else
				return NULL;
		}
		/*PRIVATE. Рекурсия для find_by_name*/
		math_obj* math_obj::find_by_name_processing(math_obj*pointer, math_obj *original)
		{
			if ((pointer == NULL)||(original==NULL))
				return NULL;
			if (original->name.compare(pointer->name) == 0)
				return pointer;
			else if ((pointer->point_right != NULL) && ((pointer->point_right->var != 0) || (pointer->point_right->type != flags::varbl)))
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

			if ((type == flags::vectr) && (var != 0) && (prop == flags::funct) && (point_left->point_left->point_collar != NULL))
			{
				back = vector_back_processing(point_left->point_left->point_collar);
			}
			else if ((type == flags::funct) && (point_collar != NULL))
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
			if (get_type() == flags::funct)//вектор-функции.
				return var_list_copy_to_vector_processing(get_pc(), vector);
			else if (type == flags::vectr)//остальные векторы. Можно дополнительно отсеивать flags::exprs и flags::cnst, только этот метод ля них не вызывается
				return var_list_copy_to_vector_processing(point_left, vector);
			else
				return -1;
		}
		/*PRIVATE. Метод рекурсивно копирует список переменных в вектор.
		-1 при ошибке*/
		int math_obj::var_list_copy_to_vector_processing(math_obj* pointer, math_obj* vect)
		{
			if (vect->prop == flags::servc)
			{
				math_obj* temp = find_by_name_processing(vect->point_left, pointer);
				if (temp == NULL)
				{
					vect->vector_push_back(new math_obj(pointer));
					vect->vector_back()->point_left = NULL;
					pointer->point_left = vect->vector_back();
				}
				else
				{
					pointer->point_left = temp;
				}
			}
			else
				vect->vector_push_back(new math_obj(pointer));

			if ((pointer->point_right != NULL) && ((pointer->point_right->var != 0) || (pointer->point_right->type != flags::varbl)))
			{
				return var_list_copy_to_vector_processing(pointer->point_right, vect);
			}
			else
				return 0;
		}
		/*Метод преобразует векторное выражение или функцию в скалярное.
		NULL в случае ошибки*/
		math_obj * math_obj::unvectorize()
		{
			if ((type == flags::vectr) && ((prop == flags::exprs) || (prop == flags::funct) || (prop == flags::equat)) && (point_left != NULL))
			{
				if ((point_left->point_left != NULL) && (point_left->point_right != NULL) && (point_left->point_left != point_left->point_right->point_left))
				{
					//ошибка - несколько различных указателей вектора не указывают на один и тот же объект
					return NULL;
				}
				math_obj * temp = point_left->point_left;
				vector_destruct();
				copy(temp);
				delete temp;
				//если функция с незамкнутым скпископ переменных - настраиваем так же указатели переменных
				if ((type == flags::funct) && (point_collar->point_left == NULL))
					link_var_list_to_funct();
			}
			else if ((type == flags::vectr) && (prop == flags::cnst) && (point_left != NULL))
			{
				type = flags::cnst;
				prop = flags::undef;
				var = 0;
				vector_destruct();
			}
			return this;
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
			type = flags::nthng;
			prop = flags::nthng;
			actn = flags::nthng;
			var = 0;
			point_left = NULL;
			point_right = NULL;
			point_collar = NULL;
			point_up = NULL;
		}
		//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double) 
		math_obj::math_obj(wstring _name, double _num)
		{
			name = _name;
			type = flags::nthng;
			prop = flags::nthng;
			actn = flags::nthng;
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = NULL;
			point_up = NULL;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double) 
		math_obj::math_obj(wstring _name, flags _type, flags _prop, double _num)
		{
			name = _name;
			type = _type;
			prop = _prop;
			actn = flags::nthng;
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = NULL;
			point_up = NULL;
		}
		//Конструктор записывает (ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(flags _type, flags _prop, double _num, math_obj *_pc)
		{
			name = L"";
			type = _type;
			prop = _prop;
			actn = flags::nthng;
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = _pc;
			point_up = NULL;
		}
		//Конструктор записывает (ТИП объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(flags _type, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
			name = L"";
			type = _type;
			prop = flags::nthng;
			actn = flags::nthng;
			var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = _pc;
			point_up = NULL;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _name, flags _type, flags _prop, flags _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
			name = _name;
			type = _type;
			prop = _prop;
			actn = _actn;
			var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = _pc;
			point_up = NULL;
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
			point_up = var1->point_up;
		}
		//Деструктор
		math_obj::~math_obj()
		{

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
			//point_up не копируется
		}
		//Метод get. ТЕКУЩИЙ ЭЛЕМЕНТ
		math_obj* math_obj::get_this()
		{
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
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
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
			{
				point_left->point_left->name = _name;
			}
			name = _name;
		}
		//Метод get. ТИП
		flags math_obj::get_type()
		{
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
			{
				return point_left->point_left->type;
			}
			else if ((type == flags::vectr) && (var != 0) && (prop == flags::cnst))
			{
				return prop;
			}
			//текущий элемет - что угодно другое
			else
			{
				return type;
			}
		}
		//Метод assing. ТИП
		void math_obj::assing_type(flags _type)
		{
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
			{
				point_left->point_left->type = _type;
				prop = _type;
			}
			else if ((type == flags::vectr) && (var != 0) && (prop == flags::cnst))
			{
				//если вектор имеет тип константы, менять его тип таким образом бессмысленно.				
			}
			//текущий элемет - что угодно другое
			else
			{
				type = _type;
			}
		}
		//Метод get. СВОЙСТВО
		flags math_obj::get_prop()
		{
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
			{
				return point_left->point_left->prop;
			}
			else if ((type == flags::vectr) && (var != 0) && (prop == flags::cnst))
			{
				//TODO:тут нужно выдавать свойство константных элементов вектора. Могут быть flags::real, cmplx и т.д.
				return prop;
			}
			//текущий элемет - что угодно другое
			else
			{
				return prop;
			}
		}
		//Метод assing. СВОЙСТВО
		void math_obj::assing_prop(flags _prop)
		{
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
			{
				point_left->point_left->prop = _prop;
			}
			else if ((type == flags::vectr) && (var != 0) && (prop == flags::cnst))
			{
				//если вектор имеет тип константы, менять его свойство таким образом бессмысленно.				
			}
			//текущий элемет - что угодно другое
			else
			{
				prop = _prop;
			}
		}
		//Метод get. ДЕЙСТВИЕ
		flags math_obj::get_actn()
		{
			//нет никакой необходимости разделять пометку действия над векторными объектами и содержимым этих объектов
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
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
		void math_obj::assing_actn(flags _acnt)
		{
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
			{
				point_left->point_left->actn = _acnt;
			}
			actn = _acnt;
		}
		//Метод get. ЧИСЛО
		double math_obj::get_var()
		{
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
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
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
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
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
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
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
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
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
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
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
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
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
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
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat)))
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
		void math_obj::convert_to(flags _type, flags _prop, double _num, math_obj *_pc)
		{
			//текущий элемент ненулевой вектор И у него есть одно из этих свойств.
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat) || (prop == flags::cnst)))
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
		void math_obj::convert_totaly(wstring _name, flags _type, flags _prop, flags _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc)
		{
			if ((type == flags::vectr) && (var != 0) && ((prop == flags::funct) || (prop == flags::exprs) || (prop == flags::equat) || (prop == flags::cnst)))
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
			type = flags::vectr;
			prop = flags::nthng;
			actn = flags::nthng;
			var = size_n;
			point_left = NULL;
			point_right = NULL;
			point_collar = NULL;
			point_up = NULL;
			math_obj *temp = vector_create(size_n, NULL, NULL);
		}
		//Конструктор вектора длины size_n по строке символов типа wchar_t, имеющей начало begin и конец end.
		math_obj::math_obj(size_t size_n, wchar_t*begin, wchar_t*end, data_list* data)
		{
			name = L"";
			type = flags::vectr;
			prop = flags::nthng;
			actn = flags::nthng;
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
					if (temp_pointer->prop == flags::error)
					{
						delete temp_pointer;
						return NULL;
					}
					//заполняем массив аргументов
					if (temp_pointer->type == flags::vectr)
					{
						//пока ошибка. Надо расмотреть векторы с векторами внутри. Скорее всего в этом случае необходимо требовать чтобы все элементы вектора были векторами равной длины, возможно переопределение вектора в матрицу.
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::EQUALY_MISSING);
						return NULL;
					}
					else if (temp_pointer->type == flags::varbl)
					{
						if (temp_pointer->prop == flags::undef)
						{
							temp_pointer->prop = flags::defnd;
							point_up->push_left(new math_obj(temp_pointer));
						}
						//если в вектор добавляются переменные - они нумеруются по порядку. Это необходимо в случае задания списка переменных, а так же для разрешения возможных неопределённостей при проходе по вектору определённых методов.
						temp_pointer->var = count_var;
						count_var++;
					}
					else if (temp_pointer->type == flags::funct)
					{
						if (temp_pointer->actn == flags::write)
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
							return NULL;
						}
						//тут в качестве аргумента получается переменная функция, которая либо  flags::defnd, либо  flags::undef
						if (temp_pointer->prop != flags::arg_c)
						{
							prop = flags::arg_v;
						}
						else //тут вегда подразумевается  flags::defnd функция с конст аргументами										
						{

						}
					}
					vector_push_back(temp_pointer);
					begin = temp;
				}
				if (count_var == size_n)
					prop = flags::only_arg_v;
				else if (prop != flags::arg_v)
				{
					if (count_var == 0)
						prop = flags::arg_c;
					else
						prop = flags::arg_v;
				}
				var = size_n;
			}
			return this;
		}

		/*Конструктор вектора служебных переменных, указывающих на один и тот же объект obj. Полагается что вектор унаследует имя объекта obj.
		*/
		math_obj::math_obj(size_t size_n, math_obj * obj)
		{
			name = obj->name;
			type = flags::vectr;
			prop = obj->type;
			actn = flags::nthng;
			var = size_n;
			point_left = convert_to_vector(size_n, obj);
			point_right = NULL;
			point_collar = NULL;
			point_up = NULL;
		}

		/*Деструктор вектора*/
		int math_obj::vector_destruct()
		{
			int s = 0;
			int flag;
			if ((type == flags::vectr) && ((prop == flags::exprs) || (prop == flags::funct) || (prop == flags::equat)) && (point_left != NULL))
			{
				flag = 0;
				s += vector_destruct_processing(point_left, &flag);
			}
			else if ((type == flags::vectr) && (point_left != NULL))
			{
				flag = 1;
				s += vector_destruct_processing(point_left, &flag);
			}
			else
				return -1;
			return s;
		}

		int math_obj::vector_destruct_processing(math_obj * pointer, int *flag)
		{
			int s = 0;//придумать что-нибудь другое
			if ((pointer->type == flags::varbl) && (pointer->prop == flags::servc))
			{
				if (*flag)
				{
					//s += tree_dest(pointer->point_left);
				}
				else
				{
					//ничего не делать. пока
				}
			}
			else
			{
				//s += tree_dest(pointer);
			}
			if (pointer->point_right != NULL)
			{
				s += vector_destruct_processing(pointer->point_right, flag);
			}
			delete pointer;
			return s;
		}

		/*PRIVATE. Метод рекурсивно создаёт связный список указанной длинны size из служебных переменных, указывающих на один и тот же объект.*/
		math_obj* math_obj::convert_to_vector(int size, math_obj*obj)
		{
			math_obj* temp = NULL;
			if (size > 0)
			{
				temp = new math_obj(L"", flags::varbl, flags::servc, flags::nthng, -1, obj, NULL, NULL);
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
					if ((pointer->type == flags::funct) || (pointer->type == flags::exprs) || (pointer->type == flags::numbr))
					{
						point_left = new math_obj(L"", flags::varbl, flags::servc, flags::nthng, -1, pointer, place->point_right, place->point_collar);
					}
					else if ((pointer->type == flags::cnst) || (pointer->type == flags::vectr))
					{
						point_left = pointer;
						point_left->point_right = place->point_right;
						point_left->point_collar = place->point_collar;
					}
					else if (pointer->type == flags::varbl)
					{
						point_left = pointer;
						point_left->point_right = place->point_right;
						point_left->point_collar = place->point_collar;
						point_left->var = 0;
					}
				}
				else
				{
					math_obj*place_minus_1 = vector_at(index - 1);
					place = place_minus_1->point_right;
					if ((pointer->type == flags::funct) || (pointer->type == flags::exprs) || (pointer->type == flags::numbr))
					{
						place_minus_1->point_right = new math_obj(L"", flags::varbl, flags::servc, flags::nthng, -1, pointer, place->point_right, place->point_collar);
						place_minus_1->point_collar = place_minus_1->point_right;
					}
					else if ((pointer->type == flags::varbl) || (pointer->type == flags::cnst) || (pointer->type == flags::vectr))
					{
						place_minus_1->point_right = pointer;
						place_minus_1->point_collar = place_minus_1->point_right;
						place_minus_1->point_right->point_right = place->point_right;
						place_minus_1->point_right->point_collar = place->point_collar;
					}
					else if (pointer->type == flags::varbl)
					{
						place_minus_1->point_right = pointer;
						place_minus_1->point_collar = place_minus_1->point_right;
						place_minus_1->point_right->point_right = place->point_right;
						place_minus_1->point_right->point_collar = place->point_collar;
						place_minus_1->point_right->var = index;
					}
				}
				if ((place->prop == flags::servc) && (place->type == flags::varbl))
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
			if ((type != flags::vectr) || (point_left == NULL) || (index >= vector_size()))
			{
				//выход за пределы вектора можно сделать отдельной ошибкой
				return NULL;
			}
			else
			{
				int count = 0;
				math_obj*place = vector_at_processing(point_left, &index, &count);
				if ((place->prop == flags::servc) && (place->type == flags::varbl))
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
			if (type != flags::vectr)
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
			//TODO:вылет с тремя переменными
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
			if (type != flags::vectr)
			{
				//ошибка. Ничего не делать
				return -1;
			}
			else if (point_left == NULL)
			{
				if ((pointer->type == flags::funct) || (pointer->type == flags::exprs) || (pointer->type == flags::numbr))
				{
					point_left = new math_obj(L"", flags::varbl, flags::servc, flags::nthng, -1, pointer, NULL, NULL);
				}
				else if ((pointer->type == flags::cnst) || (pointer->type == flags::vectr))
				{
					point_left = pointer;
					point_left->point_right = NULL;
					point_left->point_collar = NULL;
				}
				else if (pointer->type == flags::varbl)
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
				if ((pointer->type == flags::funct) || (pointer->type == flags::exprs) || (pointer->type == flags::numbr))
				{
					place->point_right = new math_obj(L"", flags::varbl, flags::servc, flags::nthng, -1, pointer, NULL, NULL);
					place->point_collar = place->point_right;
				}
				else if ((pointer->type == flags::cnst) || (pointer->type == flags::vectr))
				{
					place->point_right = pointer;
					place->point_collar = place->point_right;
					place->point_right->point_right = NULL;
					place->point_right->point_collar = NULL;
				}
				else if (pointer->type == flags::varbl)
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
			if (type != flags::vectr)
			{
				//ошибка. Ничего не делать 	
				return -1;
			}
			else if (point_left == NULL)
			{
				if ((pointer->type == flags::funct) || (pointer->type == flags::exprs) || (pointer->type == flags::numbr))
				{
					point_left = new math_obj(L"", flags::varbl, flags::servc, flags::nthng, -1, pointer, NULL, NULL);
				}
				else if ((pointer->type == flags::cnst) || (pointer->type == flags::vectr))
				{
					point_left = pointer;
					point_left->point_right = NULL;
					point_left->point_collar = NULL;
				}
				else if (pointer->type == flags::varbl)
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
				if ((pointer->type == flags::funct) || (pointer->type == flags::exprs) || (pointer->type == flags::numbr))
				{
					point_left = new math_obj(L"", flags::varbl, flags::servc, flags::nthng, -1, pointer, place, place);
				}
				else if ((pointer->type == flags::cnst) || (pointer->type == flags::vectr))
				{
					point_left = pointer;
					point_left->point_right = place;
					point_left->point_collar = place;
				}
				else if (pointer->type == flags::varbl)
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
			if ((point_left == NULL) || (type != flags::vectr))
				return NULL;
			else
				return vector_back_processing(point_left);
		}
		/*PRIVATE. Рекурсия для var_list_back и vector_back*/
		math_obj* math_obj::vector_back_processing(math_obj *pointer)
		{
			if ((pointer->point_right == NULL) || ((pointer->point_right->var == 0) && (pointer->point_right->type == flags::varbl)))
				return pointer;
			else
				return vector_back_processing(pointer->point_right);
		}
		/*Метод преобразует односвязный список в двусвязный.
		Двусвязный список может создаваться только для переменных с именем. Проверять отсутствие других элементов, а так же служебных переменных.*/
		void math_obj::double_lincked_vector()
		{
			if ((point_left != NULL) && (type == flags::vectr))
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
			if ((type == flags::cnst) || (type == flags::exprs))
			{
				s += tree_destruct_processing(point_left);
				point_left = NULL;
			}
			else if (type == flags::funct)
			{
				if ((prop == flags::undef) || (prop == flags::defnd))
				{
					s += tree_destruct_processing(point_collar);
					point_collar = NULL;
					s += tree_destruct_processing(point_left);
					point_left = NULL;
				}
				else if ((prop == flags::arg_c) || (prop == flags::arg_v))
				{
					//s += vector_destruct(point_right->point_collar);
				}
			}
			else if (type == flags::equat)
			{
				//доделать
			}
			else if (type == flags::vectr)
			{
				//s += vector_destruct(point_left);
			}
			return s;
		}
		/*PRIVATE. Рекурсия для tree_destruct*/
		int math_obj::tree_destruct_processing(math_obj* pointer)
		{
			if (pointer == NULL) return 0;
			int temp = 0;

			if ((pointer->type == flags::numbr) || (pointer->type == flags::cnst))
			{
				delete pointer;
				pointer = NULL;
				return 0;
			}
			else if ((pointer->type == flags::addit) || (pointer->type == flags::mltpl) || (pointer->type == flags::divis) || (pointer->type == flags::power))
			{
				temp += tree_destruct_processing(pointer->point_left);
				temp += tree_destruct_processing(pointer->point_right);
				delete pointer;
				pointer = NULL;
				return temp;

			}
			else if (pointer->type == flags::exprs)
			{
				temp += tree_destruct_processing(pointer->point_left);
				delete pointer;
				pointer = NULL;
				return temp;
			}
			else if (pointer->type == flags::funct)
			{
				if ((pointer->name == L"minus") || (pointer->name == L"root"))
				{
					//temp += vector_destruct(pointer->point_right);
				}
				else
				{
					if (pointer->prop == flags::undef)
					{
						temp += tree_destruct_processing(pointer->point_left);
						temp += tree_destruct_processing(pointer->point_collar);
						delete pointer;
						pointer = NULL;
						return temp;
					}
					else if (pointer->prop == flags::defnd)
					{
						delete pointer;
						pointer = NULL;
						return temp;
					}
					else if ((pointer->prop == flags::arg_c) || (pointer->prop == flags::arg_v))
					{
						//temp += vector_destruct(pointer->point_right);
						delete pointer;
						pointer = NULL;
						return temp;
					}
				}
			}
			else if (pointer->type == flags::varbl)
			{
				//удаление связного списка
				if ((pointer->point_right == NULL) || ((pointer->point_right->var == 0) && (pointer->point_right->type == flags::varbl)))
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
			if (sorting.size() >= 1)
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
				temp->var_list_number();
				return temp;
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
		void  math_obj::var_list_number()
		{
			if (point_right != NULL)
			{
				point_right->var = var + 1;
				point_right->var_list_number();
			}
		}









		//DATA_LIST

		data_list * data_list::back_rec()
		{
			if (right != NULL)
			{
				return right->back_rec();
			}
			return this;
		}

		void data_list::index_plus_one()
		{
			index += 1;
			if (right != NULL)
				right->index_plus_one();
		}

		data_list * data_list::at_rec(int* place)
		{
			if (index == *place)
				return this;
			if (right != NULL)
				return right->at_rec(place);
			return nullptr;
		}

		int data_list::size_rec(bool* flag)
		{
			if (*flag) //если true
			{
				if (right != NULL)
				{
					return (1 + right->size_rec(flag));
				}
				else
					return 1;
			}
			else
			{
				if (left != NULL)
				{
					return (1 + left->size_rec(flag));
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
			if (in.compare(L"@start#") != 0)
				return 0;
			data_list*place = back();
			if (place == nullptr)
				return 0;
			place->right = pointer;
			place->right->left = place;
			place->right->index = index;
			index++;
			return 1;
		}

		int data_list::push_left(math_obj * pointer)
		{
			data_list * zero = begin();
			if (zero == NULL)
				return 0;
			data_list * temp = zero->left;
			zero->left = new data_list(L"", pointer);
			zero->left->right = zero;
			zero->left->left = temp;
			if (temp != NULL)
				temp->right = zero->left;
		}

		data_list * data_list::begin()
		{
			if (in.compare(L"@start#") == 0)
				return this;
			if (index == 0)
			{
				if (left->in.compare(L"@start#") == 0)
					return left;
			}
			if ((index > 0) && (left != NULL))
				return left->begin();
			return nullptr;
		}

		int data_list::implace(int place, data_list * pointer)
		{
			if (in.compare(L"@start#") != 0)
				return 0;
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
			index++;
			return 1;
		}

		data_list * data_list::at(int place)
		{
			if (in.compare(L"@start#") != 0)
				return nullptr;
			if (index < place)//дополнительно размер массива записывается в поле index стартового элемента
				return nullptr;
			if ((right != NULL) && (place >= 0))
				return right->at_rec(&place);
			return NULL;

		}

		data_list * data_list::back()
		{
			if (in.compare(L"@start#") != 0)
				return nullptr;
			else if (right != NULL)
			{
				return right->back_rec();
			}
			else
			{
				return this;
			}
		}

		int data_list::compare_in(wstring *original)
		{
			return in.compare(*original);
		}

		int data_list::compare_out(wstring* original)
		{
			return out.compare(*original);
		}

		math_obj * data_list::find_math_obj(wstring* name)
		{
			if ((math != NULL) && (math->name.length() != 0) && (name->compare(math->name) == 0))
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
			if (math->prop == flags::error)
			{
				delete math;
				//чистить дерево
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				out = err->GetErrorWStr();
				math = NULL;
				return 0;
			}
			/*if ((math->actn == flags::write)&&(math->type!=flags::funct))
				math->actn = flags::nthng;*/
			return 1;
		}

		int data_list::size_s()
		{
			if (in.compare(L"@start#") != 0)
				return -1;
			int out = 0;
			bool flag = true;
			if (right != NULL)
			{
				out += right->size_rec(&flag);
			}
			if (index != out)//доп. проверка для записанного в поле index стартового элемента размера массива данных
				index = out;
			return out;
		}

		int data_list::size_all()
		{
			if (in.compare(L"@start#") != 0)
				return -1;
			int out = 0;
			bool flag;
			if (right != NULL)
			{
				flag = true;
				out += right->size_rec(&flag);
			}
			if (index != out)//доп. проверка для записанного в поле index стартового элемента размера массива данных
				index = out;
			if (left != NULL)
			{
				flag = false;
				out += left->size_rec(&flag);
			}
			return out + 1;//так же считается элемент start
		}
		/*Метод удаляет все элементы списка данных начиная с позиции from. Позиция from не удаляется.
		В случае вызова для позиции from = -1 - удаляются все элементы справа от start (включая нулевой)
		-1 в случае ошибки. */
		int data_list::delete_starting_at(int from)
		{
			if (in.compare(L"@start#") != 0)
				return -1;
			index = from + 1;
			if (from > -1)
			{
				data_list* temp = at(from + 1);
				int k = temp->delete_starting_at_rec();
				delete temp;
				return k;
			}
			else if (from == -1)
			{
				data_list* temp = at(0);
				int k = temp->delete_starting_at_rec();
				delete temp;
				return k;
			}
			else return -1;
		}
		/*Рекурсия для delete_starting_at*/
		int data_list::delete_starting_at_rec()
		{
			int k = 0;
			if (right != NULL)
			{
				k += right->delete_starting_at_rec();
				delete right;
				right = NULL;
			}
			//math->~math_obj();//TODO: деструктор math_obj должен удалять всё полностью без разбора (деревья и т.д.) для выбранного экземпляра класса math_obj.
			delete math;
			return k;
		}
	}
};
