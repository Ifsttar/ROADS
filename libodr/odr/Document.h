//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Document.h
//! \brief odr::Document class declaration

#ifndef _ODR_DOCUMENT_H_
#define _ODR_DOCUMENT_H_

#include <odr/Factories.h>
#include <misc/WeakRefServer.h>
#include <misc/Enumerator.h>

namespace odr
{
  class Element;
  class Road;
  class Junction;
  class Header;

  //! odr::Document is the root of an OpenDRIVE database.
  class Document :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Document*>(p)->destroy(); }

    //! Return pointer to a newly created document.
    friend Document* ODR_CALL ::odr_createDocument( const char* dbName );
    
    //! Load OpenDRIVE file.
    //! \arg fileName   path to XODR file to open
    //! \arg merge      when true, merge data with current document
    //! \return         true on success.
    virtual bool ODR_CALL load( const char* fileName, bool merge=false ) = 0;

    //! Save OpenDRIVE file.
    //! \arg fileName   path to XODR file to save to
    //! \return         true on success.
    virtual bool ODR_CALL save( const char* fileName ) const = 0;

    //! Return document header.
    virtual const Header* ODR_CALL header( void ) const = 0;

    //! Return true when the document has been modified since the last time
    //! it was saved.
    virtual bool ODR_CALL modified( void ) const = 0;

    //! Return pointer to element with transmitted ID.
    virtual Element* ODR_CALL getElementById( unsigned int id ) = 0;

    //! Return pointer to element with transmitted ID, constant version.
    virtual const Element* ODR_CALL getElementById( unsigned int id ) const=0;

    //! Return the number of elements in the document.
    virtual unsigned int ODR_CALL elementCount( void ) const = 0;

    //! Return the element at transmitted index [0;elementCount()-1].
    virtual Element* ODR_CALL item( unsigned int index ) = 0;

    //! Return the element at transmitted index [0;elementCount()-1] as const.
    virtual const Element* ODR_CALL item( unsigned int index ) const = 0;

    //! Return an enumerator of elements in the document.
    //! Deleting current element will invalidate the enumerator.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<Element*>* ODR_CALL enumerate( void ) = 0;

    //! Return an enumerator of constant elements in the document.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<const Element*>*
      ODR_CALL enumerate( void ) const = 0;

    //! Return pointer to a newly created road record in this document.
    virtual Road* ODR_CALL createRoad( void ) = 0;

    //! Return pointer to a newly created junction record in this document.
    virtual Junction* ODR_CALL createJunction( void ) = 0;

  }; // class Document

} // namespace odr

#endif  // #ifndef _ODR_DOCUMENT_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
