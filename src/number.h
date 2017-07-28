#pragma once
#ifndef NUMBER_H
#define NUMBER_H
#include "math_obj.h"
namespace Project {
	namespace Core {
		using namespace std;
		class number :
			virtual public math_obj
		{
		protected:
			//��� ����� ����� ��������.
			long double num;
			//data_list* point_up;  //����������� (math_obj), �� ��������� �� ������������.
		public:

			number();
			number(long double _num);
			virtual ~number();

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
#endif  // !NUMBER_H