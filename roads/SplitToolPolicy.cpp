//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file SplitToolPolicy.cpp
//! \brief Definition of methods of class roads::SplitToolPolicy.

#include "stdafx.h"
#include "SplitToolPolicy.h"
#include "MainForm.h"
#include "Render.h"

namespace roads {

  SplitToolPolicy::SplitToolPolicy( void )
  {
    _road = new misc::WeakRef<const odr::Road>(0);
  }

  SplitToolPolicy::~SplitToolPolicy()
  {
    delete _road;
    _road=0;
  }

  SplitToolPolicy::!SplitToolPolicy()
  {
    delete _road;
  }

  void SplitToolPolicy::OnLMBDown( MainForm ^form, MouseEventArgs ^args )
  {
    OnLMBDrag( form, args );
  }

  void SplitToolPolicy::OnLMBUp( MainForm ^form, MouseEventArgs ^args )
  {
    if( *_road ) {
      DomainLogic::Instance->SplitRoad( _road->ptr(), _s );
    }

    *_road = 0;
    _s = 0.0;
    form->GLPanelRefresh();
  }

  void SplitToolPolicy::OnLMBDrag( MainForm ^form, MouseEventArgs ^args )
  {
    double sOffset;
    const odr::GeometricPrimitive *prim =
      DomainLogic::Instance->GetPrimitiveNear(
        form->GLPanelX, form->GLPanelY, form->GLPanelMeterPerPixel*50.0,
        sOffset );
    if( prim ) {
      *_road = prim->parentGeometry()->parentRoad();
      _s = sOffset + prim->s();
    }
    else {
      *_road = 0;
      _s = 0.0;
    }
    form->GLPanelRefresh();
  }

  void SplitToolPolicy::OnAbort( MainForm ^form )
  {

  }

  void SplitToolPolicy::OnGLPanelPaint( MainForm ^form )
  {
    if( !*_road ) return;

    glDisable( GL_BLEND );
    glLineWidth( 1.0f );
    glColor3f( 1.0f, 0.0f, 0.0f );
    glBegin( GL_LINES );
    double pos[3];
    Render::getEvaluator()->evaluatePositionOnRoad(
      _road->ptr(), _s, -64, 0.0, pos );
    glVertex2dv( pos );
    Render::getEvaluator()->evaluatePositionOnRoad(
      _road->ptr(), _s, 64, 0.0, pos );
    glVertex2dv( pos );
    glEnd();
  }

} // namespace roads
