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

			virtual ~expression();

			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. РАЗМЕР
			virtual size_t get_sizeof();
			
			//Метод get. УКАЗАТЕЛЬ "левый рукав"
			virtual leaf_ptr get_pl();
			//Метод assing. УКАЗАТЕЛЬ "левый рукав"
			virtual void assing_pl(leaf_ptr& _pointer);	

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);

		};



	}
}
#endif  // !EXPRESSION_H

