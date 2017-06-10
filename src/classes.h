#pragma once
#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <string>
#include <stack>
#include "IO.h"
#include "error.h"


namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;
#define eror L"error"
#define cnst L"const"
#define funct  L"funct"
#define varbl  L"varbl"
#define equat  L"equat"
#define exprs  L"exprs"
#define numbr  L"numbr"
#define addit  L"oper+"
#define minus  L"oper-"
#define mltpl  L"oper*"
#define divis  L"oper/"
#define power  L"oper^"
#define vectr L"vectr" //��� �� �������� ��������� (prop) ���������� defnd ��������� ������� � ��������, � ��� �� ����������� ��������� ���������
#define matrx L"matrx"
		//�������� �������� (prop)		
#define defnd  L"defnd"  //����� ��������
#define undef  L"undef"  //����� ��������

#define arg_c  L"arg_c" //�������/������ � ������������ �����������
#define arg_v  L"arg_v" //�������/������ � ����������� ����������� 
//#define arg_f  L"arg_f" //�������/������ � ��������������� ����������� 
#define vectr_arg_c L"v_a_c" //�������� ��������� ������� � ������������ �����������
#define vectr_arg_v L"v_a_v"//�������� ��������� ������� � ����������� �����������
//#define vectr_arg_f L"v_a_f"//�������� ��������� ������� � ��������������� �����������
#define only_arg_v  L"arg_vo" //�������� �������, ����������� ������ ����������

#define fundm  L"fundm"	//��������������� ���������

#define unslv  L"unslv"	//�������� ��������� � ������� �� ����� ���������
#define solvd  L"solvd"	//�������� ��������� � ������� �� ����� ���������

#define real L"real_"	//�������� �����
#define cmplx L"cmplx"	//�������� �����

#define servc  L"servc"	//�������� ��������� ��������
								 //�������� ��� ��������� (actn)
