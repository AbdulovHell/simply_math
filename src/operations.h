#pragma once
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "math_obj.h"
#include "number.h"
#include "error.h"

#define MATH_OBJ(x) safe_cast<math_obj*>(x)

namespace Project {
	namespace Core {
		using namespace std;

		class math_obj;

		class operations :
			public math_obj {
		protected:
			leaf_ptr prioritize_rec(int* _p);
		public:

			operations() : math_obj() {};
			operations(leaf_ptr _pl) : math_obj(_pl, leaf_ptr(), leaf_ptr()) {};
			operations(leaf_ptr _pl, leaf_ptr _pr) : math_obj(_pl, _pr, leaf_ptr()) {};
			operations(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) :math_obj(_pl, _pr, _pc) {};

			//Метод get. ТИП
			flags get_class_type();

			//Метод get. РАЗМЕР
			size_t get_sizeof();

			//Метод get. ЭЛЕМЕНТ
			void* get_this();

			//Возвращает результат выполнения операции в виде мат. объекта. 
			virtual math_obj* get_result() = 0;

			bool define_operation(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this);

			leaf_ptr prioritize(int _priority);

			virtual int get_priority() = 0;

		};

		class addition :
			public operations {
		protected:
			//uint32_t point_left;		//наследуется (operations), левый операнд
			//uint32_t point_right;	//наследуется (operations), правый операнд
			//uint32_t point_collar;	//наследуется (operations), указатель на предыдущую операцию
		public:
			addition() : operations() {}
			addition(leaf_ptr _pl) : operations(_pl) {}
			addition(leaf_ptr _pl, leaf_ptr _pr) :operations(_pl, _pr) {}
			addition(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this);

			flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			//math_obj* get_this();

			//Метод get. ИМЯ (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			void copy_to(void* _ptr);

			bool copy(addition* _original);

		};

		class subtraction :
			public operations {
		protected:
			//uint32_t point_left;		//наследуется (operations), левый операнд
			//uint32_t point_right;	//наследуется (operations), правый операнд
			//uint32_t point_collar;	//наследуется (operations), указатель на предыдущую операцию

		public:
			subtraction() :operations() {}
			subtraction(leaf_ptr _pl) :operations(_pl) {}
			subtraction(leaf_ptr _pl, leaf_ptr _pr) :operations(_pl, _pr) {}
			subtraction(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) : operations(_pl, _pr, _pc) {}
			subtraction(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this);

			flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			//virtual math_obj* get_this();

			//Метод get. ИМЯ (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			void copy_to(void * _ptr);

			bool copy(subtraction* _original);
		};

		class multiplication :
			public operations {
		protected:
			//uint32_t point_left;		//наследуется (operations), левый операнд
			//uint32_t point_right;	//наследуется (operations), правый операнд
			//uint32_t point_collar;	//наследуется (operations), указатель на предыдущую операцию
		public:
			multiplication() :operations() {};
			multiplication(leaf_ptr _pl) :operations(_pl) {};
			multiplication(leaf_ptr _pl, leaf_ptr _pr) :operations(_pl,_pr) {};
			multiplication(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this);
			
			flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			//virtual math_obj* get_this();

			//Метод get. ИМЯ (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			void copy_to(void * _ptr);

			bool copy(multiplication* _original);
		};

		class division :
			public operations {
		protected:
			//uint32_t point_left;		//наследуется (operations), левый операнд
			//uint32_t point_right;	//наследуется (operations), правый операнд
			//uint32_t point_collar;	//наследуется (operations), указатель на предыдущую операцию
		public:
			division() :operations() {};
			division(leaf_ptr _pl) :operations(_pl) {};
			division(leaf_ptr _pl, leaf_ptr _pr) :operations(_pl, _pr) {};
			division(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this);

			flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			//virtual math_obj* get_this();

			//Метод get. ИМЯ (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			void copy_to(void * _ptr);

			bool copy(division* _original);
		};

		class power :
			public operations {
		protected:
			//uint32_t point_left;		//наследуется (operations), левый операнд
			//uint32_t point_right;	//наследуется (operations), правый операнд
			//uint32_t point_collar;	//наследуется (operations), указатель на предыдущую операцию
		public:
			power() :operations() {};
			power(leaf_ptr _pl) :operations(_pl) {};
			power(leaf_ptr _pl, leaf_ptr _pr) :operations(_pl,_pr) {};
			power(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this);

			flags get_class_type();

			//Метод get. ЭЛЕМЕНТ
			//virtual math_obj* get_this();

			//Метод get. ИМЯ (символ операции)
			wstring get_name();

			math_obj* get_result();

			int get_priority();

			void copy_to(void * _ptr);

			bool copy(power* _original);
		};
	}
}

#endif // !OPERATIONS_H




