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
#include "filters.h"

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
		wstring defnd = L"defnd";//����� ��������
		wstring undef = L"undef";//����� ��������

		wstring arg_c = L"arg_c"; //������� � ������������ �����������
		wstring arg_v = L"arg_v"; //������� � ����������� ����������� 
		wstring empty = L"empty"; //"������" �������. �������� ������� �������������������� �������

		wstring fundm = L"fundm";//��������������� ���������

		wstring unslv = L"unslv";//�������� ��������� � ������� �� ����� ���������
		wstring solvd = L"solvd";//�������� ��������� � ������� �� ����� ���������

		wstring real = L"real_";//�������� �����
		wstring cmplx = L"cmplx";//�������� �����

		wstring servc = L"servc";//�������� ��������� ��������
		//�������� ��� ��������� (actn)
		wstring solve = L"solve";
		wstring write = L"write";
		wstring read = L"read_";
		//wstring arg = L"argum";

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

			var_const *var_list_back_processing(var_const *pointer)
			{
				if ((pointer->point_left == NULL) || (pointer->point_left->var == 0))
					return pointer;
				else
					return var_list_back_processing(pointer->point_left);
			}

			var_const *find_varbl_processing(var_const*pointer, var_const *original)
			{
				if (original->name.compare(pointer->name) == 0)
					return pointer;
				else if ((pointer->point_left != NULL) && (pointer->point_left->var != 0))
					return find_varbl_processing(pointer->point_left, original);
				else
					return NULL;
			}

		public:
			//������� �����������
			var_const()
			{
				name = L"";
				type = L"";
				prop = L"";
				actn = L"";
				var = 0;
				arg = 0;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			//����������� ���������� (��� �������, ����� ���� double) 
			var_const(wstring _name, double _num)
			{
				name = _name;
				type = L"";
				prop = L"";
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			//����������� ���������� (��� �������, ��� �������, ����� ���� double) 
			var_const(wstring _name, wstring _type, double _num)
			{
				name = _name;
				type = _type;
				prop = L"";
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}


			//����������� ���������� (��� �������, ��� �������, �������� �������, ����� ���� double) 
			var_const(wstring _name, wstring _type, wstring _prop, double _num)
			{
				name = _name;
				type = _type;
				prop = _prop;
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}

			//����������� ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double) 
			var_const(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num)
			{
				name = _name;
				type = _type;
				prop = _prop;
				actn = _actn;
				var = _num;
				arg = 0;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = NULL;
			}


			//����������� ���������� (��� �������, ����� ���� double, ��������� "��������") 
			var_const(wstring _name, double _num, var_const *_pc)
			{
				name = _name;
				type = L"";
				prop = L"";
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = _pc;
			}

			//����������� ���������� (��� �������, ��� �������, ����� ���� double, ��������� "��������") 
			var_const(wstring _name, wstring _type, double _num, var_const *_pc)
			{
				name = _name;
				type = _type;
				prop = L"";
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = _pc;
			}

			//����������� ���������� (��� �������, ��� �������, �������� �������, ����� ���� double, ��������� "��������") 
			var_const(wstring _name, wstring _type, wstring _prop, double _num, var_const *_pc)
			{
				name = _name;
				type = _type;
				prop = _prop;
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = _pc;
			}

			//����������� ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double, ��������� "��������") 
			var_const(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, var_const *_pc)
			{
				name = _name;
				type = _type;
				prop = _prop;
				actn = _actn;
				var = _num;
				arg = 0;
				exp = 0;
				point_left = NULL;
				point_right = NULL;
				point_collar = _pc;
			}

			//����������� ���������� (��� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����") 
			var_const(wstring _name, double _num, var_const * _pl, var_const *_pr) {
				name = _name;
				type = L"";
				prop = L"";
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = NULL;
			}

			//����������� ���������� (��� �������, ��� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����") 
			var_const(wstring _name, wstring _type, double _num, var_const * _pl, var_const *_pr) {
				name = _name;
				type = _type;
				prop = L"";
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = NULL;
			}

			//����������� ���������� (��� �������, ��� �������, �������� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����") 
			var_const(wstring _name, wstring _type, wstring _prop, double _num, var_const * _pl, var_const *_pr) {
				name = _name;
				type = _type;
				prop = _prop;
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = NULL;
			}

			//����������� ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double, ��������� "����� �����", ��������� "������ �����") 
			var_const(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, var_const * _pl, var_const *_pr) {
				name = _name;
				type = _type;
				prop = _prop;
				actn = _actn;
				var = _num;
				arg = 0;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = NULL;
			}

			//����������� ���������� (��� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			var_const(wstring _name, double _num, var_const * _pl, var_const *_pr, var_const *_pc) {
				name = _name;
				type = L"";
				prop = L"";
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}

			//����������� ���������� (��� �������, ��� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			var_const(wstring _name, wstring _type, double _num, var_const * _pl, var_const *_pr, var_const *_pc) {
				name = _name;
				type = _type;
				prop = L"";
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}

			//����������� ���������� (��� �������, ��� �������, �������� �������, ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			var_const(wstring _name, wstring _type, wstring _prop, double _num, var_const * _pl, var_const *_pr, var_const *_pc) {
				name = _name;
				type = _type;
				prop = _prop;
				actn = L"";
				var = _num;
				arg = 0;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}

			//����������� ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double, ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			var_const(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, var_const * _pl, var_const *_pr, var_const *_pc) {
				name = _name;
				type = _type;
				prop = _prop;
				actn = _actn;
				var = _num;
				arg = 0;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}

			//������ �����������. ���������� (��� �������, ��� �������, �������� �������, �������� ��� �������� (��������), ����� ���� double (������), ����� ���� double (��������), ����� ���� int (������� 10), ��������� "����� �����", ��������� "������ �����", ��������� "��������") 
			var_const(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, double _arg, double _exp, var_const * _pl, var_const *_pr, var_const *_pc) {
				name = _name;
				type = _type;
				prop = _prop;
				actn = _actn;
				var = _num;
				arg = 0;
				exp = 0;
				point_left = _pl;
				point_right = _pr;
				point_collar = _pc;
			}

			//����������� �����������
			var_const(var_const* var1)
			{
				name = var1->name;
				type = var1->type;
				prop = var1->prop;
				actn = var1->actn;
				var = var1->var;
				arg = var1->arg;
				exp = var1->exp;
				point_left = var1->point_left;
				point_right = var1->point_right;
				point_collar = var1->point_collar;
			}

			~var_const()
			{

			}

			//����� �����������.
			void copy(var_const* ref) {
				name = ref->name;
				type = ref->type;
				prop = ref->prop;
				actn = ref->actn;
				var = ref->var;
				arg = ref->arg;
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
				if (type == addit)
					return 1 + (int)var;
				else if (type == mltpl)
					return 2 + (int)var;
				else if (type == divis)
					return 3 + (int)var;
				else if (type == power)
					return 4 + (int)var;
				else
					return 0;				
			}			

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

			/*����� �������� ����������� �������, ���������� �� ������ �������� � ��������� ������.
			����� ������ ���������� ����� ������ ��� ������������� delete. ��� ���� ��������
			*/
			int tree_destruct()
			{
				int s = 0;
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

			var_const *find_varbl(var_const*pointer)
			{
				if (var == 1)
				{
					if (pointer->name.compare(point_collar->name) == 0)
						return point_collar;
					else
						return NULL;
				}
				else
					return find_varbl_processing(point_collar, pointer);
			}

			/*����� ���������� ��������� �� ��������� ������� ������ ����������.
			���� ������ ��� ������� - �� ������� �������������� �������� (�������� ������ ������� �� ������ ���������).*/
			var_const *var_list_back()
			{
				return var_list_back_processing(point_collar);
			}

			/*����� ���������� ������ ���������� (���� �� ���� �� ������� ����������) ������� pointer �� ������� ���������� ������ �������.
			����� ����������:
			0 - ������ ���������� pointer ��������� ������ � ������ ���������� ������ �������.
			����� ������ ������������� ����� - ����� �������� ������� �������.
			*/
			int var_list_compare(var_const * pointer)
			{
				int count_var = 0;
				int temp = 0;
				//��������������� ������� � ����������� (����������� � ���)
				if ((pointer->prop == arg_v) || ((pointer->prop == undef) && (pointer->point_right != NULL)))
				{
					for (int count = 0; count < pointer->var; count++)
					{
						//���� ����� ���������� �������� ����������
						if (pointer->point_collar[count].type == varbl)
						{
							//���� ��� ���������� �� � ������ ������ �������
							if (find_varbl_processing(point_collar, &pointer->point_collar[count]) == NULL)
								count_var++;
						}
						//���� �������
						else if (pointer->point_collar[count].type == funct)
						{
							temp = var_list_compare(&pointer->point_collar[count]);
							count_var += temp;
							temp = 0;
						}
						else
						{
							//���� ����� ��� ��������� - ������ �� ������
						}
					}
				}
				//��������������� ������� ��� ����������
				else
				{
					var_const * iter = pointer->point_collar;
					while ((iter->point_left != NULL)&(iter->point_left->var != 0))
					{
						if (find_varbl_processing(point_collar, iter) == NULL)
							count_var++;
						iter = iter->point_left;
					}
				}
				return count_var;
			}

			//����� ����������� point_right ������ ������� ������ ���������� � ������ ������ � ����� � ����������� ����� ���������� �������. �������� ������ ��� ������� � ����������� ������� ����������. ���������� ������ ���������
			void arg_copy_plus_1()
			{
				var_const * ar = new var_const[var + 1];
				if (point_right == NULL)
				{
					//���� ������� ���������� ���������� ��� (� ������ ������� ����������� ������ ����������)
					var_const *iter = point_collar;
					for (int count = 0; count < var; count++)
					{
						ar[count].copy(iter);//��� ����� ����������� ����������
						iter = iter->point_left;
					}
				}
				else
				{
					for (int count = 0; count < var; count++)
					{
						ar[count].copy(&point_right[count]);
					}
					delete[]point_right;
					point_right = NULL;
				}
				point_right = ar;
				var += 1;
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

		var_const pi = var_const(L"pi", cnst, fundm, L"0", 3.1415926535897932384626433832);
		var_const e = var_const(L"e", cnst, fundm, L"0", 2.7182818284590452353602874713527);
		var_const i = var_const(L"i", cnst, fundm, L"0", 0);
		//��������� ���������� ��� ���������� "������" �������, ������ ����� ���, ��� ���������� � ������
		var_const temporary_variable = var_const(L"temporary_variable", varbl, servc, L"0", 0);






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
				general_var_const->push_back(new var_const(L"minus", funct, empty, L"", 1, new var_const(&temporary_variable)));
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
				general_var_const->back()->point_left->point_right = new var_const(L"", power, L"", L"", 0, general_var_const->back()->point_collar, new var_const(L"0", 2), NULL);
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
			unsigned int count, comma, count_var;
			var_const *high_pointer = NULL; //��������� ���������� ��������
			var_const *low_pointer = NULL; //��������� ���������� ���������/����������/�-���/���������
			var_const *temp_pointer = NULL;
			var_const *multiple_var = NULL;
			wchar_t* temp;
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
						if(current_element->prop != undef)
							current_element->actn = solve;
						else
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
							return NULL;
						}
					}
					else
					{
						//TODO: ����������� �����.
						temp_pointer = filling_vector(pDest + 1, endPtr, new var_const(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
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
										current_element->type = equat;
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
									current_element = new var_const(L"", equat, unslv, L"", 0, high_pointer, temp_pointer->point_left, new var_const(high_pointer->point_collar));
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
									current_element = new var_const(L"", equat, unslv, L"", 0, high_pointer, temp_pointer, new var_const(high_pointer->point_collar));
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
						current_element->point_left = new var_const(L"+", addit, brakets_counter + brakets, low_pointer, NULL, current_element);
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
							high_pointer->point_right = new var_const(L"+", addit,  brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new var_const(L"+", addit, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 1);
								high_pointer->point_right->point_collar = new var_const(L"+", addit, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=,");
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
						current_element->point_left = new var_const(L"*", mltpl, brakets_counter + brakets, low_pointer, NULL, current_element);
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
							high_pointer->point_right = new var_const(L"*", mltpl, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 2))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new var_const(L"*", mltpl, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 2);
								high_pointer->point_right->point_collar = new var_const(L"*", mltpl, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=,");
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
						current_element->point_left = new var_const(L"/", divis, brakets_counter + brakets, low_pointer, NULL, current_element);
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
							high_pointer->point_right = new var_const(L"/", divis, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 3))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new var_const(L"/", divis, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 3);
								high_pointer->point_right->point_collar = new var_const(L"/", divis, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=,");
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
						current_element->point_left = new var_const(L"^", power, brakets_counter + brakets, low_pointer, NULL, current_element);
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
							high_pointer->point_right = new var_const(L"^", power, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 4))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new var_const(L"^", power, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 4);
								high_pointer->point_right->point_collar = new var_const(L"^", power, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
								high_pointer->point_right = high_pointer->point_right->point_collar;
								high_pointer = high_pointer->point_right;
							}

						}
					}
					pDest++;
					temp = wcspbrk(pDest, L")+-*^/=,");
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
							current_element->point_left = new var_const(L"+", addit, brakets_counter + brakets, low_pointer, NULL, current_element);
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
								high_pointer->point_right = new var_const(L"+", addit, brakets_counter + brakets, low_pointer, NULL, high_pointer);
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
									high_pointer = new var_const(L"+", addit, brakets_counter + brakets, current_element->point_left, NULL, current_element);
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
									high_pointer->point_right->point_collar = new var_const(L"+", addit, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
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
					temp = wcspbrk(pDest, L")+-*^/=,");
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
						//������ ������ � �������. ����� �� ������, �� ����� � ���������). ������ � �������� ���������� ������ ��� �������� ������ ����. ������� ���� ���������.
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::EMTY_BRACKETS);
						return NULL;
					}
					else
					{
						name.assign(pDest + 1, temp);
						temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
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
								//����� ���� �� ����� �������� ���������
								current_element->point_left = temp_pointer->point_left;
								high_pointer = current_element->point_left;
								low_pointer = current_element->point_left;
								delete temp_pointer;
								temp_pointer = NULL;
							}
							else if (temp_pointer->type == varbl)
							{
								if (current_element->type == exprs)
								{
									current_element->type = funct;
									current_element->var = 1;
									if (temp_pointer->prop == undef)
									{
										temp_pointer->type = defnd;
										general_var_const->push_back(new var_const(temp_pointer));
									}
									current_element->point_collar = temp_pointer;
									current_element->point_left = current_element->point_collar;
									high_pointer = current_element->point_left;
									low_pointer = high_pointer;
								}
								else if (current_element->type == funct)
								{
									if (current_element->point_collar->point_right != NULL)
									{
										multiple_var = current_element->find_varbl(temp_pointer);
										if (multiple_var == NULL)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											current_element->point_left = multiple_var;
										}
									}
									//��� ��� ������ ����� � ����� ������ ������, ���� ������� ������� - �������, � ������ ���������� ������ �������
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
										return NULL;
									}
								}
							}
							else if (temp_pointer->type == funct)
							{
								if (temp_pointer->prop == arg_c)
								{
									current_element->point_left = temp_pointer;
									high_pointer = temp_pointer;
									low_pointer = high_pointer;
								}
								else if (current_element->type == exprs)
								{
									current_element->type = funct;
									current_element->prop = undef;
									current_element->var = 1;
									current_element->point_collar = new var_const(&temporary_variable);									
									current_element->point_right = temp_pointer;
									current_element->point_collar->var = 0; //������ ��� �������������)
									current_element->point_collar->point_collar = current_element;
									current_element->point_left = current_element->point_collar;
									high_pointer = current_element->point_left;
									low_pointer = high_pointer;
								}
								else if (current_element->type == funct)
								{
									if (current_element->point_collar->point_right != NULL)
									{
										if (current_element->var_list_compare(temp_pointer) != 0)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											current_element->point_left = temp_pointer;
										}
									}
									//��� ��� ������ ����� � ����� ������ ������, ���� ������� ������� - �������, � ������ ���������� ������ �������
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
										return NULL;
									}
								}
							}							
						}

						// ������� ������ ����� �����
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
									if (current_element->type == exprs)
									{
										current_element->type = funct;
										current_element->var = 1;
										if (temp_pointer->prop == undef)
										{
											temp_pointer->prop = defnd;
											general_var_const->push_back(new var_const(temp_pointer));
										}
										current_element->point_collar = temp_pointer;
										high_pointer->point_right = current_element->point_collar;
									}
									else if (current_element->type == funct)
									{
										if (current_element->point_collar->point_right != NULL)
										{
											multiple_var = current_element->find_varbl(temp_pointer);
											if (multiple_var == NULL)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											else
											{
												high_pointer->point_right = multiple_var;
											}
										}
										//��� ��� ������ ����� � ����� ������ ������, ���� ������� ������� - �������, � ������ ���������� ������ �������
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
											return NULL;
										}
									}
								}
								else if (temp_pointer->type == funct)
								{
									if (temp_pointer->prop == arg_c)
									{
										high_pointer->point_right = temp_pointer;										
									}
									else if (current_element->type == exprs)
									{
										current_element->type = funct;
										current_element->prop = undef;
										current_element->var = 1;
										current_element->point_collar = new var_const(&temporary_variable);
										current_element->point_right = temp_pointer;
										current_element->point_collar->var = 0; //������ ��� �������������)
										current_element->point_collar->point_collar = current_element;
										high_pointer->point_right = current_element->point_collar;										
									}
									else if (current_element->type == funct)
									{
										if (current_element->point_collar->point_right != NULL)
										{
											if (current_element->var_list_compare(temp_pointer) != 0)
											{
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
												return NULL;
											}
											else
											{
												high_pointer->point_right = temp_pointer;
											}
										}
										//��� ��� ������ ����� � ����� ������ ������, ���� ������� ������� - �������, � ������ ���������� ������ �������
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
											return NULL;
										}
									}									
								}
							}
							//����� ������� ���-�� ����� ������. ����� ������� ����� ������ �����-���� �����, �-� 5(...) = 5*(...). ���� ������. 
							else 
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
								return NULL;
							}

						}
						//���� ������ ����� �� � ������ ������ - ��������������� ������� �����-���� �������� ����� ���
						else 
						{
							if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
							{
								if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
								{
									high_pointer->point_right->point_right = temp_pointer;
								}
								else
								{
									high_pointer->point_right = temp_pointer;
								}
								low_pointer = high_pointer->point_right;															
							}
							else if (temp_pointer->type == varbl)
							{
								if (current_element->type == funct)
								{
									//������ ���������� �������
									multiple_var = current_element->find_varbl(temp_pointer);
									if (current_element->point_collar->point_right != NULL)
									{
										
										if (multiple_var == NULL)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											//����� �� ����� ���������� �����
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
											{
												high_pointer->point_right->point_right = multiple_var;
											}
											else
											{
												high_pointer->point_right = multiple_var;
											}
											low_pointer = high_pointer->point_right;
										}
									}
									//�� �������
									else
									{										
										if (multiple_var == NULL)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->type = defnd;
												general_var_const->push_back(new var_const(temp_pointer));
											}
											multiple_var = current_element->var_list_back();
											multiple_var->point_left = temp_pointer;
											multiple_var->point_left->point_right = multiple_var;
											multiple_var->point_left->point_collar = current_element;
											multiple_var->point_left->var = current_element->var;
											current_element->var += 1;
											//��������� ������ �� � ������ ������ - ������ ����� ����� �����-���� ��������. 
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//����� ����� ������
											{
												high_pointer->point_right->point_right = multiple_var->point_left;
											}
											else
											{
												high_pointer->point_right = multiple_var->point_left;
											}
											low_pointer = high_pointer->point_right;											
										}
										else
										{
											//����� �� ����� ���������� �����
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
											{
												high_pointer->point_right->point_right = multiple_var;
											}
											else
											{
												high_pointer->point_right = multiple_var;
											}
											low_pointer = high_pointer->point_right;
										}
									}
								}
								else if (current_element->type == exprs)
								{
									current_element->type = funct;
									current_element->var = 1;
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(new var_const(temp_pointer));
									}
									current_element->point_collar = temp_pointer;
									//����������� ����� ����� ����������
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
							else if (temp_pointer->type == funct)
							{
								if (temp_pointer->prop == arg_c)
								{
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = temp_pointer;
									}
									else
									{
										high_pointer->point_right = temp_pointer;
									}
									low_pointer = high_pointer->point_right;									
								}
								else if (current_element->type == exprs)
								{									
									current_element->type = funct;
									current_element->prop = undef;
									current_element->var = 1;
									current_element->point_collar = new var_const(&temporary_variable);
									current_element->point_right = temp_pointer;
									current_element->point_collar->var = 0; //������ ��� �������������)
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
								else if (current_element->type == funct)
								{
									if (current_element->point_collar->point_right != NULL)
									{
										if (current_element->var_list_compare(temp_pointer) != 0)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										else
										{
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
											{
												high_pointer->point_right->point_right = temp_pointer;
											}
											else
											{
												high_pointer->point_right = temp_pointer;
											}
											low_pointer = high_pointer->point_right;											
										}
									}
									else
									{
										current_element->arg_copy_plus_1();
										current_element->point_right[(int)current_element->var - 1] = temp_pointer;
										temp_pointer = NULL;
										temp_pointer = current_element->var_list_back();
										temp_pointer->point_left = new var_const(&temporary_variable);
										temp_pointer->point_left->point_right = temp_pointer;
										temp_pointer->point_left->var = current_element->var - 1;
										
										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
										{
											high_pointer->point_right->point_right = temp_pointer->point_left;
										}
										else
										{
											high_pointer->point_right = temp_pointer->point_left;
										}
										low_pointer = high_pointer->point_right;
									}
								}
							}
						}						
					}
					//����� ��������� � ��������� ������� ����� ������
					brakets_counter -= 4;
					pDest = temp + 1;
					if (*pDest == '(,')
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
						current_element->point_left = new var_const(L"", numbr, real, wcstod(pDest, &pDest), current_element);
						//��� ��������� -> �� �����, �������� ��� �������� ������� ��� ������ ��������
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					//����� ������ � ������ ������ ����� �����
					else if (high_pointer == low_pointer)
					{
						if (high_pointer->name == L"minus")
						{
							high_pointer->point_right = new var_const(L"", numbr, real, wcstod(pDest, &pDest), low_pointer);
						}
						else
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
							return NULL;
						}
					}
					// ���� ������ ����� �� ������ � ������ (���� �����-���� ��������)
					else
					{
						//�������� �������� ������ � ������ �����, �������� -> ���� �������� 						
						if ((high_pointer->point_right != NULL) && (low_pointer->name == L"minus"))
						{
							low_pointer->point_right = new var_const(L"", numbr, real, wcstod(pDest, &pDest), low_pointer);
						}
						else
						{
							high_pointer->point_right = new var_const(L"", numbr, real, wcstod(pDest, &pDest), high_pointer);
							low_pointer = high_pointer->point_right;
						}
					}
				}

				//������ � ��������� � ������
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
										//������� �������. ���������� ������� � ������� +1 = ����� ��������� ����������
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
								//����� ������ ������������� ������ � ����� ����� ������
								if (pDest + 1 == temp)
								{
									//������ ������ � �������.  �������� ������� �� �������������� ���������� ����������
									//���������� ������� ������� ������ ��� �������� ����������. ��������� �� ���������� - ������
									current_element->type = funct;
									current_element->name = name;
									current_element->prop = undef; //������� ����� ������� undef, �� ��� ���� �� ����� ��� � �� ���� ���
									current_element->actn = write;  //�� ����������
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								else
								{
									//���������� ����� ����������
									current_element->var = comma + 1;
									current_element->type = funct;
									current_element->name = name;
									current_element->prop = undef; //������� ����� ������� undef, �� ��� ���� �� ����� ��� � �� ���� ���
									current_element->actn = write;  //�� ����������
									for (count = 0; count < comma + 1; count++)
									{
										temp = wcspbrk(pDest + 1, L",)");
										name.assign(pDest + 1, temp);
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"", exprs, undef, 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new var_const(temp_pointer));
											}
											temp_pointer->point_collar = current_element;
											temp_pointer->var = count; //��� ���������� ������ ������ ���������� � ������� ������ ����� ����������. ��������������� ��� ���������� �� ����� �������������� �������� => ����� ������������ var ��� �������

											if (count == 0)
												//����� �� �������� �������� �� ������ ���������� � ������
												current_element->point_collar = temp_pointer;
											else
											{
												//�������� ���������������� ������ ����������, ��� �� ������� ������� ��������� �������� �������.
												temp_pointer->point_right = current_element->var_list_back();
												temp_pointer->point_right->point_left = temp_pointer;
											}
											if (count == comma)
											{
												//�������� ���������� � ������ ���������������� ������ ����������. ������� ������� = ����������.
												temp_pointer->point_left = current_element->point_collar;
												current_element->point_collar->point_right = temp_pointer;
											}
											//temp_pointer � ������ ������ ������� ������. ����� ��������, �� ������� ������
										}
										else
										{
											//��������� ������� ��� ������������� ���������� �� ����� ��������� ����� �������
											//TODO:������� ����� ����� ������
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										pDest = temp;
									}
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
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->type = funct;
								current_element->prop = undef;
								//���������� ����� ����������
								current_element->var = 1;
								general_var_const->push_back(new var_const(name, varbl, defnd, 0));
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(general_var_const->back());
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
						//������ ������� ������� � ����������� ������ - ���������
						else if (high_pointer->type == cnst)
						{
							//�������� ���������
							if ((temp == NULL) || (*temp == '='))
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//��� ���� � ���, ��� ���� ��� ���� �����-�� ��������� - ����� �� �� ���� ��������� ��� �� - ������� ����� �������� ���������.
							//������ ������ ������ ����� �������� ��������������� ����� � ���� �������
							else if (*temp == '(')
							{
								//����� ����� ����� ��������� ��� ������ ��� ����������,
								//����������� � ������ � ������� ��� ���
								//current_element->copy(high_pointer);
								brakets_counter += 4;
								count = 1;
								comma = 0;
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
										//������� �������. ���������� ������� � ������� +1 = ����� ��������� ����������
										comma++;
									}
								}
								if (*(temp + 1) != '=') //��� �������� - ����� ����� ����� ������ ��� ���
								{
									//���� ��� ���, �� ������ ������ � ����� ������ ����������� ������ ������ ���������, ����� �� ��������� �� �� ������������
									//�������� ���������
									if (*(temp + 1) == NULL) // ���� �� ����� � ����� ������ - �� ���� - �.�. ����� ������ ����������� �������
									{
										current_element->copy(high_pointer);
										high_pointer = current_element;
										low_pointer = high_pointer;
									}
									else if ((*(temp + 1) == '+') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^') || (*(temp + 1) == '-'))
									{
										current_element->point_left = new var_const(high_pointer);
										current_element->point_left->point_collar = current_element;
										high_pointer = current_element->point_left;
										low_pointer = high_pointer;
									}
									/*else if ((*(temp + 1) == '('))
									{
										//���� ������� ��� �������. ������ ���� �(...)(...) - ������ ��� �������� ������������ �()*(...)
									}*/
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
										return NULL;
									}
								}
								//����� ������ ������������� ������ � ����� ����� ������
								else if (pDest + 1 == temp)
								{
									// ��� ���� ����� ��������, ���� ����� ����������� c()=
									current_element->copy(high_pointer);
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								//��� ���� ������ ���������, ����� ��������� � ������� ���� �����������
								else
								{
									current_element->copy(high_pointer);
									for (count = 0; count < comma + 1; count++)
									{
										temp = wcspbrk(pDest + 1, L",)");
										name.assign(pDest + 1, temp);
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"", exprs, undef, 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new var_const(temp_pointer));
											}
											temp_pointer->point_collar = current_element;
											temp_pointer->var = count; //��� ���������� ������ ������ ���������� � ������� ������ ����� ����������. ��������������� ��� ���������� �� ����� �������������� �������� => ����� ������������ var ��� �������

											if (count == 0)
												//����� �� �������� �������� �� ������ ���������� � ������
												current_element->point_collar = temp_pointer;
											else
											{
												//�������� ���������������� ������ ����������, ��� �� ������� ������� ��������� �������� �������.
												temp_pointer->point_right = current_element->var_list_back();
												temp_pointer->point_right->point_left = temp_pointer;
											}
											if (count == comma)
											{
												//�������� ���������� � ������ ���������������� ������ ����������. ������� ������� = ����������.
												temp_pointer->point_left = current_element->point_collar;
												current_element->point_collar->point_right = temp_pointer;
											}
											//temp_pointer � ������ ������ ������� ������. ����� ��������, �� ������� ������
										}
										else
										{
											//TODO:� ������ ���� ����� ������� ������ ��� ����������� ����������
											break;
										}
										pDest = temp;
									}

									if ((count == comma + 1) && (high_pointer->prop != fundm))//�� ���� ��������������� ��������
									{
										//���������� ����� ����������
										current_element->var = comma + 1;
										current_element->type = funct;
										current_element->prop = undef; //������� ����� ������� undef, �� ��� ���� �� ����� ��� � �� ���� ���
										current_element->actn = write; //�� ������										
										low_pointer = high_pointer;
									}
									else
									{
										// ��� ���� ����� ��������, ���� ����� �����������, ��������� ���������
										//��� ��� ����� �������� ��������� �(...) = �*(...). � ������� ������ �� ������ �*(...)= 
										//������ � ������ ������ ��� ��������� �� ������ - ��� �� �����������, �������� ����� ���-�� �������� ������	
										high_pointer = current_element;
										low_pointer = high_pointer;
									}
								}
								brakets_counter -= 4;
								pDest = temp + 1;
							}
							else if (*temp == ')')
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//������� ������� ������� ����������, ��������� ��������� (� �����) - ���� �� ���������
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->point_left = new var_const(high_pointer);
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
						//������ ������� ������� � ����������� ������ - ����������
						else if (high_pointer->type == varbl)
						{
							//�������� ����������
							if ((temp == NULL) || (*temp == '='))
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
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
								brakets_counter += 4;
								count = 1;
								comma = 0;
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
										//������� �������. ���������� ������� � ������� +1 = ����� ��������� ����������
										comma++;
									}
								}
								if (*(temp + 1) != '=')
								{
									//���� � ����� ������ ������ ����� ���������� � ����� ������ - ������� ��� ��� ����������� ����� �������
									//���� ����� ����� ��� ����� - ������
									//��� �� - ����� ���� ��������� ���� x(...) ��� ������ x*(...)
									//������ �� ����� �������� � ������� ����������� �������. ����� ��� ��� ����� ��������� �������
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								//����� ������ ������������� ������ � ����� ����� ������
								else if (pDest + 1 == temp)
								{
									//������ ������ � �������. ����������, �������� �������� ������� �� �������������� ���������� ����������
									//���������� ������� ������� ������ ��� �������� ����������. ��������� �� ���������� - ������
									current_element->copy(high_pointer);
									current_element->type = funct;
									current_element->prop = undef; //������� ����� ������� undef, �� ��� ���� �� ����� ��� � �� ���� ���		
									current_element->actn = write; //�� ������
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								else
								{
									current_element->copy(high_pointer);
									for (count = 0; count < comma + 1; count++)
									{
										temp = wcspbrk(pDest + 1, L",)");
										name.assign(pDest + 1, temp);
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new var_const(temp_pointer));
											}
											temp_pointer->point_collar = current_element;
											temp_pointer->var = count; //��� ���������� ������ ������ ���������� � ������� ������ ����� ����������. ��������������� ��� ���������� �� ����� �������������� �������� => ����� ������������ var ��� �������

											if (count == 0)
												//����� �� �������� �������� �� ������ ���������� � ������
												current_element->point_collar = temp_pointer;
											else
											{
												//�������� ���������������� ������ ����������, ��� �� ������� ������� ��������� �������� �������.
												temp_pointer->point_right = current_element->var_list_back();
												temp_pointer->point_right->point_left = temp_pointer;
											}
											if (count == comma)
											{
												//�������� ���������� � ������ ���������������� ������ ����������. ������� ������� = ����������.
												temp_pointer->point_left = current_element->point_collar;
												current_element->point_collar->point_right = temp_pointer;
											}
											//temp_pointer � ������ ������ ������� ������. ����� ��������, �� ������� ������
										}
										else
										{
											//��������� ������� ��� ������������� ���������� �� ����� ��������� ����� �������
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										pDest = temp;
									}
									//���������� ����� ����������
									current_element->var = comma + 1;
									current_element->type = funct;
									current_element->prop = undef; //������� ����� ������� undef, �� ��� ���� �� ����� ��� � �� ���� ���
									current_element->actn = write; //�� ������	
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								brakets_counter -= 4;
								pDest = temp + 1;
							}
							//������� ������� => �������, ���������� �������� ����� ����������
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->type = funct;
								current_element->prop = undef;
								//���������� ����� ����������
								current_element->var = 1;
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(high_pointer);
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
						//������ ������� ������� � ����������� ������ - �������
						else if (high_pointer->type == funct)
						{
							//�������� �������
							if ((temp == NULL) || (*temp == '='))
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;
								//������ ������� ����� ���� ������������ � �����-�� ���������/������ ������� � �������� ��������. 
								//����� ��� �� �������� � ��������������� � ����������� ����������� ����������
							}
							//� ������ ������ �������� ������ ����� �������� ��� ����������� �-��� (�-� f(x)) ����� �������:
							//--����� �����������, � ��� ����� �� ����� ����������
							//--�������� ������� ����������, ��� ����������� ���������, ��� � ����� ����������. � ���� ������ �������, 
							//��� ��������������� f(t)= - ������ �� ����� ��������� �� t, ���� ������� f � ��������� � ���������� t, �� ����. ������ � ����������������.
							//--�������� ������� � �������� ��������� ����� ������� g(s) - ��� ���� ������������ ������� f(g(s)) ������ ����� �������� �� s, � �� �� �,
							//������ ������ �������� �� ����� ������������ �������, ���������� g(s) - ������ ������ - f(5*s+2)
							else if (*temp == '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
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
										//������� �������. ���������� ������� � ������� +1 = ����� ��������� ����������
										comma++;
									}
								}
								if (pDest + 1 == temp)
								{
									//������ ������ � �������. ����������, ���� ����������� ������� �� �������������� ���������� ���������� ��� ������ ������������� ������� � ���������
									//���������� ������� ������� ������ ��� �������� ����������. ��������� �� ���������� - ��� ��� ��� ������
									if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
									{
										current_element->copy(high_pointer);
										high_pointer = current_element;
										low_pointer = high_pointer;
									}
									//������, ����������� ���������� ������� � ������ ����� 
									else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
									{
										current_element->type = funct;
										current_element->prop = undef;
										current_element->var = 1; // ���� ����������
										current_element->point_right = new var_const(high_pointer); //��������� ������� - �������� �������
										//����� ��������� ���������� � ���������� �� �������
										current_element->point_collar = new var_const(&temporary_variable);
										current_element->point_collar->var = 0; //������ ��� �������������)
										current_element->point_collar->point_collar = current_element;
										current_element->point_left = current_element->point_collar;

										high_pointer = current_element->point_left;
										low_pointer = high_pointer;
									}
								}
								//����� ���������� � ������� ��������� � ������ ���������� � ����������� ������� �������
								else if (high_pointer->var == comma + 1)
								{
									//��� ������� 
									if (comma == 0)
									{
										name.assign(pDest + 1, temp);
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if ((temp_pointer->type == cnst) || (temp_pointer->type == exprs))
										{
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												current_element->point_right = temp_pointer;
												current_element->prop = arg_c; //��������� ��� � ������� �������� ��������
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->point_left = new var_const(high_pointer);
												current_element->point_left->point_right = temp_pointer;
												current_element->point_left->prop = arg_c;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
										else if (temp_pointer->type == funct)
										{
											if (temp_pointer->prop == arg_c)
											{
												if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
												{
													current_element->copy(high_pointer);
													current_element->point_right = temp_pointer;
													current_element->prop = arg_c; //��������� ��� � ������� �������� ��������
													high_pointer = current_element;
													low_pointer = high_pointer;
												}
												else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													current_element->point_left = new var_const(high_pointer);
													current_element->point_left->point_right = temp_pointer;
													current_element->point_left->prop = arg_c;
													high_pointer = current_element->point_left;
													low_pointer = high_pointer;
												}
											}
											else
											{
												if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
												{
													current_element->copy(high_pointer);
													current_element->point_right = temp_pointer;
													current_element->prop = arg_v; //��������� ��� � ������� �������� ��������
													high_pointer = current_element;
													low_pointer = high_pointer;
												}
												else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													current_element->type = funct;
													current_element->prop = undef;
													current_element->var = 1; // ���� ����������
													current_element->point_right = new var_const(high_pointer); //��������� ������� - �������� �������
																												//����� ��������� ���������� � ���������� �� �������
													current_element->point_collar = new var_const(&temporary_variable);
													current_element->point_collar->var = 0; //������ ��� �������������)
													current_element->point_collar->point_collar = current_element;
													current_element->point_left = current_element->point_collar;
													current_element->point_right->point_right = temp_pointer;
													current_element->point_right->prop = arg_v;
													high_pointer = current_element->point_left;
													low_pointer = high_pointer;
												}
											}
										}
										else if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new var_const(temp_pointer));
											}
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												current_element->point_right = temp_pointer;
												current_element->prop = arg_v; //��������� ��� � ������� �������� ��������
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->type = funct;
												current_element->prop = undef;
												current_element->var = 1; // ���� ����������
												current_element->point_right = new var_const(high_pointer); //��������� ������� - �������� �������
																											//����� ��������� ���������� � ���������� �� �������
												current_element->point_collar = new var_const(&temporary_variable);
												current_element->point_collar->var = 0; //������ ��� �������������)
												current_element->point_collar->point_collar = current_element;
												current_element->point_left = current_element->point_collar;
												current_element->point_right->point_right = temp_pointer;
												current_element->point_right->prop = arg_v;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
									}
									//���� � ������� ����� ������� - ����������� ������� ���������� ����������
									else
									{
										multiple_var = new var_const[comma + 1];
										count_var = 0;
										for (count = 0; count < comma + 1; count++)
										{
											temp = wcspbrk(pDest + 1, L",)");
											name.assign(pDest + 1, temp);
											temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
											if (temp_pointer == NULL)
												return temp_pointer;
											//��������� ������ ����������
											if (temp_pointer->type == varbl)
											{
												count_var++;
												if (temp_pointer->prop == undef)
												{
													temp_pointer->prop = defnd;
													general_var_const->push_back(new var_const(temp_pointer));
												}
											}
											else if (temp_pointer->type == funct)
											{
												if (temp_pointer->prop != arg_c)
												{
													count_var++;
												}
											}
											multiple_var[count].copy(temp_pointer);
											pDest = temp;
										}
										if (count_var != 0)//���� ����� ���������� ���� �� ���� ����������
										{
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												current_element->point_right = multiple_var;
												current_element->prop = arg_v; //��������� ��� � ������� �������� ��������
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->type = funct;
												current_element->prop = undef;
												current_element->var = 1; // ���� ����������
												current_element->point_right = new var_const(high_pointer); //��������� ������� - �������� �������
																											//����� ��������� ���������� � ���������� �� �������
												current_element->point_collar = new var_const(&temporary_variable);
												current_element->point_collar->var = 0; //������ ��� �������������)
												current_element->point_collar->point_collar = current_element;
												current_element->point_left = current_element->point_collar;
												current_element->point_right->point_right = multiple_var;
												current_element->point_right->prop = arg_v;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
										else // ��� ��������� �� ����������
										{
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												current_element->point_right = multiple_var;//� �������� ��������� - ������ ��������
												current_element->prop = arg_c; //��������� ��� � ������� �������� ��������
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else
											{
												current_element->point_left = new var_const(high_pointer);
												current_element->point_left->point_right = temp_pointer;
												current_element->point_left->prop = arg_c;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
									}
								}
								//����� ���������� - ��������� 
								else
								{
									if (*(temp + 1) != '=')
									{
										//��� ������: ����� ���� f(x,y), �������� f(k)+... - ���������� ���������� � ���������� ������� �� ���������
										//TODO:����� ���� ����� �������� ���������� ��� ������� ���������� ����������, ������� ����� ���������� ��� ������� ������ ���������
										//������ root(5) - ���������� ������ �� 5, root (5,2) - ���� �����, root (5,3) - ���������� ������ �� 5
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;

									}
									//�� ���� ����� ����� ����� - �� ���������,  ���������������. 
									//TODO:��� ���� ���������� ������� ���������� ������ ��������, ������ �� �����, � ����� ��� �������� ����� �����, �.�. ��� ��� ����� ���� ������ �����.	
									//��� ��, � ������ ������ ����� - �������� ��-���� � ������ �������, �.�. ������ ������� ��� �� �����, � ��� ���������� ��� �������)

									else
									{
										current_element->copy(high_pointer);
										for (count = 0; count < comma + 1; count++)
										{
											temp = wcspbrk(pDest + 1, L",)");
											name.assign(pDest + 1, temp);
											temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
											if (temp_pointer == NULL)
												return temp_pointer;
											if (temp_pointer->type == varbl)
											{
												if (temp_pointer->prop == undef)
												{
													temp_pointer->prop = defnd;
													general_var_const->push_back(new var_const(temp_pointer));
												}
												temp_pointer->point_collar = current_element;
												temp_pointer->var = count; //��� ���������� ������ ������ ���������� � ������� ������ ����� ����������. ��������������� ��� ���������� �� ����� �������������� �������� => ����� ������������ var ��� �������

												if (count == 0)
													//����� �� �������� �������� �� ������ ���������� � ������
													current_element->point_collar = temp_pointer;
												else
												{
													//�������� ���������������� ������ ����������, ��� �� ������� ������� ��������� �������� �������.
													temp_pointer->point_right = current_element->var_list_back();
													temp_pointer->point_right->point_left = temp_pointer;
												}
												if (count == comma)
												{
													//�������� ���������� � ������ ���������������� ������ ����������. ������� ������� = ����������.
													temp_pointer->point_left = current_element->point_collar;
													current_element->point_collar->point_right = temp_pointer;
												}
												//temp_pointer � ������ ������ ������� ������. ����� ��������, �� ������� ������
											}
											else
											{
												//��������� ������� ��� ������������� ���������� �� ����� ��������� ����� �������, �.�. ���������� ���������� �� ���������
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
												return NULL;
											}
											pDest = temp;
										}
										//���������� ����� ����������
										current_element->var = comma + 1;
										current_element->prop = undef; //������� ����� ������� undef, �� ��� ���� �� ����� ��� � �� ���� ���
										current_element->actn = write;  //�� ����������		
										high_pointer = current_element;
										low_pointer = high_pointer;
									}

								}
								brakets_counter -= 4;
								pDest = temp + 1;
							}
							else if (*temp == ')')
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//������� ��������� => �������, ����� ��������� ������� - �������� ���������.
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->type = funct;
								current_element->prop = undef;
								current_element->point_right = new var_const(high_pointer); //�������� ����� ������������� �������, ������� ������� �� �����-���� ����������,
																							//������� �������� � ���� ������ (�����������) �������. ��� ���� ���� ��������, ���
																							//� ��������� ��������� ��������� ������� (f+g), �� �������, ������ �� ����������
																							//����� ��������� �������� ���������� ����������, 
																							//���� ���� ��� ������� � � ��������� ������� �� ����� � ��� �� ����������.
								//����� ��������� ���������� � ���������� �� �������, ������� ������ ����� ����
								current_element->point_collar = new var_const(&temporary_variable);
								current_element->point_collar->var = 0; //������ ��� �������������)
								current_element->point_collar->point_collar = current_element;
								current_element->point_left = current_element->point_collar;//�.�. ������ ������� ����� ����� ���� � ���� ��������� � ���������� ������, 
																							//��� ������� ��������� �� ����������, ������� � ������� ���������� ���-�� ���������
								current_element->var = 1; //���������� ����������								
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
						}
					}
					//������ ��������� �� � ������ ������
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

						//����� ������ ����� ���� ������ ��� �������� - ������� ������� ���� �������, ���� ���������, ������ ������ undef.
						//���� �� ������ �� ���� ������� ������� � ����� ������
						if (low_pointer == NULL)
						{
							if (*temp == '(),')
							{
								//������ - ����� ���������� ����� ������������� ������. ������ �� ������ ������������� ������, �������� ����� ������ ��� ������ ��� ��������� ��� ����� ���������� ������������.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//���� ������� ������� - �������, �� �������� ������� ����� ���������� � ������ ���������. 
							
							
							if (current_element->type == funct)
							{
								if (current_element->point_collar->point_right != NULL)
								{
									//������ ������ ���������� ������� => ����� ���������� - ������
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
									return NULL;
								}
								//������ �� ������� - ����� �������� ������ ����������
								else
								{
									general_var_const->push_back(new var_const(name, varbl, defnd, 0));
									temp_pointer = current_element->var_list_back();
									temp_pointer->point_left = new var_const(general_var_const->back());
									temp_pointer->point_left->point_right = temp_pointer;
									temp_pointer->point_left->point_collar = current_element;
									temp_pointer->point_left->var = current_element->var;
									current_element->var += 1;
									//��������� ������ �� � ������ ������ - ������ ����� ����� �����-���� ��������. 
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//����� ����� ������
									{
										high_pointer->point_right->point_right = temp_pointer->point_left;
									}
									else
									{
										high_pointer->point_right = temp_pointer->point_left;
									}
									low_pointer = high_pointer->point_right;
									temp_pointer = NULL;
								}
							}
							//���� ���������
							else if (current_element->type == exprs)
							{
								// ��������� � undef ������� 
								current_element->type = funct;
								current_element->prop = undef;
								general_var_const->push_back(new var_const(name, varbl, defnd, 0));
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(general_var_const->back());
								current_element->point_collar->point_collar = current_element;
								current_element->var = 1;//���� ����������
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
							if (*temp == '(),')
							{
								//������ - ����� ���������� ����� ������������� ������. ������ �� ������ ������������� ������, �������� ����� ������ ��� ������ ��� ��������� ��� ����� ���������� ������������.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							if (current_element->type == funct)
							{
								temp_pointer = current_element->find_varbl(low_pointer);
								if (current_element->point_collar->point_right != NULL)
								{
									if (temp_pointer == NULL)
									{
										//������ ������ ���������� ������� => ����� ���������� - ������
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									else
									{
										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
										{
											high_pointer->point_right->point_right = temp_pointer;//��������� �� ������ ����� � ������ ����������
										}
										else
										{
											high_pointer->point_right = temp_pointer;
										}
										low_pointer = high_pointer->point_right;
									}
								}
								//������ �� ������� - ����� �������� ������ ����������
								else if (temp_pointer == NULL) //���� ��� ���������� ��� ���������� - ������� ����� ����������
								{
									temp_pointer = current_element->var_list_back();
									temp_pointer->point_left = new var_const(low_pointer);
									temp_pointer->point_left->point_right = temp_pointer;
									temp_pointer->point_left->point_collar = current_element;
									temp_pointer->point_left->var = current_element->var;
									current_element->var += 1;
									//��������� ������ �� � ������ ������ - ������ ����� ����� �����-���� ��������. 
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//����� ����� ������
									{
										high_pointer->point_right->point_right = temp_pointer->point_left;
									}
									else
									{
										high_pointer->point_right = temp_pointer->point_left;
									}
									low_pointer = high_pointer->point_right;									
								}
								else //��� ���������� ������ ���������� ���������� � ���������
								{
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = temp_pointer;//��������� �� ������ ����� � ������ ����������
									}
									else
									{
										high_pointer->point_right = temp_pointer;
									}
									low_pointer = high_pointer->point_right;
								}
							}
							else if (current_element->type == exprs)
							{
								current_element->type = funct;
								current_element->prop = undef;
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new var_const(low_pointer);
								current_element->point_collar->point_collar = current_element;
								current_element->var = 1;//���� ����������
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
							if (*temp = '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
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
										//������� �������. ���������� ������� � ������� +1 = ����� ��������� ����������
										comma++;
									}
								}
								//���� ���������� ���������� �� ��������� � ����������� ����������
								if (comma + 1 != low_pointer->var)
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
									return NULL;
								}
								multiple_var = new var_const[comma + 1];
								count_var = 0;
								for (count = 0; count < comma + 1; count++)
								{
									temp = wcspbrk(pDest + 1, L",)");
									name.assign(pDest + 1, temp);
									temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"", exprs, undef, 0, current_element), brakets + brakets_counter);
									if (temp_pointer == NULL)
										return temp_pointer;
									//��������� ������ ����������
									if (temp_pointer->type == varbl)
									{
										count_var++;
										if (temp_pointer->prop == undef)
										{
											temp_pointer->prop = defnd;
											general_var_const->push_back(temp_pointer);
										}
										multiple_var[count].copy(temp_pointer);
									}
									else if ((temp_pointer->type == cnst) || (temp_pointer->type == exprs))
									{
										multiple_var[count].copy(temp_pointer);
									}
									else if (temp_pointer->type == funct)
									{
										//TODO:��� �������� ����������� ������ ������ f(..,..,..), ��� f � ) �������� �������������� ������� � ������ ������ (� ������ �������� �����)
										//��� � �������� ��������� ���������� ���������� �������, ������� ���� defnd, ���� undef
										if (temp_pointer->prop != arg_c)
										{
											count_var++;
											if ((temp_pointer->prop == defnd) || (temp_pointer->prop == undef))
												multiple_var[count].copy(temp_pointer);
											else if (temp_pointer->prop == arg_v)
											{
												multiple_var[count].copy(temp_pointer);
												temp_pointer->point_right = NULL;//�������� ������ ����������. ��� �������!
											}
										}
										else //��� ����� ��������������� defnd ������� � ����� �����������										
											multiple_var[count].copy(temp_pointer);
									}
									pDest = temp;
								}
							}
							else if ((*temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{

							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
							if (current_element->type == funct)
							{
								//���� ��� ��������� ���������� �� ������� ���� ���������, ���� �����. ���������, ���� ������� � ����� �����������
								if (count_var == 0)
								{
									temp_pointer = new var_const(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_c;//��������� ��� ��������� - ���������
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = temp_pointer;
									}
									else
									{
										high_pointer->point_right = temp_pointer;
									}
									low_pointer = high_pointer->point_right;
								}
								//� �������� ���������� ���� ����������
								else
								{
									temp_pointer = new var_const(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_v;//��������� ��� ����� ���������� ���� ����������
									//���� ������ ���������� �� �������
									if (current_element->point_collar->point_right == NULL)
									{
										current_element->arg_copy_plus_1();
										current_element->point_right[(int)current_element->var - 1] = temp_pointer;
										temp_pointer = NULL;
										temp_pointer = current_element->var_list_back();
										temp_pointer->point_left = new var_const(&temporary_variable);
										temp_pointer->point_left->point_right = temp_pointer;
										temp_pointer->point_left->var = current_element->var - 1;
									}
									//�������
									else
									{
										//���� ����� ���������� ���������� ��������� ������� ���� ���� ���� �� �������� � ��������� ������
										if (current_element->var_list_compare(temp_pointer) != 0)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										//��� ���������� ��������� ������� ������ � ������ ���������� �������� ��������
										else
										{
											//����� ��������� ������� ����������� � ���������
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
											{
												high_pointer->point_right->point_right = temp_pointer;
											}
											else
											{
												high_pointer->point_right = temp_pointer;
											}
											low_pointer = high_pointer->point_right;
										}
									}
								}
							}
							else if (current_element->type == exprs)
							{
								//������� ������� ������� ����������.
								if (count_var == 0)
								{
									temp_pointer = new var_const(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_c;//��������� ��� ��������� - ���������

									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = temp_pointer;
									}
									else
									{
										high_pointer->point_right = temp_pointer;
									}
									low_pointer = high_pointer->point_right;
								}
								//������� ������� ���������� ��������.
								else
								{
									temp_pointer = new var_const(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_v;//��������� ��� ����� ���������� ���� ����������
									current_element->type = funct;
									current_element->prop = undef;
									current_element->prop = arg_v;
									current_element->point_right = temp_pointer; //�������� ����� ������������� �������, ������� ������� �� �����-���� ����������,
																								//������� �������� � ���� ������ (�����������) �������. ��� ���� ���� ��������, ���
																								//� ��������� ��������� ��������� ������� (f+g), �� �������, ������ �� ����������
																								//����� ��������� �������� ���������� ����������, 
																								//���� ���� ��� ������� � � ��������� ������� �� ����� � ��� �� ����������.
																								//����� ��������� ���������� � ���������� �� �������, ������� ������ ����� ����
									current_element->point_collar = new var_const(&temporary_variable);
									current_element->point_collar->var = 0; //������ ��� �������������)
									current_element->point_collar->point_collar = current_element;
									current_element->point_left = current_element->point_collar;//�.�. ������ ������� ����� ����� ���� � ���� ��������� � ���������� ������, 
																								//��� ������� ��������� �� ����������, ������� � ������� ���������� ���-�� ���������
									current_element->var = 1; //���������� ����������
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
			wstring output;
			size_t output_size;
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
				else if (CE->prop == defnd)
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


			//general_var_const->push_back(new var_const(L"", exprs, undef, write, 0));

			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer	
			return analized_output(point_start, point_end, new var_const(L"", exprs, undef, write, 0));
		}
	}
}
