#pragma once
#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <string>
#include <stack>
#include "IO.h"
#include "error.h"


namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;
#define eror L"error"
#define cnst L"const"
#define funct  L"funct"
#define varbl  L"varbl"
#define equat  L"equat"
#define exprs  L"exprs"
#define numbr  L"numbr"
#define addit  L"oper+"
#define minus  L"oper-"
#define mltpl  L"oper*"
#define divis  L"oper/"
#define power  L"oper^"
#define vectr L"vectr" //так же является свойством (prop) опредлённых defnd векторных функций и констант, а так же константных векторных выражений
#define matrx L"matrx"
		//свойства обЪектов (prop)		
#define defnd  L"defnd"  //общее свойство
#define undef  L"undef"  //общее свойство

#define arg_c  L"arg_c" //функция/вектор с константными аргументами
#define arg_v  L"arg_v" //функция/вектор с переменными аргументами 
//#define arg_f  L"arg_f" //функция/вектор с функциональными аргументами 
#define vectr_arg_c L"v_a_c" //свойство векторной функции с константными аргументами
#define vectr_arg_v L"v_a_v"//свойство векторной функции с переменными аргументами
//#define vectr_arg_f L"v_a_f"//свойство векторной функции с функциональными аргументами
#define only_arg_v  L"arg_vo" //свойство вектора, содержащего только переменные

#define fundm  L"fundm"	//Фундаментальная константа

#define unslv  L"unslv"	//свойство уравнений и функций от конст выражений
#define solvd  L"solvd"	//свойство уравнений и функций от конст выражений

#define real L"real_"	//свойства чисел
#define cmplx L"cmplx"	//свойства чисел

#define servc  L"servc"	//свойство служебных объектов
								 //действия над объектами (actn)
