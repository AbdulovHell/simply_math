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
		class data_list;
		class math_obj;

		enum class flags : int16_t
		{
			//типы мат. объектов
			error = -10,
			math_dummy = -2,
			variable =-1,

			base = 0,

			number = 1,
			complex = 2,
			//qwauterion = 3

			constant = 10,
			constant_cmplx = 11,

			expression = 20,
			function = 21, //undef
			funct_defnd = 22,
			funct_arg_c = 23,
			funct_arg_v = 24,


			equation = 30,

			vector = 40,

			matrix = 50,

			operation = 60,

			addit = 61,
			minus = 62,
			mltpl = 63,
			divis = 64,
			power = 65,				
			

		};


		
		class math_obj {
		private:			
		protected://базовый класс содежит только указатель на элемент общего списка данных
			data_list* point_up;			
		public:		

			//Нулевой конструктор
			math_obj();			

			virtual ~math_obj();

			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			virtual math_obj* get_this();

			//Метод get. ИМЯ
			virtual wstring get_name();
			//Метод assing. ИМЯ
			virtual void assing_name(wstring _name);

			//Метод get. ЧИСЛО
			virtual double get_num();
			//Метод assing. ЧИСЛО
			virtual void assing_num(long double _num);

			//Метод get. УКАЗАТЕЛЬ "левый рукав"
			virtual math_obj* get_pl();
			//Метод assing. УКАЗАТЕЛЬ "левый рукав"
			virtual void assing_pl(math_obj* _pointer);


			//Метод get. УКАЗАТЕЛЬ "правый рукав"
			virtual math_obj* get_pr();
			//Метод assing. УКАЗАТЕЛЬ "правый рукав"
			virtual void assing_pr(math_obj* _pointer);


			//Метод get. УКАЗАТЕЛЬ "воротник"
			virtual math_obj* get_pc();
			//Метод assing. УКАЗАТЕЛЬ "воротник"
			virtual void assing_pc(math_obj* _pointer);

			//Метод get. УКАЗАТЕЛЬ "вверх"
			virtual data_list* get_pu();
			//Метод assing. УКАЗАТЕЛЬ "вверх"
			virtual void assing_pu(data_list* _pointer);

		};





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
	}
};
#endif //CLASSES_H
