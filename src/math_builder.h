#pragma once
#ifndef MATH_BUILDER_H
#define MATH_BUILDER_H
#include "classes.h"

namespace Project {
	namespace Core {
		using namespace std;
		class math_builder
		{
		protected:
			math_obj* object;
		public:
			math_builder();
			~math_builder();
			/*����� ������ ������ �������������� �������� ��� ������� ��������������� �������.
			������ ����� � ����� ������ ����� ���� ����������.
			����� ���������� ��������� �� ��������� ��������� ������ math_obj, �������������� ����� �������� (������) ������, ���� �� ����� ��� ���������� ����������.
			NULL � ������ �����-���� ������, ��. error.h.*/
			math_obj* build_tree(wchar_t* strPtr, wchar_t*endPtr);
		};
	}
}


#endif // !MATH_BUILDER_H
