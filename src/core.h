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

		//типы обЪектов	(type)	
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
		//свойства обЪектов (prop)		
		wstring defnd = L"defnd";
		wstring undef = L"undef";
		wstring fundm = L"fundm";
		wstring unslv = L"unslv";
		wstring empty = L"empty";
		wstring real = L"real_";
		wstring cmplx = L"cmplx";
		//действия над объектами (actn)
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
				//если приоритет проверяемой операции !БОЛЬШЕ! текущей операции
				if (pc->get_priority() > current_priority)
				{
					//вызываем метод ещё раз для следующей операции
					return prioritize_processing(pc, current_priority);
				}
				//если приоритет проверяемой операции !МЕНЬШЕ! или равен приоритету текущей операции
				else
				{
					//вернуть указатель на проверенную операцию
					return pc;
				}
			}

			double processing(var_const *pointer, var_const * last_func)
			{
				//TODO: застраховать от ошибок вычисления

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
				//для функции - просто проходим по указателю дальше к выражению для неё
				else if (pointer->type == funct)
				{
					//возможно существует более изящное решение для функции знак, но я не придумал. Только прямо проверять знак числа - сравнивать с нулём.
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
				//когда находим переменную - ссылаемся на функцию для этой переменной, потом на выражение/константу/число вложенную в данную функцию. 
				//Поэтому у каждой функции должна быть переменная с уникальным указателем
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

			/*Метод возвращает приоритет операции.
			1 - сложение
			2 - умножение
			3 - деление
			4 - возведение в степень
			0 - не операция*/
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


			/*метод read возвращает строку, содержащую информацию об элементе класса в соответствии с запрошенным аргументом:
			type - тип элемента класса, всё что записано в строке var_id до символа @
			prop - свойство элемента класса, всё что записано после #
			name - имя элемента класса, подстроку в var_id между @ и #, для функций и уравнений - между @ и (
			func - имя элемента класса, подстроку в var_id между @ и #, без исключений для функций
			nvar - имя переменной, на которую указывает элемент класса. Для функций и уравнений (пока без проверки указателя)
			
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
								out.assign(id_a + 1, id_H);	//TODO: хах)) вот и нашли) создаешь строку длинной 0xFFFF FFFF FFFF FFFF
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
					/* TODO: сделать проверку соответствия имени переменной на которую указывает элемент и имени переменной в названии функции.
					Если не совпадают, заменять имя в названии Н-р: f(x) -> f(y)
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

			/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и коструирующую строку с формальной записью текущего выражения.
			Возвращает строку. ПОКА НЕ РАБОТАЕТ*/
			wstring expresion(int comma)
			{
				return name + L" = " + expression_processing(point_left, &comma);
			}

			/*Метод вызывает рекурсивную функцию проверки приоритета операций в текущем дереве операций.
			Возвращает указатель на элемент дерева (операцию), имеющий приоритет меньший или равный в сравнении с текущим элементом. */
			var_const *prioritize(int current_priority)
			{
				return prioritize_processing(point_collar, current_priority);
			}

			/*Метод вызывает рекурсивную функцию, проходящую по дереву операций и выполняющую их.
			Результатом работы метода является запись результата вычислений в double var текущего элемента класса. */
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
			double arg; //может пригодится.
			int exp;
			var_const *point_left;		//левый рукав
			var_const *point_right;		//правый рукав
			var_const *point_collar;	//воротник
		};
		

		vector<var_const*>* general_var_const;

		var_const pi = var_const(L"pi", cnst, fundm,L"0", 3.1415926535897932384626433832);
		var_const e = var_const(L"e", cnst, fundm, L"0", 2.7182818284590452353602874713527);
		var_const i = var_const(L"i", cnst, fundm, L"0", 0);
		//служебная переменная для заполнения "пустых" функций, просто висит тут, без добавления в массив
		var_const temporary_variable = var_const(L"temporary_variable", varbl, L"servc", L"0", 0);

		
		

		

		//Инициализация массива констант, переменных, функций и уравнений
		void Init() {
			ProjectError::Init();
			var_const* temp;
			general_var_const = new vector<var_const*>;
			general_var_const->push_back(&pi);
			general_var_const->push_back(&e);
			general_var_const->push_back(&i);
			//минус
			{
				general_var_const->push_back(new var_const(L"minus", funct, empty,L"", 0, new var_const(&temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
				temp->point_left = new var_const(L"", mltpl, L"", L"", 0, new var_const(L"0", -1), temp->point_collar, temp);
			}
			
			//корень (пока квадратный)
			{
				general_var_const->push_back(new var_const(L"root", funct, empty, L"", 0, new var_const(&temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
			}
			//модуль (корень из квадрата числа)
			{
				general_var_const->push_back(new var_const(L"abs", funct, empty, L"", 0, new var_const(&temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new var_const(temp);
				general_var_const->back()->point_left->point_collar->point_collar = general_var_const->back()->point_left;
				general_var_const->back()->point_left->point_right = new var_const(L"", power, L"", L"", 0,general_var_const->back()->point_collar,new var_const(L"0",2),NULL);
			}
			//знак
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

		//функция проходит по вектору general_var_const и ищет объект с нужным именем
		var_const* run_vector(wstring _pv)
		{
			size_t temp_size_of_vect = general_var_const->size();
			int count;
			for (count = 0; count < temp_size_of_vect; count++)
			{
				try {
					int iPv = _pv.length();
					wstring wstr = general_var_const->at(count)->name;	//тут вылет
					int iVect = wstr.length();
					if (iPv == iVect)
						if (_pv.compare(general_var_const->at(count)->name) == 0)
							return general_var_const->at(count);
				}
				catch (exception e) {	//выдает string too long кароч

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
			var_const *high_pointer = NULL; //последняя записанная операция
			var_const *low_pointer = NULL; //последняя записанная константа/переменная/ф-ция/выражение
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
					//если равно стоит в самом начале строки
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::EQUALY_FIRST);
						return NULL;
					}
					//равно в конце строки, запрос на действие
					else if (pDest == endPtr)
					{
						//просто выдаём наверх текущий заполненный элемент с параметром solve, а там уже пытаемся его расчитать и вывести результат или сообщить ошибку
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
								//копия переменной с указателем на функцию
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
								//копия переменной с указателем на функцию
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
								//копия переменной с указателем на функцию
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
								//копия переменной с указателем на функцию
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
										//явно заданная функция слева и какая-то функция справа - переопределение той что слева
										current_element->point_left = temp_pointer->point_left;
									}
									else
									{
										//неявно заданная ф-ция слева - уравнение
										current_element->type= equat;
										current_element->point_right = temp_pointer->point_left;
										current_element->prop = unslv;
									}
								}
								else
								{
									//тут доделать функции/уравнения нескольких переменных									
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
										//явно заданная функция слева и переменная этой функции справа - переопределение той что слева
										current_element->point_left = temp_pointer;
									}
									else
									{
										//неявно заданная ф-ция слева - уравнение
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
									//копия переменной с указателем на функцию
									current_element->point_collar = new var_const(temp_pointer);
									current_element->point_collar->point_collar = current_element;

									current_element->point_left = current_element->point_collar;
									
									current_element->type = funct;
									current_element->prop = defnd;
								}
							}
							else if (temp_pointer->type == exprs)
							{
								//уравнение вида f(x) = 2
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
								//уравнение вида f(x) = const
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
							//слева - конст выражение - справа функция => уравнение
							if (temp_pointer->type == funct)
							{
								current_element->type = equat;
								current_element->point_right = temp_pointer;
								//копия переменной с указателем на функцию
								current_element->point_collar = new var_const(temp_pointer->point_collar);
								current_element->point_collar->point_collar = current_element;
								current_element->prop = unslv;								
							}
							else if (temp_pointer->type == varbl)
							{
								current_element->type = equat;
								current_element->point_right = temp_pointer;
								//тут уже должна вылезать копия переменной
								current_element->point_collar = temp_pointer;
								current_element->point_collar->point_collar = current_element;
								current_element->prop = unslv;								
							}
							else if ((temp_pointer->type == cnst) || (temp_pointer->type == exprs))
							{
								//запись вида 2+3=7. Пока не знаю как реагировать на такое
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
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new var_const(L"+",addit,L"",L"", brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"+", addit, L"", L"", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new var_const(L"+", addit, L"", L"", brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_element->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
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
					//если это первая операция в выражении
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new var_const(L"*", mltpl, L"", L"", brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 2))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"*", mltpl, L"", L"", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 2))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new var_const(L"*", mltpl, L"", L"", brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_element->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
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
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new var_const(L"/", divis, L"", L"", brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 3))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"/", divis, L"", L"", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 3))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new var_const(L"/", divis, L"", L"", brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_element->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
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
					//если это первая операция в выражении
					else if (high_pointer == low_pointer)
					{
						//записываем операцию, левый рукав -> на предыдущее число, воротник - на текущую константу
						current_element->point_left = new var_const(L"^", power, L"", L"", brakets_counter + brakets, low_pointer, NULL, current_element);
						//левый рукав текущей вычисляемой константы указывает на созданную операцию
						high_pointer = current_element->point_left;
					}
					//если была какая-либо операция до этого
					else
					{
						//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 4))
						{
							//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
							high_pointer->point_right = new var_const(L"^", power, L"", L"", brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
						}
						//если приоритет предыдущей обработанной операции !БОЛЬШЕ! чем приоритет текущей
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 4))
							{
								//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
								high_pointer = new var_const(L"^", power, L"", L"", brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//воротник предыдущей легчайшей операции -> на новую операцию
								current_element->point_left->point_collar = high_pointer;
								//указываем левым рукавом константы на созданную операцию
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
					//операция минус понимается как сложение с отрицательным числом					
					//если выражение начинается с минуса (случай когда после минуса идёт число, минус перед скобкой не реализован)
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> константу
						current_element->point_left = new var_const(general_var_const->at(2));
						current_element->point_left->type = defnd;

						//оба указателя -> на число, тебуется для проверки условия при записи операции
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					//если были другие числа/операции перед минусом
					else
					{
						//если это первая операция в выражении (сложение)
						if (high_pointer == low_pointer)
						{
							//сначала записываем операцию, левый рукав -> на предыдущее число, воротник на конст
							current_element->point_left = new var_const(L"+", addit, L"", L"",  brakets_counter, low_pointer, NULL, current_element);
							high_pointer = current_element->point_left;
							high_pointer->point_right = new var_const(general_var_const->at(3));

							high_pointer->point_right->type = defnd;

							low_pointer = high_pointer->point_right;
						}
						//если ранее были другие операции
						else
						{
							//если приоритет предыдущей обработанной операции !МЕНЬШЕ! или равен  приоритету текущей
							if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
							{
								//записываем операцию, левый рукав -> на предыдущее число, воротник на предыдущую операцию
								high_pointer->point_right = new var_const(L"+", addit, L"", L"",  brakets_counter, low_pointer, NULL, high_pointer);
								high_pointer = high_pointer->point_right;                 //верхний указатель -> на созданную операцию
								high_pointer->point_right = new var_const(general_var_const->at(2));
								high_pointer->point_right->type = defnd;
								low_pointer = high_pointer->point_right;
							}

							else
							{
								if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
								{
									//записываем операцию как самую лёгкую, левый рукав -> на предыдущую наилегчайшую операцию, воротник - на текущую константу
									high_pointer = new var_const(L"+", addit, L"", L"",  brakets_counter, current_element->point_left, NULL, current_element);
									//воротник предыдущей легчайшей операции -> на новую операцию
									current_element->point_left->point_collar = high_pointer;
									//указываем левым рукавом константы на созданную операцию
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
					//левая скобка увеличивает счётчик скобок на число, равное самому высокому приоритету функции
					brakets_counter += 4;
					temp = pDest;
					count = 1;
					while (count != 0)
					{
						temp++;
						//если попали в конец строчки не найдя закрывающих скобок
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
						else if (*temp == '=') //открытая скобка и дальше равно
						{							
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
							return NULL;
						}
					}
					if (pDest + 1 == temp)
					{
						//пустая строка в скобках. вроде не ошибка, но можно и запретить)
					}
					else
					{
						name.assign(pDest + 1, temp);
						temp_pointer = filling_vector(&name[0], &name[name.length() - 1], new var_const(L"",exprs,undef,L"", 0, low_pointer), brakets + brakets_counter);
						if (temp_pointer == NULL)
						{
							return temp_pointer;
						}
						//скобка в самом начале строки
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
							//что-то должно происходить после закрытой скобки, возможно надо проверять что дальше после неё
						}

						// вначале строки что-то стоит - минус или буквосочетание - потом скобка
						else if (low_pointer == high_pointer)
						{
							//перед скобкой стоит минус
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
							//перед скобкой стоит буквосочетание - перенести в определение функции
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
										//после закрытой скобки стоит равно
										if (*(temp + 1) == L'=')
										{
											if (current_element->point_collar->name != temp_pointer->name)
											{
												if (temp_pointer->prop == undef)
												{
													temp_pointer->prop= defnd;
													general_var_const->push_back(new var_const(temp_pointer));
												}
												//записываем новую переменную 
												current_element->point_collar = temp_pointer;												
												current_element->type = funct;
												current_element->prop = undef;
											}
											else
											{
												//ничего не делать!
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
									//тут пока так и оставлю. вложенные функции надо додумать
									else if (temp_pointer->type == funct)
									{
										current_element->type = funct;										
										current_element->point_collar = temp_pointer->point_collar;
										current_element->point_left = temp_pointer;
										high_pointer = current_element->point_left;
										low_pointer = high_pointer;
									}


								}


								//какая-то новая функция
								else if (current_element->prop == undef)
								{
									if ((temp_pointer->type == exprs) || (temp_pointer->type == cnst))
									{
										//попытка посчитать неопределённую ранее функцию от числа/константы. 
										//Возможно для константы сделать отдельную проверку и в этом случае переопределить её как переменную. Приэтом удалится её значение
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
									else if (temp_pointer->type == varbl)
									{
										//после закрытой скобки стоит равно
										if (*(temp + 1) == L'=')
										{
											if (temp_pointer->prop == undef)
											{
												temp_pointer->prop = defnd;
												general_var_const->push_back(new var_const(temp_pointer));
											}
											//записываем новую переменную 
											current_element->point_collar = temp_pointer; //тут проверить выдаётся ли копия в случае используемой переменной да выдаётся											
											current_element->prop = defnd;
										}
										else
										{
											//пока ошибка. Вообще это неявное определение функции через какое-то уравнение. 
											//в таком случае разумнее подобное определение ограничить результатом решения уравнений нескольких переменных относительно какой-то одной.
											//а такую запись считать синтаксической ошибкой.
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
									}
									//тут пока так и оставлю. вложенные функции надо додумать
									else if (temp_pointer->type == funct)
									{
										//неопределённая функция с определённой в качестве аргумента - не знаю как это рассматривать и где это может встретится
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
										return NULL;
									}
								}
							}

						}
						//если стоящая перед скобкой операция не имеет правого операнда
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
										//тут доделать функции/уравнения нескольких переменных
										
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
										//тут доделать функции/уравнения нескольких переменных
										
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
						//если что-то ещё
						else
						{
							if (low_pointer->type == funct)
							{
								if (low_pointer->name == L"minus")
								{
									if (low_pointer->point_right == NULL)
									{
										//еще проверки
										low_pointer->point_right = temp_pointer;
									}
									else
									{
										//тут отложу пока - минус перед функцией или конст или переменной и дальше скобка
										
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
												//тут доделать функции/уравнения нескольких переменных												
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
												//тут доделать функции/уравнения нескольких переменных												
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
								//возможно этот случай лучше исключить в разделе обработки чисел
							}
							else if ((low_pointer->type == mltpl)|| (low_pointer->type == addit)|| (low_pointer->type == divis)|| (low_pointer->type == power))
							{								
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::INTERNAL_POINTER_ERR);
								return NULL;
							}
						}
					}
					//сразу переходим к следующей позиции после скобки
					brakets_counter -= 4;
					pDest = temp + 1;
					if (*pDest == '(')
					{
						//по идее запись вида )( может означать )*(. Потом добавть сюда определение для подобного случая
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
						return NULL;
					}
				}
				else if (*pDest == '|')
				{
					//ну как бы модуль. маленькая проблема - в случае со скобками открывающая ( и закрывающая ) - разные символы. 
					//В случае с модулем | - эта палочка - один и тот же символ. Модуль доступен через вызов abs().
				}
				else if ((*pDest == '1') || (*pDest == '2') || (*pDest == '3') || (*pDest == '4') || (*pDest == '5') || (*pDest == '6') || (*pDest == '7') || (*pDest == '8') || (*pDest == '9') || (*pDest == '0'))
				{                   //любое число имеет id "0"					
					// если данное число первое в записи выражения
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//создание элемента класса и запись числа, воротник -> константу
						current_element->point_left = new var_const(L"", numbr,real,L"", wcstod(pDest, &pDest), current_element);
						//оба указателя -> на число, тебуется для проверки условия при записи операции
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					// если данное число не первое в записи (была какая-либо операция)
					else
					{
						//создание элемента класса и запись числа, воротник -> пред операцию 						
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
					//если строчка начинается с cимвола
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
						//если не найден ни один элемент массива с таким именем
						if (high_pointer == NULL)
						{
							//считаем найденный элемент переменной
							if (temp == NULL)
							{
								current_element->type = varbl;
								current_element->name = name;
								current_element->prop = undef;
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//считаем найденный элемент конст
							else if (*temp == '=')
							{
								current_element->type = cnst;
								current_element->name = name;
								current_element->prop = undef;
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//считаем найденный элемент функцией
							else if (*temp == '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
								p_temp = temp;
								while (count != 0)
								{
									temp++;
									//если попали в конец строчки не найдя закрывающих скобок
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
									else if (*temp == '=') //открытая скобка и дальше равно
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
									//если в самом начале строки стоит новое буквосочетание и дальше скобка - это всегда определение новой функции
									//если после этого нет равно - ошибка
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								if (p_temp + 1 == temp)
								{
									//пустая строка в скобках. получается наверное создание функции от неопределённого количества переменных
									//аналогично заданию функции вообще без указания переменных.
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
										//неопределённая функция от определённой - видимо как бы неявное задание, но это тогда скорее уравнение
									}
								}
								//если в скобках стоят запятые - определение функции нескольких переменных
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
											//поскольку функция ещё неопределённая передавать ей любые аргументы будет неверно
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
							//считаем найденный элемент переменной, а текущее выражение - функцией
							else
							{
								current_element->type = funct;								
								current_element->prop = undef;								
								general_var_const->push_back(new var_const(name,varbl,defnd,L"", 0));
								//копия переменной с указателем на функцию
								current_element->point_collar = new var_const(general_var_const->back());
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						//найден элемент массива с совпадающим именем - константа
						else if (high_pointer->type == cnst)
						{
							//копируем константу
							if (temp == NULL)
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//текущий элемент => константа
							else if (*temp == '=')
							{								
									current_element = high_pointer;
									low_pointer = high_pointer;							
								
							}
							//тут идея в том, что если уже есть какая-то константа - какие бы ни были аргументы для неё - ответом будет являться константа.
							//однако данная запись может означать переопределение конст в виде функции
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
							//текущий элемент остаётся выражением, найденная константа (её копия) - один из операндов
							else
							{
								current_element->point_left = new var_const(high_pointer);
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						//найден элемент массива с совпадающим именем - переменная
						else if (high_pointer->type == varbl)
						{
							//копируем переменную
							if (temp == NULL)
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;

							}
							//текущий элемент => переменная
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
							//такая запись может означать ТОЛЬКО переопределение переменной в качестве функции нового аргумента
							else if (*temp == '(')
							{
								current_element = high_pointer;
								low_pointer = high_pointer;
								current_element->point_collar = new var_const();
							}
							//текущий элемент => функция, аргументом является копия переменной
							else
							{
								current_element->type = funct;
								current_element->prop = undef;
								//копия переменной с указателем на функцию
								current_element->point_collar = new var_const(high_pointer);
								current_element->point_left = current_element->point_collar;
								current_element->point_left->point_collar = current_element;
								high_pointer = current_element->point_left;
								low_pointer = high_pointer;
							}
						}
						//найден элемент массива с совпадающим именем - функция
						else if (high_pointer->type == funct)
						{
							//копируем функцию
							if (temp == NULL)
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;

							}
							//данная функция может быть использована в каком-то уравнении/выражении/другой функции в качестве аргумента. 
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
							/*else if (*temp == '(') //тут пока вопрос
							{
								current_element->var_id = L"funct@";
								current_element->var_id += L"(" + high_pointer->read(L"nvar") + L")";
								current_element->var_id += L"#undef";
								current_element->point_left = new var_const(high_pointer);
								//копия переменной с указателем на функцию
								current_element->point_collar = new var_const(high_pointer->point_collar);
								current_element->point_collar->point_collar = current_element;
								high_pointer = current_element->point_left;
								high_pointer->var_id.replace(high_pointer->var_id.find_first_of('#') + 1, 5, L"undef");
								//если в самом начале строки найдена существующая ф-ция перед скобкой. счтием её undef - в скобках что-то важное
								low_pointer = high_pointer;
							}*/
							//текущее выражение => функция, найденная функция - операнд выражения.
							else
							{
								current_element->type = funct;								
								current_element->prop = undef;
								current_element->point_left = new var_const(high_pointer);
								//копия переменной с указателем на функцию
								current_element->point_collar = new var_const(high_pointer->point_collar);
								current_element->point_collar->point_collar = current_element;
								//просто название самой функции без аргументов
								high_pointer = current_element->point_left;
								high_pointer->type = defnd;
								low_pointer = high_pointer;
							}
						}
					}
					//символ не в начале строки
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
						//далее всегда может быть только два варианта - текщуий элемент либо функция, либо выражение
						//если не найден ни один элемент массива с таким именем
						if (low_pointer == NULL)
						{
							//если текущий элемент - функция, то очевидно найдена новая переменная в записи выражения. 
							if (current_element->type == funct)
							{
								//тут доделать функции/уравнения нескольких переменных								
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
								return NULL;
							}							
							//если выражение
							else if (current_element->type == exprs)
							{
								// выражение в undef функцию 
								current_element->type = funct;
								current_element->prop = undef;							
								general_var_const->push_back(new var_const(name,varbl,defnd,L"", 0));
								//копия переменной с указателем на функцию
								current_element->point_collar = new var_const(general_var_const->back());
								current_element->point_collar->point_collar = current_element;
								//тут проблема может возникнуть - я не помню какая))
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
						//найден элемент массива с совпадающим именем - переменная
						else if (low_pointer->type == varbl)
						{
							if (current_element->type == funct)
								//не совпадение имён переменных
								if (low_pointer->name != current_element->point_collar->name)
								{
									//тут доделать функции/уравнения нескольких переменных									
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::MULTIPLE_VARIABLES);
									return NULL;
								}
								// при совпадении просто записываем переменную в выражение
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
								//копия переменной с указателем на функцию
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
						//найден элемент массива с совпадающим именем - константа - просто записываем её в выражение
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
						//найден элемент массива с совпадающим именем - функция
						else if (low_pointer->type == funct)
						{
							if (current_element->type == funct)
							{
								//тут необходимо проверять совпадение переменных. если мы ражнее объявили f(x) а далее используем её в выражении для функции зависяжей от y, 
								//скажем как слагаемое, то получится функция двух переменных. Не касается случая передачи одной функции другой в качестве аргумента
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
									//тут доделать функции/уравнения нескольких переменных									
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
										high_pointer->point_right->point_right->prop = defnd; //она и так defnd по потроению. возможно надо что-то другое делать
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
									//тут нужна доп проверка на именные функции. для них всегда надо явно указывать переменные.
									current_element->type = funct;
									current_element->prop = undef;									
									//копия переменной с указателем на функцию
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


		/*Функция выполняет проверку/анализ результата заполнения дерева операций и запросов пользователя.
		Возвращает строку с результатом текущей итерации вычислений.*/
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
				// ну делать тут как бы ничего не надо. Пользователь вбил уравнение и оно просто записалось. Когда научимся делать парралельные потоки вычислений можно на этом
				//моменте решать это уравнение с низким приоритетом, чтобы когда пользователь запросил ответ - вычисления занимали меньше времени - для него.
				//mutex* mut=new mutex();
				//thread tTest(testfunc);	//сразу запускается и пашет
				//thread* ptTest;	//для глобальной видимости потока, можно глобально хранить на него указатель
				//ptTest = new thread(testfunc);	//инициализировать, и он сразу запуститься
				//mut->unlock();
			}
			else if (CE->type == funct)
			{
				if (CE->actn == solve)//тут ещё условие
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
					//опять же, видимой реакции от программы быть не должно. Забили функцию - записали. Возможно в отдельный поток отдать разложение сложной функции 
					//на элементарные. Можно и не в отдельный, врядли там будет высокая сложность вычислений
				}
			}
			else if (CE->type == cnst)
			{
				if (CE->actn == solve)
				{
					//ответ 
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
					//заполнили - посчитали
					CE->arithmetic();
					CE->tree_destruct();
				}
			}
			else if (CE->type == varbl)
			{
				CE->type = defnd;
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
		Возвращает строку с результатом текущей итерации вычислений.*/
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
