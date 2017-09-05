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
			
		public:

			number();
			number(long double _num);
			virtual ~number();

			//����� get. ���
			virtual flags get_class_type();

			//����� get. �������
			virtual void* get_this();

			//����� get. ������
			virtual size_t get_sizeof();

			//����� get. ���
			virtual wstring get_name();
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
#endif  // !NUMBER_H