#include "operations.h"

namespace Project {
	namespace Core {
		using namespace std;
		operations::operations() :math_obj()
		{
			point_left = leaf_ptr();
			point_right = leaf_ptr();
			point_collar = leaf_ptr();
		}

		operations::operations(leaf_ptr _pl)
		{
			point_left = _pl;
			point_right = leaf_ptr();
			point_collar = leaf_ptr();
		}

		operations::operations(leaf_ptr _pl, leaf_ptr _pr)
		{
			point_left = _pl;
			point_right = _pr;
			point_collar = leaf_ptr();
		}
			

		operations::~operations()
		{
		}

		leaf_ptr operations::get_pl()
		{
			return point_left;
		}

		void operations::assing_pl(leaf_ptr& _pointer)
		{
			point_left = _pointer;
		}

		leaf_ptr operations::get_pr()
		{
			return point_right;
		}

		void operations::assing_pr(leaf_ptr _pointer)
		{
			point_right = _pointer;
		}

		leaf_ptr operations::get_pc()
		{
			return point_collar;
		}

		void operations::assing_pc(leaf_ptr _pointer)
		{
			point_collar = _pointer;
		}

		bool operations::define_operation(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this)
		{
			if (_high.is_null_ptr() && _low.is_null_ptr()) {
				//err
				return false;
			}
			//если это первая операция в выражении
			else if (_high.is_null_ptr() && !_low.is_null_ptr()) {
				point_left = _low;
				point_collar = _obj;
				_obj->assing_pl(_this);
			}
			//если была какая-либо операция до этого
			else {
				//если приоритет предыдущей обработанной операции !МЕНЬШЕ! приоритета текущей
				if (_high->get_priority() < this->get_priority()) {
					point_left = _low;
					_high->assing_pr(_this);
					point_collar = _high;
				}
				else {
					//leaf_ptr temp;
					leaf_ptr top = _obj->get_pl();
					flags type = top->get_class_type();
					if (type != flags::addition&&type != flags::subtraction&&type != flags::multiplication&&type != flags::division&&type != flags::power)
						return false; //err					
					if (top->get_priority() >= this->get_priority()) {
						point_left = top;//много преобразований указателей. Проверить правильность работы
						point_collar = _obj;
						_obj->assing_pl(_this);
						top->assing_pr(_this);
					}
					else {
						top = ((operations*)_high.get_ptr_unsafe())->prioritize(this->get_priority());//операция с нужным приоритетом
						if (top.is_null_ptr()) {
							return false;
						}
						point_left = top->get_pr();
						point_collar = top;
						point_collar->assing_pr(_this);
						point_left->assing_pc(_this);
					}
				}
			}
			return true;
		}

		leaf_ptr operations::prioritize(int _priority)
		{
			//flags type = point_collar->get_class_type();
			//if (type != flags::addition&&type != flags::subtraction&&type != flags::multiplication&&type != flags::division&&type != flags::power)
			//	return leaf_ptr(); //err
			if (point_collar->get_priority() <= _priority)
				return point_collar;
			operations* next = (operations*)point_collar.get_ptr_unsafe();			
			return next->prioritize_rec(&_priority);
		}

		

		leaf_ptr operations::prioritize_rec(int * _p)
		{
			if (point_collar->get_priority() <= *_p)
				return point_collar;
			operations* next = (operations*)point_collar.get_ptr_unsafe();
			return next->prioritize_rec(_p);			
		}

		flags operations::get_class_type()
		{
			return flags::operation;
		}

		size_t operations::get_sizeof()
		{
			return sizeof(*this);
		}
		
			

		addition::addition() :operations()
		{
		}

		

		addition::addition(leaf_ptr _pl) : operations(_pl)
		{
		}

		addition::addition(leaf_ptr _pl, leaf_ptr _pr) : operations(_pl, _pr)
		{
		}

		addition::addition(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this) : operations()
		{
			if (!define_operation(_high, _low, _obj, _this)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
		}

		addition::~addition()
		{
		}		

		flags addition::get_class_type()
		{
			return flags::addition;
		}

		/*math_obj * addition::get_this()
		{
			return this;
		}*/

		wstring addition::get_name()
		{
			return L"+";
		}

		math_obj * addition::get_result()
		{
			return nullptr;
		}
				

		int addition::get_priority()
		{
			return 1;
		}

		void addition::copy_to(void * _ptr)
		{
			addition *place = new (_ptr) addition();
			place->copy(this);
		}

		math_obj * addition::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::addition|| type == flags::subtraction||type == flags::multiplication||type==flags::division||type==flags::power) {
				this->point_left = _original->get_pl();				
				this->point_collar = _original->get_pc();
				this->point_right = _original->get_pr();
				return this;
			}
			return nullptr;
		}



