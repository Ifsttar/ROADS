//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LaneWidthImpl.cpp
//! \brief odr::LaneWidth class implementation

#include "LaneWidthImpl.h"
#include "LaneImpl.h"
#include <odr/LaneSection.h>

using namespace odr;

LaneWidthImpl::LaneWidthImpl( LaneImpl *parent, double nStartOffset ) :
_parentLane ( parent ),
_startOffset( -1.0 ),
_a          ( 0.0 ),
_b          ( 0.0 ),
_c          ( 0.0 ),
_d          ( 0.0 )
{
  setStartOffset(nStartOffset);
}

ODR_CALL LaneWidthImpl::~LaneWidthImpl()
{
  setModified();
  _parentLane->setLengthsNeedUpdate();
}

Lane* ODR_CALL LaneWidthImpl::parentLane( void )
{ return _parentLane; }

const Lane* ODR_CALL LaneWidthImpl::parentLane( void ) const
{ return _parentLane; }

double ODR_CALL LaneWidthImpl::startOffset( void ) const
{ return _startOffset; }

double ODR_CALL LaneWidthImpl::length( void ) const
{
  _parentLane->updateLengths();
  return _length;
}

double ODR_CALL LaneWidthImpl::a( void ) const
{ return _a; }

double ODR_CALL LaneWidthImpl::b( void ) const
{ return _b; }

double ODR_CALL LaneWidthImpl::c( void ) const
{ return _c; }

double ODR_CALL LaneWidthImpl::d( void ) const
{ return _d; }

bool ODR_CALL LaneWidthImpl::setStartOffset( double nStartOffset )
{
  LaneImpl::laneWidthsType &widths = _parentLane->laneWidths();

  setModified();
  _parentLane->setLengthsNeedUpdate();

  // Find previous position in container...
  LaneImpl::laneWidthsType::iterator it = widths.find( _startOffset );

  // Update s-coordinate offset...
  _startOffset = nStartOffset;

  // If already in container...
  if( it!=widths.end() && it->second.ptr()==this )
    // Remove width record from previous position...
    widths.erase(it);

  // Delete the record already at new position if there was one...
  it = widths.find( nStartOffset );
  if( it!=widths.end() )
    delete it->second.ptr();

  // Insert this record at new position...
  widths[ nStartOffset ] = this;

  return true;
}

void ODR_CALL LaneWidthImpl::setLength( double v )
{
  // Length delta...
  double delta = std::max(v,0.001)-length();

  // Lane section...
  LaneSection *section = _parentLane->parentSection();
  section->setLength( section->length()+delta );

  // If shortening the width record...
  if( delta<0.0 ) {
    // Move every width succeeding this one, starting from the next...
    LaneImpl::laneWidthsType &widths = _parentLane->laneWidths();
    LaneImpl::laneWidthsType::iterator it = widths.find(_startOffset);
    if( it!=widths.begin() ) do {
      LaneImpl::laneWidthsType::iterator it2 = it;
      --it2;
      it2->second->setStartOffset( it2->second->_startOffset+delta );
      --it;
    }
    while( it!=widths.begin() );
  }

  // Else (if not shortening the primitive)...
  else {
    // Move every primitive succeeding this one, starting from the last...
    LaneImpl::laneWidthsType &widths = _parentLane->laneWidths();
    LaneImpl::laneWidthsType::iterator it = widths.begin();
    if( it->second.ptr()!=this ) {
      it->second->setStartOffset( it->second->_startOffset+delta );
      it = widths.begin();
      do {
        LaneImpl::laneWidthsType::iterator it2 = it;
        ++it2;
        if( it2->second.ptr()!=this )
          it2->second->setStartOffset( it2->second->_startOffset+delta );
        ++it;
      }
      while( it->second.ptr()!=this );
    }
  }

  // Propagate modification...
  setModified();
}

void ODR_CALL LaneWidthImpl::setA( double v )
{ _a=v; setModified(); }

void ODR_CALL LaneWidthImpl::setB( double v )
{ _b=v; setModified(); };

void ODR_CALL LaneWidthImpl::setC( double v )
{ _c=v; setModified(); }

void ODR_CALL LaneWidthImpl::setD( double v )
{ _d=v; setModified(); }

void LaneWidthImpl::setModified( void )
{ _parentLane->setModified(); }

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
