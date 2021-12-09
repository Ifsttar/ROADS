//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Road.h
//! \brief odr::Road class declaration

#ifndef _ODR_ROAD_H_
#define _ODR_ROAD_H_

#include <odr/Element.h>

namespace odr
{
  class Junction;
  class Geometry;
  class Lanes;
  class ElevationProfile;
  class LateralProfile;

  //! OpenDRIVE road record.
  class Road :
    public virtual Element
  {
  public:
    //! Type indicating at which end of a road a contact point is.
    enum ContactPointType {
      CP_START=0,
      CP_END
    };

    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Road*>(p)->destroy(); }

    //! Return the total length of the chord line in the xy-plane.
    virtual double ODR_CALL length( void ) const = 0;

    //! Change the total length of the chord line in the xy-plane.
    //! The length of the following elements is also changed accordingly:
    //! - last geometry primitive in road's chord line
    //! - last lane section
    //! - last elevation record
    //! - last crossfall record
    //! - last superelevation record
    virtual void ODR_CALL setLength( double ) = 0;

    //! Return a pointer to the junction to which the road belongs as a path,
    //! 0 for none.
    virtual Junction* ODR_CALL junction( void ) = 0;

    //! Return a pointer to the junction to which the road belongs as a path,
    //! 0 for none, constant version.
    virtual const Junction* ODR_CALL junction( void ) const = 0;

    //! Return a pointer to this road's predecessor, which may be a road or a
    //! junction.
    virtual Element* ODR_CALL predecessor( void ) = 0;

    //! Return a pointer to this road's predecessor, which may be a road or a
    //! junction, constant version.
    virtual const Element* ODR_CALL predecessor( void ) const = 0;

    //! Return which end of the predecessor is connected to this road.
    virtual ContactPointType ODR_CALL predecessorContactPoint( void ) const
      = 0;

    //! Set road predecessor.
    //! @param pred   Pointer to new predecessor
    //! @param end    Which end of the new predecessor the road should connect
    virtual void ODR_CALL setPredecessor(
      Element* pred, ContactPointType end=CP_END ) = 0;

    //! Return a pointer to this road's successor, which may be a road or a
    //! junction, 0 for none.
    virtual Element* ODR_CALL successor( void ) = 0;

    //! Return a pointer to this road's successor, which may be a road or a
    //! junction, 0 for none, constant version.
    virtual const Element* ODR_CALL successor( void ) const = 0;

    //! Return which end of the successor is connected to this road.
    virtual ContactPointType ODR_CALL successorContactPoint( void ) const = 0;

    //! Set road successor.
    //! @param succ   Pointer to new successor
    //! @param end    Which end of the new successor the road should connect
    virtual void ODR_CALL setSuccessor(
      Element* succ, ContactPointType end=CP_START ) = 0;

    //! Return a pointer to this road's neighbor, 0 for none.
    virtual Road* ODR_CALL neighbor( void ) = 0;

    //! Return a pointer to this road's neighbor, 0 for none,
    //! constant version.
    virtual const Road* ODR_CALL neighbor( void ) const = 0;

    //! Return true when this road's neighbor is in the same direction,
    //! when applicable.
    virtual bool ODR_CALL neighborIsSameDirection( void ) const = 0;

    //! Set this road's neighbor.
    //! @param nNeighbor      pointer to new neighbor, 0 for none
    //! @param sameDirection  true if the new neighbor is in the same direction
    virtual void ODR_CALL setNeighbor(
      Road* nNeighbor, bool sameDirection=true ) = 0;

    //! Return road geometry.
    virtual Geometry* ODR_CALL geometry( void ) = 0;

    //! Return road geometry, constant version.
    virtual const Geometry* ODR_CALL geometry( void ) const = 0;

    //! Return road elevation profile.
    virtual ElevationProfile* ODR_CALL elevationProfile( void ) = 0;

    //! Return road elevation profile as a constant.
    virtual const ElevationProfile* ODR_CALL elevationProfile(void) const = 0;

    //! Return road lateral profile.
    virtual LateralProfile* ODR_CALL lateralProfile( void ) = 0;

    //! Return road lateral profile as a constant.
    virtual const LateralProfile* ODR_CALL lateralProfile( void ) const = 0;

    //! Return road lanes.
    virtual Lanes* ODR_CALL lanes( void ) = 0;

    //! Return road lanes, read-only.
    virtual const Lanes* ODR_CALL lanes( void ) const = 0;

  }; // class Road

} // namespace odr

#endif  // #ifndef _ODR_ROAD_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
