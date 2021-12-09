//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file DocumentImpl.cpp
//! \brief odr::Document class implementation

#include <odr/Factories.h>
#include "DocumentImpl.h"
#include "HeaderImpl.h"
#include "RoadImpl.h"
#include "LanesImpl.h"
#include "JunctionImpl.h"
#include <odr/Lanes.h>
#include <odr/LaneSection.h>
#include <odr/Lane.h>
#include <odr/LaneWidth.h>
#include <odr/ElevationProfile.h>
#include <odr/Elevation.h>
#include <odr/LateralProfile.h>
#include <odr/Superelevation.h>
#include <odr/Crossfall.h>
#include <misc/EnumeratorImpl.h>
#include <tinyxml.h>
#include <map>
#include <list>
#include <fstream>
#include <cstdio>

#ifdef WIN32
# define snprintf(...) _snprintf_s(__VA_ARGS__)
#endif

extern "C" ODR_DLL odr::Document* ODR_CALL
  odr_createDocument( const char* dbName )
{
  odr::DocumentImpl *doc = new odr::DocumentImpl();
  if( !(doc && doc->init(dbName)) ) { delete doc; doc=0; }
  return doc;
}

using namespace odr;

DocumentImpl::DocumentImpl( void ) :
DllImpl<Document>(),
WeakRefServerImpl(),
_modified        ( true ),
_elements        (),
_id2Element      ()
{}

ODR_CALL DocumentImpl::~DocumentImpl()
{
  while( !_elements.empty() )
    delete _elements.begin()->ptr();
}

bool DocumentImpl::init( const char *dbName )
{
  _header = new HeaderImpl();
  if( !_header ) return false;
  _header->setName( dbName );
  return true;
}

