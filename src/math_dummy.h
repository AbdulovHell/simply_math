#pragma once
#ifndef MATH_DUNNY_H
#define MATH_DUNNY_H
#include "math_obj.h"
namespace Project {
	namespace Core {
		using namespace std;
		class math_dummy :
			public math_obj
		{
		protected:			
			uint32_t num_var;	//номер элемента в списке (векторе)
			leaf_ptr point_left;	//Указатель на "левый" элемент списка
			leaf_ptr point_right;	//Указатель на "правый" элемент списка
			leaf_ptr point_collar; //Указатель "наверх"
			
		public:
			math_dummy();
			math_dummy(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			math_dummy(uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			virtual ~math_dummy();

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

			//Метод get. Поправка для перечисляемых элементов
			uint32_t get_num_var();
			//Метод assing. Поправка для перечисляемых элементов
			void assing_num_var(unsigned int _num);

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
#endif //!MATH_DUNNY_H