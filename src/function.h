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
			uint32_t num_var;	//���������� ���������� �������
			//uint32_t point_left; //����������� (expression)
			//leaf_ptr point_collar; //��������� �� ������ ����������
			
		public:
			function() :expression() {};
			
			//����� get. ���
			flags get_class_type();

			//����� get. �����
			long double get_num();
			//����� assing. �����
			void assing_num(long double _num);

			//����� get. �������� ��� ������������� ���������
			unsigned int get_num_var();
			//����� assing. �������� ��� ������������� ���������
			void assing_num_var(unsigned int _num);

			void copy_to(void * _ptr);

			bool copy(function* _original);
		};

		class funct_defnd :
			public function
		{
		protected:
			wstring name;//��� �������
			//unsigned int num_var;	//����������� (function), ���������� ���������� �������
			//math_ptr point_left; //����������� (expression)
			//math_ptr point_collar; //����������� (function)
			
		public:
			funct_defnd() :function() {};
			
			//����� get. ���
			flags get_class_type();

			//����� get. ���
			wstring get_name();
			//����� assing. ���
			void assing_name(wstring _name);

			void copy_to(void * _ptr);

			bool copy(funct_defnd* _original);
		};


		class funct_arg_c :
			public funct_defnd
		{
		protected :
			//wstring name;//����������� (funct_defnd),��� �������
			//unsigned int num_var;	//����������� (function), ���������� ���������� �������
			//math_ptr point_left; //����������� (expression)
			//leaf_ptr point_right; //��������� �� ������ ����������
			//math_ptr point_collar; //����������� (function)
			
		public:
			funct_arg_c() :funct_defnd() {};
			
			//����� get. ���
			flags get_class_type();

			void copy_to(void * _ptr);

			bool copy(funct_arg_c* _original);
		};


		class funct_arg_v :
			public funct_defnd
		{
		protected:
			//wstring name;//����������� (funct_defnd),��� �������   //28 ����! 28 ���� ������ ��� ������ wstring.
			//unsigned int num_var;	//����������� (function), ���������� ���������� �������  
			//math_ptr point_left; //����������� (expression)
			//leaf_ptr point_right; //��������� �� ������ ����������
			//math_ptr point_collar; //����������� (function)
			
		public:
			funct_arg_v() :funct_defnd() {};
			
			//����� get. ���
			flags get_class_type();

			void copy_to(void * _ptr);

			bool copy(funct_arg_v* _original);
		};
	}
}
#endif //!FUNCTION_H
