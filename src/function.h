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
			unsigned int num_var;	//количество переменных функции
			//math_obj* point_left; //наследуется (expression)
			math_obj* point_collar; //указатель на список переменных
			//data_list* point_up;  //наследуется (math_obj)
		public:
			function();
			virtual ~function();

			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. ЧИСЛО
			double get_num();
			//Метод assing. ЧИСЛО
			void assing_num(long double _num);

			//Метод get. Поправка для перечисляемых элементов
			unsigned int get_num_var();
			//Метод assing. Поправка для перечисляемых элементов
			void assing_num_var(unsigned int _num);

			//Метод get. УКАЗАТЕЛЬ "воротник"
			math_obj* get_pc();
			//Метод assing. УКАЗАТЕЛЬ "воротник"
			void assing_pc(math_obj* _pointer);

		};

		class funct_defnd :
			public function
		{
		protected:
			wstring name;//имя функции
			//unsigned int num_var;	//наследуется (function), количество переменных функции
			//math_obj* point_left; //наследуется (expression)
			//math_obj* point_collar; //наследуется (function)
			//data_list* point_up;  //наследуется (math_obj)
		public:
			funct_defnd();
			virtual ~funct_defnd();

			//Метод get. ТИП
			virtual flags get_class_type();

			//Метод get. ИМЯ
			wstring get_name();
			//Метод assing. ИМЯ
			void assing_name(wstring _name);
		};


		class funct_arg_c :
			public funct_defnd
		{
		protected :
			//wstring name;//наследуется (funct_defnd),имя функции
			//unsigned int num_var;	//наследуется (function), количество переменных функции
			//math_obj* point_left; //наследуется (expression)
			math_obj* point_right; //указатель на список аргументов
			//math_obj* point_collar; //наследуется (function)
			//data_list* point_up;  //наследуется (math_obj)
		public:
			funct_arg_c();
			~funct_arg_c();

			//Метод get. ТИП
			flags get_class_type();

			//Метод get. УКАЗАТЕЛЬ "правый рукав"
			math_obj* get_pr();
			//Метод assing. УКАЗАТЕЛЬ "правый рукав"
			void assing_pr(math_obj* _pointer);
		};


		class funct_arg_v :
			public funct_defnd
		{
		protected:
			//wstring name;//наследуется (funct_defnd),имя функции
			//unsigned int num_var;	//наследуется (function), количество переменных функции
			//math_obj* point_left; //наследуется (expression)
			math_obj* point_right; //указатель на список аргументов
			//math_obj* point_collar; //наследуется (function)
			//data_list* point_up;  //наследуется (math_obj)
		public:
			funct_arg_v();
			~funct_arg_v();

			//Метод get. ТИП
			flags get_class_type();

			//Метод get. УКАЗАТЕЛЬ "правый рукав"
			math_obj* get_pr();
			//Метод assing. УКАЗАТЕЛЬ "правый рукав"
			void assing_pr(math_obj* _pointer);
		};
	}
}
#endif //!FUNCTION_H
