#include "constant.h"

namespace Project {
	namespace Core {
		using namespace std;

		constant::constant() :number()
		{
			name = wstring();
			point_left = nullptr;  
		}

		constant::constant(math_obj * _pl):number()
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
			point_left = nullptr;
		}

		constant::constant(math_obj * _pl, long double _num, wstring _name):math_obj()
		{
			point_left = _pl;
			num = _num;
			name = _name;
		}

		constant::constant(math_obj * _pl, long double _num, wstring _name, data_list * _pu)
		{
			point_left = _pl;
			num = _num;
			name = _name;
			point_up = _pu;
		}

		constant::~constant()
		{
		}
		flags constant::get_class_type()
		{
			return flags::constant;
		}
	}
}