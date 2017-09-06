#include "math_dummy.h"

namespace Project {
	namespace Core {
		using namespace std;

		math_dummy::math_dummy():math_obj()
		{
			num_var = 0;
			point_left = leaf_ptr();
			point_right = leaf_ptr();
			point_collar = leaf_ptr();
		}

		math_dummy::math_dummy(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) :math_obj()
		{
			num_var = 0;
			point_left = _pl;
			point_right = _pr;
			point_collar = _pc;
		}

		math_dummy::math_dummy(uint32_t _num, leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) :math_obj()
		{			
			num_var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = _pc;
		}

		math_dummy::~math_dummy()
		{
		}

		flags math_dummy::get_class_type()
		{
			return flags::math_dummy;
		}

		size_t math_dummy::get_sizeof()
		{
			return sizeof(*this);
		}		
		
		long double math_dummy::get_num()
		{
			return (double)num_var;
		}

		void math_dummy::assing_num(long double _num)
		{
			num_var = (unsigned int)_num;
		}

		uint32_t math_dummy::get_num_var()
		{
			return num_var;
		}

		void math_dummy::assing_num_var(unsigned int _num)
		{
			num_var = _num;
		}

		leaf_ptr math_dummy::get_pl()
		{
			return point_left;
		}

		void math_dummy::assing_pl(leaf_ptr& _pointer)
		{
			point_left = _pointer;
		}

		leaf_ptr math_dummy::get_pr()
		{
			return point_right;
		}

		void math_dummy::assing_pr(leaf_ptr _pointer)
		{
			point_right = _pointer;
		}

		leaf_ptr math_dummy::get_pc()
		{
			return point_collar;
		}

		void math_dummy::assing_pc(leaf_ptr _pointer)
		{
			point_collar = _pointer;
		}
		
		void math_dummy::copy_to(void * _ptr)
		{
			math_dummy *place = new (_ptr) math_dummy();
			place->copy(this);
		}
		math_obj * math_dummy::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::math_dummy) {
				this->point_left = _original->get_pl();
				this->num_var = (uint32_t)_original->get_num();
				this->point_collar = _original->get_pc();				
				this->point_right = _original->get_pr();
				return this;
			}
			return nullptr;
		}
	}
}