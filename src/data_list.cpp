#include "data_list.h"

namespace Project {
	using namespace Core;
	data_list * data_list::back_rec()
	{
		if (right != NULL)
		{
			return right->back_rec();
		}
		return this;
	}


	data_list * data_list::at_rec(int* place, int * count)
	{
		if (*count == *place)
			return this;
		if (right != NULL) {
			(*count)++;
			return right->at_rec(place, count);
		}
		return nullptr;
	}

	int data_list::size_rec(bool* flag)
	{
		if (*flag) //если true
		{
			if (right != NULL)
			{
				return (1 + right->size_rec(flag));
			}
			else
				return 1;
		}
		else
		{
			if (left != NULL)
			{
				return (1 + left->size_rec(flag));
			}
			else
				return 1;
		}
	}

	data_list::data_list()
	{
		in = wstring();
		out = wstring();
		left = nullptr;
		right = nullptr;
		object = tree_ptr();
	}

	data_list::data_list(wstring * _in)
	{
		in = *_in;
		out = wstring();
		left = nullptr;
		right = nullptr;
		object = tree_ptr();
	}

	data_list::data_list(wstring* _in, data_list* _start)
	{
		in = *_in;
		out = wstring();
		left = nullptr;
		right = nullptr;
		_start->push_back(this);
		//проверка ввода. Текст ошибки записывается в поле out
		if (!IO::VerifyInput(&in[0])) {
			ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
			ProjectError::GetProjectLastError(err);
			out.append(err->GetErrorWStr());
		}
		else {
			expression temp = expression();
			object = tree_ptr(&temp, 10);
			try {
				this->build_iternal(&in[0], &in[in.length()] - 1);
			}
			catch (ProjectError::ErrorCode error_caught) {
				ProjectError::SetProjectLastError(error_caught);
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				out.append(err->GetErrorWStr());
			}
			catch (...) {
				out.append(L"Unresolved error.");
			}
		}
	}

	data_list::data_list(wstring _in, Core::tree_ptr _math)
	{
		in = _in;
		left = nullptr;
		right = nullptr;
		object = _math;
	}

	bool data_list::push_back(data_list * pointer)
	{
		if (in.compare(L"@start#") != 0)
			return 0;
		data_list*place = end();
		if (place == nullptr)
			return 0;
		place->right = pointer;
		place->right->left = place;
		return 1;
	}

	bool data_list::push_left(Core::tree_ptr pointer)
	{
		data_list * zero = begin();
		if (zero == NULL)
			return 0;
		data_list * temp = zero->left;
		zero->left = new data_list(L"", pointer);
		zero->left->right = zero;
		zero->left->left = temp;
		if (temp != NULL)
			temp->right = zero->left;
		return 1;
	}

	data_list * data_list::begin()
	{
		if (in.compare(L"@start#") == 0)
			return this;
		if ((left != NULL))
			return left->begin();
		return nullptr;
	}

	data_list::__iterator data_list::front()
	{
		__iterator out;//0
		out.letter = this->begin();
		return out;
	}

	data_list::__iterator data_list::back()
	{
		__iterator out;
		if (!in.compare(L"@start#"))
			out.letter = this->end();
		else if (this->right != NULL)
			out.letter = this->right->back_rec();
		else
			out.letter = this;
		return out;
	}

	bool data_list::implace(int place, data_list * pointer)
	{
		if (in.compare(L"@start#") != 0)
			return 0;
		data_list* temp = at(place);
		if (temp == NULL)
			return 0;
		if (!temp->implace_p(pointer))
			return 0;
		return 1;
	}

	data_list * data_list::at(int place)
	{
		if (in.compare(L"@start#") != 0)
			return nullptr;
		if ((right != NULL) && (place >= 0)) {
			int c = 0;
			return right->at_rec(&place, &c);
		}
		return NULL;

	}

	data_list * data_list::end()
	{
		if (in.compare(L"@start#") != 0)
			return nullptr;
		else if (right != NULL)
		{
			return right->back_rec();
		}
		else
		{
			return this;
		}
	}

	int data_list::compare_in(wstring *original)
	{
		return in.compare(*original);
	}

