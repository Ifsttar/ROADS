//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Header.h
//! \brief odr::Header class definition

#ifndef _ODR_HEADER_H_
#define _ODR_HEADER_H_

#include <odr/Factories.h>
#include <misc/WeakRefServer.h>

namespace odr
{
  //! OpenDRIVE database header.
  class Header :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Direct deletion of header is forbidden.
    inline void ODR_CALL operator delete( void *p )
    { { *((int*)0) = 0; } }

    //! Return darabase name.
    virtual const char* ODR_CALL name( void ) const = 0;

    //! Return number version of this database (format: a.bb).
    virtual float ODR_CALL version( void ) const = 0;

    //! Return date/time of database conversion.
    virtual const char* ODR_CALL date( void ) const = 0;

    //! Return maximum inertial y value (in [m]).
    virtual double ODR_CALL north( void ) const = 0;

    //! Return minimum inertial y value (in [m]).
    virtual double ODR_CALL south( void ) const = 0;

    //! Return maximum inertial x value (in [m]).
    virtual double ODR_CALL east( void ) const = 0;

    //! Return minimum inertial x value (in [m]).
    virtual double ODR_CALL west( void ) const = 0;

  }; // class Header

} // namespace odr

#endif  // #ifndef _ODR_HEADER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
