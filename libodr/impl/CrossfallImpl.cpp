//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file CrossfallImpl.cpp
//! \brief odr::Crossfall class implementation

#include "CrossfallImpl.h"
#include "LateralProfileImpl.h"
#include <odr/Road.h>

using namespace odr;

CrossfallImpl::CrossfallImpl( LateralProfileImpl *parent, double nStart ) :
::misc::DllImpl<Crossfall>(),
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

CrossfallImpl::~CrossfallImpl()
{
  setModified();
  _parent->setLengthsNeedUpdate();
}

LateralProfile* CrossfallImpl::parentProfile( void )
{ return _parent; }

const LateralProfile* CrossfallImpl::parentProfile( void ) const
{ return _parent; }

double CrossfallImpl::start( void ) const { return _start; }

double CrossfallImpl::length( void ) const
{
  _parent->updateLengths();
  return _length;
}

double CrossfallImpl::a( void ) const { return _a; }

double CrossfallImpl::b( void ) const { return _b; }

double CrossfallImpl::c( void ) const { return _c; }

double CrossfallImpl::d( void ) const { return _d; }

bool CrossfallImpl::setStart( double nStart )
{
  setModified();
  _parent->setLengthsNeedUpdate();

  LateralProfileImpl::crossfallsType &crossfalls = 
    _parent->crossfalls();

  // Find previous position in container...
  LateralProfileImpl::crossfallsType::iterator it =
    crossfalls.find( _start );

  // Update s-coordinate...
  _start = nStart;

  // If already in container...
  if( it!=crossfalls.end() && it->second.ptr()==this )
    // Remove crossfall record from previous position...
    crossfalls.erase(it);

  // Delete the record already at new position if there was one...
  it = crossfalls.find( nStart );
  if( it!=crossfalls.end() )
    delete it->second.ptr();

  // Insert this record at new position...
  crossfalls[ nStart ] = this;

  return true;
}

void CrossfallImpl::setLength( double v )
{
  // Length delta...
  double delta = std::max(v,0.001)-length();

  // Lane section...
  Road *road = _parent->parentRoad();
  road->setLength( road->length()+delta );

  // If shortening the crossfall record...
  if( delta<0.0 ) {
    // Move every crossfall succeeding this one, starting from the next...
    LateralProfileImpl::crossfallsType &crossfalls = _parent->crossfalls();
    LateralProfileImpl::crossfallsType::iterator it = crossfalls.find(_start);
    if( it!=crossfalls.begin() ) do {
      LateralProfileImpl::crossfallsType::iterator it2 = it;
      --it2;
      it2->second->setStart( it2->second->_start+delta );
      --it;
    }
    while( it!=crossfalls.begin() );
  }

  // Else (if not shortening the crossfall record)...
  else {
    // Move every primitive succeeding this one, starting from the last...
    LateralProfileImpl::crossfallsType &crossfalls = _parent->crossfalls();
    LateralProfileImpl::crossfallsType::iterator it = crossfalls.begin();
    if( it->second.ptr()!=this ) {
      it->second->setStart( it->second->_start+delta );
      it = crossfalls.begin();
      do {
        LateralProfileImpl::crossfallsType::iterator it2 = it;
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

void CrossfallImpl::setA( double v ) { _a=v; setModified(); }

void CrossfallImpl::setB( double v ) { _b=v; setModified(); }

void CrossfallImpl::setC( double v ) { _c=v; setModified(); }

void CrossfallImpl::setD( double v ) { _d=v; setModified(); }

void CrossfallImpl::setModified( void ) { _parent->setModified(); }

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
