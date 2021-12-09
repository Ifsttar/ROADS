//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file MainForm_Treeview.cpp
//! \brief Implementation of roads::MainForm's treeview-related methods.


#include "stdafx.h"
#include "MainForm.h"
#include "NewLaneForm.h"
#include "Poly3HelperForm.h"

namespace roads {

  bool MainForm::TreeviewInit()
  {
    // Bind event handlers...
    DomainLogic::Instance->DocumentCreated += Adapter<DomainLogicEventArgs^>(
      gcnew EventHandler(this,&MainForm::TreeviewPopulate) );

    DomainLogic::Instance->DocumentLoaded += Adapter<DomainLogicEventArgs^>(
      gcnew EventHandler(this,&MainForm::TreeviewPopulate) );

    DomainLogic::Instance->DocumentClosed += Adapter<DomainLogicEventArgs^>(
      gcnew EventHandler(this,&MainForm::TreeviewPopulate) );

    DomainLogic::Instance->NodeDeleted +=
      gcnew DomainLogicEventHandler( this, &MainForm::TreeviewNodeDeleted );

    DomainLogic::Instance->NodeCreated +=
      gcnew DomainLogicEventHandler( this, &MainForm::TreeviewNodeCreated );

    DomainLogic::Instance->NodeModified +=
      gcnew DomainLogicEventHandler( this, &MainForm::TreeviewNodeModified );

    DomainLogic::Instance->SelectionChanged +=
      gcnew DomainLogicEventHandler( this, &MainForm::TreeviewSelectionChanged );

    return true;
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewPopulate( Object ^sender, EventArgs ^args )
  {
    _treeView->Nodes->Clear();
    if( !DomainLogic::Instance->Document ) return;

    misc::Enumerator<const odr::Element*> *e =
      DomainLogic::Instance->Document->enumerate();
    if( !e ) return;
    while( const odr::Element *el=e->item() ) {
      e->moveNext();

      if( const odr::Road *road = dynamic_cast<const odr::Road*>(el) ) {
        if( road->junction() ) continue;
        TreeviewCreateRoadNode(road);
      }

      else if( const odr::Junction *junc = dynamic_cast<const odr::Junction*>(el) ) {
        TreeviewCreateJunctionNode(junc);
      }
    }
    delete e;
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewCreateJunctionNode( const odr::Junction* junction )
  {
    TreeNode ^junctionNode =
      _treeView->Nodes->Add( gcnew String("Junction ") + junction->ID() );
    junctionNode->Tag = gcnew ManagedRef(junction);
    junctionNode->Name = junctionNode->Tag->ToString();
    junctionNode->ContextMenuStrip = _junctionContextMenu;

    misc::Enumerator<const odr::Road*> *roads = junction->enumerate();
    while( const odr::Road *road = roads->item() ) {
      roads->moveNext();
      TreeviewCreateRoadNode(road);
    }
    delete roads;
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewCreateRoadNode( const odr::Road *road )
  {
    // Create node...
    TreeNode ^node =
      gcnew TreeNode( String::Format("Road {0}",road->ID()) );
    node->Tag = gcnew ManagedRef(road);
    node->Name = node->Tag->ToString();
    node->ContextMenuStrip = _treeItemContextMenu;

    // Insert node into tree...
    if( road->junction() ) {
      TreeNode ^parent = TreeviewFindNode( road->junction() );
      parent->Nodes->Add(node);
    }
    else
      _treeView->Nodes->Add(node);

    // Create geometry sub-tree...
    TreeNode ^geometryNode = node->Nodes->Add( "Geometry" );
    geometryNode->Tag = gcnew ManagedRef(road->geometry());
    geometryNode->Name = geometryNode->Tag->ToString();
    geometryNode->ContextMenuStrip = _geometryContextMenu;

    misc::Enumerator<const odr::GeometricPrimitive*> *geoEnum =
      road->geometry()->enumerate();
    const odr::GeometricPrimitive *geo = 0;
    while( geoEnum && (geo=geoEnum->item()) ) {
      geoEnum->moveNext();
      TreeviewCreatePrimitiveNode(geo);
    }
    delete geoEnum;

    // Create lanes sub-tree...
    TreeNode ^lanesNode = node->Nodes->Add( "Lanes" );
    lanesNode->Tag = gcnew ManagedRef(road->lanes());
    lanesNode->Name = lanesNode->Tag->ToString();
    lanesNode->ContextMenuStrip = _lanesContextMenu;

    misc::Enumerator<const odr::LaneSection*> *sectionEnum =
      road->lanes()->enumerate();
    const odr::LaneSection *section = 0;
    while( sectionEnum && (section=sectionEnum->item()) ) {
      sectionEnum->moveNext();
      TreeviewCreateLaneSectionNode(section);
    }
    delete sectionEnum;

    // Create elevation profile sub-tree...
    TreeNode ^elevationProfNode = node->Nodes->Add( "Elevation profile" );
    elevationProfNode->Tag = gcnew ManagedRef(road->elevationProfile());
    elevationProfNode->Name = elevationProfNode->Tag->ToString();
    elevationProfNode->ContextMenuStrip = _elevationProfileContextMenu;

    misc::Enumerator<const odr::Elevation*> *elevationEnum =
      road->elevationProfile()->enumerate();
    const odr::Elevation *elevation = 0;
    while( elevationEnum && (elevation=elevationEnum->item()) ) {
      elevationEnum->moveNext();
      TreeviewCreateElevationNode(elevation);
    }
    delete elevationEnum;

    // Create lateral profile sub-tree...
    TreeNode ^lateralProfNode = node->Nodes->Add( "Lateral profile" );
    lateralProfNode->Tag = gcnew ManagedRef(road->lateralProfile());
    lateralProfNode->Name = lateralProfNode->Tag->ToString();
    lateralProfNode->ContextMenuStrip = _lateralProfileContextMenu;

    misc::Enumerator<const odr::Superelevation*> *superEnum =
      road->lateralProfile()->enumerateSuperelevations();
    const odr::Superelevation *super = 0;
    while( superEnum && (super=superEnum->item()) ) {
      superEnum->moveNext();
      TreeviewCreateSuperelevationNode(super);
    }
    delete superEnum;

    misc::Enumerator<const odr::Crossfall*> *crossfallEnum =
      road->lateralProfile()->enumerateCrossfalls();
    const odr::Crossfall *crossfall = 0;
    while( crossfallEnum && (crossfall=crossfallEnum->item()) ) {
      crossfallEnum->moveNext();
      TreeviewCreateCrossfallNode(crossfall);
    }
    delete crossfallEnum;
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewCreatePrimitiveNode(
    const odr::GeometricPrimitive* geo )
  {
    String ^str;
    switch( geo->type() ) {
    case odr::GeometricPrimitive::T_LINE:
      str = "Line";
      break;
    case odr::GeometricPrimitive::T_SPIRAL:
      str = "Spiral";
      break;
    case odr::GeometricPrimitive::T_ARC:
      str = "Arc";
      break;
    default:
      str = "Poly3";
      break;
    }
    TreeNode ^geometryNode = TreeviewFindNode( geo->parentGeometry() );
    TreeNode ^geoNode = geometryNode->Nodes->Add(
      String::Format( "{0} (s={1})", str, geo->s() )  );
    geoNode->Tag = gcnew ManagedRef(geo);
    geoNode->Name = geoNode->Tag->ToString();
    geoNode->ContextMenuStrip = _treeItemContextMenu;
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewCreateLaneSectionNode(
    const odr::LaneSection* section )
  {
    TreeNode ^lanesNode = TreeviewFindNode( section->parentLanes() );
    TreeNode ^sectionNode = lanesNode->Nodes->Add(
      String::Format( "Section (s={0})", section->start() )  );
    sectionNode->Tag = gcnew ManagedRef(section);
    sectionNode->Name = sectionNode->Tag->ToString();
    sectionNode->ContextMenuStrip = _laneSectionContextMenu;

    misc::Enumerator<const odr::Lane*> *laneEnum = section->enumerate();
    const odr::Lane *lane = 0;
    while( laneEnum && (lane=laneEnum->item()) ) {
      laneEnum->moveNext();
      TreeviewCreateLaneNode(lane);
    }
    delete laneEnum;
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewCreateLaneNode( const odr::Lane* lane )
  {
    TreeNode ^sectionNode = TreeviewFindNode( lane->parentSection() );
    TreeNode ^laneNode = sectionNode->Nodes->Add(
      String::Format( "Lane {0}", lane->index() )  );
    laneNode->Tag = gcnew ManagedRef(lane);
    laneNode->Name = laneNode->Tag->ToString();
    laneNode->ContextMenuStrip = _laneContextMenu;

    misc::Enumerator<const odr::LaneWidth*> *widthEnum =
      lane->enumerateWidths();
    const odr::LaneWidth *width = 0;
    while( widthEnum && (width=widthEnum->item()) ) {
      widthEnum->moveNext();
      TreeviewCreateLaneWidthNode(width);
    }
    delete widthEnum;
  }
  
  //--------------------------------------------------------------------------
  void MainForm::TreeviewCreateLaneWidthNode( const odr::LaneWidth* width )
  {
    TreeNode ^laneNode = TreeviewFindNode( width->parentLane() );
    TreeNode ^widthNode = laneNode->Nodes->Add(
      String::Format( "Width (s={0})", width->startOffset() )  );
    widthNode->Tag = gcnew ManagedRef(width);
    widthNode->Name = widthNode->Tag->ToString();
    widthNode->ContextMenuStrip = _poly3ContextMenu;
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewCreateCrossfallNode( const odr::Crossfall* cross )
  {
    TreeNode ^lateralProfNode = TreeviewFindNode( cross->parentProfile() );
    TreeNode ^crossfallNode = lateralProfNode->Nodes->Add(
      String::Format( "Crossfall (s={0})", cross->start() )  );
    crossfallNode->Tag = gcnew ManagedRef(cross);
    crossfallNode->Name = crossfallNode->Tag->ToString();
    crossfallNode->ContextMenuStrip = _poly3ContextMenu;
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewCreateSuperelevationNode(
    const odr::Superelevation* super )
  {
    TreeNode ^lateralProfNode = TreeviewFindNode( super->parentProfile() );
    TreeNode ^superNode = lateralProfNode->Nodes->Add(
      String::Format( "Superelevation (s={0})", super->start() )  );
    superNode->Tag = gcnew ManagedRef(super);
    superNode->Name = superNode->Tag->ToString();
    superNode->ContextMenuStrip = _poly3ContextMenu;
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewCreateElevationNode( const odr::Elevation* elev )
  {
    TreeNode ^elevationProfNode = TreeviewFindNode( elev->parentProfile() );
    TreeNode ^elevationNode = elevationProfNode->Nodes->Add(
      String::Format( "Elevation (s={0})", elev->start() )  );
    elevationNode->Tag = gcnew ManagedRef(elev);
    elevationNode->Name = elevationNode->Tag->ToString();
    elevationNode->ContextMenuStrip = _poly3ContextMenu;
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewNodeDeleted(
    Object ^sender, DomainLogicEventArgs ^args )
  {
    // Find tree node associated with deleted data node...
    TreeNode ^node = TreeviewFindNode( args->Node );
    if( !node ) return;
    TreeNode ^parent = node->Parent;

    // Remove node from treeview...
    _treeView->Nodes->Remove( node );
    delete node;

    // If data node was a lane, update lane numbers...
    if( const odr::LaneSection* section =
          TreeviewNode2Data<odr::LaneSection>(parent) )
    {
      for( int i=0 ; i<parent->Nodes->Count ; ++i ) {
        TreeNode ^node = parent->Nodes[i];
        node->Text = String::Format(
          "Lane {0}", TreeviewNode2Data<odr::Lane>(node)->index() );
      }
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewNodeCreated(
    Object ^sender, DomainLogicEventArgs ^args )
  {
    if( const odr::Junction* junction =
          dynamic_cast<const odr::Junction*>(args->Node) )
      TreeviewCreateJunctionNode(junction);

    else if( const odr::Road* road =
              dynamic_cast<const odr::Road*>(args->Node) )
      TreeviewCreateRoadNode(road);

    else if( const odr::GeometricPrimitive* prim =
              dynamic_cast<const odr::GeometricPrimitive*>(args->Node) )
      TreeviewCreatePrimitiveNode(prim);

    else if( const odr::LaneSection* section =
              dynamic_cast<const odr::LaneSection*>(args->Node) )
      TreeviewCreateLaneSectionNode(section);

    else if( const odr::Lane* lane =
              dynamic_cast<const odr::Lane*>(args->Node) )
    {
      TreeNode ^node = TreeviewFindNode(lane->parentSection());
      node->Parent->Nodes->Remove(node);
      delete node;
      TreeviewCreateLaneSectionNode(lane->parentSection());
    }

    else if( const odr::LaneWidth* width =
              dynamic_cast<const odr::LaneWidth*>(args->Node) )
      TreeviewCreateLaneWidthNode(width);

    else if( const odr::Elevation* elev =
              dynamic_cast<const odr::Elevation*>(args->Node) )
      TreeviewCreateElevationNode(elev);

    else if( const odr::Superelevation* super =
              dynamic_cast<const odr::Superelevation*>(args->Node) )
      TreeviewCreateSuperelevationNode(super);

    else if( const odr::Crossfall* cross =
              dynamic_cast<const odr::Crossfall*>(args->Node) )
      TreeviewCreateCrossfallNode(cross);
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewNodeModified(
    Object ^sender, DomainLogicEventArgs ^args )
  {
    // Find treeview node associated with modified data node...
    TreeNode ^node = TreeviewFindNode( args->Node );
    if( !node ) return;
    TreeNode ^parentNode = node->Parent;

    // Unbind selection from DomainLogic::Instance->SelectedNode property...
    _treeView->AfterSelect -=
      gcnew TreeViewEventHandler( this, &MainForm::TreeviewAfterSelect );

    // Adopt approrpiate behavior depending on node type...
    if( const odr::GeometricPrimitive* prim =
      dynamic_cast<const odr::GeometricPrimitive*>(args->Node) )
    {
      // Update displayed S-coord...
      for( int i=0 ; i<parentNode->Nodes->Count ; ++i ) {
        TreeNode ^node = parentNode->Nodes[i];
        const odr::GeometricPrimitive* prim =
          TreeviewNode2Data<odr::GeometricPrimitive>(node);
        if( !prim ) {
          parentNode->Nodes->RemoveAt(i--);
          delete node;
        }

        else {        
          String ^str;
          switch( prim->type() ) {
          case odr::GeometricPrimitive::T_LINE:
            str = "Line";
            break;
          case odr::GeometricPrimitive::T_SPIRAL:
            str = "Spiral";
            break;
          case odr::GeometricPrimitive::T_ARC:
            str = "Arc";
            break;
          default:
            str = "Poly3";
            break;
          }
          node->Text = String::Format( "{0} (s={1})", str, prim->s() );
        }
      }
      
      // Move node to appropriate position...
      parentNode->Nodes->Remove(node);
      int pos=0;
      while( pos<parentNode->Nodes->Count && prim->s() >
        TreeviewNode2Data<odr::GeometricPrimitive>(parentNode->Nodes[pos])->s() )
        ++pos;
      parentNode->Nodes->Insert( pos, node );
      _treeView->SelectedNode = node;
    }

    else if( const odr::LaneSection* section =
      dynamic_cast<const odr::LaneSection*>(args->Node) )
    {
      // Update displayed S-coord...
      for( int i=0 ; i<parentNode->Nodes->Count ; ++i ) {
        TreeNode ^node = parentNode->Nodes[i];
        const odr::LaneSection* section =
          TreeviewNode2Data<odr::LaneSection>(node);
        if( !section ) {
          parentNode->Nodes->RemoveAt(i--);
          delete node;
        }
        else
          node->Text = String::Format( "Section (s={0})", section->start() );
      }

      // Move to appropriate position
      parentNode->Nodes->Remove(node);
      int pos=0;
      while( pos<parentNode->Nodes->Count && section->start() >
        TreeviewNode2Data<odr::LaneSection>(parentNode->Nodes[pos])->start() )
        ++pos;
      parentNode->Nodes->Insert( pos, node );
      _treeView->SelectedNode = node;
    }

    else if( const odr::LaneWidth* width =
      dynamic_cast<const odr::LaneWidth*>(args->Node) )
    {
      // Update displayed S-coord...
      for( int i=0 ; i<parentNode->Nodes->Count ; ++i ) {
        TreeNode ^node = parentNode->Nodes[i];
        const odr::LaneWidth* width =
          TreeviewNode2Data<odr::LaneWidth>(node);
        if( !width ) {
          parentNode->Nodes->RemoveAt(i--);
          delete node;
        }
        else
          node->Text = String::Format( "Width (s={0})", width->startOffset() );
      }

      // Move to appropriate position
      parentNode->Nodes->Remove(node);
      int pos=0;
      while( pos<parentNode->Nodes->Count && width->startOffset() >
        TreeviewNode2Data<odr::LaneWidth>(parentNode->Nodes[pos])->startOffset() )
        ++pos;
      parentNode->Nodes->Insert( pos, node );
      _treeView->SelectedNode = node;
    }

    else if( const odr::Elevation* elev =
      dynamic_cast<const odr::Elevation*>(args->Node) )
    {
      // Update displayed S-coord...
      for( int i=0 ; i<parentNode->Nodes->Count ; ++i ) {
        TreeNode ^node = parentNode->Nodes[i];
        const odr::Elevation* elev =
          TreeviewNode2Data<odr::Elevation>(node);
        if( !elev ) {
          parentNode->Nodes->RemoveAt(i--);
          delete node;
        }
        else
          node->Text = String::Format( "Elevation (s={0})", elev->start() );
      }

      // Move to appropriate position
      parentNode->Nodes->Remove(node);
      int pos=0;
      while( pos<parentNode->Nodes->Count && elev->start() >
        TreeviewNode2Data<odr::Elevation>(parentNode->Nodes[pos])->start() )
        ++pos;
      parentNode->Nodes->Insert( pos, node );
      _treeView->SelectedNode = node;
    }

    else if( const odr::Crossfall* cross =
      dynamic_cast<const odr::Crossfall*>(args->Node) )
    {
      // Update displayed S-coord...
      for( int i=0 ; i<parentNode->Nodes->Count ; ++i ) {
        TreeNode ^node = parentNode->Nodes[i];
        const odr::Crossfall* cross =
          TreeviewNode2Data<odr::Crossfall>(node);
        const odr::Superelevation* super =
          TreeviewNode2Data<odr::Superelevation>(node);
        if( !cross && !super ) {
          parentNode->Nodes->RemoveAt(i--);
          delete node;
        }
        else if( cross )
          node->Text = String::Format( "Crossfall (s={0})", cross->start() );
      }

      // Move to appropriate position
      parentNode->Nodes->Remove(node);
      int pos=0;
      while( pos<parentNode->Nodes->Count ) {
        const misc::WeakRefServer* data =
          TreeviewNode2Data<misc::WeakRefServer>(parentNode->Nodes[pos]);
        const odr::Crossfall* asCross =
          dynamic_cast<const odr::Crossfall*>(data);
        const odr::Superelevation* asSuper =
          dynamic_cast<const odr::Superelevation*>(data);
        double s =
          asCross ? asCross->start() : ( asSuper ? asSuper->start() : 0.0 );
        if( s>=cross->start() ) break;
        ++pos;
      }
      parentNode->Nodes->Insert( pos, node );
      _treeView->SelectedNode = node;
    }

    else if( const odr::Superelevation* super =
      dynamic_cast<const odr::Superelevation*>(args->Node) )
    {
      // Update displayed S-coord...
      for( int i=0 ; i<parentNode->Nodes->Count ; ++i ) {
        TreeNode ^node = parentNode->Nodes[i];
        const odr::Crossfall* cross =
          TreeviewNode2Data<odr::Crossfall>(node);
        const odr::Superelevation* super =
          TreeviewNode2Data<odr::Superelevation>(node);
        if( !super && !cross ) {
          parentNode->Nodes->RemoveAt(i--);
          delete node;
        }
        else if( super )
          node->Text = String::Format( "Superelevation (s={0})", super->start() );
      }

      // Move to appropriate position
      parentNode->Nodes->Remove(node);
      int pos=0;
      while( pos<parentNode->Nodes->Count ) {
        const misc::WeakRefServer* data =
          TreeviewNode2Data<misc::WeakRefServer>(parentNode->Nodes[pos]);
        const odr::Crossfall* asCross =
          dynamic_cast<const odr::Crossfall*>(data);
        const odr::Superelevation* asSuper =
          dynamic_cast<const odr::Superelevation*>(data);
        double s =
          asCross ? asCross->start() : ( asSuper ? asSuper->start() : 0.0 );
        if( s>=super->start() ) break;
        ++pos;
      }
      parentNode->Nodes->Insert( pos, node );
      _treeView->SelectedNode = node;
    }

    // Restore treeview selection...
    _treeView->SelectedNode =
      TreeviewFindNode( DomainLogic::Instance->SelectedNode );

    // Re-bind selection to DomainLogic::Instance->SelectedNode property...
    _treeView->AfterSelect +=
      gcnew TreeViewEventHandler( this, &MainForm::TreeviewAfterSelect );
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewKeyUp( Object ^sender, KeyEventArgs ^e )
  {
    // Manage DELETE key...
    if( e->KeyData == Keys::Delete ) {
      // Retrieve selection...
      const misc::WeakRefServer* object = DomainLogic::Instance->SelectedNode;

      // Delete selection...
      DomainLogic::Instance->DeleteNode(DomainLogic::Instance->SelectedNode);
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewAfterSelect( Object ^sender, TreeViewEventArgs ^e )
  {
    // Unbind event handler for domain logic-side selection change...
    DomainLogic::Instance->SelectionChanged -=
      gcnew DomainLogicEventHandler( this, &MainForm::TreeviewSelectionChanged );

    // Change domain logic-side selection...
    DomainLogic::Instance->SelectedNode =
      TreeviewNode2Data<misc::WeakRefServer>(_treeView->SelectedNode);

    // Rebind event handler for domain logic-side selection change...
    DomainLogic::Instance->SelectionChanged +=
      gcnew DomainLogicEventHandler( this, &MainForm::TreeviewSelectionChanged );
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewSelectionChanged(
    Object ^sender, DomainLogicEventArgs ^e )
  {
    // Unbind event handler for UI-side selection change...
    _treeView->AfterSelect -=
      gcnew TreeViewEventHandler( this, &MainForm::TreeviewAfterSelect );

    // Change UI-side selection...
    _treeView->SelectedNode =
      TreeviewFindNode( DomainLogic::Instance->SelectedNode );

    // Rebind event handler for UI-side selection change...
    _treeView->AfterSelect +=
      gcnew TreeViewEventHandler( this, &MainForm::TreeviewAfterSelect );
  }

  //--------------------------------------------------------------------------
  void MainForm::TreeviewNodeClick(
    Object ^sender, TreeNodeMouseClickEventArgs ^e )
  {
    if( e->Button==Forms::MouseButtons::Right )
      _treeView->SelectedNode = e->Node;
  }

  //--------------------------------------------------------------------------
  TreeNode^ MainForm::TreeviewFindNode( const misc::WeakRefServer* dataNode )
  {
    cli::array<TreeNode^> ^nodes =
      _treeView->Nodes->Find( IntPtr((void*)dataNode).ToString(), true );
    if( !nodes->Length ) return nullptr;
    return nodes[0];
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextDelete( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->DeleteNode(
      TreeviewNode2Data<misc::WeakRefServer>(_treeView->SelectedNode) );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAssistedPolyEdit( Object ^sender, EventArgs ^e )
  {
    const misc::WeakRefServer* selected = DomainLogic::Instance->SelectedNode;
    Poly3HelperForm ^form = gcnew Poly3HelperForm();

    if( const odr::LaneWidth* width =
          dynamic_cast<const odr::LaneWidth*>(selected) )
    {
      form->Text = "Edit width change";
      form->EditedValueName = "width (m)";
      double
        l=width->length(),
        a=width->a(), b=width->b(), c=width->c(), d=width->d();

      form->Length = l;
      form->InitialValue = a;
      form->InitialDerivative = b;
      form->FinalValue = a + l*b + l*l*c + l*l*l*d;
      form->FinalDerivative = b + 2.0*l*c + 3.0*l*l*d;

      if( form->ShowDialog()==Forms::DialogResult::OK ) {
        DomainLogic::Instance->SetLaneWidthA( gcnew Double(form->A) );
        DomainLogic::Instance->SetLaneWidthB( gcnew Double(form->B) );
        DomainLogic::Instance->SetLaneWidthC( gcnew Double(form->C) );
        DomainLogic::Instance->SetLaneWidthD( gcnew Double(form->D) );
      }
    }

    else if( const odr::Elevation* elev =
              dynamic_cast<const odr::Elevation*>(selected) )
    {
      form->Text = "Edit elevation change";
      form->EditedValueName = "height (m)";
      double
        l=elev->length(),
        a=elev->a(), b=elev->b(), c=elev->c(), d=elev->d();

      form->Length = l;
      form->InitialValue = a;
      form->InitialDerivative = b;
      form->FinalValue = a + l*b + l*l*c + l*l*l*d;
      form->FinalDerivative = b + 2.0*l*c + 3.0*l*l*d;

      if( form->ShowDialog()==Forms::DialogResult::OK ) {
        DomainLogic::Instance->SetElevationA( gcnew Double(form->A) );
        DomainLogic::Instance->SetElevationB( gcnew Double(form->B) );
        DomainLogic::Instance->SetElevationC( gcnew Double(form->C) );
        DomainLogic::Instance->SetElevationD( gcnew Double(form->D) );
      }
    }

    else if( const odr::Superelevation* super =
              dynamic_cast<const odr::Superelevation*>(selected) )
    {
      form->Text = "Edit superelevation change";
      form->EditedValueName = "angle (rad)";
      double
        l=super->length(),
        a=super->a(), b=super->b(), c=super->c(), d=super->d();

      form->Length = l;
      form->InitialValue = a;
      form->InitialDerivative = b;
      form->FinalValue = a + l*b + l*l*c + l*l*l*d;
      form->FinalDerivative = b + 2.0*l*c + 3.0*l*l*d;

      if( form->ShowDialog()==Forms::DialogResult::OK ) {
        DomainLogic::Instance->SetSuperelevationA( gcnew Double(form->A) );
        DomainLogic::Instance->SetSuperelevationB( gcnew Double(form->B) );
        DomainLogic::Instance->SetSuperelevationC( gcnew Double(form->C) );
        DomainLogic::Instance->SetSuperelevationD( gcnew Double(form->D) );
      }
    }

    else if( const odr::Crossfall* cross =
          dynamic_cast<const odr::Crossfall*>(selected) )
    {
      form->Text = "Edit crossfall change";
      form->EditedValueName = "angle (rad)";
      double
        l=cross->length(),
        a=cross->a(), b=cross->b(), c=cross->c(), d=cross->d();

      form->Length = l;
      form->InitialValue = a;
      form->InitialDerivative = b;
      form->FinalValue = a + l*b + l*l*c + l*l*l*d;
      form->FinalDerivative = b + 2.0*l*c + 3.0*l*l*d;

      if( form->ShowDialog()==Forms::DialogResult::OK ) {
        DomainLogic::Instance->SetCrossfallA( gcnew Double(form->A) );
        DomainLogic::Instance->SetCrossfallB( gcnew Double(form->B) );
        DomainLogic::Instance->SetCrossfallC( gcnew Double(form->C) );
        DomainLogic::Instance->SetCrossfallD( gcnew Double(form->D) );
      }
    }

    delete form;
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddRoad( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreateRoad(  dynamic_cast<const odr::Junction*>(
      DomainLogic::Instance->SelectedNode )  );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddJunction( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreateJunction();
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddLine( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreateLine(  dynamic_cast<const odr::Geometry*>(
      DomainLogic::Instance->SelectedNode )  );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddArc( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreateArc(  dynamic_cast<const odr::Geometry*>(
      DomainLogic::Instance->SelectedNode )  );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddSpiral( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreateSpiral(  dynamic_cast<const odr::Geometry*>(
      DomainLogic::Instance->SelectedNode )  );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddPoly3( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreatePoly3(  dynamic_cast<const odr::Geometry*>(
      DomainLogic::Instance->SelectedNode )  );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddSection( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreateLaneSection(
      dynamic_cast<const odr::Lanes*>(DomainLogic::Instance->SelectedNode) );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddLane( Object ^sender, EventArgs ^e )
  {
    const odr::LaneSection* section = dynamic_cast<const odr::LaneSection*>(
      DomainLogic::Instance->SelectedNode );
    if( !section ) return;
    NewLaneForm ^diag = gcnew NewLaneForm(
      -(int)section->countRight()-1, (int)section->countLeft()+1 );
    diag->ShowDialog();
    if( diag->DialogResult==Forms::DialogResult::OK )
      DomainLogic::Instance->CreateLane( section, diag->LaneIndex );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddWidth( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreateLaneWidth(
      dynamic_cast<const odr::Lane*>(DomainLogic::Instance->SelectedNode) );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddCrossfall( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreateCrossfall(
      dynamic_cast<const odr::LateralProfile*>(
        DomainLogic::Instance->SelectedNode )  );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddSuperelevation( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreateSuperelevation(
      dynamic_cast<const odr::LateralProfile*>(
        DomainLogic::Instance->SelectedNode )  );
  }

  //--------------------------------------------------------------------------
  void MainForm::ContextAddElevation( Object ^sender, EventArgs ^e )
  {
    DomainLogic::Instance->CreateElevation(
      dynamic_cast<const odr::ElevationProfile*>(
        DomainLogic::Instance->SelectedNode )  );
  }

} // namespace roads
