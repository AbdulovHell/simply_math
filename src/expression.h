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

			//����� get. �������
			virtual void* get_this();

			//����� get. ���
			virtual wstring get_name();//�������� ����� ����� ������ ������ ���������� ���������� ������ ���������
			//����� assing. ���
			virtual void assing_name(wstring _name);

			//����� get. �����
			virtual long double get_num();
			//����� assing. �����
			virtual void assing_num(long double _num);

			//����� get. ��������� "����� �����"
			virtual leaf_ptr get_pl();
			//����� assing. ��������� "����� �����"
			virtual void assing_pl(leaf_ptr& _pointer);


			//����� get. ��������� "������ �����"
			virtual leaf_ptr get_pr();
			//����� assing. ��������� "������ �����"
			virtual void assing_pr(leaf_ptr _pointer);


			//����� get. ��������� "��������"
			virtual leaf_ptr get_pc();
			//����� assing. ��������� "��������"
			virtual void assing_pc(leaf_ptr _pointer);

			virtual int get_priority();

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);

		};



	}
}
#endif  // !EXPRESSION_H

