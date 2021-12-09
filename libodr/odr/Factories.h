//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Factories.h
//! \brief Declaration of factory functions

#ifndef _ODR_FACTORIES_H_
#define _ODR_FACTORIES_H_

//----------------------------------------------------------------------------
//! \defgroup libodr_macros    Platform-specific macros for libodr
//! \brief Platform-specific macros inserted in libodr function declarations
//@{
#ifdef WIN32
# define ODR_CALL __stdcall
# ifdef LIBODR_EXPORTS
#  define ODR_DLL __declspec( dllexport )
# else
#  define ODR_DLL __declspec( dllimport )
# endif
#else
//! Explicit calling convention specifier for libodr functions/methods
# define ODR_CALL
//! Import/export specifier for libodr functions
# define ODR_DLL
#endif
//@}


//----------------------------------------------------------------------------
namespace odr
{
  class Base;
  class Document;
  class Line;
  class Spiral;
  class Arc;
  class Poly3;
};  // namespace odr


//----------------------------------------------------------------------------
//! \defgroup Factories   Factory functions
//! \brief Factory functions for direclty constructible libodr types
//@{

#ifndef ODR_DLOPEN

extern "C" {

  //! Return pointer to a newly created document.
  ODR_DLL odr::Document* ODR_CALL odr_createDocument( const char* dbName );

} // extern "C" {

#endif // ODR_DLOPEN

//@}


//----------------------------------------------------------------------------
//! \defgroup Factory_names   Name of factory functions
//! \brief Decorated name of factory functions for libodr types,
//!        used when manually loading the lib with dlopen() / LoadLibrary().
//@{

//! Name of the factory function returning a newly created document.
#ifdef WIN32
#define ODR_CREATE_DOCUMENT_STRING  "_odr_createDocument@4"
#else
#define ODR_CREATE_DOCUMENT_STRING  "odr_createDocument"
#endif

//@}


//----------------------------------------------------------------------------
//! \defgroup Factory_types   Pointer types of factory functions
//! \brief Type of pointers to factory functions for libodr types
//@{

//! Pointer to function returning a newly created document.
typedef odr::Document* (*odr_pfn_createDocument)( const char* );

//@}

#endif  // #ifndef _ODR_FACTORIES_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
