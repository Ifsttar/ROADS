//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file GeometryImpl.cpp
//! \brief odr::Geometry class implementation

#include "GeometryImpl.h"
#include <misc/EnumeratorImpl.h>
#include "GeometricPrimitiveImpl.h"
#include "RoadImpl.h"

odr::GeometryImpl::GeometryImpl( RoadImpl *parent ) :
DllImpl<Geometry>(),
WeakRefServerImpl(),
_parent          (parent),
_primitives      (),
_updateLengths   (true)
{}

ODR_CALL odr::GeometryImpl::~GeometryImpl()
{
  while( !_primitives.empty() )
    delete _primitives.begin()->second.ptr();
  setModified();
}

odr::Road* ODR_CALL odr::GeometryImpl::parentRoad( void )
{ return _parent; }

const odr::Road* ODR_CALL odr::GeometryImpl::parentRoad( void ) const
{ return _parent; }

::misc::Enumerator<odr::GeometricPrimitive*>*
odr::GeometryImpl::enumerate( void )
{
  return ::misc::newContainerEnumerator(
    _primitives.rbegin(), _primitives.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<GeometricPrimitive>,
      ::misc::ExtractSecond< ::misc::WeakRefCradle<GeometricPrimitiveImpl> > >() );
}

::misc::Enumerator<const odr::GeometricPrimitive*>*
odr::GeometryImpl::enumerate( void ) const
{
  return ::misc::newContainerEnumerator(
    _primitives.rbegin(), _primitives.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<const GeometricPrimitive>,
      ::misc::ExtractSecond< ::misc::WeakRefCradle<GeometricPrimitiveImpl> > >() );
}

unsigned int odr::GeometryImpl::count( void ) const
{ return (unsigned int)_primitives.size(); }

odr::GeometricPrimitive* ODR_CALL
  odr::GeometryImpl::item( unsigned int index )
{
  primitivesType::const_reverse_iterator it=_primitives.rbegin();
  while( it!=_primitives.rend() && index ) { --index; ++it; }
  if( it==_primitives.rend() ) return 0;
  return it->second.ptr();
}

const odr::GeometricPrimitive* ODR_CALL
  odr::GeometryImpl::item( unsigned int index ) const
{
  primitivesType::const_reverse_iterator it=_primitives.rbegin();
  while( it!=_primitives.rend() && index ) { --index; ++it; }
  if( it==_primitives.rend() ) return 0;
  return it->second.ptr();
}

odr::GeometricPrimitive* ODR_CALL odr::GeometryImpl::primitiveAt( double s )
{
  primitivesType::const_iterator it=_primitives.lower_bound(s);
  if( it==_primitives.end() )
    return _primitives.rbegin()->second.ptr();
  return it->second.ptr();
}

const odr::GeometricPrimitive* ODR_CALL odr::GeometryImpl::primitiveAt( double s ) const
{
  primitivesType::const_iterator it=_primitives.lower_bound(s);
  if( it==_primitives.end() )
    return _primitives.rbegin()->second.ptr();
  return it->second.ptr();
}

odr::Line* ODR_CALL odr::GeometryImpl::createLine( double start )
{ return new LineImpl(this,start); }

odr::Spiral* ODR_CALL odr::GeometryImpl::createSpiral( double start )
{ return new SpiralImpl(this,start); }

odr::Arc* ODR_CALL odr::GeometryImpl::createArc( double start )
{ return new ArcImpl(this,start); }

odr::Poly3* ODR_CALL odr::GeometryImpl::createPoly3( double start )
{ return new Poly3Impl(this,start); }

void odr::GeometryImpl::setModified( void )
{ _parent->setModified(); }

void odr::GeometryImpl::updateLengths( void ) const
{
  if( !_updateLengths ) return;
  _updateLengths=false;

  if( !_primitives.empty() ) {
    primitivesType::const_reverse_iterator prev = primitivesType::const_reverse_iterator();
    for( primitivesType::const_reverse_iterator it=_primitives.rbegin() ;
         it!=_primitives.rend() ; ++it )
    {
      if( it!=_primitives.rbegin() )
        prev->second->setCachedLength( it->second->s() - prev->second->s() );
      prev = it;
    }
    prev->second->setCachedLength( _parent->length() - prev->second->s() );
  }
}

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
