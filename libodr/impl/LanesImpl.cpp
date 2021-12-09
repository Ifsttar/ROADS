//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LanesImpl.cpp
//! \brief odr::Lanes class implementation

#include "LanesImpl.h"
#include "RoadImpl.h"
#include "LaneImpl.h"
#include <misc/EnumeratorImpl.h>

using namespace odr;

LanesImpl::LanesImpl( RoadImpl *parent ):
::misc::DllImpl<Lanes>   (),
::misc::WeakRefServerImpl(),
_parent                  ( parent ),
_sections                (),
_updateLengths           ( true )
{
  createSection(0.0);
}

LanesImpl::~LanesImpl()
{
  while( !_sections.empty() )
    delete _sections.begin()->second.ptr();
  setModified();
}

Road* LanesImpl::parentRoad( void )
{ return _parent; }

const Road* LanesImpl::parentRoad( void ) const
{ return _parent; }

::misc::Enumerator<LaneSection*>* LanesImpl::enumerate( void )
{
  return ::misc::newContainerEnumerator(
    _sections.rbegin(), _sections.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<LaneSection>,
      ::misc::ExtractSecond<::misc::WeakRefCradle<LaneSectionImpl> > >() );
}

::misc::Enumerator<const LaneSection*>*
LanesImpl::enumerate( void ) const
{
  return ::misc::newContainerEnumerator(
    _sections.rbegin(), _sections.rend(),
    ::misc::Compose<
      ::misc::WeakRef2Ptr<const LaneSection>,
      ::misc::ExtractSecond<::misc::WeakRefCradle<LaneSectionImpl> > >() );
}

unsigned int LanesImpl::count( void ) const
{ return (unsigned int)_sections.size(); }

LaneSection* LanesImpl::item( unsigned int index )
{
  sectionsType::reverse_iterator it=_sections.rbegin();
  while( it!=_sections.rend() && index ) { --index; ++it; }
  if( it==_sections.rend() ) return 0;
  return it->second.ptr();
}

const LaneSection* LanesImpl::item( unsigned int index ) const
{
  sectionsType::const_reverse_iterator it=_sections.rbegin();
  while( it!=_sections.rend() && index ) { --index; ++it; }
  if( it==_sections.rend() ) return 0;
  return it->second.ptr();
}

LaneSection* LanesImpl::sectionAt( double s )
{
  sectionsType::const_iterator it=_sections.lower_bound(s);
  if( it==_sections.end() )
    return _sections.rbegin()->second.ptr();
  return it->second.ptr();
}

const LaneSection* LanesImpl::sectionAt( double s ) const
{
  sectionsType::const_iterator it=_sections.lower_bound(s);
  if( it==_sections.end() )
    return _sections.rbegin()->second.ptr();
  return it->second.ptr();
}

LaneSection* LanesImpl::createSection( double start )
{
  LaneSection *result = new LaneSectionImpl( this, start );
  setModified();
  return result;
}

void LanesImpl::setModified( void )
{ _parent->setModified(); }

void LanesImpl::setLengthsNeedUpdate( void )
{
  _updateLengths=true;
  for( sectionsType::iterator si=_sections.begin() ; si!=_sections.end() ; ++si ) {
    LaneSectionImpl::lanesType &lanes = si->second->lanes();
    for( LaneSectionImpl::lanesType::iterator li=lanes.begin() ; li!=lanes.end() ; ++li )
      (*li)->setLengthsNeedUpdate();
  }
}

void LanesImpl::updateLengths( void ) const
{
  if( !_updateLengths ) return;
  _updateLengths = false;

  if( !_sections.empty() ) {
    sectionsType::const_reverse_iterator prev = sectionsType::const_reverse_iterator();
    for( sectionsType::const_reverse_iterator it=_sections.rbegin() ;
         it!=_sections.rend() ; ++it )
    {
      if( it!=_sections.rbegin() )
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
