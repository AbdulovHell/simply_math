// Точка входа для консольного приложения
//

#include "../../src/core.h"

#include <algorithm>

int main()
{
	using namespace std;
	
	Project::Core::Init();
	do {
		cout << "Input:" << endl;
		string sInput;
		cin >> sInput;

		string data = sInput;
		transform(data.begin(), data.end(), data.begin(), ::tolower);
		if (!data.compare("exit"))
			return 0;

		char* input = (char*)malloc(sInput.size() + 1);
		strcpy(input, sInput.c_str());

		char* check = Project::Core::input_to_analize(input);
		if (check != NULL)
			cout << check << endl;

	} while (1);

	return 0;
}

