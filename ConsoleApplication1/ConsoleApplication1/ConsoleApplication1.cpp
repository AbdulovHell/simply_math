// ConsoleApplication1.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

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
			*/ 2^(3/5)
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
	string input;
	cin >> input;
	
	

	int temp_size_of_vect = current_size_of_vect;
	char* charinput = (char*)malloc(input.size());
	strcpy(charinput, input.c_str());

	char* pDest = charinput;	//start pointer
	char* endPtr = charinput + input.size() - 1;	//end pointer
	char *temp = NULL;

	var_const *high_pointer = NULL;
	var_const *low_pointer = NULL;

	char* varnameDest = strstr(charinput, "=");

	char *varname = (char*)malloc(varnameDest - charinput + 1);
	memcpy(varname, charinput, varnameDest - charinput);
	varname[varnameDest - charinput] = 0;

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

	vector<var_const> general_var_const(30);
	//general_var_const.reserve(30);
	//array<var_const, 1> general_var_const;
	cout << "Size: " << general_var_const.capacity() << "  " << sizeof(general_var_const) << endl;

	var_const pi = var_const("pi", 3.1415926535897932384626433832);
	var_const e = var_const("e", 2.71828182846);

	general_var_const[0].copy(pi);
	general_var_const[1].copy(e);



	int check = input_to_analize(general_var_const.data(), 2);


	getchar();	//eat endl
	getchar();	//press any key to exit

	return 0;


}

