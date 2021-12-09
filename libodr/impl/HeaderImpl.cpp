//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file HeaderImpl.cpp
//! \brief odr::Header class implementation

#include "HeaderImpl.h"

odr::HeaderImpl::HeaderImpl( void ) :
DllImpl<Header>  (),
WeakRefServerImpl(),
_name   ( "untitled" ),
_version( 1.00f ),
_date   ( "" ),
_north  ( 0.0 ),
_south  ( 0.0 ),
_east   ( 0.0 ),
_west   ( 0.0 )
{}

ODR_CALL odr::HeaderImpl::~HeaderImpl() {}

const char* odr::HeaderImpl::name( void ) const
{ return _name.c_str(); }

float odr::HeaderImpl::version( void ) const
{ return _version; }

const char* odr::HeaderImpl::date( void ) const
{ return _date.c_str(); }

double odr::HeaderImpl::north( void ) const
{ return _north; }

double odr::HeaderImpl::south( void ) const
{ return _south; }

double odr::HeaderImpl::east( void ) const
{ return _east; }

double odr::HeaderImpl::west( void ) const
{ return _west; }

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
