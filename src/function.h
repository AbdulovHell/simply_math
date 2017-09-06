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
			//leaf_ptr point_collar; //указатель на список переменных
			
		public:
			function() :expression() {};
			
			//Метод get. ТИП
			flags get_class_type();

			//Метод get. ЧИСЛО
			long double get_num();
			//Метод assing. ЧИСЛО
			void assing_num(long double _num);

			//Метод get. Поправка для перечисляемых элементов
			unsigned int get_num_var();
			//Метод assing. Поправка для перечисляемых элементов
			void assing_num_var(unsigned int _num);

			void copy_to(void * _ptr);

			bool copy(function* _original);
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
			funct_defnd() :function() {};
			
			//Метод get. ТИП
			flags get_class_type();

			//Метод get. ИМЯ
			wstring get_name();
			//Метод assing. ИМЯ
			void assing_name(wstring _name);

			void copy_to(void * _ptr);

			bool copy(funct_defnd* _original);
		};


		class funct_arg_c :
			public funct_defnd
		{
		protected :
			//wstring name;//наследуется (funct_defnd),имя функции
			//unsigned int num_var;	//наследуется (function), количество переменных функции
			//math_ptr point_left; //наследуется (expression)
			//leaf_ptr point_right; //указатель на список аргументов
			//math_ptr point_collar; //наследуется (function)
			
		public:
			funct_arg_c() :funct_defnd() {};
			
			//Метод get. ТИП
			flags get_class_type();

			void copy_to(void * _ptr);

			bool copy(funct_arg_c* _original);
		};


		class funct_arg_v :
			public funct_defnd
		{
		protected:
			//wstring name;//наследуется (funct_defnd),имя функции   //28 байт! 28 байт только сам объект wstring.
			//unsigned int num_var;	//наследуется (function), количество переменных функции  
			//math_ptr point_left; //наследуется (expression)
			//leaf_ptr point_right; //указатель на список аргументов
			//math_ptr point_collar; //наследуется (function)
			
		public:
			funct_arg_v() :funct_defnd() {};
			
			//Метод get. ТИП
			flags get_class_type();

			void copy_to(void * _ptr);

			bool copy(funct_arg_v* _original);
		};
	}
}
#endif //!FUNCTION_H
