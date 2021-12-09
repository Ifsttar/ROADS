//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Enumerator.h
//! \brief misc::Enumerator template class definition.

#ifndef _ENUMERATOR_H_
#define _ENUMERATOR_H_

#include <misc/DllHelper.h>

namespace misc
{
  //--------------------------------------------------------------------------
  //! Provides an ABI-safe interface to enumerate elements of a collection
  template<typename T> class Enumerator : public DllInterface
  {
  public:
    typedef typename T output_type;

    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void MISC_CALL operator delete( void *p )
    { if(p) static_cast<Enumerator<T>*>(p)->destroy(); }

    //! Return current element, 0 when the end of the collection was reached.
    virtual output_type MISC_CALL item( void ) = 0;
    
    //! Move to next element in collection.
    virtual void MISC_CALL moveNext( void ) = 0;
    
  }; // template<typename T> class Enumerator

} // namespace misc

#endif  // #ifndef _ENUMERATOR_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
