#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
#include "expression.h"
namespace Project {
	namespace Core {
		using namespace std;
		class function ://����� ������ �������� ������� (��� �����)
			public expression
		{
		protected:			
			unsigned int num_var;	//���������� ���������� �������
			//math_obj* point_left; //����������� (expression)
			math_obj* point_collar; //��������� �� ������ ����������
			//data_list* point_up;  //����������� (math_obj)
		public:
			function();
			virtual ~function();

			//����� get. ���
			virtual flags get_class_type();

			//����� get. �����
			double get_num();
			//����� assing. �����
			void assing_num(long double _num);

			//����� get. �������� ��� ������������� ���������
			unsigned int get_num_var();
			//����� assing. �������� ��� ������������� ���������
			void assing_num_var(unsigned int _num);

			//����� get. ��������� "��������"
			math_obj* get_pc();
			//����� assing. ��������� "��������"
			void assing_pc(math_obj* _pointer);

		};

		class funct_defnd :
			public function
		{
		protected:
			wstring name;//��� �������
			//unsigned int num_var;	//����������� (function), ���������� ���������� �������
			//math_obj* point_left; //����������� (expression)
			//math_obj* point_collar; //����������� (function)
			//data_list* point_up;  //����������� (math_obj)
		public:
			funct_defnd();
			virtual ~funct_defnd();

			//����� get. ���
			virtual flags get_class_type();

			//����� get. ���
			wstring get_name();
			//����� assing. ���
			void assing_name(wstring _name);
		};


		class funct_arg_c :
			public funct_defnd
		{
		protected :
			//wstring name;//����������� (funct_defnd),��� �������
			//unsigned int num_var;	//����������� (function), ���������� ���������� �������
			//math_obj* point_left; //����������� (expression)
			math_obj* point_right; //��������� �� ������ ����������
			//math_obj* point_collar; //����������� (function)
			//data_list* point_up;  //����������� (math_obj)
		public:
			funct_arg_c();
			~funct_arg_c();

			//����� get. ���
			flags get_class_type();

			//����� get. ��������� "������ �����"
			math_obj* get_pr();
			//����� assing. ��������� "������ �����"
			void assing_pr(math_obj* _pointer);
		};


		class funct_arg_v :
			public funct_defnd
		{
		protected:
			//wstring name;//����������� (funct_defnd),��� �������
			//unsigned int num_var;	//����������� (function), ���������� ���������� �������
			//math_obj* point_left; //����������� (expression)
			math_obj* point_right; //��������� �� ������ ����������
			//math_obj* point_collar; //����������� (function)
			//data_list* point_up;  //����������� (math_obj)
		public:
			funct_arg_v();
			~funct_arg_v();

			//����� get. ���
			flags get_class_type();

			//����� get. ��������� "������ �����"
			math_obj* get_pr();
			//����� assing. ��������� "������ �����"
			void assing_pr(math_obj* _pointer);
		};
	}
}
#endif //!FUNCTION_H
