#include "function.h"
namespace Project {
	namespace Core {
		using namespace std;


		function::function()
		{
		}

		function::~function()
		{
		}
		flags function::get_class_type()
		{
			return flags::function;
		}
		double function::get_num()
		{
			return (double)num_var;
		}

		void function::assing_num(long double _num)
		{
			num_var = (unsigned int)_num;
		}

		unsigned int function::get_num_var()
		{
			return num_var;
		}

		void function::assing_num_var(unsigned int _num)
		{
			num_var = _num;
		}

		math_obj * function::get_pc()
		{
			return point_collar;
		}

		void function::assing_pc(math_obj * _pointer)
		{
			point_collar = _pointer;
		}



		funct_defnd::funct_defnd()
		{
		}

		funct_defnd::~funct_defnd()
		{
		}

		flags funct_defnd::get_class_type()
		{
			return flags::funct_defnd;
		}

		wstring funct_defnd::get_name()
		{
			return name;
		}

		void funct_defnd::assing_name(wstring _name)
		{
			name = _name;
		}



		funct_arg_c::funct_arg_c()
		{
		}
		funct_arg_c::~funct_arg_c()
		{
		}
		flags funct_arg_c::get_class_type()
		{
			return flags::funct_arg_c;
		}

		math_obj * funct_arg_c::get_pr()
		{
			return point_right;
		}

		void funct_arg_c::assing_pr(math_obj * _pointer)
		{
			point_right = _pointer;
		}



		funct_arg_v::funct_arg_v()
		{
		}
		funct_arg_v::~funct_arg_v()
		{
		}
		flags funct_arg_v::get_class_type()
		{
			return flags::funct_arg_v;
		}
		math_obj * funct_arg_v::get_pr()
		{
			return point_right;
		}
		void funct_arg_v::assing_pr(math_obj * _pointer)
		{
			point_right = _pointer;
		}
	}
}