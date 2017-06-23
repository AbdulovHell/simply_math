#include "core.h"

namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;
		using namespace Project::IO;

		

		
		//Инициализация массива констант, переменных, функций и уравнений
		data_list* Init() {
			ProjectError::Init();
			math_obj* temp;
			data_list* data = new data_list();
			data->in = L"@start#";
			math_obj* temp_var = new math_obj(L"", flags::varbl, flags::servc, flags::nthng, 0, NULL, NULL, NULL);
					
			
			//корень 
			{
				temp = new math_obj((size_t)0);
				//temp_var->prop = L"root_";
				temp->vector_push_back(new math_obj(temp_var));				
				temp->vector_push_back(new math_obj(temp_var));				
				temp_var->prop = flags::servc;
				temp->double_lincked_vector();				
				data->push_left(new math_obj(L"root", flags::funct, flags::fundm, flags::nthng, 2, NULL, NULL, temp->point_left));
				delete temp;
				temp = data->left->math;				
				temp->link_var_list_to_funct();		
				temp->close_list();
				temp->point_left = new math_obj(L"^", flags::power, flags::nthng, flags::nthng, 0, temp->point_collar, new math_obj(L"/", flags::divis, flags::nthng, flags::nthng, 0, new math_obj(L"0", flags::numbr, flags::real, 1), temp->point_collar->point_right, NULL), temp);
				temp->point_left->point_right->point_collar = temp->point_left;
			}			
			//модуль (корень из квадрата числа)
			{				
				//temp_var->prop = L"abs__";
				temp=new math_obj(temp_var);
				temp_var->prop = flags::servc;				
				data->push_left(new math_obj(L"abs", flags::funct, flags::servc, flags::nthng, 1,NULL,NULL, temp));
				temp = data->left->math;
				temp->close_list();				
				temp->point_left = new math_obj(data->left->left->math);				
			}
			//знак
			{
				
			}
			data->push_left(new math_obj(L"pi", flags::cnst, flags::fundm, 3.1415926535897932384626433832));
			data->push_left(new math_obj(L"e", flags::cnst, flags::fundm, 2.7182818284590452353602874713527));
			data->push_left(new math_obj(L"i", flags::cnst, flags::fundm, 0));
			return data;
		}

		

		

		

		

		
		/*Функция выполняет проверку/анализ результата заполнения дерева операций и запросов пользователя.
		Возвращает строку с результатом текущей итерации вычислений.*/
		wstring analizer(math_obj* CE)
		{
			//math_obj* CE = new math_obj(_pDest, _endPtr,NULL,NULL);
			wstring output;
			math_obj* temp=NULL;
			//size_t output_size;			
			if (CE->type == flags::exprs)
			{
				if (!CE->math_simplify()) //сразу посчитать результат					
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
					ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
					ProjectError::GetProjectLastError(err);
					return err->GetErrorWStr();					
				}
				if (CE->prop != flags::error)
				{
					output = to_string(CE->var, var_type::FRACTIONAL, 4);
				}
				else
				{
					ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
					ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
					ProjectError::GetProjectLastError(err);
					return err->GetErrorWStr();
				}
			}
			else if (CE->type == flags::equat)
			{
				// ну делать тут как бы ничего не надо. Пользователь вбил уравнение и оно просто записалось. Когда научимся делать парралельные потоки вычислений можно на этом
				//моменте решать это уравнение с низким приоритетом, чтобы когда пользователь запросил ответ - вычисления занимали меньше времени - для него.
				//mutex* mut=new mutex();
				//thread tTest(testfunc);	//сразу запускается и пашет
				//thread* ptTest;	//для глобальной видимости потока, можно глобально хранить на него указатель
				//ptTest = new thread(testfunc);	//инициализировать, и он сразу запуститься
				//mut->unlock();
				//CE->actn = flags::solve;
				//general_var_const->push_back(CE);
			}
			else if (CE->type == flags::funct)
			{
				if (CE->actn == flags::write)
				{
					if (!CE->math_simplify())			
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
						ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
						ProjectError::GetProjectLastError(err);
						return err->GetErrorWStr();
					}
				}
				else if (CE->actn == flags::solve)
				{
					if (CE->prop == flags::arg_c)
					{
						if (CE->math_simplify())
						{
							output = to_string(CE->point_left->var, var_type::FRACTIONAL, 4);
						}
						else
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
							ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
							ProjectError::GetProjectLastError(err);
							return err->GetErrorWStr();
						}						
					}
					else
					{
						//output = CE->expresion(1);
					}
					//CE->actn = flags::solve;
				}				
				/*else if (CE->actn == flags::nthng)
				{
					output = to_string(CE->point_left->var, var_type::FRACTIONAL, 4);
				}*/
			}
			else if (CE->type == flags::cnst)
			{					
				if (CE->actn == flags::write)
				{
					if (CE->prop == flags::undef)
					{
						//CE->tree_destruct();
						//delete CE;
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
						ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
						ProjectError::GetProjectLastError(err);
						return err->GetErrorWStr();						
					}
					else if (CE->prop == flags::defnd)
					{
						if (!CE->math_simplify()) //сразу посчитать константу						
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
							ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
							ProjectError::GetProjectLastError(err);
							return err->GetErrorWStr();							
						}
					}
				}
				else if (CE->actn == flags::solve)
				{
					if (!CE->math_simplify()) //хотя значение константы уже посчитано, требуется пройти по ссылке и его забрать					
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::MATH_ERROR);
						ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
						ProjectError::GetProjectLastError(err);
						return err->GetErrorWStr();
					}
					//CE->actn = flags::solve;
					output = to_string(CE->var, var_type::FRACTIONAL, 4);
				}
			}
			else if (CE->type == flags::varbl)
			{
				//delete CE;
				//один из вариантов запроса на решение уравнения (скорее всего - последнего записанного)
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

		/*Функция является точкой входа в вычислительное ядро.
		Возвращает 0 в случае успешного выполнения.
		-1 -  ошибка.*/
		int input_to_analize(data_list* all_math_data)
		{
			/*if (!Project::IO::VerifyInput(input)) {
				ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
				ProjectError::GetProjectLastError(err);
				return err->GetErrorWStr();
			}*/

			//size_t size = all_math_data->size_s();
			data_list* iter = all_math_data;
			wstring temp;
			while (iter->right!= NULL)
			{
				iter = iter->right;
				if (iter->math != NULL)
				{
					temp = analizer(iter->math);
					if (temp.length() != 0)
						iter->out = temp;
				}
			}
			if (all_math_data->out == L"error")
				return -1;
			return 0;
		}
	}
}
