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
			math_obj* point_left;	//����� �������
			math_obj* point_right;	//������ �������
			math_obj* point_collar; //��������� �� ���������� ��������	

			operations* prioritize_rec(int* _p);



		public:		
			
			operations();	
			operations(math_obj* _pl);
			operations(math_obj * _pl, math_obj * _pr);			
			virtual ~operations();
			
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
			
			//���������� ��������� ���������� �������� � ���� ���. �������. 
			virtual math_obj* get_result()=0;

			
			bool define_operation(operations* _high, math_obj* _low, math_obj* _obj);

			operations* prioritize(int _priority);			

			virtual int get_priority() = 0;
		};

		class addition :
			public operations {
		protected:
			//math_obj* point_left;		//����������� (operations), ����� �������
			//math_obj* point_right;	//����������� (operations), ������ �������
			//math_obj* point_collar;	//����������� (operations), ��������� �� ���������� ��������
		public:
			addition();
			addition(math_obj* _pl);
			addition(math_obj * _pl, math_obj * _pr);
			addition(operations * _high, math_obj * _low, math_obj * _obj) throw (ProjectError::ErrorCode);

			~addition();
			
			flags get_class_type(); 

			//����� get. �������
			math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();			

			int get_priority();

		};

		class subtraction :
			public operations {
		protected:
			//math_obj* point_left;		//����������� (operations), ����� �������
			//math_obj* point_right;	//����������� (operations), ������ �������
			//math_obj* point_collar;	//����������� (operations), ��������� �� ���������� ��������

		public:
			subtraction();
			subtraction(math_obj* _pl);
			subtraction(math_obj * _pl, math_obj * _pr);
			subtraction(operations * _high, math_obj * _low, math_obj * _obj)throw (ProjectError::ErrorCode);
			~subtraction();

			flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();			

			int get_priority();
		};

		class multiplication :
			public operations {
		protected:
			//math_obj* point_left;		//����������� (operations), ����� �������
			//math_obj* point_right;	//����������� (operations), ������ �������
			//math_obj* point_collar;	//����������� (operations), ��������� �� ���������� ��������
		public:
			multiplication();
			multiplication(math_obj* _pl);
			multiplication(math_obj * _pl, math_obj * _pr);
			multiplication(operations * _high, math_obj * _low, math_obj * _obj)throw (ProjectError::ErrorCode);
			~multiplication();
						
			flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();

			int get_priority();
		};		

		class division :
			public operations {
		protected:
			//math_obj* point_left;		//����������� (operations), ����� �������
			//math_obj* point_right;	//����������� (operations), ������ �������
			//math_obj* point_collar;	//����������� (operations), ��������� �� ���������� ��������
		public:
			division();
			division(math_obj* _pl);
			division(math_obj * _pl, math_obj * _pr);
			division(operations * _high, math_obj * _low, math_obj * _obj)throw (ProjectError::ErrorCode);
			~division();
			
			flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();

			int get_priority();
		};	

		class power :
			public operations {
		protected:
			//math_obj* point_left;		//����������� (operations), ����� �������
			//math_obj* point_right;	//����������� (operations), ������ �������
			//math_obj* point_collar;	//����������� (operations), ��������� �� ���������� ��������
		public:
			power();
			power(math_obj* _pl);
			power(math_obj * _pl, math_obj * _pr);
			power(operations * _high, math_obj * _low, math_obj * _obj)throw (ProjectError::ErrorCode);
			~power();
			
			flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();

			int get_priority();
		};
	}
}

#endif // !OPERATIONS_H




