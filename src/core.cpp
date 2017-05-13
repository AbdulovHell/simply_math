#include "core.h"

namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;
		using namespace Project::IO;

		//��������� �������� ������
		vector<math_obj*>* general_var_const;
		math_obj* temporary_variable;

		void ClearGeneral() {
			general_var_const->clear();
			delete general_var_const;
		}
		//������������� ������� ��������, ����������, ������� � ���������
		void Init() {
			ProjectError::Init();
			math_obj* temp;
			general_var_const = new vector<math_obj*>;
			general_var_const->push_back(new math_obj(L"pi", cnst, fundm, L"0", 3.1415926535897932384626433832));
			general_var_const->push_back(new math_obj(L"e", cnst, fundm, L"0", 2.7182818284590452353602874713527));
			general_var_const->push_back(new math_obj(L"i", cnst, fundm, L"0", 0));
			temporary_variable = new math_obj(L"temporary_variable", varbl, servc, L"0", 0);
			//�����
			{
				general_var_const->push_back(new math_obj(L"minus", funct, servc, L"", 1, new math_obj(temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
				temp->point_left = new math_obj(L"", mltpl, L"", L"", 0, new math_obj(L"0", numbr, real, -1), temp->point_collar, temp);
			}

			//������ (���� ����������)
			{
				general_var_const->push_back(new math_obj(L"root", funct, servc, L"", 0, new math_obj(temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
			}
			//������ (������ �� �������� �����)
			{
				general_var_const->push_back(new math_obj(L"abs", funct, servc, L"", 0, new math_obj(temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new math_obj(temp);
				general_var_const->back()->point_left->point_collar->point_collar = general_var_const->back()->point_left;
				general_var_const->back()->point_left->point_right = new math_obj(L"", power, L"", L"", 0, general_var_const->back()->point_collar, new math_obj(L"0", 2), NULL);
			}
			//����
			{
				temp = general_var_const->back();
				general_var_const->push_back(new math_obj(L"sgn", funct, servc, L"", 0, new math_obj(temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new math_obj(L"", divis, L"", L"", 0, general_var_const->back()->point_collar, new math_obj(temp), general_var_const->back());
				general_var_const->back()->point_left->point_right->point_right = general_var_const->back()->point_collar;
				general_var_const->back()->point_left->point_right->point_collar = general_var_const->back()->point_collar;
			}

		}

		//������� �������� �� ������� general_var_const � ���� ������ � ������ ������
		math_obj* run_vector(wstring _pv)
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

		math_obj* filling_vector(wchar_t* strPtr, wchar_t*ePtr, math_obj* c_e, int brakets)
		{
			wchar_t* pDest = strPtr;
			wchar_t* endPtr = ePtr;
			unsigned int count, comma, count_var;
			math_obj *high_pointer = NULL; //��������� ���������� ��������
			math_obj *low_pointer = NULL; //��������� ���������� ���������/����������/�-���/���������
			math_obj *temp_pointer = NULL;
			math_obj *multiple_var = NULL;
			wchar_t* temp;
			wstring name;
			math_obj* current_element = c_e;
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
						//������ ����� ������ ������� ����������� ������� � ����������  solve, � ��� ��� �������� ��� ��������� � ������� ��������� ��� �������� ������
						if (current_element->actn != write)
							current_element->actn = solve;
						else
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
							return NULL;
						}
						pDest = endPtr + 1;
					}
					else
					{
						if (current_element->type == cnst)
						{
							temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer == NULL)
								return temp_pointer;
							if (temp_pointer->type == funct)
							{
								current_element->define_funct(temp_pointer);
								current_element->type = funct;
								current_element->prop = defnd;
								current_element->actn = write;
								delete temp_pointer;
							}
							else if (temp_pointer->type == varbl)
							{
								if (temp_pointer->prop == undef)
								{
									temp_pointer->prop = defnd;
									general_var_const->push_back(temp_pointer);
								}
								current_element->point_collar = temp_pointer;
								current_element->point_collar->point_collar = current_element;
								current_element->point_collar->var = 0;
								current_element->point_collar->point_left = current_element->point_collar;
								current_element->point_collar->point_right = current_element->point_collar;
								current_element->point_left = current_element->point_collar;
								current_element->var = 1;
								current_element->type = funct;
								current_element->prop = defnd;
								current_element->actn = write;
							}
							else if (temp_pointer->type == exprs)
							{
								//��� ��������� ����������. ���� ������ ���� �=�+1, ��� � ���� �������� ��� ���������, ��������� ������� ��������� �� ������ ��������� � ���������, � ����� - ������� ������ c.
								current_element->point_left = temp_pointer->point_left;
								current_element->prop = defnd;
								current_element->actn = write;
								delete temp_pointer;
							}
							else if (temp_pointer->type == cnst)
							{
								current_element->var = temp_pointer->var;
								current_element->type = defnd;
								current_element->actn = write;
								delete temp_pointer;
							}
							pDest = endPtr + 1;
						}
						else if (current_element->type == varbl)
						{
							temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer == NULL)
								return temp_pointer;
							//��� ���������� ��������������� ��������� � �������� �������. ����� ���������� ��������, �.�. �������� ��� ������ �� ���� ������� � ����� ������.
							if (temp_pointer->type == funct)
							{
								low_pointer = temp_pointer->find_varbl(current_element);
								if (low_pointer == NULL)
								{
									//���� ���������� ����� �� ����� �� ���������� ������ - ��������������� ���������� � �������. �-� : y=2*x+1
									current_element->define_funct(temp_pointer);
									current_element->type = funct;
									current_element->prop = defnd;
									current_element->actn = write;
									delete temp_pointer;
								}
								else
								{
									delete current_element;
									//���� ���������� ����� �� ����� ���������� ��� �� ������ - ���������. �-� : x=2*x+1
									current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, low_pointer, temp_pointer, temp_pointer->point_collar);
								}
							}
							else if (temp_pointer->type == varbl)
							{
								if (temp_pointer->name == current_element->name)
								{
									//��� ���������� �=�, ��� � - ����������. �� ���� ��� ��� ����� ������� � ��� �������� �������
								}
								else
								{
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(temp_pointer);
									}
									current_element->point_collar = temp_pointer;
									current_element->point_collar->point_collar = current_element;
									current_element->point_collar->var = 0;
									current_element->point_collar->point_left = current_element->point_collar;
									current_element->point_collar->point_right = current_element->point_collar;
									current_element->point_left = current_element->point_collar;
									current_element->var = 1;
									current_element->type = funct;
									current_element->prop = defnd;
									current_element->actn = write;
								}
							}
							else if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
							{
								/*������ �������� ��������������� ���������� � ��������� �������� ������ ���� ��� ���������� �� ������������ �� � ����� �������/���������.
								��������� �� ������ ����� ���������� ���������� ��� ��������� �������� (�� � ������� �������) ���������� - ������ ������ ����� ��������� �������.
								*/
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::VARBL_BLOCKED);
								return NULL;
							}
							pDest = endPtr + 1;
						}
						else if (current_element->type == funct)
						{
							if (current_element->prop == defnd)
							{
								temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
								if (temp_pointer == NULL)
									return temp_pointer;
								//������ ������ f=... ��� f - ������� ����������� �������.
								//��������, ��� ���� ����� ����� ��������� ������� (������� �� ����� ����������) - ��������������� f ����� ���������� (� ������ �����������)
								//���� ����� ����� ��������� ��� ��������� - ��������� ���� f(x) = const
								if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
								{
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, high_pointer->var, high_pointer, temp_pointer, high_pointer->point_collar);
								}
								else if (temp_pointer->type == varbl)
								{
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(temp_pointer);
									}
									temp_pointer->var = 0;
									temp_pointer->point_left = temp_pointer;
									temp_pointer->point_right = temp_pointer;
									temp_pointer->point_collar = current_element;
									current_element->point_collar = temp_pointer;
									current_element->point_left = current_element->point_collar;
									current_element->point_right = NULL;
									current_element->var = 1;
								}
								else if (temp_pointer->type == funct)
								{
									if (temp_pointer->prop == arg_c)
									{
										high_pointer = current_element;
										current_element = new math_obj(L"", equat, unslv, write, high_pointer->var, high_pointer, temp_pointer, high_pointer->point_collar);
									}
									else
									{
										current_element->define_funct(temp_pointer);
										current_element->actn = write;
									}
								}
								pDest = endPtr + 1;
							}
							else if (current_element->prop == undef)
							{
								if (current_element->actn == write)
								{
									//����� ������ ������� � ���������� �� ������ ���������� ������ �������
									temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", funct, undef, current_element->var, current_element->point_collar), brakets + brakets_counter);
									if (temp_pointer == NULL)
										return temp_pointer;
									//���� ������� ��� ������ ����� ������� - ������
									if (temp_pointer->type != funct)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
										return NULL;
									}
									else
									{
										//�������� ��������� ����� �� ������ ��������
										current_element->point_left = temp_pointer->point_left;
										current_element->prop = defnd;
									}
								}
								else
								{
									/*��� ������ ��������� ���� ���������� ����� ������ ���������� � ����� � ������ ������. ��������� ��� ����� ��������, ��� ��� ������ �������,
									�������� ������ �������� �������� �� ����������� ������ ���������
									*/
									temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
									if (temp_pointer == NULL)
										return temp_pointer;
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, high_pointer->var, high_pointer, temp_pointer, high_pointer->point_collar);
								}
								pDest = endPtr + 1;
							}
							else if (current_element->prop == arg_c)
							{
								temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
								if (temp_pointer == NULL)
									return temp_pointer;
								if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
									return NULL;
								}
								else if (temp_pointer->type == varbl)
								{
									if (temp_pointer->prop == undef)
									{
										temp_pointer->prop = defnd;
										general_var_const->push_back(temp_pointer);
									}
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, 1, high_pointer, temp_pointer, temp_pointer);
								}
								else if (temp_pointer->type == funct)
								{
									if (temp_pointer->prop == arg_c)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
										return NULL;
									}
									else
									{
										high_pointer = current_element;
										current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, high_pointer, temp_pointer, temp_pointer->point_collar);
									}
								}
								pDest = endPtr + 1;
							}
							else if (current_element->prop == arg_v)
							{
								count_var = 0;
								for (count = 0; count < current_element->var; count++)
								{
									if (current_element->point_collar[count].type == varbl)
										count_var++;
								}
								if ((current_element->name.size() != 0) && (count_var == current_element->var))
								{
									current_element->define_var_list();
									current_element->prop = undef;
									current_element->actn = write;									
								}
								else
								{
									temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
									if (temp_pointer == NULL)
										return temp_pointer;
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, high_pointer, temp_pointer, temp_pointer->point_collar);
									pDest = endPtr + 1;
								}
							}							
						}
						else if (current_element->type == exprs)
						{
							temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer == NULL)
								return temp_pointer;
							//����� - ����� ��������� - ������ ������� => ���������
							if (temp_pointer->type == funct)
							{
								if (temp_pointer->prop == arg_c)
								{
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
									return NULL;
								}
								else
								{
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, high_pointer, temp_pointer, temp_pointer->point_collar);
								}
							}
							else if (temp_pointer->type == varbl)
							{
								if (temp_pointer->prop == undef)
								{
									temp_pointer->prop = defnd;
									general_var_const->push_back(temp_pointer);
								}
								high_pointer = current_element;
								current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, high_pointer, temp_pointer, temp_pointer);
							}
							else if ((temp_pointer->type == cnst) || (temp_pointer->type == exprs))
							{
								//������ ���� 2+3=7. ���� �� ���� ��� ����������� �� �����
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
								return NULL;
							}
							pDest = endPtr + 1;
						}
					}				
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
						current_element->point_left = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, current_element);
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
							high_pointer->point_right = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new math_obj(L"+", addit, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 1);
								high_pointer->point_right->point_collar = new math_obj(L"+", addit, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
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
						current_element->point_left = new math_obj(L"*", mltpl, brakets_counter + brakets, low_pointer, NULL, current_element);
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
							high_pointer->point_right = new math_obj(L"*", mltpl, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 2))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new math_obj(L"*", mltpl, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 2);
								high_pointer->point_right->point_collar = new math_obj(L"*", mltpl, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
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
						current_element->point_left = new math_obj(L"/", divis, brakets_counter + brakets, low_pointer, NULL, current_element);
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
							high_pointer->point_right = new math_obj(L"/", divis, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 3))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new math_obj(L"/", divis, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 3);
								high_pointer->point_right->point_collar = new math_obj(L"/", divis, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
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
						current_element->point_left = new math_obj(L"^", power, brakets_counter + brakets, low_pointer, NULL, current_element);
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
							high_pointer->point_right = new math_obj(L"^", power, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
						}
						//���� ��������� ���������� ������������ �������� !������! ��� ��������� �������
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 4))
							{
								//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
								high_pointer = new math_obj(L"^", power, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//�������� ���������� ��������� �������� -> �� ����� ��������
								current_element->point_left->point_collar = high_pointer;
								//��������� ����� ������� ��������� �� ��������� ��������
								current_element->point_left = high_pointer;
							}
							else
							{
								high_pointer = high_pointer->prioritize(brakets_counter + brakets + 4);
								high_pointer->point_right->point_collar = new math_obj(L"^", power, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
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
						current_element->point_left = new math_obj(general_var_const->at(3));
						current_element->point_left->prop = arg_c;

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
							current_element->point_left = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, current_element);
							high_pointer = current_element->point_left;
							high_pointer->point_right = new math_obj(general_var_const->at(3));

							high_pointer->point_right->prop = arg_c;

							low_pointer = high_pointer->point_right;
						}
						//���� ����� ���� ������ ��������
						else
						{
							//���� ��������� ���������� ������������ �������� !������! ��� �����  ���������� �������
							if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
							{
								//���������� ��������, ����� ����� -> �� ���������� �����, �������� �� ���������� ��������
								high_pointer->point_right = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, high_pointer);
								high_pointer = high_pointer->point_right;                 //������� ��������� -> �� ��������� ��������
								high_pointer->point_right = new math_obj(general_var_const->at(3));
								high_pointer->point_right->prop = arg_c;
								low_pointer = high_pointer->point_right;
							}

							else
							{
								if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
								{
									//���������� �������� ��� ����� �����, ����� ����� -> �� ���������� ������������ ��������, �������� - �� ������� ���������
									high_pointer = new math_obj(L"+", addit, brakets_counter + brakets, current_element->point_left, NULL, current_element);
									//�������� ���������� ��������� �������� -> �� ����� ��������
									current_element->point_left->point_collar = high_pointer;
									//��������� ����� ������� ��������� �� ��������� ��������
									current_element->point_left = high_pointer;
									high_pointer->point_right = new math_obj(general_var_const->at(3));
									high_pointer->point_right->prop = arg_c;
									low_pointer = high_pointer->point_right;
								}
								else
								{
									high_pointer = high_pointer->prioritize(brakets_counter + brakets + 1);
									high_pointer->point_right->point_collar = new math_obj(L"+", addit, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
									high_pointer->point_right = high_pointer->point_right->point_collar;
									high_pointer = high_pointer->point_right;
									high_pointer->point_right = new math_obj(general_var_const->at(3));
									high_pointer->point_right->prop = arg_c;
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
						temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
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
										general_var_const->push_back(new math_obj(temp_pointer));
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
									current_element->point_collar = new math_obj(temporary_variable);
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
											general_var_const->push_back(new math_obj(temp_pointer));
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
										current_element->point_collar = new math_obj(temporary_variable);
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
												general_var_const->push_back(new math_obj(temp_pointer));
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
										general_var_const->push_back(new math_obj(temp_pointer));
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
									current_element->point_collar = new math_obj(temporary_variable);
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
										temp_pointer->point_left = new math_obj(temporary_variable);
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
				{
					// ���� ������ ����� ������ � ������ ���������
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//�������� �������� ������ � ������ �����, �������� -> ���������
						current_element->point_left = new math_obj(L"", numbr, real, wcstod(pDest, &pDest), current_element);
						//��� ��������� -> �� �����, �������� ��� �������� ������� ��� ������ ��������
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					//����� ������ � ������ ������ ����� �����
					else if (high_pointer == low_pointer)
					{
						if (high_pointer->name == L"minus")
						{
							high_pointer->point_right = new math_obj(L"", numbr, real, wcstod(pDest, &pDest), low_pointer);
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
							low_pointer->point_right = new math_obj(L"", numbr, real, wcstod(pDest, &pDest), low_pointer);
						}
						else
						{
							high_pointer->point_right = new math_obj(L"", numbr, real, wcstod(pDest, &pDest), high_pointer);
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
								current_element->actn = write;
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
									current_element->prop = undef; //������� ����� �������  undef, �� ��� ���� �� ����� ��� � �� ���� ���
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
									current_element->prop = undef; //������� ����� �������  undef, �� ��� ���� �� ����� ��� � �� ���� ���
									current_element->actn = write;  //�� ����������
									for (count = 0; count < comma + 1; count++)
									{
										temp = wcspbrk(pDest + 1, L",)");
										name.assign(pDest + 1, temp);
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new math_obj(temp_pointer));
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
								general_var_const->push_back(new math_obj(name, varbl, defnd, 0));
								//����� ���������� � ���������� �� �������
								current_element->point_collar = new math_obj(general_var_const->back());
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
										current_element->point_left = new math_obj(high_pointer);
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
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new math_obj(temp_pointer));
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
										current_element->prop = undef; //������� ����� �������  undef, �� ��� ���� �� ����� ��� � �� ���� ���
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
								current_element->point_left = new math_obj(high_pointer);
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
									current_element->prop = undef; //������� ����� �������  undef, �� ��� ���� �� ����� ��� � �� ���� ���		
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
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
										if (temp_pointer == NULL)
											return temp_pointer;
										if (temp_pointer->type == varbl)
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new math_obj(temp_pointer));
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
									current_element->prop = undef; //������� ����� �������  undef, �� ��� ���� �� ����� ��� � �� ���� ���
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
								if (current_element->type == exprs)
								{
									current_element->type = funct;
									current_element->prop = undef;
									//���������� ����� ����������
									current_element->var = 1;
									//����� ���������� � ���������� �� �������
									current_element->point_collar = new math_obj(high_pointer);
									current_element->point_left = current_element->point_collar;
									current_element->point_left->point_collar = current_element;
									high_pointer = current_element->point_left;
									low_pointer = high_pointer;
								}
								else if (current_element->type == funct)
								{
									//������, ����� ������� ��������� - �������, ������� ������� ��� ������ ����������
									temp_pointer = current_element->find_varbl(high_pointer);
									if (temp_pointer == NULL)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									current_element->point_left = temp_pointer;
									high_pointer = current_element->point_left;
									low_pointer = high_pointer;
								}
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
										current_element->prop = arg_v;
										current_element->var = 1; // ���� ����������
										current_element->point_right = new math_obj(high_pointer); //��������� ������� - �������� �������
										//����� ��������� ���������� � ���������� �� �������
										current_element->point_collar = new math_obj(temporary_variable);
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
										temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
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
												current_element->point_left = new math_obj(high_pointer);
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
													current_element->point_left = new math_obj(high_pointer);
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
													current_element->prop = arg_v;
													current_element->var = 1; // ���� ����������
													current_element->point_right = new math_obj(high_pointer); //��������� ������� - �������� �������
																												//����� ��������� ���������� � ���������� �� �������
													current_element->point_collar = new math_obj(temporary_variable);
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
												general_var_const->push_back(new math_obj(temp_pointer));
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
												current_element->prop = arg_v;
												current_element->var = 1; // ���� ����������
												current_element->point_right = new math_obj(high_pointer); //��������� ������� - �������� �������
																											//����� ��������� ���������� � ���������� �� �������
												current_element->point_collar = new math_obj(temporary_variable);
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
										multiple_var = new math_obj[comma + 1];
										count_var = 0;
										for (count = 0; count < comma + 1; count++)
										{
											temp = wcspbrk(pDest + 1, L",)");
											name.assign(pDest + 1, temp);
											temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
											if (temp_pointer == NULL)
												return temp_pointer;
											//��������� ������ ����������
											if (temp_pointer->type == varbl)
											{
												count_var++;
												if (temp_pointer->prop == undef)
												{
													temp_pointer->prop = defnd;
													general_var_const->push_back(new math_obj(temp_pointer));
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
												current_element->prop = arg_v;
												current_element->var = 1; // ���� ����������
												current_element->point_right = new math_obj(high_pointer); //��������� ������� - �������� �������
																											//����� ��������� ���������� � ���������� �� �������
												current_element->point_collar = new math_obj(temporary_variable);
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
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->point_left = new math_obj(high_pointer);
												current_element->point_left->point_right = multiple_var;
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

									else
									{
										current_element->copy(high_pointer);
										for (count = 0; count < comma + 1; count++)
										{
											temp = wcspbrk(pDest + 1, L",)");
											name.assign(pDest + 1, temp);
											temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, L"", 0, current_element), brakets + brakets_counter);
											if (temp_pointer == NULL)
												return temp_pointer;
											if (temp_pointer->type == varbl)
											{
												if (temp_pointer->prop == undef)
												{
													temp_pointer->prop = defnd;
													general_var_const->push_back(new math_obj(temp_pointer));
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
										current_element->prop = undef; //������� ����� �������  undef, �� ��� ���� �� ����� ��� � �� ���� ���
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
								current_element->prop = arg_v;
								current_element->point_right = new math_obj(high_pointer); //�������� ����� ������������� �������, ������� ������� �� �����-���� ����������,
																							//������� �������� � ���� ������ (�����������) �������. ��� ���� ���� ��������, ���
																							//� ��������� ��������� ��������� ������� (f+g), �� �������, ������ �� ����������
																							//����� ��������� �������� ���������� ����������, 
																							//���� ���� ��� ������� � � ��������� ������� �� ����� � ��� �� ����������.
								//����� ��������� ���������� � ���������� �� �������, ������� ������ ����� ����
								current_element->point_collar = new math_obj(temporary_variable);
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

						//����� ������ ����� ���� ������ ��� �������� - ������� ������� ���� �������, ���� ���������, ������ ������  undef.
						//���� �� ������ �� ���� ������� ������� � ����� ������
						if (low_pointer == NULL)
						{
							if ((temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{
								if (current_element->type == funct)
								{
									//���� ������� ������� - �������, �� �������� ������� ����� ���������� � ������ ���������. 
									if (current_element->point_collar->point_right != NULL)
									{
										//������ ������ ���������� ������� => ����� ���������� - ������
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									//������ �� ������� - ����� �������� ������ ����������
									else
									{
										general_var_const->push_back(new math_obj(name, varbl, defnd, 0));
										temp_pointer = current_element->var_list_back();
										temp_pointer->point_left = new math_obj(general_var_const->back());
										temp_pointer->point_left->point_right = temp_pointer;
										temp_pointer->point_left->point_collar = current_element;
										temp_pointer->point_left->var = current_element->var;
										current_element->var += 1;
										//��������� ������ �� � ������ ������ - ������ ����� ����� �����-���� ��������. 
										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//����� ����� ������
										{
											high_pointer->point_right->point_right = temp_pointer->point_left;
											high_pointer->point_right->prop = arg_v;
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
									// ��������� �  undef ������� 
									current_element->type = funct;
									current_element->prop = undef;
									general_var_const->push_back(new math_obj(name, varbl, defnd, 0));
									//����� ���������� � ���������� �� �������
									current_element->point_collar = new math_obj(general_var_const->back());
									current_element->point_collar->point_collar = current_element;
									current_element->var = 1;//���� ����������
															 //��� �������� ����� ���������� - � �� ����� �����))
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = current_element->point_collar;
										high_pointer->point_right->prop = arg_v;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}
							}
							else if ((*temp == '(') || (*temp == ')') || (*temp == ','))
							{
								//������ - ����� ���������� ����� ������������� ������. ������ �� ������ ������������� ������, �������� ����� ������ ��� ������ ��� ��������� ��� ����� ���������� ������������.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}		
						}
						//������ ������� ������� � ����������� ������ - ����������
						else if (low_pointer->type == varbl)
						{
							if ((temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{
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
												high_pointer->point_right->prop = arg_v;
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
										temp_pointer->point_left = new math_obj(low_pointer);
										temp_pointer->point_left->point_right = temp_pointer;
										temp_pointer->point_left->point_collar = current_element;
										temp_pointer->point_left->var = current_element->var;
										current_element->var += 1;
										//��������� ������ �� � ������ ������ - ������ ����� ����� �����-���� ��������. 
										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//����� ����� ������
										{
											high_pointer->point_right->point_right = temp_pointer->point_left;
											high_pointer->point_right->prop = arg_v;
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
											high_pointer->point_right->prop = arg_v;
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
									current_element->point_collar = new math_obj(low_pointer);
									current_element->point_collar->point_collar = current_element;
									current_element->var = 1;//���� ����������
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = current_element->point_collar;
										high_pointer->point_right->prop = arg_v;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
								}
							}
							else if ((*temp == '(')|| (*temp == ')')|| (*temp == ','))
							{
								//������ - ����� ���������� ����� ������������� ������. ������ �� ������ ������������� ������, �������� ����� ������ ��� ������ ��� ��������� ��� ����� ���������� ������������.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							
						}
						//������ ������� ������� � ����������� ������ - ��������� - ������ ���������� � � ���������
						else if (low_pointer->type == cnst)
						{
							if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
							{
								high_pointer->point_right->point_right = new math_obj(low_pointer);
							}
							else
							{
								high_pointer->point_right = new math_obj(low_pointer);
							}
							low_pointer = high_pointer->point_right;
						}
						//������ ������� ������� � ����������� ������ - ������� 
						else if (low_pointer->type == funct)
						{
							if ((*temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{
								count_var = -1;
							}
							else if (*temp = '(')
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
								multiple_var = new math_obj[comma + 1];
								count_var = 0;
								for (count = 0; count < comma + 1; count++)
								{
									temp = wcspbrk(pDest + 1, L",)");
									name.assign(pDest + 1, temp);
									temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new math_obj(L"", exprs, undef, 0, current_element), brakets + brakets_counter);
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
										//��� � �������� ��������� ���������� ���������� �������, ������� ����  defnd, ����  undef
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
										else //��� ����� ���������������  defnd ������� � ����� �����������										
											multiple_var[count].copy(temp_pointer);
									}
									pDest = temp;
								}
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
									temp_pointer = new math_obj(low_pointer);
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
									temp_pointer = new math_obj(low_pointer);
									if (count_var != -1)
									{
										temp_pointer->point_right = multiple_var;
										multiple_var = NULL;
										temp_pointer->prop = arg_v;//��������� ��� ����� ���������� ���� ����������
									}
									//���� ������ ���������� �� �������
									if (current_element->point_collar->point_right == NULL)
									{
										current_element->arg_copy_plus_1();
										current_element->point_right[(int)current_element->var - 1] = temp_pointer;
										temp_pointer = NULL;
										temp_pointer = current_element->var_list_back();
										temp_pointer->point_left = new math_obj(temporary_variable);
										temp_pointer->point_left->point_right = temp_pointer;
										temp_pointer->point_left->var = current_element->var - 1;

										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
										{
											high_pointer->point_right->point_right = temp_pointer->point_left;
											high_pointer->point_right->prop = arg_v;
										}
										else
										{
											high_pointer->point_right = temp_pointer->point_left;
										}
										low_pointer = high_pointer->point_right;

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
												high_pointer->point_right->prop = arg_v;
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
									temp_pointer = new math_obj(low_pointer);
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
									temp_pointer = new math_obj(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_v;//��������� ��� ����� ���������� ���� ����������
									current_element->type = funct;
									current_element->prop = arg_v;
									current_element->point_right = temp_pointer; //�������� ����� ������������� �������, ������� ������� �� �����-���� ����������,
																								//������� �������� � ���� ������ (�����������) �������. ��� ���� ���� ��������, ���
																								//� ��������� ��������� ��������� ������� (f+g), �� �������, ������ �� ����������
																								//����� ��������� �������� ���������� ����������, 
																								//���� ���� ��� ������� � � ��������� ������� �� ����� � ��� �� ����������.
																								//����� ��������� ���������� � ���������� �� �������, ������� ������ ����� ����
									current_element->point_collar = new math_obj(temporary_variable);
									current_element->point_collar->var = 0; //������ ��� �������������)
									current_element->point_collar->point_collar = current_element;

									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = current_element->point_collar;
										high_pointer->point_right->prop = arg_v;
									}
									else
									{
										high_pointer->point_right = current_element->point_collar;
									}
									low_pointer = high_pointer->point_right;
									current_element->var = 1; //���������� ����������

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
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, math_obj* _current_element)
		{
			math_obj* CE = filling_vector(_pDest, _endPtr, _current_element, 0);
			wstring output;
			math_obj* temp;
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
				CE->actn = solve;
				general_var_const->push_back(CE);
			}
			else if (CE->type == funct)
			{
				if (CE->actn == solve)
				{
					if (CE->prop == arg_c)
					{
						CE->arithmetic();
						output = to_string(CE->var, var_type::FRACTIONAL, 2);
						CE->tree_destruct();
						delete CE;
					}
					else
					{
						output = CE->expresion(1);
					}
				}
				else if (CE->actn == write)
				{
					//����� ��, ������� ������� �� ��������� ���� �� ������. ������ ������� - ��������. �������� � ��������� ����� ������ ���������� ������� ������� 
					//�� ������������. ����� � �� � ���������, ������ ��� ����� ������� ��������� ����������

					if (CE->name.size() != 0)
					{
						CE->actn = L"";
						temp = run_vector(CE->name);
						if (temp == NULL)
						{
							general_var_const->push_back(CE);
						}
						else
						{
							temp->tree_destruct();
							for (int count = 0; count < general_var_const->size(); count++)
							{
								if (temp == general_var_const->at(count))
								{
									general_var_const->at(count) = CE;
									delete temp;
									break;
								}
							}
						}
					}
					else
					{
						//������
					}
				}
				else
				{
					//���� ������� �� ������. �� ������ ������ ������� ��������
					CE->tree_destruct();
					delete CE;
				}
			}
			else if (CE->type == cnst)
			{
				if (CE->actn == solve)
				{
					//����� 
					output = to_string(CE->var, var_type::FRACTIONAL, 2);

				}
				else if (CE->actn == write)
				{
					if (CE->prop == undef)
					{
						CE->tree_destruct();
						delete CE;
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
						ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
						ProjectError::GetProjectLastError(err);
						return err->GetErrorWStr();
					}
					else if (CE->prop == defnd)
					{
						CE->arithmetic();
						CE->tree_destruct();
						CE->actn = L"";
						temp = run_vector(CE->name);
						if (temp == NULL)
						{
							general_var_const->push_back(CE);
						}
						else
						{
							temp->tree_destruct();
							for (int count = 0; count < general_var_const->size(); count++)
							{
								if (temp == general_var_const->at(count))
								{
									general_var_const->at(count) = CE;
									delete temp;
									break;
								}
							}
						}
					}
				}
			}
			else if (CE->type == varbl)
			{
				delete CE;
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


            general_var_const->back();

			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer	
			return analized_output(point_start, point_end, new math_obj(L"", exprs, undef, 0));
		}
	}
}
