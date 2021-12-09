//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file SuperelevationImpl.h
//! \brief odr::SuperelevationImpl class declaration

#ifndef _ODR_SUPERELEVATION_IMPL_H_
#define _ODR_SUPERELEVATION_IMPL_H_

#include <odr/Superelevation.h>
#include <misc/DllHelper.h>
#include <misc/WeakRefServerImpl.h>

namespace odr
{
  class LateralProfileImpl;

  //! Actual Superelevation implementation.
  class SuperelevationImpl:
    public ::misc::DllImpl<Superelevation>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    SuperelevationImpl( LateralProfileImpl *parent, double nStart );

    //! Destruction.
    ~SuperelevationImpl();

    //! @name Methods inherited from Superelevation
    //@{
    LateralProfile* ODR_CALL parentProfile( void );
    const LateralProfile* ODR_CALL parentProfile( void ) const;
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

    //! Update length. Called by LateralProfileImpl::updateLengths().
    inline void setCachedLength( double d ) const { _length=d; }

  private:
    LateralProfileImpl *_parent;  //!< Pointer to parent
    double             _start,    //!< Start S-coordinate of record
                       _a,        //!< Superelevation curve parameter A (m)
                       _b,        //!< Superelevation curve parameter B
                       _c,        //!< Superelevation curve parameter C
                       _d;        //!< Superelevation curve parameter D
    mutable double     _length;   //!< S-length for which rec is applicable

  }; // class SuperelevationImpl

} // namespace odr

#endif  // #ifndef _ODR_SUPERELEVATION_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
