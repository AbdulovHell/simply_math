#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "classes.h"
namespace Project {
	namespace Core {
		using namespace std;
		class expression :
			virtual public math_obj
		{
		protected:
			math_obj* point_left;//��������� �� ������ �������� ��� ���������.
			//data_list* point_up;  //����������� (math_obj)
		public:
			expression();
			expression(math_obj* _pl);
			expression(math_obj* _pl, data_list* _pu);
			virtual ~expression();

			//����� get. ���
			virtual flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ��������� "����� �����"
			virtual math_obj* get_pl();
			//����� assing. ��������� "����� �����"
			virtual void assing_pl(math_obj* _pointer);
		};



	}
}
#endif  // !EXPRESSION_H

