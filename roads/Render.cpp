#include "StdAfx.h"
#include "Render.h"

namespace roads {

  //--------------------------------------------------------------------------
  ::asphalt::Evaluator* Render::getEvaluator( void ) {
    static ::asphalt::Evaluator* e = ::asphalt_createEvaluator();
    return e;
  }

  //--------------------------------------------------------------------------
  void Render::FilledRoad( const odr::Road *road, double start, double end ) {
    end = std::min(end,road->length());
    double length = end-start;
    if( length<0.0001 ) return;

    double increment = length / std::ceil( length/2.0 );

    glBegin( GL_TRIANGLE_STRIP );
    for( double s = start ; s<=end+0.1 ; s+=increment ) {
      double pos[3];
      getEvaluator()->evaluatePositionOnRoad( road, s, -65, 0.0, pos );
      glVertex2dv( pos );
      getEvaluator()->evaluatePositionOnRoad( road, s, 65, 0.0, pos );
      glVertex2dv( pos );
    }
    glEnd();
  }

  //--------------------------------------------------------------------------
  void Render::RoadOutline( const odr::Road *road, double start, double end ) {
    end = std::min(end,road->length());
    double length = end-start;
    if( length<0.0001 ) return;

    double increment = length / std::ceil( length/2.0 );

    glBegin( GL_LINE_LOOP );
    double s;
    for( s=start ; s<=end+0.1 ; s+=increment ) {
      double pos[3];
      getEvaluator()->evaluatePositionOnRoad( road, s, -65, 0.0, pos );
      glVertex2dv( pos );
    }
    for( s-=increment ; s>=start-0.1 ; s-=increment ) {
      double pos[3];
      getEvaluator()->evaluatePositionOnRoad( road, s, 65, 0.0, pos );
      glVertex2dv( pos );
    }
    glEnd();
  }

  //--------------------------------------------------------------------------
  void Render::FilledOutlinedRoad(
    const odr::Road *road, double start, double end )
  {
    FilledRoad( road, start, end );
    glColor4f( 0.0f, 0.0f, 0.0f, 0.8f );
    RoadOutline( road, start, end );
  }

  //--------------------------------------------------------------------------
  void Render::FilledLane(
    const odr::Lane *lane, double startOffset, double endOffset )
  {
    const odr::LaneSection *section = lane->parentSection();
    endOffset = std::min(endOffset,section->length());
    const odr::Road *road = section->parentLanes()->parentRoad();
    double length = endOffset-startOffset;
    if( length<0.0001 ) return;
    const odr::Lane *lLane=0, *rLane=0;
    int index = lane->index();
    if( index>0 ) { lLane=lane; rLane=section->item(index-1); }
    else          { rLane=lane; lLane=section->item(index+1); }

    double increment = length / std::ceil( length/2.0 );

    glBegin( GL_TRIANGLE_STRIP );
    for( double sOffset = startOffset ; sOffset<=endOffset+0.1 ;
         sOffset+=increment )
    {
      double pos[3];
      getEvaluator()->evaluatePositionOnLane( lLane, sOffset, 0.0, pos );
      glVertex2dv( pos );
      getEvaluator()->evaluatePositionOnLane( rLane, sOffset, 0.0, pos );
      glVertex2dv( pos );
    }
    glEnd();
  }

  //--------------------------------------------------------------------------
  void Render::LaneOutline(
    const odr::Lane *lane, double startOffset, double endOffset )
  {
    const odr::LaneSection *section = lane->parentSection();
    endOffset = std::min(endOffset,section->length());
    const odr::Road *road = section->parentLanes()->parentRoad();
    double length = endOffset-startOffset;
    if( length<0.0001 ) return;
    const odr::Lane *lLane=0, *rLane=0;
    int index = lane->index();
    if( index>0 ) { lLane=lane; rLane=section->item(index-1); }
    else          { rLane=lane; lLane=section->item(index+1); }

    double increment = length / std::ceil( length/2.0 );

    glBegin( GL_LINE_LOOP );
    double sOffset;
    for( sOffset=startOffset ; sOffset<=endOffset+0.1 ; sOffset+=increment ) {
      double pos[3];
      getEvaluator()->evaluatePositionOnLane( rLane, sOffset, 0.0, pos );
      glVertex2dv( pos );
    }
    for( sOffset-=increment ; sOffset>=startOffset-0.1 ; sOffset-=increment ) {
      double pos[3];
      getEvaluator()->evaluatePositionOnLane( lLane, sOffset, 0.0, pos );
      glVertex2dv( pos );
    }
    glEnd();
  }

  //--------------------------------------------------------------------------
  void Render::FilledOutlinedLane(
    const odr::Lane *lane, double startOffset, double endOffset )
  {
    FilledLane( lane, startOffset, endOffset );
    glColor4f( 0.0f, 0.0f, 0.0f, 0.8f );
    LaneOutline( lane, startOffset, endOffset );
  }

  void Render::GeometricPrimitive(
    const odr::GeometricPrimitive *primitive,
    double startOffset, double endOffset )
  {
    endOffset = std::min( endOffset, primitive->length() );
    double x, y, h, dxds, dyds, length=endOffset-startOffset;
    if( length<0.0001 ) return;
    getEvaluator()->evaluatePositionOnPrimitive(
      primitive, startOffset, &x, &y, &h );
    dxds = 2.0*cos(h);
    dyds = 2.0*sin(h);

    glBegin( GL_TRIANGLES );
      glVertex2d( x+dxds, y+dyds );
      glVertex2d( x-dyds, y+dxds );
      glVertex2d( x+dyds, y-dxds );
    glEnd();

    double increment = length / std::ceil( length/2.0 );
    glBegin( GL_LINE_STRIP );
    for( double s=startOffset ; s<=endOffset+0.1 ; s+=increment ) {
      getEvaluator()->evaluatePositionOnPrimitive( primitive, s, &x, &y );
      glVertex2d( x, y );
    }
    glEnd();
  }

  //--------------------------------------------------------------------------
  void Render::LaneOuterBorder( const odr::Lane *lane )
  {
    double length = lane->parentSection()->length();
    double increment = length / ceil(length);
    glBegin( GL_LINE_STRIP );
    for( double s=0.0 ; s<=length+0.1 ; s+=increment ) {
      double pos[3];
      getEvaluator()->evaluatePositionOnLane( lane, s, 0.0, pos );
      glVertex2dv(pos);
    }
    glEnd();
  }

} // namespace roads
