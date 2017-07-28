#pragma once
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "math_obj.h"
namespace Project {
	namespace Core {
		using namespace std;
		class operations :
			public math_obj	{
		protected:
			math_obj* point_left;	//����� �������
			math_obj* point_right;	//������ �������
			//data_list* point_up;  //����������� (math_obj), �� ��������� �� ������������.
		public:		
			
			operations();	
			operations(math_obj* _pl);
			operations(math_obj* _high, math_obj* _low);
			virtual ~operations();
			
			//����� get. ���
			flags get_class_type();

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
			
			//���������� ��������� ���������� �������� � ����� ���. �������. Nullptr ��� ������ ��������������� ��� ����� ������
			virtual math_obj* get_result();
		};

		class addition :
			public operations {
		protected:
			//math_obj* point_left;		//����������� (operations), ����� �������
			//math_obj* point_right;	//����������� (operations), ������ �������
			//data_list* point_up;		//����������� (math_obj), �� ��������� �� ������������.
		public:
			addition();
			addition(math_obj* _pl);
			addition(math_obj* _high, math_obj* _low);
			~addition();
			
			//flags get_class_type(); //� ����� ������ ���������� ������ �������� �� ����� �������� ����� ������ �������� ��������� � ����, ������ ��� ���� ����, ��� ��� ��������

			//����� get. �������
			math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();
		};

		class subtraction :
			public operations {
		protected:
			//math_obj* point_left;		//����������� (operations), ����� �������
			//math_obj* point_right;	//����������� (operations), ������ �������
			//data_list* point_up;		//����������� (math_obj), �� ��������� �� ������������.

		public:
			subtraction();
			subtraction(math_obj* _pl);
			subtraction(math_obj* _high, math_obj* _low);
			~subtraction();

			//flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();
		};

		class multiplication :
			public operations {
		protected:
			//math_obj* point_left;		//����������� (operations), ����� �������
			//math_obj* point_right;	//����������� (operations), ������ �������
			//data_list* point_up;		//����������� (math_obj), �� ��������� �� ������������.
		public:
			multiplication();
			multiplication(math_obj* _pl);
			multiplication(math_obj* _high, math_obj* _low);
			~multiplication();
						
			//flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();
		};		

		class division :
			public operations {
		protected:
			//math_obj* point_left;		//����������� (operations), ����� �������
			//math_obj* point_right;	//����������� (operations), ������ �������
			//data_list* point_up;		//����������� (math_obj), �� ��������� �� ������������.
		public:
			division();
			division(math_obj* _pl);
			division(math_obj* _high, math_obj* _low);
			~division();
			
			//flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();
		};	

		class power :
			public operations {
		protected:
			//math_obj* point_left;		//����������� (operations), ����� �������
			//math_obj* point_right;	//����������� (operations), ������ �������
			//data_list* point_up;		//����������� (math_obj), �� ��������� �� ������������.
		public:
			power();
			power(math_obj* _pl);
			power(math_obj* _high, math_obj* _low);
			~power();
			
			//flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ��� (������ ��������)
			wstring get_name();

			math_obj* get_result();
		};
	}
}

#endif // !OPERATIONS_H




