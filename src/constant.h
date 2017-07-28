#pragma once
#ifndef CONSTANT_H
#define CONSTANT_H
#include "number.h"
#include "expression.h"
namespace Project {
	namespace Core {
		using namespace std;
		class constant :
			public number, public expression
		{			
		protected:
			wstring name;//содержит имя константы
			//data_list* point_up;  //наследуется (math_obj).
			//long double num;		//наследуется (number).
			//math_obj* point_left;	//наследуется (expression).
		public:
			constant();
			constant(math_obj* _pl);
			constant(long double _num);
			constant(wstring _name);
			constant(math_obj* _pl, long double _num, wstring _name);
			constant(math_obj* _pl, long double _num, wstring _name, data_list* _pu);
			virtual ~constant();

			//Метод get. ТИП
			virtual flags get_class_type();


		};
	}
}
#endif // !CONSTANT_H
