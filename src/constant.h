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

			virtual size_t get_sizeof();

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
			virtual leaf_ptr get_pl();
			//Метод assing. УКАЗАТЕЛЬ "левый рукав"
			virtual void assing_pl(leaf_ptr& _pointer);
			
			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);


		};
	}
}
#endif // !CONSTANT_H
