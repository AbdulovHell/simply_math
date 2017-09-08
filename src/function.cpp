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
		size_t function::get_sizeof()
		{
			return sizeof(*this);
		}
		math_obj * function::get_this()
		{
			return this;
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

		uint16_t function::copy_to(void * _ptr)
		{
			math_obj *place = new(_ptr) function();
			place->copy(this);
			return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
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

		size_t funct_defnd::get_sizeof()
		{
			return sizeof(*this);
		}

		math_obj * funct_defnd::get_this()
		{
			return this;
		}

		wstring funct_defnd::get_name()
		{
			return name;
		}

		void funct_defnd::assing_name(wstring _name)
		{
			name = _name;
		}

		uint16_t funct_defnd::copy_to(void * _ptr)
		{
			math_obj *place = new(_ptr) funct_defnd();
			place->copy(this);
			return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
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

		size_t funct_arg_c::get_sizeof()
		{
			return sizeof(*this);
		}

		math_obj * funct_arg_c::get_this()
		{
			return this;
		}

		leaf_ptr funct_arg_c::get_pr()
		{
			return point_right;
		}

		void funct_arg_c::assing_pr(leaf_ptr _pointer)
		{
			point_right = _pointer;
		}

		uint16_t funct_arg_c::copy_to(void * _ptr)
		{
			math_obj *place = new(_ptr) funct_arg_c();
			place->copy(this);
			return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
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
		size_t funct_arg_v::get_sizeof()
		{
			return sizeof(*this);
		}
		math_obj * funct_arg_v::get_this()
		{
			return this;
		}
		leaf_ptr funct_arg_v::get_pr()
		{
			return point_right;
		}
		void funct_arg_v::assing_pr(leaf_ptr _pointer)
		{
			point_right = _pointer;
		}
		uint16_t funct_arg_v::copy_to(void * _ptr)
		{
			math_obj *place = new(_ptr) funct_arg_v();
			place->copy(this);
			return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
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