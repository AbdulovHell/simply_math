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

		expression::expression(math_obj * _pl)
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

		math_obj * expression::get_this()
		{
			return this;
		}

		wstring expression::get_name()
		{
			return wstring();
		}

		void expression::assing_name(wstring _name)
		{

		}

		long double expression::get_num()
		{
			return NAN;
		}

		void expression::assing_num(long double _num)
		{
		}

		math_obj * expression::get_pl()
		{
			return point_left;
		}

		void expression::assing_pl(math_obj * _pointer)
		{
			point_left = _pointer;
		}

		math_obj * expression::get_pr()
		{
			return nullptr;
		}

		void expression::assing_pr(math_obj * _pointer)
		{
		}

		math_obj * expression::get_pc()
		{
			return nullptr;
		}

		void expression::assing_pc(math_obj * _pointer)
		{
		}


	}
}
