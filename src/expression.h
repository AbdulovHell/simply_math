#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "classes.h"
namespace Project {
	namespace Core {
		using namespace std;
		class expression :
			virtual public math_obj
		{
		protected:
			math_obj* point_left;//указатель на дерево операций для выражения.
			//data_list* point_up;  //наследуется (math_obj)
		public:
			expression();
			expression(math_obj* _pl);
			expression(math_obj* _pl, data_list* _pu);
			virtual ~expression();

			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			virtual math_obj* get_this();

			//Метод get. УКАЗАТЕЛЬ "левый рукав"
			virtual math_obj* get_pl();
			//Метод assing. УКАЗАТЕЛЬ "левый рукав"
			virtual void assing_pl(math_obj* _pointer);
		};



	}
}
#endif  // !EXPRESSION_H

