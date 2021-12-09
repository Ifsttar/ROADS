//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ElevationProfile.h
//! \brief odr::ElevationProfile class declaration

#ifndef _ODR_ELEVATION_PROFILE_H_
#define _ODR_ELEVATION_PROFILE_H_

#include <misc/WeakRefServer.h>
#include <misc/Enumerator.h>
#include <odr/Factories.h>

namespace odr
{
  class Elevation;
  class Road;

  //! Elevation profile of one road record.
  //! Wraps one OpenDRIVE elevationProfile item.
  class ElevationProfile :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Direct deletion of elevation profile is forbidden.
    inline void ODR_CALL operator delete( void *p )
    { *((int*)0) = 0; }

    //! Enumerate records describing road elevation profile.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<Elevation*>*
      ODR_CALL enumerate( void ) = 0;

    //! Return a pointer to the road owning this elevation profile.
    virtual Road* ODR_CALL parentRoad( void ) = 0;

    //! Return a pointer to the road owning this elevation profile, read-only.
    virtual const Road* ODR_CALL parentRoad( void ) const = 0;

    //! Enumerate records describing road elevation profile as constant.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<const Elevation*>*
      ODR_CALL enumerate( void ) const = 0;

    //! Return elevation records count.
    virtual unsigned int ODR_CALL count( void ) const = 0;

    //! Return elevation record at transmitted index.
    virtual Elevation* ODR_CALL item( unsigned int index ) = 0;

    //! Return elevation record at transmitted index as a constant.
    virtual const Elevation* ODR_CALL item( unsigned int index ) const = 0;

    //! Return applicable elevation record for transmitted S-coordinate.
    virtual Elevation* ODR_CALL elevationAt( double s ) = 0;

    //! Return applicable elevation record for transmitted S-coordinate,
    //! read-only.
    virtual const Elevation* ODR_CALL elevationAt( double s ) const = 0;

    //! Create a new elevation and insert it into geometry.
    //! \arg start    start position of new entry
    //! \return       Return pointer to created elevation record, 0 on error.
    virtual Elevation* ODR_CALL createRecord( double start ) = 0;

  }; // class ElevationProfile

} // namespace odr

#endif  // #ifndef _ODR_ELEVATION_PROFILE_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
