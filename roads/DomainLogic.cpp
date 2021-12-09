//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file DomainLogic.cpp
//! \brief Definition of methods of class roads::DomainLogic.

#include "StdAfx.h"
#include "DomainLogic.h"
#include "Render.h"
#include <vector>
#include <map>

#include <fstream>

using namespace System;
using namespace System::Runtime::InteropServices;

#ifndef M_PI
# define M_PI 3.141592654
#endif

namespace roads {

  //--------------------------------------------------------------------------
  DomainLogic^ DomainLogic::Instance::get( void )
  {
    if( !_Instance ) _Instance=gcnew DomainLogic();
    return _Instance;
  }

  DomainLogic::DomainLogic( void ) :
  _document( 0 )
	{
    _document = new misc::WeakRef<odr::Document>(0);
    _selection = new misc::WeakRef<misc::WeakRefServer>(0);
  }

  DomainLogic::~DomainLogic()
	{
    delete _document->ptr();
    delete _document;
    _document = 0;
    delete _selection;
    _selection = 0;
	}

  DomainLogic::!DomainLogic()
	{
    delete _document;
    delete _selection;
	}



  //--------------------------------------------------------------------------
  const odr::Document* DomainLogic::Document::get( void ) {
    return _document->ptr();
  }

  odr::Document* DomainLogic::DocumentRW::get( void ) {
    AsyncRaise( _DocumentModified, gcnew DomainLogicEventArgs() );
    return _document->ptr();
  }

  String^ DomainLogic::FileName::get( void ) {
    return _fileName;
  }

  void DomainLogic::FileName::set( String ^s ) {
    _fileName = s;
    FileNameChanged( this, gcnew DomainLogicEventArgs() );
  }

  const misc::WeakRefServer* DomainLogic::SelectedNode::get( void ) {
    return _selection->ptr();
  }

  void DomainLogic::SelectedNode::set( const misc::WeakRefServer* node ) {
    *_selection = const_cast<misc::WeakRefServer*>(node);
    DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
    args->Node = const_cast<misc::WeakRefServer*>(node);
    SelectionChanged( this, args );
  }

  misc::WeakRefServer* DomainLogic::SelectedNodeRW::get( void ) {
    DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs;
    args->Node = _selection->ptr();
    AsyncRaise( _DocumentModified, args );
    AsyncRaise( _NodeModified, args );
    return _selection->ptr();
  }

  void DomainLogic::SelectedNodeRW::set( misc::WeakRefServer* node ) {
    SelectedNode = node;
  }



  //--------------------------------------------------------------------------
  void DomainLogic::DocumentCreated::add( DomainLogicEventHandler ^del )
  { _DocumentCreated += del; }

  void DomainLogic::DocumentCreated::remove( DomainLogicEventHandler ^del )
  { _DocumentCreated -= del; }

  void DomainLogic::DocumentCreated::raise(
    Object ^sender, DomainLogicEventArgs ^args )
  { if(_DocumentCreated) _DocumentCreated( sender, args ); }


  void DomainLogic::DocumentLoaded::add( DomainLogicEventHandler ^del )
  { _DocumentLoaded += del; }

  void DomainLogic::DocumentLoaded::remove( DomainLogicEventHandler ^del )
  { _DocumentLoaded -= del; }

  void DomainLogic::DocumentLoaded::raise(
    Object ^sender, DomainLogicEventArgs ^args )
  { if(_DocumentLoaded) _DocumentLoaded( sender, args ); }


  void DomainLogic::DocumentSaved::add( DomainLogicEventHandler ^del )
  { _DocumentSaved += del; }

  void DomainLogic::DocumentSaved::remove( DomainLogicEventHandler ^del )
  { _DocumentSaved -= del; }

  void DomainLogic::DocumentSaved::raise(
    Object ^sender, DomainLogicEventArgs ^args )
  { if(_DocumentSaved) _DocumentSaved( sender, args ); }


  void DomainLogic::DocumentClosed::add( DomainLogicEventHandler ^del )
  { _DocumentClosed += del; }

  void DomainLogic::DocumentClosed::remove( DomainLogicEventHandler ^del )
  { _DocumentClosed -= del; }

  void DomainLogic::DocumentClosed::raise(
    Object ^sender, DomainLogicEventArgs ^args )
  { if(_DocumentClosed) _DocumentClosed( sender, args ); }


  void DomainLogic::DocumentModified::add( DomainLogicEventHandler ^del )
  { _DocumentModified += del; }

  void DomainLogic::DocumentModified::remove( DomainLogicEventHandler ^del )
  { _DocumentModified -= del; }

  void DomainLogic::DocumentModified::raise(
    Object ^sender, DomainLogicEventArgs ^args )
  { if(_DocumentModified) _DocumentModified( sender, args ); }


   void DomainLogic::NodeModified::add( DomainLogicEventHandler ^del )
  { _NodeModified += del; }

  void DomainLogic::NodeModified::remove( DomainLogicEventHandler ^del )
  { _NodeModified -= del; }

  void DomainLogic::NodeModified::raise(
    Object ^sender, DomainLogicEventArgs ^args )
  { if(_NodeModified) _NodeModified( sender, args ); }


   void DomainLogic::NodeDeleted::add( DomainLogicEventHandler ^del )
  { _NodeDeleted += del; }

  void DomainLogic::NodeDeleted::remove( DomainLogicEventHandler ^del )
  { _NodeDeleted -= del; }

  void DomainLogic::NodeDeleted::raise(
    Object ^sender, DomainLogicEventArgs ^args )
  { if(_NodeDeleted) _NodeDeleted( sender, args ); }


  void DomainLogic::NodeCreated::add( DomainLogicEventHandler ^del )
  { _NodeCreated += del; }

  void DomainLogic::NodeCreated::remove( DomainLogicEventHandler ^del )
  { _NodeCreated -= del; }

  void DomainLogic::NodeCreated::raise(
    Object ^sender, DomainLogicEventArgs ^args )
  { if(_NodeCreated) _NodeCreated( sender, args ); }


   void DomainLogic::SelectionChanged::add( DomainLogicEventHandler ^del )
  { _SelectionChanged += del; }

  void DomainLogic::SelectionChanged::remove( DomainLogicEventHandler ^del )
  { _SelectionChanged -= del; }

  void DomainLogic::SelectionChanged::raise(
    Object ^sender, DomainLogicEventArgs ^args )
  { if(_SelectionChanged) _SelectionChanged( sender, args ); }


  void DomainLogic::FileNameChanged::add( DomainLogicEventHandler ^del )
  { _FileNameChanged += del; }

  void DomainLogic::FileNameChanged::remove( DomainLogicEventHandler ^del )
  { _FileNameChanged -= del; }

  void DomainLogic::FileNameChanged::raise(
    Object ^sender, DomainLogicEventArgs ^args )
  { if(_FileNameChanged) _FileNameChanged( sender, args ); }



  //--------------------------------------------------------------------------
  DomainLogic::EventRaiser::EventRaiser( DomainLogicEventHandler ^toRaise,
                                         DomainLogic ^sender,
                                         DomainLogicEventArgs ^args ):
  _toRaise(toRaise), _sender(sender), _args(args)
  {
    _handler = gcnew EventHandler( this, &EventRaiser::DoInvoke );
    Windows::Forms::Application::Idle += _handler;
  }

  void DomainLogic::EventRaiser::DoInvoke( Object ^s, EventArgs ^a ) {
    Windows::Forms::Application::Idle -= _handler;
    _toRaise( _sender, _args );
  }

  void DomainLogic::AsyncRaise(
    DomainLogicEventHandler ^toRaise, DomainLogicEventArgs ^args )
  { gcnew EventRaiser( toRaise, this, args ); }



  //--------------------------------------------------------------------------
  void DomainLogic::NewDocument( void ) {
    delete _document->ptr();
    *_document = odr_createDocument( "untitled" );
    DocumentCreated( this, gcnew DomainLogicEventArgs() );
    DocumentModified( this, gcnew DomainLogicEventArgs() );
  }

  bool DomainLogic::OpenDocument( String ^fileName ) {
    FileName = fileName;
    delete _document->ptr();
    *_document = odr_createDocument( "" );

    IntPtr asCString = Marshal::StringToHGlobalAnsi(fileName);
    bool result = (*_document)->load( (const char*)(void*)asCString );
    Marshal::FreeHGlobal(asCString);

    DocumentLoaded( this, gcnew DomainLogicEventArgs() );
    DocumentModified( this, gcnew DomainLogicEventArgs() );

    return result;
  }

  bool DomainLogic::MergeDocument( String ^fileName ) {
    if( !_document->ptr() )
      return false;

    IntPtr asCString = Marshal::StringToHGlobalAnsi(fileName);
    bool result = (*_document)->load( (const char*)(void*)asCString, true );
    Marshal::FreeHGlobal(asCString);

    DocumentLoaded( this, gcnew DomainLogicEventArgs() );
    DocumentModified( this, gcnew DomainLogicEventArgs() );

    return result;
  }

  void DomainLogic::CloseDocument( void ) {
    delete _document->ptr();
    DocumentClosed( this, gcnew DomainLogicEventArgs() );
    DocumentModified( this, gcnew DomainLogicEventArgs() );
  }

  bool DomainLogic::SaveDocument( void ) {
    if( !_document->ptr() )
      return false;

    IntPtr asCString = Marshal::StringToHGlobalAnsi(FileName);
    bool result = (*_document)->save( (const char*)(void*)asCString );
    Marshal::FreeHGlobal(asCString);

    DocumentSaved( this, gcnew DomainLogicEventArgs() );

    return result;
  }



  //--------------------------------------------------------------------------
  bool DomainLogic::DeleteNode( const misc::WeakRefServer *constNode ) {
    misc::WeakRefServer *node = const_cast<misc::WeakRefServer*>(constNode);
    misc::WeakRefServer *oldSelection = _selection->ptr();

    const odr::GeometricPrimitive* geo=0;
    const odr::LaneSection* section=0;
    const odr::LaneWidth* width=0;

    // Leave if node should not be deleted...
    if( !node ||
        dynamic_cast<const odr::Lanes*>(node) ||
        dynamic_cast<const odr::Geometry*>(node) ||
        dynamic_cast<const odr::LateralProfile*>(node) ||
        dynamic_cast<const odr::ElevationProfile*>(node) ||
        ( (geo=dynamic_cast<const odr::GeometricPrimitive*>(node))
          && geo->s()==0.0 ) ||
        ( (section=dynamic_cast<const odr::LaneSection*>(node))
          && section->start()==0.0 ) ||
        ( (width=dynamic_cast<const odr::LaneWidth*>(node))
          && width->startOffset()==0.0 ) )
      return false;

    odr::GeometricPrimitive *primitive =
      dynamic_cast<odr::GeometricPrimitive*>(node);
    odr::Geometry* geometry = primitive ? primitive->parentGeometry() : 0;

    delete node;
    if( geometry ) UpdateGeometricPrimitives(geometry);
    DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
    args->Node = constNode;
    NodeDeleted( this, args );
    DocumentModified( this, args );
    if( node!=oldSelection )
      SelectionChanged( this, args );
    return true;
  }

  const odr::Junction* DomainLogic::CreateJunction(void)
  {
    const odr::Junction* result = (*_document)->createJunction();
    if( result ) {
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
    }
    return result;
  }

  const odr::Road* DomainLogic::CreateRoad( const odr::Junction* junction )
  {
    odr::Road* result =
      junction ? const_cast<odr::Junction*>(junction)->createRoad() :
      (*_document)->createRoad();
    if( result ) {
      result->geometry()->createLine(0.0);
      result->setLength(1.0);
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
    }
    return result;
  }

  const odr::Line* DomainLogic::CreateLine( const odr::Geometry* geometry )
  {
    if( !geometry ) return 0;
    double start = geometry->parentRoad()->length();
    odr::Line* result =
      const_cast<odr::Geometry*>(geometry)->createLine(start);
    if( result ) {
      result->setLength(1.0);
      UpdateGeometricPrimitives( const_cast<odr::Geometry*>(geometry) );
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
    }
    return result;
  }

  const odr::Spiral* DomainLogic::CreateSpiral( const odr::Geometry* geometry )
  {
    if( !geometry ) return 0;
    double start = geometry->parentRoad()->length();
    odr::Spiral* result = 
      const_cast<odr::Geometry*>(geometry)->createSpiral(start);
    if( result ) {
      result->setLength(1.0);
      UpdateGeometricPrimitives( const_cast<odr::Geometry*>(geometry) );
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
    }
    return result;
  }

  const odr::Arc* DomainLogic::CreateArc( const odr::Geometry* geometry )
  {
    if( !geometry ) return 0;
    double start = geometry->parentRoad()->length();
    odr::Arc* result = 
      const_cast<odr::Geometry*>(geometry)->createArc(start);
    if( result ) {
      result->setLength(1.0);
      UpdateGeometricPrimitives( const_cast<odr::Geometry*>(geometry) );
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
    }
    return result;
  }

  const odr::Poly3* DomainLogic::CreatePoly3( const odr::Geometry* geometry )
  {
    if( !geometry ) return 0;
    double start = geometry->parentRoad()->length();
    odr::Poly3* result = 
      const_cast<odr::Geometry*>(geometry)->createPoly3(start);
    if( result ) {
      result->setLength(1.0);
      UpdateGeometricPrimitives( const_cast<odr::Geometry*>(geometry) );
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
    }
    return result;
  }

