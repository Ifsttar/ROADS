//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file GeometricPrimitiveImpl.h
//! \brief Declaration of implementations od class odr::GeometricPrimitive 
//!        and its derivatives

#ifndef _ODR_GEOMETRIC_PRIMITIVE_IMPL_H_
#define _ODR_GEOMETRIC_PRIMITIVE_IMPL_H_

#include <odr/GeometricPrimitive.h>
#include <misc/WeakRefServerImpl.h>

namespace odr
{
  class GeometryImpl;

  class GeometricPrimitiveImpl :
    public virtual GeometricPrimitive,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction: initialize members to 0.
    GeometricPrimitiveImpl( GeometryImpl *parent, double start );

    //! Destruction.
    virtual ODR_CALL ~GeometricPrimitiveImpl();

    //! @name Methods inherited from interface odr::GeometricPrimitive
    //@{
    Geometry* ODR_CALL parentGeometry( void );
    const Geometry* ODR_CALL parentGeometry( void ) const;
    double ODR_CALL s( void ) const;
    double ODR_CALL x( void ) const;
    double ODR_CALL y( void ) const;
    double ODR_CALL heading( void ) const;
    double ODR_CALL length( void ) const;
    void ODR_CALL setS( double );
    void ODR_CALL setX( double );
    void ODR_CALL setY( double );
    void ODR_CALL setHeading( double );
    void ODR_CALL setLength( double );
    //@}

    //! Update cached length. Called by GeometryImpl::updateLengths().
    inline void setCachedLength( double d ) const { _len=d; }

    //! Propagate to document the fact that geometry was modified.
    void setModified( void );

  private:
    GeometryImpl   *_parent;
    double         _s, _x, _y, _hdg;
    mutable double _len;

  }; // class GeometricPrimitiveImpl


  class LineImpl :
    public ::misc::DllImpl<Line>,
    public GeometricPrimitiveImpl
  {
  public:
    //! Construction: initialize members to 0.
    LineImpl( GeometryImpl *parent, double start );

    //! @name Methods inherited from interface odr::Line
    //@{
    GeometricPrimitive::Type ODR_CALL type( void ) const;
    //@}

  }; // class LineImpl


  class SpiralImpl :
    public ::misc::DllImpl<Spiral>,
    public GeometricPrimitiveImpl
  {
  public:
    //! Construction: initialize members to 0.
    SpiralImpl( GeometryImpl *parent, double start );

    //! @name Methods inherited from interface odr::Spiral
    //@{
    GeometricPrimitive::Type ODR_CALL type( void ) const;
    double ODR_CALL curvatureStart( void ) const;
    double ODR_CALL curvatureEnd( void ) const;
    void ODR_CALL setCurvatureStart( double );
    void ODR_CALL setCurvatureEnd( double );
    //@}

  private:
    double _curvatureStart, _curvatureEnd;

  }; // class SpiralImpl


  class ArcImpl :
    public ::misc::DllImpl<Arc>,
    public GeometricPrimitiveImpl
  {
  public:
    //! Construction: initialize members to 0.
    ArcImpl( GeometryImpl *parent, double start );

    //! @name Methods inherited from interface odr::Arc
    //@{
    GeometricPrimitive::Type ODR_CALL type( void ) const;
    double ODR_CALL curvature( void ) const;
    void ODR_CALL setCurvature( double );
    //@}

  private:
    double _curvature;

  }; // class ArcImpl


  class Poly3Impl :
    public ::misc::DllImpl<Poly3>,
    public GeometricPrimitiveImpl
  {
  public:
    //! Construction: initialize members to 0.
    Poly3Impl( GeometryImpl *parent, double start );

    //! @name Methods inherited from interface odr::Poly3
    //@{
    GeometricPrimitive::Type ODR_CALL type( void ) const;
    double ODR_CALL a( void ) const;
    double ODR_CALL b( void ) const;
    double ODR_CALL c( void ) const;
    double ODR_CALL d( void ) const;
    void ODR_CALL setA( double );
    void ODR_CALL setB( double );
    void ODR_CALL setC( double );
    void ODR_CALL setD( double );
    //@}

  private:
    double _a, _b, _c, _d;

  }; // class Poly3Impl

} // namespace odr

#endif  // #ifndef _ODR_GEOMETRIC_PRIMITIVE_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
