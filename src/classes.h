
#include <iostream>
#include <string>
#include <stack>
//#include "IO.h"
//#include "error.h"


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

		class math_obj {
		private:
			int tree_destruct_processing(math_obj* pointer)
			{
				if (pointer == NULL) return 0;
				int temp = 0;

				if ((pointer->type == numbr) || (pointer->type == cnst))
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
						temp += tree_destruct_processing(pointer->point_collar);
						delete pointer;
						pointer = NULL;
						return temp;
					}
					else if (pointer->prop == defnd)
					{
						delete pointer;
						pointer = NULL;
						return temp;
					}
					else if ((pointer->prop == arg_c) || (pointer->prop == arg_v))
					{
						//��� ����� ����������� ������ ������
						if (pointer->var>1)
							delete[]pointer->point_right;
						else
							delete pointer->point_right;
						delete pointer;
						pointer = NULL;
						return temp;
					}
				}
			}			

			math_obj *prioritize_processing(math_obj *pc, int current_priority)
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

			double arithmetic_processing(math_obj *pointer, math_obj * last_arg)
			{
				if (pointer->type == addit)
					return arithmetic_processing(pointer->point_left, last_arg) + arithmetic_processing(pointer->point_right, last_arg);
				else if (pointer->type == mltpl)
					return arithmetic_processing(pointer->point_left, last_arg) * arithmetic_processing(pointer->point_right, last_arg);
				else if (pointer->type == divis)
					return arithmetic_processing(pointer->point_left, last_arg) / arithmetic_processing(pointer->point_right, last_arg);
				else if (pointer->type == power)
					return pow(arithmetic_processing(pointer->point_left, last_arg), arithmetic_processing(pointer->point_right, last_arg));
				else if ((pointer->type == numbr) || (pointer->type == cnst))
					return pointer->var;
				else if (pointer->type == exprs)
					return arithmetic_processing(pointer->point_left, last_arg);				
				else if (pointer->type == funct)
				{
					if (pointer->prop == arg_c)
						return arithmetic_processing(pointer->point_left, pointer->point_right);
					else
					{						
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::VARIABL_FUNCT);
						return 0;
					}
				}
				else if (pointer->type == varbl)
					return arithmetic_processing(&last_arg[(int)pointer->var], last_arg);
			}
			//���� ���������
			wstring expression_processing(math_obj *pointer, int* comma)
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

			math_obj *var_list_back_processing(math_obj *pointer)
			{
				if ((pointer->point_left == NULL) || (pointer->point_left->var == 0))
					return pointer;
				else
					return var_list_back_processing(pointer->point_left);
			}

			math_obj *find_varbl_processing(math_obj*pointer, math_obj *original)
			{
				if (original->name.compare(pointer->name) == 0)
					return pointer;
				else if ((pointer->point_left != NULL) && (pointer->point_left->var != 0))
					return find_varbl_processing(pointer->point_left, original);
				else
					return NULL;
			}

			math_obj *merge_lists(math_obj* list_a, math_obj* list_b)
			{
				math_obj * merged, *temp;
				if (list_a->name.compare(list_b->name) < 0)
				{
					merged = list_a;
					list_a = list_a->point_left;
				}
				else
				{
					merged = list_b;
					list_b = list_b->point_left;
				}
				temp = merged;
				while ((list_a != NULL) && (list_b != NULL))
				{
					if (list_a->name.compare(list_b->name) < 0)
					{
						temp->point_left = list_a;
						temp = temp->point_left;
						list_a = list_a->point_left;
					}
					else
					{
						temp->point_left = list_b;
						temp = temp->point_left;
						list_b = list_b->point_left;
					}
				}
				if (list_a == NULL)
					temp->point_left = list_b;
				else
					temp->point_left = list_a;
				return merged;
			}

			void reassing_right_pointers(math_obj* pointer)
			{
				if (pointer->point_left != NULL)
				{
					pointer->point_left->var = pointer->var + 1;
					pointer->point_left->point_right = pointer;
					reassing_right_pointers(pointer->point_left);
				}
			}

			math_obj *var_list_to_arg(math_obj*var_list, int size)
			{
				math_obj* mass_arg = new math_obj[size]; 
				for (int count = 0; count < size; count ++)
				{
					mass_arg[count].copy(var_list);
					mass_arg[count].point_left = NULL;
					mass_arg[count].point_right = NULL;
					mass_arg[count].point_collar = NULL;
					mass_arg[count].var = 0;
					var_list = var_list->point_left;
				}
				return mass_arg;
			}

			math_obj *arg_to_var_list(math_obj*mass_arg, math_obj*var_list, int size)
			{
				math_obj*iter,*place;
				if (var_list == NULL)
				{
					var_list = new math_obj(&mass_arg[0]);
					mass_arg[0].point_collar = var_list;
					iter = var_list;
					var_list->var = 0;
					for (int count = 1; count < size; count++)
					{
						iter->point_left = new math_obj(&mass_arg[count]);
						iter->point_left->point_right = iter;
						iter->point_left->var = count;
						iter = iter->point_left;						
						mass_arg[count].point_collar = iter;
					}
				}
				else
				{
					iter = var_list_back_processing(var_list);//��������� �� ��������� ������� ������
					for (int count = 0; count < size; count++)
					{
						place = find_varbl_processing(var_list, &mass_arg[count]);
						if (place == NULL)
						{
							iter->point_left = new math_obj(&mass_arg[count]);							
							iter = iter->point_left;
							mass_arg[count].point_collar = iter;
						}
						else
						{
							mass_arg[count].point_collar = place;
						}
					}
				}
				return var_list;
			}

			math_obj *create_var_list(math_obj* pointer,math_obj *var_list)
			{			
				math_obj *iter = NULL; 		
				math_obj *place = NULL;
				
				
				for (int count = 0; count < pointer->var; count++)
				{
					if (pointer->point_right[count].type == varbl)
					{
						if (var_list == NULL)
						{
							var_list = new math_obj(&pointer->point_right[count]);
							pointer->point_right[count].point_collar = var_list;							
						}
						else
						{
							iter = var_list_back_processing(var_list);
							place = find_varbl_processing(var_list, &pointer->point_right[count]);
							if (place == NULL)
							{
								iter->point_left = new math_obj(&pointer->point_right[count]);								
								pointer->point_right[count].point_collar = iter->point_left;
							}
							else
							{
								pointer->point_right[count].point_collar = place;
							}
						}
					}
					else if (pointer->point_right[count].type == funct)
					{
						if ((pointer->point_right[count].prop == defnd)|| (pointer->point_right[count].prop == undef))
						{
							pointer->point_right[count].point_right = var_list_to_arg(pointer->point_right[count].point_collar, (int)pointer->point_right[count].var);
							var_list = arg_to_var_list(pointer->point_right[count].point_right, var_list, (int)pointer->point_right[count].var);
							pointer->point_right[count].prop = arg_v;
						}							
						else if (pointer->point_right[count].prop == arg_v)
						{
							var_list = create_var_list(&pointer->point_right[count], var_list);
						}
					}
				}
				return var_list;
			}

		public:
			//������� �����������
			math_obj()
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
			math_obj(wstring _name, double _num)
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
			math_obj(wstring _name, wstring _type, double _num)
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
			math_obj(wstring _name, wstring _type, wstring _prop, double _num)
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
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num)
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
			math_obj(wstring _name, double _num, math_obj *_pc)
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
			math_obj(wstring _name, wstring _type, double _num, math_obj *_pc)
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
			math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj *_pc)
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
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj *_pc)
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
			math_obj(wstring _name, double _num, math_obj * _pl, math_obj *_pr) {
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
			math_obj(wstring _name, wstring _type, double _num, math_obj * _pl, math_obj *_pr) {
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
			math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj * _pl, math_obj *_pr) {
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
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr) {
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
			math_obj(wstring _name, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
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
			math_obj(wstring _name, wstring _type, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
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
			math_obj(wstring _name, wstring _type, wstring _prop, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
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
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
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
			math_obj(wstring _name, wstring _type, wstring _prop, wstring _actn, double _num, double _arg, double _exp, math_obj * _pl, math_obj *_pr, math_obj *_pc) {
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
			math_obj(math_obj* var1)
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

			//����������
			~math_obj()
			{

			}

			//����� �����������.
			void copy(math_obj* ref) {
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
			math_obj *prioritize(int current_priority)
			{
				return prioritize_processing(point_collar, current_priority);
			}

			/*����� �������� ����������� �������, ���������� �� ������ �������� � ����������� ��.
			����������� ������ ������ �������� ������ ���������� ���������� � double var �������� �������� ������. */
			void arithmetic()
			{
				if (type == funct)
				{
					if (prop == arg_c)
						var = arithmetic_processing(point_left, point_right);
					else
					{
						//������ ������ �� ����, ����� �������� ������.
					}
				}
				else if ((type == cnst) || (type == exprs))
					var = arithmetic_processing(point_left, NULL);
				else
				{
					//������ ������ �� ����
				}
			}

			/*����� �������� ����������� �������, ���������� �� ������ �������� � ��������� ������.
			����� ������ ���������� ����� ������ ��� ������������� delete. ��� ���� ��������
			*/
			int tree_destruct()
			{
				int s = 0;
				if ((type == cnst) || (type == exprs))
				{
					s += tree_destruct_processing(point_left);
					point_left = NULL;
				}
				else if (type == funct)
				{
					if ((prop == undef) || (prop == defnd))
					{
						s += tree_destruct_processing(point_left);
						point_left = NULL;
						s += tree_destruct_processing(point_collar);
						point_collar = NULL;
					}
					else if ((prop == arg_c) || (prop == arg_v))
					{
						//����� ���������� ������ ������
						if (var>1)
							delete[]point_right;
						else
							delete point_right;
					}
				}
				else if (type == equat)
				{
					//��������
				}
				return s;
			}

			math_obj *find_varbl(math_obj*pointer)
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
			math_obj *var_list_back()
			{
				return var_list_back_processing(point_collar);
			}

			/*����� ���������� ������ ���������� (���� �� ���� �� ������� ����������) ������� pointer �� ������� ���������� ������ �������.
			����� ����������:
			0 - ������ ���������� pointer ��������� ������ � ������ ���������� ������ �������.
			����� ������ ������������� ����� - ����� �������� ������� �������.
			*/
			int var_list_compare(math_obj * pointer)
			{
				int count_var = 0;
				int temp = 0;
				//��������������� ������� � ����������� (����������� � ���)
				if (pointer->prop == arg_v) 
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
					math_obj * iter = pointer->point_collar;
					do
					{
						if (find_varbl_processing(point_collar, iter) == NULL)
							count_var++;
						iter = iter->point_left;
					} while ((iter != NULL)&(iter->var != 0));
				}
				return count_var;
			}

			/*����� ����������� point_right ������ ������� ������ ���������� � ������ ������ � ����� � ����������� ����� ���������� �������.
			�������� ������ ��� ������� � ����������� ������� ����������. ���������� ������ ���������
			*/
			void arg_copy_plus_1()
			{
				math_obj * ar = new math_obj[var + 1];
				if (point_right == NULL)
				{
					//���� ������� ���������� ���������� ��� (� ������ ������� ����������� ������ ����������)
					math_obj *iter = point_collar;
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

			

			/*����� ��������� ����������� ������ ���������� �� ��������*/
			math_obj * sort_list(math_obj * var_list)
			{
				stack <math_obj*> sorting;
				math_obj * temp_var;
				while (var_list != NULL)
				{
					sorting.push(var_list);
					sorting.top()->var = 0;
					var_list = var_list->point_left;
					sorting.top()->point_left = NULL;
					if (sorting.size() > 1)
					{
						temp_var = sorting.top();
						sorting.pop();
						while ((sorting.size() >= 1) && (temp_var->var == sorting.top()->var))
						{
							temp_var = merge_lists(temp_var, sorting.top());
							temp_var->var += 1;
							sorting.pop();
						}
						sorting.push(temp_var);
					}
				}
				if (sorting.size() > 1)
				{
					temp_var = sorting.top();
					sorting.pop();
					while (sorting.size() >= 1)
					{
						temp_var = merge_lists(temp_var, sorting.top());
						temp_var->var += 1;
						sorting.pop();
					}
					temp_var->var = 0;
					reassing_right_pointers(temp_var);
					return temp_var;
				}				
				else
				{
					return NULL;
				}
				
			}

			/*����� ����������� �������. ������ ������ ����������, �� ������� ������� �������, ����������� �� ������ ���������� � ���������� ������� pointer.*/
			void define_funct(math_obj *pointer)
			{
				math_obj* temp_pointer;
				if (pointer->prop == defnd)
				{
					pointer->point_right = var_list_to_arg(pointer->point_collar, (int)pointer->var);
					point_collar = arg_to_var_list(pointer->point_right,NULL, (int)pointer->var);
					temp_pointer = var_list_back_processing(point_collar);
					point_collar->point_right = temp_pointer;
					temp_pointer->point_left = point_collar;
					var = temp_pointer->var + 1;
					point_left = pointer;
					point_left->prop = arg_v;
				}
				else if (pointer->prop == undef)
				{
					//������� ������: f=x+y*6..., ��� ��� ��������� ��������� (x,y,...) - ���������� � ��� ����������
					point_collar = sort_list(pointer->point_collar);
					temp_pointer = var_list_back_processing(point_collar);
					var = temp_pointer->var + 1;
					temp_pointer->point_left = point_collar;
					point_collar->point_right = temp_pointer;
					point_left = pointer->point_left;							
					delete pointer;
				}
				else if (pointer->prop == arg_v)
				{
					temp_pointer = create_var_list(pointer, NULL);
					point_collar = sort_list(temp_pointer);
					temp_pointer = var_list_back_processing(point_collar);
					var = temp_pointer->var + 1;
					temp_pointer->point_left = point_collar;
					point_collar->point_right = temp_pointer;
					point_left = pointer;
				}
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
			math_obj *point_left;		//����� �����
			math_obj *point_right;		//������ �����
			math_obj *point_collar;	//��������
		};
	}
};