	int data_list::compare_out(wstring* original)
	{
		return out.compare(*original);
	}

	Core::tree_ptr data_list::find_math_obj(wstring* name)
	{
		/*if ((object != NULL) && (object->name.length() != 0) && (name->compare(object->name) == 0))
		return object;
		else if (left != NULL)
		return left->find_math_obj(name);
		else*/
		return nullptr;
	}

	bool data_list::run_string()
	{
		//проверка ввода


		//object = new Core::math_obj(&in[0], &in[in.length() - 1], this);
		//if (object->prop == flags::error)
		//{
		//	delete object;
		//	//чистить дерево
		//	ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
		//	ProjectError::GetProjectLastError(err);
		//	out = err->GetErrorWStr();
		//	object = NULL;
		//	return 0;
		//}
		/*if ((object->actn == flags::write)&&(object->type!=flags::funct))
		object->actn = flags::nthng;*/
		return 1;
	}

	int data_list::size_s()
	{
		if (in.compare(L"@start#") != 0)
			return -1;
		int out = 0;
		bool flag = true;
		if (right != NULL)
		{
			out += right->size_rec(&flag);
		}
		return out;
	}

	int data_list::size_all()
	{
		if (in.compare(L"@start#") != 0)
			return -1;
		int out = 0;
		bool flag;
		if (right != NULL)
		{
			flag = true;
			out += right->size_rec(&flag);
		}
		if (left != NULL)
		{
			flag = false;
			out += left->size_rec(&flag);
		}
		return out + 1;//так же считается элемент start
	}
	/*Метод удаляет все элементы списка данных начиная с позиции from. Позиция from не удаляется.
	В случае вызова для позиции from = -1 - удаляются все элементы справа от start (включая нулевой)
	-1 в случае ошибки. */
	int data_list::delete_starting_at(int from)
	{
		if (in.compare(L"@start#") != 0)
			return -1;
		if (from >= -1)
		{
			data_list* temp = at(from + 1);
			int k = temp->delete_starting_at_rec();
			delete temp;
			return k;
		}
		else return -1;
	}
	/*Рекурсия для delete_starting_at*/
	int data_list::delete_starting_at_rec()
	{
		int k = 0;
		if (right != NULL)
		{
			k += right->delete_starting_at_rec();
			delete right;
			right = NULL;
		}
		//object->~math_obj();//TODO: деструктор math_obj должен удалять всё полностью без разбора (деревья и т.д.) для выбранного экземпляра класса math_obj.
		object.~tree_ptr();
		return k;
	}

	bool data_list::implace_p(data_list * _obj)
	{
		data_list*temp_plus_one = this->right;
		this->right = _obj;
		this->right->left = this;
		this->right->right = temp_plus_one;
		if (temp_plus_one != NULL) {
			temp_plus_one->left = _obj;
		}
		return 1;
	}
	//Внешний вызов для билдера
	bool data_list::build()
	{
		if (!in.length())
			return false;
		wchar_t * in_str = &in[0];
		wchar_t * in_end = &in[in.length() - 1];
		expression temp = expression();
		out = wstring();
		if (object.is_null_ptr())
			object = tree_ptr(&temp, 10);
		else
			object.~tree_ptr();
		try {
			this->build_iternal(&in[0], &in[in.length() - 1]);
		}
		catch (ProjectError::ErrorCode error_caught) {
			ProjectError::SetProjectLastError(error_caught);
			ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
			ProjectError::GetProjectLastError(err);
			out.append(err->GetErrorWStr());
			return false;
		}
		catch (...) {
			out.append(L"Unresolved error.");
		}
		return true;
	}
	//Внутренний билдер
	leaf_ptr data_list::build_iternal(wchar_t * _str, wchar_t * _end)
	{
		wchar_t* temp = wcschr(_str, L'=');
		leaf_ptr temp_ptr;
		if (temp == nullptr) {//если не найдено знака равно в строке - оконечный случай - то что будет построено по заданному куску строки должно уйти наверх.
			expression temp = expression();
			temp_ptr = object.push_obj(&temp);
			return build_tree(_str, _end, temp_ptr);
		}

		leaf_ptr left;//указатель на левую часть выражения
		leaf_ptr right;//указатель на правую часть выражения		
		wstring str_left;
		str_left.assign(_str, temp);
		left = build_tree(&str_left[0], &str_left[str_left.length() - 1], (leaf_ptr)object);


		if (left->get_class_type() == flags::function) {
			//TODO:подумать.
		}

		if (temp == _end) {
			//отсеивание ошибочных (невычисляемых) элементов
			return left;//тут есть доп условия
		}

		wstring str_right;
		str_right.assign(temp + 1, _end + 1);
		//	right = build_tree(&str_right[0], &str_right[str_right.length() - 1]);


			//TODO:далее самое веселье)
		return leaf_ptr();
	}


