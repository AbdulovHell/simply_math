#include "expression.h"

namespace Project {
	namespace Core {
		using namespace std;
		
		flags expression::get_class_type()
		{
			return flags::expression;
		}

		size_t expression::get_sizeof()
		{
			return sizeof(*this);
		}

		void * expression::get_this()
		{
			return this;
		}

		int expression::get_priority()
		{
			return 0;
		}

		void expression::copy_to(void * _ptr)
		{
			expression temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			expression *place = (expression*)_ptr;			
			place->copy(this);
		}

		bool expression::copy(expression * _original)
		{
			if (_original->get_class_type() == flags::expression) {//копирование ограничено элементами данного класса.
				math_obj::copy(_original);
				return true;
			}
			return false;
		}
	}
}
