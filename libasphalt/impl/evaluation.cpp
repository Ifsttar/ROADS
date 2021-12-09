//----------------------------------------------------------------------------
// libasphalt (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file evaluation.cpp
//! \brief Code for evaluating a position on a road.

#include "evaluation.h"
#include <map>

#define EPSILON 0.001

using namespace misc;
using namespace odr;
using namespace asphalt;


//----------------------------------------------------------------------------
void asphalt::evaluatePrimitive(
  const odr::GeometricPrimitive* primitive, double sOffset,
  double *xOut, double *yOut, double *hOut )
{
  double x = primitive->x();
  double y = primitive->y();
  double head = primitive->heading();
  double cosHead = cos( head );
  double sinHead = sin( head );
  double length = primitive->length();

  switch( primitive->type() ) {
  case odr::GeometricPrimitive::T_ARC:
    {
      const odr::Arc *arc = dynamic_cast<const odr::Arc*>(primitive);
      double curvature = arc->curvature();
      if( curvature!=0.0 ) {
        double centroidX = x - sinHead / curvature,
               centroidY = y + cosHead / curvature;
        double u =  sin( sOffset*curvature ) / curvature,
               v = -cos( sOffset*curvature ) / curvature;
        if( xOut ) *xOut = centroidX + u*cosHead - v*sinHead;
        if( yOut ) *yOut = centroidY + u*sinHead + v*cosHead;
        if( hOut ) *hOut = head + sOffset*curvature;
        return;
      }
    }
    // No break, arc with 0 curvature is processed as a straight line...

  case odr::GeometricPrimitive::T_LINE:
    if( xOut ) *xOut = x + cosHead*sOffset;
    if( yOut ) *yOut = y + sinHead*sOffset;
    if( hOut ) *hOut = head;
    return;

  case odr::GeometricPrimitive::T_SPIRAL:
    {
      const odr::Spiral *spiral =
        dynamic_cast<const odr::Spiral*>(primitive);
      double curvStart = spiral->curvatureStart(),
             curvEnd   = spiral->curvatureEnd();
      if( curvStart==0.0 && curvEnd==0.0 ) {
        if( xOut ) *xOut = x + cosHead*sOffset;
        if( yOut ) *yOut = y + sinHead*sOffset;
        if( hOut ) *hOut = head;
        return;
      }
      else if( curvStart==curvEnd ) {
        double centroidX = x - sinHead / curvStart,
               centroidY = y + cosHead / curvStart;
        double u =  sin( sOffset*curvStart ) / curvStart,
               v = -cos( sOffset*curvStart ) / curvStart;
        if( xOut ) *xOut = centroidX + u*cosHead - v*sinHead;
        if( yOut ) *yOut = centroidY + u*sinHead + v*cosHead;
        if( hOut ) *hOut = head + sOffset*curvStart;
        return;
      }
      double k = sqrt( 2.0 * length / fabs(curvStart-curvEnd) );
      double t0 = k * curvStart * 0.5,
             t1 = k * curvEnd * 0.5;
      double x0=0.0, y0=0.0;
      asphalt::fresnel(
        t0 / asphalt::math<double>::sqrt_pi_by_two, &x0, &y0 );
      x0 *= k * asphalt::math<double>::sqrt_pi_by_two;
      y0 *= k * asphalt::math<double>::sqrt_pi_by_two;
      double dx0 = cos(t0*t0),
             dy0 = sin(t0*t0);
      double t = t0 + sOffset*(t1-t0)/length;
      // Calculate fresnel integrals for t...
      double ss=0.0, cc=0.0;
      asphalt::fresnel(
        t/asphalt::math<double>::sqrt_pi_by_two, &ss, &cc );
      // Scale curve...
      ss *= k * asphalt::math<double>::sqrt_pi_by_two;
      cc *= k * asphalt::math<double>::sqrt_pi_by_two;
      // Substract coordinates of starting position...
      ss -= x0;
      cc -= y0;
      // Rotate to get the coordinates in coordframe t0, n0...
      double tangent = dx0*cc + dy0*ss,
             normal  = dx0*ss - dy0*cc;
      // Invert tangent coordinate if curvature decreases...
      if( curvEnd < curvStart ) tangent = -tangent;
      // Output coordinates...
      if( xOut ) *xOut = x + cosHead*tangent - sinHead*normal;
      if( yOut ) *yOut = y + sinHead*tangent + cosHead*normal;
      if( hOut ) *hOut = head + curvStart*sOffset +
        0.5*(curvEnd-curvStart)*sqr(sOffset)/length;
    }
    return;

  case odr::GeometricPrimitive::T_POLY3:
    {
      // Retrieve curve parameters...
      const odr::Poly3 *poly = dynamic_cast<const odr::Poly3*>(primitive);
      double a = poly->a(),
             b = poly->b(),
             c = poly->c(),
             d = poly->d();
      // Find parameter value...
      double u = asphalt::poly3length2param( a, b, c, d, sOffset );
      // Calculate polynom...
      double v = a + b*u + c*u*u + d*u*u*u;
      // Output coordinates...
      if( xOut ) *xOut = x + u*cosHead - v*sinHead;
      if( yOut ) *yOut = y + u*sinHead + v*cosHead;
      if( hOut ) *hOut = head + atan( b + 2.0*c*u + 3.0*d*u*u );
    }
    return;

  } // switch( geo->type() )

}


