#pragma once
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "math_obj.h"
namespace Project {
	namespace Core {
		using namespace std;
		class expression :
			virtual public math_obj
		{
		protected:

		public:
			expression() : math_obj(leaf_ptr(), leaf_ptr(), leaf_ptr()) {}
			expression(leaf_ptr _pl) : math_obj(_pl, leaf_ptr(), leaf_ptr()) {}

			//����� get. ���
			flags get_class_type();

			//����� get. ������
			size_t get_sizeof();

			//����� get. �������
			void* get_this();

			int get_priority();

			void copy_to(void * _ptr);

			bool copy(expression* _original);
		};



	}
}
#endif  // !EXPRESSION_H

