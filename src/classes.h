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

		enum class flags : short
		{
			error = -10,//������ ���� ����������� � ���� �������
			nthng = -100,
			//���� ���. ��������
			matrx = -7,
			vectr = -6,
			exprs = -5,
			equat = -4,
			funct = -3,
			varbl = -2,
			cnst = -1,
			numbr = 0,
			//���� ��������
			addit = 1,
			minus = 2,
			mltpl = 3,
			divis = 4,
			power = 5,
			//��������
			defnd = 6,			//����� ��������
			undef = 7,			//����� ��������
			arg_v = 8,			//�������/������ � ����������� ����������� 
			arg_c = 9,			//�������/������ � ������������ �����������
			only_arg_v = 10,	//�������� �������, ����������� ������ ����������

			fundm = 11,			//�������� �������������� ��������: ��������, ����������� ������� � �.�.

			unslv = 12,			//�������� ���������
			solvd = 13,			//�������� ���������

			real = 14,			//�������� �����, ��������, ��������, �������
			cmplx = 15,			//�������� �����, ��������, ��������, �������
			qwtrn = 16,			//�������� �����, ��������, ��������, �������

			servc = 20,			//�������� ��� ��������� ��������
			//��������
			solve = 17,
			write = 18,
			read = 19
		};

		class math_obj;

		class data_list {
		private:

			//�������� ��� back
			data_list*back_rec(data_list*pointer);

			//����� ���������� �������� ������� ��������� ������, ������� � ��������.
			void index_plus_one();

			//�������� ��� ������ at
			data_list*at_rec(int* place, data_list*pointer);

			//�������� ��� ������� size
			int size_rec(data_list*pointer, bool* flag);

			/*�������� ��� delete_starting_at*/
			int delete_starting_at_rec();

		public:
			int index;
			wstring in;
			wstring out;
			data_list* left;
			data_list* right;
			math_obj* math;

			//������� �����������
			data_list();
			//���������� ��� ������� ����� in
			data_list(wstring* _in);
			//���������� ��� ������� ����� in, ���. ������� math
			data_list(wstring _in, math_obj* _math);

			/*����� ��������� ������� pointer � ����� ������� ������.
			0 - � ������ ������ (������ place �� ����������)
			1 - � ������ ��������� ����������*/
			int push_back(data_list* pointer);

			/*����� ��������� ������� pointer � ������ ������ �� �������� ��������. ������ ������������� ������ -1.
			����� ����������:
			0 - � ������ ������ 
			1 - � ������ ��������� ����������*/
			int push_left(math_obj* pointer);

			/*����� ���������� ��������� �� ������� ������� ������ ������.
			NULL - � ������ ������ (��������, ������� ������ ����� �� ����*/
			data_list*begin();

			/*����� ��������� ������� pointer � ������ ������ ����� ������ place. ������� ��������� ��������� ������ ���������� �� 1.
			����� ����������:
			0 - � ������ ������ (������ place �� ����������)
			1 - � ������ ��������� ����������*/
			int implace(int place, data_list*pointer);

			/*����� ���������� ��������� �� ������� ������ � ������� place. 
			��������� �������� ��������� (�� ��������) ���������� � 1 (�� �� �������������, �� ��� ����� ������ �������).
			NULL - ������ place ��� � ������ ��� �� ������ ����, � � ������ ������ �� ��� ���������� (��������) ��������*/
			data_list* at(int place);

			/*����� ���������� ��������� �� ��������� ������� ������ ������. ���� ������ �� ���� �������� - ��������� �� ������� �������. 
			NULL - � ������ ������ �� ��� ���������� (��������) ��������*/
			data_list* back();

			/*����� ��������� ������ in (����) � �������� �������. ���������� ������ compare ������ wstring
			����� ����������:
			- 0 - ������ ���������
			- ������� ����� ������� ���������� ���������*/
			int compare_in(wstring *original);

			/*����� ��������� ������ out (�����) � �������� �������. ���������� ������ compare ������ wstring
			����� ����������:
			- 0 - ������ ���������
			- ������� ����� ������� ���������� ���������*/
			int compare_out(wstring *original);

			/*����� ���������� ��������� ����� �� ������ name � ������ ������.
			����� ����������:
			- ��������� math_obj* �� ������ ���. ������
			- NULL ���� ������ �� �������*/
			math_obj* find_math_obj(wstring* name);

			/*����� ������� ����������� ��������������� ������� �� ������ in. ��� �� ���������� �������� �����.			 
			����� ����������:
			0 - � ������ ������, ��� ��� ����������, ��� � ��� �������� ����� (����� ������ � ���� out).
			1 - � ������ ��������� ����������*/
			int run_string();

			/*����� ���������� ���������� ���������� ��������� (�����) � ������ ������.
			��������� �������� ��������� (�� ��������) ���������� � 1 (�� �� �������������, �� ��� ����� ������ �������).
			0 - ���� ����� �������� �������� ������ ���
			-1 - ������ - ��������� � �������� �������� ��������� ������.
			*/
			int size_s();

			/*����� ���������� ����� ���������� ��������� � ������ ������
			0 - ���� ����� �������� �������� ������ ���
			-1 - ������ - ��������� � �������� �������� ��������� ������.
			*/
			int size_all();
			/*����� ������� ��� �������� ������ ������ ������� � ������� start. ������� start �� ���������*/
			int delete_starting_at(int start);
			

		};






		class math_obj {
		private:
			/*PRIVATE. ����� ������ ������ �������������� �������� ��� ������� ��������������� �������.
			������ ����� � ����� ������ ����� ���� ����������.
			����� ���������� ��������� �� ��������� ��������� ������ math_obj, �������������� ����� �������� (������) ������, ���� �� ����� ��� ���������� ����������.
			NULL � ������ �����-���� ������, ��. error.h.*/
			math_obj* build_tree(wchar_t* strPtr, wchar_t*endPtr);
			/*PRIVATE.����� ������ ���. ������ - ���. ��������, � ������������ � op_type, � ���������� �� ������ ����� � ������ high, � ������ low.
			����� ���������� ��������� �� ��������� ��������.*/
			math_obj *operations(math_obj *high, math_obj *low, flags op_type);
			/*PRIVATE.����� ���������� ��������� ��������, �� ��������� �� ������ � ��������� ����.
			1 - ��������/���������
			2 - ���������
			3 - �������
			4 - ���������� � �������
			0 - �� ��������*/
			int priority(flags *op_type);




			math_obj* math_simplify_processing();

			wstring expression_processing();

			/*PRIVATE. �������� ��� tree_destruct*/
			int tree_destruct_processing(math_obj* pointer);
			/*PRIVATE. �������� ��� prioritize*/
			math_obj *prioritize_processing(math_obj *pc, int *current_priority);			
			/*PRIVATE. �������� ��� var_list_back*/
			math_obj *vector_back_processing(math_obj *pointer);
			/*PRIVATE. �������� ��� find_by_name*/
			math_obj *find_by_name_processing(math_obj*pointer, math_obj *original);
			/*PRIVATE. ������� ��������������� �������*/
			math_obj *merge_lists(math_obj* list_a, math_obj* list_b);
			/*PRIVATE. �������� ����������� ������.*/
			void reassing_left_pointers(math_obj* pointer);
			/*PRIVATE. �������� ��� vector_at*/
			math_obj* vector_at_processing(math_obj*pointer, int* index, int* count);
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
			/*PRIVATE. �������� ��� vector_destruct.*/
			int vector_destruct_processing(math_obj* pointer, int *flag);
			/*PRIVATE. ����� ������ �������������� ������ - ������ ����� size_n �� ������ �������� ���� wchar_t, ������� ������ begin � ����� end.
			����� ���������� ��������� �� ��������� ��������� ������ math_obj. ������� �������� � �������� ���������� �������, � ��� �� ����������, �������� ������� ���������� build_tree.
			NULL � ������ �����-���� ������, ��. error.h.*/
			math_obj* vector_create(int size_n, wchar_t*begin, wchar_t*end);


		public:
			//������� �����������
			math_obj();
			//����������� ���������� (��� �������, ����� ���� double) 
			math_obj(wstring _name, double _num);
			//����������� ���������� (��� �������, ��� �������, �������� �������, ����� ���� double) 
			math_obj(wstring _name, flags _type, flags _prop, double _num);
			//����������� ���������� (��� �������, �������� �������, ����� ���� double, ��������� "��������") 
			math_obj(flags _type, flags _prop, double _num, math_obj *_pc);
			//����������� ���������� (��� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			math_obj(flags _type, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//����������� ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			math_obj(wstring _name, flags _type, flags _prop, flags _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//����������� �����������
			math_obj(math_obj* var1);

			//����������� ������� ����� size_n
			math_obj(size_t size_n);
			//����������� ������� ����� size_n �� ������ �������� ���� wchar_t, ������� ������ begin � ����� end.
			math_obj(size_t size_n, wchar_t*begin, wchar_t*end, data_list* data);

			//����������� ������� ��������� ����������, ����������� �� ���� � ��� �� ������ obj. ���������� ��� ������ ���������� ��� ������� obj
			math_obj(size_t size_n, math_obj * obj);

			/*����������� ������ �������� �� ������ �������� ���� wchar_t, ������� ������ strPtr � ����� ePtr.
			�������������� �������� _pc ������������� ���� point_collar (��� ��������� ������ �������� ��� ������� � ������� ������������ ������� ����������).*/
			math_obj(wchar_t* strPtr, wchar_t* ePtr, math_obj* _pc, data_list* data);


			//����������� �������
			//math_obj(int size_n, int size_m);

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

			bool math_simplify();

			wstring expression();

			

			
			/*����� �������� ����������� �������, ���������� �� ������ �������� � ��������� ������.
			����� ������ ���������� ����� ������ ��� ������������� delete. ��� ���� ��������
			*/
			int tree_destruct();
			/*����� ����������� ��������� �� ������� ������ � ������ ������*/
			math_obj* find_by_name(math_obj*pointer);
			
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
			
			/*����� ���������� ��������� �� ������� ������ � ������� index.*/
			math_obj* vector_at(int index);
			/*����� ���������� ���������� ��������� ������.*/
			int vector_size();
			/*����� ���������� ��������� �� ��������� ������� �������*/
			math_obj* vector_back();

			/*���������� �������*/
			int vector_destruct();

			/*����� ��������� ������� pointer � ����� �������. ����������, ��� pointer - ����������� ����� ������ ���� ��������.
			-1 � ������ ������*/
			int vector_push_back(math_obj*pointer);
			/*����� ��������� ������� pointer � ������ �������. ����������, ��� pointer - ����������� ����� ������ ���� ��������.
			-1 � ������ ������*/
			int vector_push_foward(math_obj*pointer);
			/*����� ��������� � ������� � ������ ������� �� ����� index, ��� ������� ��� index ����������.
			-1 � ������ ������*/
			int vector_assing_at(math_obj*pointer, int index);
			
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
			void convert_to(flags _type, flags _prop, double _num, math_obj *_pc);
			/*����� ��������� ����������� this � ��������� ����������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������").
			���� ������� ������� - ������, ��� �������� �������� ��������� �� ���� � ��� �� ������, ������������� ��� �� ������*/
			void convert_totaly(wstring _name, flags _type, flags _prop, flags _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);


			//����� get. ������� �������
			math_obj*get_this();

			
			//����� get. ���
			wstring get_name();
			//����� assing. ���
			void assing_name(wstring _name);

			//����� get. ���
			flags get_type();
			//����� assing. ���
			void assing_type(flags _type);

			//����� get. ��������
			flags get_prop();
			//����� assing. ��������
			void assing_prop(flags _prop);


			//����� get. ��������
			flags get_actn();
			//����� assing. ��������
			void assing_actn(flags _acnt);


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
			//����� assing. ��������� "������ �����"
			void assing_pr(math_obj* pointer);


			//����� get. ��������� "��������"
			math_obj* get_pc();
			//����� assing. ��������� "��������"
			void assing_pc(math_obj* pointer);


			/*����� ��������� ���������� � ����� ������ ���������� ������ �������.
			� ��� ����� � ��� ���������/���������.
			-1 � ������ ������.*/
			int var_list_push_back(math_obj*pointer);
			/*����� ���� � �������� ������ ������� � ����� brackets_open ��������������� ����������� ������.
			����� ���������� ��������� �� ������ ����� � ������.
			NULL � ������ ������.*/
			wchar_t* brackets_close(wchar_t* brackets_open, wchar_t*end);
			/*����� ����������� ��������� ��������� ��� ������� � ���������.
			NULL � ������ ������*/
			math_obj * unvectorize();

			enum class variable_type {
				INTEGER,
				FRACTIONAL,
				IMAGINARY,
				REAL,
				RE_IM,
				EXP	//5E+10 5*10^10
			};

			wstring name;
			flags type;
			flags prop;
			flags actn;
			double var;
			//double arg; //����� ����������.
			//int exp;
			math_obj *point_left;		//����� �����
			math_obj *point_right;		//������ �����
			math_obj *point_collar;	//��������
			data_list *point_up;
		};


	}
};
#endif //CLASSES_H
