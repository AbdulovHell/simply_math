#pragma once
#include <iostream>
#include <vcclr.h>
#include <Windows.h>
#include "../../src/core.h"

namespace GUICLR {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	//using namespace std;
	/// <summary>
	/// —водка дл€ MainForm
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
			Project::Core::Init();
			input_indexes = gcnew cli::array<int>(300);
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
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
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;



			 bool isOut(System::String^ str) {
				 if (!(str->Length > 3)) return false;
				 if (str[0] == '>'&&str[1] == '>'&&str[2] == '>')
					 return true;
				 else return false;
			 }

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		cli::array<int>^ input_indexes;

	private: System::Windows::Forms::ToolStripProgressBar^  toolStripProgressBar1;
	private: System::ComponentModel::IContainer^  components;


#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
			 /// содержимое этого метода с помощью редактора кода.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
				 this->textBox1 = (gcnew System::Windows::Forms::TextBox());
				 this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
				 this->ProceedBtn = (gcnew System::Windows::Forms::ToolStripButton());
				 this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
				 this->toolStripProgressBar1 = (gcnew System::Windows::Forms::ToolStripProgressBar());
				 this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
				 this->toolStrip1->SuspendLayout();
				 this->SuspendLayout();
				 // 
				 // textBox1
				 // 
				 this->textBox1->Dock = System::Windows::Forms::DockStyle::Bottom;
				 this->textBox1->Location = System::Drawing::Point(0, 28);
				 this->textBox1->Multiline = true;
				 this->textBox1->Name = L"textBox1";
				 this->textBox1->Size = System::Drawing::Size(607, 329);
				 this->textBox1->TabIndex = 0;
				 this->textBox1->Text = L"x+5";
				 this->textBox1->TextChanged += gcnew System::EventHandler(this, &MainForm::verify);
				 // 
				 // toolStrip1
				 // 
				 this->toolStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
				 this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
					 this->ProceedBtn, this->toolStripSeparator1,
						 this->toolStripProgressBar1
				 });
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
				 this->ProceedBtn->Text = L"Proceed \?";
				 this->ProceedBtn->Click += gcnew System::EventHandler(this, &MainForm::ProceedBtn_Click);
				 // 
				 // toolStripSeparator1
				 // 
				 this->toolStripSeparator1->Name = L"toolStripSeparator1";
				 this->toolStripSeparator1->Size = System::Drawing::Size(6, 25);
				 // 
				 // toolStripProgressBar1
				 // 
				 this->toolStripProgressBar1->Name = L"toolStripProgressBar1";
				 this->toolStripProgressBar1->Size = System::Drawing::Size(100, 22);
				 // 
				 // timer1
				 // 
				 this->timer1->Interval = 1200;
				 this->timer1->Tick += gcnew System::EventHandler(this, &MainForm::timer1_Tick);
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
		pin_ptr<const wchar_t> str2 = PtrToStringChars(textBox1->Text);
		//int a=sizeof(str2);
		//wstring str1;
		wchar_t* str1 = new wchar_t[sizeof(str2) * 2];
		wcscpy(str1, str2);
		//textBox1->Text = gcnew System::String(Project::Core::input_to_analize(str1));
		textBox1->AppendText(gcnew System::String(Project::Core::input_to_analize(str1).c_str()));
		delete[] str1;
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
	cli::array<String^>^ strs = textBox1->Lines;
	int len = strs->Length;
	List<int>^ temp = gcnew List<int>;

	for (int i = 0;i < len;i++) {
		if (!isOut(strs[i])) {
			temp->Add(i);
		}
	}
	cli::array<String^>^ in = gcnew cli::array<String^>(temp->Count);
	for (int i = 0;i < temp->Count;i++) {
		in[i] = strs[temp[i]];
	}

	len = temp->Count;
	cli::array<String^>^ out = gcnew cli::array<String^>(len * 2);
	wchar_t* str1;
	for (int i = 0;i < len;i++) {
		pin_ptr<const wchar_t> str2 = PtrToStringChars(in[i]);
		out[i * 2] = in[i];
		size_t temps = in[i]->Length + 2;
		str1 = new wchar_t[temps];
		wcscpy(str1, str2);

		out[i * 2 + 1] = ">>> " + gcnew String(Project::Core::input_to_analize(str1).c_str());

		delete[] str1;
	}

	int a = textBox1->SelectionStart;

	this->textBox1->TextChanged -= gcnew System::EventHandler(this, &MainForm::verify);
	textBox1->Lines = out;
	this->textBox1->TextChanged += gcnew System::EventHandler(this, &MainForm::verify);

	textBox1->SelectionStart = a;
	textBox1->SelectionLength = 0;

	timer1->Enabled = false;
}
};
}
