//----------------------------------------------------------------------------
// libasphalt (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file asphalt.h
//! \brief Main libasphalt include file.

#ifndef _ASPHALT_H_
#define _ASPHALT_H_

#include <misc/WeakRefServer.h>
#include <odr/odr.h>


//----------------------------------------------------------------------------
//! \defgroup libasphalt_macros    Platform-specific macros for libasphalt
//! \brief Platform-specific macros used in libasphalt function declarations
//@{
#ifdef WIN32
# define ASPHALT_CALL __stdcall
# ifdef LIBASPHALT_EXPORTS
#  define ASPHALT_DLL __declspec( dllexport )
# else
#  define ASPHALT_DLL __declspec( dllimport )
# endif
#else
//! Explicit calling convention specifier for libasphalt functions/methods
# define ASPHALT_CALL
//! Import/export specifier for libasphalt functions
# define ASPHALT_DLL
#endif
//@}


//----------------------------------------------------------------------------
//! Namespace containing all libasphalt classes
namespace asphalt {
  class MeshBuilder;
  class Evaluator;
}


//----------------------------------------------------------------------------
//! \defgroup factories   Factory functions for libasphalt types
//! \brief Factory functions for directly constructible libasphalt types
//! Declare preprocessor macro ASPHALT_DLOPEN if you want to manually load the
//! library instead of linking against it.
//@{

#ifndef ASPHALT_DLOPEN

extern "C" {

  //! Return newly created mesh builder.
  ASPHALT_DLL asphalt::MeshBuilder* ASPHALT_CALL asphalt_createBuilder(void);

  //! Return newly created evaluator.
  ASPHALT_DLL asphalt::Evaluator* ASPHALT_CALL asphalt_createEvaluator(void);

} // extern "C"

#endif  // #ifndef ASLPHALT_DLOPEN

//@}


//----------------------------------------------------------------------------
namespace asphalt {

  //--------------------------------------------------------------------------
  //! Polygonal mesh built by libasphalt.
  class Mesh :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ASPHALT_CALL operator delete( void *p )
    { if(p) static_cast<Mesh*>(p)->destroy(); }

    //! Return the number of vertices.
    virtual unsigned int ASPHALT_CALL nbVertices( void ) const = 0;

    //! Number of double values for a vertex.
    static const unsigned int VERTEX_SIZE = 16;

    //! Return a pointer to vertex data.
    //! Data for vertex number k are packed in the following way :
    //!                     coordinates
    //! [VERTEX_SIZE*k+0]    |- x
    //! [VERTEX_SIZE*k+1]    |- y
    //! [VERTEX_SIZE*k+2]    +- z
    //!                     normal
    //! [VERTEX_SIZE*k+3]    |- x
    //! [VERTEX_SIZE*k+4]    |- y
    //! [VERTEX_SIZE*k+5]    +- z
    //!                     texture coordinates
    //! [VERTEX_SIZE*k+6]    |- u
    //! [VERTEX_SIZE*k+7]    +- v
    //!                     lightmap coordinates
    //! [VERTEX_SIZE*k+8]    |- u
    //! [VERTEX_SIZE*k+9]    +- v
    //!                     normalized gradient of tex coord U (tangent)
    //! [VERTEX_SIZE*k+10]   |- x
    //! [VERTEX_SIZE*k+11]   |- y
    //! [VERTEX_SIZE*k+12]   +- z
    //!                     normalized gradient of tex coord V (tangent)
    //! [VERTEX_SIZE*k+13]   |- x
    //! [VERTEX_SIZE*k+14]   |- y
    //! [VERTEX_SIZE*k+15]   +- z
    virtual const double* ASPHALT_CALL vertexData( void ) const = 0;

    //! Return the number of materials.
    virtual unsigned int ASPHALT_CALL nbMaterials( void ) const = 0;

    //! Return material name for transmitted index.
    virtual const char* ASPHALT_CALL materialName( unsigned int index ) const
      = 0;

    //! Return the number of triangular faces.
    virtual unsigned int ASPHALT_CALL nbTriangles( void ) const = 0;

