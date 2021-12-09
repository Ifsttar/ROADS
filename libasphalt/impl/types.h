//----------------------------------------------------------------------------
// libasphalt (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file types.h
//! \brief Declaration of libasphalt inner types.

#ifndef _ASPHALT_TYPES_H_
#define _ASPHALT_TYPES_H_

#include <asphalt/asphalt.h>
#include <asphalt/vecop.h>
#include <misc/WeakRefServerImpl.h>
#include <misc/DllHelper.h>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <string>
#include <limits>

#define ASPHALT_WELD_TOLERANCE 0.001


namespace asphalt {

  //--------------------------------------------------------------------------
  //! Utility class defining a couple of values with strict order
  template<typename T> struct Couple
  {
    T a, b;

    //! Construction: ensure that a is less than b.
    inline Couple( T na, T nb ) : a(na), b(nb) { if(nb<na) { a=nb; b=na; } }

    //! Strictly less than operator: define a strict order among couples.
    inline bool operator <( const Couple<T>& other ) const
    { return (a<other.a) || (a==other.a && b<other.b); }

    //! Equality operator: two couples are equal if both members are equal.
    inline bool operator ==( const Couple<T>& other ) const
    { return (a==other.a) && (b==other.b); }
  };


  //--------------------------------------------------------------------------
  //! Actual asphalt::Mesh implementation.
  class MeshImpl :
    public ::misc::DllImpl<Mesh>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    MeshImpl( Mesh *other=0 );

    //! Destruction.
    ~MeshImpl();

    //! @name Methods inherited from asphalt::Mesh
    //@{
    unsigned int ASPHALT_CALL nbVertices( void ) const;
    const double* ASPHALT_CALL vertexData( void ) const;
    unsigned int ASPHALT_CALL nbMaterials( void ) const;
    const char* ASPHALT_CALL materialName( unsigned int index ) const;
    unsigned int ASPHALT_CALL nbTriangles( void ) const;
    const unsigned int* ASPHALT_CALL triangleData( void ) const;
    //@}

    //! Create a new vertex and return its index.
    //! @param x, y, z      vertex coordinates
    //! @param nx, ny, nz   coordinates of normal vector
    //! @param tu, tv       texture coordinates
    //! @param lu, lv       lightmap coordinates
    //! @param ux, uy, uz   coordinates of tangent along U
    //! @param vx, vy, vz   coordinates of tangent along V
    inline unsigned int createVertex(
      double x=0.0,  double y=0.0,  double z=0.0,
      double nx=0.0, double ny=0.0, double nz=0.0,
      double tu=0.0, double tv=0.0,
      double lu=0.0, double lv=0.0,
      double ux=0.0, double uy=0.0, double uz=0.0,
      double vx=0.0, double vy=0.0, double vz=0.0 )
    {
      unsigned int base = (unsigned int)_vertexData.size();
      _vertexData.insert( _vertexData.end(), VERTEX_SIZE, 0.0 );
      _vertexData[base+0]  = x;
      _vertexData[base+1]  = y;
      _vertexData[base+2]  = z;
      _vertexData[base+3]  = nx;
      _vertexData[base+4]  = ny;
      _vertexData[base+5]  = nz;
      _vertexData[base+6]  = tu;
      _vertexData[base+7]  = tv;
      _vertexData[base+8]  = lu;
      _vertexData[base+9]  = lv;
      _vertexData[base+10] = ux;
      _vertexData[base+11] = uy;
      _vertexData[base+12] = uz;
      _vertexData[base+13] = vx;
      _vertexData[base+14] = vy;
      _vertexData[base+15] = vz;
      return base / VERTEX_SIZE;
    }

    //! Create a new vertex and return its index.
    //! @param data   pointer to vertex data stored according to convention
    inline unsigned int createVertex( const double *data )
    {
      unsigned int base = (unsigned int)_vertexData.size();
      _vertexData.insert( _vertexData.end(), VERTEX_SIZE, 0.0 );
      double *target = &(*_vertexData.begin()) + base;
      mk_vecop<16>(target) = mk_vecop<16>(data);
      return base / VERTEX_SIZE;
    }

