//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file MainForm.h
//! \brief roads::MainForm class definition.

#pragma once

#include "DomainLogic.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Globalization;

namespace roads {

  ref class ToolPolicy;

	/// <summary>
	/// Summary for MainForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	private ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void);

    //! X-coordinate of point hovered in 2D view.
    property double GLPanelX {
      double get( void ) { return _glHoverX; }
      void set( double v )
      { _glHoverX=v; _glPanelXBox->Text=String::Format("X={0:F2}",v); }
    }

    //! Y coordinate of point hovered in 2D view.
    property double GLPanelY {
      double get( void ) { return _glHoverY; }
      void set( double v )
      { _glHoverY=v; _glPanelYBox->Text=String::Format("Y={0:F2}",v); }
    }

    //! Number of meters per pixel in 2D view.
    property double GLPanelMeterPerPixel {
      double get( void )
      { return _glSpreadX / _glPanel->Width; }
    }

    //! Refresh 2D view.
    void GLPanelRefresh( void )
    { GLPanelPaint(this,EventArgs::Empty); }


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm();


  private:
    //------------------------------------------------------------------------
    //! @name UI components
    //@{
    SplitContainer          ^_splitContainer;

    Control                 ^_glPanel;

    TreeView                ^_treeView;

    ToolStripContainer      ^_toolStripContainer;

    StatusStrip             ^_statusStrip;
    ToolStripStatusLabel    ^_glPanelXBox;
    ToolStripStatusLabel    ^_glPanelYBox;

    ToolStripButton         ^_saveToolStripButton;
    ToolStripButton         ^_buildToolStripButton;

    ToolStrip               ^_editToolStrip;
    ToolStripButton         ^_selectToolStripButton;
    ToolStripButton         ^_sketchToolStripButton;
    ToolStripButton         ^_throughPointsToolStripButton;
    ToolStripButton         ^_prolongateToolStripButton;
    ToolStripButton         ^_joinToolStripButton;
    ToolStripButton         ^_splitToolStripButton;

    ToolStripMenuItem       ^_mergeToolStripMenuItem;
    ToolStripMenuItem       ^_saveToolStripMenuItem;
    ToolStripMenuItem       ^_saveAsToolStripMenuItem;
    ToolStripMenuItem       ^_viewToolStripMenuItem;

    ToolTip                 ^_toolTip;

    DataGridView              ^_dataGridView;
    DataGridViewTextBoxColumn ^_propertyColumn;
    DataGridViewTextBoxColumn ^_valueColumn;
    NumberFormatInfo          ^_doubleFormatInfo;

    Forms::ContextMenuStrip ^_treeContextMenu;
    Forms::ContextMenuStrip ^_treeItemContextMenu;
    Forms::ContextMenuStrip ^_junctionContextMenu;
    Forms::ContextMenuStrip ^_geometryContextMenu;
    Forms::ContextMenuStrip ^_lanesContextMenu;
    Forms::ContextMenuStrip ^_laneSectionContextMenu;
    Forms::ContextMenuStrip ^_laneContextMenu;
    Forms::ContextMenuStrip ^_lateralProfileContextMenu;
    Forms::ContextMenuStrip ^_elevationProfileContextMenu;
    Forms::ContextMenuStrip ^_poly3ContextMenu;
    //@}

    //------------------------------------------------------------------------
    //! @name OpenGL viewport-related members
    //@{
    ::HDC   _glHDC;             //!< Device context handle
    ::HGLRC _glHGLRC;           //!< OpenGL context handle
    double  _glCenterX,         //!< X-ccord at the center of the OpenGL view
            _glCenterY,         //!< Y-ccord at the center of the OpenGL view
            _glSpreadX,         //!< X-coord span viewable in the OpenGL view
            _glSpreadY,         //!< Y-coord span viewable in the OpenGL view
            _glHoverX,          //!< X-coord of hovered point in OpenGL view
            _glHoverY;          //!< Y-coord of hovered point in OpenGL view
    GLuint  _glListID,          //!< Display list handle for roads
            _glSelListID;       //!< Display list handle for selection
    bool    _glListNdUpdate,    //!< True if display list is out of date
            _glSelListNdUpdate; //!< True if selection list is out of date
    //@}

    //------------------------------------------------------------------------
    //! @name Document-related members
    //@{
    ::asphalt::Mesh *_mesh;
    //@}
    
    //! Policies associated with current tool.
    ToolPolicy ^_currentToolPolicy;

    //------------------------------------------------------------------------
    //! EventHandler adapter.
    //! Convert an EventHandler to a compatible handler of a chosen type.
    template<typename TArgs, typename THandler=EventHandler<TArgs> >
    ref class Adapter {
    public:
      Adapter(EventHandler ^adapted): _adapted(adapted) {}

      operator THandler^ ()
      { return gcnew THandler(this,&Adapter::Handler); }

    private:
      EventHandler ^_adapted;

      void Handler( Object ^sender, TArgs e )
      { _adapted(sender,e); }
    };

    //------------------------------------------------------------------------

    //! Called when the user demands a new document to be created.
    void newOdrDocument( Object ^sender, EventArgs ^e );

    //! Called when the user demands an existing document to be opened.
    void loadOdrDocument( Object ^sender, EventArgs ^e );

    //! Called when the user demands a document to be merged with the current.
    void mergeOdrDocument( Object ^sender, EventArgs ^e );

    //! Called when the user demands for the document to be saved.
    void saveOdrDocument( Object ^sender, EventArgs ^e );

    //! Called when the user demands for the document to be saved
    //! with a different name.
    void saveOdrDocumentAs( Object ^sender, EventArgs ^e );

    //! Check if the OpenDRIVE document was modified since last save.
    //! If a modification was made, propose the user to save the document,
    //! discard modifications or cancel action.
    //! @return true if the operation is to proceed further
    Boolean ConfirmClose();

    //! Enable/disable UI elements according to status.
    void UpdateUI( Object ^sender, EventArgs ^args );

    //! Called when closing the form.
    void onClose( Object ^sender, FormClosingEventArgs ^e );

    //! Called to quit the application
    void Quit( Object ^sender, EventArgs ^e );

    //------------------------------------------------------------------------
    //! @name OpenGL viewport related methods
    //@{
    //! Perform OpenGL view initialization.
    //! @return true on success.
    bool GLPanelInit();

    //! Resize OpenGL view according to parent panel's size.
    void GLPanelResize( Object ^sender, EventArgs ^e );

    //! Perform OpenGL rendering.
    void GLPanelPaint( Object ^sender, EventArgs ^e );

    //! Reset pan and zoom in 2D view.
    void GLPanelReset( Object ^sender, EventArgs ^e );

    //! Manage mouse down events.
    void GLPanelMouseDown( Object ^sender, MouseEventArgs ^e );

    //! Manage mouse move events.
    void GLPanelMouseMove( Object ^sender, MouseEventArgs ^e);

    //! Manage mouse up events.
    void GLPanelMouseUp( Object ^sender, MouseEventArgs ^e );