	Core::leaf_ptr& data_list::build_tree(wchar_t * _str, wchar_t * _end, leaf_ptr  _current_obj)
	{
		unsigned int count, comma, iter;
		int count_var;


		leaf_ptr last_op;//последняя записанная операция
		leaf_ptr last_obj; //последняя записанная константа/переменная/ф-ция/выражение
		leaf_ptr temp_pointer;
		leaf_ptr multiple_var;

		wchar_t* temp/*, *temp_2*/;
		wstring temp_str, name_str;
		double buf;
		wchar_t* s_iter = _str;
		while (s_iter <= _end) {

			if (*s_iter >= '0'&& *s_iter <= '9' || *s_iter == '.') {
				// если данное число первое в записи выражения
				if (last_op.is_null_ptr() && last_obj.is_null_ptr()) {
					//создание элемента класса и запись числа
					_current_obj->assing_pl(object.push_obj(&number(wcstod(s_iter, &s_iter))));
					//оба указателя -> на число, тебуется для проверки условия при записи операции
					last_obj = object->get_pl();
				}
				// если данное число не первое в записи (была какая-либо операция)
				else {
					//создание элемента класса и запись числа

					last_op->assing_pr(object.push_obj(&number(wcstod(s_iter, &s_iter))));
					last_obj = last_op->get_pr();
				}
				//temp = wcspbrk(s_iter, L",");
				temp = wcspbrk(s_iter, L"(");//если сразу после числа стоит открывающая скобка
				if (s_iter == temp)
				{//добавляется умножение
					temp_pointer = leaf_ptr(object, &multiplication());
					temp_pointer->copy(&multiplication(last_op, last_obj, _current_obj, temp_pointer));
				}
			}
			else if (*s_iter == '+') {
				temp_pointer = leaf_ptr(object, &addition());
				temp_pointer->copy(&addition(last_op, last_obj, _current_obj, temp_pointer));
				s_iter++;
				temp = wcspbrk(s_iter, L")+-*^/=,");
				if (s_iter == temp) {
					throw ProjectError::ErrorCode::UNEXPECTED_OPERATION;
				}
			}
			else if (*s_iter == '-') {
				if (last_op.is_null_ptr() && last_obj.is_null_ptr()) {//если в начале строки находится минус
					temp_pointer = leaf_ptr(object, &subtraction(leaf_ptr(object, &number()), leaf_ptr(), _current_obj));
					_current_obj->assing_pl(temp_pointer);
				}
				else	{
					temp_pointer = leaf_ptr(object, &subtraction());
					temp_pointer->copy(&subtraction(last_op, last_obj, _current_obj, temp_pointer));					
				}

				s_iter++;
				temp = wcspbrk(s_iter, L")+-*^/=,");
				if (s_iter == temp)	{
					throw ProjectError::ErrorCode::UNEXPECTED_OPERATION;
				}
			}
			else if (*s_iter == '*') {
				temp_pointer = leaf_ptr(object, &multiplication());
				temp_pointer->copy(&multiplication(last_op, last_obj, _current_obj, temp_pointer));
				s_iter++;
				temp = wcspbrk(s_iter, L")+-*^/=,");
				if (s_iter == temp) {
					throw ProjectError::ErrorCode::UNEXPECTED_OPERATION;
				}			
			}
			else if (*s_iter == '/') {
				temp_pointer = leaf_ptr(object, &division());
				temp_pointer->copy(&division(last_op, last_obj, _current_obj, temp_pointer));
				s_iter++;
				temp = wcspbrk(s_iter, L")+-*^/=,");
				if (s_iter == temp) {
					throw ProjectError::ErrorCode::UNEXPECTED_OPERATION;
				}				
			}
			else if (*s_iter == '^') {
				temp_pointer = leaf_ptr(object, &power());
				temp_pointer->copy(&power(last_op, last_obj, _current_obj, temp_pointer));
				s_iter++;
				temp = wcspbrk(s_iter, L")+-*^/=,");				
			}
			else if (*s_iter == '(') {

			}
			else if (*s_iter == '[') {

			}
			else {//другие символы

			}
		}

		return _current_obj;
	}


