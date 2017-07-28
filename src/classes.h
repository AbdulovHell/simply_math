#pragma once
#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include "IO.h"
#include "error.h"


namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;
		class data_list;
		class math_obj;

		enum class flags : int16_t
		{
			//���� ���. ��������
			error = -10,
			math_dummy = -2,
			variable =-1,

			base = 0,

			number = 1,
			complex = 2,
			//qwauterion = 3

			constant = 10,
			constant_cmplx = 11,

			expression = 20,
			function = 21, //undef
			funct_defnd = 22,
			funct_arg_c = 23,
			funct_arg_v = 24,


			equation = 30,

			vector = 40,

			matrix = 50,

			operation = 60,

			addit = 61,
			minus = 62,
			mltpl = 63,
			divis = 64,
			power = 65,				
			

		};


		
		class math_obj {
		private:			
		protected://������� ����� ������� ������ ��������� �� ������� ������ ������ ������
			data_list* point_up;			
		public:		

			//������� �����������
			math_obj();			

			virtual ~math_obj();

			//����� get. ���
			virtual flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();

			//����� get. ���
			virtual wstring get_name();
			//����� assing. ���
			virtual void assing_name(wstring _name);

			//����� get. �����
			virtual double get_num();
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

			//����� get. ��������� "�����"
			virtual data_list* get_pu();
			//����� assing. ��������� "�����"
			virtual void assing_pu(data_list* _pointer);

		};





		class data_list {
		private:

			//�������� ��� back
			data_list*back_rec();

			//����� ���������� �������� ������� ��������� ������, ������� � ��������.
			void index_plus_one();

			//�������� ��� ������ at
			data_list*at_rec(int* place);

			//�������� ��� ������� size
			int size_rec(bool* flag);

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
			int delete_starting_at(int from);


		};
	}
};
#endif //CLASSES_H
