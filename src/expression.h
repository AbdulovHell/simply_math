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
			leaf_ptr point_left;//��������� (�������������) �� ������ �������� ��� ���������.			
		public:
			expression();
			expression(leaf_ptr _pl);

			virtual ~expression();

			//����� get. ���
			virtual flags get_class_type();

			//����� get. ������
			virtual size_t get_sizeof();
			
			//����� get. ��������� "����� �����"
			virtual leaf_ptr get_pl();
			//����� assing. ��������� "����� �����"
			virtual void assing_pl(leaf_ptr& _pointer);	

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);

		};



	}
}
#endif  // !EXPRESSION_H

