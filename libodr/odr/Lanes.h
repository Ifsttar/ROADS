//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Lanes.h
//! \brief odr::Lanes class declaration

#ifndef _ODR_LANES_H_
#define _ODR_LANES_H_

#include <misc/WeakRefServer.h>
#include <misc/Enumerator.h>
#include <odr/Factories.h>

namespace odr
{
  class Road;
  class LaneSection;

  //! Container of lane sections.
  //! Wraps one OpenDRIVE lanes record. Container of lane sections.
  class Lanes :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Lanes*>(p)->destroy(); }

    //! Return road owning this lanes record.
    virtual Road* ODR_CALL parentRoad( void ) = 0;

    //! Return road owning this lanes record read-only.
    virtual const Road* ODR_CALL parentRoad( void ) const = 0;

    //! Enumerate lane sections.
    //! Sections are ordered in ascending starting s-coord.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<LaneSection*>*
      ODR_CALL enumerate( void ) = 0;

    //! Enumerate lane sections, read-only.
    //! Sections are ordered in ascending starting s-coord.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<const LaneSection*>*
      ODR_CALL enumerate( void ) const = 0;

    //! Return lane sections count.
    virtual unsigned int ODR_CALL count( void ) const = 0;

    //! Return lane section at transmitted index.
    virtual LaneSection* ODR_CALL item( unsigned int index ) = 0;

    //! Return read-only lane section at transmitted index.
    virtual const LaneSection* ODR_CALL item( unsigned int index ) const = 0;

    //! Return applicable lane section for transmitted S-coordinate.
    virtual LaneSection* ODR_CALL sectionAt( double s ) = 0;

    //! Return applicable lane section for transmitted S-coordinate, read-only.
    virtual const LaneSection* ODR_CALL sectionAt( double s ) const = 0;

    //! Create a new lane section starting at transmitted s-coordinate.
    //! \arg start    S-coord where section starts.
    //! \return       Return pointer to created section, 0 on error.
    virtual LaneSection* ODR_CALL createSection( double start ) = 0;

  }; // class Lanes

} // namespace odr

#endif  // #ifndef _ODR_LANES_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
