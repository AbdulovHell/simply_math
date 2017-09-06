#pragma once
#ifndef MATH_DUNNY_H
#define MATH_DUNNY_H
#include "math_obj.h"
namespace Project {
	namespace Core {
		using namespace std;
		class math_dummy :
			public math_obj
		{
		protected:
			uint32_t num_var;	//����� �������� � ������ (�������)
		public:
			math_dummy() : math_obj() {};
			math_dummy(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) : math_obj(_pl, _pr, _pc) { num_var = 0; };
			math_dummy(uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) : math_obj(_pl, _pr, _pc) { num_var = _num; };
			//����� get. ���
			flags get_class_type();

			//����� get. ������
			size_t get_sizeof();

			//����� get. �������
			void* get_this();

			//����� get. �����
			long double get_num();
			//����� assing. �����
			void assing_num(long double _num);

			//����� get. �������� ��� ������������� ���������
			uint32_t get_num_var();
			//����� assing. �������� ��� ������������� ���������
			void assing_num_var(unsigned int _num);

			int get_priority();

			void copy_to(void* _ptr);

			bool copy(math_dummy* _original);
		};
	}
}
#endif //!MATH_DUNNY_H