//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ElementImpl.h
//! \brief odr::ElementImpl class declaration

#ifndef _ODR_ELEMENT_IMPL_H_
#define _ODR_ELEMENT_IMPL_H_

#include <odr/Element.h>
#include <misc/WeakRefServerImpl.h>
#include <string>

namespace odr
{
  class DocumentImpl;

  class ElementImpl :
    public virtual Element,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    ElementImpl( DocumentImpl *document, unsigned int id );

    //! Destruction.
    virtual ODR_CALL ~ElementImpl();

    //! @name Methods inherited from ord::Element
    //@{
    Document* ODR_CALL document( void );
    const Document* ODR_CALL document( void ) const;
    const char* ODR_CALL name( void ) const;
    unsigned int ODR_CALL ID( void ) const;
    void ODR_CALL setName( const char* );
    //@}

    //! Propagate to document the fact that the element was modified.
    void setModified( void );

  private:
    unsigned int      _id;
    odr::DocumentImpl *_document;
    std::string       _name;

  }; // class ElementImpl

} // namespace odr

#endif  // #ifndef _ODR_ELEMENT_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
