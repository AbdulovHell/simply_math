#define _CRT_SECURE_NO_WARNINGS
#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void Main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	GUICLR::MainForm form;
	Application::Run(%form);
}

