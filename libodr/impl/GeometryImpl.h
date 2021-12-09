//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file GeometryImpl.h
//! \brief odr::GeometryImpl class declaration

#ifndef _ODR_GEOMETRY_IMPL_H_
#define _ODR_GEOMETRY_IMPL_H_

#include <odr/Geometry.h>
#include <misc/WeakRefServerImpl.h>
#include <misc/WeakRefContainers.h>
#include "GeometricPrimitiveImpl.h"

namespace odr
{
  class RoadImpl;

  //! Actual implementation of interface odr::Geometry
  class GeometryImpl :
    public ::misc::DllImpl<Geometry>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    GeometryImpl( RoadImpl *parent );

    //! Destruction: delete primitives making road geometry.
    ODR_CALL ~GeometryImpl();

    //! @name Methods inherited from odr::Geometry
    //@{
    Road* ODR_CALL parentRoad( void );
    const Road* ODR_CALL parentRoad( void ) const;
    ::misc::Enumerator<GeometricPrimitive*>* ODR_CALL enumerate( void );
    ::misc::Enumerator<const GeometricPrimitive*>*
      ODR_CALL enumerate( void ) const;
    unsigned int ODR_CALL count( void ) const;
    GeometricPrimitive* ODR_CALL item( unsigned int index );
    const GeometricPrimitive* ODR_CALL item( unsigned int index ) const;
    GeometricPrimitive* ODR_CALL primitiveAt( double s );
    const GeometricPrimitive* ODR_CALL primitiveAt( double s ) const;
    Line* ODR_CALL createLine( double start );
    Spiral* ODR_CALL createSpiral( double start );
    Arc* ODR_CALL createArc( double start );
    Poly3* ODR_CALL createPoly3( double start );
    //@}

    //! Type of the collection of geometric primitives.
    typedef
      ::misc::Map2WeakRef<double,GeometricPrimitiveImpl,std::greater<double> >
      primitivesType;

    //! Return the collection of geometric primitives. Accessed by primitives.
    inline primitivesType& primitives( void ) { return _primitives; };

    //! Propagate to document the fact that geometry was modified.
    void setModified( void );

    //! Flag geometric primitives for length computation.
    inline void setLengthsNeedUpdate( void ) { _updateLengths=true; }

    //! Update primitives length if necessary.
    void updateLengths( void ) const;

  private:
    RoadImpl *_parent;
    primitivesType _primitives;
    mutable bool _updateLengths;

  }; // class GeometryImpl

} // namespace odr

#endif  // #ifndef _ODR_GEOMETRY_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
