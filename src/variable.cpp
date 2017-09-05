#include "variable.h"

namespace Project {
	namespace Core {
		using namespace std;

		variable::variable() :math_dummy()
		{
			name = wstring();			
		}

		variable::variable(wstring _name) : math_dummy()
		{
			name = _name;			
		}

		variable::variable(wstring _name, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) : math_dummy(_pl,_pr,_pc)
		{
			name = _name;			
		}

		variable::variable(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) : math_dummy(_pl, _pr, _pc)
		{
			name = wstring();			
		}

		variable::variable(wstring _name, uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) : math_dummy(_num,_pl, _pr, _pc)
		{
			name = _name;			
		}

		variable::variable(uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) : math_dummy(_num, _pl, _pr, _pc)
		{
			name = wstring();			
		}
				

		variable::variable(variable * _original)
		{
			name = _original->get_name();
			num_var = _original->get_num_var();
			point_left = _original->get_pl();
			point_right = _original->get_pr();
			point_collar = _original->get_pc();			
		}

		variable::~variable()
		{
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
			variable temp = variable();
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			variable *place = (variable*)_ptr;			
			place->copy(this);
		}

		math_obj * variable::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::math_dummy||type==flags::math_dummy) {
				this->point_left = _original->get_pl();
				this->num_var = (uint32_t)_original->get_num();
				this->point_collar = _original->get_pc();
				this->point_right = _original->get_pr();
				this->name = _original->get_name();
				return this;
			}
			return nullptr;
		}

	}
}