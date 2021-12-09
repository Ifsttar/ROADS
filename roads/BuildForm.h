//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file BuildForm.h
//! \brief roads::BuildForm class definition.

#pragma once

#include <odr/odr.h>
#include <asphalt/asphalt.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace roads {

	/// <summary>
	/// Summary for BuildForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	private ref class BuildForm : public System::Windows::Forms::Form
	{
	public:
    BuildForm( const odr::Document *odrDocument )
		{
			InitializeComponent();
      _formatBox->SelectedIndex = 0;
      _odrDocument = odrDocument;
      _builder = asphalt_createBuilder();
      _mesh = 0;
      _buildThread = nullptr;
      _northBox->Value = Decimal( _odrDocument->header()->north() );
      _southBox->Value = Decimal( _odrDocument->header()->south() );
      _eastBox->Value  = Decimal( _odrDocument->header()->east() );
      _westBox->Value  = Decimal( _odrDocument->header()->west() );
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~BuildForm()
		{
      delete _builder;
      delete _mesh;
			if (components)
			{
				delete components;
			}
		}

  private:
    const odr::Document  *_odrDocument;
    asphalt::MeshBuilder *_builder;
    asphalt::Mesh        *_mesh;
    System::Threading::Thread ^_buildThread;

    GroupBox      ^_optionsGroup;
    NumericUpDown ^_maxEdgeAngleBox;
    NumericUpDown ^_maxSurfAngleBox;
    NumericUpDown ^_minEdgeLengthBox;
    NumericUpDown ^_maxRatioBox;
    NumericUpDown ^_maxEdgeLengthBox;

    CheckBox      ^_terrainCheckbox;
    GroupBox      ^_terrainGroup;

    TextBox       ^_heightPathBox;
    NumericUpDown ^_maxHeightBox;
    NumericUpDown ^_minHeightBox;
    NumericUpDown ^_gridBox;
    NumericUpDown ^_terrainDistanceBox;
    NumericUpDown ^_northBox;
    NumericUpDown ^_southBox;
    NumericUpDown ^_eastBox;
    NumericUpDown ^_westBox;

    GroupBox      ^_outputGroup;
    ComboBox      ^_formatBox;
    TextBox       ^_pathBox;

    Button        ^_buildBtn;
    ProgressBar   ^_progressBar;

    Timer         ^_progressTimer;

    Label         ^_triangleCount;
    Label         ^_vertexCount;

		/// <summary>
		/// Required designer variable.
		/// </summary>
    System::ComponentModel::IContainer^  components;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
      this->components = (gcnew System::ComponentModel::Container());
      System::Windows::Forms::Label^  label5;
      System::Windows::Forms::Label^  label4;
      System::Windows::Forms::Label^  label3;
      System::Windows::Forms::Label^  label2;
      System::Windows::Forms::Label^  label1;
      System::Windows::Forms::Button^  button3;
      System::Windows::Forms::Button^  button1;
      System::Windows::Forms::Label^  label7;
      System::Windows::Forms::Label^  label6;
      System::Windows::Forms::Label^  label15;
      System::Windows::Forms::Label^  label14;
      System::Windows::Forms::Label^  label13;
      System::Windows::Forms::Label^  label12;
      System::Windows::Forms::Label^  label11;
      System::Windows::Forms::Label^  label9;
      System::Windows::Forms::Label^  label10;
      System::Windows::Forms::Button^  button2;
      System::Windows::Forms::Label^  label8;
      System::Windows::Forms::GroupBox^  groupBox1;
      System::Windows::Forms::Label^  label17;
      System::Windows::Forms::Label^  label16;
      System::Windows::Forms::Label^  label18;
      System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(BuildForm::typeid));
      this->_triangleCount = (gcnew System::Windows::Forms::Label());
      this->_vertexCount = (gcnew System::Windows::Forms::Label());
      this->_terrainGroup = (gcnew System::Windows::Forms::GroupBox());
      this->_terrainDistanceBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_eastBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_westBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_southBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_northBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_gridBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_maxHeightBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_minHeightBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_heightPathBox = (gcnew System::Windows::Forms::TextBox());
      this->_optionsGroup = (gcnew System::Windows::Forms::GroupBox());
      this->_maxRatioBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_maxEdgeLengthBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_minEdgeLengthBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_maxSurfAngleBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_maxEdgeAngleBox = (gcnew System::Windows::Forms::NumericUpDown());
      this->_outputGroup = (gcnew System::Windows::Forms::GroupBox());
      this->_pathBox = (gcnew System::Windows::Forms::TextBox());
      this->_formatBox = (gcnew System::Windows::Forms::ComboBox());
      this->_progressBar = (gcnew System::Windows::Forms::ProgressBar());
      this->_buildBtn = (gcnew System::Windows::Forms::Button());
      this->_progressTimer = (gcnew System::Windows::Forms::Timer(this->components));
      this->_terrainCheckbox = (gcnew System::Windows::Forms::CheckBox());
      label5 = (gcnew System::Windows::Forms::Label());
      label4 = (gcnew System::Windows::Forms::Label());
      label3 = (gcnew System::Windows::Forms::Label());
      label2 = (gcnew System::Windows::Forms::Label());
      label1 = (gcnew System::Windows::Forms::Label());
      button3 = (gcnew System::Windows::Forms::Button());
      button1 = (gcnew System::Windows::Forms::Button());
      label7 = (gcnew System::Windows::Forms::Label());
      label6 = (gcnew System::Windows::Forms::Label());
      label15 = (gcnew System::Windows::Forms::Label());
      label14 = (gcnew System::Windows::Forms::Label());
      label13 = (gcnew System::Windows::Forms::Label());
      label12 = (gcnew System::Windows::Forms::Label());
      label11 = (gcnew System::Windows::Forms::Label());
      label9 = (gcnew System::Windows::Forms::Label());
      label10 = (gcnew System::Windows::Forms::Label());
      button2 = (gcnew System::Windows::Forms::Button());
      label8 = (gcnew System::Windows::Forms::Label());
      groupBox1 = (gcnew System::Windows::Forms::GroupBox());
      label17 = (gcnew System::Windows::Forms::Label());
      label16 = (gcnew System::Windows::Forms::Label());
      label18 = (gcnew System::Windows::Forms::Label());
      groupBox1->SuspendLayout();
      this->_terrainGroup->SuspendLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_terrainDistanceBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_eastBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_westBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_southBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_northBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_gridBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_maxHeightBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_minHeightBox))->BeginInit();
      this->_optionsGroup->SuspendLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_maxRatioBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_maxEdgeLengthBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_minEdgeLengthBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_maxSurfAngleBox))->BeginInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_maxEdgeAngleBox))->BeginInit();
      this->_outputGroup->SuspendLayout();
      this->SuspendLayout();
      // 
      // label5
      // 
      label5->AutoSize = true;
      label5->Location = System::Drawing::Point(11, 134);
      label5->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label5->Name = L"label5";
      label5->Size = System::Drawing::Size(109, 13);
      label5->TabIndex = 9;
      label5->Text = L"Maximum aspect ratio";
      // 
      // label4
      // 
      label4->AutoSize = true;
      label4->Location = System::Drawing::Point(11, 108);
      label4->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label4->Name = L"label4";
      label4->Size = System::Drawing::Size(127, 13);
      label4->TabIndex = 7;
      label4->Text = L"Maximum edge lenght (m)";
      // 
      // label3
      // 
      label3->AutoSize = true;
      label3->Location = System::Drawing::Point(11, 83);
      label3->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label3->Name = L"label3";
      label3->Size = System::Drawing::Size(124, 13);
      label3->TabIndex = 5;
      label3->Text = L"Minimum edge lenght (m)";
      // 
      // label2
      // 
      label2->AutoSize = true;
      label2->Location = System::Drawing::Point(11, 58);
      label2->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label2->Name = L"label2";
      label2->Size = System::Drawing::Size(145, 13);
      label2->TabIndex = 3;
      label2->Text = L"Maximum surface angle (deg)";
      // 
      // label1
      // 
      label1->AutoSize = true;
      label1->Location = System::Drawing::Point(11, 33);
      label1->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label1->Name = L"label1";
      label1->Size = System::Drawing::Size(151, 13);
      label1->TabIndex = 1;
      label1->Text = L"Maximum XY edge angle (deg)";
      // 
      // button3
      // 
      button3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      button3->Location = System::Drawing::Point(239, 50);
      button3->Margin = System::Windows::Forms::Padding(8);
      button3->Name = L"button3";
      button3->Size = System::Drawing::Size(60, 23);
      button3->TabIndex = 38;
      button3->Text = L"Browse...";
      button3->UseVisualStyleBackColor = true;
      button3->Click += gcnew System::EventHandler(this, &BuildForm::BrowseOutput);
      // 
      // button1
      // 
      button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      button1->Enabled = false;
      button1->Location = System::Drawing::Point(202, 23);
      button1->Margin = System::Windows::Forms::Padding(8);
      button1->Name = L"button1";
      button1->Size = System::Drawing::Size(97, 23);
      button1->TabIndex = 35;
      button1->Text = L"Format options...";
      button1->UseVisualStyleBackColor = true;
      // 
      // label7
      // 
      label7->AutoSize = true;
      label7->Location = System::Drawing::Point(11, 53);
      label7->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label7->Name = L"label7";
      label7->Size = System::Drawing::Size(29, 13);
      label7->TabIndex = 36;
      label7->Text = L"Path";
      // 
      // label6
      // 
      label6->AutoSize = true;
      label6->Location = System::Drawing::Point(11, 28);
      label6->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label6->Name = L"label6";
      label6->Size = System::Drawing::Size(39, 13);
      label6->TabIndex = 33;
      label6->Text = L"Format";
      // 
      // label15
      // 
      label15->AutoSize = true;
      label15->Location = System::Drawing::Point(173, 178);
      label15->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label15->Name = L"label15";
      label15->Size = System::Drawing::Size(45, 13);
      label15->TabIndex = 30;
      label15->Text = L"East (m)";
      label15->TextAlign = System::Drawing::ContentAlignment::TopRight;
      // 
      // label14
      // 
      label14->AutoSize = true;
      label14->Location = System::Drawing::Point(9, 178);
      label14->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label14->Name = L"label14";
      label14->Size = System::Drawing::Size(49, 13);
      label14->TabIndex = 28;
      label14->Text = L"West (m)";
      label14->TextAlign = System::Drawing::ContentAlignment::TopRight;
      // 
      // label13
      // 
      label13->AutoSize = true;
      label13->Location = System::Drawing::Point(86, 204);
      label13->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label13->Name = L"label13";
      label13->Size = System::Drawing::Size(52, 13);
      label13->TabIndex = 26;
      label13->Text = L"South (m)";
      label13->TextAlign = System::Drawing::ContentAlignment::TopRight;
      // 
      // label12
      // 
      label12->AutoSize = true;
      label12->Location = System::Drawing::Point(88, 152);
      label12->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label12->Name = L"label12";
      label12->Size = System::Drawing::Size(50, 13);
      label12->TabIndex = 24;
      label12->Text = L"North (m)";
      label12->TextAlign = System::Drawing::ContentAlignment::TopRight;
      // 
      // label11
      // 
      label11->AutoSize = true;
      label11->Location = System::Drawing::Point(11, 102);
      label11->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label11->Name = L"label11";
      label11->Size = System::Drawing::Size(83, 13);
      label11->TabIndex = 20;
      label11->Text = L"Grid spacing (m)";
      // 
      // label9
      // 
      label9->AutoSize = true;
      label9->Location = System::Drawing::Point(11, 77);
      label9->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label9->Name = L"label9";
      label9->Size = System::Drawing::Size(100, 13);
      label9->TabIndex = 18;
      label9->Text = L"Maximum height (m)";
      // 
      // label10
      // 
      label10->AutoSize = true;
      label10->Location = System::Drawing::Point(11, 52);
      label10->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label10->Name = L"label10";
      label10->Size = System::Drawing::Size(97, 13);
      label10->TabIndex = 16;
      label10->Text = L"Minimum height (m)";
      // 
      // button2
      // 
      button2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      button2->Location = System::Drawing::Point(239, 22);
      button2->Margin = System::Windows::Forms::Padding(8);
      button2->Name = L"button2";
      button2->Size = System::Drawing::Size(60, 23);
      button2->TabIndex = 15;
      button2->Text = L"Browse...";
      button2->UseVisualStyleBackColor = true;
      button2->Click += gcnew System::EventHandler(this, &BuildForm::BrowseHeightmap);
      // 
      // label8
      // 
      label8->AutoSize = true;
      label8->Location = System::Drawing::Point(11, 27);
      label8->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label8->Name = L"label8";
      label8->Size = System::Drawing::Size(82, 13);
      label8->TabIndex = 13;
      label8->Text = L"Heightmap path";
      // 
      // groupBox1
      // 
      groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      groupBox1->Controls->Add(this->_triangleCount);
      groupBox1->Controls->Add(this->_vertexCount);
      groupBox1->Controls->Add(label17);
      groupBox1->Controls->Add(label16);
      groupBox1->Location = System::Drawing::Point(12, 582);
      groupBox1->Name = L"groupBox1";
      groupBox1->Size = System::Drawing::Size(310, 56);
      groupBox1->TabIndex = 41;
      groupBox1->TabStop = false;
      groupBox1->Text = L"Statistics";
      // 
      // _triangleCount
      // 
      this->_triangleCount->AutoSize = true;
      this->_triangleCount->Location = System::Drawing::Point(225, 28);
      this->_triangleCount->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      this->_triangleCount->Name = L"_triangleCount";
      this->_triangleCount->Size = System::Drawing::Size(13, 13);
      this->_triangleCount->TabIndex = 45;
      this->_triangleCount->Text = L"0";
      // 
      // _vertexCount
      // 
      this->_vertexCount->AutoSize = true;
      this->_vertexCount->Location = System::Drawing::Point(82, 28);
      this->_vertexCount->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      this->_vertexCount->Name = L"_vertexCount";
      this->_vertexCount->Size = System::Drawing::Size(13, 13);
      this->_vertexCount->TabIndex = 43;
      this->_vertexCount->Text = L"0";
      // 
      // label17
      // 
      label17->AutoSize = true;
      label17->Location = System::Drawing::Point(141, 28);
      label17->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label17->Name = L"label17";
      label17->Size = System::Drawing::Size(78, 13);
      label17->TabIndex = 44;
      label17->Text = L"Triangle count:";
      // 
      // label16
      // 
      label16->AutoSize = true;
      label16->Location = System::Drawing::Point(6, 28);
      label16->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label16->Name = L"label16";
      label16->Size = System::Drawing::Size(70, 13);
      label16->TabIndex = 42;
      label16->Text = L"Vertex count:";
      // 
      // label18
      // 
      label18->AutoSize = true;
      label18->Location = System::Drawing::Point(11, 127);
      label18->Margin = System::Windows::Forms::Padding(3, 12, 3, 0);
      label18->Name = L"label18";
      label18->Size = System::Drawing::Size(102, 13);
      label18->TabIndex = 22;
      label18->Text = L"Distance to road (m)";
      // 
      // _terrainGroup
      // 
      this->_terrainGroup->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->_terrainGroup->Controls->Add(this->_terrainDistanceBox);
      this->_terrainGroup->Controls->Add(label18);
      this->_terrainGroup->Controls->Add(label15);
      this->_terrainGroup->Controls->Add(this->_eastBox);
      this->_terrainGroup->Controls->Add(label14);
      this->_terrainGroup->Controls->Add(this->_westBox);
      this->_terrainGroup->Controls->Add(label13);
      this->_terrainGroup->Controls->Add(this->_southBox);
      this->_terrainGroup->Controls->Add(label12);
      this->_terrainGroup->Controls->Add(this->_northBox);
      this->_terrainGroup->Controls->Add(this->_gridBox);
      this->_terrainGroup->Controls->Add(label11);
      this->_terrainGroup->Controls->Add(this->_maxHeightBox);
      this->_terrainGroup->Controls->Add(this->_minHeightBox);
      this->_terrainGroup->Controls->Add(label9);
      this->_terrainGroup->Controls->Add(label10);
      this->_terrainGroup->Controls->Add(button2);
      this->_terrainGroup->Controls->Add(this->_heightPathBox);
      this->_terrainGroup->Controls->Add(label8);
      this->_terrainGroup->Enabled = false;
      this->_terrainGroup->Location = System::Drawing::Point(12, 205);
      this->_terrainGroup->Name = L"_terrainGroup";
      this->_terrainGroup->Padding = System::Windows::Forms::Padding(8);
      this->_terrainGroup->Size = System::Drawing::Size(310, 233);
      this->_terrainGroup->TabIndex = 12;
      this->_terrainGroup->TabStop = false;
      this->_terrainGroup->Text = L"Terrain options";
      // 
      // _terrainDistanceBox
      // 
      this->_terrainDistanceBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_terrainDistanceBox->DecimalPlaces = 2;
      this->_terrainDistanceBox->Location = System::Drawing::Point(225, 125);
      this->_terrainDistanceBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
      this->_terrainDistanceBox->Name = L"_terrainDistanceBox";
      this->_terrainDistanceBox->Size = System::Drawing::Size(74, 20);
      this->_terrainDistanceBox->TabIndex = 23;
      this->_terrainDistanceBox->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
      // 
      // _eastBox
      // 
      this->_eastBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_eastBox->DecimalPlaces = 2;
      this->_eastBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      this->_eastBox->Location = System::Drawing::Point(224, 176);
      this->_eastBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
      this->_eastBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
      this->_eastBox->Name = L"_eastBox";
      this->_eastBox->Size = System::Drawing::Size(74, 20);
      this->_eastBox->TabIndex = 31;
      // 
      // _westBox
      // 
      this->_westBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_westBox->DecimalPlaces = 2;
      this->_westBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      this->_westBox->Location = System::Drawing::Point(64, 176);
      this->_westBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
      this->_westBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
      this->_westBox->Name = L"_westBox";
      this->_westBox->Size = System::Drawing::Size(74, 20);
      this->_westBox->TabIndex = 29;
      // 
      // _southBox
      // 
      this->_southBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_southBox->DecimalPlaces = 2;
      this->_southBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      this->_southBox->Location = System::Drawing::Point(144, 202);
      this->_southBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
      this->_southBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
      this->_southBox->Name = L"_southBox";
      this->_southBox->Size = System::Drawing::Size(74, 20);
      this->_southBox->TabIndex = 27;
      // 
      // _northBox
      // 
      this->_northBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_northBox->DecimalPlaces = 2;
      this->_northBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      this->_northBox->Location = System::Drawing::Point(144, 150);
      this->_northBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
      this->_northBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
      this->_northBox->Name = L"_northBox";
      this->_northBox->Size = System::Drawing::Size(74, 20);
      this->_northBox->TabIndex = 25;
      // 
      // _gridBox
      // 
      this->_gridBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_gridBox->DecimalPlaces = 4;
      this->_gridBox->Location = System::Drawing::Point(225, 100);
      this->_gridBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
      this->_gridBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
      this->_gridBox->Name = L"_gridBox";
      this->_gridBox->Size = System::Drawing::Size(74, 20);
      this->_gridBox->TabIndex = 21;
      this->_gridBox->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      // 
      // _maxHeightBox
      // 
      this->_maxHeightBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_maxHeightBox->DecimalPlaces = 2;
      this->_maxHeightBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      this->_maxHeightBox->Location = System::Drawing::Point(225, 75);
      this->_maxHeightBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
      this->_maxHeightBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, System::Int32::MinValue});
      this->_maxHeightBox->Name = L"_maxHeightBox";
      this->_maxHeightBox->Size = System::Drawing::Size(74, 20);
      this->_maxHeightBox->TabIndex = 19;
      this->_maxHeightBox->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
      // 
      // _minHeightBox
      // 
      this->_minHeightBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_minHeightBox->DecimalPlaces = 2;
      this->_minHeightBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      this->_minHeightBox->Location = System::Drawing::Point(225, 50);
      this->_minHeightBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
      this->_minHeightBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, System::Int32::MinValue});
      this->_minHeightBox->Name = L"_minHeightBox";
      this->_minHeightBox->Size = System::Drawing::Size(74, 20);
      this->_minHeightBox->TabIndex = 17;
      // 
      // _heightPathBox
      // 
      this->_heightPathBox->Location = System::Drawing::Point(99, 24);
      this->_heightPathBox->Name = L"_heightPathBox";
      this->_heightPathBox->ReadOnly = true;
      this->_heightPathBox->Size = System::Drawing::Size(129, 20);
      this->_heightPathBox->TabIndex = 14;
      this->_heightPathBox->WordWrap = false;
      // 
      // _optionsGroup
      // 
      this->_optionsGroup->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->_optionsGroup->Controls->Add(this->_maxRatioBox);
      this->_optionsGroup->Controls->Add(this->_maxEdgeLengthBox);
      this->_optionsGroup->Controls->Add(this->_minEdgeLengthBox);
      this->_optionsGroup->Controls->Add(label5);
      this->_optionsGroup->Controls->Add(label4);
      this->_optionsGroup->Controls->Add(label3);
      this->_optionsGroup->Controls->Add(this->_maxSurfAngleBox);
      this->_optionsGroup->Controls->Add(label2);
      this->_optionsGroup->Controls->Add(label1);
      this->_optionsGroup->Controls->Add(this->_maxEdgeAngleBox);
      this->_optionsGroup->Location = System::Drawing::Point(12, 12);
      this->_optionsGroup->Name = L"_optionsGroup";
      this->_optionsGroup->Padding = System::Windows::Forms::Padding(8);
      this->_optionsGroup->Size = System::Drawing::Size(310, 163);
      this->_optionsGroup->TabIndex = 0;
      this->_optionsGroup->TabStop = false;
      this->_optionsGroup->Text = L"Build options";
      // 
      // _maxRatioBox
      // 
      this->_maxRatioBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_maxRatioBox->DecimalPlaces = 1;
      this->_maxRatioBox->Enabled = false;
      this->_maxRatioBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
      this->_maxRatioBox->Location = System::Drawing::Point(225, 132);
      this->_maxRatioBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
      this->_maxRatioBox->Name = L"_maxRatioBox";
      this->_maxRatioBox->Size = System::Drawing::Size(74, 20);
      this->_maxRatioBox->TabIndex = 10;
      this->_maxRatioBox->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {20, 0, 0, 0});
      // 
      // _maxEdgeLengthBox
      // 
      this->_maxEdgeLengthBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_maxEdgeLengthBox->DecimalPlaces = 2;
      this->_maxEdgeLengthBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
      this->_maxEdgeLengthBox->Location = System::Drawing::Point(225, 106);
      this->_maxEdgeLengthBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
      this->_maxEdgeLengthBox->Name = L"_maxEdgeLengthBox";
      this->_maxEdgeLengthBox->Size = System::Drawing::Size(74, 20);
      this->_maxEdgeLengthBox->TabIndex = 8;
      this->_maxEdgeLengthBox->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {10, 0, 0, 0});
      // 
      // _minEdgeLengthBox
      // 
      this->_minEdgeLengthBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_minEdgeLengthBox->DecimalPlaces = 2;
      this->_minEdgeLengthBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
      this->_minEdgeLengthBox->Location = System::Drawing::Point(225, 81);
      this->_minEdgeLengthBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
      this->_minEdgeLengthBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 131072});
      this->_minEdgeLengthBox->Name = L"_minEdgeLengthBox";
      this->_minEdgeLengthBox->Size = System::Drawing::Size(74, 20);
      this->_minEdgeLengthBox->TabIndex = 6;
      this->_minEdgeLengthBox->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 65536});
      // 
      // _maxSurfAngleBox
      // 
      this->_maxSurfAngleBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_maxSurfAngleBox->DecimalPlaces = 2;
      this->_maxSurfAngleBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 65536});
      this->_maxSurfAngleBox->Location = System::Drawing::Point(225, 56);
      this->_maxSurfAngleBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {90, 0, 0, 0});
      this->_maxSurfAngleBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
      this->_maxSurfAngleBox->Name = L"_maxSurfAngleBox";
      this->_maxSurfAngleBox->Size = System::Drawing::Size(74, 20);
      this->_maxSurfAngleBox->TabIndex = 4;
      this->_maxSurfAngleBox->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {5, 0, 0, 0});
      // 
      // _maxEdgeAngleBox
      // 
      this->_maxEdgeAngleBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
      this->_maxEdgeAngleBox->DecimalPlaces = 2;
      this->_maxEdgeAngleBox->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {25, 0, 0, 131072});
      this->_maxEdgeAngleBox->Location = System::Drawing::Point(225, 31);
      this->_maxEdgeAngleBox->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {90, 0, 0, 0});
      this->_maxEdgeAngleBox->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {25, 0, 0, 131072});
      this->_maxEdgeAngleBox->Name = L"_maxEdgeAngleBox";
      this->_maxEdgeAngleBox->Size = System::Drawing::Size(74, 20);
      this->_maxEdgeAngleBox->TabIndex = 2;
      this->_maxEdgeAngleBox->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
      // 
      // _outputGroup
      // 
      this->_outputGroup->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->_outputGroup->Controls->Add(button3);
      this->_outputGroup->Controls->Add(this->_pathBox);
      this->_outputGroup->Controls->Add(button1);
      this->_outputGroup->Controls->Add(this->_formatBox);
      this->_outputGroup->Controls->Add(label7);
      this->_outputGroup->Controls->Add(label6);
      this->_outputGroup->Location = System::Drawing::Point(12, 444);
      this->_outputGroup->Name = L"_outputGroup";
      this->_outputGroup->Size = System::Drawing::Size(310, 93);
      this->_outputGroup->TabIndex = 32;
      this->_outputGroup->TabStop = false;
      this->_outputGroup->Text = L"Output";
      // 
      // _pathBox
      // 
      this->_pathBox->Location = System::Drawing::Point(56, 50);
      this->_pathBox->Name = L"_pathBox";
      this->_pathBox->Size = System::Drawing::Size(172, 20);
      this->_pathBox->TabIndex = 37;
      this->_pathBox->WordWrap = false;
      this->_pathBox->TextChanged += gcnew System::EventHandler(this, &BuildForm::_pathBox_TextChanged);
      // 
      // _formatBox
      // 
      this->_formatBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->_formatBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
      this->_formatBox->FormattingEnabled = true;
      this->_formatBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"Wavefront OBJ"});
      this->_formatBox->Location = System::Drawing::Point(56, 25);
      this->_formatBox->Name = L"_formatBox";
      this->_formatBox->Size = System::Drawing::Size(135, 21);
      this->_formatBox->TabIndex = 34;
      // 
      // _progressBar
      // 
      this->_progressBar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->_progressBar->Location = System::Drawing::Point(83, 548);
      this->_progressBar->Margin = System::Windows::Forms::Padding(3, 8, 3, 8);
      this->_progressBar->Maximum = 1024;
      this->_progressBar->Name = L"_progressBar";
      this->_progressBar->Size = System::Drawing::Size(239, 23);
      this->_progressBar->Step = 1;
      this->_progressBar->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
      this->_progressBar->TabIndex = 40;
      // 
      // _buildBtn
      // 
      this->_buildBtn->Enabled = false;
      this->_buildBtn->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_buildBtn.Image")));
      this->_buildBtn->ImageAlign = System::Drawing::ContentAlignment::BottomLeft;
      this->_buildBtn->Location = System::Drawing::Point(12, 548);
      this->_buildBtn->Margin = System::Windows::Forms::Padding(8);
      this->_buildBtn->Name = L"_buildBtn";
      this->_buildBtn->Size = System::Drawing::Size(60, 23);
      this->_buildBtn->TabIndex = 39;
      this->_buildBtn->Text = L"GO! ";
      this->_buildBtn->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
      this->_buildBtn->UseVisualStyleBackColor = true;
      this->_buildBtn->Click += gcnew System::EventHandler(this, &BuildForm::Build);
      // 
      // _progressTimer
      // 
      this->_progressTimer->Interval = 200;
      this->_progressTimer->Tick += gcnew System::EventHandler(this, &BuildForm::UpdateProgressBar);
      // 
      // _terrainCheckbox
      // 
      this->_terrainCheckbox->AutoSize = true;
      this->_terrainCheckbox->Location = System::Drawing::Point(13, 182);
      this->_terrainCheckbox->Name = L"_terrainCheckbox";
      this->_terrainCheckbox->Size = System::Drawing::Size(91, 17);
      this->_terrainCheckbox->TabIndex = 11;
      this->_terrainCheckbox->Text = L"Enable terrain";
      this->_terrainCheckbox->UseVisualStyleBackColor = true;
      this->_terrainCheckbox->CheckedChanged += gcnew System::EventHandler(this, &BuildForm::ToggleTerrainGroup);
      // 
      // BuildForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(334, 650);
      this->Controls->Add(groupBox1);
      this->Controls->Add(this->_terrainCheckbox);
      this->Controls->Add(this->_terrainGroup);
      this->Controls->Add(this->_buildBtn);
      this->Controls->Add(this->_progressBar);
      this->Controls->Add(this->_outputGroup);
      this->Controls->Add(this->_optionsGroup);
      this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
      this->MaximizeBox = false;
      this->MaximumSize = System::Drawing::Size(342, 677);
      this->MinimizeBox = false;
      this->MinimumSize = System::Drawing::Size(342, 677);
      this->Name = L"BuildForm";
      this->ShowInTaskbar = false;
      this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
      this->Text = L"Build polygon mesh";
      this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &BuildForm::Closing);
      groupBox1->ResumeLayout(false);
      groupBox1->PerformLayout();
      this->_terrainGroup->ResumeLayout(false);
      this->_terrainGroup->PerformLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_terrainDistanceBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_eastBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_westBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_southBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_northBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_gridBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_maxHeightBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_minHeightBox))->EndInit();
      this->_optionsGroup->ResumeLayout(false);
      this->_optionsGroup->PerformLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_maxRatioBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_maxEdgeLengthBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_minEdgeLengthBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_maxSurfAngleBox))->EndInit();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_maxEdgeAngleBox))->EndInit();
      this->_outputGroup->ResumeLayout(false);
      this->_outputGroup->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

    void ValidateInputs( void )
    {
      // Check that output file name is valid...
      try {
        _buildBtn->Enabled =
          System::IO::Path::IsPathRooted(_pathBox->Text) &&
          System::IO::Directory::Exists(
            System::IO::Path::GetDirectoryName(_pathBox->Text) );
      }
      catch(...) {
        _buildBtn->Enabled = false;
      }

      // Check that terrain options are valid...
      if( _terrainCheckbox->Checked ) {
        _buildBtn->Enabled &=
          System::IO::Path::IsPathRooted(_heightPathBox->Text) &&
          System::IO::File::Exists(_heightPathBox->Text);
      }
    }

    Void BrowseOutput( Object ^sender, EventArgs ^e )
    {
      SaveFileDialog ^diag = gcnew SaveFileDialog();
      diag->AddExtension = true;
      switch( _formatBox->SelectedIndex ) {
      case 0:
      default:
        diag->Filter = "Wavefront OBJ (*.obj)|*.obj";
        diag->DefaultExt = "obj";
      }
      Windows::Forms::DialogResult rslt = diag->ShowDialog();
      if( rslt==Windows::Forms::DialogResult::OK )
        _pathBox->Text = diag->FileName;
      ValidateInputs();
    }

    Void BrowseHeightmap( Object ^sender, EventArgs ^e )
    {
      OpenFileDialog ^diag = gcnew OpenFileDialog();
      diag->Filter =
        "Image files (*.jpg, *.png, *.gif, *.bmp)|*.jpg;*.png;*.gif;*.bmp|"
        "All files (*.*)|*.*";
      diag->FilterIndex = 0;
      diag->CheckFileExists = true;
      Windows::Forms::DialogResult rslt = diag->ShowDialog();
      if( rslt==Windows::Forms::DialogResult::OK )
        _heightPathBox->Text = diag->FileName;
      ValidateInputs();
    }

    Void _pathBox_TextChanged( Object ^sender, EventArgs ^e )
    { ValidateInputs(); }

    Void Build( Object ^sender, EventArgs ^e );

    Void Abort( Object ^sender, EventArgs ^e );

    Void BuildThreadStart();

    Void BuildEnd();

    Void UpdateProgressBar( Object ^sender, EventArgs ^e );

    Void Closing( Object ^sender, FormClosingEventArgs ^e );

    Void ToggleTerrainGroup( Object ^sender, EventArgs ^e )
    {
      _terrainGroup->Enabled = _terrainCheckbox->Checked;
      ValidateInputs();
    }

  public:
    asphalt::Mesh* GetMesh( void )
    { asphalt::Mesh* res=_mesh; _mesh=0; return res; }

  }; // public ref class BuildForm
}  // namespace roads
