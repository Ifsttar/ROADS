//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LateralProfile.h
//! \brief odr::LateralProfile class declaration

#ifndef _ODR_LATERAL_PROFILE_H_
#define _ODR_LATERAL_PROFILE_H_

#include <misc/WeakRefServer.h>
#include <misc/Enumerator.h>
#include <odr/Factories.h>

namespace odr
{
  class Superelevation;
  class Crossfall;
  class Road;

  //! Lateral profile of one road record.
  //! Wraps one OpenDRIVE LateralProfile item.
  class LateralProfile :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Direct deletion of lateral profile is forbidden.
    inline void ODR_CALL operator delete( void *p )
    { *((int*)0) = 0; }

    //! Return a pointer to the road owning this lateral profile.
    virtual Road* ODR_CALL parentRoad( void ) = 0;

    //! Return a pointer to the road owning this elevation lateral, read-only.
    virtual const Road* ODR_CALL parentRoad( void ) const = 0;

    //------------------------------------------------------------------------
    //! @name Superelevation management
    //@{

    //! Enumerate superelevation records.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<Superelevation*>*
      ODR_CALL enumerateSuperelevations( void ) = 0;

    //! Enumerate records describing road elevation profile as constant.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<const Superelevation*>*
      ODR_CALL enumerateSuperelevations( void ) const = 0;

    //! Return superelevation records count.
    virtual unsigned int ODR_CALL superelevationCount( void ) const = 0;

    //! Return superelevation record at transmitted index.
    virtual Superelevation* ODR_CALL
      superelevationItem( unsigned int index ) = 0;

    //! Return superelevation record at transmitted index as a constant.
    virtual const Superelevation* ODR_CALL 
      superelevationItem( unsigned int index ) const = 0;

    //! Return superelevation record applicable for transmitted S-coordinate.
    virtual Superelevation* ODR_CALL
      superelevationAt( double s ) = 0;

    //! Return superelevation record applicable for transmitted S-coordinate,
    //! read-only.
    virtual const Superelevation* ODR_CALL
      superelevationAt( double s ) const = 0;

    //! Create a new line segment and insert it into geometry.
    //! \arg start    start position of new entry
    //! \return       Return pointer to created elevation record, 0 on error.
    virtual Superelevation* ODR_CALL createSuperelevation( double start ) = 0;
    //@}

    //------------------------------------------------------------------------
    //! @name Crossfall management
    //@{

    //! Enumerate crossfall records.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<Crossfall*>*
      ODR_CALL enumerateCrossfalls( void ) = 0;

    //! Enumerate records describing road elevation profile as constant.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<const Crossfall*>*
      ODR_CALL enumerateCrossfalls( void ) const = 0;

    //! Return crossfall records count.
    virtual unsigned int ODR_CALL crossfallCount( void ) const = 0;

    //! Return crossfall record at transmitted index.
    virtual Crossfall* ODR_CALL
      crossfallItem( unsigned int index ) = 0;

    //! Return crossfall record at transmitted index as a constant.
    virtual const Crossfall* ODR_CALL 
      crossfallItem( unsigned int index ) const = 0;

    //! Return crossfall record appolicable at tramitted S-coordinate.
    virtual Crossfall* ODR_CALL crossfallAt( double s ) = 0;

    //! Return crossfall record appolicable at tramitted S-coordinate,
    //! read-only.
    virtual const Crossfall* ODR_CALL crossfallAt( double s ) const = 0;

    //! Create a new line segment and insert it into geometry.
    //! \arg start    start position of new entry
    //! \return       Return pointer to created elevation record, 0 on error.
    virtual Crossfall* ODR_CALL createCrossfall( double start ) = 0;
    //@}

  }; // class LateralProfile

} // namespace odr

#endif  // #ifndef _ODR_LATERAL_PROFILE_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
