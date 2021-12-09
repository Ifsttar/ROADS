//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file JoinToolPolicy.cpp
//! \brief Definition of methods of class roads::JoinToolPolicy.

#include "stdafx.h"
#include "JoinToolPolicy.h"
#include "LaneConnectionOptionsForm.h"
#include "MainForm.h"
#include "Render.h"

namespace roads {

  JoinToolPolicy::JoinToolPolicy( void )
  {
    _createdRoad = new misc::WeakRef<const odr::Road>(0);
    _toConnect = gcnew Generic::List<RoadEnd>();
    _optionsForm = gcnew LaneConnectionOptionsForm();
    _optionsForm->OptionsChanged +=
      gcnew EventHandler( this, &JoinToolPolicy::UpdateJunction );
  }

  JoinToolPolicy::~JoinToolPolicy()
  {
    delete _createdRoad;
    _createdRoad = 0;
    delete _toConnect;
    _toConnect = nullptr;
  }

  JoinToolPolicy::!JoinToolPolicy()
  {
    delete _createdRoad;
  }

  void JoinToolPolicy::OnLMBDown(
    MainForm ^form, MouseEventArgs ^args )
  {
    double sOffset;
    const odr::GeometricPrimitive* prim =
      DomainLogic::Instance->GetPrimitiveNear(
        form->GLPanelX, form->GLPanelY, 50.0 * form->GLPanelMeterPerPixel,
        sOffset );
    if( prim ) {
      RoadEnd endPoint;
      endPoint.road = prim->parentGeometry()->parentRoad();
      endPoint.end  = ( sOffset+prim->s() > 0.5*endPoint.road->length() );
      _toConnect->Add( endPoint );
      form->Invalidate();
    }

    if( _toConnect->Count >= 2 ) {
      UpdateJunction( this, EventArgs::Empty );
      if( _optionsForm->ShowDialog()!=Forms::DialogResult::OK )
        DomainLogic::Instance->DeleteNode(_createdRoad->ptr());

      delete _optionsForm;
      _optionsForm = gcnew LaneConnectionOptionsForm();
      _optionsForm->OptionsChanged +=
        gcnew EventHandler( this, &JoinToolPolicy::UpdateJunction );
      _toConnect->Clear();
      *_createdRoad = 0;
    }

    form->GLPanelRefresh();
  }

  void JoinToolPolicy::OnLMBUp( MainForm ^form, MouseEventArgs ^args )
  {
  }

  void JoinToolPolicy::OnLMBDrag( MainForm ^form, MouseEventArgs ^args )
  {
  }

  void JoinToolPolicy::OnAbort( MainForm ^form )
  {
    _toConnect->Clear();
  }

  void JoinToolPolicy::UpdateJunction( Object ^sender, EventArgs ^args )
  {
    if( *_createdRoad )
      DomainLogic::Instance->DeleteNode(_createdRoad->ptr());

    if( _toConnect->Count >= 2 ) {
      _optionsForm->SetRoads(
        _toConnect[0].road,
        _toConnect[0].end
          ? LaneConnectionOptionsForm::RoadEnd::End
          : LaneConnectionOptionsForm::RoadEnd::Start,
        _toConnect[1].road,
        _toConnect[1].end
          ? LaneConnectionOptionsForm::RoadEnd::End
          : LaneConnectionOptionsForm::RoadEnd::Start );

      *_createdRoad =
        DomainLogic::Instance->ConnectRoads(
          _optionsForm->RoadA,
          _optionsForm->EndA==LaneConnectionOptionsForm::RoadEnd::End,
          _optionsForm->LanesA,
          _optionsForm->RoadB,
          _optionsForm->EndB==LaneConnectionOptionsForm::RoadEnd::End,
          _optionsForm->LanesB );
    }
  }

  void JoinToolPolicy::OnOptionsClosed( Object ^sender, EventArgs ^args )
  {
  }

  void JoinToolPolicy::OnGLPanelPaint( MainForm ^form )
  {
    glDisable( GL_BLEND );
    glColor3f( 1.0f, 0.0f, 0.0f );
    glPointSize( 5.0f );
    glBegin( GL_POINTS );
    for( int i=0 ; i<_toConnect->Count ; ++i ) {
      double x, y;
      const RoadEnd% endPoint = _toConnect[i];
      if( endPoint.end ) {
        double pos[3];
        Render::getEvaluator()->evaluatePositionOnRoad(
          endPoint.road, endPoint.road->length(), 0, 0.0, pos );
        x=pos[0];
        y=pos[1];
      }
      else {
        const odr::GeometricPrimitive* prim =
          endPoint.road->geometry()->item(0);
        x=prim->x();
        y=prim->y();
      }
      glVertex2d( x, y );
    }
    glEnd();
  }

} // namespace roads
