#pragma once

#include <vcclr.h>
#include <Windows.h>
#include "../../src/common.h"
#include "../../src/error.h"
#include "../../src/IO.h"
#include "../../src/filters.h"
#include "../../src/core.h"
#include "../../src/html_addon.h"
#include "../../src/data_list.h"
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
	/// ������ ��� MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: �������� ��� ������������
			//

			input_indexes = gcnew cli::array<int>(300);
			all_math_data = Init();
		}

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
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
	private: System::Windows::Forms::ToolStripButton^  toolStripButton2;

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
		/// ������������ ���������� ������������.
		/// </summary>
		cli::array<int>^ input_indexes;
		data_list* all_math_data;

	private: System::ComponentModel::IContainer^  components;


#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// ��������� ����� ��� ��������� ������������ � �� ��������� 
			 /// ���������� ����� ������ � ������� ��������� ����.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->components = (gcnew System::ComponentModel::Container());
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
				 this->textBox1 = (gcnew System::Windows::Forms::TextBox());
				 this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
				 this->ProceedBtn = (gcnew System::Windows::Forms::ToolStripButton());
				 this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
				 this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
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
				 this->textBox1->Size = System::Drawing::Size(915, 527);
				 this->textBox1->TabIndex = 0;
				 this->textBox1->Text = L"-2+3*5-6^8=";
				 this->textBox1->TextChanged += gcnew System::EventHandler(this, &MainForm::verify);
				 // 
				 // toolStrip1
				 // 
				 this->toolStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
				 this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
					 this->ProceedBtn, this->toolStripButton1,
						 this->toolStripButton2
				 });
				 this->toolStrip1->Location = System::Drawing::Point(0, 0);
				 this->toolStrip1->Name = L"toolStrip1";
				 this->toolStrip1->Size = System::Drawing::Size(915, 25);
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
				 // toolStripButton2
				 // 
				 this->toolStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
				 this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripButton2.Image")));
				 this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
				 this->toolStripButton2->Name = L"toolStripButton2";
				 this->toolStripButton2->Size = System::Drawing::Size(23, 22);
				 this->toolStripButton2->Text = L"toolStripButton2";
				 this->toolStripButton2->Click += gcnew System::EventHandler(this, &MainForm::toolStripButton2_Click);
				 // 
				 // MainForm
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(915, 555);
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
		//cli::array<String^>^ strs = textBox1->Lines;	//��������� �� ����� � ����������
		//int len = strs->Length;	//���������� �����
		//List<int>^ temp = gcnew List<int>;	//������ ��� �������� ����� �����
		//									//�������� ����� ����� �� ��������
		//for (int i = 0;i < len;i++) {
		//	if (!isOutOrEmpty(strs[i])) {
		//		temp->Add(i);
		//	}
		//}
		////�������� ����� ������� �������
		//cli::array<String^>^ in = gcnew cli::array<String^>(temp->Count);
		//for (int i = 0;i < temp->Count;i++) {
		//	in[i] = strs[temp[i]];
		//}

		//len = temp->Count;	//���������� ����� �����
		//cli::array<String^>^ out = gcnew cli::array<String^>(len * 2);
		//wchar_t* str1;
		////������ �� ������� � ��������� 
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
		////���� ���� nullptr �����, �������� �� ��� �� ��� �����
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

		cli::array<String^>^ strs = textBox1->Lines;	//��������� �� ����� � ����������
		int size = strs->Length;	//���������� ����� � ����		
		int len = all_math_data->size_s();	//���������� ����� � ��� �������
		int out_strings = 0;
		//data_list* place_pp;
		data_list::iterator place = all_math_data->front();//����� data_list, ������ � � �����������.
		wstring *in_str;
		bool needReCalc = false;

		for (int i = 0; i < size; i++) {
			if (!isOutOrEmpty(strs[i])) {

				in_str = new wstring((wchar_t*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAuto(strs[i]));
				place++;//��� �������� ���������� ����� ��������� �� ������, ���� ������ �������� - ��������� ����� �������� nullptr, ��� ����������� �����
				//place_pp = all_math_data->at(i - out_strings);
				if (place != nullptr) {//��� ��������� ���������� ������ ������ �������� �� ������ ���� ����� ��������� �� ����.
					if ((place.compare_in(in_str)) || (needReCalc)) {//���� ������� �� ���������
						place.assing_in(*in_str);
						if (!place.build()) {
							ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
							ProjectError::GetProjectLastError(err);
							place.assing_out(err->GetErrorWStr());
						}
						needReCalc = true;
					}
					else {
						//������ �������
					}
				}
				else {
					place = new data_list(in_str, all_math_data);//����������� �������� ������ ������ ����� ��������� ��� � ������					
					//� run_string ����� ������ ����� ������������ � ���� ������.
					//place_pp->run_string();
				}				
			}	//if (!isOutOrEmpty(strs[i]))
			else {
				out_strings++;//��������� ���������� ����� ������ �� ���������� ���������� ���������
			}
		}
		
		if (size - out_strings < len)
		{
			//���� ���������� �������� ����� (� ������ ������ � ���������� ���������� ���������) ��������� ������ ����� ��������� � ������ ������ - ������� ������
			all_math_data->delete_starting_at(size - out_strings - 1);//��������� ������ � ����
		}
		//������ ������� ����� �������� ������� ����� ��������:
		//place.delete_after_this();
		
		if (!input_to_analize(all_math_data))
		{
			//����� ���������� ������. ���� ��� ��� �� ������������� �����.
			//err
		}
		//����� all_math_data ��� ���������� � ����		
		len = all_math_data->size_s();//���������� ���������. ��������� � 0.	
		cli::array<String^>^ out = gcnew cli::array<String^>(len * 2);	//����������� ��������� ��������
		String^ outstr;
		for (int i = 0, k = 0; i < len; i++)
		{
			out[k++] = gcnew String(all_math_data->at(i)->in.c_str());
			outstr = gcnew String(all_math_data->at(i)->out.c_str());
			if (!String::IsNullOrEmpty(outstr))
				out[k++] = ">>> " + outstr;
		}

		int a = textBox1->SelectionStart;

		this->textBox1->TextChanged -= gcnew System::EventHandler(this, &MainForm::verify);
		textBox1->Lines = out;
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
	private: System::Void toolStripButton2_Click(System::Object^  sender, System::EventArgs^  e) {
		wstring str = L"biboran";
		str = Project::HTML::ApplyEffects(str, Project::HTML::Actions::BOLD, Project::HTML::Actions::ITALIC, Project::HTML::Actions::SUPER);
		str = Project::HTML::ChangeTextColor(str, Project::HTML::HTMLColors::RED);
		str = Project::HTML::RemoveLastEffect(str);
	}
	};
}
