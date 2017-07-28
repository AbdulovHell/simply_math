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
			wstring name;//�������� ��� ���������
			//data_list* point_up;  //����������� (math_obj).
			//long double num;		//����������� (number).
			//math_obj* point_left;	//����������� (expression).
		public:
			constant();
			constant(math_obj* _pl);
			constant(long double _num);
			constant(wstring _name);
			constant(math_obj* _pl, long double _num, wstring _name);
			constant(math_obj* _pl, long double _num, wstring _name, data_list* _pu);
			virtual ~constant();

			//����� get. ���
			virtual flags get_class_type();


		};
	}
}
#endif // !CONSTANT_H
