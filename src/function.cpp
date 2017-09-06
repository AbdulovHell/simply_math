#include "function.h"
namespace Project {
	namespace Core {
		using namespace std;

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

		void function::copy_to(void * _ptr)
		{
			function temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			function *place = (function*)_ptr;			
			place->copy(this);
		}

		bool function::copy(function * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::function || type == flags::expression) {
				//this->point_left = _original->get_pl();
				this->num_var = (uint32_t)_original->get_num();
				//this->point_collar = _original->get_pc();
				math_obj::copy(_original);
				return true;
			}
			return false;
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
			funct_defnd temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			funct_defnd *place = (funct_defnd*)_ptr;			
			place->copy(this);
		}

		bool funct_defnd::copy(funct_defnd * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::function || type == flags::expression || type == flags::funct_defnd) {
				//this->point_left = _original->get_pl();
				this->num_var = (uint32_t)_original->get_num();
				//this->point_collar = _original->get_pc();
				this->name = _original->get_name();
				math_obj::copy(_original);
				return true;
			}
			return false;
		}

		flags funct_arg_c::get_class_type()
		{
			return flags::funct_arg_c;
		}

		void funct_arg_c::copy_to(void * _ptr)
		{
			funct_arg_c temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			funct_arg_c *place = (funct_arg_c*)_ptr;			
			place->copy(this);
		}

		bool funct_arg_c::copy(funct_arg_c * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::function || type == flags::expression || type == flags::funct_defnd || type == flags::funct_arg_c) {
				//this->point_left = _original->get_pl();
				this->num_var = (uint32_t)_original->get_num();
				//this->point_collar = _original->get_pc();
				this->name = _original->get_name();
				//this->point_right = _original->get_pr();
				math_obj::copy(_original);
				return true;
			}
			return false;
		}

		flags funct_arg_v::get_class_type()
		{
			return flags::funct_arg_v;
		}
		void funct_arg_v::copy_to(void * _ptr)
		{
			funct_arg_v temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			funct_arg_v *place = (funct_arg_v*)_ptr;			
			place->copy(this);
		}

		bool funct_arg_v::copy(funct_arg_v * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::function || type == flags::expression || type == flags::funct_defnd || type == flags::funct_arg_v) {
				//this->point_left = _original->get_pl();
				this->num_var = (uint32_t)_original->get_num();
				//this->point_collar = _original->get_pc();
				this->name = _original->get_name();
				//this->point_right = _original->get_pr();
				math_obj::copy(_original);
				return true;
			}
			return false;
		}
	}
}