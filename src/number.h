#pragma once
#ifndef NUMBER_H
#define NUMBER_H
#include "math_obj.h"
namespace Project {
	namespace Core {
		using namespace std;
		class number :
			virtual public math_obj
		{
		protected:
			//Тип числа можно изменить.
			long double num;
			
		public:

			number() : math_obj() { num = 0; }
			number(long double _num) : math_obj() { num = _num; }
			
			//Метод get. ТИП
			flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			void* get_this();

			//Метод get. РАЗМЕР
			size_t get_sizeof();

			//Метод get. ЧИСЛО
			long double get_num();
			//Метод assing. ЧИСЛО
			void assing_num(long double _num);

			int get_priority();

			void copy_to(void * _ptr);

			bool copy(number* _original);
		};
	}
}
#endif  // !NUMBER_H