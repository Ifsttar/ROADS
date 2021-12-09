//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file WeakRefServerImpl.h
//! \brief misc::WeakRefServerImpl class declaration.

#ifndef _WEAK_REF_SERVER_IMPL_H_
#define _WEAK_REF_SERVER_IMPL_H_

#include <misc/WeakRefServer.h>

namespace misc
{
  //--------------------------------------------------------------------------
  //| Class WeakRefServerImpl
  //!  Class WeakRefServerImpl provides an actual implementation of
  //!  WeakRefServer.
  //!  WARNING: WeakRefServerImpl should not be exposed in a C++ ABI as it is
  //!           not a pure abstract class. Any child class must be compiled
  //!           with the same C++ compiler as WeakRefServerImpl.
  class WeakRefServerImpl :
    public virtual WeakRefServer
  {
  public:
    //! Add transmitted object to the set of references to this object.
    MISC_DLL void MISC_CALL addRef( IWeakRef *r ) const;

    //! Remove transmitted object from the set of references to this object.
    MISC_DLL void MISC_CALL removeRef( IWeakRef *r ) const;

    //! Destruction: resets avery references to this object.
    MISC_DLL virtual MISC_CALL ~WeakRefServerImpl();

  protected:
    //! Construction: there are initially no references to this object.
    MISC_DLL MISC_CALL WeakRefServerImpl( void );

  private:
    mutable void *_impl; //!< Pointer to implementation.

  }; // class WeakRefServerImpl

} // namespace misc

#endif // #ifndef _WEAK_REF_SERVER_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
