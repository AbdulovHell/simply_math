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

		math_obj * number::get_pl()
		{
			return nullptr;
		}

		void number::assing_pl(math_obj * _pointer)
		{
		}

		math_obj * number::get_pr()
		{
			return nullptr;
		}

		void number::assing_pr(math_obj * _pointer)
		{
		}

		math_obj * number::get_pc()
		{
			return nullptr;
		}

		void number::assing_pc(math_obj * _pointer)
		{
		}
				

		flags number::get_class_type()
		{//number полагается вещественным
			return flags::number;
		}

		math_obj * number::get_this()
		{
			return this;
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