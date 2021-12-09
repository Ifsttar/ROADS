//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file AboutForm.h
//! \brief roads::AboutForm class definition.

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace roads {

	/// <summary>
	/// Summary for AboutForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	private ref class AboutForm : public System::Windows::Forms::Form
	{
	public:
		AboutForm(void)
		{
			InitializeComponent();
      _linkLivic->Links->Add( 0, _linkLivic->Text->Length, _linkLivic->Text );
      _linkOpendrive->Links->Add( 0, _linkOpendrive->Text->Length, _linkOpendrive->Text );
      _linkTinyxml->Links->Add( 0, _linkTinyxml->Text->Length, _linkTinyxml->Text );
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AboutForm()
		{
			if (components)
			{
				delete components;
      }
		}
    LinkLabel ^_linkLivic;
    LinkLabel ^_linkOpendrive;
    LinkLabel ^_linkTinyxml;

	private:
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
      System::Windows::Forms::Label^  label2;
      System::Windows::Forms::Label^  label3;
      System::Windows::Forms::Label^  label4;
      System::Windows::Forms::Button^  button1;
      System::Windows::Forms::Label^  label1;
      System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(AboutForm::typeid));
      this->_linkLivic = (gcnew System::Windows::Forms::LinkLabel());
      this->_linkOpendrive = (gcnew System::Windows::Forms::LinkLabel());
      this->_linkTinyxml = (gcnew System::Windows::Forms::LinkLabel());
      label2 = (gcnew System::Windows::Forms::Label());
      label3 = (gcnew System::Windows::Forms::Label());
      label4 = (gcnew System::Windows::Forms::Label());
      button1 = (gcnew System::Windows::Forms::Button());
      label1 = (gcnew System::Windows::Forms::Label());
      this->SuspendLayout();
      // 
      // label2
      // 
      label2->AutoSize = true;
      label2->Location = System::Drawing::Point(12, 44);
      label2->Margin = System::Windows::Forms::Padding(3, 16, 3, 0);
      label2->Name = L"label2";
      label2->Size = System::Drawing::Size(288, 13);
      label2->TabIndex = 1;
      label2->Text = L"roads, libodr and libasphalt are copyright ©2008-2010 LIVIC";
      // 
      // label3
      // 
      label3->AutoEllipsis = true;
      label3->AutoSize = true;
      label3->Location = System::Drawing::Point(12, 102);
      label3->Margin = System::Windows::Forms::Padding(3, 16, 3, 0);
      label3->Name = L"label3";
      label3->Size = System::Drawing::Size(373, 13);
      label3->TabIndex = 2;
      label3->Text = L"OpenDRIVE® v1.2 format specificatin is copyright ©2006-2008 VIRES GmbH";
      // 
      // label4
      // 
      label4->AutoSize = true;
      label4->Location = System::Drawing::Point(12, 160);
      label4->Margin = System::Windows::Forms::Padding(3, 16, 3, 0);
      label4->Name = L"label4";
      label4->Size = System::Drawing::Size(263, 13);
      label4->TabIndex = 3;
      label4->Text = L"TinyXML v2+ is copyright ©2000-2006 Lee Thomason";
      // 
      // button1
      // 
      button1->Location = System::Drawing::Point(157, 231);
      button1->Name = L"button1";
      button1->Size = System::Drawing::Size(75, 23);
      button1->TabIndex = 7;
      button1->Text = L"OK";
      button1->UseVisualStyleBackColor = true;
      button1->Click += gcnew System::EventHandler(this, &AboutForm::button1_Click);
      // 
      // label1
      // 
      label1->AutoSize = true;
      label1->Font = (gcnew System::Drawing::Font(L"Arial", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
        static_cast<System::Byte>(0)));
      label1->Location = System::Drawing::Point(12, 9);
      label1->Name = L"label1";
      label1->Size = System::Drawing::Size(88, 19);
      label1->TabIndex = 0;
      label1->Text = L"roads v0.2";
      // 
      // _linkLivic
      // 
      this->_linkLivic->AutoSize = true;
      this->_linkLivic->Location = System::Drawing::Point(245, 73);
      this->_linkLivic->Margin = System::Windows::Forms::Padding(3, 16, 3, 0);
      this->_linkLivic->Name = L"_linkLivic";
      this->_linkLivic->Size = System::Drawing::Size(141, 13);
      this->_linkLivic->TabIndex = 4;
      this->_linkLivic->TabStop = true;
      this->_linkLivic->Tag = L"";
      this->_linkLivic->Text = L"http://www.inrets.fr/ur/livic/";
      this->_linkLivic->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutForm::linkClicked);
      // 
      // _linkOpendrive
      // 
      this->_linkOpendrive->AutoSize = true;
      this->_linkOpendrive->Location = System::Drawing::Point(251, 131);
      this->_linkOpendrive->Margin = System::Windows::Forms::Padding(3, 16, 3, 0);
      this->_linkOpendrive->Name = L"_linkOpendrive";
      this->_linkOpendrive->Size = System::Drawing::Size(135, 13);
      this->_linkOpendrive->TabIndex = 5;
      this->_linkOpendrive->TabStop = true;
      this->_linkOpendrive->Tag = L"";
      this->_linkOpendrive->Text = L"http://www.opendrive.org/";
      this->_linkOpendrive->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutForm::linkClicked);
      // 
      // _linkTinyxml
      // 
      this->_linkTinyxml->AutoSize = true;
      this->_linkTinyxml->Location = System::Drawing::Point(196, 189);
      this->_linkTinyxml->Margin = System::Windows::Forms::Padding(3, 16, 3, 0);
      this->_linkTinyxml->Name = L"_linkTinyxml";
      this->_linkTinyxml->Size = System::Drawing::Size(190, 13);
      this->_linkTinyxml->TabIndex = 6;
      this->_linkTinyxml->TabStop = true;
      this->_linkTinyxml->Tag = L"";
      this->_linkTinyxml->Text = L"http://www.grinninglizard.com/tinyxml/";
      this->_linkTinyxml->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutForm::linkClicked);
      // 
      // AboutForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->BackColor = System::Drawing::SystemColors::Control;
      this->ClientSize = System::Drawing::Size(398, 266);
      this->Controls->Add(button1);
      this->Controls->Add(this->_linkTinyxml);
      this->Controls->Add(this->_linkOpendrive);
      this->Controls->Add(this->_linkLivic);
      this->Controls->Add(label4);
      this->Controls->Add(label3);
      this->Controls->Add(label2);
      this->Controls->Add(label1);
      this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"AboutForm";
      this->ShowInTaskbar = false;
      this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
      this->Text = L"About roads";
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  
  private:
    Void linkClicked( Object ^sender, LinkLabelLinkClickedEventArgs ^e ) {
      String ^url = dynamic_cast<String^>( e->Link->LinkData );
      if( !url ) return;
      try{ System::Diagnostics::Process::Start(url); } catch(...) {return;}
      e->Link->Visited = true;
    }

    Void button1_Click( Object ^sender,  EventArgs ^e ) {
      Close();
    }
  };
}
