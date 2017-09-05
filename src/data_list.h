#pragma once
#ifndef DATA_LIST_H
#define DATA_LIST_H

#include "common.h"
#include "IO.h"
#include "error.h"

#include "Common_math_obj.h"


namespace Project {
	
	class data_list {
	private:
		//friend struct Project::data_list::iterator;
		//Рекурсия для end
		data_list*back_rec();
		
		//Рекурсия для метода at
		data_list*at_rec(int* place, int * count);

		//Рекурсия для методов size
		int size_rec(bool* flag);

		/*Рекурсия для delete_starting_at*/
		int delete_starting_at_rec();

		bool implace_p(data_list* _obj);

		Core::leaf_ptr build_iternal( wchar_t * _str, wchar_t * _end) throw(...);

		Core::leaf_ptr& build_tree(wchar_t* _str, wchar_t* _end, Core::leaf_ptr  _current_obj)throw(...);

	public:		
		wstring in;
		wstring out;
		data_list* left;
		data_list* right;
		Core::tree_ptr object;

		struct __iterator {
		private:
			friend class data_list;
			data_list * letter;			
		public:
			__iterator();
			//дублируется часть методов data_list  с перенаправлением вызова через внутренний указатель letter
			bool implace_after_this(data_list*pointer);
			int compare_in(wstring *original);
			int compare_out(wstring *original);
			Core::tree_ptr find_math_obj(wstring* name);
			int delete_after_this();
			bool build();

			wstring get_in();
			wstring get_out();
			Core::tree_ptr get_obj();
			void assing_in(wstring & _in);
			void assing_out(wstring & _in);
			void assing_obj(Core::math_obj * _obj);

			__iterator &operator= (const __iterator _right);
			__iterator &operator= (data_list* _right);
			//постфиксный
			__iterator &operator++ (int);
			//префиксный
			__iterator &operator++ ();
			//постфиксный
			__iterator &operator-- (int);
			//префиксный
			__iterator &operator-- ();
			bool operator== (const __iterator _right);
			bool operator== (const data_list* _right);
			bool operator!= (const __iterator _right);
			bool operator!= (const data_list* _right);

			operator data_list * () const;
		};

		typedef __iterator iterator;

		//Нулевой конструктор
		data_list();
		//Коструктор для строкой ввода in
		data_list(wstring* _in);
		data_list(wstring* _in, data_list* _start);
		//Коструктор для строкой ввода in, мат. объекта object
		data_list(wstring _in, Core::tree_ptr _math);

		/*Метод вставляет элемент pointer в конец списка данных.
		0 - в случае ошибки (номера place не существует)
		1 - в случае успешного выполнения*/
		bool push_back(data_list* pointer);

		/*Метод вставляет элемент pointer в список данных до нулевого элемента. Индекс присваивается равным -1.
		Метод возвращает:
		0 - в случае ошибки
		1 - в случае успешного выполнения*/
		bool push_left(Core::tree_ptr pointer);

		/*Метод возвращает указатель на "стартовый" элемент списка данных.
		NULL - в случае ошибки (например, попытки поиска слева от нуля*/
		data_list*begin();

		__iterator front();

		__iterator back();

		/*Метод вставляет элемент pointer в список данных после номера place. Индексы следующих элементов списка сдвигаются на 1.
		Метод возвращает:
		0 - в случае ошибки (номера place не существует)
		1 - в случае успешного выполнения*/
		bool implace(int place, data_list*pointer);

		/*Метод возвращает указатель на элемент списка с номером place.
		NULL - номера place нет в списке или он меньше нуля, и в случае вызова не для начального (нулевого) элемента*/
		data_list* at(int place);

		/*Метод возвращает указатель на последний элемент списка данных. Если ничего не было записано - указатель на нулевой элемент.
		NULL - в случае вызова не для начального (нулевого) элемента*/
		data_list* end();

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
		Core::tree_ptr find_math_obj(wstring* name);

		/*Метод вызыват конструктор математического объекта по строке in. Тут же происходит проверка ввода.
		Метод возвращает:
		0 - в случае ошибки, как при заполнении, так и при проверке ввода (текст ошибки в поле out).
		1 - в случае успешного выполнения*/
		bool run_string();

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

		bool build();

		


	};

}
#endif //DATA_LIST_H