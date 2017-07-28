#include "expression.h"

namespace Project {
	namespace Core {
		using namespace std;
		expression::expression() :math_obj()
		{
			point_left = nullptr;
		}

		expression::expression(math_obj * _pl) : math_obj()
		{
			point_left = _pl;
		}

		expression::expression(math_obj * _pl, data_list * _pu)
		{
			point_left = _pl;
			point_up = _pu;
		}

		expression::~expression()
		{
		}

		flags expression::get_class_type()
		{
			return flags::expression;
		}

		math_obj * expression::get_this()
		{
			return this;
		}

		math_obj * expression::get_pl()
		{
			return point_left;
		}

		void expression::assing_pl(math_obj * _pointer)
		{
			point_left = _pointer;
		}


	}
}
