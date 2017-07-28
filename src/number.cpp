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

		double number::get_num()
		{
			return num;
		}

		void number::assing_num(long double _num)
		{
			num = _num;
		}
				

		flags number::get_class_type()
		{//number полагается вещественным
			return flags::number;
		}

		math_obj * number::get_this()
		{
			return this;
		}
		
	}
}