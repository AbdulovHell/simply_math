#pragma once
#ifndef CORE_H
#define CORE_H
///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>

#include "error.h"
#include "IO.h"
#include "filters.h"
#include "classes.h"

namespace Project {
	namespace Core {

		using namespace std;
		using namespace Project;
		//using namespace Project::IO;
		using namespace Project::Core;

		void ClearGeneral();
		//������������� ������� ��������, ����������, ������� � ���������
		void Init();
		//������� �������� �� ������� general_var_const � ���� ������ � ������ ������
		math_obj* run_vector(wstring _pv);
		/*������� ������ �������������� ������ - ������ - �� ������ �������� ���� wchar_t, ������� ������ begin � ����� end.
		������� ���������� ��������� �� ��������� ��������� ������ math_obj. ������� �������� � �������� ���������� �������, � ��� �� ����������, �������� ������� ���������� build_tree.
		NULL � ������ �����-���� ������, ��. error.h.*/
		math_obj *make_vector(int size, wchar_t*begin, wchar_t*end);		

		/*������� ������ �������������� ������� �� ������ ��� �������� � ���� ��������� ���� ������ �� ������ �������� ���� wchar_t, ������� ������ strPtr � ����� ePtr.
		������� ���������� ��������� �� ��������� ��������� ������ math_obj, �������������� ����� �������� (������) ������, ���� �� ����� ��� ���������� ����������.
		NULL � ������ �����-���� ������, ��. error.h. ���� ��� �������� ������� � classes.h*/
		math_obj* build_tree(wchar_t* strPtr, wchar_t*ePtr, math_obj* c_e, int brakets);
		/*������� ��������� ��������/������ ���������� ���������� ������ �������� � �������� ������������.
		���������� ������ � ����������� ������� �������� ����������.*/
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, math_obj* _current_element);
		/*������� �������� ������ ����� � �������������� ����.
		���������� ������ � ����������� ������� �������� ����������.*/
		wstring input_to_analize(wchar_t* input);
	}
}
#endif //CORE_H