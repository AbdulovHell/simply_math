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

	//	void ClearGeneral();
		//������������� ������� ��������, ����������, ������� � ���������
		data_list* Init();
		
		/*������� ��������� ��������/������ ���������� ���������� ������ �������� � �������� ������������.
		���������� ������ � ����������� ������� �������� ����������.*/
		wstring analizer(math_obj* CE);
		/*������� �������� ������ ����� � �������������� ����.
		���������� 0 � ������ ��������� ����������.
		-1 -  ������.*/
		int input_to_analize(data_list* all_math_data);
	}
}
#endif //CORE_H