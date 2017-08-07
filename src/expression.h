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
			math_obj* point_left;//��������� �� ������ �������� ��� ���������.			
		public:
			expression();
			expression(math_obj* _pl);
			expression(math_obj* _pl);
			virtual ~expression();

			//����� get. ���
			virtual flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ���
			virtual wstring get_name();//�������� ����� ����� ������ ������ ���������� ���������� ������ ���������
			//����� assing. ���
			virtual void assing_name(wstring _name);

			//����� get. �����
			virtual long double get_num();
			//����� assing. �����
			virtual void assing_num(long double _num);

			//����� get. ��������� "����� �����"
			virtual math_obj* get_pl();
			//����� assing. ��������� "����� �����"
			virtual void assing_pl(math_obj* _pointer);


			//����� get. ��������� "������ �����"
			virtual math_obj* get_pr();
			//����� assing. ��������� "������ �����"
			virtual void assing_pr(math_obj* _pointer);


			//����� get. ��������� "��������"
			virtual math_obj* get_pc();
			//����� assing. ��������� "��������"
			virtual void assing_pc(math_obj* _pointer);
		};



	}
}
#endif  // !EXPRESSION_H

