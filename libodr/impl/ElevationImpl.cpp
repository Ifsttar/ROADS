//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ElevationImpl.cpp
//! \brief odr::Elevation class implementation

#include "ElevationImpl.h"
#include "ElevationProfileImpl.h"
#include <odr/Road.h>

using namespace odr;

ElevationImpl::ElevationImpl( ElevationProfileImpl *parent, double nStart ) :
::misc::DllImpl<Elevation>(),
::misc::WeakRefServerImpl (),
_parent                   ( parent ),
_start                    ( -1.0 ),
_length                   ( 0.0 ),
_a                        ( 0.0 ),
_b                        ( 0.0 ),
_c                        ( 0.0 ),
_d                        ( 0.0 )
{
  setStart( nStart );
}

ElevationImpl::~ElevationImpl()
{
  setModified();
  _parent->setLengthsNeedUpdate();
}

ElevationProfile* ElevationImpl::parentProfile( void )
{ return _parent; }

const ElevationProfile* ElevationImpl::parentProfile( void ) const
{ return _parent; }

double ElevationImpl::start( void ) const { return _start; }

double ElevationImpl::length( void ) const
{
  _parent->updateLengths();
  return _length;
}

double ElevationImpl::a( void ) const { return _a; }

double ElevationImpl::b( void ) const { return _b; }

double ElevationImpl::c( void ) const { return _c; }

double ElevationImpl::d( void ) const { return _d; }

bool ElevationImpl::setStart( double nStart )
{
  setModified();
  _parent->setLengthsNeedUpdate();

  ElevationProfileImpl::elevationsType &elevations = _parent->elevations();

  // Find previous position in container...
  ElevationProfileImpl::elevationsType::iterator it=elevations.find( _start );

  // Update s-coordinate...
  _start = nStart;

  // If already in container...
  if( it!=elevations.end() && it->second.ptr()==this )
    // Remove elevation record from previous position...
    elevations.erase(it);

  // Delete the record already at new position if there was one...
  it = elevations.find( nStart );
  if( it!=elevations.end() )
    delete it->second.ptr();

  // Insert this record at new position...
  elevations[ nStart ] = this;

  return true;
}

void ElevationImpl::setLength( double v )
{
  // Length delta...
  double delta = std::max(v,0.001)-length();

  // Lane section...
  Road *road = _parent->parentRoad();
  road->setLength( road->length()+delta );

  // If shortening the elevation record...
  if( delta<0.0 ) {
    // Move every elevation succeeding this one, starting from the next...
    ElevationProfileImpl::elevationsType &elevations = _parent->elevations();
    ElevationProfileImpl::elevationsType::iterator it = elevations.find(_start);
    if( it!=elevations.begin() ) do {
      ElevationProfileImpl::elevationsType::iterator it2 = it;
      --it2;
      it2->second->setStart( it2->second->_start+delta );
      --it;
    }
    while( it!=elevations.begin() );
  }

  // Else (if not shortening the elevation record)...
  else {
    // Move every primitive succeeding this one, starting from the last...
    ElevationProfileImpl::elevationsType &elevations = _parent->elevations();
    ElevationProfileImpl::elevationsType::iterator it = elevations.begin();
    if( it->second.ptr()!=this ) {
      it->second->setStart( it->second->_start+delta );
      it = elevations.begin();
      do {
        ElevationProfileImpl::elevationsType::iterator it2 = it;
        ++it2;
        if( it2->second.ptr()!=this )
          it2->second->setStart( it2->second->_start+delta );
        ++it;
      }
      while( it->second.ptr()!=this );
    }
  }

  // Propagate modification...
  setModified();
}

void ElevationImpl::setA( double v ) { _a=v; setModified(); }

void ElevationImpl::setB( double v ) { _b=v; setModified(); }

void ElevationImpl::setC( double v ) { _c=v; setModified(); }

void ElevationImpl::setD( double v ) { _d=v; setModified(); }

void ElevationImpl::setModified( void ) { _parent->setModified(); }

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
