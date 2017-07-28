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
			unsigned int num_var;	//номер элемента в списке (векторе)
			math_obj* point_left;	//Указатель на "левый" элемент списка
			math_obj* point_right;	//Указатель на "правый" элемент списка
			math_obj* point_collar; //Указатель "наверх"
			//data_list* point_up;  //наследуется (math_obj).
		public:
			math_dummy();
			math_dummy(math_obj* _pl, math_obj* _pr, math_obj* _pc);
			math_dummy(unsigned int _num, math_obj* _pl, math_obj* _pr, math_obj* _pc);
			virtual ~math_dummy();

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

			//Метод get. Поправка для перечисляемых элементов
			unsigned int get_num_var();
			//Метод assing. Поправка для перечисляемых элементов
			void assing_num_var(unsigned int _num);

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
#endif //!MATH_DUNNY_H