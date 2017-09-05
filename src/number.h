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

			//Метод get. ЭЛЕМЕНТ
			virtual void* get_this();

			//Метод get. РАЗМЕР
			virtual size_t get_sizeof();

			//Метод get. ИМЯ
			virtual wstring get_name();
			//Метод assing. ИМЯ
			virtual void assing_name(wstring _name);

			//Метод get. ЧИСЛО
			virtual long double get_num();
			//Метод assing. ЧИСЛО
			virtual void assing_num(long double _num);

			//Метод get. УКАЗАТЕЛЬ "левый рукав"
			virtual leaf_ptr get_pl();
			//Метод assing. УКАЗАТЕЛЬ "левый рукав"
			virtual void assing_pl(leaf_ptr& _pointer);


			//Метод get. УКАЗАТЕЛЬ "правый рукав"
			virtual leaf_ptr get_pr();
			//Метод assing. УКАЗАТЕЛЬ "правый рукав"
			virtual void assing_pr(leaf_ptr _pointer);


			//Метод get. УКАЗАТЕЛЬ "воротник"
			virtual leaf_ptr get_pc();
			//Метод assing. УКАЗАТЕЛЬ "воротник"
			virtual void assing_pc(leaf_ptr _pointer);

			virtual int get_priority();


			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};
	}
}
#endif  // !NUMBER_H