//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file HeaderImpl.h
//! \brief odr::HeaderImpl class declaration

#ifndef _ODR_HEADER_IMPL_H_
#define _ODR_HEADER_IMPL_H_

#include <odr/Header.h>
#include <misc/WeakRefServerImpl.h>
#include <string>

namespace odr
{
  //! odr::HeaderImpl provides an actual implementation of class odr::Header.
  class HeaderImpl :
    public ::misc::DllImpl<Header>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    HeaderImpl( void );

    //! Destruction.
    ODR_CALL ~HeaderImpl();

    //! @name Read-only accessors inherited from odr::Header
    //@{
    const char*  ODR_CALL name   ( void ) const;
    float        ODR_CALL version( void ) const;
    const char*  ODR_CALL date   ( void ) const;
    double       ODR_CALL north  ( void ) const;
    double       ODR_CALL south  ( void ) const;
    double       ODR_CALL east   ( void ) const;
    double       ODR_CALL west   ( void ) const;
    //@}

    //! @name Write accessors
    //@{
    inline void ODR_CALL setName   ( const std::string& s ){ _name=s; }
    inline void ODR_CALL setVersion( float f )             { _version=f; }
    inline void ODR_CALL setDate   ( const std::string& s ){ _date=s; }
    inline void ODR_CALL setNorth  ( double d )            { _north=d; }
    inline void ODR_CALL setSouth  ( double d )            { _south=d; }
    inline void ODR_CALL setEast   ( double d )            { _east=d; }
    inline void ODR_CALL setWest   ( double d )            { _west=d; }
    //@}

  private:
    std::string  _name;     //!< Database name
    float        _version;  //!< Database version (format: a.bb)
    std::string  _date;     //!< Database creation date
    double       _north;    //!< Maximum inertial y value (in [m])
    double       _south;    //!< Minimum inertial y value (in [m])
    double       _east;     //!< Maximum inertial x value (in [m])
    double       _west;     //!< Minimum inertial x value (in [m])

  }; // class HeaderImpl

} // namespace odr

#endif  // #ifndef _ODR_HEADER_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
