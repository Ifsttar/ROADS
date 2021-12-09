//----------------------------------------------------------------------------
// libasphalt (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file asphalt.cpp
//! \brief Main libasphalt source file.

#include "types.h"
#include <asphalt/vecop.h>
#include <asphalt/math.h>
#include "evaluation.h"


//----------------------------------------------------------------------------
extern "C" ASPHALT_DLL asphalt::MeshBuilder*
  ASPHALT_CALL asphalt_createBuilder(void)
{
  return new asphalt::MeshBuilderImpl();
}


//----------------------------------------------------------------------------
extern "C" ASPHALT_DLL asphalt::Evaluator*
  ASPHALT_CALL asphalt_createEvaluator(void)
{
  return new asphalt::EvaluatorImpl();
}


//----------------------------------------------------------------------------
using namespace asphalt;
using namespace odr;
using namespace misc;

MeshImpl::MeshImpl( Mesh *other ) :
DllImpl<Mesh>          (),
WeakRefServerImpl      (),
_vertexData            (),
_materials             (),
_triangleData          (),
_triangleDataNeedUpdate( true ),
_faces                 ()
{
  if( other ) {
    const double *d = other->vertexData();
    for( unsigned int count=other->nbVertices() ;
          count ; --count, d+=Mesh::VERTEX_SIZE )
      createVertex( d );

    _materials.resize( other->nbMaterials() );
    for( unsigned int i=0 ; i<other->nbMaterials() ; ++i )
      _materials[i] = other->materialName(i);

    const unsigned int *u = other->triangleData();
    for( unsigned int count=other->nbTriangles() ; count ; --count, u+=4 )
      createFace( u[0], u[1], u[2], u[3] );
  }
}

MeshImpl::~MeshImpl() {}

unsigned int MeshImpl::nbVertices( void ) const
{ return (unsigned int)_vertexData.size() >> 4; }

const double* MeshImpl::vertexData( void ) const
{ return _vertexData.empty() ? 0 : &(*_vertexData.begin()); }

unsigned int MeshImpl::nbTriangles( void ) const
{
  updateTriangleData();
  return (unsigned int)_triangleData.size() >> 2;
}

const unsigned int* MeshImpl::triangleData( void ) const
{
  updateTriangleData();
  return _triangleData.empty() ? 0 : &(*_triangleData.begin());
}

void MeshImpl::updateTriangleData( void ) const
{
  if( !_triangleDataNeedUpdate ) return;

  _triangleData.clear();
  for( std::list<Face>::const_iterator fi=_faces.begin() ; fi!=_faces.end() ;
       ++fi )
  {
    if( fi->vertices.size()<3 ) continue;
    std::list<unsigned int>::const_iterator vi=fi->vertices.begin();
    unsigned int first = *vi++;
    unsigned int prev = *vi++;
    while( vi!=fi->vertices.end() ) {
      _triangleData.push_back( fi->material );
      _triangleData.push_back( first );
      _triangleData.push_back( prev );
      _triangleData.push_back( *vi );
      prev = *vi++;
    }
  }

  _triangleDataNeedUpdate = false;
}

unsigned int MeshImpl::nbMaterials( void ) const
{ return (unsigned int)_materials.size(); }

const char* MeshImpl::materialName( unsigned int index ) const
{
  return index<(unsigned int)_materials.size() ?
    _materials[index].c_str() : 0;
}

