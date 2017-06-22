#pragma once
#include <iostream>
#include <vcclr.h>
#include <Windows.h>
#include "../../src/error.h"
#include "../../src/IO.h"
#include "../../src/filters.h"
#include "../../src/core.h"

namespace GUICLR {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace Project::Core;
	//using namespace std;
	/// <summary>
	/// Сводка для MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//

			input_indexes = gcnew cli::array<int>(300);
			all_math_data = Init();
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textBox1;
	protected:
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  ProceedBtn;
	private: System::Windows::Forms::Timer^  timer1;

	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;




			 bool isOutOrEmpty(System::String^ str) {
				 if (str == "") return true;

				 if (!(str->Length > 3)) return false;
				 if ((str[0] == '>'&&str[1] == '>'&&str[2] == '>'))
					 return true;
				 else return false;
			 }

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		cli::array<int>^ input_indexes;
		data_list* all_math_data;

	private: System::ComponentModel::IContainer^  components;


#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Требуемый метод для поддержки конструктора — не изменяйте 
			 /// содержимое этого метода с помощью редактора кода.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
				 this->textBox1 = (gcnew System::Windows::Forms::TextBox());
				 this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
				 this->ProceedBtn = (gcnew System::Windows::Forms::ToolStripButton());
				 this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
				 this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
				 this->toolStrip1->SuspendLayout();
				 this->SuspendLayout();
				 // 
				 // textBox1
				 // 
				 this->textBox1->Dock = System::Windows::Forms::DockStyle::Bottom;
				 this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
					 static_cast<System::Byte>(204)));
				 this->textBox1->Location = System::Drawing::Point(0, 28);
				 this->textBox1->Multiline = true;
				 this->textBox1->Name = L"textBox1";
				 this->textBox1->Size = System::Drawing::Size(607, 329);
				 this->textBox1->TabIndex = 0;
				 this->textBox1->Text = L"f(x)=(e+pi)*x\r\nf(2)=";
				 this->textBox1->TextChanged += gcnew System::EventHandler(this, &MainForm::verify);
				 // 
				 // toolStrip1
				 // 
				 this->toolStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
				 this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->ProceedBtn, this->toolStripButton1 });
				 this->toolStrip1->Location = System::Drawing::Point(0, 0);
				 this->toolStrip1->Name = L"toolStrip1";
				 this->toolStrip1->Size = System::Drawing::Size(607, 25);
				 this->toolStrip1->TabIndex = 1;
				 this->toolStrip1->Text = L"toolStrip1";
				 // 
				 // ProceedBtn
				 // 
				 this->ProceedBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->ProceedBtn->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ProceedBtn.Image")));
				 this->ProceedBtn->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->ProceedBtn->Name = L"ProceedBtn";
				 this->ProceedBtn->Size = System::Drawing::Size(23, 22);
				 this->ProceedBtn->Text = L"Proceed";
				 this->ProceedBtn->Click += gcnew System::EventHandler(this, &MainForm::timer1_Tick);
				 // 
				 // toolStripButton1
				 // 
				 this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton1.Image")));
				 this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->toolStripButton1->Name = L"toolStripButton1";
				 this->toolStripButton1->Size = System::Drawing::Size(23, 22);
				 this->toolStripButton1->Text = L"Clear";
				 this->toolStripButton1->Click += gcnew System::EventHandler(this, &MainForm::clr_rst);
				 // 
				 // MainForm
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(607, 357);
				 this->Controls->Add(this->toolStrip1);
				 this->Controls->Add(this->textBox1);
				 this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
				 this->MaximizeBox = false;
				 this->Name = L"MainForm";
				 this->Text = L"MainForm";
				 this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
				 this->toolStrip1->ResumeLayout(false);
				 this->toolStrip1->PerformLayout();
				 this->ResumeLayout(false);
				 this->PerformLayout();

			 }
