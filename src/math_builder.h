#pragma once
#ifndef MATH_BUILDER_H
#define MATH_BUILDER_H

#include "common.h"

#include "IO.h"
#include "error.h"
#include "data_list.h"
#include "function.h"
#include "constant.h"
#include "variable.h"
#include "number.h"
#include "operations.h"
#include "math_vector.h"


namespace Project {
	namespace Core {
		using namespace std;
		class math_builder  //������ ����� ������������ ��� ���������� ������ �������� �� ������ �����.
			//�������� ������������ �����������: � ���� ������ ������� ������� ���� ���������, � � ��� �������� - �������/�������.
		{
		protected:
			math_obj* object;
			data_list* data_reference;
		public:
			math_builder();
			math_builder(wchar_t* strPtr, wchar_t*endPtr, data_list* _up);
			~math_builder();
			/*����� ������ ������ �������������� �������� ��� ������� ��������������� �������.
			������ ����� � ����� ������ ����� ���� ����������.
			����� ���������� ��������� �� ��������� ��������� ������ math_obj, �������������� ����� �������� (������) ������, ���� �� ����� ��� ���������� ����������.
			NULL � ������ �����-���� ������, ��. error.h.*/
			math_obj* build(wchar_t* strPtr, wchar_t*endPtr);
		};
	}
}


#endif // !MATH_BUILDER_H
