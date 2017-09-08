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

		

		uint16_t number::copy_to(void * _ptr)
		{
			math_obj *place = new(_ptr) number();
			place->copy(this);
			void*k = place->get_this_void();
			return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
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

		size_t number::get_sizeof()
		{
			return sizeof(*this);
		}

		math_obj * number::get_this()
		{
			 return this; 
		}
			
		
	}
}