#define solve  L"solve"
#define write  L"write"
#define read  L"read_"


		class data_list {
		private:



		public:
			int index;
			wstring in;
			wstring out;
			data_list* left;
			data_list* right;
			math_obj* math;


			data_list* at(int place);

			data_list* back();

			data_list* compare_in(wstring original);

			int run_string();

		};






		class math_obj {
		private:
			/*PRIVATE. ����� ������ ������ �������������� �������� ��� ������� ��������������� �������.
			������ ����� � ����� ������ ����� ���� ����������.
			����� ���������� ��������� �� ��������� ��������� ������ math_obj, �������������� ����� �������� (������) ������, ���� �� ����� ��� ���������� ����������.
			NULL � ������ �����-���� ������, ��. error.h.*/
			math_obj* build_tree(wchar_t* pDest, wchar_t*endPtr);

			math_obj *operations(math_obj *high, math_obj *low, wstring op_type);
			/*PRIVATE.����� ���������� ��������� ��������, �� ��������� �� ������ � ��������� ����.
			1 - ��������/���������
			2 - ���������
			3 - �������
			4 - ���������� � �������
			0 - �� ��������*/
			int priority(wstring *op_type);






			/*PRIVATE. �������� ��� tree_destruct*/
			int tree_destruct_processing(math_obj* pointer);
			/*PRIVATE. �������� ��� prioritize*/
			math_obj *prioritize_processing(math_obj *pc, int current_priority);
			/*PRIVATE. �������� ��� arithmetic*/
			double arithmetic_processing(math_obj *pointer, math_obj * last_arg, math_obj *arg);
			/*PRIVATE. �������� ��� expresion*/
			wstring expression_processing(math_obj *pointer, int* comma);
			/*PRIVATE. �������� ��� var_list_back*/
			math_obj *vector_back_processing(math_obj *pointer);
			/*PRIVATE. �������� ��� find_by_name*/
			math_obj *find_by_name_processing(math_obj*pointer, math_obj *original);
			/*PRIVATE. ������� ��������������� �������*/
			math_obj *merge_lists(math_obj* list_a, math_obj* list_b);
			/*PRIVATE. �������� ����������� ������.*/
			void reassing_left_pointers(math_obj* pointer);
			/*PRIVATE. �������� ��� vector_at*/
			math_obj* vector_at_processing(math_obj*pointer, int* index, int count);
			/*PRIVATE. �������� ��� vector_size*/
			int vector_size_processing(math_obj*pointer);			
			/*PRIVATE. ����� ���������� ������ ������� ������ ��������� ������ size �� ��������� ����������, ����������� �� ���� � ��� �� ������.*/
			math_obj* convert_to_vector(int size, math_obj*obj);
			/*PRIVATE. ����� ���������� �������� ������ ���������� � ������.
			-1 ��� ������*/
			int var_list_copy_to_vector_processing(math_obj* pointer, math_obj* vect);
			/*PRIVATE. ��������� ���������� �� �������.*/
			void var_list_number(math_obj* pointer);
			/*PRIVATE. ��������� ���������� point_collar ��� ������������ ������ ���������� �� �������*/
			void var_list_collar(math_obj* pointer, math_obj*original);
			/*PRIVATE. ����� ���������� ��������� �� �������� ��������������� ��������� ����������*/
			math_obj* get_arg_for_var(math_obj*pointer, math_obj*arg);
			/*PRIVATE. ���������� �������*/
			int vector_destruct(math_obj* pointer);
			
			math_obj* vector_create(int size_n, wchar_t*begin, wchar_t*end);
			

		public:			
			//������� �����������
			math_obj();
			//����������� ���������� (��� �������, ����� ���� double) 
			math_obj(wstring _name, double _num);
			//����������� ���������� (��� �������, ��� �������, �������� �������, ����� ���� double) 
			math_obj(wstring _name, wstring _type, wstring _prop, double _num);
			//����������� ���������� (��� �������, �������� �������, ����� ���� double, ��������� "��������") 
			math_obj(wstring _type, wstring _prop, double _num, math_obj *_pc);
			//����������� ���������� (��� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			math_obj(wstring _type, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//����������� ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);				
			//����������� �����������
			math_obj(math_obj* var1);
			
			/*����������� ������ �������� �� ������ �������� ���� wchar_t, ������� ������ strPtr � ����� ePtr. 
			�������������� �������� _pc ������������� ���� point_collar (��� ��������� ������ �������� ��� ������� � ������� ������������ ������� ����������).*/
			math_obj(wchar_t* strPtr, wchar_t*ePtr, math_obj* _pc);
			//����������� ������� ����� size_n
			math_obj(int size_n);
			//����������� ������� ����� size_n �� ������ �������� ���� wchar_t, ������� ������ begin � ����� end.
			math_obj(int size_n, wchar_t*begin, wchar_t*end);
			
			//����������� ������� ��������� ����������, ����������� �� ���� � ��� �� ������ obj. ���������� ��� ������ ���������� ��� ������� obj
			math_obj(int size_n, math_obj * obj);
			
			//����������� �������
			math_obj(int size_n, int size_m);

			//���������� TODO: ����� ��, ���� � ��� �����..., ����� ���� ��������� ���������� tree_destrust, ����� � �������.
			~math_obj();
			//����� �����������.
			void copy(math_obj* ref);
			
			/*����� ���������� ��������� ��������, ��� �������, ��� this ��������.
			1 - ��������/���������
			2 - ���������
			3 - �������
			4 - ���������� � �������
			0 - �� ��������*/
			int get_priority();

			/*����� �������� ����������� ������� �������� ���������� �������� � ������� ������ ��������.
			���������� ��������� �� ������� ������ (��������), ������� ��������� ������� ��� ������ � ��������� � ������� ���������. */
			math_obj* prioritize(int current_priority);



			/*����� �������� ����������� �������, ���������� �� ������ �������� � ������������� ������ � ���������� ������� �������� ���������.
			���������� ������. ���� �� ��������*/
			wstring expresion(int comma);
			
			/*����� �������� ����������� �������, ���������� �� ������ �������� � ����������� ��.
			����������� ������ ������ �������� ������ ���������� ���������� � double var �������� �������� ������. */
			void arithmetic();
			/*����� �������� ����������� �������, ���������� �� ������ �������� � ��������� ������.
			����� ������ ���������� ����� ������ ��� ������������� delete. ��� ���� ��������
			*/
			int tree_destruct();
			/*����� ����������� ��������� �� ������� ������ � ������ ������*/
			math_obj* find_by_name(math_obj*pointer);
			/*����� ���������� ��������� �� ��������� ������� ������ ����������.
			���� ������ ��� ������� - �� ������� �������������� �������� (�������� ������ ������� �� ������ ���������).*/
			math_obj* var_list_back();
			/*����� ���������� ������ ���������� (���� �� ���� �� ������� ����������) ������� pointer (�������, ������� ��� �������) �� ������� ���������� ������ �������.
			����� ����������:
			0 - ������ ���������� pointer ��������� ������ � ������ ���������� ������ �������.
			����� ������ ������������� ����� - ����� �������� ������� �������.
			-1 - ������ - �������� ��� �������
			*/
			int var_list_compare(math_obj * pointer);
			/*����� ���������� ������ ���������� (���� �� ���� �� ������� ����������) ������� pointer �� ������� ���������� ������ �������.
			����� ����������:
			0 - ������ ���������� pointer ��������� ������ � ������ ���������� ������ �������.
			����� ������ ������������� ����� - ����� �������� ������� �������.
			*/
			int var_list_compare_processing(math_obj * pointer);
			/*����� �������� ������ ���������� ������ ������� �� ������� ���������� �������, ����������� � � ������*/
			math_obj *create_var_list(math_obj *var_list);

			/*����� ��������� ����������� ������ ���������� �� ��������*/
			math_obj * sort_list(math_obj * var_list);
			/*����� ����������� �������. ������ ������ ����������, �� ������� ������� �������, ����������� �� ������ ���������� � ���������� ������� pointer.*/
			void funct_define(math_obj *pointer);
			
			/*����� ���������� ��������� �� ������� ������ � ������� index.*/
			math_obj* vector_at(int index);
			/*����� ���������� ���������� ��������� ������.*/
			int vector_size();
			/*����� ���������� ��������� �� ��������� ������� �������*/
			math_obj* vector_back();
			/*����� ��������� ������� pointer � ����� �������. ����������, ��� pointer - ����������� ����� ������ ���� ��������.
			-1 � ������ ������*/
			int vector_push_back(math_obj*pointer);
			/*����� ��������� ������� pointer � ������ �������. ����������, ��� pointer - ����������� ����� ������ ���� ��������.
			-1 � ������ ������*/
			int vector_push_foward(math_obj*pointer);
			/*����� ��������� � ������� � ������ ������� �� ����� index, ��� ������� ��� index ����������.
			-1 � ������ ������*/
			int vector_assing_at(math_obj*pointer, int index);
			/*����� ��������� ������� � ����� ������� ���������� �������.
			-1 � ������ ������*/
			int funct_arg_push_back(math_obj*pointer);
			/*����� ����������� ����������� ������ � ����������*/
			void double_lincked_vector();
			//����� �������� ����������� ������� ��������� ���������� point_collar ��� ������������ ������ ���������� �� ������� ��������� ������
			void link_var_list_to_funct();
			//����� �������� ������ ���������� � ������.
			void close_list();
			/*����� �������� ������ ���������� � ������. ���� this ��� �� ������, �� ���������� ��� ���������� � ������ ������ ��� ��������. 
			�� ������������ ��� ����������� �������� (���� ���� ������ �� serv� ������ ������ �� �����:-).
			-1 ��� ������*/
			int var_list_copy_to_vector(math_obj*vector);

			
			/*����� ����������� ������� ������� � ��������� ����������� (��� �������, �������� �������, ����� ���� double, ��������� "��������").
			���� ������� ������� - ������, ��� �������� �������� ��������� �� ���� � ��� �� ������, ������������� ��� �� ������*/
			void convert_to(wstring _type, wstring _prop, double _num, math_obj *_pc);
			/*����� ��������� ����������� this � ��������� ����������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������").
			���� ������� ������� - ������, ��� �������� �������� ��������� �� ���� � ��� �� ������, ������������� ��� �� ������*/
			void convert_totaly(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//����� get. ������� �������
			math_obj*get_this();
			//����� get. ���
			wstring get_name();
			//����� get. ���
			wstring get_type();
			//����� get. ��������
			wstring get_prop();
			//����� get. ��������
			wstring get_actn();
			//����� get. �����
			double get_var();
			//����� assing. �����
			void assing_var(double _num);
			//����� get. ��������� "����� �����"
			math_obj* get_pl();
			//����� assing. ��������� "����� �����"
			void assing_pl(math_obj* pointer);
			//����� get. ��������� "������ �����"
			math_obj* get_pr();
			//����� get. ��������� "��������"
			math_obj* get_pc();
			/*����� ��������� ���������� � ����� ������ ���������� ������ �������.
			� ��� ����� � ��� ���������/���������.
			-1 � ������ ������.*/
			int var_list_push_back(math_obj*pointer);

			wchar_t* brackets_close(wchar_t* brackets_open, wchar_t*end);

			enum class variable_type {
				INTEGER,
				FRACTIONAL,
				IMAGINARY,
				REAL,
				RE_IM,
				EXP	//5E+10 5*10^10
			};

			wstring name;
			wstring type;
			wstring prop;
			wstring actn;
			double var;
			//double arg; //����� ����������.
			//int exp;
			math_obj *point_left;		//����� �����
			math_obj *point_right;		//������ �����
			math_obj *point_collar;	//��������
		};


	}
};
#endif //CLASSES_H
