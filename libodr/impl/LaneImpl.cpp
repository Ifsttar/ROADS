//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LaneImpl.cpp
//! \brief odr::Lane class implementation

#include "LaneImpl.h"
#include "LaneSectionImpl.h"
#include "LanesImpl.h"
#include <odr/Road.h>
#include <misc/EnumeratorImpl.h>

using namespace odr;

LaneImpl::LaneImpl( LaneSectionImpl *parent, int index ):
_parentSection( parent ),
_index        ( 0 ),
_type         ( "" ),
_level        ( 0 ),
_predecessor  ( 0 ),
_successor    ( 0 ),
_laneWidths   (),
_updateLengths( true )
{
  // Siblings must be reindexed...
  _parentSection->setReindexNeeded();
}

ODR_CALL LaneImpl::~LaneImpl()
{
  // Delete width records...
  while( !_laneWidths.empty() )
    delete _laneWidths.begin()->second.ptr();

  // Siblings must be reindexed...
  _parentSection->setReindexNeeded();

  setModified();
}

LaneSection* ODR_CALL LaneImpl::parentSection( void )
{ return _parentSection; }

const LaneSection* ODR_CALL LaneImpl::parentSection( void ) const
{ return _parentSection; }

int LaneImpl::index( void ) const
{
  _parentSection->reindexLanes();
  return _index;
}

const char * ODR_CALL LaneImpl::type( void ) const
{ return _type.c_str(); }

void ODR_CALL LaneImpl::setType( const char *nType )
{ if(nType) _type=nType; else _type=""; }

bool ODR_CALL LaneImpl::level( void ) const
{ return _level; }

void ODR_CALL LaneImpl::setLevel( bool nLevel )
{ _level=nLevel; }

Lane* ODR_CALL LaneImpl::predecessor( void )
{ return _predecessor.ptr(); }

const Lane* ODR_CALL LaneImpl::predecessor( void ) const
{ return _predecessor.ptr(); }

bool ODR_CALL LaneImpl::setPredecessor( Lane* nPredecessor )
{
  LaneImpl *pred = dynamic_cast<LaneImpl*>( nPredecessor );

  setModified();

  // Disconnecting lanes always works...
  if( !pred ) {
    _predecessor=0;
    return true;
  }

  // Linking lanes of the same section is forbidden...
  if( pred->_parentSection==_parentSection )
    return false;

  // Retrieve pointer to lanes...
  LanesImpl *lanes =
    dynamic_cast<LanesImpl*>( _parentSection->parentLanes() );

  // If lanes are part of the same road...
  if( pred->_parentSection->parentLanes() == (Lanes*)lanes )
  {
    // Check that lanes are in consecutive sections...
    LanesImpl::sectionsType::const_iterator it =
      lanes->sections().find( pred->_parentSection->start() );
    if( it==lanes->sections().end() || (++it)==lanes->sections().end() )
      return false;    
    if( it->second.ptr()!=_parentSection )
      return false;

    // Check OK, perform linkage...
    _predecessor = pred;
    return true;
  }

  // New predecessor is on a different road.
  // Check this lane is at the beginning of the road...
  if( lanes->sections().begin()->second.ptr() != _parentSection )
    return false;

  // Retrieve pointer to other lane's LanesImpl...
  LanesImpl *predLanes=
    dynamic_cast<LanesImpl*>( pred->_parentSection->parentLanes() );

  // Fail if roads are not connected together...
  //if( lanes->parentRoad()->predecessor() != predLanes->parentRoad() )
  //  return false;

  // If new predecessor's road's sucessor is this lane's road and
  // new predecessor is not at the end of the road, fail...
  if( predLanes->parentRoad()->successor() == lanes->parentRoad() &&
      predLanes->sections().rbegin()->second.ptr() != pred->_parentSection )
    return false;

  // If new predecessor's road's predecessor is this lane's road and
  // new predecessor is not at the beginning of the road, fail...
  if( predLanes->parentRoad()->predecessor() == lanes->parentRoad() &&
      predLanes->sections().begin()->second.ptr() != pred->_parentSection )
    return false;

  // Perform linkage...
  _predecessor = pred;
  return true;
}

Lane* ODR_CALL LaneImpl::successor( void )
{ return _successor.ptr(); }

const Lane* ODR_CALL LaneImpl::successor( void ) const
{ return _successor.ptr(); }

