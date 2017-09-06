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

		expression::~expression()
		{
		}

		flags expression::get_class_type()
		{
			return flags::expression;
		}

		size_t expression::get_sizeof()
		{
			return sizeof(*this);
		}		

		leaf_ptr expression::get_pl()
		{
			return point_left;
		}

		void expression::assing_pl(leaf_ptr& _pointer)
		{
			point_left = _pointer;
		}

		void expression::copy_to(void * _ptr)
		{			
			expression *place;
			place = new (_ptr) expression();		
			int s = sizeof(*place);
			int s2 = place->get_sizeof();
			place->copy(this);
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
