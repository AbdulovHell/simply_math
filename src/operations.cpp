#include "operations.h"

namespace Project {
	namespace Core {
		using namespace std;
		operations::operations() :math_obj()
		{
			point_left = nullptr;
			point_right = nullptr;
		}

		operations::operations(math_obj * _pl)
		{
			point_left = _pl;
			point_right = nullptr;
		}

		operations::operations(math_obj * _pl, math_obj * _pr)
		{
						
		}

		operations::~operations()
		{
		}

		math_obj * operations::get_pl()
		{
			return point_left;
		}

		void operations::assing_pl(math_obj * _pointer)
		{
			point_left = _pointer;
		}

		math_obj * operations::get_pr()
		{
			return point_right;
		}

		void operations::assing_pr(math_obj * _pointer)
		{
			point_right = _pointer;
		}

		math_obj * operations::get_pc()
		{
			return nullptr;
		}

		void operations::assing_pc(math_obj * _pointer)
		{
		}

		flags operations::get_class_type()
		{
			return flags::operation;
		}

		math_obj * operations::get_this()
		{
			return nullptr;
		}

		wstring operations::get_name()
		{
			return wstring();
		}

		void operations::assing_name(wstring _name)
		{
		}

		long double operations::get_num()
		{
			return 0;
		}

		void operations::assing_num(long double _num)
		{
		}

		math_obj * operations::get_result()
		{
			return nullptr;
		}

		addition::addition() :operations()
		{
		}

		addition::addition(math_obj * _pl) : operations(_pl)
		{
		}

		addition::addition(math_obj * _pl, math_obj * _pr) : operations(_pl, _pr)
		{
		}

		addition::~addition()
		{
		}		

		/*flags addition::get_class_type()
		{
			return flags::addit;
		}*/

		math_obj * addition::get_this()
		{
			return this;
		}

		wstring addition::get_name()
		{
			return L"+";
		}



		subtraction::subtraction() :operations()
		{
		}

		subtraction::subtraction(math_obj * _pl) : operations(_pl)
		{
		}

		subtraction::subtraction(math_obj * _pl, math_obj * _pr) : operations(_pl, _pr)
		{
		}

		subtraction::~subtraction()
		{
		}		

		/*flags subtraction::get_class_type()
		{
			return flags::minus;
		}*/

		math_obj * subtraction::get_this()
		{
			return this;
		}

		wstring subtraction::get_name()
		{
			return L"-";
		}
		

		multiplication::multiplication() :operations()
		{
		}

		multiplication::multiplication(math_obj * _pl):operations(_pl)
		{
		}

		multiplication::multiplication(math_obj * _pl, math_obj * _pr) : operations(_pl, _pr)
		{
		}

		multiplication::~multiplication()
		{
		}		

		/*flags multiplication::get_class_type()
		{
			return flags::mltpl;
		}*/

		math_obj * multiplication::get_this()
		{
			return this;
		}

		wstring multiplication::get_name()
		{
			return L"*";
		}

		division::division() :operations()
		{
		}

		division::division(math_obj * _pl) : operations(_pl)
		{
		}

		division::division(math_obj * _pl, math_obj * _pr) : operations(_pl, _pr)
		{
		}

		division::~division()
		{
		}
		
		/*flags division::get_class_type()
		{
			return flags::divis;
		}*/

		math_obj * division::get_this()
		{
			return this;
		}

		wstring division::get_name()
		{
			return L"/";
		}

		power::power() :operations()
		{
		}

		power::power(math_obj * _pl) : operations(_pl)
		{
		}

		power::power(math_obj * _pl, math_obj * _pr) : operations(_pl, _pr)
		{
		}

		power::~power()
		{
		}				

		/*flags power::get_class_type()
		{
			return flags::power;
		}*/

		math_obj * power::get_this()
		{
			return this;
		}

		wstring power::get_name()
		{
			return L"^";
		}

	}
}


