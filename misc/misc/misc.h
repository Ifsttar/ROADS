//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file misc.h
//! \brief misc namespace documentation.

#ifndef _MISC_H_
#define _MISC_H_

//! Namespace containing miscellaneous utility classes.
namespace misc {}

//! \defgroup Misc_macros    Platform-specific macros for misc package
//! \brief Platform-specific macros inserted in misc. function declarations
//@{
#ifdef WIN32
# define MISC_CALL __stdcall
# ifdef MISC_EXPORTS
#  define MISC_DLL __declspec( dllexport )
# else
#  define MISC_DLL __declspec( dllimport )
# endif
#else
//! Explicit calling convention specifier for misc functions/methods
# define MISC_CALL
//! Import/export specifier for misc functions
# define MISC_DLL
#endif
//@}

#endif  // #ifndef _MISC_H_
