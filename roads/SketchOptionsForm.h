#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace roads {

	/// <summary>
	/// Summary for SketchOptionsForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SketchOptionsForm : public System::Windows::Forms::Form
	{
	public:
		SketchOptionsForm(void);
		~SketchOptionsForm();

    property double PrimitiveCost {
      double get( void );
    }

    event EventHandler^ PrimitiveCostChanged {
      void add( EventHandler ^handler );
      void remove( EventHandler ^handler );
    }
    

  private:
    TrackBar  ^_primitiveCostBar;
    Button    ^_okBtn;
    Button    ^_cancelBtn;

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
      System::Windows::Forms::Label^  label2;
      System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(SketchOptionsForm::typeid));
      this->_primitiveCostBar = (gcnew System::Windows::Forms::TrackBar());
      this->_okBtn = (gcnew System::Windows::Forms::Button());
      this->_cancelBtn = (gcnew System::Windows::Forms::Button());
      label1 = (gcnew System::Windows::Forms::Label());
      label2 = (gcnew System::Windows::Forms::Label());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_primitiveCostBar))->BeginInit();
      this->SuspendLayout();
      // 
      // label1
      // 
      label1->AutoSize = true;
      label1->Location = System::Drawing::Point(12, 13);
      label1->Name = L"label1";
      label1->Size = System::Drawing::Size(75, 13);
      label1->TabIndex = 5;
      label1->Text = L"Less primitives";
      // 
      // label2
      // 
      label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      label2->AutoSize = true;
      label2->Location = System::Drawing::Point(136, 13);
      label2->Name = L"label2";
      label2->Size = System::Drawing::Size(77, 13);
      label2->TabIndex = 6;
      label2->Text = L"More primitives";
      // 
      // _primitiveCostBar
      // 
      this->_primitiveCostBar->LargeChange = 50;
      this->_primitiveCostBar->Location = System::Drawing::Point(12, 29);
      this->_primitiveCostBar->Maximum = 200;
      this->_primitiveCostBar->Name = L"_primitiveCostBar";
      this->_primitiveCostBar->Size = System::Drawing::Size(201, 42);
      this->_primitiveCostBar->SmallChange = 10;
      this->_primitiveCostBar->TabIndex = 2;
      this->_primitiveCostBar->TickFrequency = 10;
      this->_primitiveCostBar->Value = 100;
      // 
      // _okBtn
      // 
      this->_okBtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->_okBtn->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->_okBtn->Location = System::Drawing::Point(12, 72);
      this->_okBtn->Name = L"_okBtn";
      this->_okBtn->Size = System::Drawing::Size(75, 23);
      this->_okBtn->TabIndex = 3;
      this->_okBtn->Text = L"OK";
      this->_okBtn->UseVisualStyleBackColor = true;
      this->_okBtn->Click += gcnew System::EventHandler(this, &SketchOptionsForm::OnButtonClick);
      // 
      // _cancelBtn
      // 
      this->_cancelBtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
      this->_cancelBtn->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      this->_cancelBtn->Location = System::Drawing::Point(138, 72);
      this->_cancelBtn->Name = L"_cancelBtn";
      this->_cancelBtn->Size = System::Drawing::Size(75, 23);
      this->_cancelBtn->TabIndex = 4;
      this->_cancelBtn->Text = L"Cancel";
      this->_cancelBtn->UseVisualStyleBackColor = true;
      this->_cancelBtn->Click += gcnew System::EventHandler(this, &SketchOptionsForm::OnButtonClick);
      // 
      // SketchOptionsForm
      // 
      this->AcceptButton = this->_okBtn;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->_cancelBtn;
      this->ClientSize = System::Drawing::Size(225, 107);
      this->Controls->Add(label2);
      this->Controls->Add(label1);
      this->Controls->Add(this->_cancelBtn);
      this->Controls->Add(this->_okBtn);
      this->Controls->Add(this->_primitiveCostBar);
      this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
      this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"SketchOptionsForm";
      this->ShowInTaskbar = false;
      this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
      this->Text = L"Sketch options";
      this->TopMost = true;
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_primitiveCostBar))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
    
    void OnButtonClick( Object ^sender, EventArgs ^e );
  };
}
