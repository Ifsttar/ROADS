//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file GeometricPrimitiveImpl.cpp
//! \brief Implementation of class GeometricPrimitive and its derivatives

#include <odr/Factories.h>
#include <odr/Road.h>
#include "GeometricPrimitiveImpl.h"
#include "GeometryImpl.h"

using namespace odr;

GeometricPrimitiveImpl::GeometricPrimitiveImpl( GeometryImpl *parent, double start ) :
GeometricPrimitive(),
WeakRefServerImpl (),
_parent           ( parent ),
_s                ( start ),
_x                ( 0.0 ),
_y                ( 0.0 ),
_hdg              ( 0.0 ),
_len              ( 0.0 )
{
  setS( start );
}

ODR_CALL GeometricPrimitiveImpl::~GeometricPrimitiveImpl()
{ _parent->setLengthsNeedUpdate(); setModified(); }

Geometry* GeometricPrimitiveImpl::parentGeometry( void )
{ return _parent; }

const Geometry* GeometricPrimitiveImpl::parentGeometry( void ) const
{ return _parent; }

double GeometricPrimitiveImpl::s( void ) const { return _s; }

double GeometricPrimitiveImpl::x( void ) const { return _x; }

double GeometricPrimitiveImpl::y( void ) const { return _y; }

double GeometricPrimitiveImpl::heading( void ) const { return _hdg; }

double GeometricPrimitiveImpl::length( void ) const
{
  _parent->updateLengths();
  return _len;
}

void GeometricPrimitiveImpl::setS( double v )
{
  GeometryImpl::primitivesType &primitives = _parent->primitives();

  // Find previous position in collection of primitives...
  GeometryImpl::primitivesType::iterator it = primitives.find(_s);

  // Update S coordinate...
  _s = v;

  // Remove primitive from collecion if it was inserted...
  if( it!=primitives.end() && it->second.ptr()==(GeometricPrimitive*)this )
    primitives.erase(it);

  // If there is already a primitive at the new position, delete it...
  it=primitives.find(_s);
  if( it!=primitives.end() )
    delete it->second.ptr();

  // Insert primitive at new position...
  primitives[_s] = this;

  // Document was modified...
  _parent->setLengthsNeedUpdate();
  setModified();
}

void GeometricPrimitiveImpl::setX( double v ) { _x = v; setModified(); }

void GeometricPrimitiveImpl::setY( double v ) { _y = v; setModified(); }

void GeometricPrimitiveImpl::setHeading( double v ) { _hdg=v; setModified(); }

void GeometricPrimitiveImpl::setLength( double v )
{
  // Length delta...
  double delta = std::max(v,0.001)-length();

  // Resize road...
  Road *road = _parent->parentRoad();
  road->setLength( road->length()+delta );

  // If shortening the primitive...
  if( delta<0.0 ) {
    // Move every primitive succeeding this one, starting from the next...
    GeometryImpl::primitivesType &primitives = _parent->primitives();
    GeometryImpl::primitivesType::iterator it = primitives.find(_s);
    if( it!=primitives.begin() ) do {
      GeometryImpl::primitivesType::iterator it2 = it;
      --it2;
      it2->second->setS( it2->second->_s+delta );
      --it;
    }
    while( it!=primitives.begin() );
  }

  // Else (if not shortening the primitive)...
  else {
    // Move every primitive succeeding this one, starting from the last...
    GeometryImpl::primitivesType &primitives = _parent->primitives();
    GeometryImpl::primitivesType::iterator it = primitives.begin();
    if( it->second.ptr()!=this ) {
      it->second->setS( it->second->_s+delta );
      it = primitives.begin();
      do {
        GeometryImpl::primitivesType::iterator it2 = it;
        ++it2;
        if( it2->second.ptr()!=this )
          it2->second->setS( it2->second->_s+delta );
        ++it;
      }
      while( it->second.ptr()!=this );
    }
  }

  // Propagate modification...
  setModified();
}

void GeometricPrimitiveImpl::setModified( void ) { _parent->setModified(); }

//----------------------------------------------------------------------------
LineImpl::LineImpl( GeometryImpl *parent, double start ) :
DllImpl<Line>(), GeometricPrimitiveImpl( parent, start ) {}

GeometricPrimitive::Type LineImpl::type( void ) const
{ return GeometricPrimitive::T_LINE; }


//----------------------------------------------------------------------------
SpiralImpl::SpiralImpl( GeometryImpl *parent, double start ) : 
DllImpl<Spiral>       (),
GeometricPrimitiveImpl( parent, start ),
_curvatureStart       ( 0.0 ),
_curvatureEnd         ( 0.0 )
{}

GeometricPrimitive::Type SpiralImpl::type( void ) const
{ return GeometricPrimitive::T_SPIRAL; }

double SpiralImpl::curvatureStart( void ) const {return _curvatureStart;}

double SpiralImpl::curvatureEnd( void ) const { return _curvatureEnd; }

void SpiralImpl::setCurvatureStart( double v )
{ _curvatureStart = v; setModified(); }

void SpiralImpl::setCurvatureEnd( double v )
{ _curvatureEnd = v; setModified(); }


//----------------------------------------------------------------------------
ArcImpl::ArcImpl( GeometryImpl *parent, double start ) : 
DllImpl<Arc>(), GeometricPrimitiveImpl( parent, start ), _curvature( 0.0 ) {}

GeometricPrimitive::Type ArcImpl::type( void ) const
{ return GeometricPrimitive::T_ARC; }

double ArcImpl::curvature( void ) const { return _curvature; }

void ArcImpl::setCurvature( double v ) { _curvature=v; setModified(); }


//----------------------------------------------------------------------------
Poly3Impl::Poly3Impl( GeometryImpl *parent, double start ) : 
DllImpl<Poly3>(), GeometricPrimitiveImpl( parent, start ),
_a(0.0), _b(0.0), _c(0.0), _d(0.0)
{}

GeometricPrimitive::Type Poly3Impl::type( void ) const
{ return GeometricPrimitive::T_POLY3; }

double Poly3Impl::a( void ) const { return _a; }

double Poly3Impl::b( void ) const { return _b; }

double Poly3Impl::c( void ) const { return _c; }

double Poly3Impl::d( void ) const { return _d; }

void Poly3Impl::setA( double v ) { _a=v; setModified(); }

void Poly3Impl::setB( double v ) { _b=v; setModified(); }

void Poly3Impl::setC( double v ) { _c=v; setModified(); }

void Poly3Impl::setD( double v ) { _d=v; setModified(); }

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
