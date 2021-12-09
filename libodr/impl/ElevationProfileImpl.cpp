//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ElevationProfileImpl.cpp
//! \brief odr::ElevationProfile class implementation

#include "ElevationProfileImpl.h"
#include "RoadImpl.h"
#include <misc/EnumeratorImpl.h>

using namespace odr;

ElevationProfileImpl::ElevationProfileImpl( RoadImpl *parent ) :
::misc::DllImpl<ElevationProfile>(),
::misc::WeakRefServerImpl        (),
_parent                          ( parent ),
_elevations                      (),
_updateLengths                   ( true )
{}

ElevationProfileImpl::~ElevationProfileImpl()
{
  while( !_elevations.empty() )
    delete _elevations.begin()->second.ptr();

  setModified();
}

Road* ElevationProfileImpl::parentRoad( void )
{ return _parent; }

const Road* ElevationProfileImpl::parentRoad( void ) const
{ return _parent; }

::misc::Enumerator<Elevation*>* ElevationProfileImpl::enumerate( void )
{
  return ::misc::newContainerEnumerator(
    _elevations.rbegin(), _elevations.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<Elevation>,
      ::misc::ExtractSecond< ::misc::WeakRefCradle<ElevationImpl> > >() );
}

::misc::Enumerator<const Elevation*>*
ElevationProfileImpl::enumerate( void ) const
{
  return ::misc::newContainerEnumerator(
    _elevations.rbegin(), _elevations.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<const Elevation>,
      ::misc::ExtractSecond< ::misc::WeakRefCradle<ElevationImpl> > >() );
}

unsigned int ElevationProfileImpl::count( void ) const
{ return (unsigned int)_elevations.size(); }


Elevation* ElevationProfileImpl::item( unsigned int index )
{
  elevationsType::reverse_iterator it=_elevations.rbegin();
  while( it!=_elevations.rend() && index ) { --index; ++it; }
  if( it==_elevations.rend() ) return 0;
  return it->second.ptr();
}

const Elevation* ElevationProfileImpl::item( unsigned int index ) const
{
  elevationsType::const_reverse_iterator it=_elevations.rbegin();
  while( it!=_elevations.rend() && index ) { --index; ++it; }
  if( it==_elevations.rend() ) return 0;
  return it->second.ptr();
}

Elevation* ElevationProfileImpl::elevationAt( double s )
{
  if( _elevations.empty() ) return 0;
  elevationsType::iterator it=_elevations.lower_bound(s);
  if( it==_elevations.end() )
    return _elevations.rbegin()->second.ptr();
  return it->second.ptr();
}

const Elevation* ElevationProfileImpl::elevationAt( double s ) const
{
  if( _elevations.empty() ) return 0;
  elevationsType::const_iterator it=_elevations.lower_bound(s);
  if( it==_elevations.end() )
    return _elevations.rbegin()->second.ptr();
  return it->second.ptr();
}

Elevation* ElevationProfileImpl::createRecord( double start )
{
  ElevationImpl *result = new ElevationImpl( this, start );
  setModified();
  return result;
}

void ElevationProfileImpl::setModified( void ) { _parent->setModified(); }

void ElevationProfileImpl::updateLengths( void ) const
{
  if( !_updateLengths ) return;
  _updateLengths = false;

  if( !_elevations.empty() )
  {
    elevationsType::const_reverse_iterator prev = elevationsType::const_reverse_iterator();
    for( elevationsType::const_reverse_iterator it=_elevations.rbegin() ;
         it!=_elevations.rend() ; ++it )
    {
      if( it!=_elevations.rbegin() )
        prev->second->setCachedLength( it->second->start() - prev->second->start() );
      prev = it;
    }
    prev->second->setCachedLength( _parent->length() - prev->second->start() );
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
