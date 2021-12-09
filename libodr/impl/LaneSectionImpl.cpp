//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LaneSectionImpl.cpp
//! \brief odr::LaneSection class implementation

#include "LaneSectionImpl.h"
#include "LanesImpl.h"
#include <misc/EnumeratorImpl.h>
#include <odr/Road.h>

using namespace odr;

LaneSectionImpl::LaneSectionImpl( LanesImpl *parent, double nStart ):
_parentLanes  ( parent ),
_reindexNeeded( false ),
_start        ( nStart ),
_length       ( 0.0 ),
_lanes        (),
_lanesLeft    (),
_lanesRight   ()
{
  LaneImpl *center = new LaneImpl( this, 0 );
  _lanesLeft.push_back( center );
  _lanesRight.push_back( center );
  setStart( nStart );
}

LaneSectionImpl::~LaneSectionImpl()
{
  // Remove this section from lanes record...
  LanesImpl::sectionsType &sections = _parentLanes->sections();
  sections.erase( sections.find(_start) );

  // For each lane...
  _lanes.push_back( _lanesRight[0].ptr() );
  while( !_lanes.empty() ) {
    LaneImpl *lane = _lanes.begin()->ptr();

    // Update linkage...
    if( lane->predecessor() )
      lane->predecessor()->setSuccessor( lane->successor() );
    else if( lane->successor() )
      lane->successor()->setPredecessor(0);

    // Delete lane...
    delete lane;
  }

  _parentLanes->setLengthsNeedUpdate();
  setModified();
}

Lanes* LaneSectionImpl::parentLanes( void )
{ return _parentLanes; }

const Lanes* LaneSectionImpl::parentLanes( void ) const
{ return _parentLanes; }

double LaneSectionImpl::start( void ) const
{ return _start; }

double LaneSectionImpl::length( void ) const
{
  _parentLanes->updateLengths();
  return _length;
}

bool LaneSectionImpl::setStart( double nStart )
{
  setModified();
  _parentLanes->setLengthsNeedUpdate();

  LanesImpl::sectionsType &sections = _parentLanes->sections();

  // Find previous position in container...
  LanesImpl::sectionsType::iterator it = sections.find( _start );

  // Update s-coordinate...
  _start = nStart;

  // If already in container...
  if( it!=sections.end() && it->second.ptr()==this )
    // Remove section record from previous position...
    sections.erase(it);

  // Delete the record already at new position if there was one...
  it = sections.find( nStart );
  if( it!=sections.end() )
    delete it->second.ptr();

  // Insert this record at new position...
  sections[ nStart ] = this;

  return true;
}

void LaneSectionImpl::setLength( double v )
{
  // Length delta...
  double delta = std::max(v,0.001)-length();

  // Resize road...
  Road *road = _parentLanes->parentRoad();
  road->setLength( road->length()+delta );

  // If shortening the section...
  if( delta<0.0 ) {
    // Move every section succeeding this one, starting from the next...
    LanesImpl::sectionsType &sections = _parentLanes->sections();
    LanesImpl::sectionsType::iterator it = sections.find(_start);
    if( it!=sections.begin() ) do {
      LanesImpl::sectionsType::iterator it2 = it;
      --it2;
      it2->second->setStart( it2->second->_start+delta );
      --it;
    }
    while( it!=sections.begin() );
  }

  // Else (if not shortening the section)...
  else {
    // Move every primitive succeeding this one, starting from the last...
    LanesImpl::sectionsType &sections = _parentLanes->sections();
    LanesImpl::sectionsType::iterator it = sections.begin();
    if( it->second.ptr()!=this ) {
      it->second->setStart( it->second->_start+delta );
      it = sections.begin();
      do {
        LanesImpl::sectionsType::iterator it2 = it;
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

::misc::Enumerator<Lane*>* LaneSectionImpl::enumerate( void )
{
  return ::misc::newContainerEnumerator(
    _lanes.begin(), _lanes.end(), ::misc::WeakRef2Ptr<Lane>() );
}

::misc::Enumerator<const Lane*>*
LaneSectionImpl::enumerate( void ) const
{
  return ::misc::newContainerEnumerator(
    _lanes.begin(), _lanes.end(), ::misc::WeakRef2Ptr<const Lane>() );
}

unsigned int LaneSectionImpl::countLeft( void ) const
{
  return (unsigned int)_lanesLeft.size()-1;
}

unsigned int LaneSectionImpl::countRight( void ) const
{
  return (unsigned int)_lanesRight.size()-1;
}

Lane* LaneSectionImpl::item( int index )
{
  if( index<=0 && index>-(int)_lanesRight.size() )
    return _lanesRight[ (unsigned int)-index ].ptr();

  if( index>0 && index<(int)_lanesLeft.size() )
    return _lanesLeft[ (unsigned int)index ].ptr();

  return 0;
}

const Lane* LaneSectionImpl::item( int index ) const
{
  if( index<=0 && index>-(int)_lanesRight.size() )
    return _lanesRight[ (unsigned int)-index ].ptr();

  if( index>0 && index<(int)_lanesLeft.size() )
    return _lanesLeft[ (unsigned int)index ].ptr();

  return 0;
}

Lane* LaneSectionImpl::createLane( int index )
{
  if( index==0 ||
      index>(int)_lanesLeft.size() ||
      index<-(int)_lanesRight.size() )
    return 0;

  LaneImpl *result = new LaneImpl( this, index );
  if( index<0 ) {
    _lanes.insert( _lanes.begin() + ((int)_lanesRight.size()+index), result );
    _lanesRight.insert( _lanesRight.begin()-index, result );
  }
  else {
    _lanes.insert( _lanes.begin() + ((int)_lanesRight.size()+index-2), result );
    _lanesLeft.insert( _lanesLeft.begin()+index, result );
  }

  setModified();

  return result;
}

void LaneSectionImpl::reindexLanes( void ) const
{
  if( !_reindexNeeded ) return;

  lanesType::iterator it;
  for( int i=1 ; i<(int)_lanesLeft.size() ; ++i )
    _lanesLeft[i]->setIndex( i );

  for( int i=1 ; i<(int)_lanesRight.size() ; ++i )
    _lanesRight[i]->setIndex( -i );

  _reindexNeeded=false;
}

void LaneSectionImpl::setModified( void )
{ _parentLanes->setModified(); }

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
