#include "core.h"

namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;
		using namespace Project::IO;

		//��������� �������� ������
		vector<math_obj*>* general_var_const;
		math_obj* temp_var;

		void ClearGeneral() {
			general_var_const->clear();
			delete general_var_const;
		}
		//������������� ������� ��������, ����������, ������� � ���������
		void Init() {
			ProjectError::Init();
			math_obj* temp;
			general_var_const = new vector<math_obj*>;
			general_var_const->push_back(new math_obj(L"pi", cnst, fundm, 3.1415926535897932384626433832));
			general_var_const->push_back(new math_obj(L"e", cnst, fundm, 2.7182818284590452353602874713527));
			general_var_const->push_back(new math_obj(L"i", cnst, fundm, 0));
			temp_var = new math_obj(L"", varbl, servc, L"", 0,NULL,NULL,NULL);
			
			//������ 
			{
				temp = new math_obj((size_t)0);
				temp_var->prop = L"root_";
				temp->vector_push_back(new math_obj(temp_var));
				temp_var->var += 1;
				temp->vector_push_back(new math_obj(temp_var));
				temp_var->var = 0;
				temp_var->prop = servc;
				temp->double_lincked_vector();				
				general_var_const->push_back(new math_obj(L"root", funct, servc, L"", 2, NULL, NULL, temp->point_left));
				delete temp;
				temp = general_var_const->back();
				temp->close_list();
				temp->point_collar->point_collar = temp;
				temp->point_collar->point_right->point_collar = temp;
				temp->point_left = new math_obj(L"^", power, L"", L"", 0, temp->point_collar, new math_obj(L"/", divis, L"", L"", 0, new math_obj(L"0", numbr, real, 1), temp->point_collar->point_right, temp->point_left), temp);
				temp->point_left->point_right->point_collar = temp->point_left;
			}
			//������ (������ �� �������� �����)
			{
				general_var_const->push_back(new math_obj(L"abs", funct, servc, L"", 0,NULL,NULL, new math_obj(temp_var)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new math_obj(temp);
				general_var_const->back()->point_left->point_collar->point_collar = general_var_const->back()->point_left;
				general_var_const->back()->point_left->point_right = new math_obj(L"", power, L"", L"", 0, general_var_const->back()->point_collar, new math_obj(L"0", 2), NULL);
			}
			//����
			{
				temp = general_var_const->back();
				general_var_const->push_back(new math_obj(L"sgn", funct, servc, L"", 0, NULL, NULL, new math_obj(temp_var)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new math_obj(L"", divis, L"", L"", 0, general_var_const->back()->point_collar, new math_obj(temp), general_var_const->back());
				general_var_const->back()->point_left->point_right->point_right = general_var_const->back()->point_collar;
				general_var_const->back()->point_left->point_right->point_collar = general_var_const->back()->point_collar;
			}

		}

		

		

		

		

		
		/*������� ��������� ��������/������ ���������� ���������� ������ �������� � �������� ������������.
		���������� ������ � ����������� ������� �������� ����������.*/
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, math_obj* _current_element)
		{
			math_obj* CE = new math_obj(_pDest, _endPtr,NULL,NULL);
			wstring output;
			math_obj* temp=NULL;
			//size_t output_size;
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
						//temp = run_vector(CE->name);
						if (temp == NULL)
						{
							general_var_const->push_back(CE);
						}
						else
						{
							temp->tree_destruct();
							for (size_t count = 0; count < general_var_const->size(); count++)
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
					output = to_string(CE->var, var_type::FRACTIONAL, 5);

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
						//temp = run_vector(CE->name);
						if (temp == NULL)
						{
							general_var_const->push_back(CE);
						}
						else
						{
							temp->tree_destruct();
							for (size_t count = 0; count < general_var_const->size(); count++)
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
		int input_to_analize(data_list* all_math_data)
		{
			/*if (!Project::IO::VerifyInput(input)) {
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				return err->GetErrorWStr();
			}*/

			//size_t size_of_vect = general_var_const->size();


			general_var_const->back();

			wchar_t* point_start = input;	//start pointer
			wchar_t* point_end = input + wcslen(input) - 1;	//end pointer	
			return 0;
		}
	}
}
