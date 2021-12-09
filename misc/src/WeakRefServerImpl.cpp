//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file WeakRefServerImpl.cpp
//! \brief misc::WeakRefServerImpl class implementation.

#include <misc/WeakRefServerImpl.h>
#include <set>

MISC_CALL misc::WeakRefServerImpl::WeakRefServerImpl( void ) :
WeakRefServer(), _impl( new std::multiset<IWeakRef*>() ) {}

MISC_CALL misc::WeakRefServerImpl::~WeakRefServerImpl()
{
  std::multiset<IWeakRef*> *refs =
    static_cast< std::multiset<IWeakRef*>* >( _impl );
  while( !refs->empty() ) {
    IWeakRef *r = *refs->begin();
    r->releaseAddr( this );
  }
  delete refs;
}

void MISC_CALL misc::WeakRefServerImpl::addRef( misc::IWeakRef *r ) const {
  std::multiset<IWeakRef*> *refs =
    static_cast< std::multiset<IWeakRef*>* >( _impl );
  refs->insert( r );
}

void MISC_CALL misc::WeakRefServerImpl::removeRef( misc::IWeakRef *r ) const {
  std::multiset<IWeakRef*> *refs =
    static_cast< std::multiset<IWeakRef*>* >( _impl );
  refs->erase( refs->find(r) );
}

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
