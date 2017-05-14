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

#define cnst L"const"
#define funct  L"funct"
#define varbl  L"varbl"
#define equat  L"equat"
#define exprs  L"exprs"
#define numbr  L"numbr"
#define addit  L"oper+"
#define mltpl  L"oper*"
#define divis  L"oper/"
#define power  L"oper^"
#define vectr L"vectr"
		//�������� �������� (prop)		
#define defnd  L"defnd"  //����� ��������
#define undef  L"undef"  //����� ��������

#define arg_c  L"arg_c" //������� � ������������ �����������
#define arg_v  L"arg_v"  //������� � ����������� ����������� 
//#define empty  L"empty"  //"������" �������. �������� ������� �������������������� �������

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

		class math_obj {
		private:
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
			/*PRIVATE. ����� ���������� ������ ������� ������ ��������� ������ size*/
			math_obj* vector_create(int size);
			/*PRIVATE. ����� ���������� �������� ������ ���������� � ������.
			-1 ��� ������*/
			int var_list_copy_to_vector(math_obj* pointer, math_obj* vect);
			/*PRIVATE. ��������� ���������� �� �������.*/
			void var_list_number(math_obj* pointer);
			/*PRIVATE. ��������� ���������� point_collar ��� ������������ ������ ���������� �� �������*/
			void var_list_collar(math_obj* pointer, math_obj*original);
			/*PRIVATE. ����� ���������� ��������� �� �������� ��������������� ��������� ����������*/
			math_obj* get_arg_for_var(math_obj*pointer, math_obj*arg);
			/*PRIVATE. ���������� �������*/
			int vector_destruct(math_obj* pointer);
			//math_obj *var_list_to_arg(math_obj*var_list, int size);

			//math_obj *arg_to_var_list(math_obj*mass_arg, math_obj*var_list, int size);

			

		public:
			//������� �����������
			math_obj();
			//����������� ���������� (��� �������, ����� ���� double) 
			math_obj(wstring _name, double _num);
			//����������� ���������� (��� �������, ��� �������, ����� ���� double) 
			math_obj(wstring _name, wstring _type, double _num);
			//����������� ���������� (��� �������, ��� �������, �������� �������, ����� ���� double) 
			math_obj(wstring _name, wstring _type, wstring _prop, double _num);
			//����������� ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double) 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num);
			//����������� ���������� (��� �������, ����� ���� double, ��������� "��������") 
			math_obj(wstring _name, double _num, math_obj *_pc);
			//����������� ���������� (��� �������, ��� �������, ����� ���� double, ��������� "��������") 
			math_obj(wstring _name, wstring _type, double _num, math_obj *_pc);
			//����������� ���������� (��� �������, ��� �������, �������� �������, ����� ���� double, ��������� "��������") 
			math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj *_pc);
			//����������� ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double, ��������� "��������") 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj *_pc);
			//����������� ���������� (��� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����") 
			math_obj(wstring _name, double _num, math_obj * _pl, math_obj *_pr);
			//����������� ���������� (��� �������, ��� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����") 
			math_obj(wstring _name, wstring _type, double _num, math_obj * _pl, math_obj *_pr);
			//����������� ���������� (��� �������, ��� �������, �������� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����") 
			math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj * _pl, math_obj *_pr);
			//����������� ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double, ��������� "����� �����", ��������� "������ �����") 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr);
			//����������� ���������� (��� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			math_obj(wstring _name, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//����������� ���������� (��� �������, ��� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			math_obj(wstring _name, wstring _type, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//����������� ���������� (��� �������, ��� �������, �������� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//����������� ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc);			
			//����������� �����������
			math_obj(math_obj* var1);
			//����������� �������
			math_obj(int size_n);
			//����������� ������� � ������
			math_obj::math_obj(wstring _name, int size_n);
			//����������� �������
			math_obj(int size_n, int size_m);
			//���������� TODO: ����� ��, ���� � ��� �����..., ����� ���� ��������� ���������� tree_destrust, ����� � �������.
			~math_obj();
			//����� �����������.
			void copy(math_obj* ref);
			/*����� ���������� ��������� ��������.
			1 - ��������
			2 - ���������
			3 - �������
			4 - ���������� � �������
			0 - �� ��������*/
			int get_priority();
			/*����� �������� ����������� �������, ���������� �� ������ �������� � ������������� ������ � ���������� ������� �������� ���������.
			���������� ������. ���� �� ��������*/
			wstring expresion(int comma);
			/*����� �������� ����������� ������� �������� ���������� �������� � ������� ������ ��������.
			���������� ��������� �� ������� ������ (��������), ������� ��������� ������� ��� ������ � ��������� � ������� ���������. */
			math_obj* prioritize(int current_priority);
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
			/*����� ���������� ������ ���������� (���� �� ���� �� ������� ����������) ������� pointer �� ������� ���������� ������ �������.
			����� ����������:
			0 - ������ ���������� pointer ��������� ������ � ������ ���������� ������ �������.
			����� ������ ������������� ����� - ����� �������� ������� �������.
			*/
			int var_list_compare(math_obj * pointer);
			/*����� �������� ������ ���������� ������ ������� �� ������� ���������� �������, ����������� � � ������*/
			math_obj *create_var_list(math_obj* pointer, math_obj *var_list);

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
			/*����� ��������� ������� � ����� �������.
			-1 � ������ ������*/
			int vector_push_back(math_obj*pointer);
			/*����� ��������� ������� � ������ �������.
			-1 � ������ ������*/
			int vector_push_foward(math_obj*pointer);
			/*����� ��������� � ������ ������� �� ����� index, ��� ������� ��� index ����������.
			-1 � ������ ������*/
			int vector_assing_at(math_obj*pointer, int index);
			/*����� ��������� ������� � ����� ������� ���������� �������.
			-1 � ������ ������*/
			int funct_arg_push_back(math_obj*pointer);
			/*����� ����������� ����������� ������ � ����������*/
			void math_obj::double_lincked_vector();
			

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