    //! Manage mouse wheel events.
    void GLPanelMouseWheel( Object ^sender, MouseEventArgs ^e );

    //! Invalidate display list used for road network and refresh OpenGL view.
    void GLPanelDocumentModified( Object ^sender, DomainLogicEventArgs ^args );

    //! Invalidate display list used for selection and refresh OpenGL view.
    void GLPanelSelectionChanged( Object ^sender, DomainLogicEventArgs ^args );

    //! Occurs when the user presses a key while focus is in the GL view.
    void GLPanelKeyDown( Object ^sender, PreviewKeyDownEventArgs ^e );

    //@}

    //------------------------------------------------------------------------
    //! @name Tree view related methods
    //@{

    //! Initialize treeview.
    //! @return true on success.
    bool TreeviewInit();

    //! Populate tree view with items corresponding to data object model.
    void TreeviewPopulate( Object ^sender, EventArgs ^args );

    //! Create and populate treeview node for junction.
    void TreeviewCreateJunctionNode( const odr::Junction* junction );

    //! Create and populate treeview node for road.
    void TreeviewCreateRoadNode( const odr::Road* road );

    //! Create treeview node for geometric primitive.
    void TreeviewCreatePrimitiveNode( const odr::GeometricPrimitive* prim );

    //! Create and populate treeview node for lane section.
    void TreeviewCreateLaneSectionNode( const odr::LaneSection* section );

    //! Create and populate treeview node for lane.
    void TreeviewCreateLaneNode( const odr::Lane* lane );
    
    //! Create treeview node for lane width.
    void TreeviewCreateLaneWidthNode( const odr::LaneWidth* width );

    //! Create treeview node for crossfall.
    void TreeviewCreateCrossfallNode( const odr::Crossfall* cross );

    //! Create treeview node for superelevation.
    void TreeviewCreateSuperelevationNode( const odr::Superelevation* super );

    //! Create treeview node for elevation.
    void TreeviewCreateElevationNode( const odr::Elevation* elev );

    //! Delete tree-view item corresponding to deleted data node.
    void TreeviewNodeDeleted( Object ^sender, DomainLogicEventArgs ^args );

    //! Create tree-view item corresponding to created data node.
    void TreeviewNodeCreated( Object ^sender, DomainLogicEventArgs ^args );

    //! Update tree-view item corresponding to modified data node.
    void TreeviewNodeModified( Object ^sender, DomainLogicEventArgs ^args );

    //! Manage key release event.
    void TreeviewKeyUp( Object ^sender, KeyEventArgs ^e );

    //! Manage UI-side selection change event.
    void TreeviewAfterSelect( Object ^sender, TreeViewEventArgs ^e );

    //! Manage domain logic-side selection change event.
    void TreeviewSelectionChanged( Object ^sender, DomainLogicEventArgs ^e );

    //! Ensure the right treeview node is selected before popping up
    //! a context menu.
    void TreeviewNodeClick( Object ^sender, TreeNodeMouseClickEventArgs ^e );

    //! misc::WeakRef<const misc::WeakRefServer> encapsulated in a managed
    //! class.
    ref class ManagedRef {
    public:
      ManagedRef( const misc::WeakRefServer* ptr ) {
        _ptr = new misc::WeakRef<const misc::WeakRefServer>( ptr );
      }
      ~ManagedRef() {
        delete _ptr;
        _ptr = 0;
      }
      !ManagedRef() {
        delete _ptr;
      }
      property const misc::WeakRefServer* Pointer {
        const misc::WeakRefServer* get( void ) { return _ptr->ptr(); }
        void set( const misc::WeakRefServer *ptr ) { *_ptr = ptr; }
      }
      virtual String^ ToString( void ) override {
        return IntPtr((void*)_ptr->ptr()).ToString();
      }

    private:
      misc::WeakRef<const misc::WeakRefServer> *_ptr;
    };

    //! Extract data node from Treeview node.
    template<typename T> const T* TreeviewNode2Data( TreeNode^ node ) {
      if(!node) return 0;
      return dynamic_cast<const T*>(
        dynamic_cast<ManagedRef^>(node->Tag)->Pointer );
    }

    //! Find treeview node associated with data node.
    TreeNode^ TreeviewFindNode( const misc::WeakRefServer* dataNode );

    //@}

    //------------------------------------------------------------------------
    //! @name Context menu callbacks
    //@{

    void ContextDelete( Object ^sender, EventArgs ^e );
    void ContextAssistedPolyEdit( Object ^sender, EventArgs ^args );
    void ContextAddRoad( Object ^sender, EventArgs ^e );
    void ContextAddJunction( Object ^sender, EventArgs ^e );
    void ContextAddLine( Object ^sender, EventArgs ^e );
    void ContextAddArc( Object ^sender, EventArgs ^e );
    void ContextAddSpiral( Object ^sender, EventArgs ^e );
    void ContextAddPoly3( Object ^sender, EventArgs ^e );
    void ContextAddSection( Object ^sender, EventArgs ^e );
    void ContextAddLane( Object ^sender, EventArgs ^e );
    void ContextAddWidth( Object ^sender, EventArgs ^e );
    void ContextAddCrossfall( Object ^sender, EventArgs ^e );
    void ContextAddSuperelevation( Object ^sender, EventArgs ^e );
    void ContextAddElevation( Object ^sender, EventArgs ^e );

    //@}

    //------------------------------------------------------------------------
    //! @name Node properties editor related methods
    //@{

    //! Initialize grid view.
    //! @return true on success.
    bool GridViewInit();

    //! Add/remove rows in grid view so as to reach transmitted row number.
    void GridViewResize( int numRows );

    //! Set up row in grid view.
    //! @param[in] rowIndex Index of row to set up
    //! @param[in] name     Name of field
    //! @param[in] value    Object wrapping field value
    //! @param[in] cb       Callback for modifying field, nullptr if read-only
    void GridViewUpdateRow(
      int rowIndex,
      String ^name,
      Object ^value,
      DomainLogic::NodeEditDelegate ^cb );

    //! Fill data grid view depending on selected tree view item.
    void GridViewUpdate( Object ^sender, EventArgs ^e );

    //! Manage cell modification event.
    void GridViewEdited( Object ^sender, DataGridViewCellEventArgs ^e );

    //! Manage data error.
    void GridViewError( Object ^sender, DataGridViewDataErrorEventArgs ^e );

    //@}

    void _aboutToolStripMenuItem_Click( Object ^sender, EventArgs ^e );

    void Build( Object ^sender, EventArgs ^e );

    void OnToolChange( Object ^sender, EventArgs ^e );

    //------------------------------------------------------------------------
    //! @name Backdrop related members
    //@{

