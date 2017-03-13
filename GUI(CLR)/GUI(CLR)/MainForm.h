#pragma once
#include <iostream>
#include <vcclr.h>
#include "../../src/core.h"

namespace GUICLR {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;
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
			symbBox->Text = symbol.ToString();
			//textBox1->Text = symbol.ToString();
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
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripTextBox^  symbBox;

	private: System::Windows::Forms::ToolStripButton^  PrevBtn;
	private: System::Windows::Forms::ToolStripButton^  NextBtn;

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		wchar_t symbol = '?';
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->ProceedBtn = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->symbBox = (gcnew System::Windows::Forms::ToolStripTextBox());
			this->PrevBtn = (gcnew System::Windows::Forms::ToolStripButton());
			this->NextBtn = (gcnew System::Windows::Forms::ToolStripButton());
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
			// 
			// toolStrip1
			// 
			this->toolStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->ProceedBtn, this->toolStripSeparator1,
					this->symbBox, this->PrevBtn, this->NextBtn
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
			this->ProceedBtn->Text = L"Proceed ?";
			this->ProceedBtn->Click += gcnew System::EventHandler(this, &MainForm::ProceedBtn_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(6, 25);
			// 
			// symbBox
			// 
			this->symbBox->Font = (gcnew System::Drawing::Font(L"Times New Roman", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->symbBox->Name = L"symbBox";
			this->symbBox->ReadOnly = true;
			this->symbBox->Size = System::Drawing::Size(100, 25);
			// 
			// PrevBtn
			// 
			this->PrevBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->PrevBtn->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"PrevBtn.Image")));
			this->PrevBtn->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->PrevBtn->Name = L"PrevBtn";
			this->PrevBtn->Size = System::Drawing::Size(23, 22);
			this->PrevBtn->Text = L"toolStripButton1";
			this->PrevBtn->Click += gcnew System::EventHandler(this, &MainForm::PrevBtn_Click);
			// 
			// NextBtn
			// 
			this->NextBtn->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->NextBtn->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"NextBtn.Image")));
			this->NextBtn->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->NextBtn->Name = L"NextBtn";
			this->NextBtn->Size = System::Drawing::Size(23, 22);
			this->NextBtn->Text = L"toolStripButton2";
			this->NextBtn->Click += gcnew System::EventHandler(this, &MainForm::NextBtn_Click);
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
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void ProceedBtn_Click(System::Object^  sender, System::EventArgs^  e) {
		wchar_t* str1 = (wchar_t*)malloc(50);
		pin_ptr<const wchar_t> str2 = PtrToStringChars(textBox1->Text);
		wcscpy(str1,str2);

		free(str1);
	}
	private: System::Void PrevBtn_Click(System::Object^  sender, System::EventArgs^  e) {
		if (symbol) {
			symbol--;
			symbBox->Text = symbol.ToString();
		}
	}
private: System::Void NextBtn_Click(System::Object^  sender, System::EventArgs^  e) {
	symbol++;
	symbBox->Text = symbol.ToString();
}
};
}
