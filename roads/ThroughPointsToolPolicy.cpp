//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ThroughPointsToolPolicy.cpp
//! \brief Definition of methods of class roads::ThroughPointsToolPolicy.

#include "stdafx.h"
#include "ThroughPointsToolPolicy.h"
#include "DomainLogic.h"
#include "MainForm.h"

namespace roads {

  //--------------------------------------------------------------------------
  ThroughPointsToolPolicy::ThroughPointsToolPolicy( void ):
  _x( gcnew Generic::List<double>() ),
  _y( gcnew Generic::List<double>() ),
  _createdRoad( new misc::WeakRef<const odr::Road>(0) ),
  _dragIndex( -1 )
  {}

  //--------------------------------------------------------------------------
  ThroughPointsToolPolicy::~ThroughPointsToolPolicy()
  {
    delete _x; _x=nullptr;
    delete _y; _y=nullptr;
    delete _createdRoad; _createdRoad=0;
  }

  //--------------------------------------------------------------------------
  ThroughPointsToolPolicy::!ThroughPointsToolPolicy()
  {
    delete _x;
    delete _y;
    delete _createdRoad;
  }

  //--------------------------------------------------------------------------
  void ThroughPointsToolPolicy::OnLMBDown(
    MainForm ^form, MouseEventArgs ^args )
  {
    if( form->ModifierKeys == Forms::Keys::Control ) {
      _x->Clear();
      _y->Clear();
      *_createdRoad = 0;
      form->GLPanelRefresh();
    }
    else {
      for( int i=0 ; i<_x->Count ; ++i ) {
        if( fabs(_x[i]-form->GLPanelX) / form->GLPanelMeterPerPixel <= 3.0 &&
            fabs(_y[i]-form->GLPanelY) / form->GLPanelMeterPerPixel <= 3.0   )
        {
          _dragIndex = i;
          return;
        }
      }

      DomainLogic::Instance->DeleteNode( _createdRoad->ptr() );
      _dragIndex = _x->Count;
      if( _dragIndex>1 && form->ModifierKeys==Forms::Keys::Shift ) {
        int di=_dragIndex;
        double
          dx = _x[di-1] - _x[di-2],
          dy = _y[di-1] - _y[di-2];
        double n = sqrt(dx*dx+dy*dy);
        dx /= n;
        dy /= n;
        double s =
          dx * (form->GLPanelX - _x[di-1]) + dy * (form->GLPanelY - _y[di-1]);
        _x->Add( _x[di-1] + dx*s );
        _y->Add( _y[di-1] + dy*s );
      }
      else {
        _x->Add( form->GLPanelX );
        _y->Add( form->GLPanelY );
      }
      *_createdRoad = DomainLogic::Instance->RoadThroughPoints( _x, _y );
      if( !*_createdRoad ) form->GLPanelRefresh();
    }
  }

  //--------------------------------------------------------------------------
  void ThroughPointsToolPolicy::OnLMBUp(MainForm ^form, MouseEventArgs ^args )
  {
    _dragIndex = -1;
  }

  //--------------------------------------------------------------------------
  void ThroughPointsToolPolicy::OnLMBDrag(
    MainForm ^form, MouseEventArgs ^args )
  {
    if( _dragIndex>=0 ) {
      if( _dragIndex>1 && form->ModifierKeys==Forms::Keys::Shift ) {
        int di=_dragIndex;
        double
          dx = _x[di-1] - _x[di-2],
          dy = _y[di-1] - _y[di-2];
        double n = sqrt(dx*dx+dy*dy);
        dx /= n;
        dy /= n;
        double s =
          dx * (form->GLPanelX - _x[di-1]) + dy * (form->GLPanelY - _y[di-1]);
        _x[di] = _x[di-1] + dx*s;
        _y[di] = _y[di-1] + dy*s;
      }
      else {
        _x[_dragIndex] = form->GLPanelX;
        _y[_dragIndex] = form->GLPanelY;
      }
      DomainLogic::Instance->DeleteNode( _createdRoad->ptr() );
      *_createdRoad = DomainLogic::Instance->RoadThroughPoints( _x, _y );
      if( !*_createdRoad ) form->GLPanelRefresh();
    }
  }

  //--------------------------------------------------------------------------
  void ThroughPointsToolPolicy::OnAbort( MainForm ^form )
  {
    _x->Clear();
    _y->Clear();
    _dragIndex = -1;
    if( *_createdRoad )
      DomainLogic::Instance->DeleteNode( _createdRoad->ptr() );
    else
      form->GLPanelRefresh();
  }

  //--------------------------------------------------------------------------
  void ThroughPointsToolPolicy::OnGLPanelPaint( MainForm ^form )
  {
    if( _x->Count!=_y->Count || _x->Count==0 )
      return;

    glDisable( GL_BLEND );
    glColor3f( 1.0f, 0.0f, 0.0f );
    glPointSize( 3.0f );
    glBegin( GL_POINTS );
    for( int i=0 ; i<_x->Count ; ++i )
      glVertex2d( _x[i], _y[i] );
    glEnd();
  }

}
