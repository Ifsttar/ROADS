//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file SketchToolPolicy.cpp
//! \brief Methods implementations for class roads::SketchToolPolicy.

#include "stdafx.h"
#include "ProlongateToolPolicy.h"
#include "DomainLogic.h"
#include "MainForm.h"

namespace roads {

  ProlongateToolPolicy::ProlongateToolPolicy( void )
  {}

  //--------------------------------------------------------------------------
  void ProlongateToolPolicy::OnLMBDown( MainForm ^form, MouseEventArgs ^args )
  {
    bool newRoad = false;
    const odr::Road* road =
      dynamic_cast<const odr::Road*>( DomainLogic::Instance->SelectedNode );
    if( !road ) {
      newRoad = true;
      road = DomainLogic::Instance->CreateRoad(0);
      if( !road ) return;
      const odr::Line* line =
        DomainLogic::Instance->CreateLine( road->geometry() );
      if( !line ) {
        DomainLogic::Instance->DeleteNode( road );
        return;
      }
      DomainLogic::Instance->SelectedNode = line;
      DomainLogic::Instance->SetGeometricPrimitiveStart( gcnew Double(0.0) );
      DomainLogic::Instance->SetGeometricPrimitiveX(
        gcnew Double(form->GLPanelX) );
      DomainLogic::Instance->SetGeometricPrimitiveY(
        gcnew Double(form->GLPanelY) );
      DomainLogic::Instance->SelectedNode = road;
    }
    if( !road )
      return;

    // Obtain curvature at road end...
    double curvature=0.0;
    const odr::GeometricPrimitive* prim =
      road->geometry()->primitiveAt( road->length() );
    if( const odr::Arc* arc = dynamic_cast<const odr::Arc*>(prim) )
      curvature = arc->curvature();
    else if( const odr::Spiral* spiral = 
              dynamic_cast<const odr::Spiral*>(prim) )
      curvature = spiral->curvatureEnd();

    // If the user is holding shift, create a constant curvature primitive...
    if( form->ModifierKeys == Forms::Keys::Shift ) {
      if( fabs(curvature)<0.001 )
        DomainLogic::Instance->CreateLine( road->geometry() );
      else {
        const odr::Arc *arc =
          DomainLogic::Instance->CreateArc( road->geometry() );
        if( !arc ) return;
        DomainLogic::Instance->SelectedNode = arc;
        DomainLogic::Instance->SetArcCurvature( gcnew Double(curvature) );
      }
      _zeroCurvatureAtEnd = false;
    }

    // ... else (not holding shift)...
    else if( !newRoad ) {
      // If the user is holding control, remember to set final curvature of
      // clothoid to zero...
      _zeroCurvatureAtEnd = ( form->ModifierKeys==Forms::Keys::Control );

      // Create clothoid...
      const odr::Spiral* spiral =
        DomainLogic::Instance->CreateSpiral( road->geometry() );
      if( !spiral ) return;
      DomainLogic::Instance->SelectedNode = spiral;
      DomainLogic::Instance->SetSpiralCurvatureStart( gcnew Double(curvature) );
    }

    // Adjust curve to reach pointed position...
    DomainLogic::Instance->ProlongateRoad(
      road, form->GLPanelX, form->GLPanelY, _zeroCurvatureAtEnd );
    DomainLogic::Instance->SelectedNode = road;
  }

  //--------------------------------------------------------------------------
  void ProlongateToolPolicy::OnLMBUp( MainForm ^form, MouseEventArgs ^args )
  {
    // Retrieve selected road...
    const odr::Road *road =
      dynamic_cast<const odr::Road*>( DomainLogic::Instance->SelectedNode );
    if( !road ) return;

    // Adjust curve to reach pointed position...
    DomainLogic::Instance->ProlongateRoad(
      road, form->GLPanelX, form->GLPanelY, _zeroCurvatureAtEnd );
  }

  //--------------------------------------------------------------------------
  void ProlongateToolPolicy::OnLMBDrag( MainForm ^form, MouseEventArgs ^args )
  {
    // Retrieve selected road...
    const odr::Road *road =
      dynamic_cast<const odr::Road*>( DomainLogic::Instance->SelectedNode );
    if( !road ) return;

    // Adjust curve to reach pointed position...
    DomainLogic::Instance->ProlongateRoad(
      road, form->GLPanelX, form->GLPanelY, _zeroCurvatureAtEnd );
  }

  //--------------------------------------------------------------------------
  void ProlongateToolPolicy::OnAbort( MainForm ^form )
  {

  }

  //--------------------------------------------------------------------------
  void ProlongateToolPolicy::OnGLPanelPaint( MainForm ^form )
  {}

} // namespace roads
