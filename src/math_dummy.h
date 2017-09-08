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
			uint32_t num_var;	//����� �������� � ������ (�������)
			leaf_ptr point_left;	//��������� �� "�����" ������� ������
			leaf_ptr point_right;	//��������� �� "������" ������� ������
			leaf_ptr point_collar; //��������� "������"
			
		public:
			math_dummy();
			math_dummy(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			math_dummy(uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			virtual ~math_dummy();

			//����� get. ���
			virtual flags get_class_type();

			virtual size_t get_sizeof();

			virtual math_obj* get_this();
			
			//����� get. �����
			virtual long double get_num();
			//����� assing. �����
			virtual void assing_num(long double _num);

			//����� get. �������� ��� ������������� ���������
			uint32_t get_num_var();
			//����� assing. �������� ��� ������������� ���������
			void assing_num_var(unsigned int _num);

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
			
			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};
	}
}
#endif //!MATH_DUNNY_H