#define solve  L"solve"
#define write  L"write"
#define read  L"read_"


		class data_list {
		private:



		public:
			int index;
			wstring in;
			wstring out;
			data_list* left;
			data_list* right;
			math_obj* math;


			data_list* at(int place);

			data_list* back();

			data_list* compare_in(wstring original);

			int run_string();

		};






		class math_obj {
		private:
			/*PRIVATE. Метод строит дерево математических операций для данного математического объекта.
			Данный метод в общем случае может быть рекурсивен.
			Метод возвращает указатель на созданный экземпляр класса math_obj, представляющий собой верхушку (корень) дерева, либо на копию уже созданного экземпляра.
			NULL в случае какой-либо ошибки, см. error.h.*/
			math_obj* build_tree(wchar_t* pDest, wchar_t*endPtr);

			math_obj *operations(math_obj *high, math_obj *low, wstring op_type);
			/*PRIVATE.Метод возвращает приоритет операции, по указателю на строку с названием типа.
			1 - сложение/вычитание
			2 - умножение
			3 - деление
			4 - возведение в степень
			0 - не операция*/
			int priority(wstring *op_type);






			/*PRIVATE. Рекурсия для tree_destruct*/
			int tree_destruct_processing(math_obj* pointer);
			/*PRIVATE. Рекурсия для prioritize*/
			math_obj *prioritize_processing(math_obj *pc, int current_priority);
			/*PRIVATE. Рекурсия для arithmetic*/
			double arithmetic_processing(math_obj *pointer, math_obj * last_arg, math_obj *arg);
			/*PRIVATE. Рекурсия для expresion*/
			wstring expression_processing(math_obj *pointer, int* comma);
			/*PRIVATE. Рекурсия для var_list_back*/
			math_obj *vector_back_processing(math_obj *pointer);
			/*PRIVATE. Рекурсия для find_by_name*/
			math_obj *find_by_name_processing(math_obj*pointer, math_obj *original);
			/*PRIVATE. Слияние отсортированных списков*/
			math_obj *merge_lists(math_obj* list_a, math_obj* list_b);
			/*PRIVATE. Создание двусвязного списка.*/
			void reassing_left_pointers(math_obj* pointer);
			/*PRIVATE. Рекурсия для vector_at*/
			math_obj* vector_at_processing(math_obj*pointer, int* index, int count);
			/*PRIVATE. Рекурсия для vector_size*/
			int vector_size_processing(math_obj*pointer);			
			/*PRIVATE. Метод рекурсивно создаёт связный список указанной длинны size из служебных переменных, указывающих на один и тот же объект.*/
			math_obj* convert_to_vector(int size, math_obj*obj);
			/*PRIVATE. Метод рекурсивно копирует список переменных в вектор.
			-1 при ошибке*/
			int var_list_copy_to_vector_processing(math_obj* pointer, math_obj* vect);
			/*PRIVATE. Нумерация переменных по порядку.*/
			void var_list_number(math_obj* pointer);
			/*PRIVATE. Установка указателей point_collar для НЕЗАМКНУТОГО списка переменных на функцию*/
			void var_list_collar(math_obj* pointer, math_obj*original);
			/*PRIVATE. Метод возвращает указатель на аргумент соответствующий найденной переменной*/
			math_obj* get_arg_for_var(math_obj*pointer, math_obj*arg);
			/*PRIVATE. Деструктор вектора*/
			int vector_destruct(math_obj* pointer);
			
			math_obj* vector_create(int size_n, wchar_t*begin, wchar_t*end);
			

		public:			
			//Нулевой конструктор
			math_obj();
			//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double) 
			math_obj(wstring _name, double _num);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double) 
			math_obj(wstring _name, wstring _type, wstring _prop, double _num);
			//Конструктор записывает (ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _type, wstring _prop, double _num, math_obj *_pc);
			//Конструктор записывает (ТИП объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _type, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);				
			//Конструктор копирования
			math_obj(math_obj* var1);
			
			/*Конструктор дерева операций по строке символов типа wchar_t, имеющей начало strPtr и конец ePtr. 
			Дополнительный параметр _pc присваивается полю point_collar (для получения дерева операций для функций с заранее обозначенным списком переменных).*/
			math_obj(wchar_t* strPtr, wchar_t*ePtr, math_obj* _pc);
			//Конструктор вектора длины size_n
			math_obj(int size_n);
			//Конструктор вектора длины size_n по строке символов типа wchar_t, имеющей начало begin и конец end.
			math_obj(int size_n, wchar_t*begin, wchar_t*end);
			
			//Конструктор вектора служебных переменных, указывающих на один и тот же объект obj. Полагается что вектор унаследует имя объекта obj
			math_obj(int size_n, math_obj * obj);
			
			//Конструктор матрицы
			math_obj(int size_n, int size_m);

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



			/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и коструирующую строку с формальной записью текущего выражения.
			Возвращает строку. ПОКА НЕ РАБОТАЕТ*/
			wstring expresion(int comma);
			
			/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и выполняющую их.
			Результатом работы метода является запись результата вычислений в double var текущего элемента класса. */
			void arithmetic();
			/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и очищающую память.
			Метод должен возвращать число ошибок при использовании delete. Это надо дописать
			*/
			int tree_destruct();
			/*Метод возвращаяет указатель на элемент списка с данным именем*/
			math_obj* find_by_name(math_obj*pointer);
			/*Метод возвращает указатель на последний элемент списка переменных.
			Если список уже замкнут - на элемент предшествующий нулевому (подобный запрос никогда не должен возникать).*/
			math_obj* var_list_back();
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
			/*Медод определения функции. Создаёт список переменных, от которых зависит функция, основываясь на списке переменных и аргументов функции pointer.*/
			void funct_define(math_obj *pointer);
			
			/*Метод возвращает указатель на элемент списка с номером index.*/
			math_obj* vector_at(int index);
			/*Метод возвращает количество элементов списка.*/
			int vector_size();
			/*Метод возвращает указатель на последний элемент вектора*/
			math_obj* vector_back();
			/*Метод добавляет элемент pointer в конец вектора. Полагается, что pointer - независимая копия какого либо элемента.
			-1 в случае ошибки*/
			int vector_push_back(math_obj*pointer);
			/*Метод добавляет элемент pointer в начало вектора. Полагается, что pointer - независимая копия какого либо элемента.
			-1 в случае ошибки*/
			int vector_push_foward(math_obj*pointer);
			/*Метод вставляет с заменой в вектор элемент на место index, при условии что index существует.
			-1 в случае ошибки*/
			int vector_assing_at(math_obj*pointer, int index);
			/*Метод добавляет элемент в конец вектора аргументов функции.
			-1 в случае ошибки*/
			int funct_arg_push_back(math_obj*pointer);
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
			void convert_to(wstring _type, wstring _prop, double _num, math_obj *_pc);
			/*Метод ПОЛНОСТЬЮ преобразует this с заданными параметрами (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник").
			Если текущий элемент - вектор, все элементы которого указывают на один и тот же объект, преобразуется так же объект*/
			void convert_totaly(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//Метод get. ТЕКУЩИЙ ЭЛЕМЕНТ
			math_obj*get_this();
			//Метод get. ИМЯ
			wstring get_name();
			//Метод get. ТИП
			wstring get_type();
			//Метод get. СВОЙСТВО
			wstring get_prop();
			//Метод get. ДЕЙСТВИЕ
			wstring get_actn();
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
			//Метод get. УКАЗАТЕЛЬ "воротник"
			math_obj* get_pc();
			/*Метод добавляет переменную в конец списка переменных данной функции.
			В том числе и для векторных/матричных.
			-1 в случае ошибки.*/
			int var_list_push_back(math_obj*pointer);

			wchar_t* brackets_close(wchar_t* brackets_open, wchar_t*end);

			enum class variable_type {
				INTEGER,
				FRACTIONAL,
				IMAGINARY,
				REAL,
				RE_IM,
				EXP	//5E+10 5*10^10
			};

			wstring name;
			wstring type;
			wstring prop;
			wstring actn;
			double var;
			//double arg; //может пригодится.
			//int exp;
			math_obj *point_left;		//левый рукав
			math_obj *point_right;		//правый рукав
			math_obj *point_collar;	//воротник
		};


	}
};
#endif //CLASSES_H
