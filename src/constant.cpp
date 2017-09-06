#include "constant.h"

namespace Project {
	namespace Core {
		using namespace std;

		flags constant::get_class_type()
		{
			return flags::constant;
		}
		size_t constant::get_sizeof()
		{
			return sizeof(*this);
		}
		void * constant::get_this()
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
		int constant::get_priority()
		{
			return 0;
		}
		void constant::copy_to(void * _ptr)
		{
			constant temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			constant *place = (constant*)_ptr;			
			place->copy(this);
		}
		bool constant::copy(constant * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::constant || type == flags::expression || type == flags::number) {//копирование ограничено элементами данного класса.
				math_obj::copy(_original);
				this->name = _original->get_name();
				this->num = _original->get_num();
				return true;
			}
			return false;
		}
	}
}