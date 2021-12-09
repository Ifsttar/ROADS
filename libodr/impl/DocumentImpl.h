//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file DocumentImpl.h
//! \brief odr::DocumentImpl class declaration

#ifndef _ODR_DOCUMENT_IMPL_H_
#define _ODR_DOCUMENT_IMPL_H_

#include <odr/Document.h>
#include <odr/Element.h>
#include <misc/WeakRefServerImpl.h>
#include <misc/WeakRefContainers.h>
#include <map>

namespace odr
{
  class HeaderImpl;

  //! Provides an actual implementation for abstract class odr::Document.
  class DocumentImpl :
    public ::misc::DllImpl<Document>,
    public ::misc::WeakRefServerImpl
  {
  public:
    DocumentImpl( void );
    ODR_CALL ~DocumentImpl();

    //! Create road in transmitted junction.
    Road* createRoadInJunction( Junction* junction );

    //! @name Methods inherited from odr::Document
    //@{
    bool ODR_CALL init( const char *dbName );
    bool ODR_CALL load( const char *fileName, bool merge );
    bool ODR_CALL save( const char *fileName ) const;
    bool ODR_CALL modified( void ) const;
    const Header* ODR_CALL header( void ) const;
    Element* ODR_CALL getElementById( unsigned int id );
    const Element* ODR_CALL getElementById( unsigned int id ) const;
    unsigned int ODR_CALL elementCount( void ) const;
    Element* ODR_CALL item( unsigned int index );
    const Element* ODR_CALL item( unsigned int index ) const;
    ::misc::Enumerator<Element*>* ODR_CALL enumerate( void );
    ::misc::Enumerator<const Element*>* ODR_CALL enumerate( void ) const;
    Road* ODR_CALL createRoad( void );
    Junction* ODR_CALL createJunction( void );
    //@}

    //! Mark the document as modified
    inline void setModified( void ) { _modified=true; }

  private:
    //! True when something in the document was modified.
    mutable bool _modified;

    //! Type of the list of elements in document.
    typedef ::misc::WeakRefList<Element> elementsType;

    //! List of elements in document.
    elementsType _elements;

    //! Type of the map from ID to elements.
    typedef ::misc::Map2WeakRef<unsigned int, Element> id2ElementType;

    //! Elements indexed by their Id.
    id2ElementType _id2Element;

    //! Document header.
    HeaderImpl *_header;

    //! Return a free element ID.
    unsigned int genElementID( void );

  }; // class DocumentImpl

} // namespace odr

#endif  // #ifndef _ODR_DOCUMENT_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