bool ODR_CALL LaneImpl::setSuccessor( Lane* nSuccessor )
{
  LaneImpl *next = dynamic_cast<LaneImpl*>( nSuccessor );

  setModified();

  // Disconnecting lanes always works...
  if( !next ) {
    _successor=0;
    return true;
  }

  // Linking lanes of the same section is forbidden...
  if( next->_parentSection==_parentSection )
    return false;

  // Retrieve pointer to lanes...
  LanesImpl *lanes =
    dynamic_cast<LanesImpl*>( _parentSection->parentLanes() );

  // If lanes are part of the same road...
  if( next->_parentSection->parentLanes() == (Lanes*)lanes )
  {
    // Check that lanes are in consecutive sections...
    LanesImpl::sectionsType::const_iterator it =
      lanes->sections().find( _parentSection->start() );
    if( it==lanes->sections().end() || (++it)==lanes->sections().end() )
      return false;    
    if( it->second.ptr()!=next->_parentSection )
      return false;

    // Check OK, perform linkage...
    _successor = next;
    return true;
  }

  // New successor is on a different road.
  // Check this lane is at the end of the road...
  if( lanes->sections().rbegin()->second.ptr() != _parentSection )
    return false;

  // Retrieve pointer to other lane's LanesImpl...
  LanesImpl *nextLanes=
    dynamic_cast<LanesImpl*>( next->_parentSection->parentLanes() );

  // Fail if roads are not connected together...
  //if( lanes->parentRoad()->successor() != nextLanes->parentRoad() )
  //  return false;

  // If new successor's road's predecessor is this lane's road and
  // new successor is not at the beginning of the road, fail...
  if( nextLanes->parentRoad()->predecessor() == lanes->parentRoad() &&
      nextLanes->sections().begin()->second.ptr() != next->_parentSection )
    return false;

  // If new successor's road's successor is this lane's road and
  // new successor is not at the end of the road, faile...
  if( nextLanes->parentRoad()->successor() == lanes->parentRoad() &&
      nextLanes->sections().rbegin()->second.ptr() != next->_parentSection )
    return false;

  // Perform linkage...
  _successor = next;
  return true;
}

::misc::Enumerator<LaneWidth*>* ODR_CALL LaneImpl::enumerateWidths( void )
{
  return ::misc::newContainerEnumerator(
    _laneWidths.rbegin(), _laneWidths.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<LaneWidth>,
      ::misc::ExtractSecond<::misc::WeakRefCradle<LaneWidthImpl> > >()  );
}

::misc::Enumerator<const LaneWidth*>* ODR_CALL
LaneImpl::enumerateWidths( void ) const
{
  return ::misc::newContainerEnumerator(
    _laneWidths.rbegin(), _laneWidths.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<const LaneWidth>,
      ::misc::ExtractSecond<::misc::WeakRefCradle<LaneWidthImpl> > >()  );
}

unsigned int ODR_CALL LaneImpl::widthCount( void ) const
{ return (unsigned int)_laneWidths.size(); }

LaneWidth* ODR_CALL LaneImpl::widthItem( unsigned int index )
{
  laneWidthsType::reverse_iterator it = _laneWidths.rbegin();
  while( it!=_laneWidths.rend() && index ) { ++it; --index; }
  if( it==_laneWidths.rend() ) return 0;
  return it->second.ptr();
}

const LaneWidth* ODR_CALL LaneImpl::widthItem( unsigned int index ) const
{
  laneWidthsType::const_reverse_iterator it = _laneWidths.rbegin();
  while( it!=_laneWidths.rend() && index ) { ++it; --index; }
  if( it==_laneWidths.rend() ) return 0;
  return it->second.ptr();
}

LaneWidth* ODR_CALL LaneImpl::widthAt( double sOffset )
{
  if( _laneWidths.empty() ) return 0;
  laneWidthsType::iterator it=_laneWidths.lower_bound(sOffset);
  if( it==_laneWidths.end() )
    return _laneWidths.rbegin()->second.ptr();
  return it->second.ptr();
}

const LaneWidth* ODR_CALL LaneImpl::widthAt( double sOffset ) const
{
  if( _laneWidths.empty() ) return 0;
  laneWidthsType::const_iterator it=_laneWidths.lower_bound(sOffset);
  if( it==_laneWidths.end() )
    return _laneWidths.rbegin()->second.ptr();
  return it->second.ptr();
}

LaneWidth* LaneImpl::createWidth( double sOffset )
{
  return new LaneWidthImpl( this, sOffset );
}

void LaneImpl::setModified( void )
{ _parentSection->setModified(); }

void LaneImpl::updateLengths( void ) const
{
  if( !_updateLengths ) return;
  _updateLengths = false;
  if( !_laneWidths.empty() ) {
    laneWidthsType::const_reverse_iterator prev = laneWidthsType::const_reverse_iterator();
    for( laneWidthsType::const_reverse_iterator it=_laneWidths.rbegin() ;
         it!=_laneWidths.rend() ; ++it )
    {
      if( it!=_laneWidths.rbegin() )
        prev->second->setCachedLength( it->second->startOffset() - prev->second->startOffset() );
      prev = it;
    }
    prev->second->setCachedLength( _parentSection->length() - prev->second->startOffset() );
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
