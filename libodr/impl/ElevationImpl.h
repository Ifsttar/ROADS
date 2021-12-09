//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ElevationImpl.h
//! \brief odr::ElevationImpl class declaration

#ifndef _ODR_ELEVATION_IMPL_H_
#define _ODR_ELEVATION_IMPL_H_

#include <odr/Elevation.h>
#include <misc/DllHelper.h>
#include <misc/WeakRefServerImpl.h>

namespace odr
{
  class ElevationProfileImpl;

  //! Actual Elevation implementation.
  class ElevationImpl:
    public ::misc::DllImpl<Elevation>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    ElevationImpl( ElevationProfileImpl *parent, double nStart );

    //! Destruction.
    ~ElevationImpl();

    //! @name Methods inherited from Elevation
    //@{
    ElevationProfile* ODR_CALL parentProfile( void );
    const ElevationProfile* ODR_CALL parentProfile( void ) const;
    double ODR_CALL start( void ) const;
    double ODR_CALL length( void ) const;
    double ODR_CALL a( void ) const;
    double ODR_CALL b( void ) const;
    double ODR_CALL c( void ) const;
    double ODR_CALL d( void ) const;
    bool ODR_CALL setStart( double nStart );
    void ODR_CALL setLength( double );
    void ODR_CALL setA( double v );
    void ODR_CALL setB( double v );
    void ODR_CALL setC( double v );
    void ODR_CALL setD( double v );
    //@}

    //! Propagate to document the fact that the record was modified.
    void setModified( void );

    //! Update length for which record is applicable along S-coord.
    //! Called by ElevationProfileImpl::updateLengths().
    inline void setCachedLength( double d ) const { _length=d; }

  private:
    ElevationProfileImpl *_parent;  //!< Pointer to parent
    double               _start,    //!< Start S-coordinate of record
                         _a,        //!< Elevation curve parameter A (m)
                         _b,        //!< Elevation curve parameter B
                         _c,        //!< Elevation curve parameter C
                         _d;        //!< Elevation curve parameter D
    mutable double       _length;   //!< S-length for which rec. is applicable

  }; // class ElevationImpl

} // namespace odr

#endif  // #ifndef _ODR_ELEVATION_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