    //! Return triangle data.
    //! Data for triangle number k are packed in the following way :
    //! [4*k+0] material index
    //! [4*k+1] vertex index 1
    //! [4*k+2] vertex index 2
    //! [4*k+3] vertex index 3
    virtual const unsigned int* ASPHALT_CALL triangleData( void ) const = 0;

  }; // class Mesh

  //--------------------------------------------------------------------------
  //! Class to control the build process of a mesh by libasphalt.
  //! Allows construction to be aborted at any moment.
  class MeshBuilder :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ASPHALT_CALL operator delete( void *p )
    { if(p) static_cast<MeshBuilder*>(p)->destroy(); }

    //! Build mesh.
    //! This method returns once the mesh is completely built or after
    //! build process was aborted by calling abort() from another thread.
    //! Build process can be monitored from another thread through method
    //! progress().
    //! @param document       libodr document for which to build polygon mesh
    //! @param maxEdgeAngle   max angle between consecutive edges in top view
    //! @param maxFaceAngle   maximum angle between adgacent faces (rad)
    //! @param minEdgeLength  minimum edge length (m)
    //! @param maxEdgeLength  maximum edge length (m)
    //! @param maxRatio       maximum aspect ratio of faces
    //! @param landscape      user-provided mesh for road's surroundings
    //! @param borderWidth    distance of landscape to road
    //! @return               pointer to built mesh, 0 on error
    virtual Mesh* ASPHALT_CALL build( const ::odr::Document* document,
                                      double maxEdgeAngle=0.0349,
                                      double maxFaceAngle=0.0349,
                                      double minEdgeLength=0.1,
                                      double maxEdgeLength=10.0,
                                      double maxRatio=20.0,
                                      Mesh *landscape=0,
                                      double borderWidth=5.0 ) = 0;

    //! Abort mesh construction.
    //! build() will still be returning partially constructed mesh.
    virtual void ASPHALT_CALL abort( void ) = 0;

    //! Return progress of build process in range [0-1024].
    virtual unsigned int ASPHALT_CALL progress( void ) const = 0;

    //! Factory function for constructing actual instances.
    friend MeshBuilder* ::asphalt_createBuilder(void);

  }; // class MeshBuilder

  //--------------------------------------------------------------------------
  //! Class providing geometry evaluation facilities.
  class Evaluator :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ASPHALT_CALL operator delete( void *p )
    { if(p) static_cast<Evaluator*>(p)->destroy(); }

    //! Evaluate planar position along a geometric primitive.
    //! @param[in]  primitive   Primitive along which position is evaluated
    //! @param[in]  sOffset     S-coord offset from primitive's start
    //! @param[out] x           Pointer to location where X coord is saved
    //! @param[out] y           Pointer to location where Y coord is saved
    //! @param[out] heading     Pointer to location where heading is saved
    virtual void ASPHALT_CALL evaluatePositionOnPrimitive(
      const odr::GeometricPrimitive* primitive,
      double sOffset,
      double *x=0,
      double *y=0,
      double *heading=0 ) const = 0;

    //! Evaluate the position of a point on a lane.
    //! @param[in]  lane      Pointer to lane
    //! @param[in]  sOffset   S-coordinate offset from lane section start
    //! @param[in]  tOffset   T-coordinate offset from lane's outer border
    //! @param[out] position  Receive coordinates of point on lane
    //! @param[out] normal    Receive normal to road
    //! @param[out] tangent_s Receive tangent to road along S coordinate
    //! @param[out] tangent_t Receive tangent to road along T coordinate
    virtual void ASPHALT_CALL evaluatePositionOnLane(
      const odr::Lane* lane,
      double sOffset, double tOffset,
      double *position=0,
      double *normal=0,
      double *tangent_s=0,
      double *tangent_t=0 ) const = 0;

    //! Evalutate the position of a point on a road.
    //! @param[in]  road      Pointer to road
    //! @param[in]  s         S-coordinate along road's chord line
    //! @param[in]  lane      Lane index according to convention (clamped)
    //! @param[in]  tOffset   T-coordinate offset from lane's outer border
    //! @param[out] position  Receive 3D coordinates of point on road
    //! @param[out] normal    Receive 3D normal to road
    //! @param[out] tangent_s Receive 3D tangent to road along S coordinate
    //! @param[out] tangent_t Receive 3D tangent to road along T coordinate
    virtual void ASPHALT_CALL evaluatePositionOnRoad(
      const odr::Road* road,
      double s, int lane, double tOffset,
      double *position=0,
      double *normal=0,
      double *tangent_s=0,
      double *tangent_t=0 ) const = 0;

  }; // class Evaluator

} // namespace asphalt


//----------------------------------------------------------------------------
//! \defgroup Factory_names   Name of factory functions
//! \brief Decorated name of factory functions for libasphalt types,
//!        used when manually loading the lib with dlopen() / LoadLibrary().
//@{

//! Name of the factory function returning a newly created mesh builder.
#ifdef WIN32
#define ASPHALT_CREATE_BUILDER_STRING  "_asphalt_createBuilder@0"
#else
#define ASPHALT_CREATE_BUILDER_STRING  "asphalt_createBuilder"
#endif

//! Name of the factory function returning a newly created evaluator.
#ifdef WIN32
#define ASPHALT_CREATE_EVALUATOR_STRING "_asphalt_createEvaluator@0"
#else
#define ASPHALT_CREATE_EVALUATOR_STRING "asphalt_createEvaluator"
#endif

//@}


//----------------------------------------------------------------------------
//! \defgroup Factory_types   Pointer types of factory functions
//! \brief Type of pointers to factory functions for libasphalt types
//@{

//! Pointer to function returning a newly created mesh builder.
typedef asphalt::MeshBuilder* (*asphalt_pfn_createBuilder)(void);

//! Pointer to function returning a newly created evaluator.
typedef asphalt::Evaluator* (*asphalt_pfn_createEvaluator)(void);

//@}

#endif  // #ifndef _ASPHALT_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
