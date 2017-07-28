#include "math_dummy.h"

namespace Project {
	namespace Core {
		using namespace std;

		math_dummy::math_dummy():math_obj()
		{
			num_var = 0;
			point_left = nullptr;
			point_right = nullptr;
			point_collar = nullptr;
		}

		math_dummy::math_dummy(math_obj * _pl, math_obj * _pr, math_obj * _pc) :math_obj()
		{
			num_var = 0;
			point_left = _pl;
			point_right = _pr;
			point_collar = _pc;
		}

		math_dummy::math_dummy(unsigned int _num, math_obj * _pl, math_obj * _pr, math_obj * _pc) :math_obj()
		{			
			num_var = _num;
			point_left = _pl;
			point_right = _pr;
			point_collar = _pc;
		}

		math_dummy::~math_dummy()
		{
		}

		flags math_dummy::get_class_type()
		{
			return flags::math_dummy;
		}

		math_obj * math_dummy::get_this()
		{
			return point_right;
		}
		
		double math_dummy::get_num()
		{
			return (double)num_var;
		}

		void math_dummy::assing_num(long double _num)
		{
			num_var = (unsigned int)_num;
		}

		unsigned int math_dummy::get_num_var()
		{
			return num_var;
		}

		void math_dummy::assing_num_var(unsigned int _num)
		{
			num_var = _num;
		}

		math_obj * math_dummy::get_pl()
		{
			return point_left;
		}

		void math_dummy::assing_pl(math_obj * _pointer)
		{
			point_left = _pointer;
		}

		math_obj * math_dummy::get_pr()
		{
			return point_right;
		}

		void math_dummy::assing_pr(math_obj * _pointer)
		{
			point_right = _pointer;
		}

		math_obj * math_dummy::get_pc()
		{
			return point_collar;
		}

		void math_dummy::assing_pc(math_obj * _pointer)
		{
			point_collar = _pointer;
		}
	}
}