bool DocumentImpl::load( const char *fileName, bool merge )
{
  // Load XML document...
  TiXmlDocument doc;
  if( !doc.LoadFile( fileName, TIXML_ENCODING_UTF8 ) )
    return false;

  // Check type of root element...
  TiXmlElement *root = doc.RootElement();
  if( !root || root->Value()!=std::string("OpenDRIVE") )
    return false;

  // If not merging...
  if( !merge ) {
    // Clear document...
    while( !_elements.empty() )
      delete _elements.back().ptr();

    // Retrieve document header...
    const TiXmlElement *header = root->FirstChildElement("header");
    if( !header ) return false;
    float f=0.00f;
    double d=0.0;
    int i=0;
    _header->setName( header->Attribute("name") );
    if( header->QueryFloatAttribute("version",&f)!=TIXML_SUCCESS ) return false;
    _header->setVersion( f );
    _header->setDate( header->Attribute("date") );
    if( header->QueryDoubleAttribute("north",&d)!=TIXML_SUCCESS ) return false;
    _header->setNorth( d );
    if( header->QueryDoubleAttribute("south",&d)!=TIXML_SUCCESS ) return false;
    _header->setSouth( d );
    if( header->QueryDoubleAttribute("east",&d)!=TIXML_SUCCESS ) return false;
    _header->setEast( d );
    if( header->QueryDoubleAttribute("west",&d)!=TIXML_SUCCESS ) return false;
    _header->setWest( d );
  }

  // For each road in the OpenDRIVE file...
  std::map<unsigned int,unsigned int> roadID2localID;
  std::map<unsigned int,unsigned int> junctionID2localID;
  for( TiXmlElement *roadEl = root->FirstChildElement("road") ; roadEl ;
       roadEl = roadEl->NextSiblingElement("road") )
  {
    // Retrieve OpenDRIVE ID...
    int i=0;
    if( roadEl->QueryIntAttribute("id",&i)!=TIXML_SUCCESS )
      continue;

    // Check if the road is part of a junction...
    int junctionID=-1;
    if( roadEl->QueryIntAttribute("junction",&junctionID)!=TIXML_SUCCESS )
      continue;
    Road* road=0;

    // If the road is part of a junction...
    if( junctionID>=0 ) {
      Junction *junction = 0;

      // Check for the existence of such a junction...
      std::map<unsigned int,unsigned int>::const_iterator it =
        junctionID2localID.find( (unsigned int)junctionID );

      // If the junction does not exist, create it...
      if( it==junctionID2localID.end() ) {
        junction = createJunction();
        if( !junction )
          continue;
        junctionID2localID[ (unsigned int)junctionID ] = junction->ID();
      }

      // Else, use existing junction...
      else {
        junction = dynamic_cast<Junction*>( getElementById(it->second) );
        if( !junction )
          continue;
      }

      // Create road in junction...
      road = junction->createRoad();
    }

    // If the road is not part of a junction, create road in object model...
    else
      road = createRoad();

    if( !road )
      continue;

    // Update ID map...
    roadID2localID[ (unsigned int)i ] = road->ID();

    // Retrieve road attributes...
    road->setName( roadEl->Attribute("name") );
    double l=0.0;
    if( roadEl->QueryDoubleAttribute("length",&l)!=TIXML_SUCCESS ) {
      delete road;
      continue;
    }
    dynamic_cast<RoadImpl*>(road)->setLength(l);

    // Keep track of former ID in name if there is no name...
    if( std::string(road->name()).empty() ) {
      std::ostringstream sstr;
      sstr << "former_" << i;
      road->setName( sstr.str().c_str() );
    }

    // Retrieve road geometry element...
    const TiXmlElement *geometryEl = roadEl->FirstChildElement("planView");
    if( !geometryEl ) {
      delete road;
      continue;
    }
    Geometry *geometry = road->geometry();

    // Create geometric primitives...
    const TiXmlElement *primitiveEl=0;
    for( primitiveEl = geometryEl->FirstChildElement("geometry") ;
         primitiveEl ;
         primitiveEl = primitiveEl->NextSiblingElement("geometry") )
    {
      double s=0.0, x=0.0, y=0.0, hdg=0.0, length=0.0;
      if( primitiveEl->QueryDoubleAttribute("s",&s)!=TIXML_SUCCESS ||
          primitiveEl->QueryDoubleAttribute("x",&x)!=TIXML_SUCCESS ||
          primitiveEl->QueryDoubleAttribute("y",&y)!=TIXML_SUCCESS ||
          primitiveEl->QueryDoubleAttribute("hdg",&hdg)!=TIXML_SUCCESS ||
          primitiveEl->QueryDoubleAttribute("length",&length)!=TIXML_SUCCESS )
        continue;

      const TiXmlElement *el = primitiveEl->FirstChildElement();
      if( !el )
        continue;
      GeometricPrimitive *primitive=0;

      if( el->Value()==std::string("line") ) {
        Line *line = geometry->createLine(s);
        primitive=line;
      }
      else if( el->Value()==std::string("spiral") ) {
        double curvStart=0.0, curvEnd=0.0;
        if( el->QueryDoubleAttribute("curvStart",&curvStart)!=TIXML_SUCCESS ||
            el->QueryDoubleAttribute("curvEnd",&curvEnd)!=TIXML_SUCCESS )
          continue;
        Spiral *spiral = geometry->createSpiral(s);
        if( !spiral )
          continue;
        spiral->setCurvatureStart( curvStart );
        spiral->setCurvatureEnd( curvEnd );
        primitive=spiral;
      }
      else if( el->Value()==std::string("arc") ) {
        double curvature=0.0;
        if( el->QueryDoubleAttribute("curvature",&curvature)!=TIXML_SUCCESS )
          continue;
        Arc *arc = geometry->createArc(s);
        if( !arc )
          continue;
        arc->setCurvature( curvature );
        primitive=arc;
      }
      else if( el->Value()==std::string("poly3") ) {
        double a=0.0, b=0.0, c=0.0, d=0.0;
        if( el->QueryDoubleAttribute("a",&a)!=TIXML_SUCCESS ||
            el->QueryDoubleAttribute("b",&b)!=TIXML_SUCCESS ||
            el->QueryDoubleAttribute("c",&c)!=TIXML_SUCCESS ||
            el->QueryDoubleAttribute("d",&d)!=TIXML_SUCCESS )
          continue;
        Poly3 *poly3 = geometry->createPoly3(s);
        if( !poly3 )
          continue;
        poly3->setA( a );
        poly3->setB( b );
        poly3->setC( c );
        poly3->setD( d );
        primitive=poly3;
      }
      else
        continue;

      primitive->setX(x);
      primitive->setY(y);
      primitive->setHeading(hdg);
      primitive->setLength(length);
    }

    // Retrieve lanes record...
    const TiXmlElement *lanesEl = roadEl->FirstChildElement("lanes");
    if( !lanesEl ) {
      delete road;
      continue;
    }

    // Delete default lane section...
    Lanes *lanes = road->lanes();
    delete lanes->item(0);

    // For each lane section in file...
    const TiXmlElement *sectionEl = 0;
    for( sectionEl = lanesEl->FirstChildElement("laneSection") ; sectionEl ;
         sectionEl = sectionEl->NextSiblingElement("laneSection") )
    {
      // Retrieve section attributes...
      double sectionStart=0.0;
      if( sectionEl->QueryDoubleAttribute("s",&sectionStart)!=TIXML_SUCCESS )
        continue;

      // Create section...
      LaneSection *section = lanes->createSection(sectionStart);

      // Create lanes records on both side of the road...
      const TiXmlElement *left = sectionEl->FirstChildElement("left");
      const TiXmlElement *center = sectionEl->FirstChildElement("center");
      const TiXmlElement *right = sectionEl->FirstChildElement("right");
      std::list<const TiXmlElement*> laneEls;

      if( left )
        for( const TiXmlElement *laneEl=left->FirstChildElement("lane") ;
             laneEl ; laneEl=laneEl->NextSiblingElement("lane") )
        {
          section->createLane( (int)section->countLeft() + 1 );
          laneEls.push_back(laneEl);
        }

      if( right )
        for( const TiXmlElement *laneEl=right->FirstChildElement("lane") ;
             laneEl ; laneEl=laneEl->NextSiblingElement("lane") )
        {
          section->createLane( -(int)section->countRight() - 1 );
          laneEls.push_back(laneEl);
        }

      // For each lane record...
      for( std::list<const TiXmlElement*>::const_iterator it=laneEls.begin() ;
           it!=laneEls.end() ; ++it )
      {
        // Retrieve lane record in file and in object model...
        const TiXmlElement *laneEl = *it;
        int index=0;
        if( laneEl->QueryIntAttribute("id",&index)!=TIXML_SUCCESS )
          continue;
        Lane *lane = section->item(index);
        if(!lane)
          continue;

        // Retrieve lane parameters from file...
        lane->setType( laneEl->Attribute("type") );
        {
          const char *attr = laneEl->Attribute("level");
          if( attr && std::string(attr)=="true" )
            lane->setLevel(true);
          else {
            int level=0;
            laneEl->QueryIntAttribute("level",&level);
            lane->setLevel( level ? 1:0 );
          }
        }

        // Delete default lane width record...
        delete lane->widthItem(0);

        // For each width item...
        for( const TiXmlElement *widthEl=laneEl->FirstChildElement("width") ;
             widthEl ; widthEl=widthEl->NextSiblingElement("width") )
        {
          // Retrieve parameters...
          double sOffset=0.0, a=0.0, b=0.0, c=0.0, d=0.0;
          if( widthEl->QueryDoubleAttribute("sOffset",&sOffset)!=TIXML_SUCCESS ||
              widthEl->QueryDoubleAttribute("a",&a)!=TIXML_SUCCESS ||
              widthEl->QueryDoubleAttribute("b",&b)!=TIXML_SUCCESS ||
              widthEl->QueryDoubleAttribute("c",&c)!=TIXML_SUCCESS ||
              widthEl->QueryDoubleAttribute("d",&d)!=TIXML_SUCCESS )
            continue;

          // Create width item...
          LaneWidth *width = lane->createWidth(sOffset);
          if( !width )
            continue;
          width->setA(a);
          width->setB(b);
          width->setC(c);
          width->setD(d);
        } // For each width item...
      } // For each lane record...
    } // For each lane section in file...

    // For each elevation record in file...
    for( TiXmlElement *elevationEl = TiXmlHandle(roadEl)
          .FirstChildElement("elevationProfile")
          .FirstChildElement("elevation").ToElement() ;
         elevationEl ;
         elevationEl = elevationEl->NextSiblingElement("elevation") )
    {
      // Retrieve elevation record parameters...
      double s=0.0, a=0.0, b=0.0, c=0.0, d=0.0;
      if( elevationEl->QueryDoubleAttribute("s",&s)!=TIXML_SUCCESS ||
          elevationEl->QueryDoubleAttribute("a",&a)!=TIXML_SUCCESS ||
          elevationEl->QueryDoubleAttribute("b",&b)!=TIXML_SUCCESS ||
          elevationEl->QueryDoubleAttribute("c",&c)!=TIXML_SUCCESS ||
          elevationEl->QueryDoubleAttribute("d",&d)!=TIXML_SUCCESS )
        continue;

      // Create elevation record...
      Elevation *elevation = road->elevationProfile()->createRecord(s);
      if( !elevation )
        continue;
      elevation->setA( a );
      elevation->setB( b );
      elevation->setC( c );
      elevation->setD( d );
    } // For each elevation record in file...

    // For each superelevation record in file...
    for( TiXmlElement *superEl = TiXmlHandle(roadEl)
          .FirstChildElement("lateralProfile")
          .FirstChildElement("superelevation").ToElement() ;
         superEl ;
         superEl = superEl->NextSiblingElement("superelevation") )
    {
      // Retrieve superelevation record parameters...
      double s=0.0, a=0.0, b=0.0, c=0.0, d=0.0;
      if( superEl->QueryDoubleAttribute("s",&s)!=TIXML_SUCCESS ||
          superEl->QueryDoubleAttribute("a",&a)!=TIXML_SUCCESS ||
          superEl->QueryDoubleAttribute("b",&b)!=TIXML_SUCCESS ||
          superEl->QueryDoubleAttribute("c",&c)!=TIXML_SUCCESS ||
          superEl->QueryDoubleAttribute("d",&d)!=TIXML_SUCCESS )
        continue;

      // Create superelevation record...
      Superelevation *super = road->lateralProfile()->createSuperelevation(s);
      if( !super )
        continue;
      super->setA( a );
      super->setB( b );
      super->setC( c );
      super->setD( d );
    } // For each superelevation record in file...

    // For each crossfall record in file...
    for( TiXmlElement *crossfallEl = TiXmlHandle(roadEl)
          .FirstChildElement("lateralProfile")
          .FirstChildElement("crossfall").ToElement() ;
         crossfallEl ;
         crossfallEl = crossfallEl->NextSiblingElement("crossfall") )
    {
      // Retrieve crossfall record parameters...
      double s=0.0, a=0.0, b=0.0, c=0.0, d=0.0;
      if( crossfallEl->QueryDoubleAttribute("s",&s)!=TIXML_SUCCESS ||
          crossfallEl->QueryDoubleAttribute("a",&a)!=TIXML_SUCCESS ||
          crossfallEl->QueryDoubleAttribute("b",&b)!=TIXML_SUCCESS ||
          crossfallEl->QueryDoubleAttribute("c",&c)!=TIXML_SUCCESS ||
          crossfallEl->QueryDoubleAttribute("d",&d)!=TIXML_SUCCESS )
        continue;

      // Create crossfall record...
      Crossfall *cross = road->lateralProfile()->createCrossfall(s);
      if( !cross )
        continue;
      cross->setA( a );
      cross->setB( b );
      cross->setC( c );
      cross->setD( d );
    } // For each crossfall record in file...
   } // For each road in the OpenDRIVE file...

  // Perform road linkage...
  for( TiXmlElement *roadEl=root->FirstChildElement("road") ; roadEl ;
       roadEl=roadEl->NextSiblingElement("road") )
  {
    TiXmlHandle handle(roadEl);

    // Retrieve road...
    int id=0;
    if( roadEl->QueryIntAttribute("id",&id)!=TIXML_SUCCESS )
      continue;
    {
      std::map<unsigned int,unsigned int>::const_iterator it =
        roadID2localID.find( (unsigned int)id );
      if( it==roadID2localID.end() )
        continue;
      id = (int)it->second;
    }
    Road *road = dynamic_cast<Road*>( getElementById((unsigned int)id) );
    if( !road )
      continue;

    // Manage successor...
    const TiXmlElement* successorEl =
      handle.FirstChildElement("link").FirstChildElement("successor").ToElement();
    if( successorEl &&
        successorEl->QueryIntAttribute("elementId",&id)==TIXML_SUCCESS )
    {
      const std::map<unsigned int,unsigned int>& damap =
        successorEl->Attribute("elementType")==std::string("road") ?
        roadID2localID : junctionID2localID;
      {
        std::map<unsigned int,unsigned int>::const_iterator it =
          damap.find( (unsigned int)id );
        if( it==damap.end() )
          continue;
        id = (int)it->second;
      }
      Element *el = getElementById( (unsigned int)id );
      if( successorEl->Attribute("elementType")==std::string("road") )
        el = dynamic_cast<Road*>( el );
      else if( successorEl->Attribute("elementType")==std::string("junction") )
        el = dynamic_cast<Junction*>( el );
      else
        el = 0;
      const char *end = successorEl->Attribute("contactPoint");
      if( end ) {
        if( end==std::string("start") )
          road->setSuccessor( el, Road::CP_START );
        else
          road->setSuccessor( el, Road::CP_END );
      }
      else
        road->setSuccessor( el );
    }

    // Manage predecessor...
    const TiXmlElement* predecessorEl =
      handle.FirstChildElement("link").FirstChildElement("predecessor").ToElement();
    if( predecessorEl &&
        predecessorEl->QueryIntAttribute("elementId",&id)==TIXML_SUCCESS )
    {
      const std::map<unsigned int,unsigned int>& damap =
        predecessorEl->Attribute("elementType")==std::string("road") ?
        roadID2localID : junctionID2localID;
      {
        std::map<unsigned int,unsigned int>::const_iterator it =
          damap.find( (unsigned int)id );
        if( it==damap.end() )
          continue;
        id = (int)it->second;
      }
      Element *el = getElementById( (unsigned int)id );
      if( predecessorEl->Attribute("elementType")==std::string("road") )
        el = dynamic_cast<Road*>( el );
      else if( predecessorEl->Attribute("elementType")==std::string("junction") )
        el = dynamic_cast<Junction*>( el );
      else
        el = 0;
      const char *end = predecessorEl->Attribute("contactPoint");
      if( end ) {
        if( end==std::string("start") )
          road->setPredecessor( el, Road::CP_START );
        else
          road->setPredecessor( el, Road::CP_END );
      }
      else
        road->setPredecessor( el );
    }

    // Manage neighbor...
    const TiXmlElement* neighborEl =
      handle.FirstChildElement("link").FirstChildElement("neighbor").ToElement();
    if( neighborEl &&
        neighborEl->QueryIntAttribute("elementId",&id)==TIXML_SUCCESS )
    {
      {
        std::map<unsigned int,unsigned int>::const_iterator it =
          roadID2localID.find( (unsigned int)id );
        if( it==roadID2localID.end() )
          continue;
        id = (int)it->second;
      }
      Road *other = dynamic_cast<Road*>( getElementById( (unsigned int)id ) );
      std::string dir = neighborEl->Attribute("direction");
      if( dir=="same" )
        road->setNeighbor( other, true );
      else if( dir=="opposite" )
        road->setNeighbor( other, false );
      else
        road->setNeighbor( 0 );
    }
  }

  // Perform lane linkage...
  for( TiXmlElement *roadEl = root->FirstChildElement("road") ; roadEl ;
       roadEl = roadEl->NextSiblingElement("road") )
  {
    // Retrieve road...
    int id=0;
    if( roadEl->QueryIntAttribute("id",&id)!=TIXML_SUCCESS )
      continue;
    {
      std::map<unsigned int,unsigned int>::const_iterator it =
        roadID2localID.find((unsigned int)id);
      if( it==roadID2localID.end() )
        continue;
      id = (int)it->second;
    }
    Road *road = dynamic_cast<Road*>( getElementById((unsigned int)id) );
    if( !road )
      continue;
    LanesImpl *lanes = dynamic_cast<LanesImpl*>(road->lanes());
    if( !lanes )
      continue;

    // Retrieve lanes of road successor and predecessor...
    Road *nextRoad = dynamic_cast<Road*>(road->successor()),
         *prevRoad = dynamic_cast<Road*>(road->predecessor());
    /// \todo manage linkage to a junction
    LanesImpl *nextLanes = nextRoad ?
                dynamic_cast<LanesImpl*>(nextRoad->lanes()) : 0,
              *prevLanes = prevRoad ?
                dynamic_cast<LanesImpl*>(prevRoad->lanes()) : 0;

    // For each lane section of this road in XML tree...
    TiXmlElement *secEl = TiXmlHandle(roadEl).FirstChildElement("lanes")
      .FirstChildElement("laneSection").ToElement();
    for( ; secEl ; secEl = secEl->NextSiblingElement("laneSection") ) {
      // Retrieve lane section...
      double start=0.0;
      if( secEl->QueryDoubleAttribute("s",&start)!=TIXML_SUCCESS )
        continue;
      LanesImpl::sectionsType::const_iterator it =
        lanes->sections().find(start);
      if( it==lanes->sections().end() )
        continue;
      LaneSectionImpl *section = it->second.ptr();

      // Retrieve successor section...
      LanesImpl::sectionsType::const_reverse_iterator rit =
        LanesImpl::sectionsType::const_reverse_iterator(it);
      ++it;
      LaneSectionImpl *nextSection = 0;
      if( it!=lanes->sections().end() )
        nextSection = it->second.ptr();
      else if( nextLanes && !nextLanes->sections().empty() ) {
        if( road->successorContactPoint() == Road::CP_START )
          nextSection = nextLanes->sections().begin()->second.ptr();
        else
          nextSection = nextLanes->sections().rbegin()->second.ptr();
      }
        
      // Retrieve predecessor section...
      ++rit;
      LaneSectionImpl *prevSection = 0;
      if( rit!=lanes->sections().rend() )
        prevSection = rit->second.ptr();
      else if( prevLanes && !prevLanes->sections().empty() ) {
        if( road->predecessorContactPoint() == Road::CP_START )
          prevSection = prevLanes->sections().begin()->second.ptr();
        else
          prevSection = prevLanes->sections().rbegin()->second.ptr();
      }

      // For each lane of this section in XML tree...
      for( TiXmlElement *interEl = secEl->FirstChildElement() ; interEl ;
           interEl = interEl->NextSiblingElement() )
        for( TiXmlElement *laneEl = interEl->FirstChildElement("lane") ;
             laneEl ; laneEl = laneEl->NextSiblingElement() )
        {
          // Retrieve lane...
          int id = 0;
          if( laneEl->QueryIntAttribute("id",&id)!=TIXML_SUCCESS )
            continue;
          Lane *lane = section->item(id);
          if( !lane )
            continue;

          // Manage successor lane...
          TiXmlElement *el = TiXmlHandle(laneEl).FirstChildElement("link")
            .FirstChildElement("successor").ToElement();
          if( nextSection && el &&
              el->QueryIntAttribute("id",&id)==TIXML_SUCCESS )
          {
            Lane *nextLane = nextSection->item(id);
            if( nextLane ) lane->setSuccessor(nextLane);
          }

          // Manage predecessor lane...
          el = TiXmlHandle(laneEl).FirstChildElement("link")
            .FirstChildElement("predecessor").ToElement();
          if( prevSection && el &&
              el->QueryIntAttribute("id",&id)==TIXML_SUCCESS )
          {
            Lane *prevLane = prevSection->item(id);
            if( prevLane ) lane->setPredecessor(prevLane);
          }
        }
    }
  }

  // If not merging, mark the document as not modified...
  if( !merge ) _modified=false;

  return true;
}

