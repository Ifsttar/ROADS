//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file GeometricPrimitive.h
//! \brief Declaration of class odr::GeometricPrimitive and its derivatives

#ifndef _ODR_GEOMETRIC_PRIMITIVE_H_
#define _ODR_GEOMETRIC_PRIMITIVE_H_

#include <odr/Factories.h>
#include <misc/WeakRefServer.h>

namespace odr
{
  class Geometry;

  //! Geometric primitive for describing roads in xy-plane.
  //! Each instance wraps one OpenDRIVE geometry entry.
  class GeometricPrimitive :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Primitive type enum.
    enum Type {
      T_LINE,
      T_SPIRAL,
      T_ARC,
      T_POLY3
    };

    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<GeometricPrimitive*>(p)->destroy(); }

    //! Return pointer to parent geometry.
    virtual Geometry* ODR_CALL parentGeometry( void ) = 0;

    //! Return pointer to parent geometry as read-only.
    virtual const Geometry* ODR_CALL parentGeometry( void ) const = 0;

    //! Return primitive type.
    virtual Type ODR_CALL type( void ) const = 0;

    //! @name Read-only accessors
    //@{
    //! Return S coordinate of start position
    virtual double ODR_CALL s( void ) const = 0;

    //! Return X coordinate of start position.
    //! The validity of this value is not enforced by libodr as it provides
    //! no facilities for evaluating actual road geometry. The user
    //! might call setX() to assign a value that is not coherent with the
    //! rest of the geometry.
    //! Geometry evaluation functionality is provided by libasphalt.
    virtual double ODR_CALL x( void ) const = 0;

    //! Return Y coordinate of start position.
    //! The validity of this value is not enforced by libodr as it provides
    //! no facilities for evaluating actual road geometry. The user
    //! might call setY() to assign a value that is not coherent with the
    //! rest of the geometry.
    //! Geometry evaluation functionality is provided by libasphalt.
    virtual double ODR_CALL y( void ) const = 0;

    //! Return start heading
    virtual double ODR_CALL heading( void ) const = 0;

    //! Return length of the element chord line
    virtual double ODR_CALL length( void ) const = 0;
    //@}

    //! @name Write accessors
    //@{
    //! Set S coordinate of start position.
    //! This does not move other primitives, nor does it resize parent road.
    //! Length of every geometric primitive in road is re-computed accordingly.
    virtual void ODR_CALL setS( double ) = 0;

    //! Set X coordinate of start position
    virtual void ODR_CALL setX( double ) = 0;

    //! Set Y coordinate of start position
    virtual void ODR_CALL setY( double ) = 0;

    //! Set start heading
    virtual void ODR_CALL setHeading( double ) = 0;

    //! Set primitive's length. Parent road's length is modified accordingly.
    //! Other primitives are moved in order to keep their current length.
    virtual void ODR_CALL setLength( double ) = 0;

    //@}

  }; // class GeometricPrimitive


  //! Straight line segment.
  class Line :
    public virtual GeometricPrimitive
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Line*>(p)->destroy(); }

    //! Return odr::GeometricPrimitive::T_LINE as the type.
    virtual GeometricPrimitive::Type ODR_CALL type( void ) const = 0;

  }; // class Line


  //! Spiral.
  class Spiral :
    public virtual GeometricPrimitive
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Spiral*>(p)->destroy(); }

    //! Return odr::GeometricPrimitive::T_SPIRAL as the type.
    virtual GeometricPrimitive::Type ODR_CALL type( void ) const = 0;

    //! @name Read-only accessors
    //@{
    //! Return curvature at the start of the element.
    virtual double ODR_CALL curvatureStart( void ) const = 0;

    //! Return curvature at the end of the element.
    virtual double ODR_CALL curvatureEnd( void ) const = 0;
    //@}

    //! @name Write accessors
    //@{
    //! Set curvature at the start of the element.
    virtual void ODR_CALL setCurvatureStart( double ) = 0;

    //! Set curvature at the end of the element.
    virtual void ODR_CALL setCurvatureEnd( double ) = 0;
    //@}

  }; // class Spiral


  //! Arc.
  class Arc :
    public virtual GeometricPrimitive
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Arc*>(p)->destroy(); }

    //! Return odr::GeometricPrimitive::T_ARC as the type.
    virtual GeometricPrimitive::Type ODR_CALL type( void ) const = 0;

    //! @name Read-only accessors
    //@{
    //! Return constant curvature throughout the element.
    virtual double ODR_CALL curvature( void ) const = 0;
    //@}

    //! @name Write accessors
    //@{
    //! Set constant curvature throughout the element.
    virtual void ODR_CALL setCurvature( double ) = 0;
    //@}

  }; // class Arc


  //! Cubic polynom curve.
  class Poly3 :
    public virtual GeometricPrimitive
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Poly3*>(p)->destroy(); }

    //! Return odr::GeometricPrimitive::T_POLY3 as the type.
    virtual GeometricPrimitive::Type ODR_CALL type( void ) const = 0;

    //! @name Read-only accessors
    //@{
    //! Return polynom constant parameter.
    virtual double ODR_CALL a( void ) const = 0;

    //! Return polynom linear parameter.
    virtual double ODR_CALL b( void ) const = 0;

    //! Return polynom quadratic parameter.
    virtual double ODR_CALL c( void ) const = 0;

    //! Return polynom cubic parameter.
    virtual double ODR_CALL d( void ) const = 0;
    //@}

    //! @name WRite accessors
    //@{
    //! Set polynom constant parameter.
    virtual void ODR_CALL setA( double ) = 0;

    //! Set polynom linear parameter.
    virtual void ODR_CALL setB( double ) = 0;

    //! Set polynom quadratic parameter.
    virtual void ODR_CALL setC( double ) = 0;

    //! Set polynom cubic parameter.
    virtual void ODR_CALL setD( double ) = 0;
    //@}

  }; // class Poly3

} // namespace odr

#endif  // #ifndef _ODR_GEOMETRIC_PRIMITIVE_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
