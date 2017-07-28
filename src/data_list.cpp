#include "data_list.h"

namespace Project {
	data_list * data_list::back_rec()
	{
		if (right != NULL)
		{
			return right->back_rec();
		}
		return this;
	}

	void data_list::index_plus_one()
	{
		index += 1;
		if (right != NULL)
			right->index_plus_one();
	}

	data_list * data_list::at_rec(int* place)
	{
		if (index == *place)
			return this;
		if (right != NULL)
			return right->at_rec(place);
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
		index = 0;
		left = nullptr;
		right = nullptr;
		math = nullptr;
	}

	data_list::data_list(wstring* _in)
	{

		in = *_in;
		index = 0;
		left = nullptr;
		right = nullptr;
		math = nullptr;
	}

	data_list::data_list(wstring _in, Core::math_obj * _math)
	{
		in = _in;
		index = -1;
		left = nullptr;
		right = nullptr;
		math = _math;
	}

	int data_list::push_back(data_list * pointer)
	{
		if (in.compare(L"@start#") != 0)
			return 0;
		data_list*place = back();
		if (place == nullptr)
			return 0;
		place->right = pointer;
		place->right->left = place;
		place->right->index = index;
		index++;
		return 1;
	}

	int data_list::push_left(Core::math_obj * pointer)
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
	}

	data_list * data_list::begin()
	{
		if (in.compare(L"@start#") == 0)
			return this;
		if (index == 0)
		{
			if (left->in.compare(L"@start#") == 0)
				return left;
		}
		if ((index > 0) && (left != NULL))
			return left->begin();
		return nullptr;
	}

	int data_list::implace(int place, data_list * pointer)
	{
		if (in.compare(L"@start#") != 0)
			return 0;
		data_list* temp = at(place);
		if (temp == NULL)
			return 0;
		data_list*temp_plus_one = temp->right;
		temp->right = pointer;
		temp->right->index = temp->index + 1;
		temp->right->left = temp;
		temp->right->right = temp_plus_one;
		if (temp_plus_one != NULL)
		{
			temp_plus_one->left = pointer;
			temp_plus_one->index_plus_one();
		}
		index++;
		return 1;
	}

	data_list * data_list::at(int place)
	{
		if (in.compare(L"@start#") != 0)
			return nullptr;
		if (index < place)//дополнительно размер массива записывается в поле index стартового элемента
			return nullptr;
		if ((right != NULL) && (place >= 0))
			return right->at_rec(&place);
		return NULL;

	}

	data_list * data_list::back()
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

	Core::math_obj * data_list::find_math_obj(wstring* name)
	{
		/*if ((math != NULL) && (math->name.length() != 0) && (name->compare(math->name) == 0))
		return math;
		else if (left != NULL)
		return left->find_math_obj(name);
		else*/
		return nullptr;
	}

	int data_list::run_string()
	{
		//проверка ввода
		if (!Project::IO::VerifyInput(&in[0])) {
			ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
			ProjectError::GetProjectLastError(err);
			out = err->GetErrorWStr();
			return 0;
		}

		//math = new Core::math_obj(&in[0], &in[in.length() - 1], this);
		//if (math->prop == flags::error)
		//{
		//	delete math;
		//	//чистить дерево
		//	ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
		//	ProjectError::GetProjectLastError(err);
		//	out = err->GetErrorWStr();
		//	math = NULL;
		//	return 0;
		//}
		/*if ((math->actn == flags::write)&&(math->type!=flags::funct))
		math->actn = flags::nthng;*/
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
		if (index != out)//доп. проверка для записанного в поле index стартового элемента размера массива данных
			index = out;
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
		if (index != out)//доп. проверка для записанного в поле index стартового элемента размера массива данных
			index = out;
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
		index = from + 1;
		if (from > -1)
		{
			data_list* temp = at(from + 1);
			int k = temp->delete_starting_at_rec();
			delete temp;
			return k;
		}
		else if (from == -1)
		{
			data_list* temp = at(0);
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
		//math->~math_obj();//TODO: деструктор math_obj должен удалять всё полностью без разбора (деревья и т.д.) для выбранного экземпляра класса math_obj.
		delete math;
		return k;
	}
}


