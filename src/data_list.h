#pragma once
#ifndef DATA_LIST_H
#define DATA_LIST_H

#include "common.h"
#include "IO.h"
#include "error.h"

#include "function.h"
#include "constant.h"
#include "variable.h"
#include "number.h"
#include "operations.h"
#include "math_vector.h"


namespace Project {
	//namespace Core{
	class data_list {
	private:
		friend struct Project::data_list::iterator;
		//�������� ��� end
		data_list*back_rec();
		
		//�������� ��� ������ at
		data_list*at_rec(int* place, int * count);

		//�������� ��� ������� size
		int size_rec(bool* flag);

		/*�������� ��� delete_starting_at*/
		int delete_starting_at_rec();

		bool implace_p(data_list* _obj);

		Core::math_obj* build_iternal( wchar_t * _str, wchar_t * _end);

		Core::math_obj* build_tree(wchar_t* _str, wchar_t* _end);

	public:		
		wstring in;
		wstring out;
		data_list* left;
		data_list* right;
		Core::math_obj* object;

		typedef struct iterator {
		private:
			friend class data_list;
			data_list * letter;			
		public:
			iterator();
			//����������� ����� ������� data_list  � ���������������� ������ ����� ���������� ��������� letter
			bool implace_after_this(data_list*pointer);
			int compare_in(wstring *original);
			int compare_out(wstring *original);
			Core::math_obj* find_math_obj(wstring* name);
			int delete_after_this();
			bool build();

			wstring get_in();
			wstring get_out();
			Core::math_obj * get_obj();
			void assing_in(wstring & _in);
			void assing_out(wstring & _in);
			void assing_obj(Core::math_obj * _obj);

			iterator &operator= (const iterator _left);
			iterator &operator= (data_list* _left);
			//�����������
			iterator &operator++ (int);
			//����������
			iterator &operator++ ();
			//�����������
			iterator &operator-- (int);
			//����������
			iterator &operator-- ();
			bool operator== (const iterator _left);
			bool operator== (const data_list* _left);
			bool operator!= (const iterator _left);
			bool operator!= (const data_list* _left);

			operator data_list * () const;
		};

		//������� �����������
		data_list();
		//���������� ��� ������� ����� in
		data_list(wstring* _in);
		data_list(wstring* _in, data_list* _start);
		//���������� ��� ������� ����� in, ���. ������� object
		data_list(wstring _in, Core::math_obj* _math);

		/*����� ��������� ������� pointer � ����� ������� ������.
		0 - � ������ ������ (������ place �� ����������)
		1 - � ������ ��������� ����������*/
		bool push_back(data_list* pointer);

		/*����� ��������� ������� pointer � ������ ������ �� �������� ��������. ������ ������������� ������ -1.
		����� ����������:
		0 - � ������ ������
		1 - � ������ ��������� ����������*/
		bool push_left(Core::math_obj* pointer);

		/*����� ���������� ��������� �� "���������" ������� ������ ������.
		NULL - � ������ ������ (��������, ������� ������ ����� �� ����*/
		data_list*begin();

		iterator &front();

		iterator &back();

		/*����� ��������� ������� pointer � ������ ������ ����� ������ place. ������� ��������� ��������� ������ ���������� �� 1.
		����� ����������:
		0 - � ������ ������ (������ place �� ����������)
		1 - � ������ ��������� ����������*/
		bool implace(int place, data_list*pointer);

		/*����� ���������� ��������� �� ������� ������ � ������� place.
		NULL - ������ place ��� � ������ ��� �� ������ ����, � � ������ ������ �� ��� ���������� (��������) ��������*/
		data_list* at(int place);

		/*����� ���������� ��������� �� ��������� ������� ������ ������. ���� ������ �� ���� �������� - ��������� �� ������� �������.
		NULL - � ������ ������ �� ��� ���������� (��������) ��������*/
		data_list* end();

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
		Core::math_obj* find_math_obj(wstring* name);

		/*����� ������� ����������� ��������������� ������� �� ������ in. ��� �� ���������� �������� �����.
		����� ����������:
		0 - � ������ ������, ��� ��� ����������, ��� � ��� �������� ����� (����� ������ � ���� out).
		1 - � ������ ��������� ����������*/
		bool run_string();

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

		bool build();

		


	};
//} //end namespace Core
}
#endif //DATA_LIST_H