  const odr::LaneSection* DomainLogic::CreateLaneSection(
    const odr::Lanes* lanes )
  {
    if( !lanes ) return 0;
    double start=0.0;
    if( lanes->count() ) {
      double length = lanes->parentRoad()->length();
      start = ( length + lanes->sectionAt(length)->start() ) / 2.0;
    }
    odr::LaneSection* result =
      const_cast<odr::Lanes*>(lanes)->createSection(start);
    if( result ) {
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
      if( lanes->count()>=2 ) {
        // Copy lanes from previous section...
        const odr::LaneSection *original = lanes->item( lanes->count()-2 );
        double originalLength = original->length();
        for( int i=1 ; i<=(int)original->countLeft() ; ++i ) {
          odr::Lane* lane = result->createLane(i);
          if(!lane) continue;
          args->Node = lane;
          NodeCreated(this,args);
          DocumentModified(this,args);
          const odr::LaneWidth* originalWidth =
            original->item(i)->widthAt(originalLength);
          if(!originalWidth) continue;
          odr::LaneWidth* width = lane->createWidth(0.0);
          if(!width) continue;
          width->setA( originalWidth->a() );
          width->setB( originalWidth->b() );
          width->setC( originalWidth->c() );
          width->setD( originalWidth->d() );
          args->Node = width;
          NodeCreated(this,args);
          DocumentModified(this,args);
        }

        for( int i=-1 ; i>=-(int)original->countRight() ; --i ) {
          odr::Lane* lane = result->createLane(i);
          if(!lane) continue;
          args->Node = lane;
          NodeCreated(this,args);
          DocumentModified(this,args);
          const odr::LaneWidth* originalWidth =
            original->item(i)->widthAt(originalLength);
          if(!originalWidth) continue;
          odr::LaneWidth* width = lane->createWidth(0.0);
          if(!width) continue;
          width->setA( originalWidth->a() );
          width->setB( originalWidth->b() );
          width->setC( originalWidth->c() );
          width->setD( originalWidth->d() );
          args->Node = width;
          NodeCreated(this,args);
          DocumentModified(this,args);
        }
      }
    }
    return result;
  }

  const odr::Lane* DomainLogic::CreateLane(
    const odr::LaneSection* section, int index )
  {
    if( !section || !index ) return 0;
    odr::Lane* result =
      const_cast<odr::LaneSection*>(section)->createLane(index);
    if( result ) {
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
      odr::LaneWidth* width=result->createWidth(0.0);
      if(width) {
        width->setA(3.5);
        args->Node = width;
        NodeCreated(this,args);
        DocumentModified(this,args);
      }
    }
    return result;
  }

  const odr::LaneWidth* DomainLogic::CreateLaneWidth( const odr::Lane* lane )
  {
    if( !lane ) return 0;
    double start=0.0;
    if( lane->widthCount() ) {
      double length = lane->parentSection()->length();
      start = ( length + lane->widthAt(length)->startOffset() ) / 2.0;
    }
    odr::LaneWidth* result =
      const_cast<odr::Lane*>(lane)->createWidth(start);
    if( result ) {
      result->setA(3.5);
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
    }
    return result;
  }

  const odr::Elevation* DomainLogic::CreateElevation(
    const odr::ElevationProfile* profile )
  {
    if( !profile ) return 0;
    double start=0.0;
    if( profile->count() ) {
      double length = profile->parentRoad()->length();
      start = ( length + profile->elevationAt(length)->length() ) / 2.0;
    }
    const odr::Elevation* result =
      const_cast<odr::ElevationProfile*>(profile)->createRecord(start);
    if( result ) {
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
    }
    return result;
  }

  const odr::Crossfall* DomainLogic::CreateCrossfall(
    const odr::LateralProfile* profile )
  {
    if( !profile ) return 0;
    double start=0.0;
    if( profile->crossfallCount() ) {
      double length = profile->parentRoad()->length();
      start = ( length + profile->crossfallAt(length)->length() ) / 2.0;
    }
    const odr::Crossfall* result =
      const_cast<odr::LateralProfile*>(profile)->createCrossfall(start);
    if( result ) {
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
    }
    return result;
  }

  const odr::Superelevation* DomainLogic::CreateSuperelevation(
    const odr::LateralProfile* profile )
  {
    if( !profile ) return 0;
    double start=0.0;
    if( profile->superelevationCount() ) {
      double length = profile->parentRoad()->length();
      start = ( length + profile->superelevationAt(length)->length() ) / 2.0;
    }
    const odr::Superelevation* result =
      const_cast<odr::LateralProfile*>(profile)->createSuperelevation(start);
    if( result ) {
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = result;
      NodeCreated(this,args);
      DocumentModified(this,args);
    }
    return result;
  }



  //--------------------------------------------------------------------------
  const odr::GeometricPrimitive* DomainLogic::GetRoadPrimitiveNear(
    const odr::Road* road, double x, double y, double tol,
    double% s_out, double %d )
  {
    // Find whereabouts of closest point on road...
    std::map<double,double> closests; // map distance to s-coord, keeping the 2 closest points
    closests[ 2.0*tol + 6.0 ] = 0.0;
    closests[ 3.0*tol + 6.0 ] = 0.0;
    double length = road->length();
    double increment = length / std::ceil(length/5.0);
    for( double s=0.0 ; s<length+0.5*increment ; s+=increment ) {
      if(s>length) s=length;
      double pos[3] = {0.0};
      Render::getEvaluator()->evaluatePositionOnRoad( road, s, 0, 0.0, pos );
      double distance = sqrt(
        asphalt::math<double>::sqr(pos[0]-x)+asphalt::math<double>::sqr(pos[1]-y) );
      closests[ distance ] = s;
      closests.erase( closests.rbegin()->first );
    }

    // Leave if result can't meet tolerance requirements...
    if( closests.begin()->first > tol+5.0 ) return 0;

    // Use dichotomy to get a finer result...
    for( int i=0 ; i<10 && closests.size()==2 ; ++i ) {
      double
        pos[3] = {0.0},
        s = 0.5 * ( closests.begin()->second + closests.rbegin()->second );
      Render::getEvaluator()->evaluatePositionOnRoad( road, s, 0, 0.0, pos );
      double distance = sqrt(
        asphalt::math<double>::sqr(pos[0]-x)+asphalt::math<double>::sqr(pos[1]-y) );
      closests[ distance ] = s;
      closests.erase( closests.rbegin()->first );
    }

    // Save result if tolerance requirements are satisfied...
    if( closests.begin()->first <= tol ) {
      s_out = closests.begin()->second;
      const odr::GeometricPrimitive *result = road->geometry()->primitiveAt(s_out);
      s_out -= result->s();
      d = closests.begin()->first;
      return result;
    }
    return 0;
  }

  const odr::GeometricPrimitive* DomainLogic::GetPrimitiveNear(
    double x, double y, double tol, double% s )
  {
    if( !Document ) return 0;

    double closestDistance = tol;
    const odr::GeometricPrimitive* closest = 0;

    // For each road...
    misc::Enumerator<const odr::Element*> *elEnum = Document->enumerate();
    const odr::Element *el=0;
    while( elEnum && (el=elEnum->item()) ) {
      elEnum->moveNext();
      const odr::Road* road = dynamic_cast<const odr::Road*>(el);
      if( !road ) continue;

      // Get road primitive near point...
      const odr::GeometricPrimitive* prim =
        GetRoadPrimitiveNear( road, x, y, closestDistance, s, closestDistance );

      // Remember primitive if it is the closest...
      if( prim ) closest=prim;
    }
    delete elEnum;

    return closest;
  }

  const misc::WeakRefServer* DomainLogic::PickNode(
    double x, double y, double tol )
  {
    // List of objects under the mouse...
    std::list<const misc::WeakRefServer*> selectable;

    // Manage clicking on a geometric primitive...
    double dummy=0.0;
    const misc::WeakRefServer* tmpPtr = GetPrimitiveNear( x, y, tol, dummy );
    if( tmpPtr ) selectable.push_back( tmpPtr );

    // Manage clicking on parts of a road...
    misc::Enumerator<const odr::Element*> *elEnum = Document->enumerate();
    const odr::Element *el=0;
    while( elEnum && (el=elEnum->item()) ) {
      elEnum->moveNext();
      const odr::Road* road = dynamic_cast<const odr::Road*>(el);
      if( !road ) continue;

      double sOffset, d;
      const odr::GeometricPrimitive *prim =
        GetRoadPrimitiveNear( road, x, y, 50.0, sOffset, d );
      if( !prim ) continue;

      double chordX, chordY, chordH;
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        prim, sOffset, &chordX, &chordY, &chordH );
      double cosH = cos(chordH), sinH = sin(chordH);

      if( abs((x-chordX)*cosH + (y-chordY)*sinH) > 0.1 )
        continue;

      double s = prim->s()+sOffset,
             t = (y-chordY)*cosH - (x-chordX)*sinH;

      const odr::LaneSection* section = road->lanes()->sectionAt(s);
      
      {
        double tmpPos[3];
        Render::getEvaluator()->evaluatePositionOnRoad(
          road, s, section->countLeft(), 0.0, tmpPos );
        if( (tmpPos[1]-chordY)*cosH - (tmpPos[0]-chordX)*sinH < t ) continue;

        Render::getEvaluator()->evaluatePositionOnRoad(
          road, s, -(int)section->countRight(), 0.0, tmpPos );
        if( (tmpPos[1]-chordY)*cosH - (tmpPos[0]-chordX)*sinH > t ) continue;
      }

      selectable.push_back( road );
      selectable.push_back( section );

      for( int laneIndex = -(int)section->countRight() ;
           laneIndex<(int)section->countLeft() ; ++laneIndex )
      {
        double pos[3];
        Render::getEvaluator()->evaluatePositionOnRoad(
          road, s, laneIndex+1, 0.0, pos );
        if( (pos[1]-chordY)*cosH - (pos[0]-chordX)*sinH > t ) {
          const odr::Lane* lane =
            section->item(laneIndex>=0 ? laneIndex+1 : laneIndex);
          selectable.push_back( lane );
          selectable.push_back( lane->widthAt(s-section->start()) );
          break;
        }
      }

      const misc::WeakRefServer* tmp =
        road->elevationProfile()->elevationAt(s);
      if( tmp ) selectable.push_back(tmp);

      tmp = road->lateralProfile()->crossfallAt(s);
      if( tmp ) selectable.push_back(tmp);

      tmp = road->lateralProfile()->superelevationAt(s);
      if( tmp ) selectable.push_back(tmp);
    }
    delete elEnum;

    // Cycle through objects under the mouse...
    std::list<const misc::WeakRefServer*>::const_iterator it =
      std::find( selectable.begin(), selectable.end(), SelectedNode );
    if( it==selectable.end() )
      SelectedNode = selectable.empty() ? 0 : selectable.front();
    else {
      ++it;
      SelectedNode = ( it==selectable.end() ) ? selectable.front() : *it;
    }
    
