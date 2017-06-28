#pragma once
#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include "IO.h"
#include "error.h"


namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;

		enum class flags : short
		{
			error = -10,//данный флаг поднимается в поле свойств
			nthng = -100,
			//типы мат. объектов
			matrx = -7,
			vectr = -6,
			exprs = -5,
			equat = -4,
			funct = -3,
			varbl = -2,
			cnst = -1,
			numbr = 0,
			//типы операций
			addit = 1,
			minus = 2,
			mltpl = 3,
			divis = 4,
			power = 5,
			//свойства
			defnd = 6,			//общее свойство
			undef = 7,			//общее свойство
			arg_v = 8,			//функция/вектор с переменными аргументами 
			arg_c = 9,			//функция/вектор с константными аргументами
			only_arg_v = 10,	//свойство вектора, содержащего только переменные

			fundm = 11,			//свойства фундаметальных объектов: констант, натуральных функций и т.д.

			unslv = 12,			//свойства уравнений
			solvd = 13,			//свойства уравнений

			real = 14,			//свойство чисел, констант, возможно, функций
			cmplx = 15,			//свойство чисел, констант, возможно, функций
			qwtrn = 16,			//свойство чисел, констант, возможно, функций

			servc = 20,			//свойство для служебных объектов
			//действия
			solve = 17,
			write = 18,
			read = 19
		};

		class math_obj;

		class data_list {
		private:

			//Рекурсия для back
			data_list*back_rec();

			//Метод рекурсивно сдвигает индексы элементов вправо, начиная с текущего.
			void index_plus_one();

			//Рекурсия для метода at
			data_list*at_rec(int* place);

			//Рекурсия для методов size
			int size_rec(bool* flag);

			/*Рекурсия для delete_starting_at*/
			int delete_starting_at_rec();

		public:
			int index;
			wstring in;
			wstring out;
			data_list* left;
			data_list* right;
			math_obj* math;

			//Нулевой конструктор
			data_list();
			//Коструктор для строкой ввода in
			data_list(wstring* _in);
			//Коструктор для строкой ввода in, мат. объекта math
			data_list(wstring _in, math_obj* _math);

			/*Метод вставляет элемент pointer в конец списока данных.
			0 - в случае ошибки (номера place не существует)
			1 - в случае успешного выполнения*/
			int push_back(data_list* pointer);

			/*Метод вставляет элемент pointer в список данных до нулевого элемента. Индекс присваивается равным -1.
			Метод возвращает:
			0 - в случае ошибки 
			1 - в случае успешного выполнения*/
			int push_left(math_obj* pointer);

			/*Метод возвращает указатель на нулевой элемент списка данных.
			NULL - в случае ошибки (например, попытки поиска слева от нуля*/
			data_list*begin();

			/*Метод вставляет элемент pointer в список данных после номера place. Индексы следующих элементов списка сдвигаются на 1.
			Метод возвращает:
			0 - в случае ошибки (номера place не существует)
			1 - в случае успешного выполнения*/
			int implace(int place, data_list*pointer);

			/*Метод возвращает указатель на элемент списка с номером place. 			
			NULL - номера place нет в списке или он меньше нуля, и в случае вызова не для начального (нулевого) элемента*/
			data_list* at(int place);

			/*Метод возвращает указатель на последний элемент списка данных. Если ничего не было записано - указатель на нулевой элемент. 
			NULL - в случае вызова не для начального (нулевого) элемента*/
			data_list* back();

			/*Метод сравнения строки in (ввод) с заданной строкой. Аналогичен методу compare класса wstring
			Метод возвращает:
			- 0 - строки совпадают
			- разницу между первыми различными символами*/
			int compare_in(wstring *original);

			/*Метод сравнения строки out (вывод) с заданной строкой. Аналогичен методу compare класса wstring
			Метод возвращает:
			- 0 - строки совпадают
			- разницу между первыми различными символами*/
			int compare_out(wstring *original);

			/*Метод рекурсивно выполняет поиск по строке name в списке данных.
			Метод возвращает:
			- указатель math_obj* на нужный мат. объект
			- NULL если ничего не найдено*/
			math_obj* find_math_obj(wstring* name);

			/*Метод вызыват конструктор математического объекта по строке in. Тут же происходит проверка ввода.			 
			Метод возвращает:
			0 - в случае ошибки, как при заполнении, так и при проверке ввода (текст ошибки в поле out).
			1 - в случае успешного выполнения*/
			int run_string();

			/*Метод возвращает количество записанных элементов (строк) в списке данных.
			Нумерация значащих элементов (со строками) начинается с 1 (не по православному, но так проще список хранить).
			0 - если после нулевого элемента ничего нет
			-1 - ошибка - обращение к элементу имеющему ненулевой индекс.
			*/
			int size_s();

			/*Метод возвращает общее количество элементов в списке данных
			0 - если после нулевого элемента ничего нет
			-1 - ошибка - обращение к элементу имеющему ненулевой индекс.
			*/
			int size_all();
			/*Метод удаляет все элементы списка данных начиная с позиции start. Позиция start не удаляется*/
			int delete_starting_at(int from);
			

		};






		class math_obj {
		private:
			/*PRIVATE. Метод строит дерево математических операций для данного математического объекта.
			Данный метод в общем случае может быть рекурсивен.
			Метод возвращает указатель на созданный экземпляр класса math_obj, представляющий собой верхушку (корень) дерева, либо на копию уже созданного экземпляра.
			NULL в случае какой-либо ошибки, см. error.h.*/
			math_obj* build_tree(wchar_t* strPtr, wchar_t*endPtr);
			/*PRIVATE.Метод создаёт мат. объект - мат. операцию, в соответствии с op_type, и записывает на нужное место в дереве high, с учётом low.
			Метод возвращает указатель на созданную операцию.*/
			math_obj *operations(math_obj *high, math_obj *low, flags op_type);
			/*PRIVATE.Метод возвращает приоритет операции, по указателю на строку с названием типа.
			1 - сложение/вычитание
			2 - умножение
			3 - деление
			4 - возведение в степень
			0 - не операция*/
			int priority(flags *op_type);




			math_obj * math_simplify_processing(vector<math_obj*>* last_funct);

			wstring expression_processing();

			/*PRIVATE. Рекурсия для tree_destruct*/
			int tree_destruct_processing(math_obj* pointer);
			/*PRIVATE. Рекурсия для prioritize*/
			math_obj *prioritize_processing(math_obj *pc, int *current_priority);			
			/*PRIVATE. Рекурсия для var_list_back*/
			math_obj *vector_back_processing(math_obj *pointer);
			/*PRIVATE. Рекурсия для find_by_name*/
			math_obj *find_by_name_processing(math_obj*pointer, math_obj *original);
			/*PRIVATE. Слияние отсортированных списков*/
			math_obj *merge_lists(math_obj* list_a, math_obj* list_b);
			/*PRIVATE. Создание двусвязного списка.*/
			void reassing_left_pointers(math_obj* pointer);
			/*PRIVATE. Рекурсия для vector_at*/
			math_obj* vector_at_processing(math_obj*pointer, int* index, int* count);
			/*PRIVATE. Рекурсия для vector_size*/
			int vector_size_processing(math_obj*pointer);
			/*PRIVATE. Метод рекурсивно создаёт связный список указанной длинны size из служебных переменных, указывающих на один и тот же объект.*/
			math_obj* convert_to_vector(int size, math_obj*obj);
			/*PRIVATE. Метод рекурсивно копирует список переменных в вектор.
			-1 при ошибке*/
			int var_list_copy_to_vector_processing(math_obj* pointer, math_obj* vect);
			/*PRIVATE. Нумерация переменных по порядку.*/
			void var_list_number();
			/*PRIVATE. Установка указателей point_collar для НЕЗАМКНУТОГО списка переменных на функцию*/
			void var_list_collar(math_obj* pointer, math_obj*original);			
			/*PRIVATE. Рекурсия для vector_destruct.*/
			int vector_destruct_processing(math_obj* pointer, int *flag);
			/*PRIVATE. Метод создаёт математический объект - вектор длины size_n по строке символов типа wchar_t, имеющей начало begin и конец end.
			Метод возвращает указатель на созданный экземпляр класса math_obj. Функция вызывает в качестве внутренней функции, а так же рекурсивно, основную функцию заполнения build_tree.
			NULL в случае какой-либо ошибки, см. error.h.*/
			math_obj* vector_create(int size_n, wchar_t*begin, wchar_t*end);


		public:
			//Нулевой конструктор
			math_obj();
			//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double) 
			math_obj(wstring _name, double _num);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double) 
			math_obj(wstring _name, flags _type, flags _prop, double _num);
			//Конструктор записывает (ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
			math_obj(flags _type, flags _prop, double _num, math_obj *_pc);
			//Конструктор записывает (ТИП объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
			math_obj(flags _type, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, flags _type, flags _prop, flags _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//Конструктор копирования
			math_obj(math_obj* var1);

			//Конструктор вектора длины size_n
			math_obj(size_t size_n);
			//Конструктор вектора длины size_n по строке символов типа wchar_t, имеющей начало begin и конец end.
			math_obj(size_t size_n, wchar_t*begin, wchar_t*end, data_list* data);

			//Конструктор вектора служебных переменных, указывающих на один и тот же объект obj. Полагается что вектор унаследует имя объекта obj
			math_obj(size_t size_n, math_obj * obj);

			/*Конструктор дерева операций по строке символов типа wchar_t, имеющей начало strPtr и конец ePtr.
			Дополнительный параметр _pc присваивается полю point_collar (для получения дерева операций для функций с заранее обозначенным списком переменных).*/
			math_obj(wchar_t* strPtr, wchar_t* ePtr, math_obj* _pc, data_list* data);


			//Конструктор матрицы
			//math_obj(int size_n, int size_m);

			//Деструктор TODO: зачем он, если в нем пусто..., можно сюда запихнуть функционал tree_destrust, когда её отладим.
			~math_obj();
			//Метод копирования.
			void copy(math_obj* ref);

			/*Метод возвращает приоритет операции, при условии, что this операция.
			1 - сложение/вычитание
			2 - умножение
			3 - деление
			4 - возведение в степень
			0 - не операция*/
			int get_priority();

			/*Метод вызывает рекурсивную функцию проверки приоритета операций в текущем дереве операций.
			Возвращает указатель на элемент дерева (операцию), имеющий приоритет меньший или равный в сравнении с текущим элементом. */
			math_obj* prioritize(int current_priority);

			bool math_simplify();

			wstring expression();

			wstring vector_to_string();

			//wstring vector_to_string_procesing();


			
			/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и очищающую память.
			Метод должен возвращать число ошибок при использовании delete. Это надо дописать
			*/
			int tree_destruct();
			/*Метод возвращаяет указатель на элемент списка с данным именем*/
			math_obj* find_by_name(math_obj*pointer);
			
			/*Метод сравнивает список аргументов (хотя бы один из которых переменная) обхекта pointer (функции, вектора или матрицы) со списком переменных данной функции.
			Метод возвращает:
			0 - список переменных pointer полностью входит в список переменных данной функции.
			любое другое положительное целое - число различий списков функций.
			-1 - ошибка - неверный тип оьъекта
			*/
			int var_list_compare(math_obj * pointer);
			/*Метод сравнивает список аргументов (хотя бы один из которых переменная) функции pointer со списком переменных данной функции.
			Метод возвращает:
			0 - список переменных pointer полностью входит в список переменных данной функции.
			любое другое положительное целое - число различий списков функций.
			*/
			int var_list_compare_processing(math_obj * pointer);
			/*Метод собирает список переменных данной функции по спискам переменных функций, участвующих в её записи*/
			math_obj *create_var_list(math_obj *var_list);

			/*Метод сортирует незамкнутый список переменных по алфавиту*/
			math_obj * sort_list(math_obj * var_list);
			
			/*Метод возвращает указатель на элемент списка с номером index.*/
			math_obj* vector_at(int index);
			/*Метод возвращает количество элементов списка.*/
			int vector_size();
			/*Метод возвращает указатель на последний элемент вектора*/
			math_obj* vector_back();

			/*Деструктор вектора*/
			int vector_destruct();

			/*Метод добавляет элемент pointer в конец вектора. Полагается, что pointer - независимая копия какого либо элемента.
			-1 в случае ошибки*/
			int vector_push_back(math_obj*pointer);
			/*Метод добавляет элемент pointer в начало вектора. Полагается, что pointer - независимая копия какого либо элемента.
			-1 в случае ошибки*/
			int vector_push_foward(math_obj*pointer);
			/*Метод вставляет с заменой в вектор элемент на место index, при условии что index существует.
			-1 в случае ошибки*/
			int vector_assing_at(math_obj*pointer, int index);
			
			/*Метод преобразует односвязный список в двусвязный*/
			void double_lincked_vector();
			//Метод вызывает рекурсивную функцию установки указателей point_collar для НЕЗАМКНУТОГО списка переменных на текущий экземпляр класса
			void link_var_list_to_funct();
			//Метод замыкает список переменных в кольцо.
			void close_list();
			/*Метод копирует список переменных в вектор. Если this так же вектор, то копируется его содержимое в другой вектор без повторов.
			НЕ ИСПОЛЬЗОВАТЬ ДЛЯ КОПИРОВАНИЯ ВЕКТОРОВ (хотя если вектор не servс прямой ошибки не будет:-).
			-1 при ошибке*/
			int var_list_copy_to_vector(math_obj*vector);


			/*Метод преобразует текущий элемент с заданными параметрами (ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник").
			Если текущий элемент - вектор, все элементы которого указывают на один и тот же объект, преобразуется так же объект*/
			void convert_to(flags _type, flags _prop, double _num, math_obj *_pc);
			/*Метод ПОЛНОСТЬЮ преобразует this с заданными параметрами (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник").
			Если текущий элемент - вектор, все элементы которого указывают на один и тот же объект, преобразуется так же объект*/
			void convert_totaly(wstring _name, flags _type, flags _prop, flags _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);


			//Метод get. ТЕКУЩИЙ ЭЛЕМЕНТ
			math_obj*get_this();

			
			//Метод get. ИМЯ
			wstring get_name();
			//Метод assing. ИМЯ
			void assing_name(wstring _name);

			//Метод get. ТИП
			flags get_type();
			//Метод assing. ТИП
			void assing_type(flags _type);

			//Метод get. СВОЙСТВО
			flags get_prop();
			//Метод assing. СВОЙСТВО
			void assing_prop(flags _prop);


			//Метод get. ДЕЙСТВИЕ
			flags get_actn();
			//Метод assing. ДЕЙСТВИЕ
			void assing_actn(flags _acnt);


			//Метод get. ЧИСЛО
			double get_var();
			//Метод assing. ЧИСЛО
			void assing_var(double _num);


			//Метод get. УКАЗАТЕЛЬ "левый рукав"
			math_obj* get_pl();
			//Метод assing. УКАЗАТЕЛЬ "левый рукав"
			void assing_pl(math_obj* pointer);


			//Метод get. УКАЗАТЕЛЬ "правый рукав"
			math_obj* get_pr();
			//Метод assing. УКАЗАТЕЛЬ "правый рукав"
			void assing_pr(math_obj* pointer);


			//Метод get. УКАЗАТЕЛЬ "воротник"
			math_obj* get_pc();
			//Метод assing. УКАЗАТЕЛЬ "воротник"
			void assing_pc(math_obj* pointer);


			/*Метод добавляет переменную в конец списка переменных данной функции.
			В том числе и для векторных/матричных.
			-1 в случае ошибки.*/
			int var_list_push_back(math_obj*pointer);
			/*Метод ищет в заданной строке начиная с места brackets_open соответствующую закрывающую скобку.
			Метод возвращает указатель на нужное место в строке.
			NULL в случае ошибки.*/
			wchar_t* brackets_close(wchar_t* brackets_open, wchar_t*end);
			/*Метод преобразует векторное выражение или функцию в скалярное.
			NULL в случае ошибки*/
			math_obj * unvectorize();

			enum class variable_type {
				INTEGER,
				FRACTIONAL,
				IMAGINARY,
				REAL,
				RE_IM,
				EXP	//5E+10 5*10^10
			};

			wstring name;
			flags type;
			flags prop;
			flags actn;
			double var;
			//double arg; //может пригодится.
			//int exp;
			math_obj *point_left;		//левый рукав
			math_obj *point_right;		//правый рукав
			math_obj *point_collar;	//воротник
			data_list *point_up;
		};


	}
};
#endif //CLASSES_H
