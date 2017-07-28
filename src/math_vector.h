#pragma once
#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H
#include "math_dummy.h"

namespace Project {
	namespace Core {
		using namespace std;
		class math_vector :
			public math_dummy
		{
		protected:
			//unsigned int num_var;		//наследуется (math_dummy). число элементов вектора
			//math_obj* point_left;		//наследуется (math_dummy). 
			//math_obj* point_right;	//наследуется (math_dummy).
			//math_obj* point_collar;	//наследуется (math_dummy). Указатель "наверх"
			//data_list* point_up;		//наследуется (math_obj).

		public:
			math_vector();
			virtual ~math_vector();
		};
	}
}
#endif //MATH_VECTOR_H