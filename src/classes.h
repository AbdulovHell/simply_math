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
			//��������
			int tree_destruct_processing(math_obj* pointer);
			//��������
			math_obj *prioritize_processing(math_obj *pc, int current_priority);
			//��������
			double arithmetic_processing(math_obj *pointer, math_obj * last_arg);
			//���� ���������
			wstring expression_processing(math_obj *pointer, int* comma);

			math_obj *var_list_back_processing(math_obj *pointer);

			math_obj *find_varbl_processing(math_obj*pointer, math_obj *original);

			math_obj *merge_lists(math_obj* list_a, math_obj* list_b);

			void reassing_right_pointers(math_obj* pointer);

			math_obj *var_list_to_arg(math_obj*var_list, int size);

			math_obj *arg_to_var_list(math_obj*mass_arg, math_obj*var_list, int size);

			math_obj *create_var_list(math_obj* pointer, math_obj *var_list);

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
			//������ �����������. ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double (������), ����� ���� double (��������), ����� ���� int (������� 10), ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, double _arg, double _exp, math_obj * _pl, math_obj *_pr, math_obj *_pc);
			//����������� �����������
			math_obj(math_obj* var1);
			//���������� TODO: ����� ��, ���� � ��� �����...
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
			//
			math_obj* find_varbl(math_obj*pointer);
			/*����� ���������� ��������� �� ��������� ������� ������ ����������.
			���� ������ ��� ������� - �� ������� �������������� �������� (�������� ������ ������� �� ������ ���������).*/
			math_obj* var_list_back();
			/*����� ���������� ������ ���������� (���� �� ���� �� ������� ����������) ������� pointer �� ������� ���������� ������ �������.
			����� ����������:
			0 - ������ ���������� pointer ��������� ������ � ������ ���������� ������ �������.
			����� ������ ������������� ����� - ����� �������� ������� �������.
			*/
			int var_list_compare(math_obj * pointer);
			/*����� ����������� point_right ������ ������� ������ ���������� � ������ ������ � ����� � ����������� ����� ���������� �������.
			�������� ������ ��� ������� � ����������� ������� ����������. ���������� ������ ���������
			*/
			void arg_copy_plus_1();
			/*����� ��������� ����������� ������ ���������� �� ��������*/
			math_obj * sort_list(math_obj * var_list);
			/*����� ����������� �������. ������ ������ ����������, �� ������� ������� �������, ����������� �� ������ ���������� � ���������� ������� pointer.*/
			void define_funct(math_obj *pointer);

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
			double arg; //����� ����������.
			int exp;
			math_obj *point_left;		//����� �����
			math_obj *point_right;		//������ �����
			math_obj *point_collar;	//��������
		};


	}
};
#endif //CLASSES_H