void MeshImpl::xyKnife(
  const double *start, const double *end, std::vector<unsigned int> &indices )
{
  // Find edge's bounding box in X-Y plane...
  double edgeXMin = std::min( start[0], end[0] ),
         edgeXMax = std::max( start[0], end[0] ),
         edgeYMin = std::min( start[1], end[1] ),
         edgeYMax = std::max( start[1], end[1] );

  // Map edges to the index of the vertex at their intersection with cutting
  // edge...
  std::map<Edge,unsigned int> edge2cut;

  // Map distances from segment's start position to cutting vertices...
  std::multimap<double,unsigned int> sortedCuts;

  // For each face...
  for( std::list<Face>::iterator fi=_faces.begin() ; fi!=_faces.end() ; ++fi )
  {
    Face &face = *fi;

    // Jump to next face if bounding boxes in X-Y plane do not intersect...
    if( face.xMin > edgeXMax || face.xMax < edgeXMin ||
        face.yMin > edgeYMax || face.yMax < edgeYMin )
      continue;   
    
    // Iterate on face's vertices/edges...
    unsigned int v2index = *face.vertices.rbegin();
    std::list<unsigned int>::iterator vi = face.vertices.begin();
    while( vi!=face.vertices.end() ) {
      unsigned int v1index = v2index;

      // Find next vertex in face, inserting previous cuts if necessary...
      std::map<Edge,unsigned int>::const_iterator cutIt =
        edge2cut.find( Edge(v1index,*vi) );
      while( cutIt!=edge2cut.end() ) {
        vi = face.vertices.insert( vi, cutIt->second );
        cutIt = edge2cut.find( Edge(v1index,*vi) );
      }
      v2index = *vi;

      // Retrieve coordinates of face edge's ends...
      double *v1 = &_vertexData[ v1index * VERTEX_SIZE ];
      double *v2 = &_vertexData[ v2index * VERTEX_SIZE ];

#define mvo(x) mk_vecop(x)
#define mvo2(x) mk_vecop<2>(x)
#define mvo3(x) mk_vecop<3>(x)
      
      double cutEdge[2], faceEdge[2];
      mvo(cutEdge) = mvo2(end) - mvo2(start);
      mvo(faceEdge) = mvo2(v2)  - mvo2(v1);
      double cutEdgeNormal[2];
      cutEdgeNormal[0] = -cutEdge[1];
      cutEdgeNormal[1] = cutEdge[0];

      // Give up if edges are nearly parallel...
      double faceEdge_dot_cutEdgeNormal = mvo(faceEdge)*mvo(cutEdgeNormal);
      if( fabs(faceEdge_dot_cutEdgeNormal) < 1.E-6 ) {
        ++vi;
        continue;
      }

      // Find position on face edge of intersection...
      double s = (mvo2(start)-mvo2(v1)) * mvo(cutEdgeNormal)
                  / faceEdge_dot_cutEdgeNormal;
      if( s<1.0E-6 || s>1.0-1.0E-6 ) {
        ++vi;
        continue;
      }

      // Interpolate actual position of point...
      double intersection[VERTEX_SIZE];
      double *inter = intersection;
      mvo2(inter) = mvo2(v1) + mvo(faceEdge)*s;

      // Jump to next edge if interpolated point is not on the cutting edge...
/*
      double faceEdgeNormal[2];
      faceEdgeNormal[0] = -faceEdge[1];
      faceEdgeNormal[1] = faceEdge[0];
      double t = (mvo2(v1)-mvo2(start)) * mvo(faceEdgeNormal) /
                  ( mvo(cutEdge) * mvo(faceEdgeNormal) );
*/
      double t = (mvo2(inter)-mvo2(start)) * mvo(cutEdge) / mvo(cutEdge).sqr();
      if( t<0.0 || t>1.0 ) {
        ++vi;
        continue;
      }

      // Finish interpolating other vertex information...
      inter += 2;
      v1    += 2;
      v2    += 2;
      mk_vecop<VERTEX_SIZE-2>(inter) =
        mk_vecop<VERTEX_SIZE-2>(v1)*(1.0-s) + mk_vecop<VERTEX_SIZE-2>(v2)*s;
      double tmp=0.0;
      // Normalize normal...
      inter+=1;
      tmp = mvo3(inter).norm2();
      if( tmp>0.001 ) mvo3(inter) *= 1.0/tmp;
      // Normalize U tangent...
      inter+=7;
      tmp=mvo3(inter).norm2();
      if( tmp>0.001 ) mvo3(inter) *= 1.0/tmp;
      // Normalize V tangent...
      inter+=3;
      tmp = mvo3(inter).norm2();
      if( tmp>0.001 ) mvo3(inter) *= 1.0/tmp;

#undef mvo
#undef mvo2
#undef mvo3

      // Create new vertex...
      unsigned int interIndex = createVertex(intersection);
      face.vertices.insert(vi,interIndex);
      edge2cut[ Edge(v1index,v2index) ] = interIndex;
      sortedCuts.insert( std::make_pair(t,interIndex) );
      ++vi;
    }
  }

  // Push index of created vertices to output list...
  for( std::multimap<double,unsigned int>::const_iterator it=sortedCuts.begin() ;
       it!=sortedCuts.end() ; ++it )
    indices.push_back( it->second );
}

void MeshImpl::xyKnife(
  const odr::Lane* lane, double sStart, double sEnd, double tOffset,
  std::vector<unsigned int>& indices, double sIncrement )
{
  // Evaluate starting point...
  double pos[3];
  evaluateLane( lane, sStart, tOffset, pos );

  // Setup evaluation constraint accordingly...
  EdgeLengthConstraint constraint;

  // Iterate until the ending position is reached...
  while( sStart < sEnd ) {
    mk_vecop(constraint.reference) = mk_vecop(pos);
    evaluateConstrainedPosition(
      lane, tOffset, constraint, sIncrement,
      sStart, sStart+sIncrement, sStart, pos, 0, 0, 0, 0.001, 32 );
    if( sStart > sEnd )
      evaluateLane( lane, sEnd, tOffset, pos );
    xyKnife( constraint.reference, pos, indices );
  }
}

template<typename IT1, typename IT2>
void MeshImpl::tesselate2rails(
  IT1 left, IT1 leftLast, IT2 right, IT2 rightLast, unsigned int materialID )
{
  // Leave if there's not at least one vertex index on both sides...
  if( left==leftLast || right==rightLast )
    return;

  // Retrieve pointer to vertex data...
  const double *vData = &_vertexData[0];

  // Initialize iterators...
  IT1 leftNext  = left;  ++leftNext;
  IT2 rightNext = right; ++rightNext;

  // Tesselate until the end of both rails is reached...
  while( leftNext!=leftLast || rightNext!=rightLast )
  {
    // Retrieve coordinates...
    const double *leftPos  = vData + (*left)  * Mesh::VERTEX_SIZE;
    const double *rightPos = vData + (*right) * Mesh::VERTEX_SIZE;
    const double *leftNextPos=0, *rightNextPos=0;
    if( leftNext != leftLast )
      leftNextPos = vData + (*leftNext) * Mesh::VERTEX_SIZE;
    if( rightNext != rightLast )
      rightNextPos = vData + (*rightNext) * Mesh::VERTEX_SIZE;

    // Wrap coordinates to vector operands...
    vecop<const double*,pointer_traits<const double,3>>
      lp3(leftPos), rp3(rightPos), lnp3(leftNextPos), rnp3(rightNextPos);
    vecop<const double*,pointer_traits<const double,2>>
      lp2(leftPos), rp2(rightPos), lnp2(leftNextPos), rnp2(rightNextPos);

    // Advance left if it's mandatory or if it causes less deformation...
    if( rightNext == rightLast
        || ( leftNext != leftLast
             && ((rp3-lp3)^(lnp3-lp3)).item<2>() > 0.0
             && ( ((rnp3-rp3)^(lp3-rp3)).item<2>() < 0.0
                  || (lp2-lnp2)*(rp2-lnp2)*(lp2-rnp2).norm2()*(rp2-rnp2).norm2()
                     < (lp2-rnp2)*(rp2-rnp2)*(lp2-lnp2).norm2()*(rp2-lnp2).norm2()
                     /*(rnp2-lp2).norm2() >= (lnp2-rp2).norm2()*/
                ) ) )
    {
      if( leftPos!=rightPos && leftNextPos!=rightPos )
        createFace( materialID, *left, *right, *leftNext );
      ++left;
      ++leftNext;
    }

    // ...else, advance right...
    else {
      if( leftPos!=rightPos && rightNextPos!=leftPos )
        createFace( materialID, *left, *right, *rightNext );
      ++right;
      ++rightNext;
    }
  }
}

