//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file MainForm.cpp
//! \brief Implementation of roads::MainForm's methods.

#include "stdafx.h"
#include "MainForm.h"
#include "AboutForm.h"
#include "BuildForm.h"
#include "SelectToolPolicy.h"
#include "SketchToolPolicy.h"
#include "ThroughPointsToolPolicy.h"
#include "ProlongateToolPolicy.h"
#include "JoinToolPolicy.h"
#include "SplitToolPolicy.h"
#include "BackdropOptionsForm.h"

namespace roads {

  MainForm::MainForm()
	{
		InitializeComponent();
    _mesh = 0;

    GLPanelInit();
    TreeviewInit();
    GridViewInit();

    _backdrops = gcnew Generic::List<Backdrop^>();

    DomainLogic::Instance->DocumentSaved += Adapter<DomainLogicEventArgs^>(
      gcnew EventHandler(this,&MainForm::UpdateUI) );

    DomainLogic::Instance->DocumentModified += Adapter<DomainLogicEventArgs^>(
      gcnew EventHandler(this,&MainForm::UpdateUI) );

    _currentToolPolicy = SelectToolPolicy::Instance;
    _selectToolStripButton->Tag         = SelectToolPolicy::Instance;
    _sketchToolStripButton->Tag         = SketchToolPolicy::Instance;
    _throughPointsToolStripButton->Tag  = ThroughPointsToolPolicy::Instance;
    _prolongateToolStripButton->Tag     = ProlongateToolPolicy::Instance;
    _joinToolStripButton->Tag           = JoinToolPolicy::Instance;
    _splitToolStripButton->Tag          = SplitToolPolicy::Instance;
	}

  //--------------------------------------------------------------------------
  MainForm::~MainForm()
	{
		delete components;
    delete _mesh;
    delete _backdrops;
	}

  //--------------------------------------------------------------------------
  void MainForm::OnToolChange( Object ^sender, EventArgs ^e )
  {
    for each( ToolStripItem ^item in _editToolStrip->Items )
      if( ToolStripButton ^btn = dynamic_cast<ToolStripButton^>(item) )
        btn->Checked = false;

    ToolStripButton ^btn = dynamic_cast<ToolStripButton^>(sender);
    btn->Checked = true;

    _currentToolPolicy->OnAbort(this);
    _currentToolPolicy = dynamic_cast<ToolPolicy^>(btn->Tag);
    _glPanel->Cursor = _currentToolPolicy->Cursor;
  }

  //--------------------------------------------------------------------------
  void MainForm::newOdrDocument( Object ^sender, EventArgs ^e )
  {
    // Check for modifications...
    if( !ConfirmClose() )
      return;

    /// \todo Display creation options...

    // Create document...
    DomainLogic::Instance->NewDocument();
    DomainLogic::Instance->FileName = "";

    // Reset view...
    /// \todo Reset view in GL panel-related source file.
    _glCenterX = 0.0;
    _glCenterY = 0.0;
    _glSpreadX = 100.0;
    _glSpreadY = _glSpreadX * (double)_glPanel->Height
                  / (double)_glPanel->Width;
    GLPanelPaint(sender,e);
  }

  //--------------------------------------------------------------------------
  void MainForm::loadOdrDocument( Object ^sender, EventArgs ^e )
  {
    // Check for modifications...
    if( !ConfirmClose() )
      return;

    // Display dialog...
    OpenFileDialog ^diag = gcnew OpenFileDialog();
    diag->Title = "Open file";
    diag->CheckFileExists = true;
    diag->Filter = "OpenDRIVE file (*.xodr)|*.xodr";
    System::Windows::Forms::DialogResult rslt = diag->ShowDialog();
    if( rslt!=System::Windows::Forms::DialogResult::OK )
      return;

    // Load document...
    DomainLogic::Instance->OpenDocument( diag->FileName );
  }

  //--------------------------------------------------------------------------
  void MainForm::mergeOdrDocument( Object ^sender, EventArgs ^e )
  {
    // Display dialog...
    OpenFileDialog ^diag = gcnew OpenFileDialog();
    diag->Title = "Merge file";
    diag->CheckFileExists = true;
    diag->Filter = "OpenDRIVE file (*.xodr)|*.xodr";
    Windows::Forms::DialogResult rslt = diag->ShowDialog();
    if( rslt!=Windows::Forms::DialogResult::OK )
      return;

    // Merge document...
    DomainLogic::Instance->MergeDocument( diag->FileName );
  }

