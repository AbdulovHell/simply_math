#include "classes.h"

namespace Project {

	namespace Core {
		using namespace std;
		using namespace Project;
		using namespace Project::IO;
		//BEGIN. ОБЩИЕ МЕТОДЫ ДЛЯ РАБОТЫ С ДЕРЕВОМ		

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
			else if (type == cnst)
			{
				//может не сработать)
				s += tree_destruct_processing(this);
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
				if ((pointer->name == L"minus")|| (pointer->name == L"root"))
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
						temp += vector_destruct(pointer->point_collar);
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
		/*Метод возвращает приоритет операции.
		1 - сложение
		2 - умножение
		3 - деление
		4 - возведение в степень
		0 - не операция*/
		int math_obj::get_priority()
		{
			if (type == addit)
				return 1 + (int)var;
			else if (type == mltpl)
				return 2 + (int)var;
			else if (type == divis)
				return 3 + (int)var;
			else if (type == power)
				return 4 + (int)var;
			else
				return 0;
		}
		/*Метод вызывает рекурсивную функцию проверки приоритета операций в текущем дереве операций.
		Возвращает указатель на элемент дерева (операцию), имеющий приоритет меньший или равный в сравнении с текущим элементом. */
		math_obj* math_obj::prioritize(int current_priority)
		{
			return prioritize_processing(point_collar, current_priority);
		}
		/*PRIVATE. Рекурсия для prioritize*/
		math_obj* math_obj::prioritize_processing(math_obj *pc, int current_priority)
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
		/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и коструирующую строку с формальной записью текущего выражения.
		Возвращает строку. ПОКА НЕ РАБОТАЕТ*/
		wstring math_obj::expresion(int comma)
		{
			return name + L" = " + expression_processing(point_left, &comma);
		}
		/*PRIVATE. Рекурсия для expresion*/
		wstring math_obj::expression_processing(math_obj *pointer, int* comma)
		{

			if ((pointer->type == mltpl) || (pointer->type == divis))
			{
				return expression_processing(pointer->point_left, comma) + pointer->name + expression_processing(pointer->point_right, comma);
			}
			else if (pointer->type == addit)
			{
				if (pointer->point_right->name == L"minus")
				{
					if ((pointer->point_collar->type == mltpl) || (pointer->point_collar->type == divis))
						return L"(" + expression_processing(pointer->point_left, comma) + L" - " + expression_processing(pointer->point_right, comma) + L")";
					else
						return expression_processing(pointer->point_left, comma) + L" - " + expression_processing(pointer->point_right, comma);
				}
				else if ((pointer->point_collar->type == mltpl) || (pointer->point_collar->type == divis))
					return L"(" + expression_processing(pointer->point_left, comma) + L" " + pointer->name + L" " + expression_processing(pointer->point_right, comma) + L")";
				else
					return expression_processing(pointer->point_left, comma) + L" " + pointer->name + L" " + expression_processing(pointer->point_right, comma);
			}
			else if (pointer->type == numbr)
			{
				return to_string(pointer->var, var_type::FRACTIONAL, *comma);
			}
			else if (pointer->type == funct)
			{
				if (pointer->name == L"minus")
				{
					return L"-" + to_string(pointer->point_right->var, var_type::FRACTIONAL, *comma);
				}
				else
				{
					return  expression_processing(pointer->point_left, comma);
				}
			}
			else
			{
				return pointer->name;
			}
		}

		//END. ОБЩИЕ МЕТОДЫ ДЛЯ РАБОТЫ С ДЕРЕВОМ

		//BEGIN. ВЫЧИСЛИТЕЛЬНЫЕ МЕТОДЫ

		/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и выполняющую их.
		Результатом работы метода является запись результата вычислений в double var текущего элемента класса. */
		void math_obj::arithmetic()
		{
			if (type == funct)
			{
				if (prop == arg_c)
					var = arithmetic_processing(point_left, point_right, point_right);
				else
				{
					//ничего делать не надо, можно выдавать ошибку.
				}
			}
			else if ((type == cnst) || (type == exprs))
				var = arithmetic_processing(point_left, NULL,NULL);
			else
			{
				//ничего делать не надо
			}
		}
		/*PRIVATE. Рекурсия для arithmetic*/
		double math_obj::arithmetic_processing(math_obj *pointer, math_obj * last_arg, math_obj *arg)
		{
			if (pointer->type == addit)
				return arithmetic_processing(pointer->point_left, last_arg,arg) + arithmetic_processing(pointer->point_right, last_arg, arg);
			else if (pointer->type == mltpl)
				return arithmetic_processing(pointer->point_left, last_arg, arg) * arithmetic_processing(pointer->point_right, last_arg, arg);
			else if (pointer->type == divis)
				return arithmetic_processing(pointer->point_left, last_arg, arg) / arithmetic_processing(pointer->point_right, last_arg, arg);
			else if (pointer->type == power)
				return pow(arithmetic_processing(pointer->point_left, last_arg, arg), arithmetic_processing(pointer->point_right, last_arg, arg));
			else if ((pointer->type == numbr) || (pointer->type == cnst))
				return pointer->var;
			else if (pointer->type == exprs)
				//TODO: выражение вычислить, сразу же его удалить (вместе с деревом операций), а вместо него записать вычисленное число.
				return arithmetic_processing(pointer->point_left, last_arg, arg);
			else if (pointer->type == funct)
			{
				if (pointer->prop == arg_c)
					return arithmetic_processing(pointer->point_left, pointer->point_right, pointer->point_right);
				else if (pointer->prop == arg_v)
					return arithmetic_processing(pointer->point_left, pointer->point_right, arg);
				else
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::VARIABL_FUNCT);
					return 0;
				}
			}
			else if (pointer->type == varbl)
			{				
				math_obj* try_get_arg = get_arg_for_var(last_arg->vector_at((int)pointer->var), arg);
				if (try_get_arg!=NULL)
					return arithmetic_processing(try_get_arg, last_arg, arg);
				else
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::VARIABL_FUNCT);
					return 0;
				}
			}
			else
				return 0;
		}

		/*PRIVATE. Метод возвращает указатель на аргумент соответствующий найденной переменной*/
		math_obj* math_obj::get_arg_for_var(math_obj*pointer, math_obj*arg)
		{
			if ((pointer->type == cnst) || (pointer->type == exprs)|| (pointer->type == funct)|| (pointer->type == numbr))
				return pointer;			
			else if (pointer->type == varbl)
			{
				if (pointer->point_collar == NULL)
					return NULL;
				else
					return arg->vector_at((int)pointer->point_collar->var);
			}	
			else
				return NULL;
		}

		//END. ВЫЧИСЛИТЕЛЬНЫЕ МЕТОДЫ

		//BEGIN. ОБЩИЕ МЕТОДЫ ДЛЯ РАБОТЫ С ВЕКТОРАМИ (ОДНОСВЯЗНЫМИ СПИСКАМИ)

		//Конструктор вектора
		math_obj::math_obj(int size_n)
		{
			name = L"";
			type = vectr;
			prop = L"";
			actn = L"";
			var = size_n;
			point_left = NULL;
			point_right = NULL;
			point_collar = vector_create(size_n);
		}
		//Конструктор вектора с именем
		math_obj::math_obj(wstring _name, int size_n)
		{
			name = _name;
			type = vectr;
			prop = L"";
			actn = L"";
			var = 0;
			point_left = NULL;
			point_right = NULL;
			point_collar = vector_create(size_n);
		}
		/*PRIVATE. Деструктор вектора*/
		int math_obj::vector_destruct(math_obj* pointer)
		{
			int s = 0;
			if (pointer->type == vectr)
			{
				s += vector_destruct(pointer->point_collar);				
				delete pointer;
			}
			else
			{
				if (pointer->point_right != NULL)
				{
					s += vector_destruct(pointer->point_right);
					if ((pointer->prop == servc)&&(pointer->type == varbl))
					{
						s += tree_destruct_processing(pointer->point_collar);
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
		/*PRIVATE. Метод рекурсивно создаёт связный список указанной длинны size*/
		math_obj* math_obj::vector_create(int size)
		{
			math_obj* temp = NULL;
			if (size > 0)
			{
				temp = new math_obj();
				temp->point_right = vector_create(size - 1);
			}
			return temp;
		}
		/*Метод добавляет в вектор элемент на место index, при условии что index существует.
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
					place = point_collar;
					if (pointer->type == funct)
					{
						point_collar = new math_obj(L"temporary_variable", varbl, servc, 0, NULL, place->point_right, pointer);
					}
					else if (pointer->type == vectr)
					{
						//пока ошибка. Ничего не делать
						return -1;
					}
					else if ((pointer->type == varbl) || (pointer->type == cnst) || (pointer->type == exprs))
					{
						point_collar = pointer;
						point_collar->point_right = NULL;
						point_collar->point_collar = NULL;
						point_collar->point_right = place->point_right;
					}
				}
				else
				{
					math_obj*place_minus_1 = vector_at(index - 1);					
					place = place_minus_1->point_right;
					if (pointer->type == funct)
					{
						place_minus_1->point_right = new math_obj(L"temporary_variable", varbl, servc, 0,NULL, place->point_right, pointer);						
					}
					else if (pointer->type == vectr)
					{
						//пока ошибка. Ничего не делать
						return -1;
					}
					else if ((pointer->type == varbl) || (pointer->type == cnst) || (pointer->type == exprs))
					{
						place_minus_1->point_right = pointer;
						place_minus_1->point_right->point_right = NULL;
						place_minus_1->point_right->point_collar = NULL;
						place_minus_1->point_right->point_right = place->point_right;						
					}					
				}
				if ((place->prop == servc) && (place->type == varbl))
				{
					place->point_collar->tree_destruct();
					delete place->point_collar;
				}
				delete place;
				return 0;
			}
		}
		/*Метод возвращает указатель на элемент списка с номером index.*/
		math_obj* math_obj::vector_at(int index)
		{
			if ((type != vectr) || (point_collar == NULL) || (index >= vector_size()))
			{
				//выход за пределы вектора можно сделать отдельной ошибкой
				return NULL;
			}
			else
			{
				math_obj*place = vector_at_processing(point_collar, &index, 0);
				if ((place->prop == servc)&& (place->type == varbl))
				{
					return place->point_collar;
				}
				else
				{
					return place;
				}
			}
		}
		/*PRIVATE. Рекурсия для vector_at*/
		math_obj* math_obj::vector_at_processing(math_obj*pointer, int* index, int count)
		{
			if (*index == count)
				return pointer;
			else if (pointer->point_right != NULL)
				return vector_at_processing(pointer->point_right, index, count + 1);
			else
				//такой случай невозможен, но оставил возврат NULL
				return NULL;
		}
		/*Метод возвращает количество элементов списка.*/
		int math_obj::vector_size()
		{
			if (type != vectr)
			{
				return -1;
			}
			else if (point_collar == NULL)
			{
				return 0;
			}
			else
			{
				return vector_size_processing(point_collar);
			}
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
		/*Метод добавляет элемент в конец вектора.
		-1 в случае ошибки*/
		int math_obj::vector_push_back(math_obj*pointer)
		{
			//полагаем что pointer - есть копия какого либо элемента
			if (type != vectr)
			{
				//ошибка. Ничего не делать
				return -1;
			}
			else if (point_collar == NULL)
			{
				if (pointer->type == funct)
				{
					point_collar = new math_obj(L"temporary_variable", varbl, servc, 0, pointer);
				}
				else if (pointer->type == vectr)
				{
					//пока ошибка. Ничего не делать
					return -1;
				}
				else if ((pointer->type == varbl)|| (pointer->type == cnst)|| (pointer->type == exprs)|| (pointer->type == numbr))
				{
					point_collar = pointer;
					point_collar->point_right = NULL;
					point_collar->point_collar = NULL;
				}				
			}
			else
			{
				math_obj* place = vector_back_processing(point_collar);
				if (pointer->type == funct)
				{
					place->point_right = new math_obj(L"temporary_variable", varbl, servc, 0, pointer);
				}
				else if (pointer->type == vectr)
				{
					//пока ошибка. Ничего не делать
					return -1;
				}
				else if ((pointer->type == varbl) || (pointer->type == cnst) || (pointer->type == exprs) || (pointer->type == numbr))
				{
					place->point_right = pointer;
					place->point_right->point_right = NULL;
					place->point_right->point_collar = NULL;
				}				
			}
			return 0;
		}
		/*Метод добавляет элемент в начало вектора.
		-1 в случае ошибки*/
		int math_obj::vector_push_foward(math_obj*pointer)
		{
			if (type != vectr)
			{
				//ошибка. Ничего не делать 	
				return -1;
			}
			else if (point_collar == NULL)
			{
				if (pointer->type == funct)
				{
					point_collar = new math_obj(L"temporary_variable", varbl, servc, 0, pointer);
				}
				else if (pointer->type == vectr)
				{
					//пока ошибка. Ничего не делать
					return -1;
				}
				else if ((pointer->type == varbl) || (pointer->type == cnst) || (pointer->type == exprs))
				{
					point_collar = pointer;
					point_collar->point_right = NULL;
					point_collar->point_collar = NULL;
				}				
			}
			else
			{
				math_obj* place = point_collar;
				if (pointer->type == funct)
				{
					point_collar = new math_obj(L"temporary_variable", varbl, servc, 0, pointer);
					point_collar->point_right = place;
				}
				else if (pointer->type == vectr)
				{
					//пока ошибка. Ничего не делать
					return -1;
				}
				else if ((pointer->type == varbl) || (pointer->type == cnst) || (pointer->type == exprs))
				{
					point_collar = pointer;
					point_collar->point_right = place;
					point_collar->point_collar = NULL;
				}				
			}
			return 0;
		}
		/*Метод возвращает указатель на последний элемент вектора*/
		math_obj* math_obj::vector_back()
		{
			if (point_collar == NULL)
				return NULL;
			else
				return vector_back_processing(point_collar);
		}
		/*PRIVATE. Рекурсия для var_list_back и vector_back*/
		math_obj* math_obj::vector_back_processing(math_obj *pointer)
		{
			if ((pointer->point_right == NULL) || ((pointer->point_right->var == 0) && (pointer->point_right->type == varbl)))
				return pointer;
			else
				return vector_back_processing(pointer->point_right);
		}
		/*Метод преобразует односвязный список в двусвязный*/
		void math_obj::double_lincked_vector()
		{
			if (point_collar != NULL)
			reassing_left_pointers(point_collar);						
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

		//END. ОБЩИЕ МЕТОДЫ ДЛЯ РАБОТЫ С ВЕКТОРАМИ


		//BEGIN. МЕТОДЫ ДЛЯ РАБОТЫ С ФУНКЦИЯМИ

		/*Медод определения функции. Создаёт список переменных, от которых зависит функция, основываясь на списке переменных и аргументов функции pointer.*/
		void math_obj::funct_define(math_obj *pointer)
		{
			math_obj* temp_pointer;
			if (pointer->prop == defnd)
			{
				pointer->point_right = new math_obj((int)0);
				var_list_copy_to_vector(pointer->point_collar, pointer->point_right);
				temp_pointer = new math_obj((int)0);
				temp_pointer->prop = servc;
				var_list_copy_to_vector(pointer->point_right->point_collar, temp_pointer);				
				point_collar = temp_pointer->point_collar;
				reassing_left_pointers(point_collar);
				//var_list_number(temp_var); - может понадобится
				delete temp_pointer;
				var_list_collar(point_collar, this);
				temp_pointer = vector_back_processing(point_collar);
				point_collar->point_left = temp_pointer;
				temp_pointer->point_right = point_collar;
				var = temp_pointer->var + 1;
				point_left = pointer;
				point_left->prop = arg_v;
			}
			else if (pointer->prop == undef)
			{
				//простой случай: f=x+y*6..., где все буквенные выражения (x,y,...) - переменные и нет аргументов
				point_collar = sort_list(pointer->point_collar);
				var_list_collar(point_collar, this);
				temp_pointer = vector_back_processing(point_collar);
				var = temp_pointer->var + 1;
				temp_pointer->point_right = point_collar;
				point_collar->point_left = temp_pointer;
				point_left = pointer->point_left;
				delete pointer;
			}
			else if (pointer->prop == arg_v)
			{
				temp_pointer = create_var_list(pointer, NULL);
				point_collar = sort_list(temp_pointer->point_collar);
				var_list_collar(point_collar, this);
				delete temp_pointer;
				temp_pointer = vector_back_processing(point_collar);
				var = temp_pointer->var + 1;
				temp_pointer->point_right = point_collar;
				point_collar->point_left = temp_pointer;
				point_left = pointer;
			}
		}

		/*Метод собирает список переменных данной функции по спискам переменных функций, участвующих в её записи*/
		math_obj* math_obj::create_var_list(math_obj* pointer, math_obj *var_list)
		{
			math_obj *iter = NULL;
			math_obj *place = NULL;

			if (var_list == NULL)
			{
				var_list = new math_obj((int)0);
				var_list->prop = servc;
			}
			for (int count = 0; count < (int)pointer->var; count++)
			{
				iter = pointer->point_right->vector_at(count);
				if (iter->type == varbl)
				{
					place = find_by_name_processing(var_list, iter);
					if (place == NULL)
					{
						place = new math_obj(iter);
						iter->point_collar = place;
						var_list->vector_push_back(place);
					}
					else
					{
						iter->point_collar = place;
					}					
				}
				else if (iter->type == funct)
				{
					if (iter->prop == arg_v)
					{
						var_list = create_var_list(iter, var_list);
					}
					else if ((iter->prop == undef) || (iter->prop == defnd))
					{
						place = new math_obj((int)0);						
						var_list_copy_to_vector(iter->point_collar, place);
						iter->prop = arg_v;
						iter->point_right = place;
						var_list_copy_to_vector(place->point_collar, var_list);
					}
				}
			}			
			return var_list;
		}

		/*Метод сравнивает список аргументов (хотя бы один из которых переменная) функции pointer со списком переменных данной функции.
		Метод возвращает:
		0 - список переменных pointer полностью входит в список переменных данной функции.
		любое другое положительное целое - число различий списков функций.
		*/
		int math_obj::var_list_compare(math_obj * pointer)
		{
			int count_var = 0;
			int temp = 0;
			math_obj * place;
			//рассматриваются функции с аргументами (определённые и нет)
			if (pointer->prop == arg_v)
			{

				for (int count = 0; count < pointer->var; count++)
				{
					place = pointer->point_right->vector_at(count);
					//если среди аргументов попалась переменная
					if (place->type == varbl)
					{
						//если эта переменная не в списке данной функции
						if (find_by_name_processing(point_collar, place) == NULL)
							count_var++;
					}
					//если функция
					else if (place->type == funct)
					{
						count_var +=  var_list_compare(place);						
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

		/*Метод сортирует незамкнутый список переменных по алфавиту*/
		math_obj* math_obj::sort_list(math_obj * var_list)
		{
			stack <math_obj*> sorting;
			math_obj * temp_var;
			while (var_list != NULL)
			{
				sorting.push(var_list);
				sorting.top()->var = 0;
				var_list = var_list->point_right;
				sorting.top()->point_right = NULL;
				if (sorting.size() > 1)
				{
					temp_var = sorting.top();
					sorting.pop();
					while ((sorting.size() >= 1) && (temp_var->var == sorting.top()->var))
					{
						temp_var = merge_lists(temp_var, sorting.top());
						temp_var->var += 1;
						sorting.pop();
					}
					sorting.push(temp_var);
				}
			}
			if (sorting.size() > 1)
			{
				temp_var = sorting.top();
				sorting.pop();
				while (sorting.size() >= 1)
				{
					temp_var = merge_lists(temp_var, sorting.top());
					temp_var->var += 1;
					sorting.pop();
				}
				temp_var->var = 0;
				reassing_left_pointers(temp_var);
				var_list_number(temp_var);
				return temp_var;
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
				pointer->point_right->var = pointer->var+1;
				reassing_left_pointers(pointer->point_right);
			}
		}
		/*Метод возвращает указатель на последний элемент списка переменных данной функции.
		Если список замкнут - на элемент предшествующий нулевому.*/
		math_obj* math_obj::var_list_back()
		{
			return vector_back_processing(point_collar);
		}
		/*PRIVATE. Установка указателей point_collar для НЕЗАМКНУТОГО списка переменных на функцию*/
		void math_obj::var_list_collar(math_obj* pointer, math_obj*original)
		{
			pointer->point_collar = original;
			if (pointer->point_right != NULL)
			{				
				var_list_collar(pointer->point_right,original);
			}
		}
		/*Метод возвращаяет указатель на элемент списка с данным именем*/
		math_obj* math_obj::find_by_name(math_obj*pointer)
		{
			return find_by_name_processing(point_collar, pointer);
		}
		/*PRIVATE. Рекурсия для find_by_name*/
		math_obj* math_obj::find_by_name_processing(math_obj*pointer, math_obj *original)
		{
			if (pointer == NULL)
				return NULL;
			if (original->name.compare(pointer->name) == 0)
				return pointer;
			else if ((pointer->point_right != NULL) && (pointer->point_right->var != 0))
				return find_by_name_processing(pointer->point_right, original);
			else
				return NULL;
		}

		/*Метод добавляет элемент в конец вектора аргументов функции.
		-1 в случае ошибки*/
		int math_obj::funct_arg_push_back(math_obj*pointer)
		{			
			math_obj* iter;
			if (point_right == NULL)
			{
				if (type == funct)
				{
					point_right = new math_obj((int)0);
					var_list_copy_to_vector(point_collar, point_right);
					point_right->vector_push_back(pointer);
				}
				else if (type == exprs)
				{
					point_right = new math_obj((int)0);
					point_right->vector_push_back(pointer);
				}
			}
			else
			{
				point_right->vector_push_back(pointer);
			}
			return 0;
		}
		/*PRIVATE. Метод рекурсивно копирует список переменных в вектор.
		-1 при ошибке*/
		int math_obj::var_list_copy_to_vector(math_obj* pointer, math_obj* vect)
		{
			if (vect->prop == servc)
			{
				math_obj* temp = find_by_name_processing(vect->point_collar, pointer);
				if (temp == NULL)
				{
					vect->vector_push_back(new math_obj(pointer));
					pointer->point_collar = vect->vector_back();					
				}
				else
				{
					pointer->point_collar = temp;
				}
			}
			else
				vect->vector_push_back(new math_obj(pointer));

			if ((pointer->point_right != NULL) && (pointer->point_right->var != 0))
			{
				return var_list_copy_to_vector(pointer->point_right, vect);
			}
			else
				return 0;
		}
		

		//END. МЕТОДЫ ДЛЯ РАБОТЫ С ФУНКЦИЯМИ




		//Конструктор матрицы НИЧЕГО НЕ ДЕЛАЕТ!
		math_obj::math_obj(int size_n, int size_m)
		{

		}












		/*math_obj* math_obj::var_list_to_arg(math_obj*var_list, int size)
		{
			math_obj* mass_arg = new math_obj[size];
			for (int count = 0; count < size; count++)
			{
				mass_arg[count].copy(var_list);
				mass_arg[count].point_left = NULL;
				mass_arg[count].point_right = NULL;
				mass_arg[count].point_collar = NULL;
				mass_arg[count].var = 0;
				var_list = var_list->point_left;
			}
			return mass_arg;
		}

		math_obj* math_obj::arg_to_var_list(math_obj*mass_arg, math_obj*var_list, int size)
		{
			math_obj*iter, *place;
			if (var_list == NULL)
			{
				var_list = new math_obj(&mass_arg[0]);
				mass_arg[0].point_collar = var_list;
				iter = var_list;
				var_list->var = 0;
				for (int count = 1; count < size; count++)
				{
					iter->point_left = new math_obj(&mass_arg[count]);
					iter->point_left->point_right = iter;
					iter->point_left->var = count;
					iter = iter->point_left;
					mass_arg[count].point_collar = iter;
				}
			}
			else
			{
				iter = vector_back_processing(var_list);//указатель на последний элемент списка
				for (int count = 0; count < size; count++)
				{
					place = find_by_name_processing(var_list, &mass_arg[count]);
					if (place == NULL)
					{
						iter->point_left = new math_obj(&mass_arg[count]);
						iter = iter->point_left;
						mass_arg[count].point_collar = iter;
					}
					else
					{
						mass_arg[count].point_collar = place;
					}
				}
			}
			return var_list;
		}*/


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
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, ЧИСЛО типа double) 
		math_obj::math_obj(wstring _name, wstring _type, double _num)
		{
			name = _name;
			type = _type;
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
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double) 
		math_obj::math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num)
		{
			name = _name;
			type = _type;
			prop = _prop;
			actn = _actn;
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = NULL;
		}
		//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _name, double _num, math_obj *_pc)
		{
			name = _name;
			type = L"";
			prop = L"";
			actn = L"";
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = _pc;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _name, wstring _type, double _num, math_obj *_pc)
		{
			name = _name;
			type = _type;
			prop = L"";
			actn = L"";
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = _pc;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj *_pc)
		{
			name = _name;
			type = _type;
			prop = _prop;
			actn = L"";
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = _pc;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj *_pc)
		{
			name = _name;
			type = _type;
			prop = _prop;
			actn = _actn;
			var = _num;
			point_left = NULL;
			point_right = NULL;
			point_collar = _pc;
		}
		//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав") 
		math_obj::math_obj(wstring _name, double _num, math_obj * _pl, math_obj *_pr) {
			name = _name;
			type = L"";
			prop = L"";
			actn = L"";
			var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = NULL;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав") 
		math_obj::math_obj(wstring _name, wstring _type, double _num, math_obj * _pl, math_obj *_pr) {
			name = _name;
			type = _type;
			prop = L"";
			actn = L"";
			var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = NULL;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав") 
		math_obj::math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj * _pl, math_obj *_pr) {
			name = _name;
			type = _type;
			prop = _prop;
			actn = L"";
			var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = NULL;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав") 
		math_obj::math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr) {
			name = _name;
			type = _type;
			prop = _prop;
			actn = _actn;
			var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = NULL;
		}
		//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _name, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
			name = _name;
			type = L"";
			prop = L"";
			actn = L"";
			var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = _pc;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _name, wstring _type, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
			name = _name;
			type = _type;
			prop = L"";
			actn = L"";
			var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = _pc;
		}
		//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
		math_obj::math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
			name = _name;
			type = _type;
			prop = _prop;
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

	}
};