bool MeshImpl::xyPointInContour(
  const double *point, const std::list<unsigned int>& contour ) const
{
  if( contour.size()<3 ) return false;

  bool result=false;

  unsigned int curIndex=contour.back();
  for( std::list<unsigned int>::const_iterator vi=contour.begin() ;
       vi!=contour.end() ; ++vi )
  {
    unsigned int prevIndex = curIndex;
    curIndex=*vi;
    const double *curPos  = &_vertexData[curIndex*VERTEX_SIZE],
                 *prevPos = &_vertexData[prevIndex*VERTEX_SIZE];
    double tol=-0.05;
    if( curPos[1]<prevPos[1] ) {
      const double *tmp=curPos; curPos=prevPos; prevPos=tmp;
      tol = +0.05;
    }
    if( point[1] < prevPos[1] ) continue;
    if( point[1] >= curPos[1] ) continue;
    if( point[0] >= std::max(prevPos[0],curPos[0])+tol ) continue;

    result ^=
      ( point[0] < prevPos[0] + tol +
                    (curPos[0]-prevPos[0]) * (point[1]-prevPos[1])
                      / (curPos[1]-prevPos[1]) );
  }

  return result;
}

void MeshImpl::collectEdges( std::set<Edge>& edges ) const
{
  for( std::list<Face>::const_iterator fi=_faces.begin() ;
       fi!=_faces.end() ; ++fi )
  {
    const Face &face = *fi;
    if( face.vertices.empty() ) continue;
    unsigned int v = face.vertices.back();
    for( std::list<unsigned int>::const_iterator vi=face.vertices.begin() ;
         vi!=face.vertices.end() ; ++vi )
    {
      edges.insert( Edge(v,*vi) );
      v = *vi;
    }
  }
}

void MeshImpl::removeEdges( const std::set<Edge>& edges )
{
  for( std::list<Face>::iterator fi=_faces.begin() ; fi!=_faces.end() ; )
  {
    Face &face = *fi;
    std::list< std::list<unsigned int> > newFaces;
    newFaces.push_back( std::list<unsigned int>() );

    unsigned int curIndex = face.vertices.back();
    for( std::list<unsigned int>::const_iterator vi=face.vertices.begin() ;
         vi!=face.vertices.end() ; ++vi )
    {
      unsigned int prevIndex = curIndex;
      curIndex = *vi;
      if( edges.count(Edge(prevIndex,curIndex))==0 ) {
        if( newFaces.back().empty() )
          newFaces.back().push_back(prevIndex);
        newFaces.back().push_back(curIndex);
      }
      else if( !newFaces.back().empty() )
        newFaces.push_back( std::list<unsigned int>() );
    }

    face.vertices = newFaces.front();
    newFaces.pop_front();

    if( !newFaces.empty() ) {
      face.vertices.insert(
        face.vertices.end(), newFaces.back().begin(), newFaces.back().end() );
      newFaces.pop_back();
    }

    while( !newFaces.empty() ) {
      if( newFaces.front().size()>2 ) {
        _faces.push_back( Face() );
        _faces.back().material = face.material;
        _faces.back().vertices = newFaces.front();
        _faces.back().xMin = face.xMin;
        _faces.back().xMax = face.xMax;
        _faces.back().yMin = face.yMin;
        _faces.back().yMax = face.yMax;
        _faces.back().zMin = face.zMin;
        _faces.back().zMax = face.zMax;
      }
      newFaces.pop_front();
    }

    if( face.vertices.empty() ) {
      std::list<Face>::iterator fib = fi++;
      _faces.erase( fib );
    }
    else
      ++fi;
  }
}


//----------------------------------------------------------------------------
MeshBuilderImpl::MeshBuilderImpl() :
DllImpl<MeshBuilder>(),
WeakRefServerImpl   (),
_active             ( false ),
_progress           ( 0 ),
_abort              ( false )
{}

MeshBuilderImpl::~MeshBuilderImpl()
{}

