//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Poly3HelperForm.h
//! \brief Declaration of form class roads::Poly3HelperForm.

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace roads {

	/// <summary>
	/// Summary for Poly3HelperForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	private ref class Poly3HelperForm : public System::Windows::Forms::Form
	{
	public:
		Poly3HelperForm(void);

    property String^ EditedValueName {
      String ^get( void );
      void set( String ^s );
    }

    property double Length {
      double get(void);
      void set(double);
    }

    property double InitialValue {
      double get(void);
      void set(double);
    }

    property double InitialDerivative {
      double get(void);
      void set(double);
    }

    property double FinalValue {
      double get(void);
      void set(double);
    }

    property double FinalDerivative {
      double get(void);
      void set(double);
    }

    property double A { double get(void); }
    property double B { double get(void); }
    property double C { double get(void); }
    property double D { double get(void); }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Poly3HelperForm();

	private:
    String        ^_editedValueName;
    Label         ^_initialValueLbl;
    NumericUpDown ^_initialValueBox;
    Label         ^_initialDerivativeLbl;
    NumericUpDown ^_initialDerivativeBox;
    Label         ^_finalValueLbl;
    NumericUpDown ^_finalValueBox;
    Label         ^_finalDerivativeLbl;
    NumericUpDown ^_finalDerivativeBox;
    Button        ^_okBtn;
    Button        ^_cancelBtn;
    double        _length;

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
      this->_initialValueBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_initialValueLbl = (gcnew System::Windows::Forms::Label());
      this->_initialDerivativeLbl = (gcnew System::Windows::Forms::Label());
      this->_initialDerivativeBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_finalDerivativeLbl = (gcnew System::Windows::Forms::Label());
      this->_finalDerivativeBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_finalValueLbl = (gcnew System::Windows::Forms::Label());
      this->_finalValueBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_okBtn = (gcnew System::Windows::Forms::Button());
      this->_cancelBtn = (gcnew System::Windows::Forms::Button());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_initialValueBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_initialDerivativeBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_finalDerivativeBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_finalValueBox))->BeginInit();
      this->SuspendLayout();
      // 
      // _initialValueBox
      // 
      this->_initialValueBox->DecimalPlaces = 2;
      this->_initialValueBox->Location = System::Drawing::Point(108, 12);
      this->_initialValueBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
      this->_initialValueBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, System::Int32::MinValue});
      this->_initialValueBox->Name = L"_initialValueBox";
      this->_initialValueBox->Size = System::Drawing::Size(82, 20);
      this->_initialValueBox->TabIndex = 0;
      // 
      // _initialValueLbl
      // 
      this->_initialValueLbl->AutoSize = true;
      this->_initialValueLbl->Location = System::Drawing::Point(13, 14);
      this->_initialValueLbl->Name = L"_initialValueLbl";
      this->_initialValueLbl->Size = System::Drawing::Size(60, 13);
      this->_initialValueLbl->TabIndex = 1;
      this->_initialValueLbl->Text = L"Initial value";
      // 
      // _initialDerivativeLbl
      // 
      this->_initialDerivativeLbl->AutoSize = true;
      this->_initialDerivativeLbl->Location = System::Drawing::Point(13, 40);
      this->_initialDerivativeLbl->Name = L"_initialDerivativeLbl";
      this->_initialDerivativeLbl->Size = System::Drawing::Size(80, 13);
      this->_initialDerivativeLbl->TabIndex = 3;
      this->_initialDerivativeLbl->Text = L"Initial derivative";
      // 
      // _initialDerivativeBox
      // 
      this->_initialDerivativeBox->DecimalPlaces = 2;
      this->_initialDerivativeBox->Location = System::Drawing::Point(108, 38);
      this->_initialDerivativeBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
      this->_initialDerivativeBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, System::Int32::MinValue});
      this->_initialDerivativeBox->Name = L"_initialDerivativeBox";
      this->_initialDerivativeBox->Size = System::Drawing::Size(82, 20);
      this->_initialDerivativeBox->TabIndex = 2;
      // 
      // _finalDerivativeLbl
      // 
      this->_finalDerivativeLbl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_finalDerivativeLbl->AutoSize = true;
      this->_finalDerivativeLbl->Location = System::Drawing::Point(210, 40);
      this->_finalDerivativeLbl->Name = L"_finalDerivativeLbl";
      this->_finalDerivativeLbl->Size = System::Drawing::Size(78, 13);
      this->_finalDerivativeLbl->TabIndex = 7;
      this->_finalDerivativeLbl->Text = L"Final derivative";
      // 
      // _finalDerivativeBox
      // 
      this->_finalDerivativeBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_finalDerivativeBox->DecimalPlaces = 2;
      this->_finalDerivativeBox->Location = System::Drawing::Point(305, 38);
      this->_finalDerivativeBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
      this->_finalDerivativeBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, System::Int32::MinValue});
      this->_finalDerivativeBox->Name = L"_finalDerivativeBox";
      this->_finalDerivativeBox->Size = System::Drawing::Size(82, 20);
      this->_finalDerivativeBox->TabIndex = 6;
      // 
      // _finalValueLbl
      // 
      this->_finalValueLbl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_finalValueLbl->AutoSize = true;
      this->_finalValueLbl->Location = System::Drawing::Point(210, 14);
      this->_finalValueLbl->Name = L"_finalValueLbl";
      this->_finalValueLbl->Size = System::Drawing::Size(58, 13);
      this->_finalValueLbl->TabIndex = 5;
      this->_finalValueLbl->Text = L"Final value";
      // 
      // _finalValueBox
      // 
      this->_finalValueBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_finalValueBox->DecimalPlaces = 2;
      this->_finalValueBox->Location = System::Drawing::Point(305, 12);
      this->_finalValueBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
      this->_finalValueBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, System::Int32::MinValue});
      this->_finalValueBox->Name = L"_finalValueBox";
      this->_finalValueBox->Size = System::Drawing::Size(82, 20);
      this->_finalValueBox->TabIndex = 4;
      // 
      // _okBtn
      // 
      this->_okBtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->_okBtn->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->_okBtn->Location = System::Drawing::Point(13, 86);
      this->_okBtn->Name = L"_okBtn";
      this->_okBtn->Size = System::Drawing::Size(75, 23);
      this->_okBtn->TabIndex = 8;
      this->_okBtn->Text = L"OK";
      this->_okBtn->UseVisualStyleBackColor = true;
      // 
      // _cancelBtn
      // 
      this->_cancelBtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->_cancelBtn->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->_cancelBtn->Location = System::Drawing::Point(311, 86);
      this->_cancelBtn->Name = L"_cancelBtn";
      this->_cancelBtn->Size = System::Drawing::Size(75, 23);
      this->_cancelBtn->TabIndex = 9;
      this->_cancelBtn->Text = L"Cancel";
      this->_cancelBtn->UseVisualStyleBackColor = true;
      // 
      // Poly3HelperForm
      // 
      this->AcceptButton = this->_okBtn;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->_cancelBtn;
      this->ClientSize = System::Drawing::Size(399, 121);
      this->Controls->Add(this->_cancelBtn);
      this->Controls->Add(this->_okBtn);
      this->Controls->Add(this->_finalDerivativeLbl);
      this->Controls->Add(this->_finalDerivativeBox);
      this->Controls->Add(this->_finalValueLbl);
      this->Controls->Add(this->_finalValueBox);
      this->Controls->Add(this->_initialDerivativeLbl);
      this->Controls->Add(this->_initialDerivativeBox);
      this->Controls->Add(this->_initialValueLbl);
      this->Controls->Add(this->_initialValueBox);
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"Poly3HelperForm";
      this->ShowIcon = false;
      this->ShowInTaskbar = false;
      this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
      this->Text = L"Poly3HelperForm";
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_initialValueBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_initialDerivativeBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_finalDerivativeBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_finalValueBox))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
	};
}
