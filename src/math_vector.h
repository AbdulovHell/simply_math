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
			//unsigned int num_var;		//����������� (math_dummy). ����� ��������� �������
			//math_obj* point_left;		//����������� (math_dummy). 
			//math_obj* point_right;	//����������� (math_dummy).
			//math_obj* point_collar;	//����������� (math_dummy). ��������� "������"
			//data_list* point_up;		//����������� (math_obj).

		public:
			math_vector();
			virtual ~math_vector();
		};
	}
}
#endif //MATH_VECTOR_H