struct EntityEncoder {
  typedef const std::string output_type;

  inline output_type operator()( char c ) {
    if( (c>='a' && c<='z') ||
        (c>='A' && c<='Z') ||
        (c>='0' && c<='9') ||
        c==' ' || c=='-' || c=='_' ||
        c==',' || c==';' || c=='.' || c==':' || c=='?' || c=='!' ||
        c=='/' || c=='\\' )
    {
      return output_type(1,c);
    }
    else {
      char out[6];
      snprintf( out, 6, "&#%2x;", c );
      return output_type(out);
    }
  }
};

inline const std::string toEntities( const std::string& in ) {
  std::ostringstream out;
  std::transform( in.begin(), in.end(),
                  std::ostream_iterator<EntityEncoder::output_type>(out),
                  EntityEncoder() );
  return out.str();
}

bool DocumentImpl::save( const char *fileName ) const
{
  std::ofstream out;

  // Enable exceptions for output stream...
  out.exceptions( std::ios::badbit | std::ios::failbit );
  try {
    // Open output file...
    out.open( fileName );

    // Write file header...
    out << "<?xml version=\"1.0\" standalone=\"yes\"?>" << std::endl
        << "<OpenDRIVE>" << std::endl
        << " <header" << std::endl
        << "  revMajor=\"1\" revMinor=\"2\"" << std::endl
        << "  name=\"" << toEntities(_header->name()) << "\"" << std::endl;
    out.precision(2);
    out.setf( std::ios::fixed );
    out << "  version=\"" << _header->version() << "\"" << std::endl
        << "  date=\"" << toEntities(_header->date()) << "\"" << std::endl;
    out.precision(3);
    out << "  north=\"" << _header->north() << "\" south=\"" << _header->south()
        << "\" east=\"" << _header->east() << "\" west=\"" << _header->west()
        << "\"" << std::endl
        << " />" << std::endl;
    out.unsetf( std::ios::floatfield );
    out.setf( std::ios::scientific );
    out.precision( 16 );

    // Enumerate roads...
    std::auto_ptr< misc::Enumerator<const Element*> > roadEnum( enumerate() );
    const Element *el=0;
    while( roadEnum.get() && (el=roadEnum->item()) ) {
      // Fetch road...
      const Road *road = dynamic_cast<const Road*>( el );
      roadEnum->moveNext();
      if( !road ) continue;

      // Write road element...
      out << " <road"
          << " name=\"" << toEntities(road->name()) << "\""
          << " length=\"" << road->length() << "\""
          << " id=\"" << road->ID() << "\""
          << " junction=\""
          << ( road->junction() ? (int)road->junction()->ID() : -1 ) << "\""
          << ">" << std::endl;

      // Road linkage...
      if( road->successor() || road->predecessor() ) {
        out << "  <link>" << std::endl;

        // Road predecessor...
        if( road->predecessor() )
          out << "   <predecessor"
              << " elementType=\""
              << (road->predecessor()->type()==Element::T_ROAD?"road":"junction")
              << "\""
              << " elementId=\"" << road->predecessor()->ID() << "\""
              << " contactPoint=\""
              << (road->predecessorContactPoint()==Road::CP_START?"start":"end")
              << "\" />" << std::endl;

        // Road successor...
        if( road->successor() )
          out << "   <successor"
              << " elementType=\""
              << (road->successor()->type()==Element::T_ROAD?"road":"junction")
              << "\""
              << " elementId=\"" << road->successor()->ID() << "\""
              << " contactPoint=\""
              << (road->successorContactPoint()==Road::CP_START?"start":"end")
              << "\" />" << std::endl;

        out << "  </link>" << std::endl;
      }

      // Begin road planar geometry...
      out << "  <planView>" << std::endl;

      // Enumerate geometric primitives...
      {
        std::auto_ptr< misc::Enumerator<const GeometricPrimitive*> > primEnum(
          road->geometry()->enumerate() );
        const GeometricPrimitive *prim = 0;
        while( primEnum.get() && (prim=primEnum->item()) ) {
          primEnum->moveNext();

          out << "   <geometry"
              << " s=\"" << prim->s() << "\""
              << " x=\"" << prim->x() << "\""
              << " y=\"" << prim->y() << "\""
              << " hdg=\"" << prim->heading() << "\""
              << " length=\"" << prim->length() << "\""
              << ">" << std::endl;

          // Line...
          if( const Line *line=dynamic_cast<const Line*>(prim) )
            out << "    <line />" << std::endl;

          // Spiral...
          else if( const Spiral *spiral=dynamic_cast<const Spiral*>(prim) )
            out << "    <spiral"
                << " curvStart=\"" << spiral->curvatureStart() << "\""
                << " curvEnd=\"" << spiral->curvatureEnd() << "\""
                << " />" << std::endl;

          // Arc...
          else if( const Arc *arc=dynamic_cast<const Arc*>(prim) )
            out << "    <arc"
                << " curvature=\"" << arc->curvature() << "\""
                << " />" << std::endl;

          // Cubic curve...
          else if( const Poly3 *poly3=dynamic_cast<const Poly3*>(prim) )
            out << "    <poly3"
                << " a=\"" << poly3->a() << "\""
                << " b=\"" << poly3->b() << "\""
                << " c=\"" << poly3->c() << "\""
                << " d=\"" << poly3->d() << "\""
                << " />" << std::endl;

          out << "   </geometry>" << std::endl;
        }
      }

      // End road planar geometry...
      out << "  </planView>" << std::endl;

      // Begin elevation profile...
      out << "  <elevationProfile>" << std::endl;

      // Enumerate elevation records...
      {
        std::auto_ptr< misc::Enumerator<const Elevation*> > elevEnum(
          road->elevationProfile()->enumerate() );
        const Elevation *elevation = 0;
        while( elevEnum.get() && (elevation=elevEnum->item()) ) {
          elevEnum->moveNext();
          out << "   <elevation"
              << " s=\"" << elevation->start() << "\""
              << " a=\"" << elevation->a() << "\""
              << " b=\"" << elevation->b() << "\""
              << " c=\"" << elevation->c() << "\""
              << " d=\"" << elevation->d() << "\""
              << " />" << std::endl;
        }
      }

      // End elevation profile...
      out << "  </elevationProfile>" << std::endl;
      
      // Begin lateral profile...
      out << "  <lateralProfile>" << std::endl;

      // Enumerate corssfall records...
      {
        std::auto_ptr< misc::Enumerator<const Crossfall*> > crossEnum(
          road->lateralProfile()->enumerateCrossfalls() );
        const Crossfall *crossfall = 0;
        while( crossEnum.get() && (crossfall=crossEnum->item()) ) {
          crossEnum->moveNext();
          out << "   <elevation"
              << " s=\"" << crossfall->start() << "\""
              << " a=\"" << crossfall->a() << "\""
              << " b=\"" << crossfall->b() << "\""
              << " c=\"" << crossfall->c() << "\""
              << " d=\"" << crossfall->d() << "\""
              << " />" << std::endl;
        }
      }

      // Enumerate superelevation records...
      {
        std::auto_ptr< misc::Enumerator<const Superelevation*> > superEnum(
          road->lateralProfile()->enumerateSuperelevations() );
        const Superelevation *super = 0;
        while( superEnum.get() && (super=superEnum->item()) ) {
          superEnum->moveNext();
          out << "   <superelevation"
              << " s=\"" << super->start() << "\""
              << " a=\"" << super->a() << "\""
              << " b=\"" << super->b() << "\""
              << " c=\"" << super->c() << "\""
              << " d=\"" << super->d() << "\""
              << " />" << std::endl;
        }
      }

      // End lateral profile...
      out << "  </lateralProfile>" << std::endl;

      // Begin lanes...
      out << "  <lanes>" << std::endl;

      // Enumerate lane sections...
      {
        std::auto_ptr< misc::Enumerator<const LaneSection*> > sectionEnum(
          road->lanes()->enumerate() );
        const LaneSection *section = 0;
        while( sectionEnum.get() && (section=sectionEnum->item()) ) {
          sectionEnum->moveNext();

          // Begin lane section...
          out << "   <laneSection"
              << " s=\"" << section->start() << "\""
              << ">" << std::endl;

          // Enumerate lanes...
          bool left=false, right=false;
          for( int i=-(int)section->countRight() ; i<=(int)section->countLeft() ; ++i ) {
            if( i<0 && !right ) {
              right=true;
              out << "    <right>" << std::endl;
            }
            else if( i==0 ) {
              if( right )
                out << "    </right>" << std::endl;
              out << "    <center>" << std::endl;
            }
            else if( i>0 && !left ) {
              out << "    </center>" << std::endl
                  << "    <left>" << std::endl;
              left = true;
            }

            // Begin lane...
            const Lane *lane = section->item(i);
            out << "     <lane"
                << " id=\"" << i << "\""
                << " type=\"" << lane->type() << "\""
                << " level=\"" << ( lane->level() ? "true" : "false" ) << "\""
                << ">" << std::endl;

            // Lane linkage...
            if( lane->successor() || lane->predecessor() ) {
              out << "      <link>" << std::endl;

              if( lane->predecessor() )
                out << "       <predecessor id=\"" << lane->predecessor()->index()
                    << "\" />" << std::endl;

              if( lane->successor() )
                out << "       <successor id=\"" << lane->successor()->index()
                    << "\" />" << std::endl;

              out << "      </link>" << std::endl;
            }

            // Enumerate width records...
            std::auto_ptr< misc::Enumerator<const LaneWidth*> > widthEnum(
              lane->enumerateWidths() );
            const LaneWidth *width = 0;
            while( widthEnum.get() && (width=widthEnum->item()) ) {
              widthEnum->moveNext();
              out << "      <width"
                  << " sOffset=\"" << width->startOffset() << "\""
                  << " a=\"" << width->a() << "\""
                  << " b=\"" << width->b() << "\""
                  << " c=\"" << width->c() << "\""
                  << " d=\"" << width->d() << "\""
                  << " />" << std::endl;
            }

            // End lane...
            out << "     </lane>" << std::endl;
          }
          if( left )
            out << "    </left>" << std::endl;
          else
            out << "    </center>" << std::endl;

          // End lane section...
          out << "   </laneSection>" << std::endl;
        }
      }

      // End lanes...
      out << "  </lanes>" << std::endl;

      // Close road element...
      out << " </road>" << std::endl;
    }

    // Close root element...
    out << "</OpenDRIVE>" << std::endl;
  }
  catch( std::exception e ) {
    return false;
  }

  // Everything OK...
  _modified=false;
  return true;
}

