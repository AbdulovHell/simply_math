#pragma once
#ifndef MATH_DUNNY_H
#define MATH_DUNNY_H
#include "math_obj.h"
namespace Project {
	namespace Core {
		using namespace std;
		class math_dummy :
			public math_obj
		{
		protected:			
			unsigned int num_var;	//����� �������� � ������ (�������)
			math_obj* point_left;	//��������� �� "�����" ������� ������
			math_obj* point_right;	//��������� �� "������" ������� ������
			math_obj* point_collar; //��������� "������"
			//data_list* point_up;  //����������� (math_obj).
		public:
			math_dummy();
			math_dummy(math_obj* _pl, math_obj* _pr, math_obj* _pc);
			math_dummy(unsigned int _num, math_obj* _pl, math_obj* _pr, math_obj* _pc);
			virtual ~math_dummy();

			//����� get. ���
			virtual flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ���
			virtual wstring get_name();
			//����� assing. ���
			virtual void assing_name(wstring _name);

			//����� get. �����
			virtual long double get_num();
			//����� assing. �����
			virtual void assing_num(long double _num);

			//����� get. �������� ��� ������������� ���������
			unsigned int get_num_var();
			//����� assing. �������� ��� ������������� ���������
			void assing_num_var(unsigned int _num);

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
#endif //!MATH_DUNNY_H