	data_list::__iterator::__iterator() {
		letter = nullptr;
	}

	bool data_list::__iterator::implace_after_this(data_list * pointer)
	{
		return letter->implace_p(pointer);
	}
	int data_list::__iterator::compare_in(wstring * original)
	{
		return letter->compare_in(original);
	}
	int data_list::__iterator::compare_out(wstring * original)
	{
		return letter->compare_out(original);
	}
	Core::tree_ptr data_list::__iterator::find_math_obj(wstring * name)
	{
		return letter->find_math_obj(name);
	}
	int data_list::__iterator::delete_after_this()
	{
		int k = 0;
		if (letter->right) {
			k = letter->right->delete_starting_at_rec();
			delete letter->right;
		}
		return k;
	}
	bool data_list::__iterator::build()
	{
		return letter->build();
	}
	wstring data_list::__iterator::get_in()
	{
		return letter->in;
	}
	wstring data_list::__iterator::get_out()
	{
		return letter->out;
	}
	Core::tree_ptr data_list::__iterator::get_obj()
	{
		return letter->object;
	}
	void data_list::__iterator::assing_in(wstring & _in)
	{
		letter->in = _in;
	}
	void data_list::__iterator::assing_out(wstring & _out)
	{
		letter->out = _out;
	}
	void data_list::__iterator::assing_obj(Core::math_obj * _obj)
	{
		letter->object = _obj;
	}
	data_list::__iterator &data_list::__iterator::operator=(const __iterator _right)
	{
		this->letter = _right.letter;
		return *this;
	}
	data_list::__iterator & data_list::__iterator::operator=(data_list * _right)
	{
		this->letter = _right;
		return *this;
	}
	data_list::__iterator & data_list::__iterator::operator++(int)
	{
		if (this->letter&&this->letter->right) {
			this->letter = this->letter->right;
		}
		else {
			this->letter = nullptr;
		}
		return *this;
	}
	data_list::__iterator & data_list::__iterator::operator++()
	{
		if (this->letter&&this->letter->right) {
			this->letter = this->letter->right;
		}
		else {
			this->letter = nullptr;
		}
		return *this;
	}
	data_list::__iterator & data_list::__iterator::operator--(int)
	{
		if (this->letter&&this->letter->left) {//имеет ли смысл останавливать его на позиции start или продолжать идти дальше?
			this->letter = this->letter->left;
		}
		else {
			this->letter = nullptr;
		}
		return *this;
	}
	data_list::__iterator & data_list::__iterator::operator--()
	{
		if (this->letter&&this->letter->left) {//имеет ли смысл останавливать его на позиции start или продолжать идти дальше?
			this->letter = this->letter->left;
		}
		else {
			this->letter = nullptr;
		}
		return *this;
	}
	bool data_list::__iterator::operator==(const __iterator _right)
	{
		if (this->letter == _right.letter)
			return true;
		return false;
	}
	bool data_list::__iterator::operator==(const data_list * _right)
	{
		if (this->letter == _right)
			return true;
		return false;
	}

	bool data_list::__iterator::operator!=(const __iterator _right)
	{
		return !(*this == _right);
	}

	bool data_list::__iterator::operator!=(const data_list * _right)
	{
		return !(*this == _right);
	}

	data_list::__iterator::operator data_list*() const
	{
		return letter;
	}

}


