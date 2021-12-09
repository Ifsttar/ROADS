#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace roads {

	/// <summary>
	/// Summary for BackdropOptionsForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class BackdropOptionsForm : public System::Windows::Forms::Form
	{
	public:
		BackdropOptionsForm(void)
		{
			InitializeComponent();
		}

    property double North {
      double get( void ) { return (double)_northBox->Value; }
      void set( double d ) { _northBox->Value = Decimal(d); }
    }
    property double South {
      double get( void ) { return (double)_southBox->Value; }
      void set( double d ) { _southBox->Value = Decimal(d); }
    }
    property double East {
      double get( void ) { return (double)_eastBox->Value; }
      void set( double d ) { _eastBox->Value = Decimal(d); }
    }
    property double West {
      double get( void ) { return (double)_westBox->Value; }
      void set( double d ) { _westBox->Value = Decimal(d); }
    }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~BackdropOptionsForm()
		{
			delete components;
		}

	private:
    Button        ^_okBtn;
    Button        ^_cancelBtn;
    NumericUpDown ^_eastBox;
    NumericUpDown ^_westBox;
    NumericUpDown ^_southBox;
    NumericUpDown ^_northBox;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
      System::Windows::Forms::Label^  label15;
      System::Windows::Forms::Label^  label14;
      System::Windows::Forms::Label^  label13;
      System::Windows::Forms::Label^  label12;
      this->_okBtn = (gcnew System::Windows::Forms::Button());
      this->_cancelBtn = (gcnew System::Windows::Forms::Button());
      this->_eastBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_westBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_southBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_northBox = (gcnew System::Windows::Forms::NumericUpDown());
      label15 = (gcnew System::Windows::Forms::Label());
      label14 = (gcnew System::Windows::Forms::Label());
      label13 = (gcnew System::Windows::Forms::Label());
      label12 = (gcnew System::Windows::Forms::Label());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_eastBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_westBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_southBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_northBox))->BeginInit();
      this->SuspendLayout();
      // 
      // _okBtn
      // 
      this->_okBtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->_okBtn->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->_okBtn->Location = System::Drawing::Point(12, 105);
      this->_okBtn->Name = L"_okBtn";
      this->_okBtn->Size = System::Drawing::Size(75, 23);
      this->_okBtn->TabIndex = 0;
      this->_okBtn->Text = L"OK";
      this->_okBtn->UseVisualStyleBackColor = true;
      // 
      // _cancelBtn
      // 
      this->_cancelBtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->_cancelBtn->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->_cancelBtn->Location = System::Drawing::Point(221, 105);
      this->_cancelBtn->Name = L"_cancelBtn";
      this->_cancelBtn->Size = System::Drawing::Size(75, 23);
      this->_cancelBtn->TabIndex = 1;
      this->_cancelBtn->Text = L"Cancel";
      this->_cancelBtn->UseVisualStyleBackColor = true;
      // 
      // label15
      // 
      label15->AutoSize = true;
      label15->Location = System::Drawing::Point(171, 40);
      label15->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label15->Name = L"label15";
      label15->Size = System::Drawing::Size(45, 13);
      label15->TabIndex = 38;
      label15->Text = L"East (m)";
      label15->TextAlign = System::Drawing::ContentAlignment::TopRight;
      // 
      // _eastBox
      // 
      this->_eastBox->DecimalPlaces = 2;
      this->_eastBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      this->_eastBox->Location = System::Drawing::Point(222, 38);
      this->_eastBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
      this->_eastBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
      this->_eastBox->Name = L"_eastBox";
      this->_eastBox->Size = System::Drawing::Size(74, 20);
      this->_eastBox->TabIndex = 39;
      // 
      // label14
      // 
      label14->AutoSize = true;
      label14->Location = System::Drawing::Point(9, 40);
      label14->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label14->Name = L"label14";
      label14->Size = System::Drawing::Size(49, 13);
      label14->TabIndex = 36;
      label14->Text = L"West (m)";
      label14->TextAlign = System::Drawing::ContentAlignment::TopRight;
      // 
      // _westBox
      // 
      this->_westBox->DecimalPlaces = 2;
      this->_westBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      this->_westBox->Location = System::Drawing::Point(64, 38);
      this->_westBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
      this->_westBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
      this->_westBox->Name = L"_westBox";
      this->_westBox->Size = System::Drawing::Size(74, 20);
      this->_westBox->TabIndex = 37;
      // 
      // label13
      // 
      label13->AutoSize = true;
      label13->Location = System::Drawing::Point(87, 66);
      label13->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label13->Name = L"label13";
      label13->Size = System::Drawing::Size(52, 13);
      label13->TabIndex = 34;
      label13->Text = L"South (m)";
      label13->TextAlign = System::Drawing::ContentAlignment::TopRight;
      // 
      // _southBox
      // 
      this->_southBox->DecimalPlaces = 2;
      this->_southBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      this->_southBox->Location = System::Drawing::Point(145, 64);
      this->_southBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
      this->_southBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
      this->_southBox->Name = L"_southBox";
      this->_southBox->Size = System::Drawing::Size(74, 20);
      this->_southBox->TabIndex = 35;
      // 
      // label12
      // 
      label12->AutoSize = true;
      label12->Location = System::Drawing::Point(89, 14);
      label12->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label12->Name = L"label12";
      label12->Size = System::Drawing::Size(50, 13);
      label12->TabIndex = 32;
      label12->Text = L"North (m)";
      label12->TextAlign = System::Drawing::ContentAlignment::TopRight;
      // 
      // _northBox
      // 
      this->_northBox->DecimalPlaces = 2;
      this->_northBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      this->_northBox->Location = System::Drawing::Point(145, 12);
      this->_northBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
      this->_northBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
      this->_northBox->Name = L"_northBox";
      this->_northBox->Size = System::Drawing::Size(74, 20);
      this->_northBox->TabIndex = 33;
      // 
      // BackdropOptionsForm
      // 
      this->AcceptButton = this->_okBtn;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->_cancelBtn;
      this->ClientSize = System::Drawing::Size(308, 140);
      this->Controls->Add(label15);
      this->Controls->Add(this->_eastBox);
      this->Controls->Add(label14);
      this->Controls->Add(this->_westBox);
      this->Controls->Add(label13);
      this->Controls->Add(this->_southBox);
      this->Controls->Add(label12);
      this->Controls->Add(this->_northBox);
      this->Controls->Add(this->_cancelBtn);
      this->Controls->Add(this->_okBtn);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"BackdropOptionsForm";
      this->ShowIcon = false;
      this->ShowInTaskbar = false;
      this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
      this->Text = L"Backdrop options";
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_eastBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_westBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_southBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_northBox))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
	};
}
