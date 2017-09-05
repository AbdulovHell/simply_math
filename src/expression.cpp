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

		void * expression::get_this()
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

		leaf_ptr expression::get_pr()
		{
			return leaf_ptr();
		}

		void expression::assing_pr(leaf_ptr _pointer)
		{
		}

		leaf_ptr expression::get_pc()
		{
			return leaf_ptr();
		}

		void expression::assing_pc(leaf_ptr _pointer)
		{
		}

		int expression::get_priority()
		{
			return 0;
		}

		void expression::copy_to(void * _ptr)
		{
			expression temp = expression();
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			expression *place = (expression*)_ptr;			
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
