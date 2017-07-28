#pragma once
#ifndef NUMBER_H
#define NUMBER_H
#include "classes.h"
namespace Project {
	namespace Core {
		using namespace std;
		class number :
			virtual public math_obj
		{
		protected:
			//Тип числа можно изменить.
			long double num;
			//data_list* point_up;  //наследуется (math_obj), по умолчанию не используется.
		public:

			number();
			number(long double _num);
			virtual ~number();

			//Метод get. ЧИСЛО
			virtual double get_num();
			//Метод assing. ЧИСЛО
			virtual void assing_num(long double _num);
			
			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			virtual math_obj* get_this();
		};
	}
}
#endif  // !NUMBER_H