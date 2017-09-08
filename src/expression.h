#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "math_obj.h"
namespace Project {
	namespace Core {
		using namespace std;
		class expression :
			virtual public math_obj
		{
		protected:
			leaf_ptr point_left;//указатель (относительный) на дерево операций для выражения.			
		public:
			expression();
			expression(leaf_ptr _pl);
			expression(expression&);

			~expression();

			//Метод get. ТИП
			flags get_class_type();

			virtual size_t get_sizeof();

			math_obj* get_this();
						
			//Метод get. ЧИСЛО
			long double get_num();
			//Метод assing. ЧИСЛО
			void assing_num(long double _num);

			//Метод get. УКАЗАТЕЛЬ "левый рукав"
			leaf_ptr get_pl();
			//Метод assing. УКАЗАТЕЛЬ "левый рукав"
			void assing_pl(leaf_ptr& _pointer);
			
			uint16_t copy_to(void * _ptr);

			math_obj* copy(math_obj* _original);
		};



	}
}
#endif  // !EXPRESSION_H

