///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include "error.h"
#include "IO.h"

namespace Project {
	namespace Core {

		using namespace std;
		using namespace Project;
		using namespace Project::IO;

		class var_const {
		private:
			var_const *prioritize_processing(var_const *pc, int current_priority)
			{
				//���� ��������� ����������� �������� !������! ������� ��������
				if (pc->get_priority() > current_priority)
				{
					//�������� ����� ��� ��� ��� ��������� ��������
					return prioritize_processing(pc, current_priority);
				}
				//���� ��������� ����������� �������� !������! ��� ����� ���������� ������� ��������
				else
				{
					//������� ��������� �� ����������� ��������
					return pc;
				}
			}

			double processing(var_const *pointer)
			{
				//TODO: �������� ����������� ���������� �������
				const wchar_t* operation = pointer->var_id.c_str();
				switch (*operation) {
				case'+':
					return processing(pointer->point_left) + processing(pointer->point_right);
				case '*':
					return processing(pointer->point_left) * processing(pointer->point_right);
				case '/':
					return processing(pointer->point_left) / processing(pointer->point_right);
				case '^':
					return pow(processing(pointer->point_left), processing(pointer->point_right));

				default:       //��� ��������, ����� � ����������
					return pointer->var;
				}
			}

			wstring expression_processing(var_const *pointer)
			{
				if ((pointer->var_id == L"*") || (pointer->var_id == L"/"))
				{
					return expression_processing(pointer->point_left) + L" " + pointer->var_id + L" " + expression_processing(pointer->point_right);
				}
				else if (pointer->var_id == L"+")
				{
					if ((pointer->point_collar->var_id == L"*") || (pointer->point_collar->var_id == L"/"))
						return L"(" + expression_processing(pointer->point_left) + L" " + pointer->var_id + L" " + expression_processing(pointer->point_right) + L")";
					else
						return expression_processing(pointer->point_left) + L" " + pointer->var_id + L" " + expression_processing(pointer->point_right);
				}
				else if (pointer->var_id == L"0")
				{
					return to_string(pointer->var, var_type::FRACTIONAL, 2);
				}
				else
				{
					return pointer->read(L"name");
				}
			}

