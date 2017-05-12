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
		//свойства обЪектов (prop)		
#define defnd  L"defnd"  //общее свойство
#define undef  L"undef"  //общее свойство

#define arg_c  L"arg_c" //функция с константными аргументами
#define arg_v  L"arg_v"  //функция с переменными аргументами 
//#define empty  L"empty"  //"пустая" функция. Свойство именных неинициализированных функций

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
			//описание
			int tree_destruct_processing(math_obj* pointer);
			//описание
			math_obj *prioritize_processing(math_obj *pc, int current_priority);
			//описание
			double arithmetic_processing(math_obj *pointer, math_obj * last_arg);
			//надо исправить
			wstring expression_processing(math_obj *pointer, int* comma);

			math_obj *var_list_back_processing(math_obj *pointer);

			math_obj *find_varbl_processing(math_obj*pointer, math_obj *original);

			math_obj *merge_lists(math_obj* list_a, math_obj* list_b);

			void reassing_right_pointers(math_obj* pointer);

			math_obj *var_list_to_arg(math_obj*var_list, int size);

			math_obj *arg_to_var_list(math_obj*mass_arg, math_obj*var_list, int size);

			math_obj *create_var_list(math_obj* pointer, math_obj *var_list);

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
			//Полный конструктор. Записывает (ИМЯ объекта, ТИП объекта, СВОЙСТВО объекта, ДЕЙСТВИЕ над объектом (указание), ЧИСЛО типа double (модуль), ЧИСЛО типа double (аргумент), ЧИСЛО типа int (степень 10), УКАЗАТЕЛЬ "левый рукав", УКАЗАТЕЛЬ "правый рукав", УКАЗАТЕЛЬ "воротник") 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, double _arg, double _exp, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//Конструктор копирования
			math_obj(math_obj* var1);
			//Деструктор TODO: зачем он, если в нем пусто...
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
			//
			math_obj* find_varbl(math_obj*pointer);
			/*Метод возвращает указатель на последний элемент списка переменных.
			Если список уже замкнут - на элемент предшествующий нулевому (подобный запрос никогда не должен возникать).*/
			math_obj* var_list_back();
			/*Метод сравнивает список аргументов (хотя бы один из которых переменная) функции pointer со списком переменных данной функции.
			Метод возвращает:
			0 - список переменных pointer полностью входит в список переменных данной функции.
			любое другое положительное целое - число различий списков функций.
			*/
			int var_list_compare(math_obj * pointer);
			/*Метод присваивает point_right данной функции массив аргументов с пустым местом в конце и увеличивает число переменных функции.
			Работает ТОЛЬКО для функций с незамкнутым списком переменных. Предыдущий массив удаляется
			*/
			void arg_copy_plus_1();
			/*Метод сортирует незамкнутый список переменных по алфавиту*/
			math_obj * sort_list(math_obj * var_list);
			/*Медод определения функции. Создаёт список переменных, от которых зависит функция, основываясь на списке переменных и аргументов функции pointer.*/
			void define_funct(math_obj *pointer);

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
			double arg; //может пригодится.
			int exp;
			math_obj *point_left;		//левый рукав
			math_obj *point_right;		//правый рукав
			math_obj *point_collar;	//воротник
		};


	}
};
#endif //CLASSES_H
