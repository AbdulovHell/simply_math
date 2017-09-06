#include "number.h"

namespace Project {
	namespace Core {
		using namespace std;

		long double number::get_num()
		{
			return num;
		}

		void number::assing_num(long double _num)
		{
			num = _num;
		}

		int number::get_priority()
		{
			return 0;
		}

		void number::copy_to(void * _ptr)
		{
			number temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			number *place = (number*)_ptr;			
			place->copy(this);
		}

		bool number::copy(number * _original)
		{
			if (_original->get_class_type() == flags::number) {//копирование ограничено элементами данного класса.
				this->num = _original->get_num();
				math_obj::copy(_original);
				return true;
			}
			return false;
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

	}
}