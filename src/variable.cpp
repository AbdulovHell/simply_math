#include "variable.h"

namespace Project {
	namespace Core {
		using namespace std;

		variable::variable() :math_dummy()
		{
			name = wstring();			
		}

		variable::variable(wstring _name) : math_dummy()
		{
			name = _name;			
		}

		variable::variable(wstring _name, math_obj * _pl, math_obj * _pr, math_obj * _pc) : math_dummy(_pl,_pr,_pc)
		{
			name = _name;			
		}

		variable::variable(math_obj * _pl, math_obj * _pr, math_obj * _pc) : math_dummy(_pl, _pr, _pc)
		{
			name = wstring();			
		}

		variable::variable(wstring _name, unsigned int _num, math_obj * _pl, math_obj * _pr, math_obj * _pc) : math_dummy(_num,_pl, _pr, _pc)
		{
			name = _name;			
		}

		variable::variable(unsigned int _num, math_obj * _pl, math_obj * _pr, math_obj * _pc) : math_dummy(_num, _pl, _pr, _pc)
		{
			name = wstring();			
		}

		variable::variable(wstring _name, data_list * _data)
		{
			name = _name;
			num_var = 0;
			point_left = nullptr;
			point_right = nullptr;
			point_collar = nullptr;
			point_up = _data;
		}

		variable::variable(variable * _original)
		{
			name = _original->get_name();
			num_var = _original->get_num_var();
			point_left = _original->get_pl();
			point_right = _original->get_pr();
			point_collar = _original->get_pc();
			point_up = _original->point_up;
		}

		variable::~variable()
		{
		}

		flags variable::get_class_type()
		{
			return flags::variable;
		}
		math_obj * variable::get_this()
		{
			return this;
		}
		wstring variable::get_name()
		{
			return name;
		}

		void variable::assing_name(wstring _name)
		{
			name = _name;
		}		

	}
}