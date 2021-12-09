//----------------------------------------------------------------------------
// libasphalt (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file evaluation.h
//! \brief Function headers for evaluating a position on a road.

#include <odr/odr.h>
#include <asphalt/vecop.h>

namespace asphalt {

  //--------------------------------------------------------------------------
  //! Evaluate planar position along a geometric primitive.
  //! @param[in]  primitive   Primitive along which position is evaluated
  //! @param[in]  sOffset     S-coord offset from primitive's start
  //! @param[out] x           Pointer to location where X coord is saved
  //! @param[out] y           Pointer to location where Y coord is saved
  //! @param[out] heading     Pointer to location where heading is saved
  void evaluatePrimitive(
    const odr::GeometricPrimitive* primitive,
    double sOffset,
    double *x=0,
    double *y=0,
    double *heading=0 );


  //--------------------------------------------------------------------------
  //! Evaluate the position of a point on a lane.
  //! @param[in]  lane      Pointer to lane
  //! @param[in]  sOffset   S-coordinate offset from lane section start
  //! @param[in]  tOffset   T-coordinate offset from lane's outer border
  //! @param[out] position  Receive coordinates of point on road
  //! @param[out] normal    Receive normal to road
  //! @param[out] dopsds    Receive tangent to road along s parameter
  //! @param[out] dposdt    Receive tangent to road along t parameter
  void evaluateLane(
    const odr::Lane* lane,
    double sOffset, double tOffset,
    double *position=0,
    double *normal=0,
    double *dposds=0,
    double *dposdt=0 );

  //--------------------------------------------------------------------------
  //! Evalutate the position of a point on the road.
  //! @param[in]  road      Pointer to road
  //! @param[in]  s         S-coordinate along road's chord line
  //! @param[in]  lane      Lane index according to convention (clamped)
  //! @param[in]  tOffset   T-coordinate offset from lane's outer border
  //! @param[out] position  Receive coordinates of point on road
  //! @param[out] normal    Receive normal to road
  //! @param[out] dopsds    Receive tangent to road along s parameter
  //! @param[out] dopsdt    Receive tangent to road along t parameter
  void evaluatePosition( const odr::Road* road,
                         double s, int lane, double tOffset,
                         double *position=0,
                         double *normal=0,
                         double *dposds=0,
                         double *dposdt=0 );


  //--------------------------------------------------------------------------
  //! Adjust parameter s to satisfy a constraint and evaluate a point on road.
  //! @param[in]  lane        Pointer to lane
  //! @param[in]  tOffset     T-coordinate offset from lane's outer border
  //! @param[in]  constraint  Functor evaluating constraint for position
  //! @param[in]  target      Target value of the constraint function
  //! @param[in]  sMin        Minimum S-coordinate
  //! @param[in]  sGuess      Guessed S-coordinate along road's chord line
  //! @param[out] sSolved     Solved value of parameter s
  //! @param[out] position    Receive coordinates of point on road
  //! @param[out] normal      Receive normal to road
  //! @param[out] tangent_s   Receive tangent to road along s parameter
  //! @param[out] tangent_t   Receive tangent to road along t parameter
  //! @param[in]  tolerance   Tolerance between target and function value
  //! @param[in]  maxIter     Maximum nb of iterations
  template<typename C>
  void evaluateConstrainedPosition( const odr::Lane* lane,
                                    double tOffset,
                                    C constraint,
                                    double target,
                                    double sZero,
                                    double sGuess,
                                    double &sSolved,
                                    double *position=0,
                                    double *normal=0,
                                    double *dposds=0,
                                    double *dposdt=0,
                                    double tolerance = 0.001,
                                    unsigned int maxIter = 10 );


  //--------------------------------------------------------------------------
  //! Edge length constraint to be used with evaluateConstrainedPosition().
  struct EdgeLengthConstraint {
    double reference[3];
    inline double operator()( const double *position,
                              const double *normal,
                              const double *dposds,
                              const double *dposdt ) const
    {
      return ( mk_vecop<3>(position) - mk_vecop(reference) ).norm2();
    }
  };


  //--------------------------------------------------------------------------
  //! X-Y angle constraint to be used with evaluateConstrainedPosition().
  struct XYAngleConstraint {
    double referencePoint[2];
    double referenceDirection[2];
    inline double operator()( const double *position,
                              const double *normal,
                              const double *dposds,
                              const double *dposdt ) const
    {
      double dx = position[0]-referencePoint[0],
             dy = position[1]-referencePoint[1];
      double norm = sqrt(
        ( dx*dx + dy*dy ) *
        ( sqr(referenceDirection[0]) + sqr(referenceDirection[1]) )  );
      double a1 = acos(  ( dx*referenceDirection[0]+dy*referenceDirection[1] )
                         / norm  );
      double normb = sqrt(
        ( sqr(dposds[0]) + sqr(dposds[1]) ) *
        ( sqr(referenceDirection[0]) + sqr(referenceDirection[1]) )  );
      double a2 = acos(  ( dposds[0]*referenceDirection[0]
                            + dposds[1]*referenceDirection[1] )
                         / normb  );
      return a1 + a2;
    }
  };


  //--------------------------------------------------------------------------
  //! Face angle constraint to be used with evaluateConstrainedPosition().
  struct NormalAngleConstraint {
    double reference[3];
    inline double operator()( const double *position,
                              const double *normal,
                              const double *dposds,
                              const double *dposdt ) const
    {
      return acos( mk_vecop<3>(normal) * mk_vecop(reference) );
    }
  };
}


//----------------------------------------------------------------------------
template<typename C>
void asphalt::evaluateConstrainedPosition(
  const odr::Lane* lane, double tOffset,
  C constraint, double target, double sMin, double sGuess, double &sSolved,
  double *position, double *normal, double *dposds, double *dposdt,
  double tolerance, unsigned int maxIter )
{
  for( double error=std::numeric_limits<double>::max() ;
       error>tolerance && maxIter>0 ; --maxIter )
  {
    // Evaluate constraint function for guessed value...
    sSolved = sGuess;
    evaluateLane( lane, sSolved, tOffset, position, normal, dposds, dposdt );

    // Make a new guess to minimize error...
    double c = constraint( position, normal, dposds, dposdt );
    error = fabs( c - target );
    if( c < target ) {
      sGuess += sGuess - sMin;
      sMin = sSolved;
    }
    else
      sGuess = 0.5 * ( sMin + sGuess );
  }
}

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