#pragma endregion
	private: System::Void ProceedBtn_Click(System::Object^  sender, System::EventArgs^  e) {
		//cli::array<String^>^ strs = textBox1->Lines;	//указатель на линии с текстбокса
		//int len = strs->Length;	//количество линий
		//List<int>^ temp = gcnew List<int>;	//массив для индексов линий ввода
		//									//отделяем линии ввода по индексам
		//for (int i = 0;i < len;i++) {
		//	if (!isOutOrEmpty(strs[i])) {
		//		temp->Add(i);
		//	}
		//}
		////копируем линии нужного индекса
		//cli::array<String^>^ in = gcnew cli::array<String^>(temp->Count);
		//for (int i = 0;i < temp->Count;i++) {
		//	in[i] = strs[temp[i]];
		//}

		//len = temp->Count;	//количество линий ввода
		//cli::array<String^>^ out = gcnew cli::array<String^>(len * 2);
		//wchar_t* str1;
		////каждую по очереди в обработку 
		//for (int i = 0;i < len;i++) {
		//	pin_ptr<const wchar_t> str2 = PtrToStringChars(in[i]);
		//	out[i * 2] = in[i];
		//	size_t temps = in[i]->Length + 2;
		//	str1 = new wchar_t[temps];
		//	wcscpy(str1, str2);

		//	String^ outstr = gcnew String(Project::Core::input_to_analize(str1).c_str());
		//	if (String::IsNullOrEmpty(outstr)) {

		//	}
		//	else {
		//		out[i * 2 + 1] = ">>> " + outstr;
		//	}
		//	delete[] str1;
		//}
		////если есть nullptr линии, сдвигаем те что за ней вверх
		//int inullstrs = 0;
		//for (int i = 0;i < out->Length;i++) {
		//	if (out[i] == nullptr) {
		//		inullstrs++;
		//		for (int k = i;k < out->Length - 1;k++) {
		//			out[k] = out[k + 1];
		//		}
		//	}
		//}
		//cli::array<String^>^ out2 = gcnew cli::array<String^>(len * 2 - inullstrs);
		//for (int i = 0;i < out->Length - inullstrs;i++)
		//	out2[i] = out[i];

		//int a = textBox1->SelectionStart;

		//this->textBox1->TextChanged -= gcnew System::EventHandler(this, &MainForm::verify);
		//textBox1->Lines = out2;
		//this->textBox1->TextChanged += gcnew System::EventHandler(this, &MainForm::verify);

		//textBox1->SelectionStart = a;
		//textBox1->SelectionLength = 0;
	}
	private: System::Void PrevBtn_Click(System::Object^  sender, System::EventArgs^  e) {

	}
	private: System::Void NextBtn_Click(System::Object^  sender, System::EventArgs^  e) {

	}
	private: System::Void verify(System::Object^  sender, System::EventArgs^  e) {
		timer1->Enabled = false;
		Sleep(1);
		timer1->Enabled = true;
	}
	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {

		cli::array<String^>^ strs = textBox1->Lines;	//указатель на линии с текстбокса
		int size = strs->Length;	//количество линий		
		int len = all_math_data->size_s();
		int out_strings = 0;
		data_list* place;
		wstring *in_str;
		bool flag = false;
		//all_math_data->delete_starting_at(0);
		for (int i = 0; i < size; i++) {
			if (!isOutOrEmpty(strs[i]))
			{
				in_str = new wstring((wchar_t*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAuto(strs[i]));

				place = all_math_data->at(i - out_strings + 1);
				if (place != NULL)
				{
					if ((place->compare_in(in_str)) || (flag))//если строчки не совпадают
					{
						place->in = *in_str;
						place->run_string();
						flag = true;
					}
					else
					{
						//строки совпали
					}
				}
				else
				{
					place = new data_list(in_str);
					all_math_data->push_back(place);
					//в run_string текст ошибки сразу записывается в поле вывода.
					place->run_string();
				}
			}
			else
			{
				out_strings++;//посчитать количество строк вывода из предыдущих выполнений программы
			}
		}
		if (size - out_strings < len)
		{
			//если количество введённых строк (с учётом вывода в предыду оказалось меньше числа злементов в списке данных - удаляем лишнее
			all_math_data->delete_starting_at(size - out_strings);
		}

		if (!input_to_analize(all_math_data))
		{
			//очень глобальная ошибка. пока что она не предусмотрена нигде.
			//err
		}
		//далее all_math_data уже обработана в ядре		
		len = all_math_data->size_s();//количество элементов. Нумерация с 1.


		//List<int>^ temp = gcnew List<int>;	//массив для индексов линий ввода
		////отделяем линии ввода по индексам
		//for (int i = 0;i < len;i++) {
		//	if (!isOutOrEmpty(strs[i])) {
		//		temp->Add(i);
		//	}
		//}
		////копируем линии нужного индекса
		//cli::array<String^>^ in = gcnew cli::array<String^>(temp->Count);
		//for (int i = 0;i < temp->Count;i++) {
		//	in[i] = strs[temp[i]];
		//}

		//len = temp->Count;	//количество линий ввода

		////каждую по очереди в обработку 
		//input_to_analize(all_math_data);
		////если есть nullptr линии, сдвигаем те что за ней вверх

		//for (int i = 0;i < out->Length;i++) {
		//	if (out[i] == nullptr) {
		//		inullstrs++;
		//		for (int k = i;k < out->Length-1;k++) {
		//			out[k] = out[k + 1];
		//		}
		//	}
		//}
		int inullstrs = 0;
		//cli::array<String^>^ in = gcnew cli::array<String^>(len);		
		cli::array<String^>^ out = gcnew cli::array<String^>(len * 2);
		String^ outstr;
		for (int i = 0; i < len; i++)
		{
			out[i * 2] = gcnew String(all_math_data->at(i + 1)->in.c_str());
			outstr = gcnew String(all_math_data->at(i + 1)->out.c_str());
			if (String::IsNullOrEmpty(outstr))
				inullstrs++;
			out[i * 2 + 1] = outstr;

		}
		int empty = 0;
		cli::array<String^>^ out2 = gcnew cli::array<String^>(len * 2 - inullstrs);
		for (int i = 0; i < (2 * len); i++)
		{
			if (i % 2 == 0)
				out2[i - empty] = out[i];
			else if (i % 2 == 1)
			{
				if (String::IsNullOrEmpty(out[i]))
				{
					empty++;
				}
				else
				{
					out2[i - empty] = ">>> " + out[i];
				}
			}
		}
		int a = textBox1->SelectionStart;

		this->textBox1->TextChanged -= gcnew System::EventHandler(this, &MainForm::verify);
		textBox1->Lines = out2;
		this->textBox1->TextChanged += gcnew System::EventHandler(this, &MainForm::verify);

		textBox1->SelectionStart = a;
		textBox1->SelectionLength = 0;

		timer1->Enabled = false;
	}
	private: System::Void clr_rst(System::Object^  sender, System::EventArgs^  e) {
		textBox1->Clear();
		//Project::Core::ClearGeneral();
		//Project::Core::Init();
	}
	};
}
