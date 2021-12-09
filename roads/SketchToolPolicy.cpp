//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file SketchToolPolicy.cpp
//! \brief Definition of methods of class roads::SketchToolPolicy.

#include "stdafx.h"
#include "SketchToolPolicy.h"
#include "SketchOptionsForm.h"
#include "MainForm.h"

namespace roads {

  SketchToolPolicy::SketchToolPolicy( void )
  {
    _x = gcnew Collections::Generic::List<double>;
    _y = gcnew Collections::Generic::List<double>;
    _optionsForm = gcnew SketchOptionsForm();
    _optionsForm->PrimitiveCostChanged +=
      gcnew EventHandler( this, &SketchToolPolicy::OnPrimitiveCostChanged );
    _optionsForm->Closed +=
      gcnew EventHandler( this, &SketchToolPolicy::OnOptionsClosed );
    _createdRoad = new misc::WeakRef<const odr::Road>(0);
  }

  SketchToolPolicy::~SketchToolPolicy()
  {
    delete _createdRoad;
    _createdRoad = 0;
  }

  SketchToolPolicy::!SketchToolPolicy()
  {
    delete _createdRoad;
  }

  void SketchToolPolicy::OnLMBDown( MainForm ^form, MouseEventArgs ^args )
  {
    _x->Add( form->GLPanelX );
    _y->Add( form->GLPanelY );
    form->GLPanelRefresh();
  }

  void SketchToolPolicy::OnLMBUp( MainForm ^form, MouseEventArgs ^args )
  {
    if( *_createdRoad )
      DomainLogic::Instance->DeleteNode(_createdRoad->ptr());
    *_createdRoad =
      DomainLogic::Instance->SketchRoad(_x,_y,_optionsForm->PrimitiveCost);
    if( *_createdRoad ) {
      if( !_optionsForm->Visible )
        _optionsForm->Show(form);
    }
    else
      form->Refresh();
  }

  void SketchToolPolicy::OnLMBDrag( MainForm ^form, MouseEventArgs ^args )
  {
    if( _x->Count==0 || _y->Count==0 ) {
      OnLMBDown(form,args);
      return;
    }

    double
      dx = form->GLPanelX-_x[_x->Count-1],
      dy = form->GLPanelY-_y[_y->Count-1];
    if( sqrt(dx*dx+dy*dy) < 20.0*form->GLPanelMeterPerPixel ) return;
    _x->Add( form->GLPanelX );
    _y->Add( form->GLPanelY );
    form->GLPanelRefresh();
  }

  void SketchToolPolicy::OnAbort( MainForm ^form )
  {
    *_createdRoad=0;
    _optionsForm->Hide();
    _x->Clear();
    _y->Clear();
    form->Invalidate();
  }

  void SketchToolPolicy::OnGLPanelPaint( MainForm ^form )
  {
    glLineWidth( 1.0f );
    glDisable( GL_BLEND );
    glColor3f( 1.0f, 1.0f, 1.0f );
    glBegin( GL_LINE_STRIP );
    for( int i=0 ; i<_x->Count ; ++i )
      glVertex2d( _x[i], _y[i] );
    glEnd();
  }

  void SketchToolPolicy::OnPrimitiveCostChanged(
    Object ^sender, EventArgs ^args )
  {
    if( *_createdRoad )
      DomainLogic::Instance->DeleteNode(_createdRoad->ptr());
    *_createdRoad = DomainLogic::Instance->SketchRoad(
      _x, _y, _optionsForm->PrimitiveCost );
  }

  void SketchToolPolicy::OnOptionsClosed( Object ^sender, EventArgs ^args )
  {
    if( _createdRoad && _optionsForm->DialogResult!=Forms::DialogResult::OK ) {
      _x->Clear();
      _y->Clear();
      DomainLogic::Instance->DeleteNode(_createdRoad->ptr());
    }
    delete _optionsForm;
    _optionsForm = gcnew SketchOptionsForm();
    _optionsForm->PrimitiveCostChanged +=
      gcnew EventHandler( this, &SketchToolPolicy::OnPrimitiveCostChanged );
    _optionsForm->Closed +=
      gcnew EventHandler( this, &SketchToolPolicy::OnOptionsClosed );
    *_createdRoad = 0;
    _x->Clear();
    _y->Clear();
 }

} // namespace roads