    ref class Backdrop {
    public:
      Backdrop( String ^path );
      ~Backdrop();

      property bool Visible {
        bool get( void ) { return _visible; }
        void set( bool b ) { _visible=b; }
      }
      property double Left {
        double get( void ) { return _left; }
        void set( double d ) { _left=d; }
      }
      property double Right {
        double get( void ) { return _right; }
        void set( double d ) { _right=d; }
      }
      property double Bottom {
        double get( void ) { return _bottom; }
        void set( double d ) { _bottom=d; }
      }
      property double Top {
        double get( void ) { return _top; }
        void set( double d ) { _top=d; }
      }
      property GLint TextureID {
        GLint get( void ) { return _textureID; }
      }

    private:
      bool    _visible;
      double  _left, _right, _bottom, _top;
      GLuint  _textureID;
    };

    Generic::List<Backdrop^> ^_backdrops;

    //! Add backdrop...
    void BackdropAdd( Object ^sender, EventArgs ^args );

    //! Remove backdrop attached to sender...
    void BackdropRemove( Object ^sender, EventArgs ^args );

    //! Toggle visibility of backdrop attached to sender...
    void BackdropToggle( Object ^sender, EventArgs ^args );

    //@}

    //------------------------------------------------------------------------
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
      System::Windows::Forms::ToolStripSeparator^  _toolStripSeparator1;
      System::Windows::Forms::ToolStripSeparator^  _toolStripSeparator2;
      System::Windows::Forms::ToolStripSeparator^  _toolStripSeparator3;
      System::Windows::Forms::ToolStripMenuItem^  _fileToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  _newToolStripMenuItem;
      System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
      System::Windows::Forms::ToolStripMenuItem^  _loadToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  _quitToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  placeBackdropToolStripMenuItem;
      System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
      System::Windows::Forms::ToolStripMenuItem^  _helpToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  _aboutToolStripMenuItem;
      System::Windows::Forms::ToolStripButton^  _newToolStripButton;
      System::Windows::Forms::ToolStripButton^  _loadToolStripButton;
      System::Windows::Forms::MenuStrip^  _menuStrip;
      System::Windows::Forms::ToolStrip^  _fileToolStrip;
      System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
      System::Windows::Forms::ToolStripMenuItem^  addNewRoadToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addNewJunctionToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  deleteItemToolStripMenuItem;
      System::Windows::Forms::SplitContainer^  _splitContainer2;
      System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
      System::Windows::Forms::ToolStripMenuItem^  addLineToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addArcToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addSpiralToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addPoly3ToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addLaneToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addSectionToolStripMenuItem1;
      System::Windows::Forms::ToolStripMenuItem^  deleteSectionToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addRoadInJunctionToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  deleteJunctionToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addCrossfallToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addSuperelevationToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addElevationToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  addWidthToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  deleteLaneToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  assistedEditToolStripMenuItem;
      System::Windows::Forms::ToolStripMenuItem^  deleteNodeToolStripMenuItem;
      System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
      this->_mergeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->_saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->_saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->_viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->_saveToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->_buildToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->_treeView = (gcnew System::Windows::Forms::TreeView());
      this->_treeContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->_dataGridView = (gcnew System::Windows::Forms::DataGridView());
      this->_propertyColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
      this->_valueColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
      this->_editToolStrip = (gcnew System::Windows::Forms::ToolStrip());
      this->_selectToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->_sketchToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->_prolongateToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->_joinToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->_splitToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
      this->_splitContainer = (gcnew System::Windows::Forms::SplitContainer());
      this->_glPanel = (gcnew System::Windows::Forms::Control());
      this->_toolStripContainer = (gcnew System::Windows::Forms::ToolStripContainer());
      this->_statusStrip = (gcnew System::Windows::Forms::StatusStrip());
      this->_glPanelXBox = (gcnew System::Windows::Forms::ToolStripStatusLabel());
      this->_glPanelYBox = (gcnew System::Windows::Forms::ToolStripStatusLabel());
      this->_treeItemContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->_geometryContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->_laneSectionContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->_lanesContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->_junctionContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->_lateralProfileContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->_elevationProfileContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->_laneContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->_toolTip = (gcnew System::Windows::Forms::ToolTip(this->components));
      this->_poly3ContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->_throughPointsToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
      _toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
      _toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
      _toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
      _fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      _newToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      _loadToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      _quitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      placeBackdropToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
      _helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      _aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      _newToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
      _loadToolStripButton = (gcnew System::Windows::Forms::ToolStripButton());
      _menuStrip = (gcnew System::Windows::Forms::MenuStrip());
      _fileToolStrip = (gcnew System::Windows::Forms::ToolStrip());
      toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
      addNewRoadToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addNewJunctionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      deleteItemToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      _splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
      addLineToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addArcToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addSpiralToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addPoly3ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addLaneToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addSectionToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
      deleteSectionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addRoadInJunctionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      deleteJunctionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addCrossfallToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addSuperelevationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addElevationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      addWidthToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      deleteLaneToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      assistedEditToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      deleteNodeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
      toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
      _menuStrip->SuspendLayout();
      _fileToolStrip->SuspendLayout();
      _splitContainer2->Panel1->SuspendLayout();
      _splitContainer2->Panel2->SuspendLayout();
      _splitContainer2->SuspendLayout();
      this->_treeContextMenu->SuspendLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_dataGridView))->BeginInit();
      this->_editToolStrip->SuspendLayout();
      this->_splitContainer->Panel1->SuspendLayout();
      this->_splitContainer->Panel2->SuspendLayout();
      this->_splitContainer->SuspendLayout();
      this->_toolStripContainer->ContentPanel->SuspendLayout();
      this->_toolStripContainer->TopToolStripPanel->SuspendLayout();
      this->_toolStripContainer->SuspendLayout();
      this->_statusStrip->SuspendLayout();
      this->_treeItemContextMenu->SuspendLayout();
      this->_geometryContextMenu->SuspendLayout();
      this->_laneSectionContextMenu->SuspendLayout();
      this->_lanesContextMenu->SuspendLayout();
      this->_junctionContextMenu->SuspendLayout();
      this->_lateralProfileContextMenu->SuspendLayout();
      this->_elevationProfileContextMenu->SuspendLayout();
      this->_laneContextMenu->SuspendLayout();
      this->_poly3ContextMenu->SuspendLayout();
      this->SuspendLayout();
      // 
      // _toolStripSeparator1
      // 
      _toolStripSeparator1->Name = L"_toolStripSeparator1";
      _toolStripSeparator1->Size = System::Drawing::Size(149, 6);
      // 
      // _toolStripSeparator2
      // 
      _toolStripSeparator2->Name = L"_toolStripSeparator2";
      _toolStripSeparator2->Size = System::Drawing::Size(149, 6);
      // 
      // _toolStripSeparator3
      // 
      _toolStripSeparator3->Name = L"_toolStripSeparator3";
      _toolStripSeparator3->Size = System::Drawing::Size(149, 6);
      // 
      // _fileToolStripMenuItem
      // 
      _fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(9) {_newToolStripMenuItem, 
        _toolStripSeparator1, _loadToolStripMenuItem, this->_mergeToolStripMenuItem, _toolStripSeparator2, this->_saveToolStripMenuItem, 
        this->_saveAsToolStripMenuItem, _toolStripSeparator3, _quitToolStripMenuItem});
      _fileToolStripMenuItem->Name = L"_fileToolStripMenuItem";
      _fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
      _fileToolStripMenuItem->Text = L"File";
      // 
      // _newToolStripMenuItem
      // 
      _newToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_newToolStripMenuItem.Image")));
      _newToolStripMenuItem->Name = L"_newToolStripMenuItem";
      _newToolStripMenuItem->ShortcutKeyDisplayString = L"";
      _newToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::N));
      _newToolStripMenuItem->Size = System::Drawing::Size(152, 22);
      _newToolStripMenuItem->Text = L"New...";
      _newToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::newOdrDocument);
      // 
      // _loadToolStripMenuItem
      // 
      _loadToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_loadToolStripMenuItem.Image")));
      _loadToolStripMenuItem->Name = L"_loadToolStripMenuItem";
      _loadToolStripMenuItem->ShortcutKeyDisplayString = L"";
      _loadToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
      _loadToolStripMenuItem->Size = System::Drawing::Size(152, 22);
      _loadToolStripMenuItem->Text = L"Open...";
      _loadToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::loadOdrDocument);
      // 
      // _mergeToolStripMenuItem
      // 
      this->_mergeToolStripMenuItem->Enabled = false;
      this->_mergeToolStripMenuItem->Name = L"_mergeToolStripMenuItem";
      this->_mergeToolStripMenuItem->Size = System::Drawing::Size(152, 22);
      this->_mergeToolStripMenuItem->Text = L"Merge...";
      this->_mergeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::mergeOdrDocument);
      // 
      // _saveToolStripMenuItem
      // 
      this->_saveToolStripMenuItem->Enabled = false;
      this->_saveToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_saveToolStripMenuItem.Image")));
      this->_saveToolStripMenuItem->Name = L"_saveToolStripMenuItem";
      this->_saveToolStripMenuItem->ShortcutKeyDisplayString = L"";
      this->_saveToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::S));
      this->_saveToolStripMenuItem->Size = System::Drawing::Size(152, 22);
      this->_saveToolStripMenuItem->Text = L"Save";
      this->_saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveOdrDocument);
      // 
      // _saveAsToolStripMenuItem
      // 
      this->_saveAsToolStripMenuItem->Enabled = false;
      this->_saveAsToolStripMenuItem->Name = L"_saveAsToolStripMenuItem";
      this->_saveAsToolStripMenuItem->Size = System::Drawing::Size(152, 22);
      this->_saveAsToolStripMenuItem->Text = L"Save as...";
      this->_saveAsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveOdrDocumentAs);
      // 
      // _quitToolStripMenuItem
      // 
      _quitToolStripMenuItem->Name = L"_quitToolStripMenuItem";
      _quitToolStripMenuItem->Size = System::Drawing::Size(152, 22);
      _quitToolStripMenuItem->Text = L"Quit";
      _quitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::Quit);
      // 
      // placeBackdropToolStripMenuItem
      // 
      placeBackdropToolStripMenuItem->Name = L"placeBackdropToolStripMenuItem";
      placeBackdropToolStripMenuItem->Size = System::Drawing::Size(158, 22);
      placeBackdropToolStripMenuItem->Text = L"Place backdrop...";
      placeBackdropToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::BackdropAdd);
      // 
      // toolStripMenuItem1
      // 
      toolStripMenuItem1->Name = L"toolStripMenuItem1";
      toolStripMenuItem1->Size = System::Drawing::Size(155, 6);
      // 
      // _helpToolStripMenuItem
      // 
      _helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {_aboutToolStripMenuItem});
      _helpToolStripMenuItem->Name = L"_helpToolStripMenuItem";
      _helpToolStripMenuItem->Size = System::Drawing::Size(40, 20);
      _helpToolStripMenuItem->Text = L"Help";
      // 
      // _aboutToolStripMenuItem
      // 
      _aboutToolStripMenuItem->Name = L"_aboutToolStripMenuItem";
      _aboutToolStripMenuItem->Size = System::Drawing::Size(115, 22);
      _aboutToolStripMenuItem->Text = L"About...";
      _aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::_aboutToolStripMenuItem_Click);
      // 
      // _newToolStripButton
      // 
      _newToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      _newToolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_newToolStripButton.Image")));
      _newToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      _newToolStripButton->Name = L"_newToolStripButton";
      _newToolStripButton->Size = System::Drawing::Size(23, 22);
      _newToolStripButton->Text = L"New";
      _newToolStripButton->Click += gcnew System::EventHandler(this, &MainForm::newOdrDocument);
      // 
      // _loadToolStripButton
      // 
      _loadToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      _loadToolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_loadToolStripButton.Image")));
      _loadToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      _loadToolStripButton->Name = L"_loadToolStripButton";
      _loadToolStripButton->Size = System::Drawing::Size(23, 22);
      _loadToolStripButton->Text = L"Load...";
      _loadToolStripButton->Click += gcnew System::EventHandler(this, &MainForm::loadOdrDocument);
      // 
      // _menuStrip
      // 
      _menuStrip->Dock = System::Windows::Forms::DockStyle::None;
      _menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {_fileToolStripMenuItem, this->_viewToolStripMenuItem, 
        _helpToolStripMenuItem});
      _menuStrip->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::HorizontalStackWithOverflow;
      _menuStrip->Location = System::Drawing::Point(0, 0);
      _menuStrip->Name = L"_menuStrip";
      _menuStrip->Size = System::Drawing::Size(632, 24);
      _menuStrip->TabIndex = 0;
      _menuStrip->Text = L"_menuStrip";
      // 
      // _viewToolStripMenuItem
      // 
      this->_viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {placeBackdropToolStripMenuItem, 
        toolStripMenuItem1});
      this->_viewToolStripMenuItem->Name = L"_viewToolStripMenuItem";
      this->_viewToolStripMenuItem->Size = System::Drawing::Size(41, 20);
      this->_viewToolStripMenuItem->Text = L"View";
      // 
      // _fileToolStrip
      // 
      _fileToolStrip->Dock = System::Windows::Forms::DockStyle::None;
      _fileToolStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {_newToolStripButton, _loadToolStripButton, 
        this->_saveToolStripButton, toolStripSeparator1, this->_buildToolStripButton});
      _fileToolStrip->Location = System::Drawing::Point(3, 24);
      _fileToolStrip->Name = L"_fileToolStrip";
      _fileToolStrip->Size = System::Drawing::Size(108, 25);
      _fileToolStrip->TabIndex = 1;
      _fileToolStrip->Text = L"File";
      // 
      // _saveToolStripButton
      // 
      this->_saveToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->_saveToolStripButton->Enabled = false;
      this->_saveToolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_saveToolStripButton.Image")));
      this->_saveToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->_saveToolStripButton->Name = L"_saveToolStripButton";
      this->_saveToolStripButton->Size = System::Drawing::Size(23, 22);
      this->_saveToolStripButton->Text = L"Save";
      this->_saveToolStripButton->Click += gcnew System::EventHandler(this, &MainForm::saveOdrDocument);
      // 
      // toolStripSeparator1
      // 
      toolStripSeparator1->Name = L"toolStripSeparator1";
      toolStripSeparator1->Size = System::Drawing::Size(6, 25);
      // 
      // _buildToolStripButton
      // 
      this->_buildToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->_buildToolStripButton->Enabled = false;
      this->_buildToolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_buildToolStripButton.Image")));
      this->_buildToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->_buildToolStripButton->Name = L"_buildToolStripButton";
      this->_buildToolStripButton->Size = System::Drawing::Size(23, 22);
      this->_buildToolStripButton->Text = L"Build...";
      this->_buildToolStripButton->Click += gcnew System::EventHandler(this, &MainForm::Build);
      // 
      // addNewRoadToolStripMenuItem
      // 
      addNewRoadToolStripMenuItem->Name = L"addNewRoadToolStripMenuItem";
      addNewRoadToolStripMenuItem->Size = System::Drawing::Size(134, 22);
      addNewRoadToolStripMenuItem->Text = L"Add road";
      addNewRoadToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddRoad);
      // 
      // addNewJunctionToolStripMenuItem
      // 
      addNewJunctionToolStripMenuItem->Name = L"addNewJunctionToolStripMenuItem";
      addNewJunctionToolStripMenuItem->Size = System::Drawing::Size(134, 22);
      addNewJunctionToolStripMenuItem->Text = L"Add junction";
      addNewJunctionToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddJunction);
      // 
      // deleteItemToolStripMenuItem
      // 
      deleteItemToolStripMenuItem->Name = L"deleteItemToolStripMenuItem";
      deleteItemToolStripMenuItem->Size = System::Drawing::Size(105, 22);
      deleteItemToolStripMenuItem->Text = L"Delete";
      deleteItemToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextDelete);
      // 
      // _splitContainer2
      // 
      _splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
      _splitContainer2->Location = System::Drawing::Point(0, 0);
      _splitContainer2->Margin = System::Windows::Forms::Padding(0);
      _splitContainer2->Name = L"_splitContainer2";
      _splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
      // 
      // _splitContainer2.Panel1
      // 
      _splitContainer2->Panel1->Controls->Add(this->_treeView);
      // 
      // _splitContainer2.Panel2
      // 
      _splitContainer2->Panel2->AutoScroll = true;
      _splitContainer2->Panel2->Controls->Add(this->_dataGridView);
      _splitContainer2->Size = System::Drawing::Size(145, 481);
      _splitContainer2->SplitterDistance = 306;
      _splitContainer2->TabIndex = 0;
      // 
      // _treeView
      // 
      this->_treeView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->_treeView->BorderStyle = System::Windows::Forms::BorderStyle::None;
      this->_treeView->ContextMenuStrip = this->_treeContextMenu;
      this->_treeView->Enabled = false;
      this->_treeView->HideSelection = false;
      this->_treeView->Location = System::Drawing::Point(0, 0);
      this->_treeView->Margin = System::Windows::Forms::Padding(0);
      this->_treeView->Name = L"_treeView";
      this->_treeView->Size = System::Drawing::Size(145, 307);
      this->_treeView->TabIndex = 0;
      this->_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &MainForm::TreeviewAfterSelect);
      this->_treeView->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::TreeviewKeyUp);
      this->_treeView->NodeMouseClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &MainForm::TreeviewNodeClick);
      // 
      // _treeContextMenu
      // 
      this->_treeContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {addNewRoadToolStripMenuItem, 
        addNewJunctionToolStripMenuItem});
      this->_treeContextMenu->Name = L"_treeContextMenu";
      this->_treeContextMenu->Size = System::Drawing::Size(135, 48);
      this->_treeContextMenu->Text = L"Document";
      // 
      // _dataGridView
      // 
      this->_dataGridView->AllowUserToAddRows = false;
      this->_dataGridView->AllowUserToDeleteRows = false;
      this->_dataGridView->AllowUserToResizeRows = false;
      this->_dataGridView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->_dataGridView->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
      this->_dataGridView->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::AllCells;
      this->_dataGridView->BorderStyle = System::Windows::Forms::BorderStyle::None;
      this->_dataGridView->ClipboardCopyMode = System::Windows::Forms::DataGridViewClipboardCopyMode::EnableWithoutHeaderText;
      this->_dataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
      this->_dataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->_propertyColumn, 
        this->_valueColumn});
      this->_dataGridView->EditMode = System::Windows::Forms::DataGridViewEditMode::EditOnEnter;
      this->_dataGridView->Location = System::Drawing::Point(0, 0);
      this->_dataGridView->Margin = System::Windows::Forms::Padding(0);
      this->_dataGridView->MultiSelect = false;
      this->_dataGridView->Name = L"_dataGridView";
      this->_dataGridView->RowHeadersVisible = false;
      this->_dataGridView->RowTemplate->Resizable = System::Windows::Forms::DataGridViewTriState::False;
      this->_dataGridView->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::CellSelect;
      this->_dataGridView->Size = System::Drawing::Size(145, 172);
      this->_dataGridView->TabIndex = 0;
      this->_dataGridView->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::GridViewEdited);
      this->_dataGridView->DataError += gcnew System::Windows::Forms::DataGridViewDataErrorEventHandler(this, &MainForm::GridViewError);
      // 
      // _propertyColumn
      // 
      this->_propertyColumn->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
      dataGridViewCellStyle2->BackColor = System::Drawing::Color::Silver;
      this->_propertyColumn->DefaultCellStyle = dataGridViewCellStyle2;
      this->_propertyColumn->HeaderText = L"Property";
      this->_propertyColumn->Name = L"_propertyColumn";
      this->_propertyColumn->ReadOnly = true;
      // 
      // _valueColumn
      // 
      this->_valueColumn->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
      this->_valueColumn->HeaderText = L"Value";
      this->_valueColumn->Name = L"_valueColumn";
      // 
      // addLineToolStripMenuItem
      // 
      addLineToolStripMenuItem->Name = L"addLineToolStripMenuItem";
      addLineToolStripMenuItem->Size = System::Drawing::Size(122, 22);
      addLineToolStripMenuItem->Text = L"Add line";
      addLineToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddLine);
      // 
      // addArcToolStripMenuItem
      // 
      addArcToolStripMenuItem->Name = L"addArcToolStripMenuItem";
      addArcToolStripMenuItem->Size = System::Drawing::Size(122, 22);
      addArcToolStripMenuItem->Text = L"Add arc";
      addArcToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddArc);
      // 
      // addSpiralToolStripMenuItem
      // 
      addSpiralToolStripMenuItem->Name = L"addSpiralToolStripMenuItem";
      addSpiralToolStripMenuItem->Size = System::Drawing::Size(122, 22);
      addSpiralToolStripMenuItem->Text = L"Add spiral";
      addSpiralToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddSpiral);
      // 
      // addPoly3ToolStripMenuItem
      // 
      addPoly3ToolStripMenuItem->Name = L"addPoly3ToolStripMenuItem";
      addPoly3ToolStripMenuItem->Size = System::Drawing::Size(122, 22);
      addPoly3ToolStripMenuItem->Text = L"Add poly3";
      addPoly3ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddPoly3);
      // 
      // addLaneToolStripMenuItem
      // 
      addLaneToolStripMenuItem->Name = L"addLaneToolStripMenuItem";
      addLaneToolStripMenuItem->Size = System::Drawing::Size(142, 22);
      addLaneToolStripMenuItem->Text = L"Add lane...";
      addLaneToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddLane);
      // 
      // addSectionToolStripMenuItem1
      // 
      addSectionToolStripMenuItem1->Name = L"addSectionToolStripMenuItem1";
      addSectionToolStripMenuItem1->Size = System::Drawing::Size(130, 22);
      addSectionToolStripMenuItem1->Text = L"Add section";
      addSectionToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MainForm::ContextAddSection);
      // 
      // deleteSectionToolStripMenuItem
      // 
      deleteSectionToolStripMenuItem->Name = L"deleteSectionToolStripMenuItem";
      deleteSectionToolStripMenuItem->Size = System::Drawing::Size(142, 22);
      deleteSectionToolStripMenuItem->Text = L"Delete section";
      deleteSectionToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextDelete);
      // 
      // addRoadInJunctionToolStripMenuItem
      // 
      addRoadInJunctionToolStripMenuItem->Name = L"addRoadInJunctionToolStripMenuItem";
      addRoadInJunctionToolStripMenuItem->Size = System::Drawing::Size(170, 22);
      addRoadInJunctionToolStripMenuItem->Text = L"Add road in junction";
      addRoadInJunctionToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddRoad);
      // 
      // deleteJunctionToolStripMenuItem
      // 
      deleteJunctionToolStripMenuItem->Name = L"deleteJunctionToolStripMenuItem";
      deleteJunctionToolStripMenuItem->Size = System::Drawing::Size(170, 22);
      deleteJunctionToolStripMenuItem->Text = L"Delete junction";
      deleteJunctionToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextDelete);
      // 
      // addCrossfallToolStripMenuItem
      // 
      addCrossfallToolStripMenuItem->Name = L"addCrossfallToolStripMenuItem";
      addCrossfallToolStripMenuItem->Size = System::Drawing::Size(167, 22);
      addCrossfallToolStripMenuItem->Text = L"Add crossfall";
      addCrossfallToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddCrossfall);
      // 
      // addSuperelevationToolStripMenuItem
      // 
      addSuperelevationToolStripMenuItem->Name = L"addSuperelevationToolStripMenuItem";
      addSuperelevationToolStripMenuItem->Size = System::Drawing::Size(167, 22);
      addSuperelevationToolStripMenuItem->Text = L"Add superelevation";
      addSuperelevationToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddSuperelevation);
      // 
      // addElevationToolStripMenuItem
      // 
      addElevationToolStripMenuItem->Name = L"addElevationToolStripMenuItem";
      addElevationToolStripMenuItem->Size = System::Drawing::Size(140, 22);
      addElevationToolStripMenuItem->Text = L"Add elevation";
      addElevationToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddElevation);
      // 
      // addWidthToolStripMenuItem
      // 
      addWidthToolStripMenuItem->Name = L"addWidthToolStripMenuItem";
      addWidthToolStripMenuItem->Size = System::Drawing::Size(128, 22);
      addWidthToolStripMenuItem->Text = L"Add width";
      addWidthToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAddWidth);
      // 
      // deleteLaneToolStripMenuItem
      // 
      deleteLaneToolStripMenuItem->Name = L"deleteLaneToolStripMenuItem";
      deleteLaneToolStripMenuItem->Size = System::Drawing::Size(128, 22);
      deleteLaneToolStripMenuItem->Text = L"Delete lane";
      deleteLaneToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextDelete);
      // 
      // assistedEditToolStripMenuItem
      // 
      assistedEditToolStripMenuItem->Name = L"assistedEditToolStripMenuItem";
      assistedEditToolStripMenuItem->Size = System::Drawing::Size(147, 22);
      assistedEditToolStripMenuItem->Text = L"Assisted edit...";
      assistedEditToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextAssistedPolyEdit);
      // 
      // deleteNodeToolStripMenuItem
      // 
      deleteNodeToolStripMenuItem->Name = L"deleteNodeToolStripMenuItem";
      deleteNodeToolStripMenuItem->Size = System::Drawing::Size(147, 22);
      deleteNodeToolStripMenuItem->Text = L"Delete node";
      deleteNodeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::ContextDelete);
      // 
      // _editToolStrip
      // 
      this->_editToolStrip->Dock = System::Windows::Forms::DockStyle::None;
      this->_editToolStrip->Enabled = false;
      this->_editToolStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->_selectToolStripButton, 
        toolStripSeparator2, this->_sketchToolStripButton, this->_throughPointsToolStripButton, this->_prolongateToolStripButton, this->_joinToolStripButton, 
        this->_splitToolStripButton});
      this->_editToolStrip->Location = System::Drawing::Point(111, 24);
      this->_editToolStrip->Name = L"_editToolStrip";
      this->_editToolStrip->Size = System::Drawing::Size(185, 25);
      this->_editToolStrip->TabIndex = 2;
      // 
      // _selectToolStripButton
      // 
      this->_selectToolStripButton->Checked = true;
      this->_selectToolStripButton->CheckState = System::Windows::Forms::CheckState::Checked;
      this->_selectToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->_selectToolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_selectToolStripButton.Image")));
      this->_selectToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->_selectToolStripButton->Name = L"_selectToolStripButton";
      this->_selectToolStripButton->Size = System::Drawing::Size(23, 22);
      this->_selectToolStripButton->Text = L"Select tool";
      this->_selectToolStripButton->ToolTipText = L"Click in 2D view to select corresponding nodes in data object model.";
      this->_selectToolStripButton->Click += gcnew System::EventHandler(this, &MainForm::OnToolChange);
      // 
      // _sketchToolStripButton
      // 
      this->_sketchToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->_sketchToolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_sketchToolStripButton.Image")));
      this->_sketchToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->_sketchToolStripButton->Name = L"_sketchToolStripButton";
      this->_sketchToolStripButton->Size = System::Drawing::Size(23, 22);
      this->_sketchToolStripButton->Text = L"Sketch roads tool";
      this->_sketchToolStripButton->ToolTipText = L"Click in 2D view and drag mouse to draw new roads.";
      this->_sketchToolStripButton->Click += gcnew System::EventHandler(this, &MainForm::OnToolChange);
      // 
      // _prolongateToolStripButton
      // 
      this->_prolongateToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->_prolongateToolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_prolongateToolStripButton.Image")));
      this->_prolongateToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->_prolongateToolStripButton->Name = L"_prolongateToolStripButton";
      this->_prolongateToolStripButton->Size = System::Drawing::Size(23, 22);
      this->_prolongateToolStripButton->Text = L"Prolongate road";
      this->_prolongateToolStripButton->ToolTipText = L"Prolongate selected road up to clicked point\n[Shift+LMB] for constant curvature\n[" 
        L"Control+LMB] for 0 curvature at end";
      this->_prolongateToolStripButton->Click += gcnew System::EventHandler(this, &MainForm::OnToolChange);
      // 
      // _joinToolStripButton
      // 
      this->_joinToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->_joinToolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_joinToolStripButton.Image")));
      this->_joinToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->_joinToolStripButton->Name = L"_joinToolStripButton";
      this->_joinToolStripButton->Size = System::Drawing::Size(23, 22);
      this->_joinToolStripButton->Text = L"Junction tool";
      this->_joinToolStripButton->ToolTipText = L"Click on road ends in 2D view to create junctions.";
      this->_joinToolStripButton->Click += gcnew System::EventHandler(this, &MainForm::OnToolChange);
      // 
      // _splitToolStripButton
      // 
      this->_splitToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->_splitToolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_splitToolStripButton.Image")));
      this->_splitToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->_splitToolStripButton->Name = L"_splitToolStripButton";
      this->_splitToolStripButton->Size = System::Drawing::Size(23, 22);
      this->_splitToolStripButton->Text = L"Split tool";
      this->_splitToolStripButton->ToolTipText = L"Click along a road to cut it in two";
      this->_splitToolStripButton->Click += gcnew System::EventHandler(this, &MainForm::OnToolChange);
      // 
      // _splitContainer
      // 
      this->_splitContainer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->_splitContainer->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
      this->_splitContainer->Location = System::Drawing::Point(0, 0);
      this->_splitContainer->Margin = System::Windows::Forms::Padding(0);
      this->_splitContainer->Name = L"_splitContainer";
      // 
      // _splitContainer.Panel1
      // 
      this->_splitContainer->Panel1->Controls->Add(_splitContainer2);
      // 
      // _splitContainer.Panel2
      // 
      this->_splitContainer->Panel2->Controls->Add(this->_glPanel);
      this->_splitContainer->Size = System::Drawing::Size(632, 485);
      this->_splitContainer->SplitterDistance = 149;
      this->_splitContainer->TabIndex = 4;
      // 
      // _glPanel
      // 
      this->_glPanel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      this->_glPanel->Location = System::Drawing::Point(0, 0);
      this->_glPanel->Margin = System::Windows::Forms::Padding(0);
      this->_glPanel->Name = L"_glPanel";
      this->_glPanel->Size = System::Drawing::Size(475, 481);
      this->_glPanel->TabIndex = 0;
      this->_glPanel->Text = L"GLPanel";
      this->_glPanel->MarginChanged += gcnew System::EventHandler(this, &MainForm::GLPanelResize);
      this->_glPanel->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &MainForm::GLPanelKeyDown);
      this->_glPanel->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::GLPanelMouseMove);
      this->_glPanel->Move += gcnew System::EventHandler(this, &MainForm::GLPanelResize);
      this->_glPanel->PaddingChanged += gcnew System::EventHandler(this, &MainForm::GLPanelResize);
      this->_glPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::GLPanelMouseDown);
      this->_glPanel->Resize += gcnew System::EventHandler(this, &MainForm::GLPanelResize);
      this->_glPanel->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::GLPanelMouseUp);
      // 
      // _toolStripContainer
      // 
      this->_toolStripContainer->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
        | System::Windows::Forms::AnchorStyles::Left) 
        | System::Windows::Forms::AnchorStyles::Right));
      // 
      // _toolStripContainer.ContentPanel
      // 
      this->_toolStripContainer->ContentPanel->Controls->Add(this->_splitContainer);
      this->_toolStripContainer->ContentPanel->Margin = System::Windows::Forms::Padding(0);
      this->_toolStripContainer->ContentPanel->Size = System::Drawing::Size(632, 486);
      this->_toolStripContainer->Location = System::Drawing::Point(0, 0);
      this->_toolStripContainer->Margin = System::Windows::Forms::Padding(0);
      this->_toolStripContainer->Name = L"_toolStripContainer";
      this->_toolStripContainer->Size = System::Drawing::Size(632, 535);
      this->_toolStripContainer->TabIndex = 5;
      this->_toolStripContainer->Text = L"_toolStripContainer";
      // 
      // _toolStripContainer.TopToolStripPanel
      // 
      this->_toolStripContainer->TopToolStripPanel->Controls->Add(_menuStrip);
      this->_toolStripContainer->TopToolStripPanel->Controls->Add(_fileToolStrip);
      this->_toolStripContainer->TopToolStripPanel->Controls->Add(this->_editToolStrip);
      // 
      // _statusStrip
      // 
      this->_statusStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->_glPanelXBox, this->_glPanelYBox});
      this->_statusStrip->Location = System::Drawing::Point(0, 535);
      this->_statusStrip->Name = L"_statusStrip";
      this->_statusStrip->RenderMode = System::Windows::Forms::ToolStripRenderMode::ManagerRenderMode;
      this->_statusStrip->Size = System::Drawing::Size(632, 22);
      this->_statusStrip->TabIndex = 3;
      this->_statusStrip->Text = L"statusStrip1";
      // 
      // _glPanelXBox
      // 
      this->_glPanelXBox->AutoSize = false;
      this->_glPanelXBox->BorderSides = static_cast<System::Windows::Forms::ToolStripStatusLabelBorderSides>((((System::Windows::Forms::ToolStripStatusLabelBorderSides::Left | System::Windows::Forms::ToolStripStatusLabelBorderSides::Top) 
        | System::Windows::Forms::ToolStripStatusLabelBorderSides::Right) 
        | System::Windows::Forms::ToolStripStatusLabelBorderSides::Bottom));
      this->_glPanelXBox->BorderStyle = System::Windows::Forms::Border3DStyle::SunkenOuter;
      this->_glPanelXBox->Name = L"_glPanelXBox";
      this->_glPanelXBox->Size = System::Drawing::Size(96, 17);
      this->_glPanelXBox->Text = L"X=0.00";
      // 
      // _glPanelYBox
      // 
      this->_glPanelYBox->AutoSize = false;
      this->_glPanelYBox->BorderSides = static_cast<System::Windows::Forms::ToolStripStatusLabelBorderSides>((((System::Windows::Forms::ToolStripStatusLabelBorderSides::Left | System::Windows::Forms::ToolStripStatusLabelBorderSides::Top) 
        | System::Windows::Forms::ToolStripStatusLabelBorderSides::Right) 
        | System::Windows::Forms::ToolStripStatusLabelBorderSides::Bottom));
      this->_glPanelYBox->BorderStyle = System::Windows::Forms::Border3DStyle::SunkenOuter;
      this->_glPanelYBox->Name = L"_glPanelYBox";
      this->_glPanelYBox->Size = System::Drawing::Size(96, 17);
      this->_glPanelYBox->Text = L"Y=0.00";
      // 
      // _treeItemContextMenu
      // 
      this->_treeItemContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {deleteItemToolStripMenuItem});
      this->_treeItemContextMenu->Name = L"_treeItemContextMenu";
      this->_treeItemContextMenu->Size = System::Drawing::Size(106, 26);
      this->_treeItemContextMenu->Text = L"Node";
      // 
      // _geometryContextMenu
      // 
      this->_geometryContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {addLineToolStripMenuItem, 
        addArcToolStripMenuItem, addSpiralToolStripMenuItem, addPoly3ToolStripMenuItem});
      this->_geometryContextMenu->Name = L"_geometryContextMenu";
      this->_geometryContextMenu->Size = System::Drawing::Size(123, 92);
      this->_geometryContextMenu->Text = L"Geometry";
      // 
      // _laneSectionContextMenu
      // 
      this->_laneSectionContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {addLaneToolStripMenuItem, 
        deleteSectionToolStripMenuItem});
      this->_laneSectionContextMenu->Name = L"_laneSectionContextMenu";
      this->_laneSectionContextMenu->Size = System::Drawing::Size(143, 48);
      this->_laneSectionContextMenu->Text = L"Lane section";
      // 
      // _lanesContextMenu
      // 
      this->_lanesContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {addSectionToolStripMenuItem1});
      this->_lanesContextMenu->Name = L"_lanesContextMenu";
      this->_lanesContextMenu->Size = System::Drawing::Size(131, 26);
      this->_lanesContextMenu->Text = L"Lanes";
      // 
      // _junctionContextMenu
      // 
      this->_junctionContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {addRoadInJunctionToolStripMenuItem, 
        deleteJunctionToolStripMenuItem});
      this->_junctionContextMenu->Name = L"_junctionContextMenu";
      this->_junctionContextMenu->Size = System::Drawing::Size(171, 48);
      this->_junctionContextMenu->Text = L"Junction";
      // 
      // _lateralProfileContextMenu
      // 
      this->_lateralProfileContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {addCrossfallToolStripMenuItem, 
        addSuperelevationToolStripMenuItem});
      this->_lateralProfileContextMenu->Name = L"_lateralProfileContextMenu";
      this->_lateralProfileContextMenu->Size = System::Drawing::Size(168, 48);
      this->_lateralProfileContextMenu->Text = L"Lateral profile";
      // 
      // _elevationProfileContextMenu
      // 
      this->_elevationProfileContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {addElevationToolStripMenuItem});
      this->_elevationProfileContextMenu->Name = L"_elevationProfileContextMenu";
      this->_elevationProfileContextMenu->Size = System::Drawing::Size(141, 26);
      this->_elevationProfileContextMenu->Text = L"Elevation profile";
      // 
      // _laneContextMenu
      // 
      this->_laneContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {addWidthToolStripMenuItem, 
        deleteLaneToolStripMenuItem});
      this->_laneContextMenu->Name = L"_laneContextMenu";
      this->_laneContextMenu->Size = System::Drawing::Size(129, 48);
      this->_laneContextMenu->Text = L"Lane";
      // 
      // _poly3ContextMenu
      // 
      this->_poly3ContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {assistedEditToolStripMenuItem, 
        deleteNodeToolStripMenuItem});
      this->_poly3ContextMenu->Name = L"_laneWidthContextMenu";
      this->_poly3ContextMenu->Size = System::Drawing::Size(148, 48);
      // 
      // _throughPointsToolStripButton
      // 
      this->_throughPointsToolStripButton->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
      this->_throughPointsToolStripButton->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"_throughPointsToolStripButton.Image")));
      this->_throughPointsToolStripButton->ImageTransparentColor = System::Drawing::Color::Magenta;
      this->_throughPointsToolStripButton->Name = L"_throughPointsToolStripButton";
      this->_throughPointsToolStripButton->Size = System::Drawing::Size(23, 22);
      this->_throughPointsToolStripButton->Text = L"Road through points tool";
      this->_throughPointsToolStripButton->ToolTipText = L"Create road passing through clicked points\n[Shift+LMB] to force point to be aligned with predecessors\n[Control+LMB] when finished";
      this->_throughPointsToolStripButton->Click += gcnew System::EventHandler(this, &MainForm::OnToolChange);
      // 
      // toolStripSeparator2
      // 
      toolStripSeparator2->Name = L"toolStripSeparator2";
      toolStripSeparator2->Size = System::Drawing::Size(6, 25);
      // 
      // MainForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(632, 557);
      this->Controls->Add(this->_statusStrip);
      this->Controls->Add(this->_toolStripContainer);
      this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
      this->MainMenuStrip = _menuStrip;
      this->Name = L"MainForm";
      this->Text = L"roads";
      this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
      this->Activated += gcnew System::EventHandler(this, &MainForm::GLPanelResize);
      this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::onClose);
      _menuStrip->ResumeLayout(false);
      _menuStrip->PerformLayout();
      _fileToolStrip->ResumeLayout(false);
      _fileToolStrip->PerformLayout();
      _splitContainer2->Panel1->ResumeLayout(false);
      _splitContainer2->Panel2->ResumeLayout(false);
      _splitContainer2->ResumeLayout(false);
      this->_treeContextMenu->ResumeLayout(false);
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->_dataGridView))->EndInit();
      this->_editToolStrip->ResumeLayout(false);
      this->_editToolStrip->PerformLayout();
      this->_splitContainer->Panel1->ResumeLayout(false);
      this->_splitContainer->Panel2->ResumeLayout(false);
      this->_splitContainer->ResumeLayout(false);
      this->_toolStripContainer->ContentPanel->ResumeLayout(false);
      this->_toolStripContainer->TopToolStripPanel->ResumeLayout(false);
      this->_toolStripContainer->TopToolStripPanel->PerformLayout();
      this->_toolStripContainer->ResumeLayout(false);
      this->_toolStripContainer->PerformLayout();
      this->_statusStrip->ResumeLayout(false);
      this->_statusStrip->PerformLayout();
      this->_treeItemContextMenu->ResumeLayout(false);
      this->_geometryContextMenu->ResumeLayout(false);
      this->_laneSectionContextMenu->ResumeLayout(false);
      this->_lanesContextMenu->ResumeLayout(false);
      this->_junctionContextMenu->ResumeLayout(false);
      this->_lateralProfileContextMenu->ResumeLayout(false);
      this->_elevationProfileContextMenu->ResumeLayout(false);
      this->_laneContextMenu->ResumeLayout(false);
      this->_poly3ContextMenu->ResumeLayout(false);
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion

  }; // public ref class MainForm

} // namespace roads