  //--------------------------------------------------------------------------
  void MainForm::saveOdrDocument( Object ^sender, EventArgs ^e )
  {
    if( !DomainLogic::Instance->Document )
      return;

    if( DomainLogic::Instance->FileName=="" ) {
      saveOdrDocumentAs(sender,e);
      return;
    }

    /// \todo Increase version number

    if( !DomainLogic::Instance->SaveDocument() )
      MessageBox::Show( "Failed to save file \""
                        + DomainLogic::Instance->FileName+"\".",
                        "Error",
                        Windows::Forms::MessageBoxButtons::OK,
                        Windows::Forms::MessageBoxIcon::Error );
  }

  //--------------------------------------------------------------------------
  void MainForm::saveOdrDocumentAs( Object ^sender, EventArgs ^e )
  {
    // Display dialog...
    FileDialog ^diag = gcnew SaveFileDialog();
    diag->AddExtension = true;
    diag->Filter = "OpenDRIVE file (*.xodr)|*.xodr";
    diag->DefaultExt = "xodr";
    Windows::Forms::DialogResult rslt = diag->ShowDialog();
    if( rslt!=Windows::Forms::DialogResult::OK ) {
      return;
    }

    // Save document...
    DomainLogic::Instance->FileName = diag->FileName;
    saveOdrDocument(sender,e);
  }

