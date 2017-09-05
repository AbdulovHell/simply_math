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

			//Метод get. ЭЛЕМЕНТ
			virtual void* get_this();

			//Метод get. ИМЯ
			virtual wstring get_name();//возможно вызов этого метода должен возвращать формальную запись выражения
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
#endif  // !EXPRESSION_H