		subtraction::subtraction() :operations()
		{
		}

		subtraction::subtraction(leaf_ptr _pl) : operations(_pl)
		{
		}

		subtraction::subtraction(leaf_ptr _pl, leaf_ptr _pr) : operations(_pl, _pr)
		{
		}

		subtraction::subtraction(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc) : operations(_pl, _pr)
		{
			point_collar = _pc;
		}

		subtraction::subtraction(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this): operations()
		{
			 if (!define_operation(_high, _low, _obj, _this)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
		}

		subtraction::~subtraction()
		{
		}		

		flags subtraction::get_class_type()
		{
			return flags::subtraction;
		}

		/*math_obj * subtraction::get_this()
		{
			return this;
		}*/

		wstring subtraction::get_name()
		{
			return L"-";
		}

		math_obj * subtraction::get_result()
		{
			return nullptr;
		}

		int subtraction::get_priority()
		{
			return 1;
		}

		void subtraction::copy_to(void * _ptr)
		{
			subtraction *place = new (_ptr) subtraction();
			place->copy(this);
		}

		math_obj * subtraction::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power) {
				this->point_left = _original->get_pl();
				this->point_collar = _original->get_pc();
				this->point_right = _original->get_pr();
				return this;
			}
			return nullptr;
		}
		

		multiplication::multiplication() :operations()
		{
		}

		multiplication::multiplication(leaf_ptr _pl):operations(_pl)
		{
		}

		multiplication::multiplication(leaf_ptr _pl, leaf_ptr _pr) : operations(_pl, _pr)
		{
		}

		multiplication::multiplication(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this): operations()
		{
			if (!define_operation(_high, _low, _obj, _this)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
		}

		multiplication::~multiplication()
		{
		}		

		flags multiplication::get_class_type()
		{
			return flags::multiplication;
		}

		/*math_obj * multiplication::get_this()
		{
			return this;
		}*/

		wstring multiplication::get_name()
		{
			return L"*";
		}

		math_obj * multiplication::get_result()
		{
			return nullptr;
		}

		int multiplication::get_priority()
		{
			return 2;
		}

		void multiplication::copy_to(void * _ptr)
		{
			multiplication *place = new (_ptr) multiplication();
			place->copy(this);
		}

		math_obj * multiplication::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power) {
				this->point_left = _original->get_pl();
				this->point_collar = _original->get_pc();
				this->point_right = _original->get_pr();
				return this;
			}
			return nullptr;
		}

		division::division() :operations()
		{
		}

		division::division(leaf_ptr _pl) : operations(_pl)
		{
		}

		division::division(leaf_ptr _pl, leaf_ptr _pr) : operations(_pl, _pr)
		{
		}

		division::division(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this): operations()
		{
			if (!define_operation(_high, _low, _obj, _this)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
		}

		division::~division()
		{
		}
		
		flags division::get_class_type()
		{
			return flags::division;
		}

		/*math_obj * division::get_this()
		{
			return this;
		}*/

		wstring division::get_name()
		{
			return L"/";
		}

		math_obj * division::get_result()
		{
			return nullptr;
		}

		int division::get_priority()
		{
			return 2;
		}

		void division::copy_to(void * _ptr)
		{
			division *place = new (_ptr) division();
			place->copy(this);
		}

		math_obj * division::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power) {
				this->point_left = _original->get_pl();
				this->point_collar = _original->get_pc();
				this->point_right = _original->get_pr();
				return this;
			}
			return nullptr;
		}

		power::power() :operations()
		{
		}

		power::power(leaf_ptr _pl) : operations(_pl)
		{
		}

		power::power(leaf_ptr _pl, leaf_ptr _pr) : operations(_pl, _pr)
		{
		}

		power::power(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this) : operations()
		{
			if (!define_operation(_high, _low, _obj, _this)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
		}

		power::~power()
		{
		}				

		flags power::get_class_type()
		{
			return flags::power;
		}

		/*math_obj * power::get_this()
		{
			return this;
		}*/

		wstring power::get_name()
		{
			return L"^";
		}

		math_obj * power::get_result()
		{
			return nullptr;
		}

		int power::get_priority()
		{
			return 3;
		}

		void power::copy_to(void * _ptr)
		{
			power *place = new (_ptr) power();
			place->copy(this);
		}

		math_obj * power::copy(math_obj * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power) {
				this->point_left = _original->get_pl();
				this->point_collar = _original->get_pc();
				this->point_right = _original->get_pr();
				return this;
			}
			return nullptr;
		}

	}
}


