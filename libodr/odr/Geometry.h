//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Geometry.h
//! \brief odr::Geometry class declaration

#ifndef _ODR_GEOMETRY_H_
#define _ODR_GEOMETRY_H_

#include <misc/WeakRefServer.h>
#include <misc/Enumerator.h>
#include <odr/Factories.h>
#include <odr/GeometricPrimitive.h>

namespace odr
{
  class Road;

  //! Geometry of one road record in the xy-plane.
  //! Wraps one OpenDRIVE planview item.
  class Geometry :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Direct deletion of geometry is forbidden.
    inline void ODR_CALL operator delete( void *p )
    { *((int*)0) = 0; }

    //! Return pointer to parent road.
    virtual Road* ODR_CALL parentRoad( void ) = 0;

    //! Return pointer to parent road as read-only.
    virtual const Road* ODR_CALL parentRoad( void ) const = 0;

    //! Enumerate primitives describing road geometry.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<GeometricPrimitive*>*
      ODR_CALL enumerate( void ) = 0;

    //! Enumerate primitives describing geometry as constant.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<const GeometricPrimitive*>*
      ODR_CALL enumerate( void ) const = 0;

    //! Return geometric primitive count.
    virtual unsigned int ODR_CALL count( void ) const = 0;

    //! Return geometric primitive at transmitted index.
    virtual GeometricPrimitive* ODR_CALL item( unsigned int index ) = 0;

    //! Return geometric primitive at transmitted index as a constant.
    virtual const GeometricPrimitive* ODR_CALL
      item( unsigned int index ) const = 0;

    //! Return the geometric primitive applicable for transmitted S coordinate
    virtual GeometricPrimitive* ODR_CALL primitiveAt( double s ) = 0;

    //! Return the geometric primitive applicable for transmitted S coordinate
    //! as read-only.
    virtual const GeometricPrimitive* ODR_CALL
      primitiveAt( double s ) const = 0;

    //! Create a new line segment and insert it into geometry.
    //! \arg start    Start S-coordinate of geometric primitive
    //! \return       Return pointer to created line segment, 0 on error.
    virtual Line* ODR_CALL createLine( double start ) = 0;

    //! Create a new spiral and insert it into geometry.
    //! \arg start    Start S-coordinate of geometric primitive
    //! \return       Return pointer to created spiral, 0 on error.
    virtual Spiral* ODR_CALL createSpiral( double start ) = 0;

    //! Create a new arc and insert it into geometry.
    //! \arg start    Start S-coordinate of geometric primitive
    //! \return       Return pointer to created arc, 0 on error.
    virtual Arc* ODR_CALL createArc( double start ) = 0;

    //! Create a new polynom curve and insert it into geometry.
    //! \arg start    Start S-coordinate of geometric primitive
    //! \return       Return pointer to created polynom curve, 0 on error.
    virtual Poly3* ODR_CALL createPoly3( double start ) = 0;

  }; // class Geometry

} // namespace odr

#endif  // #ifndef _ODR_GEOMETRY_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
