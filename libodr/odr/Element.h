//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Element.h
//! \brief odr::Element class declaration

#ifndef _ODR_ELEMENT_H_
#define _ODR_ELEMENT_H_

#include <odr/Factories.h>
#include <misc/WeakRefServer.h>

namespace odr
{
  class Document;

  //! odr::Element is a base class for every element in an OpenDRIVE database.
  class Element :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Element*>(p)->destroy(); }

    //! Return document owning this node.
    virtual Document* ODR_CALL document( void ) = 0;

    //! Return document owning this node, constant version.
    virtual const Document* ODR_CALL document( void ) const = 0;

    //! Return element name.
    virtual const char* ODR_CALL name( void ) const = 0;

    //! Return unique element ID in database.
    virtual unsigned int ODR_CALL ID( void ) const = 0;

    //! Element type enum.
    enum Type {
      T_ROAD,
      T_JUNCTION
    };

    //! Return element type.
    virtual Type ODR_CALL type( void ) const = 0;

    //! Set element name.
    virtual void ODR_CALL setName( const char* ) = 0;

  }; // class Element

} // namespace odr

#endif  // #ifndef _ODR_ELEMENT_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