Mesh* MeshBuilderImpl::build( const Document* document,
                              double maxEdgeAngle,
                              double maxFaceAngle,
                              double minEdgeLength,
                              double maxEdgeLength,
                              double maxRatio,
                              Mesh* origLandscape,
                              double borderWidth )
{
  if( _active || !document ) return 0;

  _abort = false;
  _active = true;

  MeshImpl *mesh = new MeshImpl();
  Intermediate model( mesh );
  unsigned int matID = mesh->materialID("asphalt");

  MeshImpl *landscape = new MeshImpl( origLandscape );

  // For each road in document...
  Enumerator<const Element*> *elEnum = document->enumerate();
  const Element *el=0;
  unsigned int roadProgress = 0;
  while( elEnum && (!_abort) && (el=elEnum->item()) ) {
    elEnum->moveNext();
    _progress = ( roadProgress << 10 ) / document->elementCount();
    roadProgress++;
    const Road *road = dynamic_cast<const Road*>(el);
    if( !road ) continue;
    
    // For each section of that road...
    Enumerator<const LaneSection*> *sectionEnum = road->lanes()->enumerate();
    const LaneSection *section = 0;
    while( (!_abort) && sectionEnum && (section=sectionEnum->item()) ) {
      sectionEnum->moveNext();

      // Create corresponding intermediate object...
      Intermediate::LanesChords *chords = new Intermediate::LanesChords;
      model.laneSection2Chords[section] = chords;
      
      // For each lane in that section...
      for( int i=-(int)section->countRight();
           !_abort && i<=(int)section->countLeft() ; ++i )
      {
        // Retrieve lane...
        const Lane *lane = section->item(i);

        // Create or retrieve existing chord line...
        chords->laneBorders[i] = model.getChordLink(
          lane, 0.0, maxEdgeAngle, maxFaceAngle,
          minEdgeLength, maxEdgeLength, maxRatio );

      } // For each lane in section

      // Cut landscape along left border of the road...
      int leftLaneIndex = (int)section->countLeft();
      const odr::Lane *leftLane = section->item(leftLaneIndex);

      Intermediate::Chord *chord = new Intermediate::Chord();
      chord->startNode = model.getNode( leftLane, 0.0, borderWidth );
      chord->startNode.lane2vertex[leftLaneIndex] = landscape->createVertex(
        mesh->vertexData()
        + Mesh::VERTEX_SIZE * chord->startNode.lane2vertex[leftLaneIndex] );
      chord->endNode =
        model.getNode( leftLane, section->length(), borderWidth );
      chord->endNode.lane2vertex[leftLaneIndex] = landscape->createVertex(
        mesh->vertexData()
        + Mesh::VERTEX_SIZE * chord->endNode.lane2vertex[leftLaneIndex] );
      chord->support = leftLane;
      chord->tOffset = borderWidth;
      landscape->xyKnife(
        leftLane, 0.0, section->length(),
        borderWidth, chord->vertices, maxEdgeLength );

      chords->roadLeftBorder.forward = true;
      chords->roadLeftBorder.chord = chord;

      // Cut landscape along right border of the road...
      int rightLaneIndex = -(int)section->countRight();
      const odr::Lane *rightLane = section->item(rightLaneIndex);

      chord = new Intermediate::Chord();
      chord->startNode = model.getNode( rightLane, 0.0, -borderWidth );
      chord->startNode.lane2vertex[rightLaneIndex] = landscape->createVertex(
        mesh->vertexData()
        + Mesh::VERTEX_SIZE * chord->startNode.lane2vertex[rightLaneIndex] );
      chord->endNode =
        model.getNode( rightLane, section->length(), -borderWidth );
      chord->endNode.lane2vertex[rightLaneIndex] = landscape->createVertex(
        mesh->vertexData()
        + Mesh::VERTEX_SIZE * chord->endNode.lane2vertex[rightLaneIndex] );
      chord->support = rightLane;
      chord->tOffset = -borderWidth;
      landscape->xyKnife(
        rightLane, 0.0, section->length(),
        -borderWidth, chord->vertices, maxEdgeLength );

      chords->roadRightBorder.forward = true;
      chords->roadRightBorder.chord = chord;

      // For each lane in section...
      for( int i=-(int)section->countRight() ;
           !_abort && i<(int)section->countLeft() ; ++i )
      {
        // Retrieve left and right chords...
        Intermediate::ChordLink rightChord = chords->laneBorders[i];
        const std::vector<unsigned int> &rightVertices =
          rightChord.chord->vertices;
        Intermediate::ChordLink leftChord  = chords->laneBorders[i+1];
        const std::vector<unsigned int> &leftVertices =
          leftChord.chord->vertices;

        // Tesselate between the two chords...
        if( leftChord.forward ) {
          if( rightChord.forward )
            mesh->tesselate2rails(
              leftVertices.begin(), leftVertices.end(),
              rightVertices.begin(), rightVertices.end(), matID );
          else
            mesh->tesselate2rails(
              leftVertices.begin(), leftVertices.end(),
              rightVertices.rbegin(), rightVertices.rend(), matID );
        }
        else {
          if( rightChord.forward )
            mesh->tesselate2rails(
              leftVertices.rbegin(), leftVertices.rend(),
              rightVertices.begin(), rightVertices.end(), matID );
          else
            mesh->tesselate2rails(
              leftVertices.rbegin(), leftVertices.rend(),
              rightVertices.rbegin(), rightVertices.rend(), matID );
        }
      } // For each lane in section
    } // For each section in road
    delete sectionEnum;
  } // For each road in document
  delete elEnum;

  // Collect edges in landscape...
  std::set<MeshImpl::Edge> allEdges, edgesToDelete;
  landscape->collectEdges( allEdges );

  // Collect edges near road in landscape...
  elEnum = document->enumerate();
  while( (!_abort) && elEnum && (el=elEnum->item()) ) {
    elEnum->moveNext();
    const Road* road = dynamic_cast<const Road*>(el);
    if( !road ) continue;

    Enumerator<const LaneSection*> *sectionEnum = road->lanes()->enumerate();
    const LaneSection* section=0;
    while( sectionEnum && (section=sectionEnum->item()) ) {
      sectionEnum->moveNext();

      Intermediate::LanesChords *chords = model.laneSection2Chords[section];
      int laneLeft  = (int)section->countLeft(),
          laneRight = -(int)section->countRight();

      std::list<unsigned int> contour;
      contour.push_back(
        chords->roadLeftBorder.chord->startNode.lane2vertex[laneLeft] );
      contour.push_back(
        chords->roadRightBorder.chord->startNode.lane2vertex[laneRight] );
      contour.insert(
        contour.end(),
        chords->roadRightBorder.chord->vertices.begin(), 
        chords->roadRightBorder.chord->vertices.end() );
      contour.push_back(
        chords->roadRightBorder.chord->endNode.lane2vertex[laneRight] );
      contour.push_back(
        chords->roadLeftBorder.chord->endNode.lane2vertex[laneLeft] );
      contour.insert(
        contour.end(),
        chords->roadLeftBorder.chord->vertices.rbegin(), 
        chords->roadLeftBorder.chord->vertices.rend() );

      for( std::set<MeshImpl::Edge>::iterator it=allEdges.begin() ;
           it!=allEdges.end() ; ++it )
      {
        double midPoint[3];
        const double *end = landscape->vertexData() + Mesh::VERTEX_SIZE * it->a;
        mk_vecop(midPoint) = mk_vecop<3>(end);
        end = landscape->vertexData() + Mesh::VERTEX_SIZE * it->b;
        mk_vecop(midPoint) += mk_vecop<3>(end);
        mk_vecop(midPoint) *= 0.5;
        if( landscape->xyPointInContour(midPoint,contour) )
          edgesToDelete.insert( *it );
      }
    }
    delete sectionEnum;
  }
  delete elEnum;

  // Delete edges near road from landscape...
  landscape->removeEdges( edgesToDelete );

  // Merge landscape with road...
  std::map<unsigned int, unsigned int> landVertex2OutVertex;
  const double *lv = landscape->vertexData();
  for( unsigned int i=0 ; i<landscape->nbVertices() ; ++i, lv+=16 )
    landVertex2OutVertex[i] = mesh->createVertex( lv );
  const unsigned int *lt = landscape->triangleData();
  for( unsigned int i=0 ; i<landscape->nbTriangles() ; ++i, lt+=4 )
    mesh->createFace( mesh->materialID(landscape->materialName(lt[0])),
                      landVertex2OutVertex[ lt[1] ],
                      landVertex2OutVertex[ lt[2] ],
                      landVertex2OutVertex[ lt[3] ] );

  // Add junction between landscape and individual roads...
  unsigned int junctionMaterialID = mesh->materialID( "junction" );

  // For each non-junction road in document...
  elEnum = document->enumerate();
  while( (!_abort) && elEnum && (el=elEnum->item()) ) {
    elEnum->moveNext();
    const Road* road = dynamic_cast<const Road*>(el);
    if( !road || road->junction() ) continue;

    std::list<unsigned int>
      leftLandscapeRail,  // Indices of landscape vertices on road's left side
      leftRoadRail,       // Indices of road vertices on road's left side
      rightRoadRail,      // Indices of road vertices on road's right side
      rightLandscapeRail; // Indices of landscape vertices on road's right side

    // For each section of that road...
    Enumerator<const LaneSection*> *sectionEnum = road->lanes()->enumerate();
    const LaneSection *section = 0;
    int previousLeftVertexIndex=-1, previousRightVertexIndex=-1;
    while( (!_abort) && sectionEnum && (section=sectionEnum->item()) ) {
      sectionEnum->moveNext();

      // Retrieve corresponding chord line collection...
      Intermediate::LanesChords *chords = model.laneSection2Chords[section];

      // Translate vertex indices for left landscape rail...
      Intermediate::Chord *leftBorder = chords->roadLeftBorder.chord;
      for( std::vector<unsigned int>::iterator it=leftBorder->vertices.begin()
            ; it!=leftBorder->vertices.end() ; ++it )
        *it = landVertex2OutVertex[ *it ];

      // Concatenate vertex indices for left landscape rail...
      {
        std::vector<unsigned int>::const_iterator origin =
          leftBorder->vertices.begin();

        if( leftLandscapeRail.size() && leftBorder->vertices.size() &&
            leftLandscapeRail.back()==*origin )
          ++origin;
        
        leftLandscapeRail.insert< std::vector<unsigned int>::const_iterator >(
          leftLandscapeRail.end(), origin, leftBorder->vertices.end() );
      }

      // Concatenate vertex indices for left road rail...
      Intermediate::ChordLink leftLane =
        chords->laneBorders[ (int)section->countLeft() ];
      if( leftLane.forward ) {
        std::vector<unsigned int>::const_iterator origin =
          leftLane.chord->vertices.begin();

        if( leftRoadRail.size() && leftLane.chord->vertices.size() &&
            leftRoadRail.back()==*origin )
          ++origin;

        leftRoadRail.insert< std::vector<unsigned int>::const_iterator >(
          leftRoadRail.end(), origin, leftLane.chord->vertices.end() );
      }
      else {
        std::vector<unsigned int>::const_reverse_iterator origin =
          leftLane.chord->vertices.rbegin();

        if( leftRoadRail.size() && leftLane.chord->vertices.size() &&
            leftRoadRail.back()==*origin )
          ++origin;

        leftRoadRail.insert< std::vector<unsigned int>::const_reverse_iterator >(
          leftRoadRail.end(), origin, leftLane.chord->vertices.rend() );
      }

      // Translate vertex indices for right landscape rail...
      Intermediate::Chord *rightBorder = chords->roadRightBorder.chord;
      for( std::vector<unsigned int>::iterator it=rightBorder->vertices.begin()
            ; it!=rightBorder->vertices.end() ; ++it )
        *it = landVertex2OutVertex[ *it ];

      // Concatenate vertex indices for right landscape rail...
      {
        std::vector<unsigned int>::const_iterator origin =
          rightBorder->vertices.begin();

        if( rightLandscapeRail.size() && rightBorder->vertices.size() &&
            rightLandscapeRail.back()==*origin )
          ++origin;

        rightLandscapeRail.insert< std::vector<unsigned int>::const_iterator >(
          rightLandscapeRail.end(), origin, rightBorder->vertices.end() );
      }

      // Concatenate vertex indices for right road rail...
      Intermediate::ChordLink rightLane =
        chords->laneBorders[ -(int)section->countRight() ];
      if( rightLane.forward ) {
        std::vector<unsigned int>::const_iterator origin =
          rightLane.chord->vertices.begin();

        if( rightRoadRail.size() && rightLane.chord->vertices.size() &&
            rightRoadRail.back()==*origin )
          ++origin;

        rightRoadRail.insert< std::vector<unsigned int>::const_iterator >(
          rightRoadRail.end(), origin, rightLane.chord->vertices.end() );
      }
      else {
        std::vector<unsigned int>::const_reverse_iterator origin =
          rightLane.chord->vertices.rbegin();

        if( rightRoadRail.size() && rightLane.chord->vertices.size() &&
            rightRoadRail.back()==*origin )
          ++origin;

        rightRoadRail.insert< std::vector<unsigned int>::const_reverse_iterator >(
          rightRoadRail.end(), origin, rightLane.chord->vertices.rend() );
      }

    } // For each section of that road
    delete sectionEnum;

    // Tesselate left road border...
    mesh->tesselate2rails(
      leftLandscapeRail.begin(), leftLandscapeRail.end(),
      leftRoadRail.begin(), leftRoadRail.end(),
      junctionMaterialID );

    // Tesselate right road border...
    mesh->tesselate2rails(
      rightRoadRail.begin(), rightRoadRail.end(),
      rightLandscapeRail.begin(), rightLandscapeRail.end(),
      junctionMaterialID );

  } // For each road in document
  delete elEnum;

  // Add missing junction triangles at connections between roads...

  // First, collect connections between non-junction roads...
  std::set< Couple<const odr::Road*> > junctions;
  elEnum = document->enumerate();
  while( (!_abort) && elEnum && (el=elEnum->item()) ) {
    elEnum->moveNext();
    const Road* road = dynamic_cast<const Road*>(el);
    if( !road || road->junction() ) continue;

    const odr::Road* other =
      dynamic_cast<const odr::Road*>(road->successor());
    if( other && other->junction()==0 )
      junctions.insert( Couple<const odr::Road*>(road,other) );

    other = dynamic_cast<const odr::Road*>(road->predecessor());
    if( other && other->junction()==0 )
      junctions.insert( Couple<const odr::Road*>(road,other) );
  }
  delete elEnum;

  // ...then, for every connection, create missing junction triangles...
  for( std::set< Couple<const odr::Road*> >::const_iterator it=junctions.begin() ;
        it!=junctions.end() ; ++it )
  {
    // Retrieve junction...
    const Couple<const odr::Road*>& junction = *it;
    // Junction is oriented from road B to road A...

    // Vertex indices for triangles to be created, -1 for no vertex...
    int
      leftRoadVertexIndex=-1,
      left1stLandscapeVertexIndex=-1,
      left2ndLandscapeVertexIndex=-1,
      rightRoadVertexIndex=-1,
      right1stLandscapeVertexIndex=-1,
      right2ndLandscapeVertexIndex=-1;

    // If junction contact point for road A is road start...
    if( junction.a->predecessor()==junction.b ) {
      // Retrieve applicable lane section at road's start...
      const odr::LaneSection *section = junction.a->lanes()->sectionAt(0.0);
      if( !section ) continue;

      // Retrieve associated intermediate data...
      Intermediate::LanesChords *chords = model.laneSection2Chords[section];

      // Deduce landscape vertex index on the left of the junction...
      Intermediate::ChordLink leftLandscape = chords->roadLeftBorder;
      if( leftLandscape.chord->vertices.size() ) {
        if( leftLandscape.forward )
          left1stLandscapeVertexIndex =
            (int)leftLandscape.chord->vertices[0];
        else
          left1stLandscapeVertexIndex =
            (int)leftLandscape.chord->vertices.back();
      }

      // Deduce landscape vertex index on the right of the junction...
      Intermediate::ChordLink rightLandscape = chords->roadRightBorder;
      if( rightLandscape.chord->vertices.size() ) {
        if( rightLandscape.forward )
          right1stLandscapeVertexIndex =
            (int)rightLandscape.chord->vertices[0];
        else
          right1stLandscapeVertexIndex =
            (int)rightLandscape.chord->vertices.back();
      }

      // Deduce road vertex index on the left of the junction...
      Intermediate::ChordLink
        leftLane  = chords->laneBorders[ (int)section->countLeft() ];
      if( leftLane.chord->vertices.size() ) {
        if( leftLane.forward )
          leftRoadVertexIndex = (int)leftLane.chord->vertices[0];
        else
          leftRoadVertexIndex = (int)leftLane.chord->vertices.back();
      }

      // Deduce road vertex index on the right of the junction...
      Intermediate::ChordLink
        rightLane = chords->laneBorders[ -(int)section->countRight() ];
      if( rightLane.chord->vertices.size() ) {
        if( rightLane.forward )
          rightRoadVertexIndex = (int)rightLane.chord->vertices[0];
        else
          rightRoadVertexIndex = (int)rightLane.chord->vertices.back();
      }
    } // junction contact point for road A is road start...

    // ... else, if junction contact point for road A is road end...
    else {
      // Retrieve applicable lane section at road's end...
      const odr::LaneSection *section =
        junction.a->lanes()->sectionAt( junction.a->length() );
      if( !section ) continue;

      // Retrieve associated intermediate data...
      Intermediate::LanesChords *chords = model.laneSection2Chords[section];

      // Deduce landscape vertex index on the left of the junction...
      Intermediate::ChordLink rightLandscape = chords->roadRightBorder;
      if( rightLandscape.chord->vertices.size() ) {
        if( rightLandscape.forward )
          left1stLandscapeVertexIndex =
            (int)rightLandscape.chord->vertices.back();
        else
          left1stLandscapeVertexIndex =
            (int)rightLandscape.chord->vertices[0];
      }

      // Deduce landscape vertex index on the right of the junction...
      Intermediate::ChordLink leftLandscape = chords->roadLeftBorder;
      if( leftLandscape.chord->vertices.size() ) {
        if( leftLandscape.forward )
          right1stLandscapeVertexIndex =
            (int)leftLandscape.chord->vertices.back();
        else
          right1stLandscapeVertexIndex =
            (int)leftLandscape.chord->vertices[0];
      }

      // Deduce road vertex index on the left of the junction...
      Intermediate::ChordLink
        rightLane  = chords->laneBorders[ -(int)section->countRight() ];
      if( rightLane.chord->vertices.size() ) {
        if( rightLane.forward )
          leftRoadVertexIndex = (int)rightLane.chord->vertices.back();
        else
          leftRoadVertexIndex = (int)rightLane.chord->vertices[0];
      }

      // Deduce road vertex index on the right of the junction...
      Intermediate::ChordLink
        leftLane = chords->laneBorders[ (int)section->countLeft() ];
      if( leftLane.chord->vertices.size() ) {
        if( leftLane.forward )
          rightRoadVertexIndex = (int)leftLane.chord->vertices.back();
        else
          rightRoadVertexIndex = (int)leftLane.chord->vertices[0];
      }
    }

    // If junction contact point for road B is road's end...
    if( junction.b->successor()==junction.a ) {
      // Retrieve applicable lane section at road's end...
      const odr::LaneSection *section =
        junction.b->lanes()->sectionAt( junction.b->length() );
      if( !section ) continue;

      // Retrieve associated intermediate data...
      Intermediate::LanesChords *chords = model.laneSection2Chords[section];

      // Deduce landscape vertex index on the left of the junction...
      Intermediate::ChordLink leftLandscape = chords->roadLeftBorder;
      if( leftLandscape.chord->vertices.size() ) {
        if( leftLandscape.forward )
          left2ndLandscapeVertexIndex =
            (int)leftLandscape.chord->vertices.back();
        else
          left2ndLandscapeVertexIndex =
            (int)leftLandscape.chord->vertices[0];
      }

      // Deduce landscape vertex index on the right of the junction...
      Intermediate::ChordLink rightLandscape = chords->roadRightBorder;
      if( rightLandscape.chord->vertices.size() ) {
        if( rightLandscape.forward )
          right2ndLandscapeVertexIndex =
            (int)rightLandscape.chord->vertices.back();
        else
          right2ndLandscapeVertexIndex =
            (int)rightLandscape.chord->vertices[0];
      }
    } // junction contact point for road B is road end

    // ... else, if junction contact point for road B is road's start...
    else {
      // Retrieve applicable lane section at road's start...
      const odr::LaneSection *section = junction.b->lanes()->sectionAt(0.0);
      if( !section ) continue;

      // Retrieve associated intermediate data...
      Intermediate::LanesChords *chords = model.laneSection2Chords[section];

      // Deduce landscape vertex index on the left of the junction...
      Intermediate::ChordLink rightLandscape = chords->roadRightBorder;
      if( rightLandscape.chord->vertices.size() ) {
        if( rightLandscape.forward )
          left2ndLandscapeVertexIndex =
            (int)rightLandscape.chord->vertices[0];
        else
          left2ndLandscapeVertexIndex =
            (int)rightLandscape.chord->vertices.back();
      }

      // Deduce landscape vertex index on the right of the junction...
      Intermediate::ChordLink leftLandscape = chords->roadLeftBorder;
      if( leftLandscape.chord->vertices.size() ) {
        if( leftLandscape.forward )
          right2ndLandscapeVertexIndex =
            (int)leftLandscape.chord->vertices[0];
        else
          right2ndLandscapeVertexIndex =
            (int)leftLandscape.chord->vertices.back();
      }
    }

    // Create triangle on the left of the junction...
    if( leftRoadVertexIndex>=0 &&
        left1stLandscapeVertexIndex>=0 &&
        left2ndLandscapeVertexIndex>=0 )
      mesh->createFace( junctionMaterialID,
                        (unsigned int)left1stLandscapeVertexIndex,
                        (unsigned int)left2ndLandscapeVertexIndex,
                        (unsigned int)leftRoadVertexIndex );

    // Create triangle on the right of the junction...
    if( rightRoadVertexIndex>=0 &&
        right1stLandscapeVertexIndex>=0 &&
        right2ndLandscapeVertexIndex>=0 )
      mesh->createFace( junctionMaterialID,
                        (unsigned int)right2ndLandscapeVertexIndex,
                        (unsigned int)right1stLandscapeVertexIndex,
                        (unsigned int)rightRoadVertexIndex );


  } // for every junction, create missing triangles

  // Done...
  _active = false;
  return mesh;
}

