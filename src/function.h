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
			leaf_ptr point_collar; //��������� �� ������ ����������
			
		public:
			function();
			virtual ~function();

			//����� get. ���
			virtual flags get_class_type();

			virtual size_t get_sizeof();

			virtual math_obj* get_this();

			//����� get. �����
			long double get_num();
			//����� assing. �����
			void assing_num(long double _num);

			//����� get. �������� ��� ������������� ���������
			unsigned int get_num_var();
			//����� assing. �������� ��� ������������� ���������
			void assing_num_var(unsigned int _num);

			//����� get. ��������� "��������"
			leaf_ptr get_pc();
			//����� assing. ��������� "��������"
			void assing_pc(leaf_ptr _pointer);

			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);


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
			funct_defnd();
			virtual ~funct_defnd();

			//����� get. ���
			virtual flags get_class_type();

			virtual size_t get_sizeof();

			virtual math_obj* get_this();

			//����� get. ���
			wstring get_name();
			//����� assing. ���
			void assing_name(wstring _name);

			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};


		class funct_arg_c :
			public funct_defnd
		{
		protected :
			//wstring name;//����������� (funct_defnd),��� �������
			//unsigned int num_var;	//����������� (function), ���������� ���������� �������
			//math_ptr point_left; //����������� (expression)
			leaf_ptr point_right; //��������� �� ������ ����������
			//math_ptr point_collar; //����������� (function)
			
		public:
			funct_arg_c();
			~funct_arg_c();

			//����� get. ���
			flags get_class_type();

			virtual size_t get_sizeof();

			virtual math_obj* get_this();

			//����� get. ��������� "������ �����"
			leaf_ptr get_pr();
			//����� assing. ��������� "������ �����"
			void assing_pr(leaf_ptr _pointer);

			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};


		class funct_arg_v :
			public funct_defnd
		{
		protected:
			//wstring name;//����������� (funct_defnd),��� �������   //28 ����! 28 ���� ������ ��� ������ wstring.
			//unsigned int num_var;	//����������� (function), ���������� ���������� �������  
			//math_ptr point_left; //����������� (expression)
			leaf_ptr point_right; //��������� �� ������ ����������
			//math_ptr point_collar; //����������� (function)
			
		public:
			funct_arg_v();
			~funct_arg_v();

			//����� get. ���
			flags get_class_type();

			virtual size_t get_sizeof();

			virtual math_obj* get_this();

			//����� get. ��������� "������ �����"
			leaf_ptr get_pr();
			//����� assing. ��������� "������ �����"
			void assing_pr(leaf_ptr _pointer);

			virtual uint16_t copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};
	}
}
#endif //!FUNCTION_H
