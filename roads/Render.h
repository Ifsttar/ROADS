//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Render.h
//! \brief Helper functions for rendering road elements in OpenGL

#ifndef _ROADS_RENDER_H_
#define _ROADS_RENDER_H_

namespace odr {
  class Road;
  class Lane;
}

namespace asphalt {
  class Evaluator;
}

namespace roads {
  //! Static class providing helper functions for rendering road elements.
  private class Render {
  public:
    //! Return pointer to position evaluator.
    static asphalt::Evaluator* getEvaluator( void );

    //! Render a portion of a road as a series of quad strips.
    static void FilledRoad(
      const odr::Road *road,
      double start=0.0,
      double end=std::numeric_limits<double>::infinity() );

    //! Render the outline of a portion of a road as a line loop.
    static void RoadOutline(
      const odr::Road *road,
      double start=0.0,
      double end=std::numeric_limits<double>::infinity() );

    //! Render a portion of a road with a black outline.
    static void FilledOutlinedRoad(
      const odr::Road *road,
      double start=0.0,
      double end=std::numeric_limits<double>::infinity() );

    //! Render a portion of a lane as a series of quad strips.
    static void FilledLane(
      const odr::Lane *lane,
      double startOffset=0.,
      double endOffset=std::numeric_limits<double>::infinity() );

    //! Render the outline of a portion of a lane as a line loop.
    static void LaneOutline(
      const odr::Lane *lane,
      double startOffset=0.,
      double endOffset=std::numeric_limits<double>::infinity() );

    //! Render a portion of a lane with a black outline.
    static void FilledOutlinedLane(
      const odr::Lane *lane,
      double startOffset=0.,
      double endOffset=std::numeric_limits<double>::infinity() );

    //! Render a geometric primitive as an arrow and a line strip.
    static void GeometricPrimitive(
      const odr::GeometricPrimitive *primitive,
      double startOffset=0.,
      double endOffset=std::numeric_limits<double>::infinity() );

    //! Render transmitted lane's outer border as a line strip.
    static void LaneOuterBorder( const odr::Lane *lane );

  }; // private class Render
} // namespace roads

#endif  // #ifndef _ROADS_RENDER_H_
