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
#include "classes.h"

namespace Project {
	namespace Core {

		using namespace std;
		using namespace Project;
		using namespace Project::IO;
		/*
		//òèïû îáÚåêòîâ	(type)	
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
		//ñâîéñòâà îáÚåêòîâ (prop)		
		wstring defnd = L"defnd";//îáùåå ñâîéñòâî
		wstring undef = L"undef";//îáùåå ñâîéñòâî

		wstring arg_c = L"arg_c"; //ôóíêöèÿ ñ êîíñòàíòíûìè àðãóìåíòàìè
		wstring arg_v = L"arg_v"; //ôóíêöèÿ ñ ïåðåìåííûìè àðãóìåíòàìè 
		wstring empty = L"empty"; //"ïóñòàÿ" ôóíêöèÿ. Ñâîéñòâî èìåííûõ íåèíèöèàëèçèðîâàííûõ ôóíêöèé

		wstring fundm = L"fundm";//Ôóíäàìåíòàëüíàÿ êîíñòàíòà

		wstring unslv = L"unslv";//ñâîéñòâî óðàâíåíèé è ôóíêöèé îò êîíñò âûðàæåíèé
		wstring solvd = L"solvd";//ñâîéñòâî óðàâíåíèé è ôóíêöèé îò êîíñò âûðàæåíèé

		wstring real = L"real_";//ñâîéñòâà ÷èñåë
		wstring cmplx = L"cmplx";//ñâîéñòâà ÷èñåë

		wstring servc = L"servc";//ñâîéñòâî ñëóæåáíûõ îáúåêòîâ
		//äåéñòâèÿ íàä îáúåêòàìè (actn)
		wstring solve = L"solve";
		wstring write = L"write";
		wstring read = L"read_";		
		*/
		//ñòðóêòóðà õðàíåíèÿ äàííûõ
		vector<math_obj*>* general_var_const;

		math_obj pi = math_obj(L"pi", cnst, fundm, L"0", 3.1415926535897932384626433832);
		math_obj e = math_obj(L"e", cnst, fundm, L"0", 2.7182818284590452353602874713527);
		math_obj i = math_obj(L"i", cnst, fundm, L"0", 0);
		//ñëóæåáíàÿ ïåðåìåííàÿ äëÿ çàïîëíåíèÿ "ïóñòûõ" ôóíêöèé, ïðîñòî âèñèò òóò, áåç äîáàâëåíèÿ â ìàññèâ
		math_obj temporary_variable = math_obj(L"temporary_variable", varbl, servc, L"0", 0);






