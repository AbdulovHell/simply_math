#include "operations.h"

namespace Project {
	namespace Core {
		using namespace std;
		operations::operations() :math_obj()
		{
			point_left = nullptr;
			point_right = nullptr;
			point_collar = nullptr;
		}

		operations::operations(math_obj * _pl)
		{
			point_left = _pl;
			point_right = nullptr;
			point_collar = nullptr;
		}

		operations::operations(math_obj * _pl, math_obj * _pr)
		{
			point_left = _pl;
			point_right = _pr;
			point_collar = nullptr;
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
			return point_collar;
		}

		void operations::assing_pc(math_obj * _pointer)
		{
			point_collar = _pointer;
		}

		bool operations::define_operation(operations * _high, math_obj * _low, math_obj * _obj)
		{
			if ((_high == nullptr) && (_high == nullptr)) {
				//err
				return false;
			}
			//если это первая операция в выражении
			else if (_high != nullptr&&_low == nullptr) {
				point_left = _low;
				point_collar = _obj;
				_obj->assing_pl(this);				
			}
			//если была какая-либо операция до этого
			else {
				//если приоритет предыдущей обработанной операции !МЕНЬШЕ! приоритету текущей
				if (_high->get_priority() < this->get_priority()) {
					point_left = _low;
					_high->assing_pr(this);
					point_collar = _high;
				}
				else {
					operations* top;
					math_obj * temp = _obj->get_pl();
					flags type = temp->get_class_type();
					if (type != flags::addit&&type != flags::minus&&type != flags::mltpl&&type != flags::divis&&type != flags::power)
						return false; //err
					top = (operations*)temp;
					if (top->get_priority() >= this->get_priority()) {
						point_left = top;//много преобразований указателей. Проверить правильность работы
						point_collar = _obj;
						_obj->assing_pl(this);
						top->assing_pr(this);
					}
					else {
						top = _high->prioritize(this->get_priority());//операция с нужным приоритетом
						if (!top) {
							return false;
						}
						point_left = top->get_pr();
						point_collar = top;
						point_collar->assing_pr(this);
						point_left->assing_pc(this);
					}
				}
			}
			return true;
		}

		operations * operations::prioritize(int _priority)
		{
			flags type = point_collar->get_class_type();
			if (type != flags::addit&&type != flags::minus&&type != flags::mltpl&&type != flags::divis&&type != flags::power)
				return nullptr; //err
			operations* next = (operations*)point_collar;			
			return next->prioritize_rec(&_priority);
		}

		operations * operations::prioritize_rec(int * _p)
		{
			//если приоритет проверяемой операции !БОЛЬШЕ! текущей операции
			if (this->get_priority() > *_p) {
				flags type = point_collar->get_class_type();
				if (type != flags::addit&&type != flags::minus&&type != flags::mltpl&&type != flags::divis&&type != flags::power)
					return nullptr; //err
				operations* next = (operations*)point_collar;
				return next->prioritize_rec(_p);
			}
			else{
				return this;
			}
			return nullptr;
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
		
			

		addition::addition() :operations()
		{
		}

		addition::addition(math_obj * _pl) : operations(_pl)
		{
		}

		addition::addition(math_obj * _pl, math_obj * _pr) : operations(_pl, _pr)
		{
		}

		addition::addition(operations * _high, math_obj * _low, math_obj * _obj): operations()
		{
			if (!define_operation(_high, _low, _obj)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
		}

		addition::~addition()
		{
		}		

		flags addition::get_class_type()
		{
			return flags::addit;
		}

		math_obj * addition::get_this()
		{
			return this;
		}

		wstring addition::get_name()
		{
			return L"+";
		}
				

		int addition::get_priority()
		{
			return 1;
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

		subtraction::subtraction(operations * _high, math_obj * _low, math_obj * _obj): operations()
		{
			if (_high == nullptr&&_low == nullptr) {//если в начале строки находится минус
				point_left = new number();//для конструктора вычитания (и только для него) требуется подключать number.h с реализацией класса number. Возможно имеет смысл такую проверку делать в builder
				point_collar = _obj;
				_obj->assing_pl(this);
			}
			else if (!define_operation(_high, _low, _obj)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
		}

		subtraction::~subtraction()
		{
		}		

		flags subtraction::get_class_type()
		{
			return flags::minus;
		}

		math_obj * subtraction::get_this()
		{
			return this;
		}

		wstring subtraction::get_name()
		{
			return L"-";
		}		

		int subtraction::get_priority()
		{
			return 1;
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

		multiplication::multiplication(operations * _high, math_obj * _low, math_obj * _obj): operations()
		{
			if (!define_operation(_high, _low, _obj)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
		}

		multiplication::~multiplication()
		{
		}		

		flags multiplication::get_class_type()
		{
			return flags::mltpl;
		}

		math_obj * multiplication::get_this()
		{
			return this;
		}

		wstring multiplication::get_name()
		{
			return L"*";
		}

		int multiplication::get_priority()
		{
			return 2;
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

		division::division(operations * _high, math_obj * _low, math_obj * _obj): operations()
		{
			if (!define_operation(_high, _low, _obj)) {
				throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
			}
		}

		division::~division()
		{
		}
		
		flags division::get_class_type()
		{
			return flags::divis;
		}

		math_obj * division::get_this()
		{
			return this;
		}

		wstring division::get_name()
		{
			return L"/";
		}

		int division::get_priority()
		{
			return 2;
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

		power::power(operations * _high, math_obj * _low, math_obj * _obj) : operations()
		{
			if (!define_operation(_high, _low, _obj)) {
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

		math_obj * power::get_this()
		{
			return this;
		}

		wstring power::get_name()
		{
			return L"^";
		}

		int power::get_priority()
		{
			return 3;
		}

	}
}


