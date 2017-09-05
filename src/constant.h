#pragma once
#ifndef CONSTANT_H
#define CONSTANT_H
#include "number.h"
#include "expression.h"
namespace Project {
	namespace Core {
		using namespace std;
		class constant :
			virtual public number, virtual public expression
		{			
		protected:
			wstring name;//содержит имя константы			
			//long double num;		//наследуется (number).
			//uint32_t point_left;	//наследуется (expression).
		public:
			constant();
			constant(leaf_ptr _pl);
			constant(long double _num);
			constant(wstring _name);
			constant(leaf_ptr _pl, long double _num, wstring _name);
			virtual ~constant();

			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. РАЗМЕР
			virtual size_t get_sizeof();

			//Метод get. ЭЛЕМЕНТ
			virtual void* get_this();

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
#endif // !CONSTANT_H
