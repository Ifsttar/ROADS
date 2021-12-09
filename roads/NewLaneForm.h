#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace roads {

	/// <summary>
	/// Summary for NewLaneForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	private ref class NewLaneForm : public System::Windows::Forms::Form
	{
	public:
		NewLaneForm( int min, int max );

    property int LaneIndex {
      int get(void);
    }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~NewLaneForm();

  private:
    Button        ^_OKBtn;
    Button        ^_CancelBtn;
    NumericUpDown ^_indexBox;
    int           _prevValue;

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
      System::Windows::Forms::Label^  label1;
      System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(NewLaneForm::typeid));
      this->_OKBtn = (gcnew System::Windows::Forms::Button());
      this->_CancelBtn = (gcnew System::Windows::Forms::Button());
      this->_indexBox = (gcnew System::Windows::Forms::NumericUpDown());
      label1 = (gcnew System::Windows::Forms::Label());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_indexBox))->BeginInit();
      this->SuspendLayout();
      // 
      // label1
      // 
      label1->AutoSize = true;
      label1->Location = System::Drawing::Point(13, 19);
      label1->Name = L"label1";
      label1->Size = System::Drawing::Size(59, 13);
      label1->TabIndex = 0;
      label1->Text = L"Lane index";
      // 
      // _OKBtn
      // 
      this->_OKBtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->_OKBtn->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->_OKBtn->Location = System::Drawing::Point(13, 54);
      this->_OKBtn->Name = L"_OKBtn";
      this->_OKBtn->Size = System::Drawing::Size(75, 23);
      this->_OKBtn->TabIndex = 2;
      this->_OKBtn->Text = L"OK";
      this->_OKBtn->UseVisualStyleBackColor = true;
      // 
      // _CancelBtn
      // 
      this->_CancelBtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->_CancelBtn->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->_CancelBtn->Location = System::Drawing::Point(108, 54);
      this->_CancelBtn->Name = L"_CancelBtn";
      this->_CancelBtn->Size = System::Drawing::Size(75, 23);
      this->_CancelBtn->TabIndex = 3;
      this->_CancelBtn->Text = L"Cancel";
      this->_CancelBtn->UseVisualStyleBackColor = true;
      // 
      // _indexBox
      // 
      this->_indexBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_indexBox->Location = System::Drawing::Point(108, 13);
      this->_indexBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {64, 0, 0, 0});
      this->_indexBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {64, 0, 0, System::Int32::MinValue});
      this->_indexBox->Name = L"_indexBox";
      this->_indexBox->Size = System::Drawing::Size(75, 20);
      this->_indexBox->TabIndex = 1;
      this->_indexBox->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, System::Int32::MinValue});
      this->_indexBox->ValueChanged += gcnew System::EventHandler(this, &NewLaneForm::ValueChanged);
      // 
      // NewLaneForm
      // 
      this->AcceptButton = this->_OKBtn;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->_CancelBtn;
      this->ClientSize = System::Drawing::Size(195, 89);
      this->Controls->Add(label1);
      this->Controls->Add(this->_indexBox);
      this->Controls->Add(this->_CancelBtn);
      this->Controls->Add(this->_OKBtn);
      this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
      this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"NewLaneForm";
      this->ShowIcon = false;
      this->ShowInTaskbar = false;
      this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
      this->Text = L"New lane";
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_indexBox))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

    void ValueChanged( Object ^sender, EventArgs ^e );

  };
}
