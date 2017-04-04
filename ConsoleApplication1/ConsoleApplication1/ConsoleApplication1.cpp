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
		wstring sInput;
		wcin >> sInput;

		wstring data = sInput;
		transform(data.begin(), data.end(), data.begin(), ::tolower);
		if (!data.compare(L"exit"))
			return 0;

		wchar_t* input = (wchar_t*)malloc((sInput.size() + 1)*2);
		wcscpy(input, sInput.c_str());

		wchar_t* check = Project::Core::input_to_analize(input);
		if (check != NULL)
			wcout << check << endl;

	} while (1);

	return 0;
}

