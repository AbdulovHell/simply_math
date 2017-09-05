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
			//unsigned int num_var;		//наследуетс€ (math_dummy). число элементов вектора
			//math_obj* point_left;		//наследуетс€ (math_dummy). 
			//math_obj* point_right;	//наследуетс€ (math_dummy).
			//math_obj* point_collar;	//наследуетс€ (math_dummy). ”казатель "наверх"			

		public:
			math_vector();
			virtual ~math_vector();
		};
	}
}
#endif //MATH_VECTOR_H