    //! Create a new face.
    //! @param mat    material index
    //! @param v1     index of first vertex
    //! @param v2     index of second vertex (counter-clockwise order)
    //! @param v3     index of third vertex (counter-closkwise order)
    inline void createFace(
      unsigned int mat, unsigned int v1, unsigned int v2, unsigned int v3 )
    {
      _faces.push_back(Face());
      Face &face = _faces.back();
      face.material = mat;
      face.vertices.push_back(v1);
      face.vertices.push_back(v2);
      face.vertices.push_back(v3);
      face.xMin = face.xMax = _vertexData[ VERTEX_SIZE*v1 ];
      face.yMin = face.yMax = _vertexData[ VERTEX_SIZE*v1+1 ];
      face.zMin = face.zMax = _vertexData[ VERTEX_SIZE*v1+2 ];

      face.xMin = std::min( face.xMin, _vertexData[ VERTEX_SIZE*v2 ] );
      face.yMin = std::min( face.yMin, _vertexData[ VERTEX_SIZE*v2+1 ] );
      face.zMin = std::min( face.zMin, _vertexData[ VERTEX_SIZE*v2+2 ] );
      face.xMax = std::max( face.xMax, _vertexData[ VERTEX_SIZE*v2 ] );
      face.yMax = std::max( face.yMax, _vertexData[ VERTEX_SIZE*v2+1 ] );
      face.zMax = std::max( face.zMax, _vertexData[ VERTEX_SIZE*v2+2 ] );

      face.xMin = std::min( face.xMin, _vertexData[ VERTEX_SIZE*v3 ] );
      face.yMin = std::min( face.yMin, _vertexData[ VERTEX_SIZE*v3+1 ] );
      face.zMin = std::min( face.zMin, _vertexData[ VERTEX_SIZE*v3+2 ] );
      face.xMax = std::max( face.xMax, _vertexData[ VERTEX_SIZE*v3 ] );
      face.yMax = std::max( face.yMax, _vertexData[ VERTEX_SIZE*v3+1 ] );
      face.zMax = std::max( face.zMax, _vertexData[ VERTEX_SIZE*v3+2 ] );

      triangleDataNeedUpdate();
    }

    //! Return material ID for transmitted name. Create material if necessary.
    inline unsigned int materialID( const std::string& name )
    {
      std::map<std::string,unsigned int>::iterator it =
        _materialName2ID.find( name );
      if( it!=_materialName2ID.end() )
        return it->second;
      unsigned int id = _materialName2ID[name] = (unsigned int)_materials.size();
      _materials.push_back(name);
      return id;
    }

    //! Define an edge as a couple of vertex indices.
    typedef Couple<unsigned int> Edge;

    //! Cut every edge in the mesh with transmitted path in the XY-plane.
    //! @param[in]  start     pointer to coordinates of cutting segment's start
    //! @param[in]  end       pointer to coordinates of cutting segment's end
    //! @param[out] indices   list where created vertex IDs will be pushed back
    void xyKnife( const double *start,
                  const double *end,
                  std::vector<unsigned int>& indices );

    //! Cut every edge in the mesh along a lane chord line in the XY-plane.
    //! @param[in]  lane        pointer to lane along which mesh is to be cut
    //! @param[in]  sStart      start S-coordinate offset
    //! @param[in]  sEnd        end S-coordinate offset
    //! @param[in]  tOffset     offset from lane separation
    //! @param[out] indices     list where created vertex IDs are be pushed back
    //! @param[in]  sIncrement  distance between evaluated positions
    void xyKnife( const odr::Lane* lane, 
                  double sStart, double sEnd,
                  double tOffset,
                  std::vector<unsigned int>& indices,
                  double sIncrement=10.0 );

    //! Create faces between two rails.
    //! @param leftFirst    first vertex index iterator for the left rail
    //! @param leftLast     termination vertex index iterator for the left rail
    //! @param rightFirst   first vertex index iterator for the right rail
    //! @param rightLast    termination vertex index iterator for the right rail
    //! @param materialID   material for created faces
    template<typename IT1, typename IT2>
    void tesselate2rails( IT1 leftFirst,  IT1 leftLast,
                          IT2 rightFirst, IT2 rightLast,
                          unsigned int materialID );

    //! Check if a point is within a contour in the X-Y plane.
    //! @param point    pointer to coordinates of point to check
    //! @param contour  collection of vertex indices making contour
    //! @return       true if the point is within the contour
    bool xyPointInContour( const double *point,
                           const std::list<unsigned int>& contour ) const;