  //--------------------------------------------------------------------------
  Boolean MainForm::ConfirmClose()
  {
    if( !DomainLogic::Instance->Document
        || !DomainLogic::Instance->Document->modified() )
      return true;

    Windows::Forms::DialogResult rslt = MessageBox::Show(
      "Current project was modified since last save.\n"
      "Do you wish to save your work, discard modifications or cancel operation?",
      "Warning",
      MessageBoxButtons::YesNoCancel, MessageBoxIcon::Asterisk );

    switch( rslt ) {
    case System::Windows::Forms::DialogResult::Yes:
      saveOdrDocument(nullptr,nullptr);
    case System::Windows::Forms::DialogResult::No:
      return true;
    default:
      return false;
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::onClose( Object ^sender, FormClosingEventArgs ^e )
  {
    if( !ConfirmClose() )
      e->Cancel = true;
  }

  //--------------------------------------------------------------------------
  void MainForm::Quit( Object ^sender, EventArgs ^e )
  {
    Close();
  }

  //--------------------------------------------------------------------------
  void MainForm::UpdateUI( Object ^sender, EventArgs ^args )
  {
    bool haveOdrDoc = (DomainLogic::Instance->Document!=0);
    _saveToolStripMenuItem->Enabled   = haveOdrDoc;
    _saveAsToolStripMenuItem->Enabled = haveOdrDoc;
    _mergeToolStripMenuItem->Enabled  = haveOdrDoc;
    _saveToolStripButton->Enabled     = haveOdrDoc;
    _buildToolStripButton->Enabled    = haveOdrDoc;
    _treeView->Enabled                = haveOdrDoc;
    _editToolStrip->Enabled           = haveOdrDoc;

    if( haveOdrDoc ) {
      Text =
        gcnew String("roads - ") +
        gcnew String(
          DomainLogic::Instance->Document->header()->name()!=std::string("") ?
          DomainLogic::Instance->Document->header()->name() : "untitled" );

      if( DomainLogic::Instance->FileName!="" )
        Text = Text + " (" + DomainLogic::Instance->FileName + ")";

      if( DomainLogic::Instance->Document->modified() )
        Text = Text + "*";
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::_aboutToolStripMenuItem_Click( Object ^sender, EventArgs ^e )
  {
    Enabled = false;
    (gcnew AboutForm())->ShowDialog();
    Enabled = true;
  }

  //--------------------------------------------------------------------------
  void MainForm::Build( Object ^sender, EventArgs ^e )
  {
    Enabled = false;
    delete _mesh;
    _mesh=0;
    BuildForm ^bf = gcnew BuildForm(DomainLogic::Instance->Document);
    bf->ShowDialog();
    _mesh = bf->GetMesh();
    delete bf;
    Enabled = true;
  }

  //--------------------------------------------------------------------------
  void MainForm::BackdropAdd( Object ^sender, EventArgs ^args )
  {
    OpenFileDialog ^dlg = gcnew OpenFileDialog();
    dlg->CheckFileExists = true;
    dlg->Multiselect = false;
    dlg->Title = "Choose image file";
    dlg->Filter = "Image files (*.jpg,*.jpeg,*.png,*.bmp,*.gif)|"
                  "*.jpg;*.jpeg;*.png;*.bmp;*.gif";
    Forms::DialogResult rslt = dlg->ShowDialog();
    if( rslt!=Forms::DialogResult::OK )
      return;

    Cursor = Forms::Cursors::WaitCursor;
    Backdrop ^backdrop=nullptr;
    try {
      backdrop = gcnew Backdrop(dlg->FileName);
    }
    catch( Exception^ ) {
      delete backdrop;
      Cursor = Forms::Cursors::Default;
      return;
    }
    Cursor = Forms::Cursors::Default;

    BackdropOptionsForm ^bof = gcnew BackdropOptionsForm();
    if( const odr::Document* doc = DomainLogic::Instance->Document ) {
      const odr::Header *header = doc->header();
      bof->West  = header->west();
      bof->East  = header->east();
      bof->South = header->south();
      bof->North = header->north();
    }
    rslt = bof->ShowDialog();
    if( rslt!=Forms::DialogResult::OK ) {
      delete bof;
      delete backdrop;
      return;
    }
    backdrop->Left    = bof->West;
    backdrop->Right   = bof->East;
    backdrop->Top     = bof->North;
    backdrop->Bottom  = bof->South;
    backdrop->Visible = true;
    delete bof;

    _backdrops->Add( backdrop );

    ToolStripMenuItem ^item = gcnew ToolStripMenuItem( dlg->FileName );
    item->Tag = backdrop;

    ToolStripMenuItem ^visibleItem = gcnew ToolStripMenuItem(
      "Visible", nullptr,
      gcnew EventHandler(this,&MainForm::BackdropToggle) );
    visibleItem->CheckOnClick = true;
    visibleItem->Checked = true;
    item->DropDownItems->Add( visibleItem );

    ToolStripMenuItem ^removeItem = gcnew ToolStripMenuItem(
      "Remove", nullptr,
      gcnew EventHandler(this,&MainForm::BackdropRemove) );
    item->DropDownItems->Add( removeItem );

    _viewToolStripMenuItem->DropDownItems->Add( item );

    Invalidate();
  }

  //--------------------------------------------------------------------------
  void MainForm::BackdropRemove( Object ^sender, EventArgs ^args )
  {
    ToolStripItem ^item = ((ToolStripMenuItem^)sender)->OwnerItem;
    Backdrop ^backdrop = (Backdrop^)item->Tag;
    _backdrops->Remove(backdrop);
    delete backdrop;
    _viewToolStripMenuItem->DropDownItems->Remove(item);
    Invalidate();
  }

  //--------------------------------------------------------------------------
  void MainForm::BackdropToggle( Object ^sender, EventArgs ^args )
  {
    ToolStripItem ^item = ((ToolStripMenuItem^)sender)->OwnerItem;
    Backdrop ^backdrop = (Backdrop^)item->Tag;
    backdrop->Visible = !backdrop->Visible;
    Invalidate();
  }

  //--------------------------------------------------------------------------
  MainForm::Backdrop::Backdrop( String ^path ) :
  _textureID( 0 )
  {
    Bitmap ^bitmap = gcnew Bitmap( path );

    unsigned char *data =
      new unsigned char[ bitmap->Width*bitmap->Height*4 ];
    for( int j=0, offset=0 ; j<bitmap->Height ; ++j ) {
      for( int i=0 ; i<bitmap->Width ; ++i ) {
        Color c = bitmap->GetPixel(i,j);
        data[offset++] = c.R;
        data[offset++] = c.G;
        data[offset++] = c.B;
        data[offset++] = c.A;
      }
    }

    GLuint texID=0;
    glGenTextures( 1, &texID );
    _textureID = texID;

    glBindTexture( GL_TEXTURE_2D, texID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, bitmap->Width, bitmap->Height, 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, data );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glBindTexture( GL_TEXTURE_2D, 0 );
    delete[] data;
  }

  MainForm::Backdrop::~Backdrop()
  {
    if( _textureID ) {
      GLuint texID = _textureID;
      glDeleteTextures( 1, &texID );
    }
    _textureID = 0;
  }

} // namespace roads
