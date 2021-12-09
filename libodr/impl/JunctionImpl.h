//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file JunctionImpl.h
//! \brief odr::JunctionImpl class declaration

#ifndef _ODR_JUNCTION_IMPL_H_
#define _ODR_JUNCTION_IMPL_H_

#include <odr/Junction.h>
#include "ElementImpl.h"
#include <odr/Road.h>
#include <misc/WeakRefContainers.h>

namespace odr
{
  class DocumentImpl;

  class JunctionImpl :
    public ::misc::DllImpl<Junction>,
    public ElementImpl
  {
  public:
    //! Construction.
    JunctionImpl( DocumentImpl *document, unsigned int id );

    //! Destruction.
    ODR_CALL ~JunctionImpl();

    //! @name Methods inherited from odr::Junction.
    //@{
    Element::Type ODR_CALL type( void ) const;
    Road* ODR_CALL createRoad( void );
    unsigned int ODR_CALL count( void ) const;
    ::misc::Enumerator<odr::Road*>* ODR_CALL enumerate( void );
    ::misc::Enumerator<const odr::Road*>* ODR_CALL enumerate( void ) const;
    //@}

  private:
    ::misc::WeakRefList<Road> _roads;
    
  }; // class JunctionImpl

} // namespace odr

#endif  // #ifndef _ODR_JUNCTION_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
