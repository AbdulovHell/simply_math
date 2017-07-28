#pragma once
#ifndef MATH_OBJ_H
#define MATH_OBJ_H

#include "common.h"



namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;	
		
		class math_obj {
		private:			
		protected://базовый класс не содержит ничего
						
		public:		

			//Нулевой конструктор
			math_obj() {};

			virtual ~math_obj() {};

			//Метод get. ТИП
			virtual flags get_class_type() {
				return flags::base;
			};

			//Метод get. ЭЛЕМЕНТ
			virtual math_obj* get_this() =0;

			//Метод get. ИМЯ
			virtual wstring get_name() = 0;
			//Метод assing. ИМЯ
			virtual void assing_name(wstring _name) = 0;

			//Метод get. ЧИСЛО
			virtual long double get_num() = 0;
			//Метод assing. ЧИСЛО
			virtual void assing_num(long double _num) = 0;

			//Метод get. УКАЗАТЕЛЬ "левый рукав"
			virtual math_obj* get_pl() = 0;
			//Метод assing. УКАЗАТЕЛЬ "левый рукав"
			virtual void assing_pl(math_obj* _pointer) = 0;


			//Метод get. УКАЗАТЕЛЬ "правый рукав"
			virtual math_obj* get_pr() = 0;
			//Метод assing. УКАЗАТЕЛЬ "правый рукав"
			virtual void assing_pr(math_obj* _pointer) = 0;


			//Метод get. УКАЗАТЕЛЬ "воротник"
			virtual math_obj* get_pc() = 0;
			//Метод assing. УКАЗАТЕЛЬ "воротник"
			virtual void assing_pc(math_obj* _pointer) = 0;
			
		};
				
	}
};
#endif //MATH_OBJ_H
