#pragma once
#ifndef CORE_H
#define CORE_H
///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///

#include <iostream>
#include <string>


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
		data_list* Init();
		
		/*������� ��������� ��������/������ ���������� ���������� ������ �������� � �������� ������������.
		���������� ������ � ����������� ������� �������� ����������.*/
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, math_obj* _current_element);
		/*������� �������� ������ ����� � �������������� ����.
		���������� ������ � ����������� ������� �������� ����������.*/
		int input_to_analize(data_list* all_math_data);
	}
}
#endif //CORE_H