#pragma once
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "math_obj.h"
#include "number.h"
#include "error.h"
namespace Project {
	namespace Core {
		using namespace std;
		class operations :
			public math_obj	{
		protected:
			leaf_ptr point_left;	//����� �������
			leaf_ptr point_right;	//������ �������
			leaf_ptr point_collar; //��������� �� ���������� ��������	

			leaf_ptr prioritize_rec(int* _p);



		public:		
			
			operations();	
			operations(leaf_ptr _pl);
			operations(leaf_ptr _pl, leaf_ptr _pr);
			virtual ~operations();
			
			//����� get. ���
			virtual flags get_class_type();
			

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
			
			//���������� ��������� ���������� �������� � ���� ���. �������. 
			virtual math_obj* get_result()=0;
			
			bool define_operation(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this);

			leaf_ptr prioritize(int _priority);

			virtual int get_priority() = 0;
			
		};

		class addition :
			public operations {
		protected:
			//uint32_t point_left;		//����������� (operations), ����� �������
			//uint32_t point_right;	//����������� (operations), ������ �������
			//uint32_t point_collar;	//����������� (operations), ��������� �� ���������� ��������
		public:
			addition();
			addition(leaf_ptr _pl);
			addition(leaf_ptr _pl, leaf_ptr _pr);
			addition(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this) throw (...);

			~addition();
			
			flags get_class_type(); 

			virtual size_t get_sizeof();

			virtual math_obj* get_this();

			//����� get. �������
			//math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();			

			int get_priority();

			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);

		};

		class subtraction :
			public operations {
		protected:
			//uint32_t point_left;		//����������� (operations), ����� �������
			//uint32_t point_right;	//����������� (operations), ������ �������
			//uint32_t point_collar;	//����������� (operations), ��������� �� ���������� ��������

		public:
			subtraction();
			subtraction(leaf_ptr _pl);
			subtraction(leaf_ptr _pl, leaf_ptr _pr);
			subtraction(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc);
			subtraction(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this)throw (...);
			~subtraction();

			flags get_class_type();

			virtual size_t get_sizeof();

			virtual math_obj* get_this();

			//����� get. �������
			//virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();			

			int get_priority();

			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};

		class multiplication :
			public operations {
		protected:
			//uint32_t point_left;		//����������� (operations), ����� �������
			//uint32_t point_right;	//����������� (operations), ������ �������
			//uint32_t point_collar;	//����������� (operations), ��������� �� ���������� ��������
		public:
			multiplication();
			multiplication(leaf_ptr _pl);
			multiplication(leaf_ptr _pl, leaf_ptr _pr);
			multiplication(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this)throw (...);
			~multiplication();
						
			flags get_class_type();

			virtual size_t get_sizeof();

			virtual math_obj* get_this();

			//����� get. �������
			//virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};		

		class division :
			public operations {
		protected:
			//uint32_t point_left;		//����������� (operations), ����� �������
			//uint32_t point_right;	//����������� (operations), ������ �������
			//uint32_t point_collar;	//����������� (operations), ��������� �� ���������� ��������
		public:
			division();
			division(leaf_ptr _pl);
			division(leaf_ptr _pl, leaf_ptr _pr);
			division(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this)throw (...);
			~division();
			
			flags get_class_type();

			virtual size_t get_sizeof();

			virtual math_obj* get_this();

			//����� get. �������
			//virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};	

		class power :
			public operations {
		protected:
			//uint32_t point_left;		//����������� (operations), ����� �������
			//uint32_t point_right;	//����������� (operations), ������ �������
			//uint32_t point_collar;	//����������� (operations), ��������� �� ���������� ��������
		public:
			power();
			power(leaf_ptr _pl);
			power(leaf_ptr _pl, leaf_ptr _pr);
			power(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this) throw (...);
			~power();
			
			flags get_class_type();

			virtual size_t get_sizeof();

			virtual math_obj* get_this();

			//����� get. �������
			//virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};
	}
}

#endif // !OPERATIONS_H




