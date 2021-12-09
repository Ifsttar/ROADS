//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file SuperelevationImpl.cpp
//! \brief odr::Superelevation class implementation

#include "SuperelevationImpl.h"
#include "LateralProfileImpl.h"
#include <odr/Road.h>

using namespace odr;

SuperelevationImpl::SuperelevationImpl(
  LateralProfileImpl *parent, double nStart ) :
::misc::DllImpl<Superelevation>(),
::misc::WeakRefServerImpl      (),
_parent                        ( parent ),
_start                         ( -1.0 ),
_length                        ( 0.0 ),
_a                             ( 0.0 ),
_b                             ( 0.0 ),
_c                             ( 0.0 ),
_d                             ( 0.0 )
{
  setStart( nStart );
}

SuperelevationImpl::~SuperelevationImpl()
{ 
  setModified();
  _parent->setLengthsNeedUpdate();
}

LateralProfile* SuperelevationImpl::parentProfile( void )
{ return _parent; }

const LateralProfile* SuperelevationImpl::parentProfile( void ) const
{ return _parent; }

double SuperelevationImpl::start( void ) const { return _start; }

double SuperelevationImpl::length( void ) const
{
  _parent->updateLengths();
  return _length;
}

double SuperelevationImpl::a( void ) const { return _a; }

double SuperelevationImpl::b( void ) const { return _b; }

double SuperelevationImpl::c( void ) const { return _c; }

double SuperelevationImpl::d( void ) const { return _d; }

bool SuperelevationImpl::setStart( double nStart )
{
  setModified();
  _parent->setLengthsNeedUpdate();

  LateralProfileImpl::superelevationsType &superelevations = 
    _parent->superelevations();

  // Find previous position in container...
  LateralProfileImpl::superelevationsType::iterator it =
    superelevations.find( _start );

  // Update s-coordinate...
  _start = nStart;

  // If already in container...
  if( it!=superelevations.end() && it->second.ptr()==this )
    // Remove superelevation record from previous position...
    superelevations.erase(it);

  // Delete the record already at new position if there was one...
  it = superelevations.find( nStart );
  if( it!=superelevations.end() )
    delete it->second.ptr();

  // Insert this record at new position...
  superelevations[ nStart ] = this;

  return true;
}

void SuperelevationImpl::setLength( double v )
{
  // Length delta...
  double delta = std::max(v,0.001)-length();

  // Lane section...
  Road *road = _parent->parentRoad();
  road->setLength( road->length()+delta );

  // If shortening the superelevation record...
  if( delta<0.0 ) {
    // Move every superelevation succeeding this one, starting from the next...
    LateralProfileImpl::superelevationsType &superelevations = _parent->superelevations();
    LateralProfileImpl::superelevationsType::iterator it = superelevations.find(_start);
    if( it!=superelevations.begin() ) do {
      LateralProfileImpl::superelevationsType::iterator it2 = it;
      --it2;
      it2->second->setStart( it2->second->_start+delta );
      --it;
    }
    while( it!=superelevations.begin() );
  }

  // Else (if not shortening the primitive)...
  else {
    // Move every primitive succeeding this one, starting from the last...
    LateralProfileImpl::superelevationsType &superelevations = _parent->superelevations();
    LateralProfileImpl::superelevationsType::iterator it = superelevations.begin();
    if( it->second.ptr()!=this ) {
      it->second->setStart( it->second->_start+delta );
      it = superelevations.begin();
      do {
        LateralProfileImpl::superelevationsType::iterator it2 = it;
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

void SuperelevationImpl::setA( double v ) { _a=v; setModified(); }

void SuperelevationImpl::setB( double v ) { _b=v; setModified(); }

void SuperelevationImpl::setC( double v ) { _c=v; setModified(); }

void SuperelevationImpl::setD( double v ) { _d=v; setModified(); }

void SuperelevationImpl::setModified( void ) { _parent->setModified(); }

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
