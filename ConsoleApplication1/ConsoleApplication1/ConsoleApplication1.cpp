// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

class minivariable {
public:
	minivariable(string _name, double _num) {
		name = _name;
		var = _num;
	}

	double var;
	string name;
};

int main()
{
	string input;
	cin >> input;

	minivariable* param = new minivariable("x", 0);

	char* charinput = (char*)malloc(input.size());
	strcpy(charinput, input.c_str());

	char* pDest = charinput;	//start pointer
	char* endPtr = charinput + input.size() - 1;	//end pointer
	double temp = 0;

	char* varnameDest = strstr(charinput, "=");
	char* varname = (char*)malloc(varnameDest - charinput+1);
	memcpy(varname, charinput, varnameDest - charinput);
	varname[varnameDest - charinput] = 0;
	param->name = varname;

	param->var = strtod(varnameDest + 1, &pDest);
	while (pDest < endPtr) {
		switch (*pDest)
		{
		case '+':
			temp = strtod(pDest + 1, &pDest);
			param->var += temp;
			break;
		case '-':
			temp = strtod(pDest + 1, &pDest);
			param->var -= temp;
			break;
		default:
			break;
		}
	}
	cout << endl << param->name << "=" << param->var << endl;
	getchar();	//eat endl
	getchar();	//press any key to exit
	return 0;
}

