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

			number();
			number(long double _num);
			virtual ~number();

			//Метод get. ТИП
			virtual flags get_class_type();			

			virtual size_t get_sizeof();

			virtual math_obj* get_this();

			//Метод get. ЧИСЛО
			virtual long double get_num();
			//Метод assing. ЧИСЛО
			virtual void assing_num(long double _num);
			
			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};
	}
}
#endif  // !NUMBER_H