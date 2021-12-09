//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ElementImpl.cpp
//! \brief Element class implementation

#include "ElementImpl.h"
#include "DocumentImpl.h"

using namespace odr;

ElementImpl::ElementImpl( DocumentImpl* document, unsigned int id ) :
Element          (),
WeakRefServerImpl(),
_id              ( id ),
_name            ( "" ),
_document        ( document )
{}

ODR_CALL ElementImpl::~ElementImpl()
{ setModified(); }

Document* ElementImpl::document( void )
{ return _document; }

const Document* ElementImpl::document( void ) const
{ return _document; }

unsigned int ElementImpl::ID( void ) const { return _id; }

const char* ElementImpl::name( void ) const { return _name.c_str(); }

void ElementImpl::setModified( void ) { _document->setModified(); }

void ElementImpl::setName( const char* name ) { _name=name; setModified(); }

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
