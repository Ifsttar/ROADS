//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file JunctionImpl.cpp
//! \brief odr::Junction class implementation

#include "JunctionImpl.h"
#include "RoadImpl.h"
#include "DocumentImpl.h"
#include <misc/EnumeratorImpl.h>

using namespace odr;

JunctionImpl::JunctionImpl( DocumentImpl *document, unsigned int id ) :
DllImpl<Junction>(),
ElementImpl      ( document, id ),
_roads           ()
{}

ODR_CALL JunctionImpl::~JunctionImpl()
{
  while( !_roads.empty() )
    delete _roads.begin()->ptr();
}

Element::Type JunctionImpl::type( void ) const
{ return T_JUNCTION; }

Road* JunctionImpl::createRoad( void )
{
  Road *result = dynamic_cast<DocumentImpl*>(document())
    ->createRoadInJunction(this);
  if( result ) _roads.push_back( result );
  return result;
}

unsigned int JunctionImpl::count( void ) const
{ return (unsigned int)_roads.size(); }

::misc::Enumerator<Road*>* JunctionImpl::enumerate( void )
{
  return ::misc::newContainerEnumerator(
    _roads.begin(), _roads.end(), ::misc::WeakRef2Ptr<Road>() );
}

::misc::Enumerator<const Road*>* JunctionImpl::enumerate( void ) const
{
  return ::misc::newContainerEnumerator(
    _roads.begin(), _roads.end(), ::misc::WeakRef2Ptr<const Road>() );
}

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
