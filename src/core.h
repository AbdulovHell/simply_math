///Macros
#define GET_FRAC(var) ((double)((double)var-(int)var))
#define GET_INTGR(var) ((int)var)
///
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
//#include <thread>
//#include <mutex>
#include "error.h"
#include "IO.h"

namespace Project {
	namespace Core {

		using namespace std;
		using namespace Project;
		using namespace Project::IO;

		//���� ��������	(type)	
		wstring cnst = L"const";
		wstring funct = L"funct";
		wstring varbl = L"varbl";
		wstring equat = L"equat";
		wstring exprs = L"exprs";
		wstring numbr = L"numbr";
		wstring addit = L"oper+";
		wstring mltpl = L"oper*";
		wstring divis = L"oper/";
		wstring power = L"oper^";
		//�������� �������� (prop)		
		wstring defnd = L"defnd";
		wstring undef = L"undef";
		wstring fundm = L"fundm";
		wstring unslv = L"unslv";
		wstring empty = L"empty";
		wstring real = L"real_";
		wstring cmplx = L"cmplx";
		//�������� ��� ��������� (actn)
		wstring solve = L"solve";
		wstring write = L"write";
		wstring read = L"read_";

		class var_const {
		private:
			int tree_destruct_processing(var_const* pointer)
			{
				if (pointer == NULL) return 0;
				int temp = 0;

				if (pointer->type == numbr)
				{
					delete pointer;
					pointer = NULL;
					return 0;
				}
				else if ((pointer->type == addit) || (pointer->type == mltpl) || (pointer->type == divis) || (pointer->type == power))
				{
					temp += tree_destruct_processing(pointer->point_left);
					temp += tree_destruct_processing(pointer->point_right);
					delete pointer;
					pointer = NULL;
					return temp;

				}
				else if (pointer->type == exprs)
				{
					temp += tree_destruct_processing(pointer->point_left);
					delete pointer;
					pointer = NULL;
					return temp;
				}
				else if (pointer->type == funct)
				{
					if (pointer->prop == undef)
					{
						temp += tree_destruct_processing(pointer->point_left);
						temp += tree_destruct_processing(pointer->point_right);
						delete pointer->point_collar;
						delete pointer;
						pointer = NULL;
						return temp;
					}
					else
					{
						temp += tree_destruct_processing(pointer->point_right);
						delete pointer->point_collar;
						delete pointer;
						pointer = NULL;
						return temp;
					}
				}

			}

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

			double processing(var_const *pointer, var_const * last_func)
			{
				//TODO: ������������ �� ������ ����������

				if (pointer->type == addit)
					return processing(pointer->point_left, last_func) + processing(pointer->point_right, last_func);
				else if (pointer->type == mltpl)
					return processing(pointer->point_left, last_func) * processing(pointer->point_right, last_func);
				else if (pointer->type == divis)
					return processing(pointer->point_left, last_func) / processing(pointer->point_right, last_func);
				else if (pointer->type == power)
					return pow(processing(pointer->point_left, last_func), processing(pointer->point_right, last_func));
				else if ((pointer->type == numbr) || (pointer->type == cnst))
					return pointer->var;
				else if (pointer->type == exprs)
					return processing(pointer->point_left, last_func);
				//��� ������� - ������ �������� �� ��������� ������ � ��������� ��� ��
				else if (pointer->type == funct)
				{
					//�������� ���������� ����� ������� ������� ��� ������� ����, �� � �� ��������. ������ ����� ��������� ���� ����� - ���������� � ����.
					/*if (pointer->read(L"name") == L"sgn")
					{
						return signum(processing(pointer->point_right, pointer));
					}
					else*/ if (pointer->name == L"root")
					{
						return sqrt(processing(pointer->point_right, last_func));
					}
					else
						return processing(pointer->point_left, pointer);
				}
				//����� ������� ���������� - ��������� �� ������� ��� ���� ����������, ����� �� ���������/���������/����� ��������� � ������ �������. 
				//������� � ������ ������� ������ ���� ���������� � ���������� ����������
				else if (pointer->type == varbl)
					return processing(last_func->point_right, last_func);
			}

			wstring expression_processing(var_const *pointer, int* comma)
			{

				if ((pointer->type == mltpl) || (pointer->type == divis))
				{
					return expression_processing(pointer->point_left, comma) + pointer->name + expression_processing(pointer->point_right, comma);
				}
				else if (pointer->type == addit)
				{
					if (pointer->point_right->name == L"minus")
					{
						if ((pointer->point_collar->type == mltpl) || (pointer->point_collar->type == divis))
							return L"(" + expression_processing(pointer->point_left, comma) + L" - " + expression_processing(pointer->point_right, comma) + L")";
						else
							return expression_processing(pointer->point_left, comma) + L" - " + expression_processing(pointer->point_right, comma);
					}
					else if ((pointer->point_collar->type == mltpl) || (pointer->point_collar->type == divis))
						return L"(" + expression_processing(pointer->point_left, comma) + L" " + pointer->name + L" " + expression_processing(pointer->point_right, comma) + L")";
					else
						return expression_processing(pointer->point_left, comma) + L" " + pointer->name + L" " + expression_processing(pointer->point_right, comma);
				}
				else if (pointer->type == numbr)
				{
					return to_string(pointer->var, var_type::FRACTIONAL, *comma);
				}
				else if (pointer->type == funct)
				{
					if (pointer->name == L"minus")
					{
						return L"-" + to_string(pointer->point_right->var, var_type::FRACTIONAL, *comma);
					}
					else
					{
						return  expression_processing(pointer->point_left, comma);
					}
				}
				else
				{
					return pointer->name;
				}
			}

