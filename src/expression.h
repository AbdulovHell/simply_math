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
			math_obj* point_left;//указатель на дерево операций для выражения.
			//data_list* point_up;  //наследуется (math_obj)
		public:
			expression();
			expression(math_obj* _pl);
			expression(math_obj* _pl);
			virtual ~expression();

			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			virtual math_obj* get_this();

			//Метод get. ИМЯ
			virtual wstring get_name();//возможно вызов этого метода должен возвращать формальную запись выражения
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
#endif  // !EXPRESSION_H

