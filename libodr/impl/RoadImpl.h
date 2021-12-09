//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file RoadImpl.h
//! \brief odr::RoadImpl class declaration

#ifndef _ODR_ROAD_IMPL_H_
#define _ODR_ROAD_IMPL_H_

#include <odr/Road.h>
#include <odr/Junction.h>
#include "ElementImpl.h"
#include <misc/WeakRef.h>
#include <odr/Geometry.h>
#include <odr/Lanes.h>
#include <odr/ElevationProfile.h>
#include <odr/LateralProfile.h>

namespace odr
{
  class DocumentImpl;

  //! Provides an actual implementation for abstract class odr::Road.
  class RoadImpl :
    public ::misc::DllImpl<Road>,
    public ElementImpl
  {
  public:
    //! Construction.
    RoadImpl( DocumentImpl *document, unsigned int id, Junction *j=0 );

    //! Destruction.
    ODR_CALL ~RoadImpl();

    //! @name Methods inherited from odr::Road.
    //@{
    Element::Type ODR_CALL type( void ) const;
    double ODR_CALL length( void ) const;
    void ODR_CALL setLength( double d );
    Junction* ODR_CALL junction( void );
    const Junction* ODR_CALL junction( void ) const;
    Element* ODR_CALL predecessor( void );
    const Element* ODR_CALL predecessor( void ) const;
    Road::ContactPointType ODR_CALL predecessorContactPoint( void ) const;
    void ODR_CALL setPredecessor( Element* pred, ContactPointType end );
    Element* ODR_CALL successor( void );
    const Element* ODR_CALL successor( void ) const;
    Road::ContactPointType ODR_CALL successorContactPoint( void ) const;
    void ODR_CALL setSuccessor( Element* succ, ContactPointType end );
    Road* ODR_CALL neighbor( void );
    const Road* ODR_CALL neighbor( void ) const;
    bool ODR_CALL neighborIsSameDirection( void ) const;
    void ODR_CALL setNeighbor( Road* nNeighbor, bool sameDirection );
    Geometry* ODR_CALL geometry( void );
    const Geometry* ODR_CALL geometry( void ) const;
    ElevationProfile* ODR_CALL elevationProfile( void );
    const ElevationProfile* ODR_CALL elevationProfile( void ) const;
    LateralProfile* ODR_CALL lateralProfile( void );
    const LateralProfile* ODR_CALL lateralProfile( void ) const;
    Lanes* ODR_CALL lanes( void );
    const Lanes* ODR_CALL lanes( void ) const;
    //@}

  private:
#define wr_ ::misc::WeakRef
#define ct_ Road::ContactPointType

    double                _length;         //!< Road length
    wr_<Element>          _predecessor;    //!< Road predecessor
    ct_                   _predecessorEnd; //!< Contact end of predecessor
    wr_<Element>          _successor;      //!< Road successor
    ct_                   _successorEnd;   //!< Contact end ot successor
    wr_<Junction>         _junction;       //!< Junction containing road
    wr_<RoadImpl>         _neighbor;       //!< Neighbor road
    bool                  _ngbrSameDir;    //!< Neighbor is in same direction
    wr_<Geometry>         _geometry;       //!< Road geometry
    wr_<Lanes>            _lanes;          //!< Road lanes
    wr_<ElevationProfile> _elevationProf;  //!< Road elevation profile
    wr_<LateralProfile>   _lateralProf;    //!< Road lateral profile

#undef wr_
#undef ct_

  }; // class RoadImpl

} // namespace odr

#endif  // #ifndef _ODR_ROAD_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
