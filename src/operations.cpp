#include "operations.h"

namespace Project {
	namespace Core {
		using namespace std;
				
		bool operations::define_operation(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this)
		{
			if (_high.is_null_ptr() && _low.is_null_ptr()) {
				//err
				return false;
			}
			//если это первая операция в выражении
			else if (_high.is_null_ptr() && !_low.is_null_ptr()) {
				this->assing_pl(_low);
				this->assing_pc(_obj);
				_obj->assing_pl(_this);
			}
			//если была какая-либо операция до этого
			else {
				//если приоритет предыдущей обработанной операции !МЕНЬШЕ! приоритета текущей
				if (_high->get_priority() < this->get_priority()) {
					this->assing_pl(_low);
					_high->assing_pr(_this);
					this->assing_pc(_high);
				}
				else {
					//leaf_ptr temp;
					leaf_ptr top = _obj->get_pl();
					flags type = top->get_class_type();
					if (type != flags::addition&&type != flags::subtraction&&type != flags::multiplication&&type != flags::division&&type != flags::power)
						return false; //err					
					if (top->get_priority() >= this->get_priority()) {
						this->assing_pl(top);	//много преобразований указателей. Проверить правильность работы
						this->assing_pc(_obj);
						_obj->assing_pl(_this);
						top->assing_pr(_this);
					}
					else {
						top = ((operations*)_high.get_ptr_unsafe())->prioritize(this->get_priority());//операция с нужным приоритетом
						if (top.is_null_ptr()) {
							return false;
						}
						this->assing_pl(top->get_pr());
						this->assing_pc(top);
						this->get_pc()->assing_pr(_this);
						this->get_pl()->assing_pc(_this);
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
			if (get_pc()->get_priority() <= _priority)
				return get_pc();
			operations* next = (operations*)get_pc().get_ptr_unsafe();
			return next->prioritize_rec(&_priority);
		}

		

		leaf_ptr operations::prioritize_rec(int * _p)
		{
			if (get_pc()->get_priority() <= *_p)
				return get_pc();
			operations* next = (operations*)get_pc().get_ptr_unsafe();
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

		void * operations::get_this()
		{
			return nullptr;
		}			

		addition::addition(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this) : operations()
		{
			if (!define_operation(_high, _low, _obj, _this)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
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
			addition temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			addition *place = (addition*)_ptr;			
			place->copy(this);
		}

		bool addition::copy(addition * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power) {
				//this->point_left = _original->get_pl();
				//this->point_collar = _original->get_pc();
				//this->point_right = _original->get_pr();
				math_obj::copy(_original);
				return true;
			}
			return false;
		}

		subtraction::subtraction(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this) : operations()
		{
			if (!define_operation(_high, _low, _obj, _this)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
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
			subtraction temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			subtraction *place = (subtraction*)_ptr;			
			place->copy(this);
		}

		bool subtraction::copy(subtraction * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power) {
				//this->point_left = _original->get_pl();
				//this->point_collar = _original->get_pc();
				//this->point_right = _original->get_pr();
				math_obj::copy(_original);
				return true;
			}
			return false;
		}

		multiplication::multiplication(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this): operations()
		{
			if (!define_operation(_high, _low, _obj, _this)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
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
			multiplication temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			multiplication *place = (multiplication*)_ptr;			
			place->copy(this);
		}

		bool multiplication::copy(multiplication * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power) {
				//this->point_left = _original->get_pl();
				//this->point_collar = _original->get_pc();
				//this->point_right = _original->get_pr();
				math_obj::copy(_original);
				return true;
			}
			return false;
		}

		division::division(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this): operations()
		{
			if (!define_operation(_high, _low, _obj, _this)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
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
			division temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			division *place = (division*)_ptr;			
			place->copy(this);
		}

		bool division::copy(division * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power) {
				//this->point_left = _original->get_pl();
				//this->point_collar = _original->get_pc();
				//this->point_right = _original->get_pr();
				math_obj::copy(_original);
				return true;
			}
			return false;
		}

		power::power(leaf_ptr _high, leaf_ptr _low, leaf_ptr _obj, leaf_ptr _this) : operations()
		{
			if (!define_operation(_high, _low, _obj, _this)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
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
			power temp;
			std::memcpy(_ptr, &temp, temp.get_sizeof());
			power *place = (power*)_ptr;			
			place->copy(this);
		}

		bool power::copy(power * _original)
		{
			flags type = _original->get_class_type();
			if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power) {
				//this->point_left = _original->get_pl();
				//this->point_collar = _original->get_pc();
				//this->point_right = _original->get_pr();
				math_obj::copy(_original);
				return true;
			}
			return false;
		}
	}
}