		public:
			var_const()
			{
				var_id = L"";
				var = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(wstring _name, double _num)
			{
				var_id = _name;
				var = _num;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(wstring _name, double _num, var_const *_pc)
			{
				var_id = _name;
				var = _num;
				point_left = NULL;
				point_right = NULL;
				point_collar = _pc;
			}

			var_const(wstring _name, double _num, var_const * _pl, var_const *_pr) {
				var_id = _name;
				var = _num;
				point_left = _pl;
				point_right = _pr;
				point_collar = NULL;
			}

			var_const(wstring _name, double _num, var_const * _pl, var_const *_pr, var_const *_pc) {
				var_id = _name;
				var = _num;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}

			var_const(var_const* var1)
			{
				var_id = var1->var_id;
				var = var1->var;
				point_left = var1->point_left;
				point_right = var1->point_right;
				point_collar = var1->point_collar;
			}

			~var_const()
			{

			}

			//var_const* operator=(var_const* other);

			/*var_const* var_const::operator=(var_const* other) {
				if (this == other) {
					return this;
				}
				this->var_id = other->var_id;
				this->var = other->var;
				this->point_left = other->point_left;
				this->point_right = other->point_right;
				this->point_collar = other->point_collar;
				return this;
			}*/

			void copy(var_const* ref) {
				var_id = ref->var_id;
				var = ref->var;
				point_left = ref->point_left;
				point_right = ref->point_right;
				point_collar = ref->point_collar;
			}
			/*����� ���������� ��������� ��������.
			1 - ��������
			2 - ���������
			3 - �������
			4 - ���������� � �������
			0 - �� ��������*/
			int get_priority()
			{
				wchar_t operation = var_id[0];
				switch (operation)
				{
				case '+':
					return 1 + (int)var;
				case '*':
					return 2 + (int)var;
				case '/':
					return 3 + (int)var;
				case '^':
					return 4 + (int)var;
				default:
					return 0;
				}
			}


			/*����� read ���������� ������, ���������� ���������� �� �������� ������ � ������������ � ����������� ����������:
			type - ��� �������� ������, �� ��� �������� � ������ var_id �� ������� @
			prop - �������� �������� ������, �� ��� �������� ����� #
			name - ��� �������� ������, ��������� � var_id ����� @ � #, ��� ������� � ��������� - ����� @ � (
			func - ��� �������� ������, ��������� � var_id ����� @ � #, ��� ���������� ��� �������
			nvar - ��� ����������, �� ������� ��������� ������� ������. ��� ������� � ��������� (���� ��� �������� ���������)
			*/
			wstring read(wstring arg)
			{
				wchar_t* id_a = wcsstr(&var_id[0], L"@");
				wchar_t* id_H = wcsstr(&var_id[0], L"#");
				wchar_t* id_f = wcsstr(&var_id[0], L"(");
				wchar_t* id_b = wcsstr(&var_id[0], L")");
				wstring out;

				if (arg == L"type")
				{
					if (id_a != NULL)
					{
						out.assign(var_id, 0, 5);
					}
					else
					{
						if (var_id == L"0")
						{
							out = L"numbr";
						}
						else
						{
							out = L"opert";
						}
					}
				}
				else if (arg == L"prop")
				{
					if (id_H != NULL)
					{
						out.assign(id_H + 1);
					}
					else
					{
						out = L"0";
					}
				}
				else if (arg == L"name")
				{
					if (id_a != NULL)
					{
						if (id_f != NULL)
						{
							if (id_a + 1 == id_f)
							{
								out = L"";
							}
							else
								out.assign(id_a + 1, id_f - 1);
						}
						else
						{
							if (id_a + 1 == id_H)
							{
								out = L"";
							}
							else
								out.assign(id_a + 1, id_H - 1);
						}
					}
					else
					{
						out = var_id;
					}
				}
				else if (arg == L"func")
				{
					if (id_a != NULL)
						if (id_H != NULL)
							if (id_a + 1 == id_H)
								out = L"";
							else
								out.assign(id_a + 1, id_H - 1);
						else
							out.assign(id_a + 1);
					else
						out = var_id;
				}
				else if (arg == L"nvar")
				{
					if ((point_collar != NULL) && (point_collar->read(L"type") == L"varbl"))
						out = point_collar->read(L"name");
					else
						out = L"";
					/* TODO: ������� �������� ������������ ����� ���������� �� ������� ��������� ������� � ����� ���������� � �������� �������.
					���� �� ���������, �������� ��� � �������� �-�: f(x) -> f(y)
					if ((id_f != NULL) && (id_b != NULL))
					{
						out.assign(id_f + 1, id_b - 1);
					}
					else
						out = L"";
						*/
				}
				else
				{
					out = L"fuck up";
				}
				return out;
			}

			/*����� �������� ����������� �������, ���������� �� ������ �������� � ������������� ������ � ���������� ������� �������� ���������.
			���������� ������. ���� �� ��������*/
			wstring expresion()
			{
				//TODO: ����������!
				return read(L"name") + L" = " + expression_processing(point_left);
			}

			/*����� �������� ����������� ������� �������� ���������� �������� � ������� ������ ��������.
			���������� ��������� �� ������� ������ (��������), ������� ��������� ������� ��� ������ � ��������� � ������� ���������. */
			var_const *prioritize(int current_priority)
			{
				return prioritize_processing(point_collar, current_priority);
			}

			/*����� �������� ����������� �������, ���������� �� ������ �������� � ����������� ��.
			����������� ������ ������ �������� ������ ���������� ���������� � double var �������� �������� ������. */
			void arithmetic()
			{
				var = processing(point_left);
			}


			enum class variable_type {
				INTEGER,
				FRACTIONAL,
				IMAGINARY,
				REAL,
				RE_IM,
				EXP	//5E+10 5*10^10
			};

			wstring var_id;
			double var;
			//double var_im;  �� �����. �������� ������������ �������
			int exp;
			var_const *point_left;		//����� �����
			var_const *point_right;		//������ �����
			var_const *point_collar;	//��������
		};

		var_const pi = var_const(L"const@pi", 3.1415926535897932384626433832);
		var_const e = var_const(L"const@e", 2.7182818284590452353602874713527);

		vector<var_const*>* general_var_const;

		void Init() {
			ProjectError::Init();
			general_var_const = new vector<var_const*>;
			general_var_const->push_back(&pi);
			general_var_const->push_back(&e);
			general_var_const->push_back(new var_const(L"funct@minus#empty", 0));
		}

		var_const* run_vector(wstring _pv)
		{
			size_t temp_size_of_vect = general_var_const->size();
			int count;
			for (count = 0; count < temp_size_of_vect; count++)
			{
				if (_pv == general_var_const->at(count)->read(L"name"))
					return general_var_const->at(count);
			}
			if (count == temp_size_of_vect)
			{
				return NULL;
			}
		}


		var_const* filling_vector(wchar_t* strPtr, wchar_t*endPtr, var_const* c_e, int brakets)
		{
			size_t temp_size_of_vect;
			wchar_t* pDest = strPtr;
			unsigned int count;
			var_const *high_pointer = NULL;
			var_const *low_pointer = NULL;
			var_const *temp_pointer = NULL;
			wchar_t* temp;
			wstring name;
			var_const* current_element = c_e;
			double num;
			int brakets_counter = 0;
			while (pDest <= endPtr) {

				if (*pDest == '=')
				{
					//���� ����� ����� � ����� ������ ������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//high_pointer = new var_const(L"error@", 1);
						//general_var_const->pop_back(); //�� ��������
						//return high_pointer;						

						ProjectError::SetProjectLastError(ProjectError::ErrorCode::EQUALY_FIRST);
						return NULL;
					}
					//����� � ����� ������, ������ �� ��������
					else if (pDest == endPtr)
					{
						//������ ����� ������ ������� ����������� ������� � ���������� solve, � ��� ��� �������� ��� ��������� � ������� ��������� ��� �������� ������
						current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"solve");
					}
					else
					{
						temp_pointer = filling_vector(pDest + 1, endPtr, new var_const(L"exprs@#undef", 0, low_pointer), brakets + brakets_counter);
						if (current_element->read(L"type") == L"const")
						{							
							if (temp_pointer->read(L"type") == L"funct")
							{
								current_element->point_left = temp_pointer->point_left;
								current_element->point_collar = temp_pointer->point_collar;
								current_element->point_right = temp_pointer->point_right;
								name = temp_pointer->read(L"func");
								current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), name);
								current_element->var_id.replace(0, 5, L"funct");
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");								
							}
							else if (temp_pointer->read(L"type") == L"varbl")
							{
								if (temp_pointer->read(L"prop") == L"undef")
								{
									temp_pointer->var_id.replace(temp_pointer->var_id.find_first_of(L'#') + 1, 5, L"defnd");
									general_var_const->push_back(temp_pointer);
								}
								current_element->point_collar->copy(temp_pointer);
								current_element->point_left = current_element->point_collar;
								name = temp_pointer->read(L"name");
								current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + name + L")");
								current_element->var_id.replace(0, 5, L"funct");
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");								
							}
							else if (temp_pointer->read(L"type") == L"exprs")
							{
								current_element->point_left = temp_pointer->point_left;
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");
							}
							else if (temp_pointer->read(L"type") == L"const")
							{
								current_element->var = temp_pointer->var;
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");
							}							
						}
						else if (current_element->read(L"type") == L"varbl")
						{
							if (temp_pointer->read(L"type") == L"funct")
							{
								current_element->point_left = temp_pointer->point_left;
								current_element->point_collar = temp_pointer->point_collar;
								current_element->point_right = temp_pointer->point_right;
								name = temp_pointer->read(L"func");
								current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), name);
								current_element->var_id.replace(0, 5, L"funct");
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");
							}
							else if (temp_pointer->read(L"type") == L"varbl")
							{
								if (temp_pointer->read(L"prop") == L"undef")
								{
									temp_pointer->var_id.replace(temp_pointer->var_id.find_first_of(L'#') + 1, 5, L"defnd");
									general_var_const->push_back(temp_pointer);
								}
								current_element->point_collar->copy(temp_pointer);
								current_element->point_left = current_element->point_collar;
								name = temp_pointer->read(L"name");
								current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + name + L")");
								current_element->var_id.replace(0, 5, L"funct");
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");
							}
							else if (temp_pointer->read(L"type") == L"exprs")
							{
								current_element->point_left = temp_pointer->point_left;
								current_element->var_id.replace(0, 5, L"const");
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");
							}
							else if (temp_pointer->read(L"type") == L"const")
							{
								current_element->var = temp_pointer->var;
								current_element->var_id.replace(0, 5, L"const");
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");
							}
						}
						else if (current_element->read(L"type") == L"funct")
						{
							if (temp_pointer->read(L"type") == L"funct")
							{
								if (current_element->read(L"nvar") == temp_pointer->read(L"nvar"))
								{
									if (current_element->read(L"prop") == L"defnd")
									{
										high_pointer->copy(current_element);
										current_element = new var_const(L"equat@("+ high_pointer->read(L"nvar") + L")#unslv", 0, high_pointer, temp_pointer->point_right, temp_pointer->point_collar);
									}
									else
									{
										current_element->var_id.replace(0, 5, L"equat");
										current_element->point_right = temp_pointer->point_right;
										current_element->point_collar = temp_pointer->point_collar;
										current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"unslv");
									}
								}
								else
								{
									//��� �������� �������/��������� ���������� ����������									
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
									return NULL;
								}							
							}
							else if (temp_pointer->read(L"type") == L"varbl")
							{
								if (temp_pointer->read(L"prop") == L"undef")
								{
									temp_pointer->var_id.replace(temp_pointer->var_id.find_first_of(L'#') + 1, 5, L"defnd");
									general_var_const->push_back(temp_pointer);
								}
								current_element->point_collar->copy(temp_pointer);
								current_element->point_left = current_element->point_collar;
								name = temp_pointer->read(L"name");
								current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + name + L")");
								current_element->var_id.replace(0, 5, L"funct");
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");
							}
							else if (temp_pointer->read(L"type") == L"exprs")
							{
								current_element->point_left = temp_pointer->point_left;
								current_element->var_id.replace(0, 5, L"const");
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");
							}
							else if (temp_pointer->read(L"type") == L"const")
							{
								current_element->var = temp_pointer->var;
								current_element->var_id.replace(0, 5, L"const");
								current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"defnd");
							}
						}
						else if (current_element->read(L"type") == L"exprs")
						{
							high_pointer = new var_const(L"error@", -4);
							general_var_const->pop_back();
							return high_pointer;

						}

					}
					pDest++;
				}
				else if (*pDest == '+')
				{
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
						
					}
					//���� ��� ������ �������� � ���������
					else if (high_pointer == low_pointer)
					{
						//���������� ��������, ����� ����� -> �� ���������� �����, �������� - �� ������� ���������
						current_element->point_left = new var_const(L"+", brakets_counter + brakets, low_pointer, NULL, current_element);
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_element->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + 1))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = new var_const(L"+", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
							high_pointer = new var_const(L"+", brakets_counter + brakets, current_element->point_left, NULL, current_element);
							//�������� ���������� ��������� �������� -> �� ����� ��������
							current_element->point_left->point_collar = high_pointer;
							//��������� ����� ������� ��������� �� ��������� ��������
							current_element->point_left = high_pointer;

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=");
					if (pDest == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*pDest == '*')
				{
					//���� ��� ������ �������� � ���������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
					//���� ��� ������ �������� � ���������
					else if (high_pointer == low_pointer)
					{
						//���������� ��������, ����� ����� -> �� ���������� �����, �������� - �� ������� ���������
						current_element->point_left = new var_const(L"*", brakets_counter + brakets, low_pointer, NULL, current_element);
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_element->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + 2))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = new var_const(L"*", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
							high_pointer = new var_const(L"*", brakets_counter + brakets, current_element->point_left, NULL, current_element);
							//�������� ���������� ��������� �������� -> �� ����� ��������
							current_element->point_left->point_collar = high_pointer;
							//��������� ����� ������� ��������� �� ��������� ��������
							current_element->point_left = high_pointer;

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=");
					if (pDest == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*pDest == '/')
				{
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
					//���� ��� ������ �������� � ���������
					else if (high_pointer == low_pointer)
					{
						//���������� ��������, ����� ����� -> �� ���������� �����, �������� - �� ������� ���������
						current_element->point_left = new var_const(L"/", brakets_counter + brakets, low_pointer, NULL, current_element);
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_element->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + 3))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = new var_const(L"/", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
							high_pointer = new var_const(L"/", brakets_counter + brakets, current_element->point_left, NULL, current_element);
							//�������� ���������� ��������� �������� -> �� ����� ��������
							current_element->point_left->point_collar = high_pointer;
							//��������� ����� ������� ��������� �� ��������� ��������
							current_element->point_left = high_pointer;

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=");
					if (pDest == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*pDest == '^')
				{
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
					//���� ��� ������ �������� � ���������
					else if (high_pointer == low_pointer)
					{
						//���������� ��������, ����� ����� -> �� ���������� �����, �������� - �� ������� ���������
						current_element->point_left = new var_const(L"^", brakets_counter + brakets, low_pointer, NULL, current_element);
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_element->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + 4))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = new var_const(L"^", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
							high_pointer = new var_const(L"^", brakets_counter + brakets, current_element->point_left, NULL, current_element);
							//�������� ���������� ��������� �������� -> �� ����� ��������
							current_element->point_left->point_collar = high_pointer;
							//��������� ����� ������� ��������� �� ��������� ��������
							current_element->point_left = high_pointer;

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=");
					if (pDest == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*pDest == '-')
				{
					//�������� ����� ���������� ��� �������� � ������������� ������					
					//���� ��������� ���������� � ������ (������ ����� ����� ������ ��� �����, ����� ����� ������� �� ����������)
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//�������� �������� ������ � ������ �����, �������� -> ���������
						current_element->point_left = new var_const(general_var_const->at(2));
						current_element->point_left->var_id.replace(current_element->point_left->var_id.find_first_of(L"#") + 1, 5, L"defnd");

						//��� ��������� -> �� �����, �������� ��� �������� ������� ��� ������ ��������
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					//���� ���� ������ �����/�������� ����� �������
					else
					{
						//���� ��� ������ �������� � ��������� (��������)
						if (high_pointer == low_pointer)
						{
							//������� ���������� ��������, ����� ����� -> �� ���������� �����, �������� �� �����
							current_element->point_left = new var_const(L"+", brakets_counter, low_pointer, NULL, current_element);
							high_pointer = current_element->point_left;
							high_pointer->point_right = new var_const(general_var_const->at(2));
							high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L"#") + 1, 5, L"defnd");

							low_pointer = high_pointer->point_right;
						}
						//���� ����� ���� ������ ��������
						else
						{
							//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
							if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
							{
								//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
								high_pointer->point_right = new var_const(L"+", brakets_counter, low_pointer, NULL, high_pointer);
								high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
								high_pointer->point_right = new var_const(general_var_const->at(2));
								high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L"#") + 1, 5, L"specf");
								low_pointer = high_pointer->point_right;
							}

							else
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new var_const(L"+", brakets_counter, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
								high_pointer->point_right = new var_const(general_var_const->at(2));
								high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L"#") + 1, 5, L"defnd");
								low_pointer = high_pointer->point_right;
							}
						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=");
					if (pDest == temp)
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
				}
				else if (*pDest == '(')
				{
					//����� ������ ����������� ������� ������ �� �����, ������ ������ �������� ���������� �������
					brakets_counter += 4;
					temp = pDest;
					count = 1;
					while (count != 0)
					{
						temp++;
						//���� ������ � ����� ������� �� ����� ����������� ������
						if (temp == endPtr)
						{
							//high_pointer = new var_const(L"error@", 2);
							//general_var_const->pop_back();
							//return high_pointer;
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
							return NULL;
						}
						else if (*temp == '(')
						{
							count++;
						}
						else if (*temp == ')')
						{
							count--;
						}
						else if (*temp == '=') //�������� ������ � ������ �����
						{
							//high_pointer = new var_const(L"error@", 3);
							//general_var_const->pop_back();
							//return high_pointer;
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
							return NULL;
						}
					}
					if (pDest + 1 == temp)
					{
						//������ ������ � �������. ����� �� ������, �� ����� � ���������)
					}
					else
					{						
						temp_pointer = filling_vector(pDest + 1, temp - 1, new var_const(L"exprs@#undef", 0, low_pointer), brakets + brakets_counter);
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							if ((temp_pointer->read(L"type") == L"exprs") || (temp_pointer->read(L"type") == L"const"))
							{
								high_pointer = temp_pointer;
								low_pointer = high_pointer;
							}
							else if (temp_pointer->read(L"type") == L"varbl")
							{
								current_element->var_id.replace(0, 5, L"funct");
								current_element->var_id.insert(current_element->var_id.find_first_of('@') + 1, L"(" + temp_pointer->read(L"name") + L")");
								if (temp_pointer->read(L"prop") == L"undfn")
								{
									temp_pointer->var_id.replace(current_element->var_id.find_first_of('#') + 1, 5, L"defnd");
									general_var_const->push_back(new var_const(temp_pointer));
								}
								current_element->point_collar = temp_pointer;
								current_element->point_left = temp_pointer;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else if (temp_pointer->read(L"type") == L"funct")
							{
								current_element->var_id.replace(0, 5, L"funct");
								current_element->var_id.insert(current_element->var_id.find_first_of('@') + 1, L"(" + temp_pointer->read(L"nvar") + L")");
								current_element->point_collar = temp_pointer->point_collar;
								current_element->point_left = temp_pointer;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						else if (low_pointer = high_pointer)
						{
							// ������� ������ ���-�� ����� - ����� ��� ����� - ����� ������
						}
						else if (high_pointer->point_right == NULL)
						{
							if ((temp_pointer->read(L"type") == L"exprs") || (temp_pointer->read(L"type") == L"const"))
							{
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
							else if (temp_pointer->read(L"type") == L"varbl")
							{
								if (current_element->read(L"type") == L"funct")
								{
									if (current_element->read(L"nvar") == temp_pointer->read(L"name"))
									{
										high_pointer->point_right = temp_pointer;
										low_pointer = high_pointer->point_right;
									}
									else
									{
										//��� �������� �������/��������� ���������� ����������
										//high_pointer = new var_const(L"error@", -5);
										//general_var_const->pop_back();
										//return high_pointer;
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
										return NULL;
									}
								}
								else if (current_element->read(L"type") == L"exprs")
								{
									current_element->var_id.replace(0, 5, L"funct");
									current_element->var_id.insert(current_element->var_id.find_first_of('@') + 1, L"(" + temp_pointer->read(L"name") + L")");
									if (temp_pointer->read(L"prop") == L"undfn")
									{
										temp_pointer->var_id.replace(current_element->var_id.find_first_of('#') + 1, 5, L"defnd");
										general_var_const->push_back(new var_const(temp_pointer));
									}
									current_element->point_collar = temp_pointer;
									high_pointer->point_right = current_element->point_collar;
									low_pointer = high_pointer->point_right;;
								}
							}
							else if (temp_pointer->read(L"type") == L"funct")
							{
								if (current_element->read(L"type") == L"funct")
								{
									if (current_element->read(L"nvar") == temp_pointer->read(L"nvar"))
									{
										high_pointer->point_right = temp_pointer;
										low_pointer = high_pointer->point_right;
									}
									else
									{
										//��� �������� �������/��������� ���������� ����������
										//high_pointer = new var_const(L"error@", -5);
										//general_var_const->pop_back();
										//return high_pointer;
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
										return NULL;
									}
								}
								else if (current_element->read(L"type") == L"exprs")
								{
									current_element->var_id.replace(0, 5, L"funct");
									current_element->var_id.insert(current_element->var_id.find_first_of('@') + 1, L"(" + temp_pointer->read(L"nvar") + L")");
									current_element->point_collar = temp_pointer->point_collar;
									high_pointer->point_left = temp_pointer;
									low_pointer = high_pointer->point_left;
								}
							}
						}
						else
						{
							if (low_pointer->read(L"type") == L"funct")
							{
								if (low_pointer->read(L"name") == L"minus")
								{
									if (low_pointer->point_right == NULL)
									{
										//��� ��������
										low_pointer->point_right = temp_pointer;
									}
									else
									{
										//��� ������ ���� - ����� ����� �������� ��� ����� ��� ���������� � ������ ������
										//high_pointer = new var_const(L"error@", -7);
										//general_var_const->pop_back();
										//return high_pointer;
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::NEGATIVE_FUNC);
										return NULL;
									}
								}
								else
								{
									if (temp_pointer->read(L"type") == L"exprs")
									{
										low_pointer->point_right = temp_pointer->point_left;
									}
									else if (temp_pointer->read(L"type") == L"funct")
									{
										if (current_element->point_collar == NULL)
										{
											low_pointer->point_right = temp_pointer;
											low_pointer->point_collar = temp_pointer->point_collar;
											current_element->point_collar = temp_pointer->point_collar;
											current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + temp_pointer->read(L"nvar") + L")");
										}
										else
										{
											if (temp_pointer->read(L"nvar") == current_element->read(L"nvar"))
											{
												low_pointer->point_right = temp_pointer;
											}
											else
											{
												//��� �������� �������/��������� ���������� ����������
												//high_pointer = new var_const(L"error@", -5);
												//general_var_const->pop_back();
												//return high_pointer;
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
												return NULL;
											}
										}
									}
									else if (temp_pointer->read(L"type") == L"const")
									{
										low_pointer->point_right = temp_pointer;
									}
									else if (temp_pointer->read(L"type") == L"varbl")
									{
										if (current_element->point_collar == NULL)
										{
											low_pointer->point_collar = temp_pointer;
											current_element->point_collar = temp_pointer;
											current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + temp_pointer->read(L"namer") + L")");
										}
										else
										{
											if (temp_pointer->read(L"name") != current_element->read(L"nvar"))
											{
												//��� �������� �������/��������� ���������� ����������
												/*high_pointer = new var_const(L"error@", -5);
												general_var_const->pop_back();
												return high_pointer;*/
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
												return NULL;
											}
										}
									}
								}
							}
							else if (low_pointer->read(L"type") == L"varbl")
							{

							}
							else if (low_pointer->read(L"type") == L"const")
							{

							}
							else if (low_pointer->read(L"type") == L"numbr")
							{
								//�������� ���� ������ ����� ��������� � ������� ��������� �����
							}
							else if (low_pointer->read(L"type") == L"opert")
							{
								//high_pointer = new var_const(L"error@", -6);
								//general_var_const->pop_back();
								//return high_pointer;
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::INTERNAL_POINTER_ERR);
								return NULL;
							}
						}
					}
					//����� ��������� � ��������� ������� ����� ������
					brakets_counter -= 4;
					pDest = temp + 1;
					if (*pDest == '(')
					{
						//�� ���� ������ ���� )( ����� �������� )*(. ����� ������� ���� ����������� ��� ��������� ������
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
						return NULL;
					}
				}
				else if ((*pDest == '1') || (*pDest == '2') || (*pDest == '3') || (*pDest == '4') || (*pDest == '5') || (*pDest == '6') || (*pDest == '7') || (*pDest == '8') || (*pDest == '9') || (*pDest == '0'))
				{                   //����� ����� ����� id "0"					
					// ���� ������ ����� ������ � ������ ���������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//�������� �������� ������ � ������ �����, �������� -> ���������
						current_element->point_left = new var_const(L"0", wcstod(pDest, &pDest), current_element);
						//��� ��������� -> �� �����, �������� ��� �������� ������� ��� ������ ��������
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					// ���� ������ ����� �� ������ � ������ (���� �����-���� ��������)
					else
					{
						//�������� �������� ������ � ������ �����, �������� -> ���� �������� 						
						if (low_pointer->read(L"name") == L"minus")
						{
							low_pointer->point_right = new var_const(L"0", wcstod(pDest, &pDest), low_pointer);
						}
						else
						{
							high_pointer->point_right = new var_const(L"0", wcstod(pDest, &pDest), high_pointer);
							low_pointer = high_pointer->point_right;
						}
					}
				}
				else
				{
					temp_size_of_vect = general_var_const->size();
					temp = wcspbrk(pDest, L"()+-*^/=");
					//���� ������� ���������� � c������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						if ((temp == NULL) && (brakets == 0))
						{
							//high_pointer = new var_const(L"error@", 2);
							//general_var_const->pop_back();
							//return high_pointer;
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::EQUALY_MISSING);
							return NULL;
						}
						else if (temp == NULL)
						{
							name.assign(pDest, endPtr);
							pDest = endPtr;
						}
						else
						{
							name.assign(pDest, temp);
							pDest = temp;
						}
						high_pointer = run_vector(name);
						if (high_pointer == NULL)
						{
							if (temp == NULL)
							{
								current_element->var_id.replace(0, 6, L"varbl@");
								current_element->var_id += name;
								current_element->var_id += L"#undef";
							}
							else if (*temp == '=')
							{
								current_element->var_id.replace(0, 6, L"varbl@");
								current_element->var_id += name;
								current_element->var_id += L"#undef";
							}
							else if (*temp == '(')
							{
								current_element->var_id.replace(0, 6, L"funct@");
								current_element->var_id += name;
								current_element->var_id += L"#undef";
							}
							else if (*temp == ')')
							{
								//high_pointer = new var_const(L"error@", 7);
								//general_var_const->pop_back();
								//return high_pointer;
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							else
							{
								current_element->var_id = L"funct@";
								current_element->var_id += L"(" + name + L")";
								current_element->var_id += L"#undef";
								name.insert(0, L"varbl@");
								name += L"#defnd";
								general_var_const->push_back(new var_const(name, 0));
								current_element->point_collar = new var_const(general_var_const->at(temp_size_of_vect));
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}

						}
						else if (high_pointer->read(L"type") == L"const")
						{
							if (temp == NULL)
							{
								current_element->copy(high_pointer);
							}
							else if (*temp == '=')
							{
								current_element = high_pointer;								
								general_var_const->pop_back();
							}
							else if (*temp == '(')
							{
								current_element = high_pointer;
								general_var_const->pop_back();
								current_element->point_collar = new var_const();
							}
							else if (*temp == ')')
							{
								/*high_pointer = new var_const(L"error@", 7);
								general_var_const->pop_back();
								return high_pointer;*/
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							else
							{
								current_element->point_left = new var_const(high_pointer);
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						else if (high_pointer->read(L"type") == L"varbl")
						{
							if (temp == NULL)
							{
								current_element->copy(high_pointer);

							}
							else if (*temp == '=')
							{
								
								current_element = high_pointer;
								general_var_const->pop_back();
							}
							else if (*temp == ')')
							{
								/*high_pointer = new var_const(L"error@", 7);
								general_var_const->pop_back();
								return high_pointer;*/
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							else if (*temp == '(')
							{

								current_element = high_pointer;
								general_var_const->pop_back();
								current_element->point_collar = new var_const();
							}
							else
							{
								current_element->var_id = L"funct@";
								current_element->var_id += L"(" + name + L")";
								current_element->var_id += L"#undef";
								current_element->point_collar = new var_const(high_pointer);
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						else if (high_pointer->read(L"type") == L"funct")
						{
							if (temp == NULL)
							{
								current_element->copy(high_pointer);

							}
							else if (*temp == '=')
							{
								current_element = high_pointer;
								general_var_const->pop_back();
							}
							else if (*temp == ')')
							{
								/*high_pointer = new var_const(L"error@", 7);
								general_var_const->pop_back();
								return high_pointer;*/
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							else if (*temp == '(')
							{
								current_element->var_id = L"funct@";
								current_element->var_id += L"(" + high_pointer->read(L"nvar") + L")";
								current_element->var_id += L"#undef";
								current_element->point_left = new var_const(high_pointer);
								current_element->point_collar = high_pointer->point_collar;
								high_pointer = current_element->point_left;
								high_pointer->var_id.replace(high_pointer->var_id.find_first_of('#') + 1, 5, L"undef");
								low_pointer = high_pointer;
							}
							else
							{
								current_element->var_id = L"funct@";
								current_element->var_id += L"(" + high_pointer->read(L"nvar") + L")";
								current_element->var_id += L"#undef";
								current_element->point_left = new var_const(high_pointer);
								current_element->point_collar = high_pointer->point_collar;
								high_pointer = current_element->point_left;
								high_pointer->var_id.replace(high_pointer->var_id.find_first_of('#') + 1, 5, L"defnd");
								low_pointer = high_pointer;
							}
						}
						//������ �� � ������ ������
						else
						{
							if (temp == NULL)
							{
								name.assign(pDest, endPtr);
								pDest = endPtr;
							}
							else
							{
								name.assign(pDest, temp);
								pDest = temp;
							}
							low_pointer = run_vector(name);
							if (low_pointer == NULL)
							{
								if (current_element->read(L"type") == L"funct")
								{
									//��� �������� �������/��������� ���������� ����������
									/*high_pointer = new var_const(L"error@", -5);
									general_var_const->pop_back();
									return high_pointer;*/
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
									return NULL;
								}

								else if (current_element->read(L"type") == L"exprs")
								{
									// ��������� � undef ������� 
									current_element->var_id.replace(0, 6, L"funct@");
									current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"undef");
									current_element->var_id.insert(current_element->var_id.find_first_of(L'@') + 1, L"(" + name + L")");
									name.insert(0, L"varbl@");
									name += L"#defnd";
									general_var_const->push_back(new var_const(name, 0));
									current_element->point_collar = new var_const(general_var_const->at(temp_size_of_vect));
									if (high_pointer->point_right->read(L"name") == L"minus")
									{
										high_pointer->point_right->point_right = current_element->point_collar;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}
							}
							else if (low_pointer->read(L"type") == L"varbl")
							{
								if (current_element->read(L"type") == L"funct")
									if (low_pointer->read(L"name") != current_element->read(L"nvar"))
									{
										//��� �������� �������/��������� ���������� ����������
										/*high_pointer = new var_const(L"error@", -5);
										general_var_const->pop_back();
										return high_pointer;*/
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
										return NULL;
									}
									else
									{
										if (high_pointer->point_right->read(L"name") == L"minus")
										{
											high_pointer->point_right->point_right = current_element->point_collar;
											low_pointer = high_pointer->point_right;
										}
										else
										{
											high_pointer->point_right = current_element->point_collar;
											low_pointer = high_pointer->point_right;
										}
									}

								else if (current_element->read(L"type") == L"exprs")
								{
									current_element->var_id.replace(0, 6, L"funct@");
									current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"undef");
									current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + name + L")");
									current_element->point_collar = new var_const(low_pointer);
									if (high_pointer->point_right->read(L"name") == L"minus")
									{
										high_pointer->point_right->point_right = current_element->point_collar;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}
							}
							else if (low_pointer->read(L"type") == L"const")
							{
								if (high_pointer->point_right->read(L"name") == L"minus")
								{
									high_pointer->point_right->point_right = new var_const(low_pointer);
								}
								else
								{
									high_pointer->point_right = new var_const(low_pointer);
								}
								low_pointer = high_pointer->point_right;
							}
							else if (low_pointer->read(L"type") == L"funct")
							{
								if (current_element->read(L"type") == L"funct")
								{
									if (low_pointer->read(L"nvar") == current_element->read(L"nvar"))
									{
										if (high_pointer->point_right->read(L"name") == L"minus")
										{
											high_pointer->point_right->point_right = new var_const(low_pointer);
										}
										else
										{
											high_pointer->point_right = new var_const(low_pointer);
										}
										low_pointer = high_pointer->point_right;
									}
									else
									{
										//��� �������� �������/��������� ���������� ����������
										/*high_pointer = new var_const(L"error@", -5);
										general_var_const->pop_back();
										return high_pointer;*/
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
										return NULL;
									}
								}
								else if (current_element->read(L"type") == L"exprs")
								{
									if (*temp == '(')
									{
										current_element->var_id.replace(0, 6, L"funct@");
										current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"undef");
										if (high_pointer->point_right->read(L"name") == L"minus")
										{
											high_pointer->point_right->point_right = new var_const(low_pointer);
											high_pointer->point_right->point_right->var_id.replace(high_pointer->point_right->point_right->var_id.find_first_of(L'#') + 1, 5, L"defnd");
										}
										else
										{
											high_pointer->point_right = new var_const(low_pointer);
											high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L'#') + 1, 5, L"defnd");
										}
										low_pointer = high_pointer->point_right;
									}
									else
									{
										//��� ����� ��� �������� �� ������� �������. ��� ��� ������ ���� ���� ��������� ����������.
										current_element->var_id.replace(0, 6, L"funct@");
										current_element->var_id.replace(current_element->var_id.find_first_of(L'#') + 1, 5, L"undef");
										current_element->var_id.insert(current_element->var_id.find_first_of(L'#'), L"(" + low_pointer->read(L"nvar") + L")");
										if (high_pointer->point_right->read(L"name") == L"minus")
										{
											high_pointer->point_right->point_right = new var_const(low_pointer);
											high_pointer->point_right->point_right->var_id.replace(high_pointer->point_right->point_right->var_id.find_first_of(L'#') + 1, 5, L"defnd");
										}
										else
										{
											high_pointer->point_right = new var_const(low_pointer);
											high_pointer->point_right->var_id.replace(high_pointer->point_right->var_id.find_first_of(L'#') + 1, 5, L"defnd");
										}
										current_element->point_collar = high_pointer->point_right->point_collar;
										low_pointer = high_pointer->point_right;
									}
								}
							}
						}
					}
				}
			}
			return current_element;
		}

		/*������� ��������� ��������/������ ���������� ���������� ������ �������� � �������� ������������.
		���������� ������ � ����������� ������� �������� ����������.*/
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, var_const* _current_element)
		{
			var_const* CE = filling_vector(_pDest, _endPtr, _current_element, 0);
			wstring output;
			size_t output_size;
			//if (_current_element->read(L"type") == L"error")
			if (CE == NULL)
			{
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				return err->GetErrorWStr();
				//TODO: ������!
				/*���� ���� ��������� ���� ������ � ����������� �����, ������� ����� ���������� ��� ���������� ������
				����������
				///-1 - ������ ���� f(y) - ��� y - ����������� ������� �������. ���������/������� ������� �� �����������.
				///-2 - ������ ���� f(x)+2...=..., f+2...=... ��� ������������� ��� ����������� ������� f. ����� ������������� ��� ������� �������
				������� ��� �������������� ���������, �� ���� �� �������
				!//������ ������? ��� �� ���, ��� ����!//-3 - ������ ���� f(2+3*x), f(2*5) - ������� �������� ������� ��� ������������� ������������ ��������� ��� ���������
				///-4 - ��������� ��������� ������������, ��� ������ �������� ��� ������� ����� ���������
				������
				///-5 - �������/��������� ���������� ����������
				///-6 - �����-�� ������� low_pointer �������� �� ��������
				///-7 - ����� ����� ��������

				///1 - ������ �������� � ������ ����� �����
				///2 - ������ ��������, �� ������� ����� ����� � (���������!) ����� ��������������// ���� ����� �� ����������, ��� �������� ����������� ��������
				�� ��������� � ����������
				///3 - ������ ���� (... = - ������ ������� � �� ������� ����� �����
				///4 - ������� �������� ��������� ��������� ���� x(2), ����� x ������������
				///5 - ������� �������� ��������� ��������� ���� x(2), ����� x ���������� ��� ����������
				///6 - ������ ���� f(2xyz...) - ����������/��������� �� ����� ���������� � ����, ���� ������� ����� ��������� - ������ �����, ��� ������
				��������
				///7 - ������ ���� f) - � ����� ������ ������ ������� � ����������� ������ ��� �����������, �������� ���� ���-�� ��������� �������
				///8 - ������ ���� �+2=(����� ������) ��� ������������� ��� ����������� ��� ���������� �. ��������� ��������� ����� ������
				����� ������ ���������
				!///9 - � ������ ������ ��� ����� ������??? ����� ���� ��������
				*/
			}
			else if (CE->read(L"type") == L"exprs")
			{
				CE->arithmetic();
				output = to_string(CE->var, var_type::FRACTIONAL, 2);
			}
			else
			{
				//else else esle!
			}
			return output;
		}

		/*������� �������� ������ ����� � �������������� ����.
		���������� ������ � ����������� ������� �������� ����������.*/
		wstring input_to_analize(wchar_t* input)
		{
			if (!Project::IO::VerifyInput(input)) {
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				return err->GetErrorWStr();
			}

			size_t size_of_vect = general_var_const->size();

			wstring temp = L"exprs@#undef";
			general_var_const->push_back(new var_const(temp, 0));

			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer	
			return analized_output(point_start, point_end, general_var_const->at(size_of_vect));
		}
	}
}
