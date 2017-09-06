#include "variable.h"

namespace Project {
	namespace Core {
		using namespace std;

		variable::variable(variable * _original)
		{
			name = _original->get_name();
			num_var = _original->get_num_var();
			//point_left = _original->get_pl();
			//point_right = _original->get_pr();
			//point_collar = _original->get_pc();			
			math_obj::copy(_original);
		}

		flags variable::get_class_type()
		{
			return flags::variable;
		}
		void * variable::get_this()
		{
			return this;
		}
		wstring variable::get_name()
		{
			return name;
		}

		void variable::assing_name(wstring _name)
		{
			name = _name;
		}

		int variable::get_priority()
		{
			return 0;
		}

		void variable::copy_to(void * _ptr)
		{
			variable temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			variable *place = (variable*)_ptr;			
			place->copy(this);
		}

		bool variable::copy(variable * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::math_dummy || type == flags::math_dummy) {
				//this->point_left = _original->get_pl();
				this->num_var = (uint32_t)_original->get_num();
				//this->point_collar = _original->get_pc();
				//this->point_right = _original->get_pr();
				this->name = _original->get_name();
				math_obj::copy(_original);
				return true;
			}
			return false;
		}
	}
}