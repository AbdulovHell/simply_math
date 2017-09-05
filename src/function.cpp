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
		long double function::get_num()
		{
			return (double)num_var;
		}

		void function::assing_num(long double _num)
		{
			num_var = (unsigned int)_num;
		}

		uint32_t function::get_num_var()
		{
			return num_var;
		}

		void function::assing_num_var(unsigned int _num)
		{
			num_var = _num;
		}

		leaf_ptr function::get_pc()
		{
			return point_collar;
		}

		void function::assing_pc(leaf_ptr _pointer)
		{
			point_collar = _pointer;
		}

		void function::copy_to(void * _ptr)
		{
			function temp = function();
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			function *place = (function*)_ptr;			
			place->copy(this);
		}

		math_obj * function::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::function || type == flags::expression) {
				this->point_left = _original->get_pl();
				this->num_var =(uint32_t) _original->get_num();
				this->point_collar = _original->get_pc();

				return this;
			}
			return nullptr;
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

		void funct_defnd::copy_to(void * _ptr)
		{
			funct_defnd temp = funct_defnd();
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			funct_defnd *place = (funct_defnd*)_ptr;			
			place->copy(this);
		}

		math_obj * funct_defnd::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::function || type == flags::expression|| type == flags::funct_defnd) {
				this->point_left = _original->get_pl();
				this->num_var = (uint32_t)_original->get_num();
				this->point_collar = _original->get_pc();
				this->name = _original->get_name();
				return this;
			}
			return nullptr;
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

		leaf_ptr funct_arg_c::get_pr()
		{
			return point_right;
		}

		void funct_arg_c::assing_pr(leaf_ptr _pointer)
		{
			point_right = _pointer;
		}

		void funct_arg_c::copy_to(void * _ptr)
		{
			funct_arg_c temp = funct_arg_c();
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			funct_arg_c *place = (funct_arg_c*)_ptr;			
			place->copy(this);
		}

		math_obj * funct_arg_c::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::function || type == flags::expression || type == flags::funct_defnd || type == flags::funct_arg_c) {
				this->point_left = _original->get_pl();
				this->num_var = (uint32_t)_original->get_num();
				this->point_collar = _original->get_pc();
				this->name = _original->get_name();
				this->point_right = _original->get_pr();
				return this;
			}
			return nullptr;
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
		leaf_ptr funct_arg_v::get_pr()
		{
			return point_right;
		}
		void funct_arg_v::assing_pr(leaf_ptr _pointer)
		{
			point_right = _pointer;
		}
		void funct_arg_v::copy_to(void * _ptr)
		{
			funct_arg_v temp = funct_arg_v();
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			funct_arg_v *place = (funct_arg_v*)_ptr;			
			place->copy(this);
		}

		math_obj * funct_arg_v::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::function || type == flags::expression || type == flags::funct_defnd || type == flags::funct_arg_v) {
				this->point_left = _original->get_pl();
				this->num_var = (uint32_t)_original->get_num();
				this->point_collar = _original->get_pc();
				this->name = _original->get_name();
				this->point_right = _original->get_pr();
				return this;
			}
			return nullptr;
		}
	}
}