    return SelectedNode;
  }



  //--------------------------------------------------------------------------
  struct LinRegResult {
    double a, b, error;
  };

  typedef std::map< std::pair<size_t,size_t>, LinRegResult > LinRegResults;

  struct Solution {
    double cost;
    size_t next_index;
  };

  typedef std::map< std::pair<size_t,size_t>, Solution > Solutions;

  const odr::Road* DomainLogic::SketchRoad(
    Collections::Generic::List<double> ^x,
    Collections::Generic::List<double> ^y,
    double primitive_cost )
  {
    if( x->Count<2 || x->Count!=y->Count ) return 0;

    // Extract curvature and length along curve...
    std::vector<double> s, k;
    s.reserve( x->Count );
    s.push_back(0.0);
    k.reserve( x->Count );
    k.push_back(0.0);

    {
      double
        cur[2] = { x[0], y[0] },
        nxt[2] = { x[1], y[1] };

      for( int i=1 ; i<x->Count ; ++i ) {
        double prv[2], va[2];
#define v(x) asphalt::mk_vecop(x)
        v(prv) = v(cur);
        v(cur) = v(nxt);
        v(va) = v(cur)-v(prv);
        double lva = v(va).norm2();
        s.push_back( s.back() + lva );
        if( i+1 < x->Count ) {
          nxt[0] = x[i+1];
          nxt[1] = y[i+1];
          double vb[2];
          v(vb) = v(nxt)-v(cur);
          double lvb = v(vb).norm2();
          double theta = acos( v(va) * v(vb) / (lva*lvb) );
          if( va[0]*vb[1]-va[1]*vb[0] < 0.0 )
            theta *= -1.0;
          k.push_back( 2.0 * sin(0.5*theta) / sqrt(lva*lvb) );
        }
        else
          k.push_back(0.0);
        if( s.back()!=s.back() || k.back()!=k.back() ) {
          s.pop_back();
          k.pop_back();
          x->RemoveAt(i);
          y->RemoveAt(i);
          --i;
        }
#undef v
      }
    }

    // Compute linear regression of curve intrinsic function
    // curvature=f(length) for every sub-sequence of points...
    LinRegResults linearRegResults;
    for( size_t i=0 ; i+1<s.size() ; ++i ) {
      for( size_t j=i+1 ; j<s.size() ; ++j ) {
        // Compute average length, average curvature, average square length
        // and average length.curvature product...
        double avg_s=0.0, avg_k=0.0, avg_s2=0.0, avg_sk=0.0;
        for( size_t l=i ; l<=j ; ++l ) {
          avg_s  += s[l];
          avg_k  += k[l];
          avg_s2 += s[l]*s[l];
          avg_sk += s[l]*k[l];
        }
        double n = double(j+1-i);
        avg_s  /= n;
        avg_k  /= n;
        avg_s2 /= n;
        avg_sk /= n;

        // Compute length variance...
        double var_s = avg_s2 - avg_s*avg_s;

        // Compute length.curvature covariance...
        double cov_sk = avg_sk - avg_s*avg_k;

        // Compute linear regression coefficients...
        LinRegResult result;
        result.a = cov_sk / var_s;
        result.b = avg_k - result.a*avg_s;

        // Compute integral error...
        result.error = 0.0;
        for( size_t l=i ; l<j ; ++l ) {
          double
            e_start = k[l] - result.a*s[l] - result.b,
            e_end = k[l+1] - result.a*s[l+1] - result.b;

          double ea = (e_end-e_start) / (s[l+1]-s[l]);
          double eb = e_start - ea*s[l];

          if( e_start*e_end < 0.0 ) {
            double s_zero = -eb / ea;
            // "Normal" error
            result.error +=
              0.5*abs(e_start)*(s_zero-s[l]) +
              0.5*abs(e_end)*(s[l+1]-s_zero);

            // Quadratic error
            //result.error +=
            //  ( abs( pow(ea*s[l+1]+eb,3.0) - pow(ea*s_zero+eb,3.0) ) +
            //    abs( pow(ea*s_zero+eb,3.0) - pow(ea*s[l]+eb,3.0) )     )
            //  / abs(3.0*ea);
          }
          else {
            // "Normal" error
            result.error += 0.5*abs(e_start+e_end)*(s[l+1]-s[l]);

            // Quadratic error
            //result.error +=
            //  abs( pow(ea*s[l+1]+eb,3.0) - pow(ea*s[l]+eb,3.0) ) / abs(3.0*ea);
          }
        }

        // Save result...
        linearRegResults[ std::make_pair(i,j) ] = result;
      }
    }

    // Find cost-minimizing solution (e.g. piecewise linear regression of
    // intrinsic function) going from point i to point j for every (i,j)...
    Solutions solutions;
    for( size_t j=1 ; j<s.size() ; ++j ) {
      for( size_t i_plus_1=j ; i_plus_1>0 ; --i_plus_1 ) {
        size_t i = i_plus_1-1;

        // Default solution is to go straight from Pi to Pj...
        Solution solution;
        solution.cost = linearRegResults[ std::make_pair(i,j) ].error + primitive_cost;
        solution.next_index = j;

        // Look for a better solution using known partial solutions...
        for( size_t k=i+1 ; k<j ; ++k ) {
          const Solution
            &sol_ik = solutions[ std::make_pair(i,k) ],
            &sol_kj = solutions[ std::make_pair(k,j) ];
          double cost = sol_ik.cost + sol_kj.cost;
          if( cost < solution.cost ) {
            solution.cost = cost;
            solution.next_index = k;
          }
        }

        // Save solution...
        solutions[ std::make_pair(i,j) ] = solution;
      }
    }

    // Collect points of polyline approximating intrinsic curve...
    std::vector<double> out_s, out_k;
    out_s.reserve(s.size());
    out_k.reserve(k.size());
    size_t cur=0, last=s.size()-1;
    while( cur<last ) {
      // Retrieve arc advancing toward drawing end...
      size_t next = last;
      Solution solution;
      while( (solution=solutions[ std::make_pair(cur,next) ]).next_index != next )
        next = solution.next_index;
      const LinRegResult &arc = linearRegResults[
        std::make_pair(cur,next) ];

      // Save first point if applicable...
      if( cur==0 ) {
        out_s.push_back(0.0);
        out_k.push_back(arc.b);
      }

      // Find where linear function crosses next piece...
      double end;
      if( solution.next_index==last )
        end = s.back();
      else {
        size_t next_next=last;
        Solution next_solution;
        while( (next_solution=solutions[std::make_pair(next,next_next)]).next_index!=next_next )
          next_next = next_solution.next_index;
        const LinRegResult &next_arc =
          linearRegResults[ std::make_pair(next,next_next) ];
        end = ( next_arc.b - arc.b ) / ( arc.a - next_arc.a );
      }

      // Save corresponding point...
      out_s.push_back( end );
      out_k.push_back( arc.a*end + arc.b );

      // Move toward drawing end...
      cur = next;
    }

    // Remove intersection points for which chord-absciss falls before
    // previous point or after next point...
    for( size_t i=1 ; i+1<out_s.size() ; ++i ) {
      if( ( out_s[i]>out_s[i-1] && out_s[i]>out_s[i+1] )
          || ( out_s[i]<out_s[i-1] && out_s[i]<out_s[i+1] ) )
      {
        out_s.erase( out_s.begin()+i );
        out_k.erase( out_k.begin()+i );
        i=0;
      }
    }

    // Flatten nearly horizontal line segments...
    const double min_curvature_slope = 0.0015;
    const double min_curvature = 0.005;
    for( size_t i=0 ; i+1<out_s.size() ; ++i ) {
      if( abs(out_k[i+1]-out_k[i])/(out_s[i+1]-out_s[i]) < min_curvature_slope ) {
        if( abs(out_k[i]) < min_curvature )
          out_k[i] = 0.0;
        out_k[i+1] = out_k[i];
      }
    }

    // Create resulting road...
    odr::Road *road = (*_document)->createRoad();
    if( !road ) return 0;
    odr::Geometry *geometry = road->geometry();
    for( size_t i=0 ; i+1<out_s.size() ; ++i ) {
      if( out_k[i]!=out_k[i+1] ) {
        odr::Spiral *spiral = geometry->createSpiral( out_s[i] );
        spiral->setCurvatureStart( out_k[i] );
        spiral->setCurvatureEnd( out_k[i+1] );
      }
      else if( out_k[i]!=0.0 ) {
        odr::Arc *arc = geometry->createArc( out_s[i] );
        arc->setCurvature( out_k[i] );
      }
      else
        geometry->createLine( out_s[i] );
    }
    road->setLength( out_s.back() );
    geometry->item(0)->setHeading( atan2(y[1]-y[0],x[1]-x[0]) );

    // Compute road and drawing centroid...
    UpdateGeometricPrimitives( geometry );
    double
      drawing_centroid_x=0.0,
      drawing_centroid_y=0.0,
      road_centroid_x=0.0,
      road_centroid_y=0.0;
    for( size_t i=0 ; i<s.size() ; ++i ) {
      drawing_centroid_x += x[i];
      drawing_centroid_y += y[i];

      double xi, yi;
      const odr::GeometricPrimitive *prim = geometry->primitiveAt(s[i]);
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        prim, s[i]-prim->s(), &xi, &yi );
      road_centroid_x += xi;
      road_centroid_y += yi;
    }
    drawing_centroid_x /= (double)s.size();
    drawing_centroid_y /= (double)s.size();
    road_centroid_x    /= (double)s.size();
    road_centroid_y    /= (double)s.size();

    // Compute mean angle between direction from drawing centroid to drawing
    // and direction from road centroid to road...
    double mean_delta_angle = 0.0;
    for( size_t i=0 ; i<s.size() ; ++i ) {
      double delta =
        atan2( y[i]-drawing_centroid_y, x[i]-drawing_centroid_x );

      double xi, yi;
      const odr::GeometricPrimitive *prim = geometry->primitiveAt(s[i]);
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        prim, s[i]-prim->s(), &xi, &yi );
      delta -=
        atan2( yi-road_centroid_y, xi-road_centroid_x );

      while( delta>3.141592654 )  delta -= 2.0 * 3.141592654;
      while( delta<-3.141592654 ) delta += 2.0 * 3.141592654;
      mean_delta_angle += delta;
    }
    mean_delta_angle /= (double)s.size();

    geometry->item(0)->setX(
      drawing_centroid_x
      -cos(mean_delta_angle)*road_centroid_x
      +sin(mean_delta_angle)*road_centroid_y );

    geometry->item(0)->setY(
      drawing_centroid_y
      -cos(mean_delta_angle)*road_centroid_y
      -sin(mean_delta_angle)*road_centroid_x );
    geometry->item(0)->setHeading( geometry->item(0)->heading() + mean_delta_angle );
    UpdateGeometricPrimitives( geometry );

    // Raise events and return result...
    DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
    args->Node = road;
    NodeCreated(this,args);
    DocumentModified(this,args);

    return road;
  }



  //--------------------------------------------------------------------------
  const odr::Road* DomainLogic::RoadThroughPoints(
    Collections::Generic::List<double>^ X,
    Collections::Generic::List<double>^ Y )
  {
    if( !*_document || X->Count!=Y->Count || X->Count<2 )
      return 0;

    odr::Road *road = (*_document)->createRoad();
    if( !road )
      return 0;
    road->setLength( 0.0 );
    odr::Geometry *geometry = road->geometry();
    while( geometry->count()>0 ) delete geometry->item(0);

    // 2 passage points is a trivial case...
    if( X->Count==2 ) {
      odr::Line *line = geometry->createLine(0.0);
      if( !line ) {
        delete road;
        return 0;
      }
      line->setX( X[0] );
      line->setY( Y[0] );
      line->setHeading( atan2( Y[1]-Y[0], X[1]-X[0] ) );
      line->setLength(
        sqrt( asphalt::sqr(Y[1]-Y[0]) + asphalt::sqr(X[1]-X[0]) )  );

      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = road;
      NodeCreated(this,args);
      DocumentModified(this,args);
      return road;
    }

    // For every passage point, find circle passing through the point and its
    // neighbors to compute posture (heading + curvature)...
    Generic::List<double>
      ^C  = gcnew Generic::List<double>(),
      ^H  = gcnew Generic::List<double>();
    C->Add(0.0);
    H->Add(0.0);

    for( int i=1 ; i+1<X->Count ; ++i ) {
      double det =
        (X[i+1]-X[i])*(Y[i-1]-Y[i]) - (Y[i+1]-Y[i])*(X[i-1]-X[i]);

      // If the points are nearly aligned, zero curvature...
      if( fabs(det) < 0.01 ) {
        C->Add(0.0);
        H->Add( atan2( Y[i+1]-Y[i-1], X[i+1]-X[i-1] ) );

        if( i==1 ) {
          C[0] = 0.0;
          H[0] = H[1];
        }
        if( i+2==X->Count ) {
          C->Add( 0.0 );
          H->Add( H[i] );
        }
      }

      // ... else (points not aligned), find circle and compute posture...
      else {
        double
          Pi2   = asphalt::sqr(X[i]) + asphalt::sqr(Y[i]),
          Pim12 = asphalt::sqr(X[i-1]) + asphalt::sqr(Y[i-1]),
          Pip12 = asphalt::sqr(X[i+1]) + asphalt::sqr(Y[i+1]);
        double
          a = 0.5 * ( Pip12 - Pi2 ),
          b = 0.5 * ( Pim12 - Pi2 );
        double
          Xc = ( a*(Y[i-1]-Y[i]) + b*(Y[i]-Y[i+1]) ) / det,
          Yc = ( a*(X[i]-X[i-1]) + b*(X[i+1]-X[i]) ) / det;
        double c = 1.0/sqrt( asphalt::sqr(X[i]-Xc) + asphalt::sqr(Y[i]-Yc) );
        if( det<0.0 ) c*=-1.0;
        C->Add(c);
        if( c>0.0 )
          H->Add( atan2( X[i]-Xc, Yc-Y[i] ) );
        else
          H->Add( atan2( Xc-X[i], Y[i]-Yc ) );

        if( i==1 ) {
          C[0] = c;
          if( c>0.0 )
            H[0] = atan2( X[0]-Xc, Yc-Y[0] );
          else
            H[0] = atan2( Xc-X[0], Y[0]-Yc );
        }
        if( i+2==X->Count ) {
          C->Add(c);
          if( c>0.0 )
            H->Add( atan2( X[i+1]-Xc, Yc-Y[i+1] ) );
          else
            H->Add( atan2( Xc-X[i+1], Y[i+1]-Yc ) );
        }
      }
    }
    srand( GetTickCount() );

    // For every couple of successive postures, join postures with at most
    // three clothoids...
    for( int i=0 ; i+1<X->Count ; ++i ) {
      // Fetch initial posture...
      double
        Xi = X[i],
        Yi = Y[i],
        Hi = H[i],
        Ci = C[i];

      // Fetch final posture...
      double
        Xf = X[i+1],
        Yf = Y[i+1],
        Hf = H[i+1],
        Cf = C[i+1];

      // Let ks be the sign of the curvature slope of the first and last
      // clothoids.
      double ks = 0.0;
      
      // If initial curvature is close to zero, ks is opposed to dot product
      // of initial normal and final tangent...
      if( fabs(Ci)<0.001 ) {
        double temp = sin(Hf)*cos(Hi) - cos(Hf)*sin(Hi);
        if( fabs(temp)>0.01 )
          ks = (temp>0.0) ? -1.0 : 1.0;
      }
      // Else, if final heading is directed outward circle passing through
      // initial posture, first clothoid is inside the circle and ks has the
      // same sign as Ci, opposed otherwise...
      else {
        double temp =
          cos(Hf) * (Xf - Xi + sin(Hi)/Ci) + sin(Hf) * (Yf - Yi - cos(Hi)/Ci);
        if( fabs(temp*Ci)>0.01 )
          ks = (temp*Ci>0.0) ? 1.0 : -1.0;
      }

      // If ks could not yet be determined...
      if( ks==0.0 ) {
        // If final curvature is close to zero, ks is opposed to the dot
        // product of initial tangent and final normal...
        if( fabs(Cf)<0.001 ) {
          double temp = sin(Hi)*cos(Hf) - cos(Hi)*sin(Hf);
          if( fabs(temp)>0.01 )
            ks = (temp>0.0) ? -1.0 : 1.0;
        }
        // Else, if initial heading is directed outward circle passing through
        // final posture, last clothoid is outside the circle and ks has the same
        // sign as Cf, opposed otherwise...
        else {
          double temp =
            cos(Hi) * (Xi - Xf + sin(Hf)/Cf) + sin(Hi) * (Yi - Yf - cos(Hf)/Cf);
          if( fabs(temp*Cf)>0.01 )
            ks = (temp*Cf>0.0) ? 1.0 : -1.0;
        }
      }

      // If initial and final curvature slopes are null, postures can be
      // connected with a constant curvature curve...
      if( ks==0.0 ) {
        if( fabs(Ci)<0.001 ) {
          odr::Line *line = geometry->createLine( road->length() );
          if( !line ) continue;
          line->setX(Xi);
          line->setY(Yi);
          line->setHeading(Hi);
          line->setLength( sqrt(asphalt::sqr(Xf-Xi) + asphalt::sqr(Yf-Yi)) );
        }
        else {
          odr::Arc *arc = geometry->createArc( road->length() );
          if( !arc ) continue;
          arc->setX(Xi);
          arc->setY(Yi);
          arc->setHeading(Hi);
          arc->setCurvature(Ci);
          double deltaH = Hf-Hi;
          while( Ci>0.0 && deltaH<0.0 )
            deltaH += 2.0*M_PI;
          while( Ci<0.0 && deltaH>0.0 )
            deltaH -= 2.0*M_PI;
          arc->setLength( deltaH/Ci );
        }
      }

      // ...else (ks non null), connect postures with 3 clothoids...
      else {
        double l=road->length();
        odr::Spiral
          *spiral1=geometry->createSpiral(l),
          *spiral2=geometry->createSpiral(l+1.0),
          *spiral3=geometry->createSpiral(l+2.0);
        if( !(spiral1 && spiral2 && spiral3) ) {
          delete spiral1;
          delete spiral2;
          delete spiral3;
          road->setLength(l);
          continue;
        }

        // Compute length of arc continuing initial posture and reaching
        // close to final position...
        double arc1 = 0.0;
        if( fabs(Ci)>0.001 ) {
          // Let v1 go from circle center to initial point...
          double
            v1x = sin(Hi)/Ci,
            v1y = -cos(Hi)/Ci;
          // Let v2 go from circle center to ending point...
          double
            v2x = Xf - (Xi-sin(Hi)/Ci),
            v2y = Yf - (Yi+cos(Hi)/Ci);
          // Compte angle between v1 and v2...
          double deltaH = atan2( v1x*v2y - v1y*v2x, v1x*v2x + v1y*v2y );
          // Make deltaH the same sign as Ci...
          while( Ci>0.0 && deltaH<0.0 )
            deltaH += 2.0*M_PI;
          while( Ci<0.0 && deltaH>0.0 )
            deltaH -= 2.0*M_PI;
          // Compute arc length...
          arc1 = deltaH / Ci;
        }
        else
          arc1 = (Xf-Xi)*cos(Hi) + (Yf-Yi)*sin(Hi);

        // Compute length of arc continuing final posture and reaching
        // close to initial position...
        double arc2 = 0.0;
        if( fabs(Cf)>0.001 ) {
          // Let v1 go from circle center to initial point...
          double
            v1x = Xi - (Xf-sin(Hf)/Cf),
            v1y = Yi - (Yf+cos(Hf)/Cf);
          // Let v2 go from cicle center to ending point...
          double
            v2x = sin(Hf)/Cf,
            v2y = -cos(Hf)/Cf;
          // Compute angle between v1 and v2...
          double deltaH = atan2( v1x*v2y - v1y*v2x, v1x*v2x + v1y*v2y );
          // Make deltaH the same signe as Cf...
          while( Cf>0.0 && deltaH<0.0 )
            deltaH += 2.0*M_PI;
          while( Cf<0.0 && deltaH>0.0 )
            deltaH -= 2.0*M_PI;
          // Compute arc length...
          arc2 = deltaH / Cf;
        }
        else
          arc2 = (Xf-Xi)*cos(Hf) + (Yf-Yi)*sin(Hf);

        // A-priori maximum and minimum curvature slopes are chosen...
        const double max_abs_k = 0.05, min_abs_k = 0.0001;

        // Compute minimum and maximum total length of curve...
        double S_min=0.0, S_max=0.0;
        if( Ci*Cf < -0.0001 ) {
          S_min = sqrt( asphalt::sqr(Xf-Xi) + asphalt::sqr(Yf-Yi) );
          S_max = 0.5*(arc1+arc2) + fabs(1.0/Ci) + fabs(1.0/Cf) - sqrt(
            asphalt::sqr( (Xi-sin(Hi)/Ci) - (Xf-sin(Hf)/Cf) ) +
            asphalt::sqr( (Yi+cos(Hi)/Ci) - (Yf+cos(Hf)/Cf) ) );
        }
        else {
          S_min = std::min( arc1, arc2 );
          S_max = std::max( arc1, arc2 );
        }

        // Make an initial guess for S...
        double S=0.5*(S_min+S_max), k=0.0;

        // Golden ratio...
        const double g = 1.5 - 0.5*sqrt(5.0);

        // Set error to safe value...
        double error = std::numeric_limits<double>::infinity();

        // Loop until a good enough solution is found...
        for( int global_count=0 ; global_count<512 && error>0.0005 ;
             ++global_count )
        {
          // For guessed S, use golden section search to find value of k
          // providing minimum overall error...
          {
            // Find 3 consecutive values for k bracketing the minimum error...
            k = ks * std::max( min_abs_k, fabs(Cf-Ci)/S ) / 1.5;
            std::map<double,double> k2error, error2k;
            int count=0;
            do {
              if( ++count>64 ) {
                delete road;
                delete H;
                delete C;
                return 0;
              }

              k *= 1.5;
              error = Evaluate3Spirals(
                Xi, Yi, Hi, Ci, Xf, Yf, Hf, Cf, S, k, spiral1, spiral2, spiral3 );

              if( error!=error )
                continue;

              k2error[k] = error;
              error2k[error] = k;
            }
            while( k2error.size()<3 || error2k.size()<3 ||
                   error2k.begin()->second == k2error.begin()->first ||
                   error2k.begin()->second == k2error.rbegin()->first );

            // Initial bracketing...
            double
              a  = 0.0,
              ea = 0.0,
              b  = error2k.begin()->second,
              eb = error2k.begin()->first,
              c  = 0.0,
              ec = 0.0;
            {
              std::map<double,double>::const_iterator it=k2error.find(b), it2;
              it2 = it;
              --it2;
              a = it2->first;
              ea = it2->second;
              ++it;
              c = it->first;
              ec = it->second;
            }

            // Golden section search...
            for( int count=0 ; count<256 && fabs(a-c)>0.00001 ; ++count ) {
              if( fabs(b-a)>fabs(c-b) )
                k = a + (c-a)*g;
              else
                k = c + (a-c)*g;

              error = Evaluate3Spirals(
                Xi, Yi, Hi, Ci, Xf, Yf, Hf, Cf, S, k, spiral1, spiral2, spiral3 );

              if( ea!=ea || eb!=eb || ec!=ec || error!=error )
                break;

              if( error<eb ) {
                if( k>b ) { a=b; ea=eb; }
                else      { c=b; ec=eb; }
                b=k;
                eb=error;
              }
              else {
                if( k>b ) { c=k; ec=error; }
                else      { a=k; ea=error; }
                k=b;
                error=eb;
              }
            } // found k providing minimum error
          }

          // For guessed k, use golden section search to find value of S
          // providing minimum overall error...
          {
            // Initial bracketing...
            double
              a = S_min + std::max( 0.0, (Cf-Ci)/k ),
              b = S,
              c = S_max;
            double
              ea = Evaluate3Spirals(
                Xi, Yi, Hi, Ci, Xf, Yf, Hf, Cf, a, k, spiral1, spiral2, spiral3 ),
              eb = error,
              ec = Evaluate3Spirals(
                Xi, Yi, Hi, Ci, Xf, Yf, Hf, Cf, c, k, spiral1, spiral2, spiral3 );
            
            // Golden section search...
            for( int count=0 ; count<256 && fabs(a-c)>0.0001 ; ++count ) {
              if( fabs(b-a)>fabs(c-b) )
                S = a + (c-a)*g;
              else
                S = c + (a-c)*g;

              error = Evaluate3Spirals(
                Xi, Yi, Hi, Ci, Xf, Yf, Hf, Cf, S, k, spiral1, spiral2, spiral3 );

              if( ea!=ea || eb!=eb || ec!=ec || error!=error )
                break;

              if( error<eb ) {
                if( S>b ) { a=b; ea=eb; }
                else      { c=b; ec=eb; }
                b=S;
                eb=error;
              }
              else {
                if( S>b ) { c=S; ec=error; }
                else      { a=S; ea=error; }
                S=b;
                error=eb;
              }
            } // found S providing minimum error
          }
        } // found S, k couple providing minimum error

        if( error>0.05 ) {
          delete road;
          delete H;
          delete C;
          return 0;
        }

        Evaluate3Spirals(
          Xi, Yi, Hi, Ci, Xf, Yf, Hf, Cf, S, k,
          spiral1, spiral2, spiral3 );

      } // ks!=0.0
    } // iterate on couple of successive points

    // Clean up...
    delete H;
    delete C;

    // Trigger creation events...
    DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs;
    args->Node = road;
    NodeCreated( this, args );    
    DocumentModified( this, args );

    return road;
  }

  double DomainLogic::Evaluate3Spirals(
    double Xi, double Yi, double Hi, double Ci,
    double Xf, double Yf, double Hf, double Cf,
    double S, double k,
    odr::Spiral *spiral1, odr::Spiral *spiral2, odr::Spiral *spiral3 )
  {
    // Compute rotation between initial and final postures...
    double Hdelta = Hf-Hi;
    while( Hdelta > M_PI )
      Hdelta -= 2.0*M_PI;
    while( Hdelta < -M_PI )
      Hdelta += 2.0*M_PI;
    if( (Cf-Ci)*k > 0.0 ) {
      if( Hdelta<0.0 )
        Hdelta += 2.0*M_PI;
      else
        Hdelta -= 2.0*M_PI;
    }

    // For guessed S, k, compute s1, s2, s3...
    double s2 = 0.5*( S - (Cf-Ci)/k );
    double s1 =
      0.5 * s2
      + (Hdelta-Ci*S) / (S*k+Ci-Cf)
      - 0.25 * asphalt::sqr(S-2.0*s2) / s2;
    double s3 = S - s1 - s2;
    if( s1<0.0 || s2<0.0 || s3<0.0 || s1!=s1 || s2!=s2 || s3!=s3 )
      return std::numeric_limits<double>::quiet_NaN();

    // Apply values...
    double x, y, h;
    spiral1->setX( Xi );
    spiral1->setY( Yi );
    spiral1->setHeading( Hi );
    spiral1->setCurvatureStart( Ci );
    spiral1->setLength( s1 );
    spiral1->setCurvatureEnd( Ci + s1*k );

    Render::getEvaluator()->evaluatePositionOnPrimitive(
      spiral1, s1, &x, &y, &h );
    spiral2->setX( x );
    spiral2->setY( y );
    spiral2->setHeading( h );
    spiral2->setCurvatureStart( Ci + s1*k );
    spiral2->setLength( s2 );
    spiral2->setCurvatureEnd( Ci + (s1-s2)*k );

    Render::getEvaluator()->evaluatePositionOnPrimitive(
      spiral2, s2, &x, &y, &h );
    spiral3->setX( x );
    spiral3->setY( y );
    spiral3->setHeading( h );
    spiral3->setCurvatureStart( Ci + (s1-s2)*k );
    spiral3->setLength( s3 );
    spiral3->setCurvatureEnd( Cf );

    // Compute error...
    Render::getEvaluator()->evaluatePositionOnPrimitive(
      spiral3, s3, &x, &y );
    return sqrt( asphalt::sqr(Xf-x) + asphalt::sqr(Yf-y) );
  }



  //--------------------------------------------------------------------------
  double DomainLogic::ProlongateRoad(
    const odr::Road* constRoad, double Xt, double Yt, bool straightEnd )
  {
    odr::Road* road = const_cast<odr::Road*>(constRoad);
    odr::Geometry* geometry = road->geometry();
    odr::GeometricPrimitive* lastPrim = geometry->primitiveAt(road->length());
    double Xi, Yi, Hi, Ci=0.0;
    Xi = lastPrim->x();
    Yi = lastPrim->y();
    if( geometry->count()==1 )
      lastPrim->setHeading( atan2( Yt-Yi, Xt-Xi ) );
    Hi = lastPrim->heading();

    // Prolongated primitive is a clothoid...
    if( odr::Spiral *spiral = dynamic_cast<odr::Spiral*>(lastPrim) ) {
      Ci = spiral->curvatureStart();

      // If curvature must be zero at end...
      if( straightEnd ) {
        spiral->setCurvatureEnd( 0.0 );

        // If initial curvature is near zero, primitive is actually a line...
        if( fabs(Ci)<0.001 ) {
          spiral->setLength(
            std::max( 0.0, (Xt-Xi)*cos(Hi) + (Yt-Yi)*sin(Hi) )  );
        }

        // ... else (initial curvature is non-null)...
        else {
          // Find smallest positive s causing a full turn...
          double s_max = 4.0 * M_PI / fabs(Ci);

          // Make an initial guess at clothoid length...
          double s = sqrt( asphalt::sqr(Xt-Xi) + asphalt::sqr(Yt-Yi) );

          // Repeat until a solution is found or the maximum loop count is
          // reached...
          int count = 0;
          double error = std::numeric_limits<double>::infinity();
          do {
            // Find final posture for guessed s...
            spiral->setLength(s);
            double Xf, Yf, Hf;
            Render::getEvaluator()->evaluatePositionOnPrimitive(
              spiral, s, &Xf, &Yf, &Hf );
            error = (Xt-Xf)*cos(Hf) + (Yt-Yf)*sin(Hf);

            // Make a new guess based on error...
            s += 0.5*error;
            s = std::min( s_max, std::max( 0.0, s ) );
          }
          while( fabs(error)>0.0001 && ++count<1024 );
        }
      }

      // ... else (free curvature at end)...
      else {
        // Make an initial guess for clothoid length...
        double s = sqrt( asphalt::sqr(Xt-Xi) + asphalt::sqr(Yt-Yi) );

        // Let k be the curvature slope...
        const double max_abs_curvature_slope = 0.01,
                     max_abs_curvature = 0.1;
        double k=0.0;

        // If a 0 curvature slope causes the primitive to nearly be a line...
        if( fabs(Ci)<=0.0005 ) {
          // If target point is on the left of the line, k is positive...
          if( (Yt-Yi)*cos(Hi) - (Xt-Xi)*sin(Hi) > 0.0 )
            k = max_abs_curvature/s;
          // ... else, k is negative...
          else
            k = -max_abs_curvature/s;
        } // constant curvature curve is nearly a line

        // ... else primitive with constant curvature is an arc...
        else {
          // Let Rc be the radius of the circle and (Xc, Yc) be its center...
          double Rc = 1.0/fabs(Ci);
          double Xc = Xi - sin(Hi)/Ci,
                 Yc = Yi + cos(Hi)/Ci;

          // If target point is inside the circle, curvature slope has the same
          // sign as initial curvature...
          if( asphalt::sqr(Xt-Xc)+asphalt::sqr(Yt-Yc) <= Rc*Rc )
            k = (Ci>0.0) ? max_abs_curvature/s : -max_abs_curvature/s;

          // ... else, curvature slope is opposed to initial curvature...
          else
            k = (Ci>0.0) ? -max_abs_curvature/s : max_abs_curvature/s;
        }

        // Search window...
        double k_min=std::min(k,0.0), k_max=std::max(k,0.0);

        // Repeat until a solution for k is found by dichotomy or the maximum
        // number of loops is reached...
        int k_count=0;
        double k_error = std::numeric_limits<double>::infinity();
        do {
          // Find smallest positive s causing a full turn...
          double s_max=std::numeric_limits<double>::infinity();
          {
            double delta = Ci*Ci - 4.0*k*M_PI;
            if( delta>=0.0 ) {
              double sqrt_delta_by_k = sqrt(delta)/k, tmp;
              if( (tmp = sqrt_delta_by_k - Ci/k) > 0.0 )
                s_max = tmp;

              if( (tmp = -Ci/k - sqrt_delta_by_k) > 0.0 )
                s_max = std::min(s_max,tmp);
            }

            delta = Ci*Ci + 4.0*k*M_PI;
            if( delta>=0.0 ) {
              double sqrt_delta_by_k = sqrt(delta)/k, tmp;
              if( (tmp = sqrt_delta_by_k - Ci/k) > 0.0 )
                s_max = std::min(s_max,tmp);

              if( (tmp = -Ci/k - sqrt_delta_by_k) > 0.0 )
                s_max = std::min(s_max,tmp);
            }
          }

          // Repeat until a solution for s is found by distance estimation or
          // the maximum number of loops is reached...
          int s_count=0;
          double s_error=std::numeric_limits<double>::infinity();
          do {
            // Compute final posture for guessed (k,s)...
            spiral->setCurvatureEnd( std::max(
              -max_abs_curvature, std::min(
              max_abs_curvature,
              Ci + k*s ) ) );
            spiral->setLength( s );
            double Xf, Yf, Hf;
            Render::getEvaluator()->evaluatePositionOnPrimitive(
              spiral, s, &Xf, &Yf, &Hf );

            // Compute s error...
            s_error = cos(Hf)*(Xt-Xf) + sin(Hf)*(Yt-Yf);

            // Make a new guess for s...
            s += 0.5*s_error;
            s = std::min( std::max( s, 0.0 ), s_max );
          }
          while( fabs(s_error)>0.0001 && ++s_count<128 );

          // Compute final posture for guessed (k,s)...
          spiral->setCurvatureEnd( Ci + k*s );
          spiral->setLength( s );
          double Xf, Yf, Hf;
          Render::getEvaluator()->evaluatePositionOnPrimitive(
            spiral, s, &Xf, &Yf, &Hf );

          // Compute error on k...
          k_error = cos(Hf)*(Yt-Yf)-sin(Hf)*(Xt-Xf);

          // Make a new guess for k...
          if( k_error>0.0 ) {
            if( k==k_max ) k_max*=2.0;
            k_min = k;
          }
          else {
            if( k==k_min ) k_min*=2.0;
            k_max = k;
          }
          k = 0.5*(k_min+k_max);
        }
        while( fabs(k_error) > 0.0001 && ++k_count<128 );
      }

      // Trigger modification events...
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = spiral;
      DocumentModified(this,args);
      NodeModified(this,args);

      // Compute and return error...
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        spiral, spiral->length(), &Xi, &Yi, &Hi );
      return sqrt( asphalt::sqr(Xt-Xi) + asphalt::sqr(Yt-Yi) );
    }

    // Prolongated primitive is an arc...
    else if( odr::Arc *arc = dynamic_cast<odr::Arc*>(lastPrim) ) {
      Ci = arc->curvature();

      // Let Rc be the circle radius and (Xc,Yc) its center...
      double Rc = 1.0/fabs(Ci);
      double Xc = Xi - sin(Hi)/Ci,
             Yc = Yi + cos(Hi)/Ci;

      // Deduce final heading...
      double Hf =
        atan2( Yt-Yc, Xt-Xc )
        + ( Ci>0.0 ? 1.0 : -1.0 ) * 0.5*M_PI;
      double s = ( Hf - Hi ) / Ci;
      while( s < 0.0 )
        s += 2.0*M_PI*Rc;
      while( s > 2.0*M_PI*Rc )
        s -= 2.0*M_PI*Rc;
      arc->setLength( s );

      // Trigger modification events...
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = spiral;
      DocumentModified(this,args);
      NodeModified(this,args);

      /// \todo compute error
      return 0.0;
    }

    // Prolongated primitive is a line segment...
    else if( odr::Line *line = dynamic_cast<odr::Line*>(lastPrim) ) {
      Ci = 0.0;

      // Just solve arc length...
      line->setLength( cos(Hi)*(Xt-Xi) + sin(Hi)*(Yt-Yi) );
      
      // Trigger modification events...
      DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
      args->Node = line;
      DocumentModified(this,args);
      NodeModified(this,args);

      return fabs( cos(Hi)*(Yt-Yi) - sin(Hi)*(Xt-Xi) );
    }

    // Prolongated primitive is a poly3...
    else if( const odr::Poly3 *tmpPoly = dynamic_cast<odr::Poly3*>(lastPrim) )
    {
      /*
      double
        a = tmpPoly->a(),
        b = tmpPoly->b(),
        c = tmpPoly->c(),
        d = tmpPoly->d();
      double s = asphalt::poly3length2param(a,b,c,d,tmpPoly->length());
      double
        dt  = b + 2.0*c*s + 3.0*d*s*s,
        d2t = 2.0*c + 6.0*d*s;
      */
      /// \todo Manage case of poly3
      return std::numeric_limits<double>::infinity();
    }

    return std::numeric_limits<double>::infinity();
  }



  //--------------------------------------------------------------------------
  struct ConnectionResult {
    double s1, s2, s3, k;                   //!< Caracteristics of spirals
    double errorX, errorY, errorH, errorC;  //!< Error on posture components
    double error;                           //!< Norm of posture error vector
  };

  const odr::Road* DomainLogic::ConnectRoads(
    const odr::Road* aRoad, bool aEnd, Generic::List<int> ^aLanes,
    const odr::Road* bRoad, bool bEnd, Generic::List<int> ^bLanes )
  {
    if( !Document || !aLanes || !bLanes || aLanes->Count!=bLanes->Count )
      return 0;

    // Chose center lane so that no connection lane crosses center lane...
    int aLane=0, bLane=0;
    if( aLanes->Count>0 ) {
      int a, b;
      for( int i=0 ; i<aLanes->Count ; ++i ) {
        if( aLanes[i] ) {
          a = aLanes[i];
          if( aEnd ) { if( a>0 ) --a; }
          else /* !aEnd */ if( a<0 ) ++a;
          break;
        }
      }

      for( int i=0 ; i<bLanes->Count ; ++i ) {
        if( bLanes[i]!=0 ) {
          b = bLanes[i];
          if( bEnd ) { if( b<0 ) ++b; }
          else /* !bEnd */ if( b>0 ) --b;
          break;
        }
      }

      aLane = a;
      bLane = b;

      for( int i=0 ; i<aLanes->Count ; ++i ) {
        if( aLanes[i] && bLanes[i] ) {
          if( aEnd ) ++a;
          else       --a;
          if( bEnd ) --b;
          else       ++b;
        }
        else if( aLanes[i] ) {
          if( aEnd ) ++a;
          else       --a;
        }
        else {
          if( bEnd ) --b;
          else       ++b;
        }
        if( abs(a)+abs(b) < abs(aLane)+abs(bLane) ) {
          aLane = a;
          bLane = b;
        }
      }
    }

    // Retrieve start and end postures...
    double
      startX, startY, startH, startC, // Starting position, heading, curvature
      endX, endY, endH, endC;         // Ending position, heading curvature

    if( aEnd ) {
      double x, y, pos[3];
      const odr::GeometricPrimitive* prim =
        aRoad->geometry()->primitiveAt(aRoad->length());
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        prim, prim->length(), &x, &y, &startH );
      Render::getEvaluator()->evaluatePositionOnRoad(
        aRoad, aRoad->length(), aLane, 0.0, pos );
      startX = pos[0];
      startY = pos[1];
      switch( prim->type() ) {
      case odr::GeometricPrimitive::T_LINE:
        startC = 0.0;
        break;
      case odr::GeometricPrimitive::T_ARC:
        startC = dynamic_cast<const odr::Arc*>(prim)->curvature();
        break;
      case odr::GeometricPrimitive::T_SPIRAL:
        startC = dynamic_cast<const odr::Spiral*>(prim)->curvatureEnd();
        break;
      }
      startC = 1.0 /
        ( 1.0/startC - sin(startH)*(startX-x) + cos(startH)*(startY-y) );
    }
    else {
      double x, y, pos[3];
      const odr::GeometricPrimitive* prim =
        aRoad->geometry()->primitiveAt(0.0);
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        prim, 0.0, &x, &y, &startH );
      Render::getEvaluator()->evaluatePositionOnRoad(
        aRoad, 0.0, aLane, 0.0, pos );
      startX = pos[0];
      startY = pos[1];
      startH = fmod( startH+M_PI, M_PI*2.0 );
      switch( prim->type() ) {
      case odr::GeometricPrimitive::T_LINE:
        startC = 0.0;
        break;
      case odr::GeometricPrimitive::T_ARC:
        startC = -dynamic_cast<const odr::Arc*>(prim)->curvature();
        break;
      case odr::GeometricPrimitive::T_SPIRAL:
        startC = -dynamic_cast<const odr::Spiral*>(prim)->curvatureStart();
        break;
      }
      startC = 1.0 /
        ( 1.0/startC - sin(startH)*(startX-x) + cos(startH)*(startY-y) );
    }

    if( bEnd ) {
      double x, y, pos[3];
      const odr::GeometricPrimitive* prim =
        bRoad->geometry()->primitiveAt(bRoad->length());
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        prim, prim->length(), &x, &y, &endH );
      Render::getEvaluator()->evaluatePositionOnRoad(
        bRoad, bRoad->length(), bLane, 0.0, pos );
      endX = pos[0];
      endY = pos[1];
      endH = fmod( endH+M_PI, M_PI*2.0 );
      switch( prim->type() ) {
      case odr::GeometricPrimitive::T_LINE:
        endC = 0.0;
        break;
      case odr::GeometricPrimitive::T_ARC:
        endC = -dynamic_cast<const odr::Arc*>(prim)->curvature();
        break;
      case odr::GeometricPrimitive::T_SPIRAL:
        endC = -dynamic_cast<const odr::Spiral*>(prim)->curvatureEnd();
        break;
      }
      endC = 1.0 / ( 1.0/endC - sin(endH)*(endX-x) + cos(endH)*(endY-y) );
    }
    else {
      double x, y, pos[3];
      const odr::GeometricPrimitive* prim =
        bRoad->geometry()->primitiveAt(0.0);
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        prim, 0.0, &x, &y, &endH );
      Render::getEvaluator()->evaluatePositionOnRoad(
        bRoad, 0.0, bLane, 0.0, pos );
      endX = pos[0];
      endY = pos[1];
      switch( prim->type() ) {
      case odr::GeometricPrimitive::T_LINE:
        endC = 0.0;
        break;
      case odr::GeometricPrimitive::T_ARC:
        endC = dynamic_cast<const odr::Arc*>(prim)->curvature();
        break;
      case odr::GeometricPrimitive::T_SPIRAL:
        endC = dynamic_cast<const odr::Spiral*>(prim)->curvatureStart();
        break;
      }
      endC = 1.0 / ( 1.0/endC - sin(endH)*(endX-x) + cos(endH)*(endY-y) );
    }

    // Create road...
    odr::Road* road = const_cast<odr::Road*>( (*_document)->createRoad() );
    odr::Geometry* geometry = road->geometry();
    delete geometry->item(0);
    road->setLength(0.0);

    // Create connection primitives...
    bool result = ConnectPosturesInRoad(
      road, startX, startY, startH, startC, endX, endY, endH, endC );
    if( !result ) {
      delete road;
      return 0;
    }

    // Rebuild geometry...
    UpdateGeometricPrimitives( geometry );

    // Collect lane widths at connection ends...
    std::map<int,double> aWidths, bWidths;
    aWidths[0] = 0.0;
    bWidths[0] = 0.0;
    if( aEnd ) {
      const odr::LaneSection* section =
        aRoad->lanes()->sectionAt( aRoad->length() );
      double t = section->length();
      misc::Enumerator<const odr::Lane*> *laneEnum = section->enumerate();
      const odr::Lane* lane = 0;
      while( laneEnum && (lane=laneEnum->item()) ) {
        laneEnum->moveNext();
        const odr::LaneWidth* width = lane->widthAt( section->length() );
        aWidths[ lane->index() ] =
          width->a() + width->b()*t + width->c()*t*t + width->d()*t*t*t;
      }
      delete laneEnum;
    }
    else {
      const odr::LaneSection* section =
        aRoad->lanes()->sectionAt(0.0);
      misc::Enumerator<const odr::Lane*> *laneEnum = section->enumerate();
      const odr::Lane* lane = 0;
      while( laneEnum && (lane=laneEnum->item()) ) {
        laneEnum->moveNext();
        aWidths[ lane->index() ] = lane->widthAt(0.0)->a();
      }
      delete laneEnum;
    }

    if( bEnd ) {
      const odr::LaneSection* section =
        bRoad->lanes()->sectionAt( bRoad->length() );
      double t = section->length();
      misc::Enumerator<const odr::Lane*> *laneEnum = section->enumerate();
      const odr::Lane* lane = 0;
      while( laneEnum && (lane=laneEnum->item()) ) {
        laneEnum->moveNext();
        const odr::LaneWidth* width = lane->widthAt( section->length() );
        bWidths[ lane->index() ] =
          width->a() + width->b()*t + width->c()*t*t + width->d()*t*t*t;
      }
      delete laneEnum;
    }
    else {
      const odr::LaneSection* section =
        bRoad->lanes()->sectionAt(0.0);
      misc::Enumerator<const odr::Lane*> *laneEnum = section->enumerate();
      const odr::Lane* lane = 0;
      while( laneEnum && (lane=laneEnum->item()) ) {
        laneEnum->moveNext();
        bWidths[ lane->index() ] = lane->widthAt(0.0)->a();
      }
      delete laneEnum;
    }

    // Create connection lanes...
    double l = road->length();
    odr::LaneSection *section = road->lanes()->item(0);
    for( int i=0 ; i<aLanes->Count ; ++i ) {
      bool right = ( aLanes[i]==0 || ( aEnd ? aLanes[i]<aLane+(aLane>0?1:0) : aLanes[i]>aLane-(aLane<0?1:0) ) )
                   && ( bLanes[i]==0 || ( bEnd ? bLanes[i]>bLane-(bLane<0?1:0) : bLanes[i]<bLane+(bLane>0?1:0) ) );
      odr::Lane *lane = section->createLane(
        right ? -1 : (int)section->countLeft()+1 );
      if( !lane ) continue;
      odr::LaneWidth *width = lane->createWidth(0.0);
      if( !width ) continue;
      double aWidth = aWidths[aLanes[i]], bWidth = bWidths[bLanes[i]];
      width->setA( aWidths[aLanes[i]] );
      width->setB( 0.0 );
      width->setC( 3.0 * ( bWidth - aWidth ) / (l*l) );
      width->setD( 2.0 * ( aWidth - bWidth ) / (l*l*l) );
    }

    // Connect elevation profiles...
    {
      double elevStart=0.0, elevEnd=0.0, elevStartDeriv=0.0, elevEndDeriv=0.0;
      if( aEnd ) {
        double s = aRoad->length();
        const odr::Elevation *elev =
          aRoad->elevationProfile()->elevationAt( s );
        if( elev ) {
          elevStart =
            elev->a() + s*elev->b() + s*s*elev->c() + s*s*s*elev->d();
          elevStartDeriv = elev->b() + 2.0*s*elev->c() + 3.0*s*s*elev->d();
        }
      }
      else {
        const odr::Elevation *elev =
          aRoad->elevationProfile()->elevationAt( 0.0 );
        if( elev ) {
          elevStart = elev->a();
          elevStartDeriv = -elev->b();
        }
      }

      if( bEnd ) {
        double s = bRoad->length();
        const odr::Elevation *elev =
          bRoad->elevationProfile()->elevationAt( s );
        if( elev ) {
          elevEnd =
            elev->a() + s*elev->b() + s*s*elev->c() + s*s*s*elev->d();
          elevEndDeriv = -elev->b() - 2.0*s*elev->c() - 3.0*s*s*elev->d();
        }
      }
      else {
        const odr::Elevation *elev =
          bRoad->elevationProfile()->elevationAt( 0.0 );
        if( elev ) {
          elevEnd = elev->a();
          elevEndDeriv = elev->b();
        }
      }

      double
        l = road->length(),
        a = elevStart,
        b = elevStartDeriv;
      double
        c = ( 3.0*elevEnd - 3.0*a - 2.0*b*l - l*elevEndDeriv ) / (l*l),
        d = ( 2.0*a + b*l + l*elevEndDeriv - 2.0*elevEnd ) / (l*l*l);

      odr::Elevation *elev = road->elevationProfile()->createRecord(0.0);
      if( elev ) {
        elev->setA( a );
        elev->setB( b );
        elev->setC( c );
        elev->setD( d );
      }
    }

    // Connect crossfalls...
    {
      double crossStart=0.0, crossEnd=0.0, crossStartDeriv=0.0, crossEndDeriv=0.0;
      if( aEnd ) {
        double s = aRoad->length();
        const odr::Crossfall *cross =
          aRoad->lateralProfile()->crossfallAt( s );
        if( cross ) {
          crossStart =
            cross->a() + s*cross->b() + s*s*cross->c() + s*s*s*cross->d();
          crossStartDeriv = cross->b() + 2.0*s*cross->c() + 3.0*s*s*cross->d();
        }
      }
      else {
        const odr::Crossfall *cross =
          aRoad->lateralProfile()->crossfallAt( 0.0 );
        if( cross ) {
          crossStart = cross->a();
          crossStartDeriv = -cross->b();
        }
      }

      if( bEnd ) {
        double s = bRoad->length();
        const odr::Crossfall *cross =
          bRoad->lateralProfile()->crossfallAt( s );
        if( cross ) {
          crossEnd =
            cross->a() + s*cross->b() + s*s*cross->c() + s*s*s*cross->d();
          crossEndDeriv = -cross->b() - 2.0*s*cross->c() - 3.0*s*s*cross->d();
        }
      }
      else {
        const odr::Crossfall *cross =
          bRoad->lateralProfile()->crossfallAt( 0.0 );
        if( cross ) {
          crossEnd = cross->a();
          crossEndDeriv = cross->b();
        }
      }

      double
        l = road->length(),
        a = crossStart,
        b = crossStartDeriv;
      double
        c = ( 3.0*crossEnd - 3.0*a - 2.0*b*l - l*crossEndDeriv ) / (l*l),
        d = ( 2.0*a + b*l + l*crossEndDeriv - 2.0*crossEnd ) / (l*l*l);

      odr::Crossfall *cross = road->lateralProfile()->createCrossfall(0.0);
      if( cross ) {
        cross->setA( a );
        cross->setB( b );
        cross->setC( c );
        cross->setD( d );
      }
    }

    // Connect superelevations...
    {
      double superStart=0.0, superEnd=0.0, superStartDeriv=0.0, superEndDeriv=0.0;
      if( aEnd ) {
        double s = aRoad->length();
        const odr::Superelevation *super =
          aRoad->lateralProfile()->superelevationAt( s );
        if( super ) {
          superStart =
            super->a() + s*super->b() + s*s*super->c() + s*s*s*super->d();
          superStartDeriv = super->b() + 2.0*s*super->c() + 3.0*s*s*super->d();
        }
      }
      else {
        const odr::Superelevation *super =
          aRoad->lateralProfile()->superelevationAt( 0.0 );
        if( super ) {
          superStart = super->a();
          superStartDeriv = -super->b();
        }
      }

      if( bEnd ) {
        double s = bRoad->length();
        const odr::Superelevation *super =
          bRoad->lateralProfile()->superelevationAt( s );
        if( super ) {
          superEnd =
            super->a() + s*super->b() + s*s*super->c() + s*s*s*super->d();
          superEndDeriv = -super->b() - 2.0*s*super->c() - 3.0*s*s*super->d();
        }
      }
      else {
        const odr::Superelevation *super =
          bRoad->lateralProfile()->superelevationAt( 0.0 );
        if( super ) {
          superEnd = super->a();
          superEndDeriv = super->b();
        }
      }

      double
        l = road->length(),
        a = superStart,
        b = superStartDeriv;
      double
        c = ( 3.0*superEnd - 3.0*a - 2.0*b*l - l*superEndDeriv ) / (l*l),
        d = ( 2.0*a + b*l + l*superEndDeriv - 2.0*superEnd ) / (l*l*l);

      odr::Superelevation *super = road->lateralProfile()->createSuperelevation(0.0);
      if( super ) {
        super->setA( a );
        super->setB( b );
        super->setC( c );
        super->setD( d );
      }
    }

    // Trigger creation event...
    DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
    args->Node = road;
    NodeCreated( this, args );
    DocumentModified( this, args );

    return road;
  }

  bool DomainLogic::ConnectPosturesInRoad(
    odr::Road* road,
    double initialX, double initialY, double initialH, double initialC,
    double finalX, double finalY, double finalH, double finalC )
  {
    odr::Geometry *geometry = road->geometry();

    const double max_curvature_slope = 0.01;

    // Create spiral to reach null curvature from starting posture ; use it
    // to compute null-curvature posture near start...
    double afterStartX, afterStartY, afterStartH;
    if( initialC!=0.0 ) {
      odr::Spiral *tmpSpiral = geometry->createSpiral(road->length());
      tmpSpiral->setX( initialX );
      tmpSpiral->setY( initialY );
      tmpSpiral->setHeading( initialH );
      tmpSpiral->setCurvatureStart( initialC );
      tmpSpiral->setCurvatureEnd( 0.0 );
      tmpSpiral->setLength( abs(initialC/max_curvature_slope) );
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        tmpSpiral, tmpSpiral->length(),
        &afterStartX, &afterStartY, &afterStartH );
    }
    else {
      afterStartX = initialX;
      afterStartY = initialY;
      afterStartH = initialH;
    }

    // Create temporary road going backward from ending posture and reaching
    // null curvature ; use it to compute null-curvature posture near end...
    double beforeEndX, beforeEndY, beforeEndH;
    if( finalC!=0.0 )
    {
      odr::Road *tmpRoad = (*_document)->createRoad();
      odr::Geometry *tmpGeometry = tmpRoad->geometry();
      delete tmpGeometry->item(0);
      odr::Spiral *tmpSpiral = tmpGeometry->createSpiral(0.0);
      tmpSpiral->setCurvatureStart( -finalC );
      tmpSpiral->setCurvatureEnd( 0.0 );
      tmpSpiral->setX( finalX );
      tmpSpiral->setY( finalY );
      tmpSpiral->setHeading(
        fmod(finalH+M_PI, 2.0*M_PI) );
      tmpSpiral->setLength( abs(finalC/max_curvature_slope) );
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        tmpSpiral, tmpSpiral->length(),
        &beforeEndX, &beforeEndY, &beforeEndH );
      beforeEndH += M_PI;
      while( beforeEndH > M_PI )
        beforeEndH -= 2.0*M_PI;
      delete tmpRoad;
    }
    else {
      beforeEndX = finalX;
      beforeEndY = finalY;
      beforeEndH = finalH;
    }

    // Find intersection of lines segments continuing null-curvature postures
    // near ends...
    double
      afterStartDX = cos(afterStartH),
      afterStartDY = sin(afterStartH),
      beforeEndDX  = -cos(beforeEndH),
      beforeEndDY  = -sin(beforeEndH);

    double ttiStart, ttiEnd;

    {
      double det = afterStartDY*beforeEndDX - afterStartDX*beforeEndDY;

      // If lines are parallel...
      if( abs(det)<0.001 ) {
        // If postures are aligned...
        if( fabs( (beforeEndX-afterStartX)*afterStartDY
                  - (beforeEndY-afterStartY)*afterStartDX ) < 0.001 )
        {
          // Fail if postures go in opposite directions...
          if( afterStartDX*beforeEndDX+afterStartDY*beforeEndDY+0.001 > 0.0 )
            return false;

          // Fail if target position is not in starting direction...
          if( (beforeEndX-afterStartX)*afterStartDX
              + (beforeEndY-afterStartY)*afterStartDY < 0.001 )
            return false;

          // Else, connect postures with a line segment...
          odr::Line *line = geometry->createLine( road->length() );
          line->setX( afterStartX );
          line->setY( afterStartY );
          line->setHeading( afterStartH );
          line->setLength( sqrt(
            asphalt::math<double>::sqr(beforeEndX-afterStartX)
            + asphalt::math<double>::sqr(beforeEndY-afterStartY) ) );

          // Create final clothoid to reach target curvature...
          if( finalC!=0.0 ) {
            odr::Spiral *tmpSpiral = geometry->createSpiral( road->length() );
            tmpSpiral->setCurvatureStart( 0.0 );
            tmpSpiral->setCurvatureEnd( finalC );
            tmpSpiral->setLength( abs(finalC/max_curvature_slope) );
          }

          return true;
        }

        // Else (postures not aligned), use recursion to connect postures...
        ttiStart = ttiEnd = -1.0;
      }
      else {
        ttiStart = ( (beforeEndY-afterStartY)*beforeEndDX
                     - (beforeEndX-afterStartX)*beforeEndDY ) / det;
        ttiEnd = ( (beforeEndY-afterStartY)*afterStartDX
                   - (beforeEndX-afterStartX)*afterStartDY ) / det;
      }
    }

    // If line segments don't intersect...
    if( ttiStart<0.0 || ttiEnd<0.0 ) {
      // Use cubic spline to compute intermediate posture...
      double interX, interY, interH;
      double mult =
        2.0*sqrt(
          asphalt::math<double>::sqr(beforeEndX-afterStartX)
          + asphalt::math<double>::sqr(beforeEndY-afterStartY) ) /*/ sqrt(2.0)*/;
      afterStartDX *= mult;
      afterStartDY *= mult;
      beforeEndDX  *= -mult;
      beforeEndDY  *= -mult;
      double xd = afterStartX, yd = afterStartY;
      double xc = afterStartDX, yc = afterStartDY;
      double xb = 3.0*beforeEndX - beforeEndDX - 2.0*xc - 3.0*xd,
             yb = 3.0*beforeEndY - beforeEndDY - 2.0*yc - 3.0*yd;
      double xa = beforeEndX - xb - xc - xd,
             ya = beforeEndY - yb - yc - yd;
      interX = xa*0.125 + xb*0.25 + xc*0.5 + xd;
      interY = ya*0.125 + yb*0.25 + yc*0.5 + yd;
      interH = atan2( 0.75*ya + yb + yc, 0.75*xa + xb + xc );
   
      // Use recursion to connect postures...
      if( !ConnectPosturesInRoad(
            road,
            afterStartX, afterStartY, afterStartH, 0.0,
            interX, interY, interH, 0.0 ) )
        return false;

      if( !ConnectPosturesInRoad(
            road,
            interX, interY, interH, 0.0,
            beforeEndX, beforeEndY, beforeEndH, 0.0 ) )
        return false;

      // Create final clothoid to target curvature...
      if( finalC!=0.0 ) {
        odr::Spiral *tmpSpiral = geometry->createSpiral( road->length() );
        tmpSpiral->setCurvatureStart( 0.0 );
        tmpSpiral->setCurvatureEnd( finalC );
        tmpSpiral->setLength( abs(finalC/max_curvature_slope) );
      }
      return true;
    }

    // Line segments intersect, postures can be connected with 2 symetric
    // clothoids and a line segment...

    // If intersection point is closer to target posture than initial posture,
    // Insert line segment...
    if( ttiStart-ttiEnd>0.001 ) {
      odr::Line *line = geometry->createLine(road->length());
      line->setLength( ttiStart-ttiEnd );
      line->setX( afterStartX );
      line->setY( afterStartY );
      line->setHeading( afterStartH );
    }

    // Intersection point...
    double
      interX = afterStartX + ttiStart*afterStartDX,
      interY = afterStartY + ttiStart*afterStartDY;

    // Heading change...
    double deltaH = beforeEndH - afterStartH;
    while( deltaH > M_PI )
      deltaH -= 2.0*M_PI;
    while( deltaH < -M_PI )
      deltaH += 2.0*M_PI;
    
    // Direction tangent to bisector of angle between the two line segments...
    double
      tangentDX = cos( afterStartH + 0.5*deltaH ),
      tangentDY = sin( afterStartH + 0.5*deltaH );

    // First clothoid...
    odr::Spiral *firstSpiral = geometry->createSpiral( road->length() );
    firstSpiral->setX(
      afterStartX + afterStartDX*std::max( 0.0, ttiStart-ttiEnd ) );
    firstSpiral->setY(
      afterStartY + afterStartDY*std::max( 0.0, ttiStart-ttiEnd ) );
    firstSpiral->setHeading( afterStartH );
    firstSpiral->setCurvatureStart(0.0);
    UpdateGeometricPrimitives(geometry);

    // Find clothoid length so that its end lie on the bisector...
    {
      double lengthGuess = std::min( ttiStart, ttiEnd );
      double error=0.0;
      do {
        firstSpiral->setCurvatureEnd( deltaH/lengthGuess );
        firstSpiral->setLength( lengthGuess );
        double x, y;
        Render::getEvaluator()->evaluatePositionOnPrimitive(
          firstSpiral, lengthGuess, &x, &y );
        error = (x-interX)*tangentDX + (y-interY)*tangentDY;
        lengthGuess -= 0.8*error;
      }
      while( fabs(error)>0.001 );
    }

    // Second clothoid...
    odr::Spiral *secondSpiral = geometry->createSpiral( road->length() );
    secondSpiral->setCurvatureStart( firstSpiral->curvatureEnd() );
    secondSpiral->setCurvatureEnd( 0.0 );
    secondSpiral->setLength( firstSpiral->length() );

    // If intersection point is closer to initial posture, add line segment...
    if( ttiEnd-ttiStart>0.001 ) {
      odr::Line *line = geometry->createLine(road->length());
      line->setLength( ttiEnd-ttiStart );
    }

    // Create final clothoid to reach target curvature...
    if( finalC!=0.0 ) {
      odr::Spiral *tmpSpiral = geometry->createSpiral( road->length() );
      tmpSpiral->setCurvatureStart( 0.0 );
      tmpSpiral->setCurvatureEnd( finalC );
      tmpSpiral->setLength( abs(finalC/max_curvature_slope) );
    }

    return true;
  }



  //--------------------------------------------------------------------------
  bool DomainLogic::SplitRoad( const odr::Road* constRoad, double s )
  {
    if( !*_document || !constRoad || s<0.01 || s>constRoad->length()-0.01 )
      return false;

    odr::Road
      *road  = const_cast<odr::Road*>(constRoad),
      *road2 = (*_document)->createRoad();
    if( !road2 ) return false;

    std::list<misc::WeakRefServer*> toDelete;

    // Manage road geometry...
    odr::GeometricPrimitive *primAtCut =
      const_cast<odr::GeometricPrimitive*>(road->geometry()->primitiveAt(s));

    double xCut, yCut, hCut;
    Render::getEvaluator()->evaluatePositionOnPrimitive(
      primAtCut, s-primAtCut->s(), &xCut, &yCut, &hCut );

    if( const odr::Line* line = dynamic_cast<const odr::Line*>(primAtCut) ) {
      odr::Line* newLine = road2->geometry()->createLine( 0.0 );
      newLine->setHeading( hCut );
      newLine->setX( xCut );
      newLine->setY( yCut );
      newLine->setLength( line->s()+line->length()-s );
    }
    else if( const odr::Arc* arc = dynamic_cast<const odr::Arc*>(primAtCut) ) {
      odr::Arc* newArc = road2->geometry()->createArc( 0.0 );
      newArc->setHeading( hCut );
      newArc->setX( xCut );
      newArc->setY( yCut );
      newArc->setCurvature( arc->curvature() );
      newArc->setLength( arc->s()+arc->length()-s );
    }
    else if( odr::Spiral* spiral = dynamic_cast<odr::Spiral*>(primAtCut) )
    {
      odr::Spiral* newSpiral = road2->geometry()->createSpiral( 0.0 );
      newSpiral->setHeading( hCut );
      newSpiral->setX( xCut );
      newSpiral->setY( yCut );
      newSpiral->setCurvatureStart(
        spiral->curvatureStart()
        + ( spiral->curvatureEnd() - spiral->curvatureStart() )
          * ( s - spiral->s() ) / spiral->length()  );
      newSpiral->setCurvatureEnd( spiral->curvatureEnd() );
      newSpiral->setLength( spiral->s()+spiral->length()-s );
      spiral->setCurvatureEnd( newSpiral->curvatureStart() );
    }
    else if( const odr::Poly3* poly3 =
      dynamic_cast<odr::Poly3*>(primAtCut) )
    {
      odr::Poly3* newPoly3 = road2->geometry()->createPoly3( 0.0 );
      double a=poly3->a(), b=poly3->b(), c=poly3->c(), d=poly3->d();
      double t = asphalt::poly3length2param( a, b, c, d, s - poly3->s() );
      newPoly3->setX( poly3->x() + t*cos(poly3->heading()) );
      newPoly3->setY( poly3->y() + t*sin(poly3->heading()) );
      newPoly3->setA( a + b*t + c*t*t + d*t*t*t );
      newPoly3->setB( b + 2.0*c*t + 3.0*d*t*t );
      newPoly3->setC( c + 3.0*d*t );
      newPoly3->setD( d );
      newPoly3->setLength( poly3->s()+poly3->length()-s );
    }

    misc::Enumerator<odr::GeometricPrimitive*> *primEnum =
      road->geometry()->enumerate();
    odr::GeometricPrimitive* prim=0;
    while( primEnum && (prim=primEnum->item()) ) {
      primEnum->moveNext();

      if( prim->s()<=s ) continue;

      toDelete.push_back(prim);

      if( odr::Line* line=dynamic_cast<odr::Line*>(prim) ) {
        odr::Line *newLine = road2->geometry()->createLine(road2->length());
        newLine->setX( line->x() );
        newLine->setY( line->y() );
        newLine->setHeading( line->heading() );
        newLine->setLength( line->length() );
      }
      else if( odr::Arc* arc=dynamic_cast<odr::Arc*>(prim) ) {
        odr::Arc *newArc = road2->geometry()->createArc(road2->length());
        newArc->setX( arc->x() );
        newArc->setY( arc->y() );
        newArc->setHeading( arc->heading() );
        newArc->setCurvature( arc->curvature() );
        newArc->setLength( arc->length() );
      }
      else if( odr::Spiral* spiral=dynamic_cast<odr::Spiral*>(prim) ) {
        odr::Spiral *newSpiral =
          road2->geometry()->createSpiral(road2->length());
        newSpiral->setX( spiral->x() );
        newSpiral->setY( spiral->y() );
        newSpiral->setHeading( spiral->heading() );
        newSpiral->setCurvatureStart( spiral->curvatureStart() );
        newSpiral->setCurvatureEnd( spiral->curvatureEnd() );
        newSpiral->setLength( spiral->length() );
      }
      else if( odr::Poly3* poly3=dynamic_cast<odr::Poly3*>(prim) ) {
        odr::Poly3 *newPoly3 =
          road2->geometry()->createPoly3(road2->length());
        newPoly3->setX( poly3->x() );
        newPoly3->setY( poly3->y() );
        newPoly3->setHeading( poly3->heading() );
        newPoly3->setA( poly3->a() );
        newPoly3->setB( poly3->b() );
        newPoly3->setC( poly3->c() );
        newPoly3->setD( poly3->d() );
        newPoly3->setLength( poly3->length() );
      }
    }
    delete primEnum;

    road->setLength( s );

    // Manage road lanes at cut...
    odr::LaneSection
      *sectionAtCut = road->lanes()->sectionAt(s),
      *newSection   = road2->lanes()->item(0);
    for( int i=-(int)sectionAtCut->countRight() ;
         i<=(int)sectionAtCut->countLeft() ; ++i )
    {
      if( i==0 ) continue;

      odr::Lane
        *origLane = sectionAtCut->item(i),
        *newLane  = newSection->createLane( i<0 ? -1 : i );

      if( !newLane ) continue;

      // Lane width at cut...
      odr::LaneWidth
        *widthAtCut = origLane->widthAt( s - sectionAtCut->start() ),
        *newWidth   = newLane->createWidth( 0.0 );

      double
        a=widthAtCut->a(), b=widthAtCut->b(),
        c=widthAtCut->c(), d=widthAtCut->d(),
        t=s - sectionAtCut->start() - widthAtCut->startOffset();

      if( newWidth ) {
        newWidth->setA( a + b*t + c*t*t + d*t*t*t );
        newWidth->setB( b + 2.0*c*t + 3.0*d*t*t );
        newWidth->setC( c + 3.0*d*t );
        newWidth->setD( d );
      }

      // Lane width records after cut...
      misc::Enumerator<odr::LaneWidth*> *widthEnum =
        origLane->enumerateWidths();
      odr::LaneWidth *width=0;
      while( widthEnum && (width=widthEnum->item()) ) {
        widthEnum->moveNext();

        double sOffset = width->startOffset() + sectionAtCut->start() - s;
        if( sOffset <= 0.0 ) continue;

        toDelete.push_back( width );

        newWidth = newLane->createWidth( sOffset );
        if( !newWidth ) continue;
        newWidth->setA( width->a() );
        newWidth->setB( width->b() );
        newWidth->setC( width->c() );
        newWidth->setD( width->d() );
      }
      delete widthEnum;
    }

    // Manage road lanes after cut...
    misc::Enumerator<odr::LaneSection*> *sectionEnum =
      road->lanes()->enumerate();
    odr::LaneSection *section = 0;
    while( sectionEnum && (section = sectionEnum->item()) ) {
      sectionEnum->moveNext();

      double sectionStart = section->start() - s;
      if( sectionStart <= 0.0 ) continue;

      toDelete.push_back( section );

      odr::LaneSection *newSection =
        road2->lanes()->createSection( sectionStart );
      if( !newSection ) continue;

      misc::Enumerator<odr::Lane*> *laneEnum = section->enumerate();
      odr::Lane *lane = 0;
      while( laneEnum && (lane=laneEnum->item()) ) {
        laneEnum->moveNext();

        if( lane->index()==0 ) continue;

        odr::Lane *newLane = newSection->createLane(
          lane->index()>0 ? (int)newSection->countLeft()+1 : -1 );
        if( !newLane ) continue;

        misc::Enumerator<odr::LaneWidth*> *widthEnum =
          lane->enumerateWidths();
        odr::LaneWidth *width=0;
        while( widthEnum && (width=widthEnum->item()) ) {
          widthEnum->moveNext();

          odr::LaneWidth *newWidth =
            newLane->createWidth( width->startOffset() );
          if( !newWidth ) continue;
          newWidth->setA( width->a() );
          newWidth->setB( width->b() );
          newWidth->setC( width->c() );
          newWidth->setD( width->d() );
        }
        delete widthEnum;
      }
      delete laneEnum;
    }
    delete sectionEnum;

    // Manage road elevation at cut...
    odr::Elevation *elevationAtCut = road->elevationProfile()->elevationAt(s);
    if( elevationAtCut ) {
      double
        t = s - elevationAtCut->start(),
        a = elevationAtCut->a(), b = elevationAtCut->b(),
        c = elevationAtCut->c(), d = elevationAtCut->d();

      odr::Elevation *newElevation =
        road2->elevationProfile()->createRecord( 0.0 );
      if( newElevation ) {
        newElevation->setA( a + b*t + c*t*t + d*t*t*t );
        newElevation->setB( b + 2.0*c*t + 3.0*d*t*t );
        newElevation->setC( c + 3.0*d*t );
        newElevation->setD( d );
      }

      // Manage elevation records after cut...
      misc::Enumerator<odr::Elevation*> *elevEnum =
        road->elevationProfile()->enumerate();
      odr::Elevation *elev = 0;
      while( elevEnum && (elev=elevEnum->item()) ) {
        elevEnum->moveNext();

        double start = elev->start()-s;
        if( start<=0.0 ) continue;

        toDelete.push_back( elev );

        newElevation = road2->elevationProfile()->createRecord( start );
        if( !newElevation ) continue;
        
        newElevation->setA( elev->a() );
        newElevation->setB( elev->b() );
        newElevation->setC( elev->c() );
        newElevation->setD( elev->d() );
      }
      delete elevEnum;
    }

    // Manage road crossfall at cut...
    odr::Crossfall *crossfallAtCut = road->lateralProfile()->crossfallAt(s);
    if( crossfallAtCut ) {
      double
        t = s - crossfallAtCut->start(),
        a = crossfallAtCut->a(), b = crossfallAtCut->b(),
        c = crossfallAtCut->c(), d = crossfallAtCut->d();

      odr::Crossfall *newCrossfall =
        road2->lateralProfile()->createCrossfall( 0.0 );
      if( newCrossfall ) {
        newCrossfall->setA( a + b*t + c*t*t + d*t*t*t );
        newCrossfall->setB( b + 2.0*c*t + 3.0*d*t*t );
        newCrossfall->setC( c + 3.0*d*t );
        newCrossfall->setD( d );
      }

      // Manage crossfall records after cut...
      misc::Enumerator<odr::Crossfall*> *crossEnum =
        road->lateralProfile()->enumerateCrossfalls();
      odr::Crossfall *cross = 0;
      while( crossEnum && (cross=crossEnum->item()) ) {
        crossEnum->moveNext();

        double start = cross->start()-s;
        if( start<=0.0 ) continue;

        toDelete.push_back( cross );

        newCrossfall = road2->lateralProfile()->createCrossfall( start );
        if( !newCrossfall ) continue;
        
        newCrossfall->setA( cross->a() );
        newCrossfall->setB( cross->b() );
        newCrossfall->setC( cross->c() );
        newCrossfall->setD( cross->d() );
      }
      delete crossEnum;
    }

    // Manage road superelevation at cut...
    odr::Superelevation *superelevationAtCut =
      road->lateralProfile()->superelevationAt(s);
    if( superelevationAtCut ) {
      double
        t = s - superelevationAtCut->start(),
        a = superelevationAtCut->a(), b = superelevationAtCut->b(),
        c = superelevationAtCut->c(), d = superelevationAtCut->d();

      odr::Superelevation *newSuperelevation =
        road2->lateralProfile()->createSuperelevation( 0.0 );
      if( newSuperelevation ) {
        newSuperelevation->setA( a + b*t + c*t*t + d*t*t*t );
        newSuperelevation->setB( b + 2.0*c*t + 3.0*d*t*t );
        newSuperelevation->setC( c + 3.0*d*t );
        newSuperelevation->setD( d );
      }

      // Manage superelevation records after cut...
      misc::Enumerator<odr::Superelevation*> *elevEnum =
        road->lateralProfile()->enumerateSuperelevations();
      odr::Superelevation *super = 0;
      while( elevEnum && (super=elevEnum->item()) ) {
        elevEnum->moveNext();

        double start = super->start()-s;
        if( start<=0.0 ) continue;

        toDelete.push_back( super );

        newSuperelevation =
          road2->lateralProfile()->createSuperelevation( start );
        if( !newSuperelevation ) continue;
        
        newSuperelevation->setA( super->a() );
        newSuperelevation->setB( super->b() );
        newSuperelevation->setC( super->c() );
        newSuperelevation->setD( super->d() );
      }
      delete elevEnum;
    }

    // Delete former road elements beyond cutting S-coord.
    DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
    while( !toDelete.empty() ) {
      args->Node = toDelete.back();
      NodeDeleted( this, args );
      delete toDelete.back();
      toDelete.pop_back();
    }

    // Trigger creation event...
    args->Node = road;
    NodeModified( this, args );
    args->Node = road2;
    NodeCreated( this, args );
    DocumentModified( this, args );

    return true;
  }



  //--------------------------------------------------------------------------
  void DomainLogic::RaiseNodeModified( void ) {
    DomainLogicEventArgs ^args = gcnew DomainLogicEventArgs();
    args->Node = _selection->ptr();
    NodeModified( this, args );
    DocumentModified( this, args );
  }

  void DomainLogic::UpdateGeometricPrimitives( odr::Geometry* geo )
  {
    misc::Enumerator<odr::GeometricPrimitive*>* primEnum =
      geo->enumerate();
    odr::GeometricPrimitive *previousPrim = primEnum->item();
    primEnum->moveNext();
    while( odr::GeometricPrimitive *prim = primEnum->item() ) {
      double x, y, h;
      Render::getEvaluator()->evaluatePositionOnPrimitive(
        previousPrim, previousPrim->length(), &x, &y, &h );
      prim->setX(x);
      prim->setY(y);
      prim->setHeading(h);
      previousPrim=prim;
      primEnum->moveNext();
    }
    delete primEnum;
  }

  void DomainLogic::SetElementName( Object ^value ) {
    odr::Element* el = dynamic_cast<odr::Element*>( _selection->ptr() );
    String ^name = dynamic_cast<String^>( value );
    if( el && name ) {
      IntPtr tmp = Marshal::StringToHGlobalAnsi(name);
      el->setName( (const char*)(void*)tmp );
      Marshal::FreeHGlobal(tmp);
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetRoadLength( Object ^value ) {
    odr::Road* road = dynamic_cast<odr::Road*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( road && d ) {
      road->setLength( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetLaneSectionStart( Object ^value ) {
    odr::LaneSection* section = dynamic_cast<odr::LaneSection*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( section && d ) {
      section->setStart( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetLaneSectionLength( Object ^value ) {
    odr::LaneSection* section = dynamic_cast<odr::LaneSection*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( section && d ) {
      section->setLength( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetLaneWidthStartOffset( Object ^value ) {
    odr::LaneWidth* width = dynamic_cast<odr::LaneWidth*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( width && d ) {
      width->setStartOffset( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetLaneWidthLength( Object ^value ) {
    odr::LaneWidth* width = dynamic_cast<odr::LaneWidth*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( width && d ) {
      width->setLength( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetLaneWidthA( Object ^value ) {
    odr::LaneWidth* width = dynamic_cast<odr::LaneWidth*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( width && d ) {
      width->setA( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetLaneWidthB( Object ^value ) {
    odr::LaneWidth* width = dynamic_cast<odr::LaneWidth*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( width && d ) {
      width->setB( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetLaneWidthC( Object ^value ) {
    odr::LaneWidth* width = dynamic_cast<odr::LaneWidth*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( width && d ) {
      width->setC( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetLaneWidthD( Object ^value ) {
    odr::LaneWidth* width = dynamic_cast<odr::LaneWidth*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( width && d ) {
      width->setD( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetGeometricPrimitiveStart( Object ^value ) {
    odr::GeometricPrimitive* prim = dynamic_cast<odr::GeometricPrimitive*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( prim && d ) {
      if( *d==0.0 ) {
        odr::GeometricPrimitive *primAt0 = prim->parentGeometry()->primitiveAt(0.0);
        prim->setX( primAt0->x() );
        prim->setY( primAt0->y() );
      }
      prim->setS( *d );
      UpdateGeometricPrimitives( prim->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetGeometricPrimitiveLength( Object ^value ) {
    odr::GeometricPrimitive* prim = dynamic_cast<odr::GeometricPrimitive*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( prim && d ) {
      prim->setLength( *d );
      UpdateGeometricPrimitives( prim->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetGeometricPrimitiveX( Object ^value ) {
    odr::GeometricPrimitive* prim = dynamic_cast<odr::GeometricPrimitive*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( prim && d ) {
      prim->setX( *d );
      UpdateGeometricPrimitives( prim->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetGeometricPrimitiveY( Object ^value ) {
    odr::GeometricPrimitive* prim = dynamic_cast<odr::GeometricPrimitive*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( prim && d ) {
      prim->setY( *d );
      UpdateGeometricPrimitives( prim->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetGeometricPrimitiveHeadingDeg( Object ^value ) {
    odr::GeometricPrimitive* prim = dynamic_cast<odr::GeometricPrimitive*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( prim && d ) {
      prim->setHeading( (*d)*3.141592654/180.0 );
      UpdateGeometricPrimitives( prim->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetArcCurvature( Object ^value ) {
    odr::Arc* arc = dynamic_cast<odr::Arc*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( arc && d ) {
      arc->setCurvature( *d );
      UpdateGeometricPrimitives( arc->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetSpiralCurvatureStart( Object ^value ) {
    odr::Spiral* spiral = dynamic_cast<odr::Spiral*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( spiral && d ) {
      spiral->setCurvatureStart( *d );
      UpdateGeometricPrimitives( spiral->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetSpiralCurvatureEnd( Object ^value ) {
    odr::Spiral* spiral = dynamic_cast<odr::Spiral*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( spiral && d ) {
      spiral->setCurvatureEnd( *d );
      UpdateGeometricPrimitives( spiral->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetPoly3A( Object ^value ) {
    odr::Poly3* poly3 = dynamic_cast<odr::Poly3*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( poly3 && d ) {
      poly3->setA( *d );
      UpdateGeometricPrimitives( poly3->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetPoly3B( Object ^value ) {
    odr::Poly3* poly3 = dynamic_cast<odr::Poly3*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( poly3 && d ) {
      poly3->setB( *d );
      UpdateGeometricPrimitives( poly3->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetPoly3C( Object ^value ) {
    odr::Poly3* poly3 = dynamic_cast<odr::Poly3*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( poly3 && d ) {
      poly3->setC( *d );
      UpdateGeometricPrimitives( poly3->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetPoly3D( Object ^value ) {
    odr::Poly3* poly3 = dynamic_cast<odr::Poly3*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( poly3 && d ) {
      poly3->setD( *d );
      UpdateGeometricPrimitives( poly3->parentGeometry() );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetElevationStart( Object ^value ) {
    odr::Elevation* elev = dynamic_cast<odr::Elevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( elev && d ) {
      elev->setStart( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetElevationLength( Object ^value ) {
    odr::Elevation* elev = dynamic_cast<odr::Elevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( elev && d ) {
      elev->setLength( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetElevationA( Object ^value ) {
    odr::Elevation* elev = dynamic_cast<odr::Elevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( elev && d ) {
      elev->setA( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetElevationB( Object ^value ) {
    odr::Elevation* elev = dynamic_cast<odr::Elevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( elev && d ) {
      elev->setB( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetElevationC( Object ^value ) {
    odr::Elevation* elev = dynamic_cast<odr::Elevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( elev && d ) {
      elev->setC( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetElevationD( Object ^value ) {
    odr::Elevation* elev = dynamic_cast<odr::Elevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( elev && d ) {
      elev->setD( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetCrossfallStart( Object ^value ) {
    odr::Crossfall* cross = dynamic_cast<odr::Crossfall*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( cross && d ) {
      cross->setStart( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetCrossfallLength( Object ^value ) {
    odr::Crossfall* cross = dynamic_cast<odr::Crossfall*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( cross && d ) {
      cross->setLength( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetCrossfallA( Object ^value ) {
    odr::Crossfall* cross = dynamic_cast<odr::Crossfall*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( cross && d ) {
      cross->setA( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetCrossfallB( Object ^value ) {
    odr::Crossfall* cross = dynamic_cast<odr::Crossfall*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( cross && d ) {
      cross->setB( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetCrossfallC( Object ^value ) {
    odr::Crossfall* cross = dynamic_cast<odr::Crossfall*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( cross && d ) {
      cross->setC( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetCrossfallD( Object ^value ) {
    odr::Crossfall* cross = dynamic_cast<odr::Crossfall*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( cross && d ) {
      cross->setD( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetSuperelevationStart( Object ^value ) {
    odr::Superelevation* super = dynamic_cast<odr::Superelevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( super && d ) {
      super->setStart( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetSuperelevationLength( Object ^value ) {
    odr::Superelevation* super = dynamic_cast<odr::Superelevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( super && d ) {
      super->setLength( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetSuperelevationA( Object ^value ) {
    odr::Superelevation* super = dynamic_cast<odr::Superelevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( super && d ) {
      super->setA( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetSuperelevationB( Object ^value ) {
    odr::Superelevation* super = dynamic_cast<odr::Superelevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( super && d ) {
      super->setB( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetSuperelevationC( Object ^value ) {
    odr::Superelevation* super = dynamic_cast<odr::Superelevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( super && d ) {
      super->setC( *d );
      RaiseNodeModified();
    }
  }

  void DomainLogic::SetSuperelevationD( Object ^value ) {
    odr::Superelevation* super = dynamic_cast<odr::Superelevation*>( _selection->ptr() );
    Double ^d = dynamic_cast<Double^>(value);
    if( super && d ) {
      super->setD( *d );
      RaiseNodeModified();
    }
  }

} // namespace roads
