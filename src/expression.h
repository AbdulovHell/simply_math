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

			//Ìועמה get. ÒÈÏ
			flags get_class_type();

			//Ìועמה get. ÐÀÇÌÅÐ
			size_t get_sizeof();

			//Ìועמה get. ÝËÅÌÅÍÒ
			void* get_this();

			int get_priority();

			void copy_to(void * _ptr);

			bool copy(expression* _original);
		};



	}
}
#endif  // !EXPRESSION_H

