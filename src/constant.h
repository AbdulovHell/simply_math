#pragma once
#ifndef CONSTANT_H
#define CONSTANT_H
#include "number.h"
#include "expression.h"
namespace Project {
	namespace Core {
		using namespace std;
		class constant :
			virtual public number, virtual public expression
		{			
		protected:
			wstring name;//�������� ��� ���������			
			//long double num;		//����������� (number).
			//uint32_t point_left;	//����������� (expression).
		public:
			constant();
			constant(leaf_ptr _pl);
			constant(long double _num);
			constant(wstring _name);
			constant(leaf_ptr _pl, long double _num, wstring _name);
			virtual ~constant();

			//����� get. ���
			virtual flags get_class_type();

			//����� get. ������
			virtual size_t get_sizeof();

			//����� get. �������
			virtual void* get_this();

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
#endif // !CONSTANT_H
