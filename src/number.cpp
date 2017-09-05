#include "number.h"

namespace Project {
	namespace Core {
		using namespace std;

		number::number():math_obj()
		{
			num = 0;
		}

		number::number(long double _num) : math_obj()
		{
			num = _num;
		}

		number::~number()
		{
		}

		long double number::get_num()
		{
			return num;
		}

		void number::assing_num(long double _num)
		{
			num = _num;
		}

		leaf_ptr number::get_pl()
		{
			return leaf_ptr();
		}

		void number::assing_pl(leaf_ptr& _pointer)
		{
		}

		leaf_ptr number::get_pr()
		{
			return leaf_ptr();
		}

		void number::assing_pr(leaf_ptr _pointer)
		{
		}

		leaf_ptr number::get_pc()
		{
			return leaf_ptr();
		}

		void number::assing_pc(leaf_ptr _pointer)
		{
		}

		int number::get_priority()
		{
			return 0;
		}

		void number::copy_to(void * _ptr)
		{
			number temp = number();
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			number *place = (number*)_ptr;			
			place->copy(this);
		}

		math_obj * number::copy(math_obj * _original)
		{
			if (_original->get_class_type() == flags::number) {//копирование ограничено элементами данного класса.
				this->num = _original->get_num();
				return this;
			}			
			return nullptr;
		}
				

		flags number::get_class_type()
		{//number полагается вещественным
			return flags::number;
		}

		void * number::get_this()
		{
			return this;
		}

		size_t number::get_sizeof()
		{
			return sizeof(*this);
		}

		wstring number::get_name()
		{
			return wstring();
		}

		void number::assing_name(wstring _name)
		{
		}
		
	}
}