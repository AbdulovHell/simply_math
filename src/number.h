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
			//data_list* point_up;  //наследуется (math_obj), по умолчанию не используется.
		public:

			number();
			number(long double _num);
			virtual ~number();

			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			virtual math_obj* get_this();

			//Метод get. ИМЯ
			virtual wstring get_name();
			//Метод assing. ИМЯ
			virtual void assing_name(wstring _name);

			//Метод get. ЧИСЛО
			virtual long double get_num();
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
		};
	}
}
#endif  // !NUMBER_H