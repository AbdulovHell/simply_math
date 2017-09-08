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
			expression(expression&);

			~expression();

			//����� get. ���
			flags get_class_type();

			virtual size_t get_sizeof();

			math_obj* get_this();
						
			//����� get. �����
			long double get_num();
			//����� assing. �����
			void assing_num(long double _num);

			//����� get. ��������� "����� �����"
			leaf_ptr get_pl();
			//����� assing. ��������� "����� �����"
			void assing_pl(leaf_ptr& _pointer);
			
			uint16_t copy_to(void * _ptr);

			math_obj* copy(math_obj* _original);
		};



	}
}
#endif  // !EXPRESSION_H