bool DocumentImpl::modified( void ) const
{ return _modified; }

const Header* DocumentImpl::header( void ) const {
  return _header;
}

Element* DocumentImpl::getElementById( unsigned int id )
{
  id2ElementType::iterator it = _id2Element.find( id );
  if( it==_id2Element.end() )
    return 0;
  return it->second.ptr();
}

const Element* DocumentImpl::getElementById( unsigned int id ) const
{
  id2ElementType::const_iterator it = _id2Element.find( id );
  if( it==_id2Element.end() )
    return 0;
  return it->second.ptr();
}

unsigned int DocumentImpl::elementCount( void ) const
{ return (unsigned int)_elements.size(); }

Element* DocumentImpl::item( unsigned int index )
{
  elementsType::iterator it=_elements.begin();
  while( it!=_elements.end() && index ) { ++index; ++it; }
  if( it==_elements.end() ) return 0;
  return it->ptr();
}

const Element* DocumentImpl::item( unsigned int index ) const
{
  elementsType::const_iterator it=_elements.begin();
  while( it!=_elements.end() && index ) { ++index; ++it; }
  if( it==_elements.end() ) return 0;
  return it->ptr();
}

::misc::Enumerator<Element*>* DocumentImpl::enumerate( void )
{
  return ::misc::newContainerEnumerator(
    _elements.begin(), _elements.end(), ::misc::WeakRef2Ptr<Element>() );
}

::misc::Enumerator<const Element*>* DocumentImpl::enumerate( void ) const
{
  return ::misc::newContainerEnumerator(
    _elements.begin(), _elements.end(), ::misc::WeakRef2Ptr<const Element>());
}

Road* DocumentImpl::createRoad( void )
{ return createRoadInJunction(0); }

Road* DocumentImpl::createRoadInJunction( Junction *junction )
{
  unsigned int id = genElementID();
  RoadImpl *res = new RoadImpl( this, id, junction );
  _elements.push_back( res );
  _id2Element[id] = res;
  setModified();
  return res;
}

Junction* DocumentImpl::createJunction( void )
{
  unsigned int id = genElementID();
  JunctionImpl *res = new JunctionImpl( this, id );
  _elements.push_back( res );
  _id2Element[id] = res;
  setModified();
  return res;
}

unsigned int DocumentImpl::genElementID( void )
{
  if( _elements.empty() ) return 1;
  return _id2Element.rbegin()->second->ID() + 1;
}

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
