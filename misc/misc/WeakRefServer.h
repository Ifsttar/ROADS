//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file WeakRefServer.h
//! \brief WeakRefServer class declaration.

#ifndef _WEAK_REF_SERVER_H_
#define _WEAK_REF_SERVER_H_

#include <misc/DllHelper.h>
#include <misc/WeakRef.h>

namespace misc {

  //--------------------------------------------------------------------------
  //| Class WeakRefServer
  //!  Class WeakRefServer allows weak references to its inheriting types.
  //!  References have the type WeakRef<T> where T is inherits WeakRefServer.
  class WeakRefServer : public DllInterface
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void MISC_CALL operator delete( void *p )
    { if(p) static_cast<WeakRefServer*>(p)->destroy(); }

    //! Add transmitted object to the set of references to this object.
    virtual void MISC_CALL addRef( IWeakRef *r ) const = 0;

    //! Remove transmitted object from the set of references to this object.
    virtual void MISC_CALL removeRef( IWeakRef *r ) const = 0;

  }; // class WeakRefServer

} // namespace misc

#endif // #ifndef _WEAK_REF_SERVER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