//----------------------------------------------------------------------------
void asphalt::evaluateLane(
  const odr::Lane* lane, double sOffset, double tOffset,
  double *positionOut, double *normalOut,
  double *dposdsOut, double *dposdtOut )
{
  // Retrieve lane section and road...
  const LaneSection* section = lane->parentSection();
  const Road* road = section->parentLanes()->parentRoad();
  double sectionStart = section->start();
  double s = sectionStart+sOffset;

  // Find applicable geometric primitive...
  const GeometricPrimitive *geo =
    road->geometry()->primitiveAt(section->start()+sOffset);

  // Find applicable superelevation (there might not be any)...
  double super = 0.0;
  {
    const Superelevation *se = road->lateralProfile()->superelevationAt(s);
    if( se ) {
      double ss = s - se->start();
      super = se->a() + se->b()*ss + se->c()*ss*ss + se->d()*ss*ss*ss;
    }
  }

  // Find applicable crossfall (there might not be any)...
  double crossfall = 0.0;
  {
    const Crossfall *cf = road->lateralProfile()->crossfallAt(s);
    if( cf ) {
      double ss = s - cf->start();
      crossfall = cf->a() + cf->b()*ss + cf->c()*ss*ss + cf->d()*ss*ss*ss;
    }
  }

  // Find applicable elevation (there might not be any)...
  double elevation = 0.0, delevationds = 0.0;
  {
    const Elevation *e = road->elevationProfile()->elevationAt(s);
    if( e ) {
      double ss = s - e->start();
      elevation = e->a() + e->b()*ss + e->c()*ss*ss + e->d()*ss*ss*ss;
      delevationds = e->b() + 2.0*e->c()*ss + 3.0*e->d()*ss*ss;
    }
  }

  // Find applicable lane widths...
  std::map<int,double> widths, dwidthds;
  widths[0] = 0.0;
  dwidthds[0] = 0.0;
  {
    Enumerator<const Lane*> *laneEnum = section->enumerate();
    const Lane *lane = 0;
    while( laneEnum && (lane=laneEnum->item()) ) {
      laneEnum->moveNext();

      const LaneWidth *lw = lane->widthAt(sOffset);
      if( !lw )
        return;

      double ss = sOffset - lw->startOffset();
      widths[ lane->index() ] =
        lw->a() + lw->b()*ss + lw->c()*ss*ss + lw->d()*ss*ss*ss;
      dwidthds[ lane->index() ] =
        lw->b() + 2.0*lw->c()*ss + 3.0*lw->d()*ss*ss;
    }
    delete laneEnum;
  }

  // Calculate height of lane separations...
  std::map<int,double> heights;
  heights[0] = elevation;
  for( int i=1 ; i<=(int)section->countLeft() ; ++i ) {
    if( section->item(i)->level() )
      heights[i] = heights[i-1];
    else
      heights[i] = heights[i-1] - widths[i] * tan( super + crossfall );
  }
  for( int i=-1 ; i>=-(int)section->countRight() ; --i ) {
    if( section->item(i)->level() )
      heights[i] = heights[i+1];
    else
      heights[i] = heights[i+1] + widths[i] * tan( super - crossfall );
  }

  // Find actual t coordinate of evaluated point...
  double t = tOffset;
  int laneIndex = lane->index();
  if( laneIndex > (int)section->countLeft() )
    laneIndex = (int)section->countLeft();
  if( laneIndex < -(int)section->countRight() )
    laneIndex = -(int)section->countRight();
  while( laneIndex>0 ) t += widths[ laneIndex-- ];
  while( laneIndex<0 ) t -= widths[ laneIndex++ ];

  // Find the lane actually containing evaluated point and let tOffset become
  // the interpolation parameter...
  tOffset = t;
  double dtds=0.0;
  laneIndex = -(int)section->countRight();
  for( int i=laneIndex ; i<0 ; ++i ) {
    tOffset += widths[i];
    dtds -= dwidthds[i];
  }
  while( tOffset>widths[laneIndex] && laneIndex<(int)section->countLeft() ) {
    tOffset -= widths[laneIndex];
    dtds += dwidthds[laneIndex];
    laneIndex++;
  }
  if( fabs(widths[laneIndex]) > EPSILON )
    tOffset /= widths[laneIndex];
  else
    tOffset = 0.0;
  dtds += dwidthds[laneIndex] * tOffset;
  if( laneIndex>0 ) laneIndex--;

  // Evaluate X-Y position on road chord line and heading, depending on the
  // type of geometric primitive...
  double Xchord = std::numeric_limits<double>::quiet_NaN();
  double Ychord = std::numeric_limits<double>::quiet_NaN();
  double Hchord = std::numeric_limits<double>::quiet_NaN();
  evaluatePrimitive( geo, s - geo->s(), &Xchord, &Ychord, &Hchord );

  // Compute result...
  double cosHeading = cos( Hchord ),
         sinHeading = sin( Hchord );
  double position[3];
  position[0] = Xchord - t * sinHeading;
  position[1] = Ychord + t * cosHeading;
  position[2] =
    heights[laneIndex] + tOffset*(heights[laneIndex+1]-heights[laneIndex]);
  if( positionOut )
    mk_vecop<3>(positionOut) = mk_vecop(position);

  // Tangent along s...
  double dposds[3];
  dposds[0] = cos( Hchord + atan(dtds) );
  dposds[1] = sin( Hchord + atan(dtds) );
  dposds[2] = delevationds;
  vecop<double[3]> vdposds(dposds);
  vdposds *= 1.0 / vdposds.norm2();
  if( dposdsOut )
    mk_vecop<3>(dposdsOut) = vdposds;

  // Tangent along t...
  double dposdt[3];
  vecop<double[3]> vdposdt(dposdt);
  dposdt[0] = -sinHeading;
  dposdt[1] = cosHeading;
  dposdt[2] = heights[laneIndex+1] - heights[laneIndex];
  if( fabs(dposdt[2]) > EPSILON )
   dposdt[2] /= laneIndex>=0 ? widths[laneIndex+1] : widths[laneIndex];
  vdposdt *= 1.0 / vdposdt.norm2();
  if( dposdtOut )
    mk_vecop<3>(dposdtOut) = vdposdt;

  // Normal...
  if( normalOut ) {
    vecop<double*,pointer_traits<double,3>> v(normalOut);
    v = vdposds ^ vdposdt;
    v *= 1.0 / v.norm2();
  }
}


//----------------------------------------------------------------------------
void asphalt::evaluatePosition(
  const odr::Road* road, double s, int laneIndex, double tOffset,
  double *positionOut, double *normalOut,
  double *dposdsOut, double *dposdtOut )
{
  // Find applicable lane section...
  const LaneSection *section = road->lanes()->sectionAt(s);

  // Adjust lane index...
  if( laneIndex < -(int)section->countRight() )
    laneIndex = -(int)section->countRight();
  if( laneIndex > (int)section->countLeft() )
    laneIndex = (int)section->countLeft();

  // Perform evaluation...
  evaluateLane( section->item(laneIndex), s-section->start(), tOffset,
                positionOut, normalOut, dposdsOut, dposdtOut );
}

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
