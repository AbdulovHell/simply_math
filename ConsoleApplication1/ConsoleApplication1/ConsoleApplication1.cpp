// ConsoleApplication1.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <vector>
#include <malloc.h> 

using namespace std;

char* err_str(int pos) {
	char* str = (char*)malloc(pos + 1);
	for (int i = 0;i < pos;i++)
		str[i] = ' ';
	str[pos] = 0;
	str[pos - 1] = '^';
	return str;
}

char* VerifyInput(char* input) {	//âîçâðàùàåò ñòðîêó, îïèñûâàþùóþ îøèáêó, èíà÷å nullptr, 0 êàðî÷.
	//char* EndStr = &input[strlen(input)];
	//char* cursor = input;
	int len = strlen(input);
	int left_bracket = 0, right_bracket = 0, ravno = 0;
	char buf[300];

	for (int i = 0;i < len;i++)
		if (input[i] == '(')
			left_bracket++;
		else if (input[i] == ')')
			right_bracket++;
		else if (input[i] == '=')
			ravno++;
		else if ((input[i] >= 0x2A && input[i] <= 0x39) || (input[i] >= 0x41 && input[i] <= 0x5A) || (input[i] >= 0x61 && input[i] <= 0x7A)) {}
		else {
			sprintf(buf,"\nBad symbol, '%c':\n", input[i]);
			strcat(buf,input);
			strcat(buf,"\n");
			strcat(buf,err_str(i));
			strcat(buf,"\n");
			return buf;
		}
	if (left_bracket != right_bracket) {
			sprintf(buf,"\n ( and ) error.\n");
			return buf;
	}
	if (ravno > 1) {
			sprintf(buf,"\n'='>1 error.\n");
			return buf;
	}
	/*
	while (cursor < EndStr) {

	}*/
		return NULL;
}

class var_const {
public:
	var_const()
	{
		var_id = "";
		var = 0;
		point_left = NULL;
		point_right = NULL;
	}

	var_const(string _name, double _num)
	{
		var_id = _name;
		var = _num;
		point_left = NULL;
		point_right = NULL;
	}

	var_const(string _name, double _num, var_const * _pl, var_const *pr) {
		var_id = _name;
		var = _num;
		point_left = _pl;
		point_right = pr;
	}

	void copy(var_const var1)
	{
		var_id = var1.var_id;
		var = var1.var;
		point_left = var1.point_left;
		point_right = var1.point_right;
	}

	double recursion(var_const *pointer)
	{
		char operation = pointer[0].var_id[0];
		switch (operation) {
		case '0':
			return pointer[0].var;
		case'1':
			return recursion(pointer[0].point_left) + recursion(pointer[0].point_right);
		case '2':
			return recursion(pointer[0].point_left) * recursion(pointer[0].point_right);
		case '3':
			return recursion(pointer[0].point_left) / recursion(pointer[0].point_right);
			/*case '4':
			return pow(recursion(pointer[0].point_left), recursion(pointer[0].point_right));
			*/
		}
	}

	void arithmetic()
	{
		var = recursion(point_left);
	}


	string var_id;
	double var;
	var_const *point_left;
	var_const *point_right;
};

