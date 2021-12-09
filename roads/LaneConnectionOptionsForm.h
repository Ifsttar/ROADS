#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "DomainLogic.h"

namespace roads {

	/// <summary>
	/// Summary for LaneConnectionOptionsForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class LaneConnectionOptionsForm : public System::Windows::Forms::Form
	{
	public:
		LaneConnectionOptionsForm(void);

    /// Indicate one end of a road.
    enum class RoadEnd {
      Start,
      End
    };

    /// Set road ends for which connection options are shown.
    void SetRoads( const odr::Road *roadA, RoadEnd endA,
                   const odr::Road *roadB, RoadEnd endB );

    /// Get first road for which connection options are shown.
    property const odr::Road* RoadA {
      const odr::Road* get( void );
    }

    /// Get end of first road for which connection options are shown.
    property RoadEnd EndA {
      RoadEnd get( void );
    }

    /// Get second road for which connection options are shown.
    property const odr::Road* RoadB {
      const odr::Road* get( void );
    }

    /// Get end of second road for which connection options are shown.
    property RoadEnd EndB {
      RoadEnd get( void );
    }
    
    /// Event triggered upon a change in options.
    event EventHandler^ OptionsChanged {
      void add( EventHandler ^handler );
      void remove( EventHandler ^handler );
    }

    /// Get a list of indices of lanes of first road to be connected,
    /// ordered from right to left relative to connection road's direction.
    /// Index 0 means corresponding lane in collection returned by LanesB is
    /// collapsed.
    property Generic::List<int>^ LanesA {
      Generic::List<int>^ get( void );
    }

    /// Get a list of indices of lanes of second road to be connected,
    /// ordered from right to left relative to connection road's direction.
    /// Index 0 means corresponding lane in collection returned by LanesA is
    /// collapsed.
    property Generic::List<int>^ LanesB {
      Generic::List<int>^ get( void );
    }

	protected:
		~LaneConnectionOptionsForm();
    !LaneConnectionOptionsForm();

  private:
    Button                      ^_okBtn;
    Button                      ^_cancelBtn;
    DataGridView                ^_roadALanesBox;
    Label                       ^_roadALabelBox;
    Label                       ^_roadBLabelBox;
    DataGridView                ^_roadBLanesBox;
    DataGridViewTextBoxColumn   ^_roadALaneColumn;
    DataGridViewCheckBoxColumn  ^_roadAUseLaneColumn;
    DataGridViewCheckBoxColumn  ^_roadACollapseLaneColumn;
    DataGridViewTextBoxColumn   ^_roadBLaneColumn;
    DataGridViewCheckBoxColumn  ^_roadBUseLaneColumn;
    DataGridViewCheckBoxColumn  ^_roadBCollapseLaneColumn;

    EventHandler                    ^_optionsChanged;

    misc::WeakRef<const odr::Road>  *_roadA;
    RoadEnd                         _endA;
    misc::WeakRef<const odr::Road>  *_roadB;
    RoadEnd                         _endB;

    void OnButtonClick( Object ^sender, EventArgs ^e );
    void OnCellEnter( Object ^sender, DataGridViewCellEventArgs ^e );
    void OnCellEdit( Object ^sender, DataGridViewCellEventArgs ^e );
    void OnDirtyCell( Object ^sender, EventArgs ^e );

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
      this->_okBtn = (gcnew System::Windows::Forms::Button());
      this->_cancelBtn = (gcnew System::Windows::Forms::Button());
      this->_roadALanesBox = (gcnew System::Windows::Forms::DataGridView());
      this->_roadALaneColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
      this->_roadAUseLaneColumn = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
      this->_roadACollapseLaneColumn = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
      this->_roadALabelBox = (gcnew System::Windows::Forms::Label());
      this->_roadBLabelBox = (gcnew System::Windows::Forms::Label());
      this->_roadBLanesBox = (gcnew System::Windows::Forms::DataGridView());
      this->_roadBLaneColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
      this->_roadBUseLaneColumn = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
      this->_roadBCollapseLaneColumn = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_roadALanesBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_roadBLanesBox))->BeginInit();
      this->SuspendLayout();
      // 
      // _okBtn
      // 
      this->_okBtn->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
      this->_okBtn->DialogResult = System::Windows::Forms::DialogResult::OK;
      this->_okBtn->Location = System::Drawing::Point(12, 200);
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
      this->_cancelBtn->Location = System::Drawing::Point(252, 200);
      this->_cancelBtn->Name = L"_cancelBtn";
      this->_cancelBtn->Size = System::Drawing::Size(75, 23);
      this->_cancelBtn->TabIndex = 1;
      this->_cancelBtn->Text = L"Cancel";
      this->_cancelBtn->UseVisualStyleBackColor = true;
      // 
      // _roadALanesBox
      // 
      this->_roadALanesBox->AllowUserToAddRows = false;
      this->_roadALanesBox->AllowUserToDeleteRows = false;
      this->_roadALanesBox->AllowUserToResizeRows = false;
      this->_roadALanesBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left));
      this->_roadALanesBox->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
      this->_roadALanesBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
      this->_roadALanesBox->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
      this->_roadALanesBox->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->_roadALaneColumn, 
        this->_roadAUseLaneColumn, this->_roadACollapseLaneColumn});
      this->_roadALanesBox->Location = System::Drawing::Point(12, 29);
      this->_roadALanesBox->MultiSelect = false;
      this->_roadALanesBox->Name = L"_roadALanesBox";
      this->_roadALanesBox->RowHeadersVisible = false;
      this->_roadALanesBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
      this->_roadALanesBox->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
      this->_roadALanesBox->Size = System::Drawing::Size(150, 165);
      this->_roadALanesBox->TabIndex = 2;
      this->_roadALanesBox->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &LaneConnectionOptionsForm::OnCellEdit);
      this->_roadALanesBox->CurrentCellDirtyStateChanged += gcnew System::EventHandler(this, &LaneConnectionOptionsForm::OnDirtyCell);
      this->_roadALanesBox->CellEnter += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &LaneConnectionOptionsForm::OnCellEnter);
      // 
      // _roadALaneColumn
      // 
      this->_roadALaneColumn->HeaderText = L"Lane";
      this->_roadALaneColumn->Name = L"_roadALaneColumn";
      this->_roadALaneColumn->ReadOnly = true;
      // 
      // _roadAUseLaneColumn
      // 
      this->_roadAUseLaneColumn->HeaderText = L"Use";
      this->_roadAUseLaneColumn->Name = L"_roadAUseLaneColumn";
      this->_roadAUseLaneColumn->Resizable = System::Windows::Forms::DataGridViewTriState::True;
      // 
      // _roadACollapseLaneColumn
      // 
      this->_roadACollapseLaneColumn->HeaderText = L"Collapse";
      this->_roadACollapseLaneColumn->Name = L"_roadACollapseLaneColumn";
      // 
      // _roadALabelBox
      // 
      this->_roadALabelBox->AutoSize = true;
      this->_roadALabelBox->Location = System::Drawing::Point(13, 13);
      this->_roadALabelBox->Name = L"_roadALabelBox";
      this->_roadALabelBox->Size = System::Drawing::Size(113, 13);
      this->_roadALabelBox->TabIndex = 3;
      this->_roadALabelBox->Text = L"(no road end selected)";
      // 
      // _roadBLabelBox
      // 
      this->_roadBLabelBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_roadBLabelBox->AutoSize = true;
      this->_roadBLabelBox->Location = System::Drawing::Point(174, 13);
      this->_roadBLabelBox->Name = L"_roadBLabelBox";
      this->_roadBLabelBox->Size = System::Drawing::Size(113, 13);
      this->_roadBLabelBox->TabIndex = 4;
      this->_roadBLabelBox->Text = L"(no road end selected)";
      // 
      // _roadBLanesBox
      // 
      this->_roadBLanesBox->AllowUserToAddRows = false;
      this->_roadBLanesBox->AllowUserToDeleteRows = false;
      this->_roadBLanesBox->AllowUserToResizeRows = false;
      this->_roadBLanesBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->_roadBLanesBox->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
      this->_roadBLanesBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
      this->_roadBLanesBox->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
      this->_roadBLanesBox->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->_roadBLaneColumn, 
        this->_roadBUseLaneColumn, this->_roadBCollapseLaneColumn});
      this->_roadBLanesBox->Location = System::Drawing::Point(177, 29);
      this->_roadBLanesBox->MultiSelect = false;
      this->_roadBLanesBox->Name = L"_roadBLanesBox";
      this->_roadBLanesBox->RowHeadersVisible = false;
      this->_roadBLanesBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
      this->_roadBLanesBox->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
      this->_roadBLanesBox->Size = System::Drawing::Size(150, 165);
      this->_roadBLanesBox->TabIndex = 5;
      this->_roadBLanesBox->CellValueChanged += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &LaneConnectionOptionsForm::OnCellEdit);
      this->_roadBLanesBox->CurrentCellDirtyStateChanged += gcnew System::EventHandler(this, &LaneConnectionOptionsForm::OnDirtyCell);
      this->_roadBLanesBox->CellEnter += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &LaneConnectionOptionsForm::OnCellEnter);
      // 
      // _roadBLaneColumn
      // 
      this->_roadBLaneColumn->HeaderText = L"Lane";
      this->_roadBLaneColumn->Name = L"_roadBLaneColumn";
      this->_roadBLaneColumn->ReadOnly = true;
      // 
      // _roadBUseLaneColumn
      // 
      this->_roadBUseLaneColumn->HeaderText = L"Use";
      this->_roadBUseLaneColumn->Name = L"_roadBUseLaneColumn";
      this->_roadBUseLaneColumn->Resizable = System::Windows::Forms::DataGridViewTriState::True;
      // 
      // _roadBCollapseLaneColumn
      // 
      this->_roadBCollapseLaneColumn->HeaderText = L"Collapse";
      this->_roadBCollapseLaneColumn->Name = L"_roadBCollapseLaneColumn";
      // 
      // LaneConnectionOptionsForm
      // 
      this->AcceptButton = this->_okBtn;
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->CancelButton = this->_cancelBtn;
      this->ClientSize = System::Drawing::Size(339, 235);
      this->Controls->Add(this->_roadBLanesBox);
      this->Controls->Add(this->_roadBLabelBox);
      this->Controls->Add(this->_roadALabelBox);
      this->Controls->Add(this->_roadALanesBox);
      this->Controls->Add(this->_cancelBtn);
      this->Controls->Add(this->_okBtn);
      this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
      this->MaximizeBox = false;
      this->MinimizeBox = false;
      this->Name = L"LaneConnectionOptionsForm";
      this->ShowIcon = false;
      this->ShowInTaskbar = false;
      this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
      this->Text = L"Connection options";
      this->TopMost = true;
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_roadALanesBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_roadBLanesBox))->EndInit();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  };
}
