#include "expression.h"

namespace Project {
	namespace Core {
		using namespace std;
		expression::expression() :math_obj()
		{
			point_left = leaf_ptr();
		}

		expression::expression(leaf_ptr _pl) : math_obj()
		{
			point_left = _pl;
		}

		expression::expression(expression &_original)
		{
			this->point_left = _original.point_left;
		}

		expression::~expression()
		{
		}

		flags expression::get_class_type()
		{
			return flags::expression;
		}

		size_t expression::get_sizeof() { return sizeof(*this); }

		math_obj* expression::get_this() { return this; }
		
		long double expression::get_num()
		{
			return 0;
		}

		void expression::assing_num(long double _num)
		{
		}

		leaf_ptr expression::get_pl()
		{
			return point_left;
		}

		void expression::assing_pl(leaf_ptr& _pointer)
		{
			point_left = _pointer;
		}
				
		uint16_t expression::copy_to(void * _ptr)
		{			
			math_obj *place = new(_ptr) expression();			
			place->copy(this);
			return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
		}

		math_obj * expression::copy(math_obj * _original)
		{
			if (_original->get_class_type() == flags::expression) {//копирование ограничено элементами данного класса.
				this->point_left = _original->get_pl();
				return this;
			}
			return nullptr;
		}


	}
}