		//Èíèöèàëèçàöèÿ ìàññèâà êîíñòàíò, ïåðåìåííûõ, ôóíêöèé è óðàâíåíèé
		void Init() {
			ProjectError::Init();
			math_obj* temp;
			general_var_const = new vector<math_obj*>;
			general_var_const->push_back(&pi);
			general_var_const->push_back(&e);
			general_var_const->push_back(&i);
			//ìèíóñ
			{
				general_var_const->push_back(new math_obj(L"minus", funct, empty, L"", 1, new math_obj(&temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
				temp->point_left = new math_obj(L"", mltpl, L"", L"", 0, new math_obj(L"0", -1), temp->point_collar, temp);
			}

			//êîðåíü (ïîêà êâàäðàòíûé)
			{
				general_var_const->push_back(new math_obj(L"root", funct, empty, L"", 0, new math_obj(&temporary_variable)));
				temp = general_var_const->back();
				temp->point_collar->point_collar = temp;
			}
			//ìîäóëü (êîðåíü èç êâàäðàòà ÷èñëà)
			{
				general_var_const->push_back(new math_obj(L"abs", funct, empty, L"", 0, new math_obj(&temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new math_obj(temp);
				general_var_const->back()->point_left->point_collar->point_collar = general_var_const->back()->point_left;
				general_var_const->back()->point_left->point_right = new math_obj(L"", power, L"", L"", 0, general_var_const->back()->point_collar, new math_obj(L"0", 2), NULL);
			}
			//çíàê
			{
				temp = general_var_const->back();
				general_var_const->push_back(new math_obj(L"sgn", funct, empty, L"", 0, new math_obj(&temporary_variable)));
				general_var_const->back()->point_collar->point_collar = general_var_const->back();
				general_var_const->back()->point_left = new math_obj(L"", divis, L"", L"", 0, general_var_const->back()->point_collar, new math_obj(temp), general_var_const->back());
				general_var_const->back()->point_left->point_right->point_right = general_var_const->back()->point_collar;
				general_var_const->back()->point_left->point_right->point_collar = general_var_const->back()->point_collar;
			}

		}

		/*void testfunc(mutex &mut) {
			mut.lock();
			MessageBoxA(0,"testfunc","tThread",0);
		}*/

		//ôóíêöèÿ ïðîõîäèò ïî âåêòîðó general_var_const è èùåò îáúåêò ñ íóæíûì èìåíåì
		math_obj* run_vector(wstring _pv)
		{
			size_t temp_size_of_vect = general_var_const->size();
			int count;
			for (count = 0; count < temp_size_of_vect; count++)
			{
				try {
					int iPv = _pv.length();
					wstring wstr = general_var_const->at(count)->name;	//òóò âûëåò
					int iVect = wstr.length();
					if (iPv == iVect)
						if (_pv.compare(general_var_const->at(count)->name) == 0)
							return general_var_const->at(count);
				}
				catch (exception e) {	//âûäàåò string too long êàðî÷

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
			math_obj *high_pointer = NULL; //ïîñëåäíÿÿ çàïèñàííàÿ îïåðàöèÿ
			math_obj *low_pointer = NULL; //ïîñëåäíÿÿ çàïèñàííàÿ êîíñòàíòà/ïåðåìåííàÿ/ô-öèÿ/âûðàæåíèå
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
					//åñëè ðàâíî ñòîèò â ñàìîì íà÷àëå ñòðîêè
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::EQUALY_FIRST);
						return NULL;
					}
					//ðàâíî â êîíöå ñòðîêè, çàïðîñ íà äåéñòâèå
					else if (pDest == endPtr)
					{
						//ïðîñòî âûäà¸ì íàâåðõ òåêóùèé çàïîëíåííûé ýëåìåíò ñ ïàðàìåòðîì solve, à òàì óæå ïûòàåìñÿ åãî ðàñ÷èòàòü è âûâåñòè ðåçóëüòàò èëè ñîîáùèòü îøèáêó
						if (current_element->actn != write)
							current_element->actn = solve;
						else
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
							return NULL;
						}
					}
					else
					{
						if (current_element->type == cnst)
						{
							temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer = NULL)
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
								//òóò ëþáîïûòíî ïîëó÷èëîñü. Åñëè çàïèñü âèäà ñ=ñ+1, ãäå ñ áûëà çàïèñàíà êàê êîíñòàíòà, ïðîãðàììà ñíà÷àëà ïîñ÷èòàåò ñî ñòàðûì çíà÷åíèåì ñ âûðàæåíèå, à ïîòîì - çàïèøåò âìåñòî c.
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
						}
						else if (current_element->type == varbl)
						{
							temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer = NULL)
								return temp_pointer;
							//òóò ïðîèñõîäèò ïåðåîïðåäåëåíèå ïåðåìåíûõ â êà÷åñòâå ôóíêöèé. Ìîãóò âîçíèêíóòü ïðîáëåìû, ò.ê. ïîÿâÿòñÿ äâà ðàçíûõ ïî òèïó îáúåêòà ñ îäíèì èìåíåì.
							if (temp_pointer->type == funct)
							{
								low_pointer = temp_pointer->find_varbl(current_element);
								if (low_pointer == NULL)
								{
									//åñëè ïåðåìåííàÿ ñëåâà îò ðàâíî íå ïîÿâëÿåòñÿ ñïðàâà - ïåðåîïðåäåëåíèå ïåðåìåííîé â ôóíêöèþ. í-ð : y=2*x+1
									current_element->define_funct(temp_pointer);
									current_element->type = funct;
									current_element->prop = defnd;
									current_element->actn = write;
									delete temp_pointer;
								}
								else
								{
									delete current_element;
									//åñëè ïåðåìåííàÿ ñëåâà îò ðàâíî ïîÿâëÿåòñÿ òàê æå ñïðàâà - óðàâíåíèå. í-ð : x=2*x+1
									current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, low_pointer, temp_pointer, temp_pointer->point_collar);
								}
							}
							else if (temp_pointer->type == varbl)
							{
								if (temp_pointer->name == current_element->name)
								{
									//âèä âûàðàæåíèÿ õ=õ, ãäå õ - ïåðåìåííàÿ. Íå çíàþ ÷òî ýòî ìîæåò çíà÷èòü è êòî ïîäîáíîå íàïèøåò
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
								/*âîîáùå ïîäîáíîå ïåðåîïðåäåëåíèå ïåðåìåííîé â êîíñòàíòó âîçìîæíî òîëüêî åñëè ýòà ïåðåìåííàÿ íå èñïîëüçóåòñÿ íè â îäíîé ôóíêöèè/óðàâíåíèè.
								Ïîñêîëüêó íà äàííîì ýòàïå îáúÿâëåíèå ïåðåìåííûõ êàê îòäåëüíûõ îáúåêòîâ (íå â ñîñòàâå ôóíêöèé) íåâîçìîæíî - äàííàÿ çàïèñü áóäåò ñ÷èòàòüñÿ îøèáêîé.
								*/
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::VARBL_BLOCKED);
								return NULL;
							}

						}
						else if (current_element->type == funct)
						{
							if (current_element->prop == defnd)
							{
								temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
								if (temp_pointer = NULL)
									return temp_pointer;
								//ñëó÷àé çàïèñè f=... ãäå f - çàðàíåå îïðåäåë¸ííàÿ ôóíêöèÿ.
								//ïîëàãàåì, ÷òî åñëè ïîñëå ðàâíî íàõîäèòñÿ ôóíêöèÿ (íåâàæíî îò êàêèõ ïåðåìåííûõ) - ïåðåîïðåäåëåíèå f íîâûì âûðàæåíèåì (ñ íîâûìè ïåðåìåííûìè)
								//åñëè ïîñëå ðàâíî êîíñòàíòà èëè âûðàæåíèå - óðàâíåíèå âèäà f(x) = const
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
							}
							else if (current_element->prop == undef)
							{
								if (current_element->actn == write)
								{
									//ïðè âûçîâå ôóíêöèè çàïîëíåíèÿ çàðàíåå óêàçûâàåòñÿ íàáîð ïåðåìåííûõ çàïîëíÿåìîé ôóíêöèè è êîëè÷åñòâî ïåðåìåííûõ
									temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", funct, undef, current_element->var, current_element->point_collar), brakets + brakets_counter);
									if (temp_pointer = NULL)
										return temp_pointer;
									//åñëè âûëåçëî âñ¸ ÷òî óãîäíî êðîìå ôóíêöèè - îøèáêà
									if (temp_pointer->type != funct)
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
										return NULL;
									}
									else
									{
										//óêàçàòåëü íà äåðåâî îïåðàöèé
										current_element->point_left = temp_pointer->point_left;
										current_element->prop = defnd;
									}
								}
								else
								{
									/*íåîïðåäåë¸ííàÿ ôóíêöèÿ ÷åìó-òî ðàâíà. Âñåãäà óðàâíåíèå, îäíàêî òðåáóåòñÿ ïðîâåðêà ïåðåìåííûõ â ïðàâîé è ëåâîé ÷àñòÿõ óðàâíåíèÿ.
									ìîæíî îñòàâèòü íà ïîòîì, íà àíàëèç óðàâíåíèÿ
									*/
									temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
									if (temp_pointer = NULL)
										return temp_pointer;
									high_pointer = current_element;
									current_element = new math_obj(L"", equat, unslv, write, high_pointer->var, high_pointer, temp_pointer, high_pointer->point_collar);
								}
							}
							else if (current_element->prop == arg_c)
							{
								temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
								if (temp_pointer = NULL)
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
							}
							//îïðåäåë¸ííàÿ çàðàíåå ôóíêöèÿ, êîòîðîé ïåðåäàíû ïåðåìåííûå àðãóìåíòû. âíå çàâèñèìîñòè îò âûðàæåíèÿ ñïðàâà - óðàâíåíèå. 
							//Îïÿòü æå îñòà¸òñÿ âîïðîñ ïåðåìåííûõ.
							else if (current_element->prop == arg_v)
							{
								temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
								if (temp_pointer = NULL)
									return temp_pointer;
								high_pointer = current_element;
								current_element = new math_obj(L"", equat, unslv, write, temp_pointer->var, high_pointer, temp_pointer, temp_pointer->point_collar);
							}
						}
						else if (current_element->type == exprs)
						{
							temp_pointer = filling_vector(pDest + 1, endPtr, new math_obj(L"", exprs, undef, 0, low_pointer), brakets + brakets_counter);
							if (temp_pointer = NULL)
								return temp_pointer;
							//ñëåâà - êîíñò âûðàæåíèå - ñïðàâà ôóíêöèÿ => óðàâíåíèå
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
								//çàïèñü âèäà 2+3=7. Ïîêà íå çíàþ êàê ðåàãèðîâàòü íà òàêîå
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::BOOL_EXPRESSION);
								return NULL;
							}
						}

					}
					//òóò çàêàí÷èâàåòñÿ ñòðîêà.
					pDest = endPtr + 1;
				}
				else if (*pDest == '+')
				{
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;

					}
					//åñëè ýòî ïåðâàÿ îïåðàöèÿ â âûðàæåíèè
					else if (high_pointer == low_pointer)
					{
						//çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî, âîðîòíèê - íà òåêóùóþ êîíñòàíòó
						current_element->point_left = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, current_element);
						//ëåâûé ðóêàâ òåêóùåé âû÷èñëÿåìîé êîíñòàíòû óêàçûâàåò íà ñîçäàííóþ îïåðàöèþ
						high_pointer = current_element->point_left;
					}
					//åñëè áûëà êàêàÿ-ëèáî îïåðàöèÿ äî ýòîãî
					else
					{
						//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÌÅÍÜØÅ! èëè ðàâåí  ïðèîðèòåòó òåêóùåé
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
						{
							//çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî, âîðîòíèê íà ïðåäûäóùóþ îïåðàöèþ
							high_pointer->point_right = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //âåðõíèé óêàçàòåëü -> íà ñîçäàííóþ îïåðàöèþ
						}
						//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÁÎËÜØÅ! ÷åì ïðèîðèòåò òåêóùåé
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
							{
								//çàïèñûâàåì îïåðàöèþ êàê ñàìóþ ë¸ãêóþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ íàèëåã÷àéøóþ îïåðàöèþ, âîðîòíèê - íà òåêóùóþ êîíñòàíòó
								high_pointer = new math_obj(L"+", addit, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//âîðîòíèê ïðåäûäóùåé ëåã÷àéøåé îïåðàöèè -> íà íîâóþ îïåðàöèþ
								current_element->point_left->point_collar = high_pointer;
								//óêàçûâàåì ëåâûì ðóêàâîì êîíñòàíòû íà ñîçäàííóþ îïåðàöèþ
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
					//åñëè ýòî ïåðâàÿ îïåðàöèÿ â âûðàæåíèè
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
						return NULL;
					}
					//åñëè ýòî ïåðâàÿ îïåðàöèÿ â âûðàæåíèè
					else if (high_pointer == low_pointer)
					{
						//çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî, âîðîòíèê - íà òåêóùóþ êîíñòàíòó
						current_element->point_left = new math_obj(L"*", mltpl, brakets_counter + brakets, low_pointer, NULL, current_element);
						//ëåâûé ðóêàâ òåêóùåé âû÷èñëÿåìîé êîíñòàíòû óêàçûâàåò íà ñîçäàííóþ îïåðàöèþ
						high_pointer = current_element->point_left;
					}
					//åñëè áûëà êàêàÿ-ëèáî îïåðàöèÿ äî ýòîãî
					else
					{
						//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÌÅÍÜØÅ! èëè ðàâåí  ïðèîðèòåòó òåêóùåé
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 2))
						{
							//çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî, âîðîòíèê íà ïðåäûäóùóþ îïåðàöèþ
							high_pointer->point_right = new math_obj(L"*", mltpl, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //âåðõíèé óêàçàòåëü -> íà ñîçäàííóþ îïåðàöèþ
						}
						//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÁÎËÜØÅ! ÷åì ïðèîðèòåò òåêóùåé
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 2))
							{
								//çàïèñûâàåì îïåðàöèþ êàê ñàìóþ ë¸ãêóþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ íàèëåã÷àéøóþ îïåðàöèþ, âîðîòíèê - íà òåêóùóþ êîíñòàíòó
								high_pointer = new math_obj(L"*", mltpl, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//âîðîòíèê ïðåäûäóùåé ëåã÷àéøåé îïåðàöèè -> íà íîâóþ îïåðàöèþ
								current_element->point_left->point_collar = high_pointer;
								//óêàçûâàåì ëåâûì ðóêàâîì êîíñòàíòû íà ñîçäàííóþ îïåðàöèþ
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
					//åñëè ýòî ïåðâàÿ îïåðàöèÿ â âûðàæåíèè
					else if (high_pointer == low_pointer)
					{
						//çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî, âîðîòíèê - íà òåêóùóþ êîíñòàíòó
						current_element->point_left = new math_obj(L"/", divis, brakets_counter + brakets, low_pointer, NULL, current_element);
						//ëåâûé ðóêàâ òåêóùåé âû÷èñëÿåìîé êîíñòàíòû óêàçûâàåò íà ñîçäàííóþ îïåðàöèþ
						high_pointer = current_element->point_left;
					}
					//åñëè áûëà êàêàÿ-ëèáî îïåðàöèÿ äî ýòîãî
					else
					{
						//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÌÅÍÜØÅ! èëè ðàâåí  ïðèîðèòåòó òåêóùåé
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 3))
						{
							//çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî, âîðîòíèê íà ïðåäûäóùóþ îïåðàöèþ
							high_pointer->point_right = new math_obj(L"/", divis, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //âåðõíèé óêàçàòåëü -> íà ñîçäàííóþ îïåðàöèþ
						}
						//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÁÎËÜØÅ! ÷åì ïðèîðèòåò òåêóùåé
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 3))
							{
								//çàïèñûâàåì îïåðàöèþ êàê ñàìóþ ë¸ãêóþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ íàèëåã÷àéøóþ îïåðàöèþ, âîðîòíèê - íà òåêóùóþ êîíñòàíòó
								high_pointer = new math_obj(L"/", divis, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//âîðîòíèê ïðåäûäóùåé ëåã÷àéøåé îïåðàöèè -> íà íîâóþ îïåðàöèþ
								current_element->point_left->point_collar = high_pointer;
								//óêàçûâàåì ëåâûì ðóêàâîì êîíñòàíòû íà ñîçäàííóþ îïåðàöèþ
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
					//åñëè ýòî ïåðâàÿ îïåðàöèÿ â âûðàæåíèè
					else if (high_pointer == low_pointer)
					{
						//çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî, âîðîòíèê - íà òåêóùóþ êîíñòàíòó
						current_element->point_left = new math_obj(L"^", power, brakets_counter + brakets, low_pointer, NULL, current_element);
						//ëåâûé ðóêàâ òåêóùåé âû÷èñëÿåìîé êîíñòàíòû óêàçûâàåò íà ñîçäàííóþ îïåðàöèþ
						high_pointer = current_element->point_left;
					}
					//åñëè áûëà êàêàÿ-ëèáî îïåðàöèÿ äî ýòîãî
					else
					{
						//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÌÅÍÜØÅ! èëè ðàâåí  ïðèîðèòåòó òåêóùåé
						if (high_pointer->get_priority() <= (brakets_counter + brakets + 4))
						{
							//çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî, âîðîòíèê íà ïðåäûäóùóþ îïåðàöèþ
							high_pointer->point_right = new math_obj(L"^", power, brakets_counter + brakets, low_pointer, NULL, high_pointer);
							high_pointer = high_pointer->point_right;                 //âåðõíèé óêàçàòåëü -> íà ñîçäàííóþ îïåðàöèþ
						}
						//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÁÎËÜØÅ! ÷åì ïðèîðèòåò òåêóùåé
						else
						{
							if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 4))
							{
								//çàïèñûâàåì îïåðàöèþ êàê ñàìóþ ë¸ãêóþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ íàèëåã÷àéøóþ îïåðàöèþ, âîðîòíèê - íà òåêóùóþ êîíñòàíòó
								high_pointer = new math_obj(L"^", power, brakets_counter + brakets, current_element->point_left, NULL, current_element);
								//âîðîòíèê ïðåäûäóùåé ëåã÷àéøåé îïåðàöèè -> íà íîâóþ îïåðàöèþ
								current_element->point_left->point_collar = high_pointer;
								//óêàçûâàåì ëåâûì ðóêàâîì êîíñòàíòû íà ñîçäàííóþ îïåðàöèþ
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
					//îïåðàöèÿ ìèíóñ ïîíèìàåòñÿ êàê ñëîæåíèå ñ îòðèöàòåëüíûì ÷èñëîì					
					//åñëè âûðàæåíèå íà÷èíàåòñÿ ñ ìèíóñà (ñëó÷àé êîãäà ïîñëå ìèíóñà èä¸ò ÷èñëî, ìèíóñ ïåðåä ñêîáêîé íå ðåàëèçîâàí)
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//ñîçäàíèå ýëåìåíòà êëàññà è çàïèñü ÷èñëà, âîðîòíèê -> êîíñòàíòó
						current_element->point_left = new math_obj(general_var_const->at(3));
						current_element->point_left->type = defnd;

						//îáà óêàçàòåëÿ -> íà ÷èñëî, òåáóåòñÿ äëÿ ïðîâåðêè óñëîâèÿ ïðè çàïèñè îïåðàöèè
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					//åñëè áûëè äðóãèå ÷èñëà/îïåðàöèè ïåðåä ìèíóñîì
					else
					{
						//åñëè ýòî ïåðâàÿ îïåðàöèÿ â âûðàæåíèè (ñëîæåíèå)
						if (high_pointer == low_pointer)
						{
							//ñíà÷àëà çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî, âîðîòíèê íà êîíñò
							current_element->point_left = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, current_element);
							high_pointer = current_element->point_left;
							high_pointer->point_right = new math_obj(general_var_const->at(3));

							high_pointer->point_right->type = defnd;

							low_pointer = high_pointer->point_right;
						}
						//åñëè ðàíåå áûëè äðóãèå îïåðàöèè
						else
						{
							//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÌÅÍÜØÅ! èëè ðàâåí  ïðèîðèòåòó òåêóùåé
							if (high_pointer->get_priority() <= (brakets_counter + brakets + 1))
							{
								//çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî, âîðîòíèê íà ïðåäûäóùóþ îïåðàöèþ
								high_pointer->point_right = new math_obj(L"+", addit, brakets_counter + brakets, low_pointer, NULL, high_pointer);
								high_pointer = high_pointer->point_right;                 //âåðõíèé óêàçàòåëü -> íà ñîçäàííóþ îïåðàöèþ
								high_pointer->point_right = new math_obj(general_var_const->at(3));
								high_pointer->point_right->type = defnd;
								low_pointer = high_pointer->point_right;
							}

							else
							{
								if (current_element->point_left->get_priority() >= (brakets_counter + brakets + 1))
								{
									//çàïèñûâàåì îïåðàöèþ êàê ñàìóþ ë¸ãêóþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ íàèëåã÷àéøóþ îïåðàöèþ, âîðîòíèê - íà òåêóùóþ êîíñòàíòó
									high_pointer = new math_obj(L"+", addit, brakets_counter + brakets, current_element->point_left, NULL, current_element);
									//âîðîòíèê ïðåäûäóùåé ëåã÷àéøåé îïåðàöèè -> íà íîâóþ îïåðàöèþ
									current_element->point_left->point_collar = high_pointer;
									//óêàçûâàåì ëåâûì ðóêàâîì êîíñòàíòû íà ñîçäàííóþ îïåðàöèþ
									current_element->point_left = high_pointer;
									high_pointer->point_right = new math_obj(general_var_const->at(3));
									high_pointer->point_right->type = defnd;
									low_pointer = high_pointer->point_right;
								}
								else
								{
									high_pointer = high_pointer->prioritize(brakets_counter + brakets + 1);
									high_pointer->point_right->point_collar = new math_obj(L"+", addit, brakets_counter + brakets, high_pointer->point_right, NULL, high_pointer);
									high_pointer->point_right = high_pointer->point_right->point_collar;
									high_pointer = high_pointer->point_right;
									high_pointer->point_right = new math_obj(general_var_const->at(3));
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
					//ëåâàÿ ñêîáêà óâåëè÷èâàåò ñ÷¸ò÷èê ñêîáîê íà ÷èñëî, ðàâíîå ñàìîìó âûñîêîìó ïðèîðèòåòó ôóíêöèè
					brakets_counter += 4;
					temp = pDest;
					count = 1;
					while (count != 0)
					{
						temp++;
						//åñëè ïîïàëè â êîíåö ñòðî÷êè íå íàéäÿ çàêðûâàþùèõ ñêîáîê
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
						else if (*temp == '=') //îòêðûòàÿ ñêîáêà è äàëüøå ðàâíî
						{
							ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
							return NULL;
						}
					}
					if (pDest + 1 == temp)
					{
						//ïóñòàÿ ñòðîêà â ñêîáêàõ. âðîäå íå îøèáêà, íî ìîæíî è çàïðåòèòü). Âîîáùå â êîíòåêòå ïîñòðîåíèÿ äåðåâà ýòî îçíà÷àåò çàïèñü íóëÿ. Ïîýòîìó ïîêà çàïðåùåíî.
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
						//ñêîáêà â ñàìîì íà÷àëå ñòðîêè
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
								//ìîæåò áûòü íå áóäåò ðàáîòàòü ïðàâèëüíî
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
									//òàê êàê ñêîáêà ñòîèò â ñàìîì íà÷àëå ñòðîêè, åñëè òåêóùèé ýëåìåíò - ôóíêöèÿ, å¸ ñïèñîê ïåðåìåííûõ âñåãäà çàìêíóò
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
									current_element->point_collar = new math_obj(&temporary_variable);
									current_element->point_right = temp_pointer;
									current_element->point_collar->var = 0; //ïðîñòî äëÿ äîñòîâåðíîñòè)
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
									//òàê êàê ñêîáêà ñòîèò â ñàìîì íà÷àëå ñòðîêè, åñëè òåêóùèé ýëåìåíò - ôóíêöèÿ, å¸ ñïèñîê ïåðåìåííûõ âñåãäà çàìêíóò
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
										return NULL;
									}
								}
							}
						}

						// âíà÷àëå ñòðîêè ñòîèò ìèíóñ
						else if (low_pointer == high_pointer)
						{
							//ïåðåä ñêîáêîé ñòîèò ìèíóñ
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
										//òàê êàê ñêîáêà ñòîèò â ñàìîì íà÷àëå ñòðîêè, åñëè òåêóùèé ýëåìåíò - ôóíêöèÿ, å¸ ñïèñîê ïåðåìåííûõ âñåãäà çàìêíóò
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
										current_element->point_collar = new math_obj(&temporary_variable);
										current_element->point_right = temp_pointer;
										current_element->point_collar->var = 0; //ïðîñòî äëÿ äîñòîâåðíîñòè)
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
										//òàê êàê ñêîáêà ñòîèò â ñàìîì íà÷àëå ñòðîêè, åñëè òåêóùèé ýëåìåíò - ôóíêöèÿ, å¸ ñïèñîê ïåðåìåííûõ âñåãäà çàìêíóò
										else
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNREAL_ERROR);
											return NULL;
										}
									}
								}
							}
							//ïåðåä ñêîáêîé ÷òî-òî êðîìå ìèíóñà. Ïåðåä ñêîáêîé ìîæåò ñòîÿòü êàêîå-ëèáî ÷èñëî, í-ð 5(...) = 5*(...). Ïîêà îøèáêà. 
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_OPERATION);
								return NULL;
							}

						}
						//åñëè ñêîáêà ñòîèò íå â íà÷àëå ñòðîêè - ïîäðàçóìåâàåòñÿ íàëè÷èå êàêîé-ëèáî îïåðàöèè ïåðåä íåé
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
									//ñïèñîê ïåðåìåííûõ çàìêíóò
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
											//ñòîèò ëè ïåðåä ïåðåìåííîé ìèíóñ
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
									//íå çàìêíóò
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
											//ïîñêîëüêó ñèìâîë íå â íà÷àëå ñòðîêè - çíà÷èò ñòîèò ïîñëå êàêîé-ëèáî îïåðàöèè. 
											if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//ñòîèò ïîñëå ìèíóñà
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
											//ñòîèò ëè ïåðåä ïåðåìåííîé ìèíóñ
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
									//ó÷èòûâàåòñÿ ìèíóñ ïåðåä ïåðåìåííîé
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
									current_element->point_collar = new math_obj(&temporary_variable);
									current_element->point_right = temp_pointer;
									current_element->point_collar->var = 0; //ïðîñòî äëÿ äîñòîâåðíîñòè)
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
										temp_pointer->point_left = new math_obj(&temporary_variable);
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
					//ñðàçó ïåðåõîäèì ê ñëåäóþùåé ïîçèöèè ïîñëå ñêîáêè
					brakets_counter -= 4;
					pDest = temp + 1;
					if (*pDest == '(,')
					{
						//ïî èäåå çàïèñü âèäà )( ìîæåò îçíà÷àòü )*(. Ïîòîì äîáàâòü ñþäà îïðåäåëåíèå äëÿ ïîäîáíîãî ñëó÷àÿ
						ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
						return NULL;
					}
				}
				else if (*pDest == '|')
				{
					//íó êàê áû ìîäóëü. ìàëåíüêàÿ ïðîáëåìà - â ñëó÷àå ñî ñêîáêàìè îòêðûâàþùàÿ ( è çàêðûâàþùàÿ ) - ðàçíûå ñèìâîëû. 
					//Â ñëó÷àå ñ ìîäóëåì | - ýòà ïàëî÷êà - îäèí è òîò æå ñèìâîë. Ìîäóëü äîñòóïåí ÷åðåç âûçîâ abs().
				}
				else if ((*pDest == '1') || (*pDest == '2') || (*pDest == '3') || (*pDest == '4') || (*pDest == '5') || (*pDest == '6') || (*pDest == '7') || (*pDest == '8') || (*pDest == '9') || (*pDest == '0'))
				{
					// åñëè äàííîå ÷èñëî ïåðâîå â çàïèñè âûðàæåíèÿ
					if ((high_pointer == NULL) && (low_pointer == NULL))
					{
						//ñîçäàíèå ýëåìåíòà êëàññà è çàïèñü ÷èñëà, âîðîòíèê -> êîíñòàíòó
						current_element->point_left = new math_obj(L"", numbr, real, wcstod(pDest, &pDest), current_element);
						//îáà óêàçàòåëÿ -> íà ÷èñëî, òåáóåòñÿ äëÿ ïðîâåðêè óñëîâèÿ ïðè çàïèñè îïåðàöèè
						low_pointer = current_element->point_left;
						high_pointer = low_pointer;
					}
					//ïåðåä ÷èñëîì â íà÷àëå ñòðîêè ñòîèò ìèíóñ
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
					// åñëè äàííîå ÷èñëî íå ïåðâîå â çàïèñè (áûëà êàêàÿ-ëèáî îïåðàöèÿ)
					else
					{
						//ñîçäàíèå ýëåìåíòà êëàññà è çàïèñü ÷èñëà, âîðîòíèê -> ïðåä îïåðàöèþ 						
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

				//ñëó÷àé ñ ñèìâîëàìè â ñòðîêå
				else
				{
					temp = wcspbrk(pDest, L"()+-*^/=");
					//åñëè ñòðî÷êà íà÷èíàåòñÿ ñ cèìâîëà
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
						//åñëè íå íàéäåí íè îäèí ýëåìåíò ìàññèâà ñ òàêèì èìåíåì
						if (high_pointer == NULL)
						{
							//ñ÷èòàåì íàéäåííûé ýëåìåíò ïåðåìåííîé
							if (temp == NULL)
							{
								current_element->type = varbl;
								current_element->name = name;
								current_element->prop = undef;
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//ñ÷èòàåì íàéäåííûé ýëåìåíò êîíñò
							else if (*temp == '=')
							{
								current_element->type = cnst;
								current_element->name = name;
								current_element->prop = undef;
								current_element->actn = write;
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//ñ÷èòàåì íàéäåííûé ýëåìåíò ôóíêöèåé
							else if (*temp == '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
								while (count != 0)
								{
									temp++;
									//åñëè ïîïàëè â êîíåö ñòðî÷êè íå íàéäÿ çàêðûâàþùèõ ñêîáîê
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
									else if (*temp == '=') //îòêðûòàÿ ñêîáêà è äàëüøå ðàâíî
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										//ñ÷èòàåì çàïÿòûå. êîëè÷åñòâî çàïÿòûõ â ñêîáêàõ +1 = ÷èñëî ðàçëè÷íûõ ïåðåìåííûõ
										comma++;
									}
								}
								if (*(temp + 1) != '=')
								{
									//åñëè â ñàìîì íà÷àëå ñòðîêè ñòîèò íîâîå áóêâîñî÷åòàíèå è äàëüøå ñêîáêà - ýòî âñåãäà îïðåäåëåíèå íîâîé ôóíêöèè
									//åñëè ïîñëå ýòîãî íåò ðàâíî - îøèáêà
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								//äàëåå ÂÑÅÃÄÀ ðàññìàòðèâàåì ñëó÷àé ñ ðàâíî ïîñëå ñêîáêè
								if (pDest + 1 == temp)
								{
									//ïóñòàÿ ñòðîêà â ñêîáêàõ.  ñîçäàíèå ôóíêöèè îò íåîïðåäåë¸ííîãî êîëè÷åñòâà ïåðåìåííûõ
									//àíàëîãè÷íî çàäàíèþ ôóíêöèè âîîáùå áåç óêàçàíèÿ ïåðåìåííûõ. Óêàçàòåëü íà ïåðåìåííûå - ïóñòîé
									current_element->type = funct;
									current_element->name = name;
									current_element->prop = undef; //ñ÷èòàåì íîâóþ ôóíêöèþ undef, íî ïðè ýòîì ìû çíàåì ÷òî ó íå¸ åñòü èìÿ
									current_element->actn = write;  //íà çàïîëíåíèå
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								else
								{
									//çàïîìèíàåì ÷èñëî ïåðåìåííûõ
									current_element->var = comma + 1;
									current_element->type = funct;
									current_element->name = name;
									current_element->prop = undef; //ñ÷èòàåì íîâóþ ôóíêöèþ undef, íî ïðè ýòîì ìû çíàåì ÷òî ó íå¸ åñòü èìÿ
									current_element->actn = write;  //íà çàïîëíåíèå
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
											temp_pointer->var = count; //òóò ïðîèñõîäèò çàïèñü íîìåðà ïåðåìåííîé â ìàññèâå âíóòðü ñàìîé ïåðåìåííîé. ïîäðàçóìåâàåòñÿ ÷òî ïåðåìåííûå ÍÅ èìåþò óñòàíîâëåííîãî çíà÷åíèÿ => ìîæíî èñïîëüçîâàòü var êàê ñ÷¸ò÷èê

											if (count == 0)
												//ñðàçó æå ïîëó÷àåì óàçàòåëü íà ïåðâóþ ïåðåìåííóþ â ñïèñêå
												current_element->point_collar = temp_pointer;
											else
											{
												//ñîçäàíèå äâóíàïðàâëåííîãî ñïèñêà ïåðåìåííûõ, ãäå íà íóëåâîé ýëåìåíò óêàçûâàåò âîðîòíèê ôóíêöèè.
												temp_pointer->point_right = current_element->var_list_back();
												temp_pointer->point_right->point_left = temp_pointer;
											}
											if (count == comma)
											{
												//ñîçäàíèå çàìêíóòîãî â êîëüöî äâóíàïðàâëåííîãî ñïèñêà ïåðåìåííûõ. Íóëåâîé ýëåìåíò = ïîñëåäíåìó.
												temp_pointer->point_left = current_element->point_collar;
												current_element->point_collar->point_right = temp_pointer;
											}
											//temp_pointer â äàííîì ñëó÷àå óäàëÿòü íåëüçÿ. Ìîæíî çàíóëèòü, íî óäàëÿòü íåëüçÿ
										}
										else
										{
											//ïîñêîëüêó ôóíêöèÿ åù¸ íåîïðåäåë¸ííàÿ ïåðåäàâàòü åé ëþáûå àðãóìåíòû áóäåò íåâåðíî
											//TODO:÷èñòèòü ìóñîð ïåðåä óõîäîì
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
							//ñ÷èòàåì íàéäåííûé ýëåìåíò ïåðåìåííîé, à òåêóùåå âûðàæåíèå - ôóíêöèåé
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->type = funct;
								current_element->prop = undef;
								//çàïîìèíàåì ÷èñëî ïåðåìåííûõ
								current_element->var = 1;
								general_var_const->push_back(new math_obj(name, varbl, defnd, 0));
								//êîïèÿ ïåðåìåííîé ñ óêàçàòåëåì íà ôóíêöèþ
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
						//íàéäåí ýëåìåíò ìàññèâà ñ ñîâïàäàþùèì èìåíåì - êîíñòàíòà
						else if (high_pointer->type == cnst)
						{
							//êîïèðóåì êîíñòàíòó
							if ((temp == NULL) || (*temp == '='))
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;
							}
							//òóò èäåÿ â òîì, ÷òî åñëè óæå åñòü êàêàÿ-òî êîíñòàíòà - êàêèå áû íè áûëè àðãóìåíòû äëÿ íå¸ - îòâåòîì áóäåò ÿâëÿòüñÿ êîíñòàíòà.
							//îäíàêî äàííàÿ çàïèñü ìîæåò îçíà÷àòü ïåðåîïðåäåëåíèå êîíñò â âèäå ôóíêöèè
							else if (*temp == '(')
							{
								//òîãäà ñðàçó ëó÷øå ïðîÿñíèòü ÷òî èìåííî òóò ïðîèñõîäèò,
								//çàãëÿäûâàåì â ñêîáêè è ñìîòðèì ÷òî òàì
								//current_element->copy(high_pointer);
								brakets_counter += 4;
								count = 1;
								comma = 0;
								while (count != 0)
								{
									temp++;
									//åñëè ïîïàëè â êîíåö ñòðî÷êè íå íàéäÿ çàêðûâàþùèõ ñêîáîê
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
									else if (*temp == '=') //îòêðûòàÿ ñêîáêà è äàëüøå ðàâíî
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										//ñ÷èòàåì çàïÿòûå. êîëè÷åñòâî çàïÿòûõ â ñêîáêàõ +1 = ÷èñëî ðàçëè÷íûõ ïåðåìåííûõ
										comma++;
									}
								}
								if (*(temp + 1) != '=') //äâà âàðèàíòà - ñòîèò ðàâíî ïîñëå ñêîáêè èëè íåò
								{
									//åñëè åãî íåò, òî äàííàÿ çàïèñü â ëþáîì ñëó÷àå ðàâíîñèëüíà ïðîñòî âûçîâó êîíñòàíòû, êàêèå áû àðãóìåíòû åé íå ïåðåäàâàëèñü
									//êîïèðóåì êîíñòàíòó
									if (*(temp + 1) == NULL) // åñëè ìû âûøëè â êîíåö ñòðîêè - òî íîëü - ò.å. ïîñëå ñêîáêè îòñóòñòâóþò ñèìâîëû
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
										//ïîêà îñòàâëþ çòî óñëîâèå. Çàïèñü òèïà ñ(...)(...) - îáû÷íî ýòî îçíà÷àåò ïðîèçâåäåíèå ñ()*(...)
									}*/
									else
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
										return NULL;
									}
								}
								//äàëåå ÂÑÅÃÄÀ ðàññìàòðèâàåì ñëó÷àé ñ ðàâíî ïîñëå ñêîáêè
								else if (pDest + 1 == temp)
								{
									// òóò ëèáî âûâîä çíà÷åíèÿ, ëèáî íîâîå îïðåäåëåíèå c()=
									current_element->copy(high_pointer);
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								//òóò íàäî òîëüêî ïðîâåðèòü, ÷òîáû àðãóìåíòû â ñêîáêàõ áûëè ïåðåìåííûìè
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
											temp_pointer->var = count; //òóò ïðîèñõîäèò çàïèñü íîìåðà ïåðåìåííîé â ìàññèâå âíóòðü ñàìîé ïåðåìåííîé. ïîäðàçóìåâàåòñÿ ÷òî ïåðåìåííûå ÍÅ èìåþò óñòàíîâëåííîãî çíà÷åíèÿ => ìîæíî èñïîëüçîâàòü var êàê ñ÷¸ò÷èê

											if (count == 0)
												//ñðàçó æå ïîëó÷àåì óàçàòåëü íà ïåðâóþ ïåðåìåííóþ â ñïèñêå
												current_element->point_collar = temp_pointer;
											else
											{
												//ñîçäàíèå äâóíàïðàâëåííîãî ñïèñêà ïåðåìåííûõ, ãäå íà íóëåâîé ýëåìåíò óêàçûâàåò âîðîòíèê ôóíêöèè.
												temp_pointer->point_right = current_element->var_list_back();
												temp_pointer->point_right->point_left = temp_pointer;
											}
											if (count == comma)
											{
												//ñîçäàíèå çàìêíóòîãî â êîëüöî äâóíàïðàâëåííîãî ñïèñêà ïåðåìåííûõ. Íóëåâîé ýëåìåíò = ïîñëåäíåìó.
												temp_pointer->point_left = current_element->point_collar;
												current_element->point_collar->point_right = temp_pointer;
											}
											//temp_pointer â äàííîì ñëó÷àå óäàëÿòü íåëüçÿ. Ìîæíî çàíóëèòü, íî óäàëÿòü íåëüçÿ
										}
										else
										{
											//TODO:â ñëó÷àå ýòîé âåòêè ÷èñòèòü ñïèñîê óæå çàïîëíåííûõ ïåðåìåííûõ
											break;
										}
										pDest = temp;
									}

									if ((count == comma + 1) && (high_pointer->prop != fundm))//çà èñêë ôóíäàìåíòàëüíûõ êîíñòàíò
									{
										//çàïîìèíàåì ÷èñëî ïåðåìåííûõ
										current_element->var = comma + 1;
										current_element->type = funct;
										current_element->prop = undef; //ñ÷èòàåì íîâóþ ôóíêöèþ undef, íî ïðè ýòîì ìû çíàåì ÷òî ó íå¸ åñòü èìÿ
										current_element->actn = write; //íà çàïèñü										
										low_pointer = high_pointer;
									}
									else
									{
										// òóò ëèáî âûâîä çíà÷åíèÿ, ëèáî íîâîå îïðåäåëåíèå, èãíîðèðóÿ àðãóìåíòû
										//òóò åù¸ ìîæåò îçíà÷àòü óìíîæåíèå ñ(...) = ñ*(...). Ê ïðèìåðó çàïðîñ íà ðàñ÷¸ò ñ*(...)= 
										//îäíàêî â äàííîì ñëó÷àå ýòî óìíîæåíèå íà âåêòîð - ÷òî íå ðåàëèçîâàíî, âîçìîæíî ñòîèò êàê-òî îòäåëüíî äåëàòü	
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
							//òåêóùèé ýëåìåíò îñòà¸òñÿ âûðàæåíèåì, íàéäåííàÿ êîíñòàíòà (å¸ êîïèÿ) - îäèí èç îïåðàíäîâ
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
						//íàéäåí ýëåìåíò ìàññèâà ñ ñîâïàäàþùèì èìåíåì - ïåðåìåííàÿ
						else if (high_pointer->type == varbl)
						{
							//êîïèðóåì ïåðåìåííóþ
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
							//òàêàÿ çàïèñü ìîæåò îçíà÷àòü ÒÎËÜÊÎ ïåðåîïðåäåëåíèå ïåðåìåííîé â êà÷åñòâå ôóíêöèè íîâîãî àðãóìåíòà
							else if (*temp == '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
								while (count != 0)
								{
									temp++;
									//åñëè ïîïàëè â êîíåö ñòðî÷êè íå íàéäÿ çàêðûâàþùèõ ñêîáîê
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
									else if (*temp == '=') //îòêðûòàÿ ñêîáêà è äàëüøå ðàâíî
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										//ñ÷èòàåì çàïÿòûå. êîëè÷åñòâî çàïÿòûõ â ñêîáêàõ +1 = ÷èñëî ðàçëè÷íûõ ïåðåìåííûõ
										comma++;
									}
								}
								if (*(temp + 1) != '=')
								{
									//åñëè â ñàìîì íà÷àëå ñòðîêè ñòîèò ïåðåìåííàÿ è äàëåå ñêîáêà - ñ÷èòàåì ÷òî ýòî îïðåäåëåíèå íîâîé ôóíêöèè
									//åñëè ïîñëå ýòîãî íåò ðàâíî - îøèáêà
									//òàê æå - ìîæåò áûòü óìíîæåíèå âèäà x(...) ÷òî çíà÷èò x*(...)
									//îäíàêî íå ñòîèò çàáûâàòü î íåÿâíîì îïðåäåëåíèè ôóíêöèé. Ìîæåò îíà òóò ÷åðåç óðàâíåíèå çàäà¸òñÿ
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
									return NULL;
								}
								//äàëåå ÂÑÅÃÄÀ ðàññìàòðèâàåì ñëó÷àé ñ ðàâíî ïîñëå ñêîáêè
								else if (pDest + 1 == temp)
								{
									//ïóñòàÿ ñòðîêà â ñêîáêàõ. ïîëó÷àåòñÿ, íàâåðíîå ñîçäàíèå ôóíêöèè îò íåîïðåäåë¸ííîãî êîëè÷åñòâà ïåðåìåííûõ
									//àíàëîãè÷íî çàäàíèþ ôóíêöèè âîîáùå áåç óêàçàíèÿ ïåðåìåííûõ. Óêàçàòåëü íà ïåðåìåííûå - ïóñòîé
									current_element->copy(high_pointer);
									current_element->type = funct;
									current_element->prop = undef; //ñ÷èòàåì íîâóþ ôóíêöèþ undef, íî ïðè ýòîì ìû çíàåì ÷òî ó íå¸ åñòü èìÿ		
									current_element->actn = write; //íà çàïèñü
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
											temp_pointer->var = count; //òóò ïðîèñõîäèò çàïèñü íîìåðà ïåðåìåííîé â ìàññèâå âíóòðü ñàìîé ïåðåìåííîé. ïîäðàçóìåâàåòñÿ ÷òî ïåðåìåííûå ÍÅ èìåþò óñòàíîâëåííîãî çíà÷åíèÿ => ìîæíî èñïîëüçîâàòü var êàê ñ÷¸ò÷èê

											if (count == 0)
												//ñðàçó æå ïîëó÷àåì óàçàòåëü íà ïåðâóþ ïåðåìåííóþ â ñïèñêå
												current_element->point_collar = temp_pointer;
											else
											{
												//ñîçäàíèå äâóíàïðàâëåííîãî ñïèñêà ïåðåìåííûõ, ãäå íà íóëåâîé ýëåìåíò óêàçûâàåò âîðîòíèê ôóíêöèè.
												temp_pointer->point_right = current_element->var_list_back();
												temp_pointer->point_right->point_left = temp_pointer;
											}
											if (count == comma)
											{
												//ñîçäàíèå çàìêíóòîãî â êîëüöî äâóíàïðàâëåííîãî ñïèñêà ïåðåìåííûõ. Íóëåâîé ýëåìåíò = ïîñëåäíåìó.
												temp_pointer->point_left = current_element->point_collar;
												current_element->point_collar->point_right = temp_pointer;
											}
											//temp_pointer â äàííîì ñëó÷àå óäàëÿòü íåëüçÿ. Ìîæíî çàíóëèòü, íî óäàëÿòü íåëüçÿ
										}
										else
										{
											//ïîñêîëüêó ôóíêöèÿ åù¸ íåîïðåäåë¸ííàÿ ïåðåäàâàòü åé ëþáûå àðãóìåíòû áóäåò íåâåðíî
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
											return NULL;
										}
										pDest = temp;
									}
									//çàïîìèíàåì ÷èñëî ïåðåìåííûõ
									current_element->var = comma + 1;
									current_element->type = funct;
									current_element->prop = undef; //ñ÷èòàåì íîâóþ ôóíêöèþ undef, íî ïðè ýòîì ìû çíàåì ÷òî ó íå¸ åñòü èìÿ
									current_element->actn = write; //íà çàïèñü	
									high_pointer = current_element;
									low_pointer = high_pointer;
								}
								brakets_counter -= 4;
								pDest = temp + 1;
							}
							//òåêóùèé ýëåìåíò => ôóíêöèÿ, àðãóìåíòîì ÿâëÿåòñÿ êîïèÿ ïåðåìåííîé
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->type = funct;
								current_element->prop = undef;
								//çàïîìèíàåì ÷èñëî ïåðåìåííûõ
								current_element->var = 1;
								//êîïèÿ ïåðåìåííîé ñ óêàçàòåëåì íà ôóíêöèþ
								current_element->point_collar = new math_obj(high_pointer);
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
						//íàéäåí ýëåìåíò ìàññèâà ñ ñîâïàäàþùèì èìåíåì - ôóíêöèÿ
						else if (high_pointer->type == funct)
						{
							//êîïèðóåì ôóíêöèþ
							if ((temp == NULL) || (*temp == '='))
							{
								current_element->copy(high_pointer);
								high_pointer = current_element;
								low_pointer = high_pointer;
								//äàííàÿ ôóíêöèÿ ìîæåò áûòü èñïîëüçîâàíà â êàêîì-òî óðàâíåíèè/äðóãîé ôóíêöèè â êà÷åñòâå îïåðàíäà. 
								//ìîæåò òàê æå îçíà÷àòü å¸ ïåðåîïðåäåëåíèå ñ íåèçâåñòíûì êîëè÷åñòâîì ïåðåìåííûõ
							}
							//â äàííîì ñëó÷àå îòêðûòàÿ ñêîáêà ïîñëå íàçâàíèÿ óæå îïðåäåë¸ííîé ô-öèè (í-ð f(x)) ìîæåò çíà÷èòü:
							//--íîâîå îïðåäåëåíèå, â òîì ÷èñëå îò íîâûõ ïåðåìåííûõ
							//--ïåðåäà÷ó ôóíêöèè àðãóìåíòîâ, êàê êîíñòàíòíûõ âûðàæåíèé, òàê è íîâûõ ïåðåìåííûõ. Â ýòîì ñëó÷àå ñ÷èòàåì, 
							//÷òî ïîäðàçóìåâàåòñÿ f(t)= - çàïðîñ íà âûâîä âûðàæåíèÿ îò t, ëèáî ó÷àñòèå f â óðàâíåíèè ñ ïåðåìåííîé t, çà èñêë. ñëó÷àÿ ñ ïåðåîïðåäåëåíèåì.
							//--ïåðåäà÷ó ôóíêöèè â êà÷åñòâå àðãóìåíòà íîâóþ ôóíêöèþ g(s) - ïðè ýòîì ïîëó÷èâøàÿñÿ ôóíêöèÿ f(g(s)) äîëæíà âñþäó çàâèñåòü îò s, à íå îò õ,
							//îäíàêî äîëæíà ñîñòîÿòü èç êîïèé îðèãèíàëüíûõ ôóíêöèé, èñêëþ÷åíèå g(s) - íåÿâíî çàäàíà - f(5*s+2)
							else if (*temp == '(')
							{
								brakets_counter += 4;
								count = 1;
								comma = 0;
								while (count != 0)
								{
									temp++;
									//åñëè ïîïàëè â êîíåö ñòðî÷êè íå íàéäÿ çàêðûâàþùèõ ñêîáîê
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
									else if (*temp == '=') //îòêðûòàÿ ñêîáêà è äàëüøå ðàâíî
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										//ñ÷èòàåì çàïÿòûå. êîëè÷åñòâî çàïÿòûõ â ñêîáêàõ +1 = ÷èñëî ðàçëè÷íûõ ïåðåìåííûõ
										comma++;
									}
								}
								if (pDest + 1 == temp)
								{
									//ïóñòàÿ ñòðîêà â ñêîáêàõ. ïîëó÷àåòñÿ, ëèáî îïðåäåëåíèå ôóíêöèè îò íåîïðåäåë¸ííîãî êîëè÷åñòâà ïåðåìåííûõ èëè ïðîñòî èñïîëüçîâàíèå ôóíêöèè â âûðàæåíèè
									//àíàëîãè÷íî çàäàíèþ ôóíêöèè âîîáùå áåç óêàçàíèÿ ïåðåìåííûõ. Óêàçàòåëü íà ïåðåìåííûå - òîò ÷òî áûë ðàíüøå
									if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
									{
										current_element->copy(high_pointer);
										high_pointer = current_element;
										low_pointer = high_pointer;
									}
									//ñëó÷àé, àíàëîãè÷íûé ïîñëåäíåìó óñëîâèþ â äàííîé âåòêå 
									else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
									{
										current_element->type = funct;
										current_element->prop = arg_v;
										current_element->var = 1; // îäíà ïåðåìåííàÿ
										current_element->point_right = new math_obj(high_pointer); //íàéäåííàÿ ôóíêöèÿ - àðãóìåíò òåêóùåé
										//êîïèÿ ñëóæåáíîé ïåðåìåííîé ñ óêàçàòåëåì íà ôóíêöèþ
										current_element->point_collar = new math_obj(&temporary_variable);
										current_element->point_collar->var = 0; //ïðîñòî äëÿ äîñòîâåðíîñòè)
										current_element->point_collar->point_collar = current_element;
										current_element->point_left = current_element->point_collar;

										high_pointer = current_element->point_left;
										low_pointer = high_pointer;
									}
								}
								//÷èñëî àðãóìåíòîâ â ñêîáêàõ ñîâïàäàåò ñ ÷èñëîì àðãóìåíòîâ ó îïðåäåë¸ííîé çàðàíåå ôóíêöèè
								else if (high_pointer->var == comma + 1)
								{
									//íåò çàïÿòûõ 
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
												current_element->prop = arg_c; //óêàçûâàåì ÷òî â ôóíêöèþ ïåðåäàëè àðãóìåíò
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
													current_element->prop = arg_c; //óêàçûâàåì ÷òî â ôóíêöèþ ïåðåäàëè àðãóìåíò
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
													current_element->prop = arg_v; //óêàçûâàåì ÷òî â ôóíêöèþ ïåðåäàëè àðãóìåíò
													high_pointer = current_element;
													low_pointer = high_pointer;
												}
												else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
												{
													current_element->type = funct;
													current_element->prop = arg_v;
													current_element->var = 1; // îäíà ïåðåìåííàÿ
													current_element->point_right = new math_obj(high_pointer); //íàéäåííàÿ ôóíêöèÿ - àðãóìåíò òåêóùåé
																												//êîïèÿ ñëóæåáíîé ïåðåìåííîé ñ óêàçàòåëåì íà ôóíêöèþ
													current_element->point_collar = new math_obj(&temporary_variable);
													current_element->point_collar->var = 0; //ïðîñòî äëÿ äîñòîâåðíîñòè)
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
												current_element->prop = arg_v; //óêàçûâàåì ÷òî â ôóíêöèþ ïåðåäàëè àðãóìåíò
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->type = funct;
												current_element->prop = arg_v;
												current_element->var = 1; // îäíà ïåðåìåííàÿ
												current_element->point_right = new math_obj(high_pointer); //íàéäåííàÿ ôóíêöèÿ - àðãóìåíò òåêóùåé
																											//êîïèÿ ñëóæåáíîé ïåðåìåííîé ñ óêàçàòåëåì íà ôóíêöèþ
												current_element->point_collar = new math_obj(&temporary_variable);
												current_element->point_collar->var = 0; //ïðîñòî äëÿ äîñòîâåðíîñòè)
												current_element->point_collar->point_collar = current_element;
												current_element->point_left = current_element->point_collar;
												current_element->point_right->point_right = temp_pointer;
												current_element->point_right->prop = arg_v;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
									}
									//åñëè â ñêîáêàõ ñòîÿò çàïÿòûå - îïðåäåëåíèå ôóíêöèè íåñêîëüêèõ ïåðåìåííûõ
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
											//çàïîëíÿåì ìàññèâ àðãóìåíòîâ
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
										if (count_var != 0)//åñëè ñðåäè àðãóìåíòîâ õîòÿ áû îäíà ïåðåìåííàÿ
										{
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												current_element->point_right = multiple_var;
												current_element->prop = arg_v; //óêàçûâàåì ÷òî â ôóíêöèþ ïåðåäàëè àðãóìåíò
												high_pointer = current_element;
												low_pointer = high_pointer;
											}
											else if ((*(temp + 1) == '+') || (*(temp + 1) == '-') || (*(temp + 1) == '*') || (*(temp + 1) == '/') || (*(temp + 1) == '^'))
											{
												current_element->type = funct;
												current_element->prop = arg_v;
												current_element->var = 1; // îäíà ïåðåìåííàÿ
												current_element->point_right = new math_obj(high_pointer); //íàéäåííàÿ ôóíêöèÿ - àðãóìåíò òåêóùåé
																											//êîïèÿ ñëóæåáíîé ïåðåìåííîé ñ óêàçàòåëåì íà ôóíêöèþ
												current_element->point_collar = new math_obj(&temporary_variable);
												current_element->point_collar->var = 0; //ïðîñòî äëÿ äîñòîâåðíîñòè)
												current_element->point_collar->point_collar = current_element;
												current_element->point_left = current_element->point_collar;
												current_element->point_right->point_right = multiple_var;
												current_element->point_right->prop = arg_v;
												high_pointer = current_element->point_left;
												low_pointer = high_pointer;
											}
										}
										else // âñå àðãóìåíòû íå ïåðåìåííûå
										{
											if ((*(temp + 1) == NULL) || (*(temp + 1) == '='))
											{
												current_element->copy(high_pointer);
												current_element->point_right = multiple_var;//â êà÷åñòâå àðãóìåíòà - ìàññèâ îáúåêòîâ
												current_element->prop = arg_c; //óêàçûâàåì ÷òî â ôóíêöèþ ïåðåäàëè àðãóìåíò
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
								//÷èñëî ïåðåìåííûõ - ðàçëè÷íîå 
								else
								{
									if (*(temp + 1) != '=')
									{
										//òóò îøèáêà: ðàíåå áûëà f(x,y), çàïèñàëè f(k)+... - êîëè÷åñòâî ïåðåìåííûõ â âûçûâàåìîé ôóíêöèè íå ñîâïàäàåò
										//TODO:çäåñü íàäî áóäåò íàïèñàòü èñêëþ÷åíèÿ äëÿ ôóíêöèé íåñêîëüêèõ àðãóìåíòîâ, êîòîðûå ìîãóò ïîíèìàòüñÿ êàê ôóíêöèÿ îäíîãî àðãóìåíòà
										//ïðèìåð root(5) - êâàäðàòíûé êîðåíü èç 5, root (5,2) - òîæå ñàìîå, root (5,3) - êóáè÷åñêèé êîðåíü èç 5
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;

									}
									//íî åñëè äàëåå ñòîèò ðàâíî - âñ¸ íîðìàëüíî,  ïåðåîïðåäåëåíèå. 									

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
												temp_pointer->var = count; //òóò ïðîèñõîäèò çàïèñü íîìåðà ïåðåìåííîé â ìàññèâå âíóòðü ñàìîé ïåðåìåííîé. ïîäðàçóìåâàåòñÿ ÷òî ïåðåìåííûå ÍÅ èìåþò óñòàíîâëåííîãî çíà÷åíèÿ => ìîæíî èñïîëüçîâàòü var êàê ñ÷¸ò÷èê

												if (count == 0)
													//ñðàçó æå ïîëó÷àåì óàçàòåëü íà ïåðâóþ ïåðåìåííóþ â ñïèñêå
													current_element->point_collar = temp_pointer;
												else
												{
													//ñîçäàíèå äâóíàïðàâëåííîãî ñïèñêà ïåðåìåííûõ, ãäå íà íóëåâîé ýëåìåíò óêàçûâàåò âîðîòíèê ôóíêöèè.
													temp_pointer->point_right = current_element->var_list_back();
													temp_pointer->point_right->point_left = temp_pointer;
												}
												if (count == comma)
												{
													//ñîçäàíèå çàìêíóòîãî â êîëüöî äâóíàïðàâëåííîãî ñïèñêà ïåðåìåííûõ. Íóëåâîé ýëåìåíò = ïîñëåäíåìó.
													temp_pointer->point_left = current_element->point_collar;
													current_element->point_collar->point_right = temp_pointer;
												}
												//temp_pointer â äàííîì ñëó÷àå óäàëÿòü íåëüçÿ. Ìîæíî çàíóëèòü, íî óäàëÿòü íåëüçÿ
											}
											else
											{
												//ïîñêîëüêó ôóíêöèÿ åù¸ íåîïðåäåë¸ííàÿ ïåðåäàâàòü åé ëþáûå àðãóìåíòû áóäåò íåâåðíî, ò.ê. êîëè÷åñòâî àðãóìåíòîâ íå ñîâïàäàåò
												ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNDEFINED_FUNC);
												return NULL;
											}
											pDest = temp;
										}
										//çàïîìèíàåì ÷èñëî ïåðåìåííûõ
										current_element->var = comma + 1;
										current_element->prop = undef; //ñ÷èòàåì íîâóþ ôóíêöèþ undef, íî ïðè ýòîì ìû çíàåì ÷òî ó íå¸ åñòü èìÿ
										current_element->actn = write;  //íà çàïîëíåíèå		
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
							//òåêóùåå âûðàæåíèå => ôóíêöèÿ, êîïèÿ íàéäåííîé ôóíêöèè - àðãóìåíò âûðàæåíèÿ.
							else if ((*temp == '+') || (*temp == '*') || (*temp == '/') || (*temp == '^') || (*temp == '-'))
							{
								current_element->type = funct;
								current_element->prop = arg_v;
								current_element->point_right = new math_obj(high_pointer); //ïîëàãàåì íîâóþ íåîïðåäåë¸ííóþ ôóíêöèþ, êîòîðàÿ çàâèñèò îò êàêîé-ëèáî ïåðåìåííîé,
																							//èìåþùåé àðãóìåíò â âèäå äðóãîé (îïðåäåë¸ííîé) ôóíêöèè. Ïðè ýòîì åñëè îêàæåòñÿ, ÷òî
																							//â âûðàæåíèè ó÷àñòâóþò íåñêîëüêî ôóíêöèé (f+g), òî ôóíêöèÿ, êîòðàÿ èõ îáúåäèíÿåò
																							//áóäåò ñ÷èòàòüñÿ ôóíêöèåé íåñêîëüêèõ ïåðåìåííûõ, 
																							//äàæå åñëè âñå ôóíêöèè â å¸ âûðàæåíèè çàâèñÿò îò îäíîé è òîé æå ïåðåìåííîé.
								//êîïèÿ ñëóæåáíîé ïåðåìåííîé ñ óêàçàòåëåì íà ôóíêöèþ, êîòîðàÿ èåìååò íîìåð íîëü
								current_element->point_collar = new math_obj(&temporary_variable);
								current_element->point_collar->var = 0; //ïðîñòî äëÿ äîñòîâåðíîñòè)
								current_element->point_collar->point_collar = current_element;
								current_element->point_left = current_element->point_collar;//ò.î. êàæäàÿ ôóíêöèÿ áóäåò èìåòü îäíó è òóæå ñòðóêòóðó è ïîíèìàòüñÿ âñåãäà, 
																							//êàê ôóíêöèÿ çàâèñÿùàÿ îò ïåðåìåííûõ, êîòîðîé â êà÷åòâå àðãóìåíòîâ ÷òî-òî ïåðåäà¸òñÿ
								current_element->var = 1; //êîëè÷åñòâî ïåðåìåííûõ								
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
					//ñèìâîë íàõîäèòñÿ íå â íà÷àëå ñòðîêè
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

						//äàëåå âñåãäà ìîæåò áûòü òîëüêî äâà âàðèàíòà - òåêùóèé ýëåìåíò ëèáî ôóíêöèÿ, ëèáî âûðàæåíèå, ïðè÷¸ì âñåãäà undef.
						//åñëè íå íàéäåí íè îäèí ýëåìåíò ìàññèâà ñ òàêèì èìåíåì
						if (low_pointer == NULL)
						{
							if (*temp == '(),')
							{
								//îøèáêà - ïîñëå ïåðåìåííîé ñòîèò îòêðûâàþùàÿñÿ ñêîáêà. Âîîáùå íå ñîâñåì áåññìûñëåííàÿ çàïèñü, âîçìîæíî íóæåí äðóãîé êîä îøèáêè èëè ïðèäóìàòü êàê òàêîå èñêëþ÷åíèå îáðàáàòûâàòü.
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_BRACKET);
								return NULL;
							}
							//åñëè òåêóùèé ýëåìåíò - ôóíêöèÿ, òî î÷åâèäíî íàéäåíà íîâàÿ ïåðåìåííàÿ â çàïèñè âûðàæåíèÿ. 


							if (current_element->type == funct)
							{
								if (current_element->point_collar->point_right != NULL)
								{
									//çíà÷èò ñïèñîê ïåðåìåííûõ çàìêíóò => íîâàÿ ïåðåìåííàÿ - ëèøíÿÿ
									ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
									return NULL;
								}
								//ñïèñîê íå çàìêíóò - ìîæíî äîáàâèòü áîëüøå ïåðåìåííûõ
								else
								{
									general_var_const->push_back(new math_obj(name, varbl, defnd, 0));
									temp_pointer = current_element->var_list_back();
									temp_pointer->point_left = new math_obj(general_var_const->back());
									temp_pointer->point_left->point_right = temp_pointer;
									temp_pointer->point_left->point_collar = current_element;
									temp_pointer->point_left->var = current_element->var;
									current_element->var += 1;
									//ïîñêîëüêó ñèìâîë íå â íà÷àëå ñòðîêè - çíà÷èò ñòîèò ïîñëå êàêîé-ëèáî îïåðàöèè. 
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//ñòîèò ïîñëå ìèíóñà
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
							//åñëè âûðàæåíèå
							else if (current_element->type == exprs)
							{
								// âûðàæåíèå â undef ôóíêöèþ 
								current_element->type = funct;
								current_element->prop = undef;
								general_var_const->push_back(new math_obj(name, varbl, defnd, 0));
								//êîïèÿ ïåðåìåííîé ñ óêàçàòåëåì íà ôóíêöèþ
								current_element->point_collar = new math_obj(general_var_const->back());
								current_element->point_collar->point_collar = current_element;
								current_element->var = 1;//îäíà ïåðåìåííàÿ
								//òóò ïðîáëåìà ìîæåò âîçíèêíóòü - ÿ íå ïîìíþ êàêàÿ))
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
						//íàéäåí ýëåìåíò ìàññèâà ñ ñîâïàäàþùèì èìåíåì - ïåðåìåííàÿ
						else if (low_pointer->type == varbl)
						{
							if (*temp == '(),')
							{
								//îøèáêà - ïîñëå ïåðåìåííîé ñòîèò îòêðûâàþùàÿñÿ ñêîáêà. Âîîáùå íå ñîâñåì áåññìûñëåííàÿ çàïèñü, âîçìîæíî íóæåí äðóãîé êîä îøèáêè èëè ïðèäóìàòü êàê òàêîå èñêëþ÷åíèå îáðàáàòûâàòü.
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
										//çíà÷èò ñïèñîê ïåðåìåííûõ çàìêíóò => íîâàÿ ïåðåìåííàÿ - ëèøíÿÿ
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
										return NULL;
									}
									else
									{
										if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
										{
											high_pointer->point_right->point_right = temp_pointer;//óêàçàòåëü íà íóæíîå ìåñòî â ñïèñêå ïåðåìåííûõ
										}
										else
										{
											high_pointer->point_right = temp_pointer;
										}
										low_pointer = high_pointer->point_right;
									}
								}
								//ñïèñîê íå çàìêíóò - ìîæíî äîáàâèòü áîëüøå ïåðåìåííûõ
								else if (temp_pointer == NULL) //åñëè íåò ñîâïàäåíèé èì¸í ïåðåìåííûõ - íàéäåíà íîâàÿ ïåðåìåííàÿ
								{
									temp_pointer = current_element->var_list_back();
									temp_pointer->point_left = new math_obj(low_pointer);
									temp_pointer->point_left->point_right = temp_pointer;
									temp_pointer->point_left->point_collar = current_element;
									temp_pointer->point_left->var = current_element->var;
									current_element->var += 1;
									//ïîñêîëüêó ñèìâîë íå â íà÷àëå ñòðîêè - çíà÷èò ñòîèò ïîñëå êàêîé-ëèáî îïåðàöèè. 
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))//ñòîèò ïîñëå ìèíóñà
									{
										high_pointer->point_right->point_right = temp_pointer->point_left;
									}
									else
									{
										high_pointer->point_right = temp_pointer->point_left;
									}
									low_pointer = high_pointer->point_right;
								}
								else //ïðè ñîâïàäåíèè ïðîñòî çàïèñûâàåì ïåðåìåííóþ â âûðàæåíèå
								{
									if ((high_pointer->point_right != NULL) && (high_pointer->point_right->name == L"minus"))
									{
										high_pointer->point_right->point_right = temp_pointer;//óêàçàòåëü íà íóæíîå ìåñòî â ñïèñêå ïåðåìåííûõ
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
								//êîïèÿ ïåðåìåííîé ñ óêàçàòåëåì íà ôóíêöèþ
								current_element->point_collar = new math_obj(low_pointer);
								current_element->point_collar->point_collar = current_element;
								current_element->var = 1;//îäíà ïåðåìåííàÿ
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
						//íàéäåí ýëåìåíò ìàññèâà ñ ñîâïàäàþùèì èìåíåì - êîíñòàíòà - ïðîñòî çàïèñûâàåì å¸ â âûðàæåíèå
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
						//íàéäåí ýëåìåíò ìàññèâà ñ ñîâïàäàþùèì èìåíåì - ôóíêöèÿ 
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
									//åñëè ïîïàëè â êîíåö ñòðî÷êè íå íàéäÿ çàêðûâàþùèõ ñêîáîê
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
									else if (*temp == '=') //îòêðûòàÿ ñêîáêà è äàëüøå ðàâíî
									{
										ProjectError::SetProjectLastError(ProjectError::ErrorCode::LBRACKET_NOT_CLOSED);
										return NULL;
									}
									else if (*temp == ',')
									{
										//ñ÷èòàåì çàïÿòûå. êîëè÷åñòâî çàïÿòûõ â ñêîáêàõ +1 = ÷èñëî ðàçëè÷íûõ ïåðåìåííûõ
										comma++;
									}
								}
								//åñëè êîëè÷åñòâî àðãóìåíòîâ íå ñîâïàäàåò ñ êîëè÷åñòâîì ïåðåìåííûõ
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
									//çàïîëíÿåì ìàññèâ àðãóìåíòîâ
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
										//òóò â êà÷åñòâå àðãóìåíòà ïîëó÷àåòñÿ ïåðåìåííàÿ ôóíêöèÿ, êîòîðàÿ ëèáî defnd, ëèáî undef
										if (temp_pointer->prop != arg_c)
										{
											count_var++;
											if ((temp_pointer->prop == defnd) || (temp_pointer->prop == undef))
												multiple_var[count].copy(temp_pointer);
											else if (temp_pointer->prop == arg_v)
											{
												multiple_var[count].copy(temp_pointer);
												temp_pointer->point_right = NULL;//çàíóëÿåì ìàññèâ àðãóìåíòîâ. Áåç î÷èñòêè!
											}
										}
										else //òóò âåãäà ïîäðàçóìåâàåòñÿ defnd ôóíêöèÿ ñ êîíñò àðãóìåíòàìè										
											multiple_var[count].copy(temp_pointer);
									}
									pDest = temp;
								}
							}
							else if ((*temp == NULL) || (*temp == '+') || (*temp == '-') || (*temp == '*') || (*temp == '/') || (*temp == '^'))
							{
								count_var = -1;
							}
							else
							{
								ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEXPECTED_SYMBOL);
								return NULL;
							}
							if (current_element->type == funct)
							{
								//åñëè âñå àðãóìåíòû ïîëó÷åííûå îò ôóíêöèè ëèáî êîíñòàíòû, ëèáî êîíñò. âûðàæåíèÿ, ëèáî ôóíêöèè ñ êîíñò àðãóìåíòàìè
								if (count_var == 0)
								{
									temp_pointer = new math_obj(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_c;//óêàçûâàåì ÷òî àðãóìåíòû - êîíñòàíòû
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
								//â êà÷åñòâå àðãóìåíòîâ åñòü ïåðåìåííûå
								else
								{
									temp_pointer = new math_obj(low_pointer);
									if (count_var != -1)
									{
										temp_pointer->point_right = multiple_var;
										multiple_var = NULL;
										temp_pointer->prop = arg_v;//óêàçûâàåì ÷òî ñðåäè àðãóìåíòîâ åñòü ïåðåìåííûå
									}
									//åñëè ñïèñîê ïåðåìåííûõ íå çàìêíóò
									if (current_element->point_collar->point_right == NULL)
									{
										current_element->arg_copy_plus_1();
										current_element->point_right[(int)current_element->var - 1] = temp_pointer;
										temp_pointer = NULL;
										temp_pointer = current_element->var_list_back();
										temp_pointer->point_left = new math_obj(&temporary_variable);
										temp_pointer->point_left->point_right = temp_pointer;
										temp_pointer->point_left->var = current_element->var - 1;
									}
									//çàìêíóò
									else
									{
										//åñëè ñðåäè ïåðåìåííûõ àðãóìåíòîâ íàéäåííîé ôóíêöèè åñòü õîòü îäèí íå âõîäÿùèé â çàìêíóòûé ñïèñîê
										if (current_element->var_list_compare(temp_pointer) != 0)
										{
											ProjectError::SetProjectLastError(ProjectError::ErrorCode::UNEQUAL_NUM_OF_VAR);
											return NULL;
										}
										//âñå ïåðåìåííûå íàéäåííîé ôóíêöèè âõîäÿò â ñïèñîê ïåðåìåííûõ òåêóùåãî ýëåìåíòà
										else
										{
											//êîïèÿ íàéäåííîé ôóíêöèè äîáàâëÿåòñÿ â âûðàæåíèå
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
								//òåêóùèé ýëåìåíò îñòà¸òñÿ âûðàæåíèåì.
								if (count_var == 0)
								{
									temp_pointer = new math_obj(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_c;//óêàçûâàåì ÷òî àðãóìåíòû - êîíñòàíòû

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
								//òåêóùèé ýëåìåíò ñòàíîâèòñÿ ôóíêöèåé.
								else
								{
									temp_pointer = new math_obj(low_pointer);
									temp_pointer->point_right = multiple_var;
									multiple_var = NULL;
									temp_pointer->prop = arg_v;//óêàçûâàåì ÷òî ñðåäè àðãóìåíòîâ åñòü ïåðåìåííûå
									current_element->type = funct;
									current_element->prop = arg_v;
									current_element->point_right = temp_pointer; //ïîëàãàåì íîâóþ íåîïðåäåë¸ííóþ ôóíêöèþ, êîòîðàÿ çàâèñèò îò êàêîé-ëèáî ïåðåìåííîé,
																								//èìåþùåé àðãóìåíò â âèäå äðóãîé (îïðåäåë¸ííîé) ôóíêöèè. Ïðè ýòîì åñëè îêàæåòñÿ, ÷òî
																								//â âûðàæåíèè ó÷àñòâóþò íåñêîëüêî ôóíêöèé (f+g), òî ôóíêöèÿ, êîòðàÿ èõ îáúåäèíÿåò
																								//áóäåò ñ÷èòàòüñÿ ôóíêöèåé íåñêîëüêèõ ïåðåìåííûõ, 
																								//äàæå åñëè âñå ôóíêöèè â å¸ âûðàæåíèè çàâèñÿò îò îäíîé è òîé æå ïåðåìåííîé.
																								//êîïèÿ ñëóæåáíîé ïåðåìåííîé ñ óêàçàòåëåì íà ôóíêöèþ, êîòîðàÿ èåìååò íîìåð íîëü
									current_element->point_collar = new math_obj(&temporary_variable);
									current_element->point_collar->var = 0; //ïðîñòî äëÿ äîñòîâåðíîñòè)
									current_element->point_collar->point_collar = current_element;
									current_element->point_left = current_element->point_collar;//ò.î. êàæäàÿ ôóíêöèÿ áóäåò èìåòü îäíó è òóæå ñòðóêòóðó è ïîíèìàòüñÿ âñåãäà, 
																								//êàê ôóíêöèÿ çàâèñÿùàÿ îò ïåðåìåííûõ, êîòîðîé â êà÷åòâå àðãóìåíòîâ ÷òî-òî ïåðåäà¸òñÿ
									current_element->var = 1; //êîëè÷åñòâî ïåðåìåííûõ
									low_pointer = high_pointer->point_right;
								}
							}
						}
					}
				}
			}
			return current_element;
		}


		/*Ôóíêöèÿ âûïîëíÿåò ïðîâåðêó/àíàëèç ðåçóëüòàòà çàïîëíåíèÿ äåðåâà îïåðàöèé è çàïðîñîâ ïîëüçîâàòåëÿ.
		Âîçâðàùàåò ñòðîêó ñ ðåçóëüòàòîì òåêóùåé èòåðàöèè âû÷èñëåíèé.*/
		wstring analized_output(wchar_t* _pDest, wchar_t* _endPtr, math_obj* _current_element)
		{
			math_obj* CE = filling_vector(_pDest, _endPtr, _current_element, 0);
			math_obj* temp;
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
				// íó äåëàòü òóò êàê áû íè÷åãî íå íàäî. Ïîëüçîâàòåëü âáèë óðàâíåíèå è îíî ïðîñòî çàïèñàëîñü. Êîãäà íàó÷èìñÿ äåëàòü ïàððàëåëüíûå ïîòîêè âû÷èñëåíèé ìîæíî íà ýòîì
				//ìîìåíòå ðåøàòü ýòî óðàâíåíèå ñ íèçêèì ïðèîðèòåòîì, ÷òîáû êîãäà ïîëüçîâàòåëü çàïðîñèë îòâåò - âû÷èñëåíèÿ çàíèìàëè ìåíüøå âðåìåíè - äëÿ íåãî.
				//mutex* mut=new mutex();
				//thread tTest(testfunc);	//ñðàçó çàïóñêàåòñÿ è ïàøåò
				//thread* ptTest;	//äëÿ ãëîáàëüíîé âèäèìîñòè ïîòîêà, ìîæíî ãëîáàëüíî õðàíèòü íà íåãî óêàçàòåëü
				//ptTest = new thread(testfunc);	//èíèöèàëèçèðîâàòü, è îí ñðàçó çàïóñòèòüñÿ
				//mut->unlock();
				general_var_const->push_back(CE);
			}
			else if (CE->type == funct)
			{
				if (CE->actn == solve)//òóò åù¸ óñëîâèå
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
					//îïÿòü æå, âèäèìîé ðåàêöèè îò ïðîãðàììû áûòü íå äîëæíî. Çàáèëè ôóíêöèþ - çàïèñàëè. Âîçìîæíî â îòäåëüíûé ïîòîê îòäàòü ðàçëîæåíèå ñëîæíîé ôóíêöèè 
					//íà ýëåìåíòàðíûå. Ìîæíî è íå â îòäåëüíûé, âðÿäëè òàì áóäåò âûñîêàÿ ñëîæíîñòü âû÷èñëåíèé
					if (CE->name.size() != 0)
					{
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
				else
				{
					//âðîäå áû íå ìîæåò áûòü òàêîãî âàðèàíòà, íî íà âñÿêèé ñëó÷àé
					CE->tree_destruct();
					delete CE;
				}
			}
			else if (CE->type == cnst)
			{
				if (CE->actn == solve)
				{
					//îòâåò 
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
						//çàïîëíèëè - ïîñ÷èòàëè
						CE->arithmetic();
						CE->tree_destruct();
					}
				}
			}
			else if (CE->type == varbl)
			{		
				delete CE;
				//îäèí èç âàðèàíòîâ çàïðîñà íà ðåøåíèå óðàâíåíèÿ (ñêîðåå âñåãî - ïîñëåäíåãî çàïèñàííîãî)
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

		/*Ôóíêöèÿ ÿâëÿåòñÿ òî÷êîé âõîäà â âû÷èñëèòåëüíîå ÿäðî.
		Âîçâðàùàåò ñòðîêó ñ ðåçóëüòàòîì òåêóùåé èòåðàöèè âû÷èñëåíèé.*/
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
			return analized_output(point_start, point_end, new math_obj(L"", exprs, undef, 0));
		}
	}
}
