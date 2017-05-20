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

#define cnst L"const"
#define funct  L"funct"
#define varbl  L"varbl"
#define equat  L"equat"
#define exprs  L"exprs"
#define numbr  L"numbr"
#define addit  L"oper+"
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

#define vectr_arg_c L"v_a_c" //свойство векторной функции с константными аргументами
#define vectr_arg_v L"v_a_v"//свойство векторной функции с переменными аргументами

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

		class math_obj {
		private:
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
			/*PRIVATE. Метод рекурсивно создаёт связный список указанной длинны size*/
			math_obj* vector_create(int size);
			/*PRIVATE. Метод рекурсивно копирует список переменных в вектор.
			-1 при ошибке*/
			int var_list_copy_to_vector(math_obj* pointer, math_obj* vect);
			/*PRIVATE. Нумерация переменных по порядку.*/
			void var_list_number(math_obj* pointer);
			/*PRIVATE. Установка указателей point_collar для НЕЗАМКНУТОГО списка переменных на функцию*/
			void var_list_collar(math_obj* pointer, math_obj*original);
			/*PRIVATE. Метод возвращает указатель на аргумент соответствующий найденной переменной*/
			math_obj* get_arg_for_var(math_obj*pointer, math_obj*arg);
			/*PRIVATE. Деструктор вектора*/
			int vector_destruct(math_obj* pointer);
			//math_obj *var_list_to_arg(math_obj*var_list, int size);

			//math_obj *arg_to_var_list(math_obj*mass_arg, math_obj*var_list, int size);

			

		public:
			//Нулевой конструктор
			math_obj();
			//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double) 
			math_obj(wstring _name, double _num);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, ЧИСЛО типа double) 
			math_obj(wstring _name, wstring _type, double _num);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double) 
			math_obj(wstring _name, wstring _type, wstring _prop, double _num);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double) 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num);
			//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, double _num, math_obj *_pc);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, wstring _type, double _num, math_obj *_pc);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj *_pc);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj *_pc);
			//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав") 
			math_obj(wstring _name, double _num, math_obj * _pl, math_obj *_pr);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав") 
			math_obj(wstring _name, wstring _type, double _num, math_obj * _pl, math_obj *_pr);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав") 
			math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj * _pl, math_obj *_pr);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав") 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr);
			//Конструктор записывает (ИМЯ объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, wstring _type, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//Конструктор записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double, УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);			
			//Конструктор копирования
			math_obj(math_obj* var1);
			//Конструктор вектора
			math_obj(int size_n);
			//Конструктор вектора с именем
			math_obj(wstring _name, int size_n);
			//Конструктор вектора по строке данных. Вектором полагается любая запись вида (..,..) (хотя бы одна запятая). begin и end должны указывать на открывающую и закрывающую скобку.
			/*math_obj(int size_n, wchar_t* begin, wchar_t * end);*/
			//Конструктор матрицы
			math_obj(int size_n, int size_m);
			//Деструктор TODO: зачем он, если в нем пусто..., можно сюда запихнуть функционал tree_destrust, когда её отладим.
			~math_obj();
			//Метод копирования.
			void copy(math_obj* ref);
			/*Метод возвращает приоритет операции.
			1 - сложение
			2 - умножение
			3 - деление
			4 - возведение в степень
			0 - не операция*/
			int get_priority();
			/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и коструирующую строку с формальной записью текущего выражения.
			Возвращает строку. ПОКА НЕ РАБОТАЕТ*/
			wstring expresion(int comma);
			/*Метод вызывает рекурсивную функцию проверки приоритета операций в текущем дереве операций.
			Возвращает указатель на элемент дерева (операцию), имеющий приоритет меньший или равный в сравнении с текущим элементом. */
			math_obj* prioritize(int current_priority);
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
			/*Метод сравнивает список аргументов (хотя бы один из которых переменная) функции pointer со списком переменных данной функции.
			Метод возвращает:
			0 - список переменных pointer полностью входит в список переменных данной функции.
			любое другое положительное целое - число различий списков функций.
			*/
			int var_list_compare(math_obj * pointer);
			/*Метод собирает список переменных данной функции по спискам переменных функций, участвующих в её записи*/
			math_obj *create_var_list(math_obj* pointer, math_obj *var_list);

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
			/*Метод добавляет элемент в конец вектора.
			-1 в случае ошибки*/
			int vector_push_back(math_obj*pointer);
			/*Метод добавляет элемент в начало вектора.
			-1 в случае ошибки*/
			int vector_push_foward(math_obj*pointer);
			/*Метод добавляет в вектор элемент на место index, при условии что index существует.
			-1 в случае ошибки*/
			int vector_assing_at(math_obj*pointer, int index);
			/*Метод добавляет элемент в конец вектора аргументов функции.
			-1 в случае ошибки*/
			int funct_arg_push_back(math_obj*pointer);
			/*Метод преобразует односвязный список в двусвязный*/
			void math_obj::double_lincked_vector();
			//Метод вызывает рекурсивную функцию установки указателей point_collar для НЕЗАМКНУТОГО списка переменных на текущий экземпляр класса
			void link_varbls_to_funct();
			//Метод замыкает список переменных в кольцо.
			void close_list();

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
