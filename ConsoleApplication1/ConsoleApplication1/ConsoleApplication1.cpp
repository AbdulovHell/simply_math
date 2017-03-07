// ConsoleApplication1.cpp: ���������� ����� ����� ��� ����������� ����������.
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

bool VerifyInput(char* input) {
	//char* EndStr = &input[strlen(input)];
	//char* cursor = input;
	int len = strlen(input);
	int left_bracket = 0, right_bracket = 0, ravno = 0;

	return true;
	//�����, ��� ����������, �������� ��� A-Z,a-z � �.�. else if ���� ��
	for (int i = 0;i < len;i++)
		switch (input[i]) {
		case '(':
			left_bracket++;
			break;
		case ')':
			right_bracket++;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '+':
		case '-':
		case '*':
		case '/':
		case '.':
			break;
		case '=':
			ravno++;
			break;
		default:
			printf("\nBad symbol, '%c':\n", input[i]);
			printf(input);
			printf("\n");
			printf(err_str(i));
			printf("\n");
			return false;
		}

	if (left_bracket != right_bracket) {
		printf("\n ( and ) error.\n");
		return false;
	}
	if (ravno > 1) {
		printf("\n'='>1 error.\n");
		return false;
	}
	/*
	while (cursor < EndStr) {

	}*/
	return true;
}
char* SafeInput() {
#define MEMRATE 10
	char* buf = (char*)malloc(MEMRATE);
	char a;
	int len = 0;
	int mem_commited = MEMRATE;

	for (int i = 0;i < MEMRATE;i++)
		buf[i] = 0;

	do {
		a = _getche();
		if (a == '\r')
			break;
		buf[len] = a;
		len++;
		if (len >= mem_commited - 1) {
			char* temp = buf;
			buf = (char*)realloc(buf, mem_commited + MEMRATE);
			//free(temp); �� ��������! �� �����, ��� �� ���... ��� ������� ���-�� �� �������, �.�. �� ������ � ������ �������, ��� � ������ �� ��������� free,
			//�������� ��������, ������ ��� �� ������ ����� ��� ������� ������
			mem_commited += MEMRATE;
		}

	} while (1);
	buf[len] = 0;
	return buf;
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

int input_to_analize(var_const *input_var_const, int current_size_of_vect)
{
	/*
	string sInput;
	cin >> sInput;


	char* input = (char*)malloc(sInput.size());
	strcpy(input, sInput.c_str());
	*/
	///����� �������, ������� ������, ������ ������������, �� ���� free �� �������� � ���� ���-�� ������, ��� ������� �� ������)
	///��� ���� �� ���������� ��� � ����������� �����������...
	char* input = SafeInput();
	if (!VerifyInput(input))
		return -1;
	///
	int input_size = strlen(input);

	char* pDest = input;	//start pointer
	char* endPtr = input + strlen(input) - 1;	//end pointer
	char *temp = NULL;

	int temp_size_of_vect = current_size_of_vect;
	var_const *high_pointer = NULL;
	var_const *low_pointer = NULL;

	char* varnameDest = strstr(input, "=");

	char *varname = (char*)malloc(varnameDest - input + 1);
	memcpy(varname, input, varnameDest - input);
	varname[varnameDest - input] = 0;

	input_var_const[current_size_of_vect].copy(var_const(varname, 0));

	pDest = varnameDest + 1;
	var_const *k = &input_var_const[current_size_of_vect];

	int brakets_counter = 0;
	while (pDest <= endPtr) {

		switch (*pDest)
		{
		case '+':
			temp_size_of_vect++;
			if (high_pointer == low_pointer)
			{
				input_var_const[temp_size_of_vect] = var_const("1", brakets_counter, high_pointer, NULL);
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
				high_pointer = &input_var_const[temp_size_of_vect];
			}
			else
			{
				if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 1))
				{
					input_var_const[temp_size_of_vect] = var_const("1", brakets_counter, low_pointer, NULL);
					high_pointer[0].point_right = &input_var_const[temp_size_of_vect];
					high_pointer = &input_var_const[temp_size_of_vect];
				}
				else
				{
					input_var_const[temp_size_of_vect] = var_const("1", brakets_counter, high_pointer, NULL);
					if ((brakets_counter + 1) < (strtod(input_var_const[current_size_of_vect].point_left[0].var_id.c_str(), &temp) + input_var_const[current_size_of_vect].point_left[0].var))
					{
						high_pointer = input_var_const[current_size_of_vect].point_left;
						input_var_const[temp_size_of_vect].point_left = high_pointer;
						input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];

					}
					else
					{
						input_var_const[current_size_of_vect].point_left[0].point_right = &input_var_const[temp_size_of_vect];
					}
					high_pointer = &input_var_const[temp_size_of_vect];

				}
			}
			pDest++;
			break;
		case '*':
			temp_size_of_vect++;
			if (high_pointer == low_pointer)
			{
				input_var_const[temp_size_of_vect] = var_const("2", brakets_counter, high_pointer, NULL);
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
				high_pointer = &input_var_const[temp_size_of_vect];
			}
			else
			{
				if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 2))
				{
					input_var_const[temp_size_of_vect] = var_const("2", brakets_counter, low_pointer, NULL);
					high_pointer[0].point_right = &input_var_const[temp_size_of_vect];
					high_pointer = &input_var_const[temp_size_of_vect];
				}
				else
				{
					input_var_const[temp_size_of_vect] = var_const("2", brakets_counter, high_pointer, NULL);

					if ((brakets_counter + 2) < (strtod(input_var_const[current_size_of_vect].point_left[0].var_id.c_str(), &temp) + input_var_const[current_size_of_vect].point_left[0].var))
					{
						high_pointer = input_var_const[current_size_of_vect].point_left;
						input_var_const[temp_size_of_vect].point_left = high_pointer;
						input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
					}
					else
					{
						input_var_const[current_size_of_vect].point_left[0].point_right = &input_var_const[temp_size_of_vect];
					}
					high_pointer = &input_var_const[temp_size_of_vect];
				}
			}
			pDest++;
			break;
		case '/':
			temp_size_of_vect++;
			if (high_pointer == low_pointer)
			{
				input_var_const[temp_size_of_vect] = var_const("3", brakets_counter, high_pointer, NULL);
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
				high_pointer = &input_var_const[temp_size_of_vect];
			}
			else
			{
				if ((strtod(high_pointer[0].var_id.c_str(), &temp) + high_pointer[0].var) <= (brakets_counter + 3))
				{
					input_var_const[temp_size_of_vect] = var_const("3", brakets_counter, low_pointer, NULL);
					high_pointer[0].point_right = &input_var_const[temp_size_of_vect];
					high_pointer = &input_var_const[temp_size_of_vect];
				}
				else
				{
					input_var_const[temp_size_of_vect] = var_const("3", brakets_counter, high_pointer, NULL);
					if ((brakets_counter + 3) < (strtod(input_var_const[current_size_of_vect].point_left[0].var_id.c_str(), &temp) + input_var_const[current_size_of_vect].point_left[0].var))
					{
						high_pointer = input_var_const[current_size_of_vect].point_left;
						input_var_const[temp_size_of_vect].point_left = high_pointer;
						input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
					}
					else
					{
						input_var_const[current_size_of_vect].point_left[0].point_right = &input_var_const[temp_size_of_vect];
					}
					high_pointer = &input_var_const[temp_size_of_vect];
				}
			}
			pDest++;
			break;
		case'(':
			brakets_counter += 3;
			pDest++;
			break;
		case')':
			brakets_counter -= 3;
			pDest++;
			break;
		case '1':
			temp_size_of_vect++;
			input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				low_pointer = &input_var_const[temp_size_of_vect];
				high_pointer[0].point_right = low_pointer;
			}
			break;
		case'2':
			temp_size_of_vect++;
			input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				low_pointer = &input_var_const[temp_size_of_vect];
				high_pointer[0].point_right = low_pointer;
			}
			break;
		case'3':
			temp_size_of_vect++;
			input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				low_pointer = &input_var_const[temp_size_of_vect];
				high_pointer[0].point_right = low_pointer;
			}
			break;
		case '4':
			temp_size_of_vect++;
			input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				low_pointer = &input_var_const[temp_size_of_vect];
				high_pointer[0].point_right = low_pointer;
			}
			break;
		case'5':
			temp_size_of_vect++;
			input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				low_pointer = &input_var_const[temp_size_of_vect];
				high_pointer[0].point_right = low_pointer;
			}
			break;
		case'6':
			temp_size_of_vect++;
			input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				low_pointer = &input_var_const[temp_size_of_vect];
				high_pointer[0].point_right = low_pointer;
			}
			break;
		case '7':
			temp_size_of_vect++;
			input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				low_pointer = &input_var_const[temp_size_of_vect];
				high_pointer[0].point_right = low_pointer;
			}
			break;
		case'8':
			temp_size_of_vect++;
			input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				low_pointer = &input_var_const[temp_size_of_vect];
				high_pointer[0].point_right = low_pointer;
			}
			break;
		case'9':
			temp_size_of_vect++;
			input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				low_pointer = &input_var_const[temp_size_of_vect];
				high_pointer[0].point_right = low_pointer;
			}
			break;
		case '0':
			temp_size_of_vect++;
			input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				low_pointer = &input_var_const[temp_size_of_vect];
				high_pointer[0].point_right = low_pointer;
			}
			break;
		case '-':
			temp_size_of_vect++;
			if ((high_pointer == NULL) && (low_pointer == NULL))
			{
				input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
				high_pointer = &input_var_const[temp_size_of_vect];
				low_pointer = &input_var_const[temp_size_of_vect];
				input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
			}
			else
			{
				if (high_pointer == low_pointer)
				{
					input_var_const[temp_size_of_vect] = var_const("1", brakets_counter, high_pointer, NULL);
					input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
					high_pointer = &input_var_const[temp_size_of_vect];
					temp_size_of_vect++;
					input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
					low_pointer = &input_var_const[temp_size_of_vect];
					high_pointer[0].point_right = low_pointer;
				}
				else
				{
					if (brakets_counter > high_pointer[0].var)
						if (high_pointer[0].point_right != NULL)
						{
							input_var_const[temp_size_of_vect] = var_const("1", brakets_counter, low_pointer, NULL);
							high_pointer[0].point_right = &input_var_const[temp_size_of_vect];
							high_pointer = &input_var_const[temp_size_of_vect];
							temp_size_of_vect++;
							input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
							low_pointer = &input_var_const[temp_size_of_vect];
							high_pointer[0].point_right = low_pointer;
						}
						else
						{
							input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
							low_pointer = &input_var_const[temp_size_of_vect];
							high_pointer[0].point_right = low_pointer;
						}
					else
					{
						input_var_const[temp_size_of_vect] = var_const("1", brakets_counter, high_pointer, NULL);
						if ((brakets_counter + 1) < (strtod(input_var_const[current_size_of_vect].point_left[0].var_id.c_str(), &temp) + input_var_const[current_size_of_vect].point_left[0].var))
						{
							high_pointer = input_var_const[current_size_of_vect].point_left;
							input_var_const[temp_size_of_vect].point_left = high_pointer;
							input_var_const[current_size_of_vect].point_left = &input_var_const[temp_size_of_vect];
						}
						else
						{
							input_var_const[current_size_of_vect].point_left[0].point_right = &input_var_const[temp_size_of_vect];
						}
						high_pointer = &input_var_const[temp_size_of_vect];
						temp_size_of_vect++;
						input_var_const[temp_size_of_vect] = var_const("0", strtod(pDest, &pDest));
						low_pointer = &input_var_const[temp_size_of_vect];
						high_pointer[0].point_right = low_pointer;
					}
				}
			}
			break;
		}
	}

	input_var_const[current_size_of_vect].arithmetic();
	cout << input_var_const[current_size_of_vect].var_id << " = " << input_var_const[current_size_of_vect].var << endl;

	return 0;
}




int main()
{
	//����� ��� ��� ����, ��� ��������� ��� ����� �������. ������� ������������ �� vector
	//vector<var_const> general_var_const(1);
	//general_var_const.reserve(30);

	//cout << "Size: " << general_var_const.capacity() << "  " << sizeof(general_var_const) << endl;

	var_const pi = var_const("pi", 3.1415926535897932384626433832);
	var_const e = var_const("e", 2.71828182846);

	//general_var_const[0].copy(pi);
	//general_var_const[1].copy(e);

	//general_var_const.push_back(pi);		//���������� ������ �������� � �����
	//general_var_const.push_back(e);
	vector<var_const> general_var_const = { pi,e };

	int check = input_to_analize(general_var_const.data(), 2);


	getchar();	//eat endl
	getchar();	//press any key to exit

	return 0;


}

