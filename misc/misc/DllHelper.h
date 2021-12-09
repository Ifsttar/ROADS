//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file DllHelper.h
//! \brief misc::DllImpl template class definition.

#ifndef _DLL_HELPER_H_
#define _DLL_HELPER_H_

#include <misc/misc.h>

namespace misc
{
  //--------------------------------------------------------------------------
  //! Class to be inherited by every interface class exposed in a
  //! C++ ABI. Allow object deletion to use the appropriate deallocator.
  class DllInterface
  {
  protected:
    //! Actually destroy object and deallocate memory with DLL's deallocator.
    virtual void MISC_CALL destroy( void ) = 0;
    
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void MISC_CALL operator delete( void *p )
    { if(p) static_cast<DllInterface*>(p)->destroy(); }
    
  }; // class DllInterface


  //--------------------------------------------------------------------------
  //! Helper class to be inherited by every class implementing a DLL
  //! interface.
  //! Defines destroy() method and delete operator for implementations.
  //! Template argument must be the name of the interface.
  template<class IFACE> class DllImpl : public IFACE
  {
  public:
    //! Destroy object and deallocate memory with library's deallocator.
    inline void MISC_CALL operator delete( void *p ) { ::operator delete(p); }

    //! Allow implementer to have its own destructor properly called.
    virtual MISC_CALL ~DllImpl() {};

    //! Destroy object, making sure the destructor is called in a polymorphic
    //! manner.
    virtual void MISC_CALL destroy( void ) { delete this; }
    
  }; // class DllImpl

} // namespace misc

#endif  // #ifndef _DLL_HELPER_H_
