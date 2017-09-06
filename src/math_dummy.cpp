#include "math_dummy.h"

namespace Project {
	namespace Core {
		using namespace std;

		flags math_dummy::get_class_type()
		{
			return flags::math_dummy;
		}

		size_t math_dummy::get_sizeof()
		{
			return sizeof(*this);
		}

		void * math_dummy::get_this()
		{
			return this;
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

		int math_dummy::get_priority()
		{
			return 0;
		}
		void math_dummy::copy_to(void * _ptr)
		{
			math_dummy temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			math_dummy *place = (math_dummy*)_ptr;			
			place->copy(this);
		}
		bool math_dummy::copy(math_dummy * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::math_dummy) {
				num_var = (uint32_t)_original->get_num();
				math_obj::copy(_original);
				return true;
			}
			return false;
		}
	}
}