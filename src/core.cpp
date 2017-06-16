#include "core.h"

namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;
		using namespace Project::IO;

		

		
		//»нициализаци€ массива констант, переменных, функций и уравнений
		data_list* Init() {
			ProjectError::Init();
			math_obj* temp;
			data_list* data = new data_list();
			math_obj* temp_var = new math_obj(L"", varbl, servc, L"", 0, NULL, NULL, NULL);
					
			
			//корень 
			{
				temp = new math_obj((size_t)0);
				temp_var->prop = L"root_";
				temp->vector_push_back(new math_obj(temp_var));				
				temp->vector_push_back(new math_obj(temp_var));				
				temp_var->prop = servc;
				temp->double_lincked_vector();				
				data->push_left(new math_obj(L"root", funct, fundm, L"", 2, NULL, NULL, temp->point_left));
				delete temp;
				temp = data->left->math;
				temp->close_list();
				temp->link_var_list_to_funct();				
				temp->point_left = new math_obj(L"^", power, L"", L"", 0, temp->point_collar, new math_obj(L"/", divis, L"", L"", 0, new math_obj(L"0", numbr, real, 1), temp->point_collar->point_right, NULL), temp);
				temp->point_left->point_right->point_collar = temp->point_left;
			}
			//модуль (корень из квадрата числа)
			{				
				temp_var->prop = L"abs__";
				temp=new math_obj(temp_var);
				temp_var->prop = servc;				
				data->push_left(new math_obj(L"abs", funct, servc, L"", 1,NULL,NULL, temp));
				temp = data->left->math;
				temp->close_list();				
				temp->point_left = new math_obj(data->left->left->math);				
			}
			//знак
			{
				
			}
			data->push_left(new math_obj(L"pi", cnst, fundm, 3.1415926535897932384626433832));
			data->push_left(new math_obj(L"e", cnst, fundm, 2.7182818284590452353602874713527));
			data->push_left(new math_obj(L"i", cnst, fundm, 0));
		}

		

		

		

		

		
		/*‘ункци€ выполн€ет проверку/анализ результата заполнени€ дерева операций и запросов пользовател€.
		¬озвращает строку с результатом текущей итерации вычислений.*/
		wstring analizer(math_obj* CE)
		{
			//math_obj* CE = new math_obj(_pDest, _endPtr,NULL,NULL);
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
				//CE->arithmetic();
				output = to_string(CE->var, var_type::FRACTIONAL, 2);
				CE->tree_destruct();
				delete CE;
			}
			else if (CE->type == equat)
			{
				// ну делать тут как бы ничего не надо. ѕользователь вбил уравнение и оно просто записалось.  огда научимс€ делать парралельные потоки вычислений можно на этом
				//моменте решать это уравнение с низким приоритетом, чтобы когда пользователь запросил ответ - вычислени€ занимали меньше времени - дл€ него.
				//mutex* mut=new mutex();
				//thread tTest(testfunc);	//сразу запускаетс€ и пашет
				//thread* ptTest;	//дл€ глобальной видимости потока, можно глобально хранить на него указатель
				//ptTest = new thread(testfunc);	//инициализировать, и он сразу запуститьс€
				//mut->unlock();
				CE->actn = solve;
				//general_var_const->push_back(CE);
			}
			else if (CE->type == funct)
			{
				if (CE->actn == solve)
				{
					if (CE->prop == arg_c)
					{
						//CE->arithmetic();
						output = to_string(CE->var, var_type::FRACTIONAL, 2);
						CE->tree_destruct();
						delete CE;
					}
					else
					{
						//output = CE->expresion(1);
					}
				}
				else if (CE->actn == write)
				{
					//оп€ть же, видимой реакции от программы быть не должно. «абили функцию - записали. ¬озможно в отдельный поток отдать разложение сложной функции 
					//на элементарные. ћожно и не в отдельный, вр€дли там будет высока€ сложность вычислений

					if (CE->name.size() != 0)
					{
						CE->actn = L"";
						//temp = run_vector(CE->name);
						if (temp == NULL)
						{
							//general_var_const->push_back(CE);
						}
						else
						{
							temp->tree_destruct();
							/*for (size_t count = 0; count < general_var_const->size(); count++)
							{
								if (temp == general_var_const->at(count))
								{
									general_var_const->at(count) = CE;
									delete temp;
									break;
								}
							}*/
						}
					}
					else
					{
						//ошибка
					}
				}
				else
				{
					//сюда попасть не должны. Ќа вс€кий случай оставил удаление
					CE->tree_destruct();
					delete CE;
				}
			}
			else if (CE->type == cnst)
			{
				if (CE->actn == solve)
				{
					//ответ 
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
						//CE->arithmetic();
						CE->tree_destruct();
						CE->actn = L"";
						//temp = run_vector(CE->name);
						if (temp == NULL)
						{
							//general_var_const->push_back(CE);
						}
						else
						{
							temp->tree_destruct();
							/*for (size_t count = 0; count < general_var_const->size(); count++)
							{
								if (temp == general_var_const->at(count))
								{
									general_var_const->at(count) = CE;
									delete temp;
									break;
								}
							}*/
						}
					}
				}
			}
			else if (CE->type == varbl)
			{
				delete CE;
				//один из вариантов запроса на решение уравнени€ (скорее всего - последнего записанного)
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

		/*‘ункци€ €вл€етс€ точкой входа в вычислительное €дро.
		¬озвращает строку с результатом текущей итерации вычислений.*/
		int input_to_analize(data_list* all_math_data)
		{
			/*if (!Project::IO::VerifyInput(input)) {
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				return err->GetErrorWStr();
			}*/

			size_t size = all_math_data->size_s();
			data_list* temp = NULL;
			/*for (int count = 1; count <= size; count++)
			{
				temp = all_math_data->at(count);			
				if (temp->math!= NULL)
					temp->out = analizer(temp->math);
			}*/
			if (all_math_data->out == eror)
				return -1;
			return 0;
		}
	}
}