		public:
			var_const()
			{
				name = L"";
				type = L"";
				prop = L"";
				actn = L"";
				var = 0;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(wstring _name, double _num)
			{
				name = _name;
				type = L"";
				prop = L"";
				actn = L"";
				exp = 0;
				var = _num;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num)
			{
				name = _name;
				type = _type;
				prop = _prop;
				var = _num;
				actn = _actn;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			var_const(wstring _name, double _num, var_const *_pc)
			{
				name = _name;
				type = L"";
				prop = L"";
				actn = L"";
				var = _num;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = _pc;
			}

			var_const(wstring _name, wstring _type, wstring _prop,wstring _actn, double _num, var_const *_pc)
			{
				name = _name;				
				type = _type;
				prop = _prop;
				actn = _actn;
				var = _num;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = _pc;
			}

			var_const(wstring _name, double _num, var_const * _pl, var_const *_pr) {
				name = _name;
				var = _num;
				type = L"";
				prop = L"";
				actn = L"";
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = NULL;
			}
			var_const(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, var_const * _pl, var_const *_pr) {
				name = _name;				
				type = _type;
				prop = _prop;
				actn = _actn;
				var = _num;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = NULL;
			}

			var_const(wstring _name, double _num, var_const * _pl, var_const *_pr, var_const *_pc) {
				name = _name;
				type = L"";
				prop = L"";
				actn = L"";
				var = _num;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}

			var_const(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, var_const * _pl, var_const *_pr, var_const *_pc) {
				name = _name;
				type = _type;
				prop = _prop;
				actn = _actn;
				var = _num;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}

			var_const(var_const* var1)
			{
				name = var1->name;
				type = var1->type;
				prop = var1->prop;
				actn = var1->actn;
				var = var1->var;
				exp = var1->exp;
				point_left = var1->point_left;
				point_right = var1->point_right;
				point_collar = var1->point_collar;
			}

			~var_const()
			{

			}

			void copy(var_const* ref) {
				name = ref->name;
				type = ref->type;
				prop = ref->prop;
				actn = ref->actn;
				var = ref->var;
				exp = ref->exp;
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
				wchar_t operation = name[0];
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
			
			wstring read(wstring arg)
			{
				wchar_t* id_a = wcsstr(&name[0], L"@");
				wchar_t* id_H = wcsstr(&name[0], L"#");
				wchar_t* id_f = wcsstr(&name[0], L"(");
				wchar_t* id_b = wcsstr(&name[0], L")");
				wstring out;

				if (arg == L"type")
				{
					if (id_a != NULL)
					{
						out.assign(name, 0, 5);
					}
					else
					{
						if (name == L"0")
						{
							out = L"numbr";
						}
						else if (name == L"+")
						{
							out = L"oper+";
						}
						else if (name == L"*")
							out = L"oper*";
						else if (name == L"/")
							out = L"oper/";
						else if (name == L"^")
							out = L"oper^";
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
								out.assign(id_a + 1, id_f);
						}
						else
						{
							if (id_a + 1 == id_H)
							{
								out = L"";
							}
							else
								out.assign(id_a + 1, id_H);	//TODO: ���)) ��� � �����) �������� ������ ������� 0xFFFF FFFF FFFF FFFF
						}
					}
					else
					{
						out = name;
					}
				}
				else if (arg == L"func")
				{
					if (id_a != NULL)
						if (id_H != NULL)
							if (id_a + 1 == id_H)
								out = L"";
							else
								out.assign(id_a + 1, id_H);
						else
							out.assign(id_a + 1);
					else
						out = name;
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
						
				}
				else
				{
					out = L"fuck up";
				}
				return out;
			}*/

			/*����� �������� ����������� �������, ���������� �� ������ �������� � ������������� ������ � ���������� ������� �������� ���������.
			���������� ������. ���� �� ��������*/
			wstring expresion(int comma)
			{
				return name + L" = " + expression_processing(point_left, &comma);
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
				wchar_t* id_a = wcsstr(&name[0], L"@");
				wstring type;
				type.assign(name, 0, 5);
				if (type == L"funct")
				{
					var = processing(point_left, point_collar->point_collar);
				}
				else
					var = processing(point_left, NULL);
			}

			int tree_destruct()
			{
				int s;
				if (point_left != NULL)
				{
					s += tree_destruct_processing(point_left);
					point_left = NULL;
				}
				if (point_right != NULL)
				{
					s += tree_destruct_processing(point_right);
					point_right = NULL;
				}
				if (point_collar != NULL) {
					delete point_collar;
					point_collar = NULL;
				}
				return s;
			}

			enum class variable_type {
				INTEGER,
				FRACTIONAL,
				IMAGINARY,
				REAL,
				RE_IM,
				EXP	//5E+10 5*10^10
			};

			wstring name;
			wstring type;
			wstring prop;
			wstring actn;
			double var;
			double arg; //����� ����������.
			int exp;
			var_const *point_left;		//����� �����
			var_const *point_right;		//������ �����
			var_const *point_collar;	//��������
		};
		

		vector<var_const*>* general_var_const;

		var_const pi = var_const(L"pi", cnst, fundm,L"0", 3.1415926535897932384626433832);
		var_const e = var_const(L"e", cnst, fundm, L"0", 2.7182818284590452353602874713527);
		var_const i = var_const(L"i", cnst, fundm, L"0", 0);
		//��������� ���������� ��� ���������� "������" �������, ������ ����� ���, ��� ���������� � ������
		var_const temporary_variable = var_const(L"temporary_variable", varbl, L"servc", L"0", 0);

		
		

		

		//������������� ������� ��������, ����������, ������� � ���������
		void Init() {
			ProjectError::Init();
			var_const* temp;
			general_var_const = new vector<var_const*>;
			general_var_const->push_back(&pi);
			general_var_const->push_back(&e);
			general_var_const->push_back(&i);
			//�����
			{
				general_var_const->push_back(new var_const(L"minus", funct, empty,L"", 0, new var_const(&temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
				temp->point_left = new var_const(L"", mltpl, L"", L"", 0, new var_const(L"0", -1), temp->point_collar, temp);
			}
			
			//������ (���� ����������)
			{
				general_var_const->push_back(new var_const(L"root", funct, empty, L"", 0, new var_const(&temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
			}
			//������ (������ �� �������� �����)
			{
				general_var_const->push_back(new var_const(L"abs", funct, empty, L"", 0, new var_const(&temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new var_const(temp);
				general_var_const->back()->point_left->point_collar->point_collar = general_var_const->back()->point_left;
				general_var_const->back()->point_left->point_right = new var_const(L"", power, L"", L"", 0,general_var_const->back()->point_collar,new var_const(L"0",2),NULL);
			}
			//����
			{
				temp = general_var_const->back();
				general_var_const->push_back(new var_const(L"sgn", funct, empty, L"", 0, new var_const(&temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new var_const(L"", divis, L"", L"", 0, general_var_const->back()->point_collar, new var_const(temp), general_var_const->back());
				general_var_const->back()->point_left->point_right->point_right = general_var_const->back()->point_collar;
				general_var_const->back()->point_left->point_right->point_collar = general_var_const->back()->point_collar;
			}

		}

		/*void testfunc(mutex &mut) {
			mut.lock();
			MessageBoxA(0,"testfunc","tThread",0);
		}*/

		//������� �������� �� ������� general_var_const � ���� ������ � ������ ������
		var_const* run_vector(wstring _pv)
		{
			size_t temp_size_of_vect = general_var_const->size();
			int count;
			for (count = 0; count < temp_size_of_vect; count++)
			{
				try {
					int iPv = _pv.length();
					wstring wstr = general_var_const->at(count)->name;	//��� �����
					int iVect = wstr.length();
					if (iPv == iVect)
						if (_pv.compare(general_var_const->at(count)->name) == 0)
							return general_var_const->at(count);
				}
				catch (exception e) {	//������ string too long �����

				}
			}
			if (count == temp_size_of_vect)
			{
				return NULL;
			}
		}


		var_const* filling_vector(wchar_t* strPtr, wchar_t*ePtr, var_const* c_e, int brakets)
		{			
			wchar_t* pDest = strPtr;
			wchar_t* endPtr = ePtr;
			unsigned int count,comma;
			var_const *high_pointer = NULL; //��������� ���������� ��������
			var_const *low_pointer = NULL; //��������� ���������� ���������/����������/�-���/���������
			var_const *temp_pointer = NULL;
			var_const *multiple_var = NULL;
			wchar_t* temp, *p_temp;
			wstring name;
			var_const* current_element = c_e;
			double num;
			int brakets_counter = 0;
			while (pDest <= endPtr)
			{

				if (*pDest == '=')
				{
					//���� ����� ����� � ����� ������ ������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::EQUALY_FIRST);
						return NULL;
					}
					//����� � ����� ������, ������ �� ��������
					else if (pDest == endPtr)
					{
						//������ ����� ������ ������� ����������� ������� � ���������� solve, � ��� ��� �������� ��� ��������� � ������� ��������� ��� �������� ������
						current_element->actn= solve;
					}
					else
					{
						temp_pointer = filling_vector(pDest + 1, endPtr, new var_const(L"",exprs,undef,L"", 0, low_pointer), brakets + brakets_counter);
						if (temp_pointer = NULL)
						{
							return temp_pointer;
						}
						if (current_element->type == cnst)
						{
							if (temp_pointer->type == funct)
							{
								current_element->point_left = temp_pointer->point_left;
								//����� ���������� � ���������� �� �������
								current_element->point_collar = temp_pointer->point_collar;
								current_element->point_collar->point_collar = current_element;
								current_element->point_right = temp_pointer->point_right;								
								current_element->type = funct;
								current_element->prop =  defnd;
							}
							else if (temp_pointer->type == varbl)
							{
								if (temp_pointer->prop == undef)
								{
									temp_pointer->prop =  defnd;
									general_var_const->push_back(temp_pointer);
								}
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(temp_pointer);
								current_element->point_collar->point_collar = current_element;
								current_element->point_left = current_element->point_collar;								
								current_element->type = funct;
								current_element->prop = defnd;
							}
							else if (temp_pointer->type == exprs)
							{
								current_element->point_left = temp_pointer->point_left;
								current_element->prop = defnd;
							}
							else if (temp_pointer->type == cnst)
							{
								current_element->var = temp_pointer->var;
								current_element->type = defnd;
							}
						}
						else if (current_element->type == varbl)
						{
							if (temp_pointer->type == funct)
							{
								current_element->point_left = temp_pointer->point_left;
								//����� ���������� � ���������� �� �������
								current_element->point_collar = temp_pointer->point_collar;
								current_element->point_collar->point_collar = current_element;
								current_element->point_right = temp_pointer->point_right;
								
								current_element->type = funct;
								current_element->prop = defnd;
							}
							else if (temp_pointer->type == varbl)
							{
								if (temp_pointer->prop == undef)
								{
									temp_pointer->prop = defnd;
									general_var_const->push_back(temp_pointer);
								}
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(temp_pointer);
								current_element->point_collar->point_collar = current_element;
								current_element->point_left = current_element->point_collar;
								
								current_element->type = funct;
								current_element->prop = defnd;
							}
							else if (temp_pointer->type == exprs)
							{
								current_element->point_left = temp_pointer->point_left;
								current_element->type = cnst;
								current_element->prop = defnd;
							}
							else if (temp_pointer->type == cnst)
							{
								current_element->var = temp_pointer->var;
								current_element->type = cnst;
								current_element->prop = defnd;
							}
						}
						else if (current_element->type == funct)
						{
							if (temp_pointer->type == funct)
							{
								if (current_element->point_collar->name == temp_pointer->point_collar->name)
								{
									if (current_element->prop == defnd)
									{
										//���� �������� ������� ����� � �����-�� ������� ������ - ��������������� ��� ��� �����
										current_element->point_left = temp_pointer->point_left;
									}
									else
									{
										//������ �������� �-��� ����� - ���������
										current_element->type= equat;
										current_element->point_right = temp_pointer->point_left;
										current_element->prop = unslv;
									}
								}
								else
								{
									//��� �������� �������/��������� ���������� ����������									
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
									return NULL;
								}
							}
							else if (temp_pointer->type == varbl)
							{
								if (current_element->point_collar->name == temp_pointer->name)
								{
									if (current_element->prop == defnd)
									{
										//���� �������� ������� ����� � ���������� ���� ������� ������ - ��������������� ��� ��� �����
										current_element->point_left = temp_pointer;
									}
									else
									{
										//������ �������� �-��� ����� - ���������
										current_element->type = equat;
										current_element->point_right = temp_pointer;
										current_element->prop = unslv;
									}
								}
								else
								{
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(temp_pointer);
									}
									//����� ���������� � ���������� �� �������
									current_element->point_collar = new var_const(temp_pointer);
									current_element->point_collar->point_collar = current_element;

									current_element->point_left = current_element->point_collar;
									
									current_element->type = funct;
									current_element->prop = defnd;
								}
							}
							else if (temp_pointer->type == exprs)
							{
								//��������� ���� f(x) = 2
								if (current_element->prop == defnd)
								{
									high_pointer = current_element;
									current_element = new var_const(L"",equat,unslv,L"", 0, high_pointer, temp_pointer->point_left, new var_const(high_pointer->point_collar));
									current_element->point_collar->point_collar = current_element;
								}
								else
								{

									current_element->type = equat;
									current_element->point_right = temp_pointer->point_right;
									current_element->prop = unslv;
								}
							}
							else if (temp_pointer->type == cnst)
							{
								//��������� ���� f(x) = const
								if (current_element->prop == defnd)
								{
									high_pointer = current_element;
									current_element = new var_const(L"",equat,unslv,L"", 0, high_pointer, temp_pointer, new var_const(high_pointer->point_collar));
									current_element->point_collar->point_collar = current_element;
								}
								else
								{

									current_element->type = equat;
									current_element->point_right = temp_pointer;
									current_element->prop = unslv;
								}
							}
						}
						else if (current_element->type == exprs)
						{
							//����� - ����� ��������� - ������ ������� => ���������
							if (temp_pointer->type == funct)
							{
								current_element->type = equat;
								current_element->point_right = temp_pointer;
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(temp_pointer->point_collar);
								current_element->point_collar->point_collar = current_element;
								current_element->prop = unslv;								
							}
							else if (temp_pointer->type == varbl)
							{
								current_element->type = equat;
								current_element->point_right = temp_pointer;
								//��� ��� ������ �������� ����� ����������
								current_element->point_collar = temp_pointer;
								current_element->point_collar->point_collar = current_element;
								current_element->prop = unslv;								
							}
							else if ((temp_pointer->type == cnst) || (temp_pointer->type == exprs))
							{
								//������ ���� 2+3=7. ���� �� ���� ��� ����������� �� �����
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
								return NULL;
							}
						}

					}
					pDest = endPtr + 1;
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
						current_element->point_left = new var_const(L"+",addit,L"",L"", brakets_counter + brakets, low_pointer, NULL, current_element);
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_element->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = new var_const(L"+", addit, L"", L"", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new var_const(L"+", addit, L"", L"", brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 1);
								high_pointer->point_right->point_collar = new var_const(L"+", addit, L"", L"", brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
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
						current_element->point_left = new var_const(L"*", mltpl, L"", L"", brakets_counter + brakets, low_pointer, NULL, current_element);
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_element->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 2))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = new var_const(L"*", mltpl, L"", L"", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 2))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new var_const(L"*", mltpl, L"", L"", brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 2);
								high_pointer->point_right->point_collar = new var_const(L"*", mltpl, L"", L"", brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
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
						current_element->point_left = new var_const(L"/", divis, L"", L"", brakets_counter + brakets, low_pointer, NULL, current_element);
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_element->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 3))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = new var_const(L"/", divis, L"", L"", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 3))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new var_const(L"/", divis, L"", L"", brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 3);
								high_pointer->point_right->point_collar = new var_const(L"/", divis, L"", L"", brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
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
						current_element->point_left = new var_const(L"^", power, L"", L"", brakets_counter + brakets, low_pointer, NULL, current_element);
						//����� ����� ������� ����������� ��������� ��������� �� ��������� ��������
						high_pointer = current_element->point_left;
					}
					//���� ���� �����-���� �������� �� �����
					else
					{
						//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 4))
						{
							//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
							high_pointer->point_right = new var_const(L"^", power, L"", L"", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 4))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new var_const(L"^", power, L"", L"", brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 4);
								high_pointer->point_right->point_collar = new var_const(L"^", power, L"", L"", brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
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
				else if (*pDest == '-')
				{
					//�������� ����� ���������� ��� �������� � ������������� ������					
					//���� ��������� ���������� � ������ (������ ����� ����� ������ ��� �����, ����� ����� ������� �� ����������)
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//�������� �������� ������ � ������ �����, �������� -> ���������
						current_element->point_left = new var_const(general_var_const->at(2));
						current_element->point_left->type = defnd;

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
							current_element->point_left = new var_const(L"+", addit, L"", L"",  brakets_counter, low_pointer, NULL, current_element);
							high_pointer = current_element->point_left;
							high_pointer->point_right = new var_const(general_var_const->at(3));

							high_pointer->point_right->type = defnd;

							low_pointer = high_pointer->point_right;
						}
						//���� ����� ���� ������ ��������
						else
						{
							//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
							if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
							{
								//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
								high_pointer->point_right = new var_const(L"+", addit, L"", L"",  brakets_counter, low_pointer, NULL, high_pointer);
								high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
								high_pointer->point_right = new var_const(general_var_const->at(2));
								high_pointer->point_right->type = defnd;
								low_pointer = high_pointer->point_right;
							}

							else
							{
								if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
								{
									//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
									high_pointer = new var_const(L"+", addit, L"", L"",  brakets_counter, current_element->point_left, NULL, current_element);
									//�������� ���������� ��������� �������� -> �� ����� ��������
									current_element->point_left->point_collar = high_pointer;
									//��������� ����� ������� ��������� �� ��������� ��������
									current_element->point_left = high_pointer;
									high_pointer->point_right = new var_const(general_var_const->at(2));
									high_pointer->point_right->type = defnd;
									low_pointer = high_pointer->point_right;
								}
								else
								{
									high_pointer = high_pointer->prioritize(brakets_counter + brakets + 1);
									high_pointer->point_right->point_collar = new var_const(L"+", addit, L"", L"",  brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
									high_pointer->point_right = high_pointer->point_right->point_collar;
									high_pointer = high_pointer->point_right;
									high_pointer->point_right = new var_const(general_var_const->at(2));
									high_pointer->point_right->type = defnd;
									low_pointer = high_pointer->point_right;
								}

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
						if (temp == endPtr + 1)
						{							
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
						name.assign(pDest + 1, temp);
						temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"",exprs,undef,L"", 0, low_pointer), brakets + brakets_counter);
						if (temp_pointer == NULL)
						{
							return temp_pointer;
						}
						//������ � ����� ������ ������
						if ((high_pointer == NULL) && (low_pointer == NULL))
						{
							if (temp_pointer->type == cnst)
							{
								current_element->point_left = temp_pointer;
								high_pointer = temp_pointer;
								low_pointer = high_pointer;
							}
							else if (temp_pointer->type == exprs)
							{
								current_element->point_left = temp_pointer->point_left;
								high_pointer = temp_pointer;
								low_pointer = high_pointer;
								delete temp_pointer;
								temp_pointer = NULL;
							}
							else if (temp_pointer->type == varbl)
							{
								current_element->type = funct;								
								if (temp_pointer->prop == undef)
								{
									temp_pointer->type = defnd;
									general_var_const->push_back(new var_const(temp_pointer));
								}
								current_element->point_collar = temp_pointer;
								current_element->point_left = temp_pointer;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else if (temp_pointer->type == funct)
							{
								current_element->copy(temp_pointer);								
								current_element->point_collar->point_collar = current_element;								
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
								delete temp_pointer;
								temp_pointer = NULL;
							}
							//���-�� ������ ����������� ����� �������� ������, �������� ���� ��������� ��� ������ ����� ��
						}

						// ������� ������ ���-�� ����� - ����� ��� �������������� - ����� ������
						else if (low_pointer == high_pointer)
						{
							//����� ������� ����� �����
							if (high_pointer->name == L"minus")
							{
								if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
								{
									high_pointer->point_right = temp_pointer;
								}
								else if (temp_pointer->type == varbl)
								{
									current_element->type = funct;									
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(new var_const(temp_pointer));
									}
									current_element->point_collar = temp_pointer;
									high_pointer->point_right = temp_pointer;
								}
								else if (temp_pointer->type == funct)
								{
									current_element->type = funct;									
									current_element->point_collar = new var_const(temp_pointer->point_collar);
									high_pointer->point_right = temp_pointer;
								}								
							}
							//����� ������� ����� �������������� - ��������� � ����������� �������
							else
							{
								if (current_element->prop == defnd)
								{
									//????
									if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
									{
										high_pointer = new var_const(current_element);
										low_pointer = new var_const(L"", exprs, undef, L"", 0, high_pointer, NULL, NULL);
										current_element->copy(low_pointer);
										delete low_pointer;
										low_pointer = NULL;
										low_pointer = high_pointer;
										high_pointer->point_right = temp_pointer;
									}
									else if (temp_pointer->type == varbl)
									{
										//����� �������� ������ ����� �����
										if (*(temp + 1) == L'=')
										{
											if (current_element->point_collar->name != temp_pointer->name)
											{
												if (temp_pointer->prop == undef)
												{
													temp_pointer->prop= defnd;
													general_var_const->push_back(new var_const(temp_pointer));
												}
												//���������� ����� ���������� 
												current_element->point_collar = temp_pointer;												
												current_element->type = funct;
												current_element->prop = undef;
											}
											else
											{
												//������ �� ������!
											}
										}
										else
										{											
											high_pointer = new var_const(current_element);
											low_pointer = new var_const(L"", funct, undef, L"", 0, high_pointer, NULL, temp_pointer);
											current_element->copy (low_pointer);
											delete low_pointer;
											low_pointer = NULL;
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new var_const(temp_pointer));
											}											
											high_pointer->name = name;
											low_pointer = high_pointer;
										}
									}
									//��� ���� ��� � �������. ��������� ������� ���� ��������
									else if (temp_pointer->type == funct)
									{
										current_element->type = funct;										
										current_element->point_collar = temp_pointer->point_collar;
										current_element->point_left = temp_pointer;
										high_pointer = current_element->point_left;
										low_pointer = high_pointer;
									}


								}


								//�����-�� ����� �������
								else if (current_element->prop == undef)
								{
									if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
									{
										//������� ��������� ������������� ����� ������� �� �����/���������. 
										//�������� ��� ��������� ������� ��������� �������� � � ���� ������ �������������� � ��� ����������. ������� �������� � ��������
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									else if (temp_pointer->type == varbl)
									{
										//����� �������� ������ ����� �����
										if (*(temp + 1) == L'=')
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new var_const(temp_pointer));
											}
											//���������� ����� ���������� 
											current_element->point_collar = temp_pointer; //��� ��������� ������� �� ����� � ������ ������������ ���������� �� �������											
											current_element->prop = defnd;
										}
										else
										{
											//���� ������. ������ ��� ������� ����������� ������� ����� �����-�� ���������. 
											//� ����� ������ �������� �������� ����������� ���������� ����������� ������� ��������� ���������� ���������� ������������ �����-�� �����.
											//� ����� ������ ������� �������������� �������.
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
									}
									//��� ���� ��� � �������. ��������� ������� ���� ��������
									else if (temp_pointer->type == funct)
									{
										//������������� ������� � ����������� � �������� ��������� - �� ���� ��� ��� ������������� � ��� ��� ����� ����������
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
								}
							}

						}
						//���� ������� ����� ������� �������� �� ����� ������� ��������
						else if (high_pointer->point_right == NULL)
						{
							if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
							{
								high_pointer->point_right = temp_pointer;
								low_pointer = high_pointer->point_right;
							}
							else if (temp_pointer->type == varbl)
							{
								if (current_element->type == funct)
								{
									if (current_element->point_collar->name == temp_pointer->name)
									{
										high_pointer->point_right = temp_pointer;
										low_pointer = high_pointer->point_right;
									}
									else
									{
										//��� �������� �������/��������� ���������� ����������
										
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
										return NULL;
									}
								}
								else if (current_element->type == exprs)
								{
									current_element->type = funct;
									
									if (temp_pointer->prop ==undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(new var_const(temp_pointer));
									}
									current_element->point_collar = temp_pointer;
									high_pointer->point_right = current_element->point_collar;
									low_pointer = high_pointer->point_right;;
								}
							}
							else if (temp_pointer->type == funct)
							{
								if (current_element->type == funct)
								{
									if (current_element->point_collar->name == temp_pointer->point_collar->name)
									{
										high_pointer->point_right = temp_pointer;
										low_pointer = high_pointer->point_right;
									}
									else
									{
										//��� �������� �������/��������� ���������� ����������
										
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
										return NULL;
									}
								}
								else if (current_element->type == exprs)
								{
									current_element->type = funct;									
									current_element->point_collar = temp_pointer->point_collar;
									high_pointer->point_left = temp_pointer;
									low_pointer = high_pointer->point_left;
								}
							}
						}
						//���� ���-�� ���
						else
						{
							if (low_pointer->type == funct)
							{
								if (low_pointer->name == L"minus")
								{
									if (low_pointer->point_right == NULL)
									{
										//��� ��������
										low_pointer->point_right = temp_pointer;
									}
									else
									{
										//��� ������ ���� - ����� ����� �������� ��� ����� ��� ���������� � ������ ������
										
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::NEGATIVE_FUNC);
										return NULL;
									}
								}
								else
								{
									if (temp_pointer->type == exprs)
									{
										low_pointer->point_right = temp_pointer->point_left;
									}
									else if (temp_pointer->type == funct)
									{
										if (current_element->point_collar == NULL)
										{
											low_pointer->point_right = temp_pointer;
											low_pointer->point_collar = temp_pointer->point_collar;
											current_element->point_collar = temp_pointer->point_collar;											
										}
										else
										{
											if (temp_pointer->point_collar->name == current_element->point_collar->name)
											{
												low_pointer->point_right = temp_pointer;
											}
											else
											{
												//��� �������� �������/��������� ���������� ����������												
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
												return NULL;
											}
										}
									}
									else if (temp_pointer->type == cnst)
									{
										low_pointer->point_right = temp_pointer;
									}
									else if (temp_pointer->type == varbl)
									{
										if (current_element->point_collar == NULL)
										{
											low_pointer->point_collar = temp_pointer;
											current_element->point_collar = temp_pointer;											
										}
										else
										{
											if (temp_pointer->name != current_element->point_collar->name)
											{
												//��� �������� �������/��������� ���������� ����������												
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
												return NULL;
											}
										}
									}
								}
							}
							else if (low_pointer->type == varbl)
							{

							}
							else if (low_pointer->type == cnst)
							{

							}
							else if (low_pointer->type == numbr)
							{
								//�������� ���� ������ ����� ��������� � ������� ��������� �����
							}
							else if ((low_pointer->type == mltpl)|| (low_pointer->type == addit)|| (low_pointer->type == divis)|| (low_pointer->type == power))
							{								
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
				else if (*pDest == '|')
				{
					//�� ��� �� ������. ��������� �������� - � ������ �� �������� ����������� ( � ����������� ) - ������ �������. 
					//� ������ � ������� | - ��� ������� - ���� � ��� �� ������. ������ �������� ����� ����� abs().
				}
				else if ((*pDest == '1') || (*pDest == '2') || (*pDest == '3') || (*pDest == '4') || (*pDest == '5') || (*pDest == '6') || (*pDest == '7') || (*pDest == '8') || (*pDest == '9') || (*pDest == '0'))
				{                   //����� ����� ����� id "0"					
					// ���� ������ ����� ������ � ������ ���������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//�������� �������� ������ � ������ �����, �������� -> ���������
						current_element->point_left = new var_const(L"", numbr,real,L"", wcstod(pDest, &pDest), current_element);
						//��� ��������� -> �� �����, �������� ��� �������� ������� ��� ������ ��������
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					// ���� ������ ����� �� ������ � ������ (���� �����-���� ��������)
					else
					{
						//�������� �������� ������ � ������ �����, �������� -> ���� �������� 						
						if ((high_pointer->point_right != NULL) && (low_pointer->name == L"minus"))
						{
							low_pointer->point_right = new var_const(L"", numbr, real, L"", wcstod(pDest, &pDest), low_pointer);
						}
						else
						{
							high_pointer->point_right = new var_const(L"", numbr, real, L"", wcstod(pDest, &pDest), high_pointer);
							low_pointer = high_pointer->point_right;
						}
					}
				}
				else
				{					
					temp = wcspbrk(pDest, L"()+-*^/=");
					//���� ������� ���������� � c������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						if ((temp == NULL) && (brakets == 0))
						{							
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::EQUALY_MISSING);
							return NULL;
						}
						else if (temp == NULL)
						{
							name.assign(pDest, endPtr + 1);
							pDest = endPtr + 1;
						}
						else
						{
							name.assign(pDest, temp);
							pDest = temp;
						}
						high_pointer = run_vector(name);
						//���� �� ������ �� ���� ������� ������� � ����� ������
						if (high_pointer == NULL)
						{
							//������� ��������� ������� ����������
							if (temp == NULL)
							{
								current_element->type = varbl;
								current_element->name = name;
								current_element->prop = undef;
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//������� ��������� ������� �����
							else if (*temp == '=')
							{
								current_element->type = cnst;
								current_element->name = name;
								current_element->prop = undef;
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//������� ��������� ������� ��������
							else if (*temp == '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
								p_temp = temp;
								while (count != 0)
								{
									temp++;
									//���� ������ � ����� ������� �� ����� ����������� ������
									if (temp == endPtr + 1)
									{
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
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										comma++;
									}
								}
								if (*(temp + 1) != '=')
								{
									//���� � ����� ������ ������ ����� ����� �������������� � ������ ������ - ��� ������ ����������� ����� �������
									//���� ����� ����� ��� ����� - ������
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								if (p_temp + 1 == temp)
								{
									//������ ������ � �������. ���������� �������� �������� ������� �� �������������� ���������� ����������
									//���������� ������� ������� ������ ��� �������� ����������.
								}																
								if (comma == 0)
								{
									name.assign(p_temp + 1, temp);
									temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
									if (temp_pointer == NULL)
										return temp_pointer;
									if ((temp_pointer->type == cnst)||(temp_pointer->type == exprs))
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									else if (temp_pointer->type == varbl)
									{
										if (temp_pointer->prop == undef)
										{
											temp_pointer->prop = defnd;
											general_var_const->push_back(new var_const(temp_pointer));											
										}
										current_element->type = funct;
										current_element->name = name;
										current_element->prop = undef;
										current_element->point_collar = temp_pointer;
										high_pointer = current_element;
										low_pointer = high_pointer;
									}
									else if (temp_pointer->type == funct)
									{
										//������������� ������� �� ����������� - ������ ��� �� ������� �������, �� ��� ����� ������ ���������
									}
								}
								//���� � ������� ����� ������� - ����������� ������� ���������� ����������
								else
								{
									multiple_var = new var_const[comma + 1];
									for (count = 0; count < comma + 1; count++)
									{
										temp = wcspbrk(p_temp + 1, L",)");
										name.assign(p_temp + 1, temp);										
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
										if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new var_const(temp_pointer));
											}
											multiple_var[count].copy(temp_pointer);
											//delete[] temp_pointer;
											//temp_pointer = NULL;
										}
										else
										{
											//��������� ������� ��� ������������� ���������� �� ����� ��������� ����� �������
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										p_temp = temp;
									}									
									current_element->type = funct;
									current_element->name = name;
									current_element->prop = undef;
									current_element->point_collar = multiple_var;
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								brakets_counter -= 4;
								pDest = temp + 1;
							}
							else if (*temp == ')')
							{								
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//������� ��������� ������� ����������, � ������� ��������� - ��������
							else
							{
								current_element->type = funct;								
								current_element->prop = undef;								
								general_var_const->push_back(new var_const(name,varbl,defnd,L"", 0));
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(general_var_const->back());
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						//������ ������� ������� � ����������� ������ - ���������
						else if (high_pointer->type == cnst)
						{
							//�������� ���������
							if (temp == NULL)
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//������� ������� => ���������
							else if (*temp == '=')
							{								
									current_element = high_pointer;
									low_pointer = high_pointer;							
								
							}
							//��� ���� � ���, ��� ���� ��� ���� �����-�� ��������� - ����� �� �� ���� ��������� ��� �� - ������� ����� �������� ���������.
							//������ ������ ������ ����� �������� ��������������� ����� � ���� �������
							else if (*temp == '(')
							{
								current_element = high_pointer;
								current_element->point_collar = new var_const();
								low_pointer = high_pointer;
							}
							else if (*temp == ')')
							{								
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//������� ������� ������� ����������, ��������� ��������� (� �����) - ���� �� ���������
							else
							{
								current_element->point_left = new var_const(high_pointer);
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						//������ ������� ������� � ����������� ������ - ����������
						else if (high_pointer->type == varbl)
						{
							//�������� ����������
							if (temp == NULL)
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;

							}
							//������� ������� => ����������
							else if (*temp == '=')
							{
								current_element = high_pointer;
								low_pointer = high_pointer;								
							}
							else if (*temp == ')')
							{
								
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//����� ������ ����� �������� ������ ��������������� ���������� � �������� ������� ������ ���������
							else if (*temp == '(')
							{
								current_element = high_pointer;
								low_pointer = high_pointer;
								current_element->point_collar = new var_const();
							}
							//������� ������� => �������, ���������� �������� ����� ����������
							else
							{
								current_element->type = funct;
								current_element->prop = undef;
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(high_pointer);
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						//������ ������� ������� � ����������� ������ - �������
						else if (high_pointer->type == funct)
						{
							//�������� �������
							if (temp == NULL)
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;

							}
							//������ ������� ����� ���� ������������ � �����-�� ���������/���������/������ ������� � �������� ���������. 
							else if ((*temp == '=') || (*temp == '('))
							{
								current_element = high_pointer;
								low_pointer = high_pointer;
								
							}
							else if (*temp == ')')
							{								
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							/*else if (*temp == '(') //��� ���� ������
							{
								current_element->var_id = L"funct@";
								current_element->var_id += L"(" + high_pointer->read(L"nvar") + L")";
								current_element->var_id += L"#undef";
								current_element->point_left = new var_const(high_pointer);
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(high_pointer->point_collar);
								current_element->point_collar->point_collar = current_element;
								high_pointer = current_element->point_left;
								high_pointer->var_id.replace(high_pointer->var_id.find_first_of('#') + 1, 5, L"undef");
								//���� � ����� ������ ������ ������� ������������ �-��� ����� �������. ������ � undef - � ������� ���-�� ������
								low_pointer = high_pointer;
							}*/
							//������� ��������� => �������, ��������� ������� - ������� ���������.
							else
							{
								current_element->type = funct;								
								current_element->prop = undef;
								current_element->point_left = new var_const(high_pointer);
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(high_pointer->point_collar);
								current_element->point_collar->point_collar = current_element;
								//������ �������� ����� ������� ��� ����������
								high_pointer = current_element->point_left;
								high_pointer->type = defnd;
								low_pointer = high_pointer;
							}
						}
					}
					//������ �� � ������ ������
					else
					{
						if (temp == NULL)
						{
							name.assign(pDest, endPtr + 1);
							pDest = endPtr + 1;
						}
						else
						{
							name.assign(pDest, temp);
							pDest = temp;
						}
						low_pointer = run_vector(name);
						//����� ������ ����� ���� ������ ��� �������� - ������� ������� ���� �������, ���� ���������
						//���� �� ������ �� ���� ������� ������� � ����� ������
						if (low_pointer == NULL)
						{
							//���� ������� ������� - �������, �� �������� ������� ����� ���������� � ������ ���������. 
							if (current_element->type == funct)
							{
								//��� �������� �������/��������� ���������� ����������								
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
								return NULL;
							}							
							//���� ���������
							else if (current_element->type == exprs)
							{
								// ��������� � undef ������� 
								current_element->type = funct;
								current_element->prop = undef;							
								general_var_const->push_back(new var_const(name,varbl,defnd,L"", 0));
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(general_var_const->back());
								current_element->point_collar->point_collar = current_element;
								//��� �������� ����� ���������� - � �� ����� �����))
								if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
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
						//������ ������� ������� � ����������� ������ - ����������
						else if (low_pointer->type == varbl)
						{
							if (current_element->type == funct)
								//�� ���������� ��� ����������
								if (low_pointer->name != current_element->point_collar->name)
								{
									//��� �������� �������/��������� ���������� ����������									
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
									return NULL;
								}
								// ��� ���������� ������ ���������� ���������� � ���������
								else
								{
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = current_element->point_collar;

									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}

							else if (current_element->type == exprs)
							{
								current_element->type = funct;
								current_element->prop = undef;								
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(low_pointer);
								current_element->point_collar->point_collar = current_element;
								if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
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
						//������ ������� ������� � ����������� ������ - ��������� - ������ ���������� � � ���������
						else if (low_pointer->type == cnst)
						{
							if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
							{
								high_pointer->point_right->point_right = new var_const(low_pointer);
							}
							else
							{
								high_pointer->point_right = new var_const(low_pointer);
							}
							low_pointer = high_pointer->point_right;
						}
						//������ ������� ������� � ����������� ������ - �������
						else if (low_pointer->type == funct)
						{
							if (current_element->type == funct)
							{
								//��� ���������� ��������� ���������� ����������. ���� �� ������ �������� f(x) � ����� ���������� � � ��������� ��� ������� ��������� �� y, 
								//������ ��� ���������, �� ��������� ������� ���� ����������. �� �������� ������ �������� ����� ������� ������ � �������� ���������
								if (low_pointer->point_collar->name == current_element->point_collar->name)
								{
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
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
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
									return NULL;
								}
							}
							else if (current_element->type == exprs)
							{
								if (*temp == '(')
								{
									
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = new var_const(low_pointer);
										high_pointer->point_right->point_right->prop = defnd; //��� � ��� defnd �� ���������. �������� ���� ���-�� ������ ������
									}
									else
									{
										high_pointer->point_right = new var_const(low_pointer);
										high_pointer->point_right->prop =defnd;
									}
									low_pointer = high_pointer->point_right;
								}
								else
								{
									//��� ����� ��� �������� �� ������� �������. ��� ��� ������ ���� ���� ��������� ����������.
									current_element->type = funct;
									current_element->prop = undef;									
									//����� ���������� � ���������� �� �������
									current_element->point_collar = new var_const(low_pointer->point_collar);
									current_element->point_collar->point_collar = current_element;
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = new var_const(low_pointer);
										high_pointer->point_right->point_right->prop = defnd;
									}
									else
									{
										high_pointer->point_right = new var_const(low_pointer);
										high_pointer->point_right->prop = defnd;
									}
									low_pointer = high_pointer->point_right;
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
			if ((general_var_const->back()->type == exprs) || (general_var_const->back()->point_left == NULL))
				general_var_const->pop_back();
			wstring output;
			size_t output_size;
			//if (_current_element->read(L"type") == L"error")
			if (CE == NULL)
			{
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				return err->GetErrorWStr();
			}
			else if (CE->type == exprs)
			{
				CE->arithmetic();
				output = to_string(CE->var, var_type::FRACTIONAL, 2);
				CE->tree_destruct();
				delete CE;
			}
			else if (CE->type == equat)
			{
				// �� ������ ��� ��� �� ������ �� ����. ������������ ���� ��������� � ��� ������ ����������. ����� �������� ������ ������������ ������ ���������� ����� �� ����
				//������� ������ ��� ��������� � ������ �����������, ����� ����� ������������ �������� ����� - ���������� �������� ������ ������� - ��� ����.
				//mutex* mut=new mutex();
				//thread tTest(testfunc);	//����� ����������� � �����
				//thread* ptTest;	//��� ���������� ��������� ������, ����� ��������� ������� �� ���� ���������
				//ptTest = new thread(testfunc);	//����������������, � �� ����� �����������
				//mut->unlock();
			}
			else if (CE->type == funct)
			{
				if (CE->actn == solve)//��� ��� �������
				{
					if (CE->point_right == NULL)
					{
						output = CE->expresion(1);						
					}
					else
					{
						CE->arithmetic();
					}
				}
				else if (CE->prop == undef)
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
					ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
					ProjectError::GetProjectLastError(err);
					return err->GetErrorWStr();
				}
				else if (CE->prop == defnd)
				{
					//����� ��, ������� ������� �� ��������� ���� �� ������. ������ ������� - ��������. �������� � ��������� ����� ������ ���������� ������� ������� 
					//�� ������������. ����� � �� � ���������, ������ ��� ����� ������� ��������� ����������
				}
			}
			else if (CE->type == cnst)
			{
				if (CE->actn == solve)
				{
					//����� 
					output = to_string(CE->var, var_type::FRACTIONAL, 2);					

				}
				else if (CE->prop == undef)
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
					ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
					ProjectError::GetProjectLastError(err);
					return err->GetErrorWStr();
				}
				else if (CE->prop ==defnd)
				{
					//��������� - ���������
					CE->arithmetic();
					CE->tree_destruct();
				}
			}
			else if (CE->type == varbl)
			{
				CE->type = defnd;
				//���� �� ��������� ������� �� ������� ��������� (������ ����� - ���������� �����������)
				ProjectError::SetProjectLastError(ProjectError::ErrorCode::IS_EQUATION);
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				return err->GetErrorWStr();
			}
			else
			{
				//else else esle!!!
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

			
			general_var_const->push_back(new var_const(L"",exprs,undef,write, 0));

			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer	
			return analized_output(point_start, point_end, general_var_const->at(size_of_vect));
		}
	}
}