int input_to_analize(vector<var_const>* input_var_const)  //ïîãîäè. êàê ïåðåäà¸òñÿ óêàçàòåëü  íà âåêòîð â òàêîé çàïèñè? ýòî ÿñíî
{
	

	//"âîçâðàùàåò óêàçàòåëü íà ïåðâûé ýëåìåíò â âåêòîðå" òàê ïîìèìî òîãî, ÷òî òû íå ïîëó÷àåøü ñàì âåêòîð, òàê åùå è óòå÷êè â ïàìÿòè äåëàåøü ò.å. data() íå èñïîëüçóåì? 
	//â äàííîì ñëó÷àå ýòî íå òî, ÷òî íàäî, ÿ ñäåëàë äî ýòîãî, êàê äîëæíî áûòü. ñóïåð, òîãäà êàê ïðàâèëüíî ïîëó÷àòü äîñòóâ ê ýëåìåíòàì âåêòîðà
	
	string sInput;
	cin >> sInput;


	char* input = (char*)malloc(sInput.size());
	strcpy(input, sInput.c_str());
	
	///íîâûé âàðèàíò, ïðîáåëû êóøàåò, ïàìÿòü äèíàìè÷åñêàÿ, íî ïîêà free íå ðàáîòàåò è íàäî êàê-òî ó÷åñòü, ÷òî ñòèðàòü òî íåëüçÿ)
	///âîò åñëè áû ïåðåäåëàòü óæå ñ ãðàôè÷åñêèì èíòåðôåéñîì...
	//char* error_str = VerifyInput(input);
	///if (error_str!=NULL)
	//	return -1;
	///
	int input_size = strlen(input);

	char* pDest = input;	//start pointer
	char* endPtr = input + strlen(input) - 1;	//end pointer
	char *temp = NULL;

	
	var_const *high_pointer = NULL;
	var_const *low_pointer = NULL;

	char* varnameDest = strstr(input, "=");

	char *varname = (char*)malloc(varnameDest - input + 1);
	memcpy(varname, input, varnameDest - input);
	varname[varnameDest - input] = 0;

	int current_size_of_vect = input_var_const->size(); //ðàâåí 2, çíà÷èò ÿâëÿåòñÿ èíäåêñîì òðåòüåãî ýëåìåíòà
	input_var_const->push_back(var_const(varname, 0));
	input_var_const->reserve(input_size*2);

	cout << "size = " << input_var_const->size() << "   capacity = " << input_var_const->capacity() << endl;
	
	int temp_size_of_vect;
	pDest = varnameDest + 1;
	var_const *k = &input_var_const->at(current_size_of_vect);	//âûêèíü óæå ýòîò 4ewnfjk
	//current_size_of_vect íóæíî ñîõðàíèòü ïîñëå âñåõ ïðèñâèâàíèé))


	int brakets_counter = 0;
	while (pDest <= endPtr) {

		if (*pDest == '+')
		{
			//îïåðàöèÿ ñëîæåíèÿ èìååò id "1"	
			temp_size_of_vect = input_var_const->size();
			//åñëè ýòî ïåðâàÿ îïåðàöèÿ â âûðàæåíèè
			if (high_pointer == low_pointer)
			{
				input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî
				input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//ëåâûé ðóêàâ òåêóùåé âû÷èñëÿåìîé êîíñòàíòû óêàçûâàåò íà ñîçäàííóþ îïåðàöèþ
				high_pointer = &input_var_const->at(temp_size_of_vect);// âåðõíèé óêàçàòåëü -- íà ñîçäàííóþ îïåðàöèþ
			}
			//åñëè áûëà êàêàÿ-ëèáî îïåðàöèÿ äî ýòîãî
			else
			{
				//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÌÅÍÜØÅ! èëè ðàâåí  ïðèîðèòåòó òåêóùåé
				if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
				{
					input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî
					high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //ïðàâûé ðóêàâ ïðåäûäóùåé -> íà ñîçäàííóþ
					high_pointer = &input_var_const->at(temp_size_of_vect);                 //âåðõíèé óêàçàòåëü -> íà ñîçäàííóþ îïåðàöèþ
				}
				//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÁÎËÜØÅ! ÷åì ïðèîðèòåò òåêóùåé
				else
				{
					//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ îïåðàöèþ

					//åñëè ïðèîðèòåò îïåðàöèè ñ íàèìåíüøèì ïðèîðèòåòîì (íà êîòîðóþ óêàçûâàåò ëåâûé ðóêàâ êîíñòàííû)  !ÁÎËÜØÅ! ïðèîðèòåòà òåêóùåé
					if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
					{
						//çàïèñûâàåì îïåðàöèþ êàê ñàìóþ ë¸ãêóþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ íàèëåã÷àéøóþ îïåðàöèþ
						input_var_const->push_back(var_const("1", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
						//óêàçûâàåì ëåâûì ðóêàâîì êîíñòàííòû íà ñîçäàííóþ îïåðàöèþ
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


					}

					//åñëè ïðèîðèòåò îïåðàöèè ñ íàèìåíüøèì ïðèîðèòåòîì (íà êîòîðóþ óêàçûâàåò ëåâûé ðóêàâ êîíñòàííû)  !ÌÅÍÜØÅ! ïðèîðèòåòà òåêóùåé, 
					//ò.å. îïåðàöèÿ ïðèîðèòåíî ÃÄÅ-ÒÎ ìåæäó ïîñëåäíåé è íàèëåã÷àéøåé
					else
					{
						input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//òóò âñ¸ íåïðàâèëüíî
					}
					high_pointer = &input_var_const->at(temp_size_of_vect);  //âåðõíèé óêàçàòåëü íà ñîçäàííóþ îïåðàöèþ

				}
			}
			pDest++;
		}
		else if (*pDest == '*')
		{                     //îïåðàöèÿ óìíîæåíèÿ èìååò id "2"		
			temp_size_of_vect = input_var_const->size();
			//åñëè ýòî ïåðâàÿ îïåðàöèÿ â âûðàæåíèè
			if (high_pointer == low_pointer)
			{
				input_var_const->push_back(var_const("2", brakets_counter, low_pointer, NULL));  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî
				input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//ëåâûé ðóêàâ òåêóùåé âû÷èñëÿåìîé êîíñòàíòû óêàçûâàåò íà ñîçäàííóþ îïåðàöèþ
				high_pointer = &input_var_const->at(temp_size_of_vect);// âåðõíèé óêàçàòåëü -- íà ñîçäàííóþ îïåðàöèþ
			}
			//åñëè áûëà êàêàÿ-ëèáî îïåðàöèÿ äî ýòîãî
			else
			{
				//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÌÅÍÜØÅ! èëè ðàâåí  ïðèîðèòåòó òåêóùåé
				if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
				{
					input_var_const->push_back(var_const("2", brakets_counter, low_pointer, NULL));  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî
					high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //ïðàâûé ðóêàâ ïðåäûäóùåé -> íà ñîçäàííóþ
					high_pointer = &input_var_const->at(temp_size_of_vect);                 //âåðõíèé óêàçàòåëü -> íà ñîçäàííóþ îïåðàöèþ
				}
				//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÁÎËÜØÅ! ÷åì ïðèîðèòåò òåêóùåé
				else
				{
					//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ îïåðàöèþ

					//åñëè ïðèîðèòåò îïåðàöèè ñ íàèìåíüøèì ïðèîðèòåòîì (íà êîòîðóþ óêàçûâàåò ëåâûé ðóêàâ êîíñòàííû)  !ÁÎËÜØÅ! ïðèîðèòåòà òåêóùåé
					if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
					{
						//çàïèñûâàåì îïåðàöèþ êàê ñàìóþ ë¸ãêóþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ íàèëåã÷àéøóþ îïåðàöèþ
						input_var_const->push_back(var_const("2", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
						//óêàçûâàåì ëåâûì ðóêàâîì êîíñòàííòû íà ñîçäàííóþ îïåðàöèþ
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


					}

					//åñëè ïðèîðèòåò îïåðàöèè ñ íàèìåíüøèì ïðèîðèòåòîì (íà êîòîðóþ óêàçûâàåò ëåâûé ðóêàâ êîíñòàííû)  !ÌÅÍÜØÅ! ïðèîðèòåòà òåêóùåé, 
					//ò.å. îïåðàöèÿ ïðèîðèòåíî ÃÄÅ-ÒÎ ìåæäó ïîñëåäíåé è íàèëåã÷àéøåé
					else
					{
						input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//òóò âñ¸ íåïðàâèëüíî
					}
					high_pointer = &input_var_const->at(temp_size_of_vect);  //âåðõíèé óêàçàòåëü íà ñîçäàííóþ îïåðàöèþ

				}
			}
			pDest++;
		}
		else if (*pDest == '/')
		{                     //îïåðàöèÿ äåëåíèÿ èìååò id "3"
			temp_size_of_vect = input_var_const->size();
			//åñëè ýòî ïåðâàÿ îïåðàöèÿ â âûðàæåíèè
			if (high_pointer == low_pointer)
			{
				input_var_const->push_back(var_const("3", brakets_counter, low_pointer, NULL));  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî
				input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//ëåâûé ðóêàâ òåêóùåé âû÷èñëÿåìîé êîíñòàíòû óêàçûâàåò íà ñîçäàííóþ îïåðàöèþ
				high_pointer = &input_var_const->at(temp_size_of_vect);// âåðõíèé óêàçàòåëü -- íà ñîçäàííóþ îïåðàöèþ
			}
			//åñëè áûëà êàêàÿ-ëèáî îïåðàöèÿ äî ýòîãî
			else
			{
				//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÌÅÍÜØÅ! èëè ðàâåí  ïðèîðèòåòó òåêóùåé
				if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
				{
					input_var_const->push_back(var_const("3", brakets_counter, low_pointer, NULL));  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî
					high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //ïðàâûé ðóêàâ ïðåäûäóùåé -> íà ñîçäàííóþ
					high_pointer = &input_var_const->at(temp_size_of_vect);                 //âåðõíèé óêàçàòåëü -> íà ñîçäàííóþ îïåðàöèþ
				}
				//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÁÎËÜØÅ! ÷åì ïðèîðèòåò òåêóùåé
				else
				{
					//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ îïåðàöèþ

					//åñëè ïðèîðèòåò îïåðàöèè ñ íàèìåíüøèì ïðèîðèòåòîì (íà êîòîðóþ óêàçûâàåò ëåâûé ðóêàâ êîíñòàííû)  !ÁÎËÜØÅ! ïðèîðèòåòà òåêóùåé
					if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
					{
						//çàïèñûâàåì îïåðàöèþ êàê ñàìóþ ë¸ãêóþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ íàèëåã÷àéøóþ îïåðàöèþ
						input_var_const->push_back(var_const("3", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
						//óêàçûâàåì ëåâûì ðóêàâîì êîíñòàííòû íà ñîçäàííóþ îïåðàöèþ
						input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


					}

					//åñëè ïðèîðèòåò îïåðàöèè ñ íàèìåíüøèì ïðèîðèòåòîì (íà êîòîðóþ óêàçûâàåò ëåâûé ðóêàâ êîíñòàííû)  !ÌÅÍÜØÅ! ïðèîðèòåòà òåêóùåé, 
					//ò.å. îïåðàöèÿ ïðèîðèòåíî ÃÄÅ-ÒÎ ìåæäó ïîñëåäíåé è íàèëåã÷àéøåé
					else
					{
						input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//òóò âñ¸ íåïðàâèëüíî
					}
					high_pointer = &input_var_const->at(temp_size_of_vect);  //âåðõíèé óêàçàòåëü -> íà ñîçäàííóþ îïåðàöèþ

				}
			}
			pDest++;
			}
		else if (*pDest == '-')
			{
			//îïåðàöèÿ ìèíóñ ïîíèìàåòñÿ êàê ñëîæåíèå ñ îòðèöàòåëüíûì ÷èñëîì, id "1"
			temp_size_of_vect = input_var_const->size();
			//åñëè âûðàæåíèå íà÷èíàåòñÿ ñ ìèíóñà (ñëó÷àé êîãäà ïîñëå ìèíóñà èä¸ò ÷èñëî, ìèíóñ ïåðåä ñêîáêî íå ðåàëèçîâàí)
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //ñîçäàíèå ýëåìåíòà êëàññà è çàïèñü ÷èñëà
				//îáà óêàçàòåëÿ -> íà ÷èñëî, òåáóåòñÿ äëÿ ïðîâåðêè óñëîâèÿ ïðè çàïèñè îïåðàöèè
				high_pointer = &input_var_const->at(temp_size_of_vect);
				low_pointer = &input_var_const->at(temp_size_of_vect);
				//ëåâûé ðóêàâ âû÷èñëÿåìîé êîíñòàíòû -> ñîçäàííóþ ñòðóêòóðó ñ ÷èñëîì.
				input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
			}
			//åñëè áûëè äðóãèå ÷èñëà/îïåðàöèÿ ïåðåä ìèíóñîì
			else
			{
				//åñëè ýòî ïåðâàÿ îïåðàöèÿ â âûðàæåíèè (ñëîæåíèå)
				if (high_pointer == low_pointer)
				{
					input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //ñíà÷àëà çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî
					input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);//ëåâûé ðóêàâ òåêóùåé âû÷èñëÿåìîé êîíñòàíòû óêàçûâàåò íà ñîçäàííóþ îïåðàöèþ
					high_pointer = &input_var_const->at(temp_size_of_vect);// âåðõíèé óêàçàòåëü -- íà ñîçäàííóþ îïåðàöèþ
					temp_size_of_vect = input_var_const->size(); //ñ÷¸ò÷èê ïðîõîäà ïî ìàññèâó óâåëè÷èâàåòñÿ åù¸ ðàç äëÿ çàïèñè ñàìîãî ÷èñëà
					input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //ñîçäàíèå ýëåìåíòà êëàññà è çàïèñü ÷èñëà
					 //íèæíèé óêàçàòåëü -> íà ñîçäàííîå ÷èñëî
					low_pointer = &input_var_const->at(temp_size_of_vect);
					//Ïðàâûé ðóêàâ ïðåäøåñòâóþùåé îïåðàöèè íà ñîçäàííîå ÷èñëî
				high_pointer[0].point_right = low_pointer;
			}
				//åñëè ðàíåå áûëè äðóãèå îïåðàöèè
			else
			{
					//åñëè îïåðàöèÿ âû÷èòàíèÿ ñòîèò â çàïèñè ïîñëå îïåðàöèè, ñòîÿùåé ïåðåä ñêîáêîé (ò.å. âû÷èòàíèå ñòîèò â ñêîáêàõ)
					if (brakets_counter > high_pointer[0].var)
						//åñëè ïðåäûäóùàÿ îïåðàöèÿ çàïîëíåíà äî êîíöà (ïðàâûé ðóêàâ íà ÷òî-òî óêàçûâàåò)
						if (high_pointer[0].point_right != NULL)
			{
							input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));//ñíà÷àëà çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);//çàïîëíÿåì ïðåäûäóùóþ îïåðàöèþ äî êîíöà
							high_pointer = &input_var_const->at(temp_size_of_vect);// âåðõíèé óêàçàòåëü -- íà ñîçäàííóþ îïåðàöèþ
							temp_size_of_vect = input_var_const->size(); //ñ÷¸ò÷èê ïðîõîäà ïî ìàññèâó óâåëè÷èâàåòñÿ åù¸ ðàç äëÿ çàïèñè ñàìîãî ÷èñëà
							input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //ñîçäàíèå ýëåìåíòà êëàññà è çàïèñü ÷èñëà
							//íèæíèé óêàçàòåëü -> íà ñîçäàííîå ÷èñëî
							low_pointer = &input_var_const->at(temp_size_of_vect);
							//Ïðàâûé ðóêàâ ïðåäøåñòâóþùåé îïåðàöèè íà ñîçäàííîå ÷èñëî
				high_pointer[0].point_right = low_pointer;
			}
					//åñëè ïðåäûäóùàÿ îïåðàöèÿ íå çàïîëíåíà äî êîíöà (ïðàâûé ðóêàâ íè íà ÷òî íå óêàçûâàåò). 
					//Ðàâíîñèëüíî ñèòóàöèè í-ð: 7*(-2+5), ò.å. çíàê ìèíóñ ñòîèò ñðàçó ïîñëå ñêîáêè
			else
			{
							input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //ñîçäàíèå ýëåìåíòà êëàññà è çàïèñü ÷èñëà
							low_pointer = &input_var_const->at(temp_size_of_vect);//íèæíèé óêàçàòåëü -> íà ñîçäàííîå ÷èñëî
							//Ïðàâûé ðóêàâ ïðåäøåñòâóþùåé îïåðàöèè íà ñîçäàííîå ÷èñëî. Â äàííîì ñëó÷àå ìîæåò áûòü áåññìûñëåííîå äåéñòâèå
				high_pointer[0].point_right = low_pointer;
			}
					//åñëè îïåðàöèÿ âû÷èòàíèÿ ñòîèò â çàïèñè ïîñëå îïåðàöèè, ñòîÿùåé âíóòðè îäíèõ è òåõ æå ñêîáîê (èëè âîîáùå áåç íèõ)
			else
			{
						//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÌÅÍÜØÅ! èëè ðàâåí  ïðèîðèòåòó òåêóùåé
						if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
			{
							input_var_const->push_back(var_const("1", brakets_counter, low_pointer, NULL));  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùåå ÷èñëî
							high_pointer[0].point_right = &input_var_const->at(temp_size_of_vect);  //ïðàâûé ðóêàâ ïðåäûäóùåé -> íà ñîçäàííóþ
							high_pointer = &input_var_const->at(temp_size_of_vect);                 //âåðõíèé óêàçàòåëü -> íà ñîçäàííóþ îïåðàöèþ
			}
						//åñëè ïðèîðèòåò ïðåäûäóùåé îáðàáîòàííîé îïåðàöèè !ÁÎËÜØÅ! ÷åì ïðèîðèòåò òåêóùåé
			else
			{
							//input_var_const->at(temp_size_of_vect) = var_const("1", brakets_counter, high_pointer, NULL);  //çàïèñûâàåì îïåðàöèþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ îïåðàöèþ

							//åñëè ïðèîðèòåò îïåðàöèè ñ íàèìåíüøèì ïðèîðèòåòîì (íà êîòîðóþ óêàçûâàåò ëåâûé ðóêàâ êîíñòàííû)  !ÁÎËÜØÅ! ïðèîðèòåòà òåêóùåé
							if ((strtod(input_var_const->at(current_size_of_vect).point_left[0].var_id.c_str(), &temp) + input_var_const->at(current_size_of_vect).point_left[0].var) > (brakets_counter + 1))
			{
								//çàïèñûâàåì îïåðàöèþ êàê ñàìóþ ë¸ãêóþ, ëåâûé ðóêàâ -> íà ïðåäûäóùóþ íàèëåã÷àéøóþ îïåðàöèþ
								input_var_const->push_back(var_const("1", brakets_counter, input_var_const->at(current_size_of_vect).point_left, NULL));
								//óêàçûâàåì ëåâûì ðóêàâîì êîíñòàííòû íà ñîçäàííóþ îïåðàöèþ
								input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);


			}

							//åñëè ïðèîðèòåò îïåðàöèè ñ íàèìåíüøèì ïðèîðèòåòîì (íà êîòîðóþ óêàçûâàåò ëåâûé ðóêàâ êîíñòàííû)  !ÌÅÍÜØÅ! ïðèîðèòåòà òåêóùåé, 
							//ò.å. îïåðàöèÿ ïðèîðèòåíî ÃÄÅ-ÒÎ ìåæäó ïîñëåäíåé è íàèëåã÷àéøåé
			else
			{
								input_var_const->at(current_size_of_vect).point_left[0].point_right = &input_var_const->at(temp_size_of_vect);//òóò âñ¸ íåïðàâèëüíî
			}
							high_pointer = &input_var_const->at(temp_size_of_vect);  //âåðõíèé óêàçàòåëü íà ñîçäàííóþ îïåðàöèþ			
			}
						temp_size_of_vect = input_var_const->size(); //ñ÷¸ò÷èê ïðîõîäà ïî ìàññèâó óâåëè÷èâàåòñÿ åù¸ ðàç äëÿ çàïèñè ñàìîãî ÷èñëà
						input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //ñîçäàíèå ýëåìåíòà êëàññà è çàïèñü ÷èñëà
						//íèæíèé óêàçàòåëü -> íà ñîçäàííîå ÷èñëî
						low_pointer = &input_var_const->at(temp_size_of_vect);
						//Ïðàâûé ðóêàâ ïðåäøåñòâóþùåé îïåðàöèè íà ñîçäàííîå ÷èñëî
				high_pointer[0].point_right = low_pointer;
			}
			}
			}
				}
		else if (*pDest == '(')
		{                  //ëåâàÿ ñêîáêà óâåëè÷èâàåò ñ÷¸ò÷èê ñêîáîê íà ÷èñëî, ðàâíîå ñàìîìó âûñîêîìó id ôóíêöèè
			brakets_counter += 3;
			pDest++;
						}
		else if (*pDest == ')')
		{				//ïðàâàÿ ñêîáêà ìåíüøàåò ñ÷¸ò÷èê ñêîáîê íà ÷èñëî, ðàâíîå ñàìîìó âûñîêîìó id ôóíêöèè
			brakets_counter -= 3;
			pDest++;
						}
		else if ((*pDest == '1') || (*pDest == '2') || (*pDest == '3') || (*pDest == '4') || (*pDest == '5') || (*pDest == '6') || (*pDest == '7') || (*pDest == '8') || (*pDest == '9') || (*pDest == '0'))
		{                   //ëþáîå ÷èñëî èìååò id "0"
			temp_size_of_vect = input_var_const->size();	//3? êîãäà ðàçìåð óæå 4
			input_var_const->push_back(var_const("0", strtod(pDest, &pDest))); //ñîçäàíèå ýëåìåíòà êëàññà è çàïèñü ÷èñëà
			// åñëè äàííîå ÷èñëî ïåðâîå (è âîçìîæíî åäèíñòâåííîå) â çàïèñè âûðàæåíèÿ
			if ((high_pointer == NULL) && (low_pointer == NULL))
						{
				//îáà óêàçàòåëÿ -> íà ÷èñëî, òåáóåòñÿ äëÿ ïðîâåðêè óñëîâèÿ ïðè çàïèñè îïåðàöèè
				high_pointer = &input_var_const->at(temp_size_of_vect);
				low_pointer = &input_var_const->at(temp_size_of_vect);
				//ëåâûé ðóêàâ âû÷èñëÿåìîé êîíñòàíòû -> ñîçäàííóþ ñòðóêòóðó ñ ÷èñëîì.
				input_var_const->at(current_size_of_vect).point_left = &input_var_const->at(temp_size_of_vect);
						}
			// åñëè äàííîå ÷èñëî íå ïåðâîå â çàïèñè (áûëà êàêàÿ-ëèáî îïåðàöèÿ)
						else
						{
				//íèæíèé óêàçàòåëü -> íà ñîçäàííîå ÷èñëî
				low_pointer = &input_var_const->at(temp_size_of_vect);
				//Ïðàâûé ðóêàâ ïðåäøåñòâóþùåé îïåðàöèè íà ñîçäàííîå ÷èñëî
						high_pointer[0].point_right = low_pointer;
					}
		}
	}

	input_var_const->at(current_size_of_vect).arithmetic();
	cout << input_var_const->at(current_size_of_vect).var_id << " = " << input_var_const->at(current_size_of_vect).var << endl;

	return 0;
}




int main()
{
	//âåðíè âñå êàê áûëî, èëè ïåðåäåëàé ïîä íîâûé âàðèàíò. ïî÷èòàé äîêóìåíòàöèþ ïî vector
	//vector<var_const> general_var_const(1);
	//general_var_const.reserve(30);

	//cout << "Size: " << general_var_const.capacity() << "  " << sizeof(general_var_const) << endl;

	var_const pi = var_const("pi", 3.1415926535897932384626433832);
	var_const e = var_const("e", 2.71828182846);

	//general_var_const[0].copy(pi);
	//general_var_const[1].copy(e);

	//general_var_const.push_back(pi);		
	//general_var_const.push_back(e);
	vector<var_const> general_var_const = { pi,e };	//ãåøåôò
													//ðàçìåð âïðèíöèïå íå íóæåí òóò
	int check = input_to_analize(&general_var_const); //÷òî íàäî: ïåðåäàâàòü óêàçàòåëü íà êàêîé-òî ìàññèâ äàííûõ òèïà var_const, òàê, ÷òî áû ìàññèâ ìîæíî áûëî äèíàìè÷åñêè îáðàáàòûâàòü â ïðåäåëàõ êàæäîé ïîäïðîãðàììû, êóäà ïîïàäàåì. âåêòîð îí èëè íåò - çíà÷åíèÿ íå èìåååò, ÿ äóìàë ÷òî ìîæíî áóäåò ïî óêàçàòåëþ âûïîëíÿòü ñ íèì îïåðàöèè è ìåòîäû èç îïèñàíèÿ âåêòîðà. ïàìÿòü òàì âûäåëÿòü, ñòèðàòü è äîáàâëÿòü .ùà

	getchar();	//eat endl
	getchar();	//press any key to exit

	return 0;


}

