// Точка входа для консольного приложения
//

#include "../../src/core.h"

#include <algorithm>

int main()
{
	using namespace std;
	wchar_t *input = NULL;
	Project::Core::Init();
	do {
		cout << "Input:" << endl;
		wstring sInput;
		wcin >> sInput;

		wstring data = sInput;
		transform(data.begin(), data.end(), data.begin(), ::tolower);
		if (!data.compare(L"exit"))
			return 0;
		input = new wchar_t[sInput.size() + 1];
		//wchar_t* input = (wchar_t*)malloc((sInput.size() + 1)*sizeof(wchar_t*));
		wcscpy(input, sInput.c_str());

		wstring check = Project::Core::input_to_analize(input);
		if (!check.empty())
			wcout << check << endl;
		
	} while (1);
	delete[] input;

	return 0;
}

