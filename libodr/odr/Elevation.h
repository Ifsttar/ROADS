//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Elevation.h
//! \brief odr::Elevation class declaration

#ifndef _ODR_ELEVATION_H_
#define _ODR_ELEVATION_H_

#include <odr/Factories.h>
#include <misc/WeakRefServer.h>

namespace odr
{
  class ElevationProfile;

  //! Store elevation information for a section of a road.
  //! Wraps one OpenDRIVE elevation record.
  class Elevation :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Elevation*>(p)->destroy(); }

    //! Return a pointer to parent elevation profile.
    virtual ElevationProfile* ODR_CALL parentProfile( void ) = 0;

    //! Return a pointer to parent elevation profile, read-only.
    virtual const ElevationProfile* ODR_CALL parentProfile( void ) const = 0;

    //! @name Read-only accessors
    //@{

    //! Return start position (s-coord) of this record relative to road (m).
    virtual double ODR_CALL start( void ) const = 0;

    //! Return the length along the S-coordinate for which the elevation
    //! record is applicable.
    virtual double ODR_CALL length( void ) const = 0;

    //! Return elevation curve parameter A (in m).
    virtual double ODR_CALL a( void ) const = 0;

    //! Return elevation curve parameter B.
    virtual double ODR_CALL b( void ) const = 0;

    //! Return elevation curve parameter C.
    virtual double ODR_CALL c( void ) const = 0;

    //! Return elevation curve parameter D.
    virtual double ODR_CALL d( void ) const = 0;

    //@}

    //! @name Write accessors
    //@{

    //! Set start position (s-coord) of this record relative to road.
    //! This does not move other records, nor does it resize parent road.
    //! Length of every elevation record in road is re-computed accordingly.
    //! \arg nStart   New start coordinate of elevation record, must be valid.
    //! \return       Return true on success.
    virtual bool ODR_CALL setStart( double nStart ) = 0;

    //! Set record's length. Parent road's length is modified accordingly.
    //! Other records are moved in order to keep their current length.
    virtual void ODR_CALL setLength( double ) = 0;

    //! Set elevation curve parameter A (in m).
    virtual void ODR_CALL setA( double v ) = 0;

    //! Set elevation curve parameter B.
    virtual void ODR_CALL setB( double v ) = 0;

    //! Set elevation curve parameter C.
    virtual void ODR_CALL setC( double v ) = 0;

    //! Set elevation curve parameter D.
    virtual void ODR_CALL setD( double v ) = 0;

    //@}

  }; // class Elevation

} // namespace odr

#endif  // #ifndef _ODR_ELEVATION_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