void MeshBuilderImpl::abort( void )
{ _abort = true; }

unsigned int MeshBuilderImpl::progress( void ) const
{ return _progress; }


//----------------------------------------------------------------------------
const Intermediate::Node& Intermediate::getNode(
  const odr::Lane *lane, double sOffset, double tOffset )
{
  // Evaluate coordinates of node...
  double pos[3], normal[3], tans[3], tant[3];
  evaluateLane( lane, sOffset, tOffset, pos, normal, tans, tant );

  // Look for a similar node in the model...
  Intermediate::Node tmp( pos[0], pos[1], pos[2] );
  std::set<Intermediate::Node>::iterator it = nodes.find( tmp );

  // If no similar node could be found, add node to model...
  if( it==nodes.end() )
    it = nodes.insert( tmp ).first;

  // If the node does not hold any vertex for transmitted lane index,
  // create vertex...
  int laneIndex = lane->index();
  if( it->lane2vertex.find(laneIndex)==it->lane2vertex.end() )
    it->lane2vertex[laneIndex] = mesh->createVertex(
      it->x, it->y, it->z,
      normal[0], normal[1], normal[2],
      lane->parentSection()->start() + sOffset, (double)laneIndex+tOffset, 
      pos[0], pos[1],
      tans[0], tans[1], tans[2],
      tant[0], tant[1], tant[2] );

  return *it;
}

