#pragma once
#ifndef NUMBER_H
#define NUMBER_H
#include "classes.h"
namespace Project {
	namespace Core {
		using namespace std;
		class number :
			virtual public math_obj
		{
		protected:
			//��� ����� ����� ��������.
			long double num;
			//data_list* point_up;  //����������� (math_obj), �� ��������� �� ������������.
		public:

			number();
			number(long double _num);
			virtual ~number();

			//����� get. �����
			virtual double get_num();
			//����� assing. �����
			virtual void assing_num(long double _num);
			
			//����� get. ���
			virtual flags get_class_type();

			//����� get. �������
			virtual math_obj* get_this();
		};
	}
}
#endif  // !NUMBER_H