//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Junction.h
//! \brief odr::Junction class declaration

#ifndef _ODR_JUNCTION_H_
#define _ODR_JUNCTION_H_

#include <odr/Element.h>
#include <misc/Enumerator.h>

namespace odr
{
  class Road;

  //! OpenDRIVE junction record.
  class Junction :
    public virtual Element
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Junction*>(p)->destroy(); }

    //! Create a new road in this juntion.
    virtual Road* ODR_CALL createRoad( void ) = 0;

    //! Return the number of roads in this junction.
    virtual unsigned int ODR_CALL count( void ) const = 0;

    //! Enumerate roads in this junction.
    //! User is responsible for deleting object pointed by returned pointer.
    virtual ::misc::Enumerator<odr::Road*>* ODR_CALL enumerate( void ) = 0;

    //! Enumerate roads in this junction as constants.
    //! User is responsible for deleting object pointed by returned pointer.
    virtual ::misc::Enumerator<const odr::Road*>* ODR_CALL
      enumerate( void ) const = 0;

  }; // class Junction

} // namespace odr

#endif  // #ifndef _ODR_JUNCTION_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