Intermediate::ChordLink Intermediate::getChordLink(
  const odr::Lane *lane, double tOffset,
  double maxEdgeAngle, double maxFaceAngle,
  double minEdgeLength, double maxEdgeLength, double maxRatio )
{
  // Retrieve lanes section and road...
  const odr::LaneSection *section = lane->parentSection();
  const odr::Road *road = section->parentLanes()->parentRoad();
  int laneIndex = lane->index();

  // Evaluate coordinates at chord's ends...
  double startPos[3], startN[3], startS[3], startT[3];
  evaluateLane( lane, 0.0, tOffset,
                startPos, startN, startS, startT );

  // Setup mesh generation constraints accordingly...
  EdgeLengthConstraint  edgeLengthConstraint;
  XYAngleConstraint     edgeAngleConstraint;
  NormalAngleConstraint faceAngleConstraint;

  mk_vecop(edgeLengthConstraint.reference) = mk_vecop(startPos);

  edgeAngleConstraint.referencePoint[0] = startPos[0];
  edgeAngleConstraint.referencePoint[1] = startPos[1];
  edgeAngleConstraint.referenceDirection[0] = startS[0];
  edgeAngleConstraint.referenceDirection[1] = startS[1];

  mk_vecop(faceAngleConstraint.reference) = mk_vecop(startN);

  // Retrieve start and end nodes...
  Intermediate::Node
    start = getNode( lane, 0.0, tOffset ),
    end   = getNode( lane, section->length(), tOffset );

  // Check if chord exists...
  Intermediate::ChordLink chordLink = { 0, true };
  std::map<ChordID,Chord*>::iterator it = nodePair2Chord.find(
    ChordID( NodePair(start,end), laneIndex )  );

  // Create new chord if necessary...
  if( it==nodePair2Chord.end() ) {
    chordLink.chord = new Intermediate::Chord();
    nodePair2Chord[ ChordID( NodePair(start,end), laneIndex ) ] =
      chordLink.chord;
    chordLink.chord->startNode = start;
    chordLink.chord->endNode = end;
    chordLink.chord->support = lane;
    chordLink.forward = true;

    chordLink.chord->vertices.push_back( start.lane2vertex[laneIndex] );

    // Create chord mesh...
    for( double s=0.0 ; s < section->length() + ASPHALT_WELD_TOLERANCE ; )
    {
      double pos[3], norm[3], tans[3], tant[3];
      double oldS = s;

      // Satisfy max length constraint...
      evaluateConstrainedPosition(
        lane, tOffset,
        edgeLengthConstraint, maxEdgeLength,
        oldS, oldS+maxEdgeLength, s,
        pos, norm, tans, tant, 0.001, 32 );

      // Satisfy edge angle constraint if not satisfied...
      if( edgeAngleConstraint(pos,norm,tans,tant) > 1.05*maxEdgeAngle )
        evaluateConstrainedPosition(
          lane, tOffset,
          edgeAngleConstraint, maxEdgeAngle,
          oldS, s, s,
          pos, norm, tans, tant, 0.05*maxEdgeAngle, 32 );

      // Satisfy face angle constraint if not satisfied...
      if( faceAngleConstraint(pos,norm,tans,tant) > 1.05*maxFaceAngle )
        evaluateConstrainedPosition(
          lane, tOffset,
          faceAngleConstraint, maxFaceAngle,
          oldS, s, s,
          pos, norm, tans, tant, 0.05*maxFaceAngle, 32 );

      // Satisfy min length constraint if not satisfyed...
      if( edgeLengthConstraint(pos,norm,tans,tant) < minEdgeLength+0.001 )
        evaluateConstrainedPosition(
          lane, tOffset,
          edgeLengthConstraint, minEdgeLength,
          oldS, oldS+minEdgeLength, s,
          pos, norm, tans, tant, 0.001, 32 );

      // Add vertex when applicable...
      if( s < section->length() )
        chordLink.chord->vertices.push_back(  mesh->createVertex(
          pos[0], pos[1], pos[2],
          norm[0], norm[1], norm[2],
          s, (double)laneIndex, pos[0], pos[1],
          tans[0], tans[1], tans[2],
          tant[0], tant[1], tant[2] )  );

      // Update mesh constraints...
      mk_vecop(edgeLengthConstraint.reference) = mk_vecop(pos);

      edgeAngleConstraint.referenceDirection[0] = tans[0];
      edgeAngleConstraint.referenceDirection[1] = tans[1];
      edgeAngleConstraint.referencePoint[0] = pos[0];
      edgeAngleConstraint.referencePoint[1] = pos[1];

      mk_vecop(faceAngleConstraint.reference) = mk_vecop(norm);
    }

    chordLink.chord->vertices.push_back( end.lane2vertex[laneIndex] );
  }
  else {
    chordLink.chord = it->second;
    chordLink.forward = ( chordLink.chord->startNode==start );
  }

  // Return result...
  return chordLink;
}


//----------------------------------------------------------------------------
EvaluatorImpl::EvaluatorImpl( void )
{}

EvaluatorImpl::~EvaluatorImpl()
{}

void EvaluatorImpl::evaluatePositionOnPrimitive(
  const odr::GeometricPrimitive* primitive, double sOffset,
  double *x, double *y, double *heading ) const
{
  evaluatePrimitive( primitive, sOffset, x, y, heading );
}

void EvaluatorImpl::evaluatePositionOnLane(
  const odr::Lane* lane, double sOffset, double tOffset,
  double *position, double *normal, double *dposds, double *dposdt ) const
{
  evaluateLane( lane, sOffset, tOffset, position, normal, dposds, dposdt );
}

void EvaluatorImpl::evaluatePositionOnRoad(
  const odr::Road* road, double s, int lane, double tOffset,
  double *position, double *normal, double *dposds, double *dposdt ) const
{
  evaluatePosition( road, s, lane, tOffset, position, normal, dposds, dposdt );
}

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
