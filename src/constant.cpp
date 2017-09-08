#include "constant.h"

namespace Project {
	namespace Core {
		using namespace std;

		constant::constant() :number()
		{
			name = wstring();
			point_left = leaf_ptr();
		}

		constant::constant(leaf_ptr _pl):number()
		{
			point_left = _pl;
		}

		constant::constant(long double _num):expression()
		{
			num = _num;
		}


		constant::constant(wstring _name) :number()
		{
			name = _name;
			point_left = leaf_ptr();
		}

		constant::constant(leaf_ptr _pl, long double _num, wstring _name):math_obj()
		{
			point_left = _pl;
			num = _num;
			name = _name;
		}

		
		constant::~constant()
		{
		}
		flags constant::get_class_type()
		{
			return flags::constant;
		}

		size_t constant::get_sizeof()
		{
			return sizeof(*this);
		}

		math_obj * constant::get_this()
		{
			return this; 
		}
		
		wstring constant::get_name()
		{
			return name;
		}
		void constant::assing_name(wstring _name)
		{
			name = _name;
		}
		long double constant::get_num()
		{
			return num;
		}
		void constant::assing_num(long double _num)
		{
			num = _num;
		}
		leaf_ptr constant::get_pl()
		{
			return point_left;
		}
		void constant::assing_pl(leaf_ptr& _pointer)
		{
			point_left = _pointer;
		}
		
		uint16_t constant::copy_to(void * _ptr)
		{
			math_obj *place = new(_ptr) constant();
			place->copy(this);
			return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
		}
		math_obj * constant::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::constant|| type == flags::expression|| type == flags::number) {//копирование ограничено элементами данного класса.
				this->point_left = _original->get_pl();
				this->name = _original->get_name();
				this->num = _original->get_num();
				return this;
			}
			return nullptr;
		}
	}
}