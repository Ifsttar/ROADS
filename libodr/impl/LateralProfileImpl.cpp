//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LateralProfileImpl.cpp
//! \brief odr::LateralProfile class implementation

#include "LateralProfileImpl.h"
#include "RoadImpl.h"
#include <misc/EnumeratorImpl.h>

using namespace odr;

LateralProfileImpl::LateralProfileImpl( RoadImpl *parent ) :
::misc::DllImpl<LateralProfile>(),
::misc::WeakRefServerImpl      (),
_parent                        ( parent ),
_superelevations               (),
_crossfalls                    (),
_updateLengths                 ( true )
{}

LateralProfileImpl::~LateralProfileImpl()
{
  while( !_superelevations.empty() )
    delete _superelevations.begin()->second.ptr();

  setModified();
}

Road* LateralProfileImpl::parentRoad( void )
{ return _parent; }

const Road* LateralProfileImpl::parentRoad( void ) const
{ return _parent; }

::misc::Enumerator<Superelevation*>*
LateralProfileImpl::enumerateSuperelevations( void )
{
  return ::misc::newContainerEnumerator(
    _superelevations.rbegin(), _superelevations.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<Superelevation>,
      ::misc::ExtractSecond<::misc::WeakRefCradle<SuperelevationImpl> > >() );
}

::misc::Enumerator<const Superelevation*>*
LateralProfileImpl::enumerateSuperelevations( void ) const
{
  return ::misc::newContainerEnumerator(
    _superelevations.rbegin(), _superelevations.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<const Superelevation>,
      ::misc::ExtractSecond<::misc::WeakRefCradle<SuperelevationImpl> > >() );
}

unsigned int LateralProfileImpl::superelevationCount( void ) const
{ return (unsigned int)_superelevations.size(); }

Superelevation* LateralProfileImpl::superelevationItem( unsigned int index )
{
  superelevationsType::reverse_iterator it=_superelevations.rbegin();
  while( it!=_superelevations.rend() && index ) { --index; ++it; }
  if( it==_superelevations.rend() ) return 0;
  return it->second.ptr();
}

const Superelevation*
LateralProfileImpl::superelevationItem( unsigned int index ) const
{
  superelevationsType::const_reverse_iterator it=_superelevations.rbegin();
  while( it!=_superelevations.rend() && index ) { --index; ++it; }
  if( it==_superelevations.rend() ) return 0;
  return it->second.ptr();
}

Superelevation* LateralProfileImpl::superelevationAt( double s )
{
  if( _superelevations.empty() ) return 0;
  superelevationsType::iterator it=_superelevations.lower_bound(s);
  if( it==_superelevations.end() )
    return _superelevations.rbegin()->second.ptr();
  return it->second.ptr();
}

const Superelevation* LateralProfileImpl::superelevationAt( double s ) const
{
  if( _superelevations.empty() ) return 0;
  superelevationsType::const_iterator it=_superelevations.lower_bound(s);
  if( it==_superelevations.end() )
    return _superelevations.rbegin()->second.ptr();
  return it->second.ptr();
}

Superelevation* LateralProfileImpl::createSuperelevation( double start )
{
  SuperelevationImpl *result = new SuperelevationImpl( this, start );
  setModified();
  return result;
}

::misc::Enumerator<Crossfall*>*
LateralProfileImpl::enumerateCrossfalls( void )
{
  return ::misc::newContainerEnumerator(
    _crossfalls.rbegin(), _crossfalls.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<Crossfall>,
      ::misc::ExtractSecond<::misc::WeakRefCradle<CrossfallImpl> > >() );
}

::misc::Enumerator<const Crossfall*>*
LateralProfileImpl::enumerateCrossfalls( void ) const
{
  return ::misc::newContainerEnumerator(
    _crossfalls.rbegin(), _crossfalls.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<const Crossfall>,
      ::misc::ExtractSecond<::misc::WeakRefCradle<CrossfallImpl> > >() );
}

unsigned int LateralProfileImpl::crossfallCount( void ) const
{ return (unsigned int)_crossfalls.size(); }

Crossfall* LateralProfileImpl::crossfallItem( unsigned int index )
{
  crossfallsType::reverse_iterator it=_crossfalls.rbegin();
  while( it!=_crossfalls.rend() && index ) { --index; ++it; }
  if( it==_crossfalls.rend() ) return 0;
  return it->second.ptr();
}

const Crossfall* LateralProfileImpl::crossfallItem( unsigned int index ) const
{
  crossfallsType::const_reverse_iterator it=_crossfalls.rbegin();
  while( it!=_crossfalls.rend() && index ) { --index; ++it; }
  if( it==_crossfalls.rend() ) return 0;
  return it->second.ptr();
}

Crossfall* LateralProfileImpl::crossfallAt( double s )
{
  if( _crossfalls.empty() ) return 0;
  crossfallsType::iterator it=_crossfalls.lower_bound(s);
  if( it==_crossfalls.end() )
    return _crossfalls.rbegin()->second.ptr();
  return it->second.ptr();
}

const Crossfall* LateralProfileImpl::crossfallAt( double s ) const
{
  if( _crossfalls.empty() ) return 0;
  crossfallsType::const_iterator it=_crossfalls.lower_bound(s);
  if( it==_crossfalls.end() )
    return _crossfalls.rbegin()->second.ptr();
  return it->second.ptr();
}

Crossfall* LateralProfileImpl::createCrossfall( double start )
{
  CrossfallImpl *result = new CrossfallImpl( this, start );
  setModified();
  return result;
}

void LateralProfileImpl::setModified( void ) { _parent->setModified(); }

void LateralProfileImpl::updateLengths( void ) const
{
  if( !_updateLengths ) return;
  _updateLengths = false;

  if( !_crossfalls.empty() )
  {
    crossfallsType::const_reverse_iterator prev = crossfallsType::const_reverse_iterator();
    for( crossfallsType::const_reverse_iterator it=_crossfalls.rbegin() ;
         it!=_crossfalls.rend() ; ++it )
    {
      if( it!=_crossfalls.rbegin() )
        prev->second->setCachedLength( it->second->start() - prev->second->start() );
      prev = it;
    }
    prev->second->setCachedLength( _parent->length() - prev->second->start() );
  }

  if( !_superelevations.empty() )
  {
    superelevationsType::const_reverse_iterator prev = superelevationsType::const_reverse_iterator();
    for( superelevationsType::const_reverse_iterator it=_superelevations.rbegin() ;
         it!=_superelevations.rend() ; ++it )
    {
      if( it!=_superelevations.rbegin() )
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
