#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
#include "expression.h"
namespace Project {
	namespace Core {
		using namespace std;
		class function ://класс неявно заданных функций (без имени)
			public expression
		{
		protected:			
			uint32_t num_var;	//количество переменных функции
			//uint32_t point_left; //наследуется (expression)
			leaf_ptr point_collar; //указатель на список переменных
			
		public:
			function();
			virtual ~function();

			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. ЧИСЛО
			long double get_num();
			//Метод assing. ЧИСЛО
			void assing_num(long double _num);

			//Метод get. Поправка для перечисляемых элементов
			unsigned int get_num_var();
			//Метод assing. Поправка для перечисляемых элементов
			void assing_num_var(unsigned int _num);

			//Метод get. УКАЗАТЕЛЬ "воротник"
			leaf_ptr get_pc();
			//Метод assing. УКАЗАТЕЛЬ "воротник"
			void assing_pc(leaf_ptr _pointer);

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);


		};

		class funct_defnd :
			public function
		{
		protected:
			wstring name;//имя функции
			//unsigned int num_var;	//наследуется (function), количество переменных функции
			//math_ptr point_left; //наследуется (expression)
			//math_ptr point_collar; //наследуется (function)
			
		public:
			funct_defnd();
			virtual ~funct_defnd();

			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. ИМЯ
			wstring get_name();
			//Метод assing. ИМЯ
			void assing_name(wstring _name);

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};


		class funct_arg_c :
			public funct_defnd
		{
		protected :
			//wstring name;//наследуется (funct_defnd),имя функции
			//unsigned int num_var;	//наследуется (function), количество переменных функции
			//math_ptr point_left; //наследуется (expression)
			leaf_ptr point_right; //указатель на список аргументов
			//math_ptr point_collar; //наследуется (function)
			
		public:
			funct_arg_c();
			~funct_arg_c();

			//Метод get. ТИП
			flags get_class_type();

			//Метод get. УКАЗАТЕЛЬ "правый рукав"
			leaf_ptr get_pr();
			//Метод assing. УКАЗАТЕЛЬ "правый рукав"
			void assing_pr(leaf_ptr _pointer);

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};


		class funct_arg_v :
			public funct_defnd
		{
		protected:
			//wstring name;//наследуется (funct_defnd),имя функции   //28 байт! 28 байт только сам объект wstring.
			//unsigned int num_var;	//наследуется (function), количество переменных функции  
			//math_ptr point_left; //наследуется (expression)
			leaf_ptr point_right; //указатель на список аргументов
			//math_ptr point_collar; //наследуется (function)
			
		public:
			funct_arg_v();
			~funct_arg_v();

			//Метод get. ТИП
			flags get_class_type();

			//Метод get. УКАЗАТЕЛЬ "правый рукав"
			leaf_ptr get_pr();
			//Метод assing. УКАЗАТЕЛЬ "правый рукав"
			void assing_pr(leaf_ptr _pointer);

			virtual void copy_to(void * _ptr);

			virtual math_obj* copy(math_obj* _original);
		};
	}
}
#endif //!FUNCTION_H
