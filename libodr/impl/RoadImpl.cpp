//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file RoadImpl.cpp
//! \brief Road class implementation

#include "RoadImpl.h"
#include "GeometryImpl.h"
#include "LanesImpl.h"
#include "ElevationProfileImpl.h"
#include "LateralProfileImpl.h"

using namespace odr;

RoadImpl::RoadImpl( DocumentImpl *document,
                    unsigned int id,
                    Junction *junction ) :
DllImpl<Road>  (),
ElementImpl    ( document, id ),
_length        ( 0 ),
_predecessor   ( 0 ),
_predecessorEnd( Road::CP_END ),
_successor     ( 0 ),
_successorEnd  ( Road::CP_START ),
_junction      ( junction ),
_neighbor      ( 0 ),
_ngbrSameDir   ( true ),
_geometry      ( 0 ),
_lanes         ( 0 ),
_elevationProf ( 0 ),
_lateralProf   ( 0 )
{
  _geometry      = new GeometryImpl(this);
  _lanes         = new LanesImpl(this);
  _elevationProf = new ElevationProfileImpl(this);
  _lateralProf   = new LateralProfileImpl(this);
}

RoadImpl::~RoadImpl() {
  delete dynamic_cast<GeometryImpl*>(_geometry.ptr());
  delete dynamic_cast<LanesImpl*>(_lanes.ptr());
  delete dynamic_cast<ElevationProfileImpl*>(_elevationProf.ptr());
  delete dynamic_cast<LateralProfileImpl*>(_lateralProf.ptr());
}

Element::Type RoadImpl::type( void ) const {
  return Element::T_ROAD;
}

double RoadImpl::length( void ) const
{ return _length; }

void RoadImpl::setLength( double length )
{
  _length = length;
  setModified();
  dynamic_cast<GeometryImpl*>(_geometry.ptr())->setLengthsNeedUpdate();
  dynamic_cast<LanesImpl*>(_lanes.ptr())->setLengthsNeedUpdate();
  dynamic_cast<ElevationProfileImpl*>(_elevationProf.ptr())->setLengthsNeedUpdate();
  dynamic_cast<LateralProfileImpl*>(_lateralProf.ptr())->setLengthsNeedUpdate();
}

Junction* RoadImpl::junction( void )
{ return _junction.ptr(); }

const Junction* RoadImpl::junction( void ) const
{ return _junction.ptr(); }

Element* RoadImpl::predecessor( void )
{ return _predecessor.ptr(); }

const Element* RoadImpl::predecessor( void ) const
{ return _predecessor.ptr(); }

Road::ContactPointType
RoadImpl::predecessorContactPoint( void ) const
{ return _predecessorEnd; }

void RoadImpl::setPredecessor(
  Element* pred, Road::ContactPointType end )
{
  _predecessor = pred;
  _predecessorEnd = end;
}

Element* RoadImpl::successor( void )
{ return _successor.ptr(); }

const Element* RoadImpl::successor( void ) const
{ return _successor.ptr(); }

Road::ContactPointType RoadImpl::successorContactPoint( void ) const
{ return _successorEnd; }

void RoadImpl::setSuccessor(
  Element* succ, Road::ContactPointType end )
{
  _successor = succ;
  _successorEnd = end;
}

Road* RoadImpl::neighbor( void )
{ return _neighbor.ptr(); }

const Road* RoadImpl::neighbor( void ) const
{ return _neighbor.ptr(); }

bool RoadImpl::neighborIsSameDirection( void ) const
{ return _ngbrSameDir; }

void RoadImpl::setNeighbor( Road* nNeighbor, bool sameDirection )
{
  _neighbor = dynamic_cast<RoadImpl*>( nNeighbor );
  _ngbrSameDir = sameDirection;
}

Geometry* RoadImpl::geometry( void )
{ return _geometry.ptr(); }

const Geometry* ODR_CALL RoadImpl::geometry( void ) const
{ return _geometry.ptr(); }

Lanes* RoadImpl::lanes( void )
{ return _lanes.ptr(); }

const Lanes* RoadImpl::lanes( void ) const
{ return _lanes.ptr(); }

ElevationProfile* RoadImpl::elevationProfile( void )
{ return _elevationProf.ptr(); }

const ElevationProfile* RoadImpl::elevationProfile( void ) const
{ return _elevationProf.ptr(); }

LateralProfile* RoadImpl::lateralProfile( void )
{ return _lateralProf.ptr(); }

const LateralProfile* RoadImpl::lateralProfile( void ) const
{ return _lateralProf.ptr(); }

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