    //! Collect every edge of the mesh in a set.
    //! @param[out] edges   container where mesh edges are inserted
    void collectEdges( std::set<Edge>& edges ) const;

    //! Remove transmitted edges from mesh, exploding faces into several when
    //! necessary.
    void removeEdges( const std::set<Edge>& edges );

  private:
    //! Mark cached triangle data as out of date.
    inline void triangleDataNeedUpdate( void )
    { _triangleDataNeedUpdate = true; }

    //! Update triangle data from internal representation as necessary.
    void updateTriangleData( void ) const;

    struct Face {
      unsigned int material;
      std::list<unsigned int> vertices;
      double xMin, xMax, yMin, yMax, zMin, zMax;
    };

    //! Cached triangles returned by triangleData().
    mutable std::vector<unsigned int> _triangleData;

    //! True when triangles must be rebuilt from internal faces representation
    mutable bool _triangleDataNeedUpdate;

    std::vector<double>       _vertexData;  //!< Vertex data
    std::list<Face>           _faces;       //!< Internal faces representation
    std::vector<std::string>  _materials;   //!< Material names
    std::map<std::string,unsigned int> 
      _materialName2ID;                     //!< Maps material name to ID

  };  // class MeshImpl


  struct Chord;

  //--------------------------------------------------------------------------
  //! Actual asphalt::MeshBuilder implementation.
  class MeshBuilderImpl :
    public ::misc::DllImpl<MeshBuilder>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    MeshBuilderImpl( void );

    //! Destruction.
    ~MeshBuilderImpl();

    //! @name Methods inherited from asphalt::MeshBuilder
    //@{
    Mesh* ASPHALT_CALL build( const ::odr::Document* document,
                              double maxEdgeAngle,
                              double maxFaceAngle,
                              double minEdgeLength,
                              double maxEdgeLength,
                              double maxRatio,
                              Mesh *landscape,
                              double borderWidth );
    void ASPHALT_CALL abort( void );
    unsigned int ASPHALT_CALL progress( void ) const;
    //@}

  private:
    bool         _active;   //!< true when this builder is in use
    unsigned int _progress; //!< Build process progress (0-1024)
    bool         _abort;    //! true to abort build process

  };  // class MeshBuilderImpl


  //--------------------------------------------------------------------------
  //! Intermediate model used in the process of building a mesh.
  struct Intermediate
  {
    Intermediate( MeshImpl *nMesh ) :
    mesh(nMesh), nodes(), nodePair2Chord(), laneSection2Chords()
    {}

    //! Defines one end of a chord line along a lane
    struct Node {
      double x,           //!< X coordinate
             y,           //!< Y coordinate
             z,           //!< Z coordinate
             s;           //!< Cumulated S coord along chords (for road marks)
      std::map<int, unsigned int>
             lane2vertex; //!< Maps lane index to vertex

      //! Construction.
      inline Node( double xx=0.0, double yy=0.0, double zz=0.0 ) :
      x(xx), y(yy), z(zz),
      s(std::numeric_limits<double>::quiet_NaN()),
      lane2vertex()
      {}

      //! Two nodes are considered equal when they can be welded.
      inline bool operator ==( const Node& other ) const {
        return fabs(x-other.x) < ASPHALT_WELD_TOLERANCE &&
               fabs(y-other.y) < ASPHALT_WELD_TOLERANCE &&
               fabs(z-other.z) < ASPHALT_WELD_TOLERANCE ;
      }

      //! Define a strict order among nodes.
      inline bool operator <( const Node& other ) const {
        if( x + ASPHALT_WELD_TOLERANCE <= other.x ) return true;
        if( other.x + ASPHALT_WELD_TOLERANCE <= x ) return false;
        if( y + ASPHALT_WELD_TOLERANCE <= other.y ) return true;
        if( other.y + ASPHALT_WELD_TOLERANCE <= y ) return false;
        if( z + ASPHALT_WELD_TOLERANCE <= other.z ) return true;
        return false;
      }
    };

    //! A couple of nodes.
    typedef Couple<Node> NodePair;

    //! A couple of nodes with lane index.
    typedef std::pair<NodePair,int> ChordID;

    //! A chord line along a lane.
    struct Chord {
      Node            startNode,  //!< Node at the beginning of this chord
                      endNode;    //!< Node at the end of this chord
      const odr::Lane *support;   //!< Pointer to lane supporting this chord
      double          tOffset;    //!< T-coord offset from lane's outer border
      double          length;     //!< Length of chord (m)
      std::vector<unsigned int>
                      vertices;   //!< Index of vertices for chord
      std::vector<unsigned int>
                      l2vertices; //!< Index of decals vertices for chord
    };

    //! A link to a chord with parameters.
    struct ChordLink {
      Chord *chord;   //!< Pointer to chord line
      bool  forward;  //!< True if the chord line is to be followed forward
    };

    //! Link a lane section to chord lines used for building its mesh.
    struct LanesChords {
      const odr::LaneSection  *laneSection;     //!< Lane section to pave
      std::map<int,ChordLink> laneBorders;      //!< Lane ID 2 separation chord
      std::map<int,ChordLink> markRightBorders; //!< Lane ID 2 road mark chord
      std::map<int,ChordLink> markLeftBorders;  //!< Lane ID 2 road mark chord
      ChordLink               roadLeftBorder;   //!< Road left border chord
      ChordLink               roadRightBorder;  //!< Road right border chord
    };

    //! Mesh linked to this intermediate model.
    MeshImpl *mesh;

    //! All nodes in constructed scene.
    std::set<Node> nodes;

    //! Map node pairs to corresponding chord lines.
    std::map<ChordID,Chord*> nodePair2Chord;

    //! Map lane sections to corresponding collection of chord lines.
    std::map<const odr::LaneSection*,LanesChords*> laneSection2Chords;

    //! Return reference to node for transmitted position on road.
    //! Node is created if necessary.
    const Node& getNode(
      const odr::Lane *lane, double sOffset, double tOffset );

    //! Return chord link for transmitted lane and t-coord offset.
    //! The chord line and its vertices are created if necessary.
    ChordLink getChordLink( const odr::Lane *lane, double tOffset,
                            double maxEdgeAngle,
                            double maxFaceAngle,
                            double minEdgeLength,
                            double maxEdgeLength,
                            double maxRatio );

  };  // struct Intermediate

  //--------------------------------------------------------------------------
  //! Allow Intermediate::Node to be used as a vector operand
  template<> struct traits<Intermediate::Node> {
    typedef double scalar_type;
    enum { size=3 };

    template<unsigned int> struct item {
      static inline double& get( Intermediate::Node& n )
      { error_out_of_bounds; }

      static inline double get( const Intermediate::Node& n )
      { error_out_of_bounds; }
    };

    template<> struct item<0> {
      static inline double& get( Intermediate::Node& n )       { return n.x; }
      static inline double  get( const Intermediate::Node& n ) { return n.x; }
    };

    template<> struct item<1> {
      static inline double& get( Intermediate::Node& n )       { return n.y; }
      static inline double  get( const Intermediate::Node& n ) { return n.y; }
    };

    template<> struct item<2> {
      static inline double& get( Intermediate::Node& n )       { return n.z; }
      static inline double  get( const Intermediate::Node& n ) { return n.z; }
    };
  };

  //--------------------------------------------------------------------------
  //! Actual asphalt::Evaluator implementation.
  class EvaluatorImpl :
    public ::misc::DllImpl<Evaluator>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    EvaluatorImpl( void );

    //! Destruction.
    ~EvaluatorImpl();

    //! @name Methods inherited from asphalt::Evaluator
    //@{
    void ASPHALT_CALL evaluatePositionOnPrimitive(
      const odr::GeometricPrimitive* primitive,
      double sOffset,
      double *x=0,
      double *y=0,
      double *heading=0 ) const;

    void ASPHALT_CALL evaluatePositionOnLane(
      const odr::Lane* lane,
      double sOffset, double tOffset,
      double *position=0,
      double *normal=0,
      double *tangent_s=0,
      double *tangent_t=0 ) const;

    void ASPHALT_CALL evaluatePositionOnRoad(
      const odr::Road* road,
      double s, int lane, double tOffset,
      double *position=0,
      double *normal=0,
      double *dposds=0,
      double *dposdt=0 ) const;
    //@}
  };

} // namespace asphalt

#endif  // #ifndef _ASPHALT_TYPES_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
