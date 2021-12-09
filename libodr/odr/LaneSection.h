//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LaneSection.h
//! \brief odr::LaneSection class declaration

#ifndef _ODR_LANE_SECTION_H_
#define _ODR_LANE_SECTION_H_

#include <misc/WeakRefServer.h>
#include <misc/Enumerator.h>
#include <odr/Factories.h>

namespace odr
{
  class Lanes;
  class Lane;

  //! Class storing information on one lane section.
  //! Wraps one OpenDRIVE laneSection record. Container of lanes.
  class LaneSection:
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<LaneSection*>(p)->destroy(); }

    //! Return the lanes record this section is a part of.
    virtual Lanes* ODR_CALL parentLanes( void ) = 0;

    //! Return the lanes record this section is a part of, read-only.
    virtual const Lanes* ODR_CALL parentLanes( void ) const = 0;

    //! Return s-coordinate where this section starts.
    virtual double ODR_CALL start( void ) const = 0;

    //! Set s-coordinate where this section starts.
    //! This does not move other sections, nor does it resize parent road.
    //! Length of every section in road is re-computed accordingly.
    //! \arg nStart   New starting position.
    //! \return       Return false on invalid position or failure.
    virtual bool ODR_CALL setStart( double nStart ) = 0;

    //! Return length of this section along road's chord line.
    virtual double ODR_CALL length( void ) const = 0;

    //! Modify section's length. Road's length is modified accordingly.
    //! Other lane sections in road are moved in order to keep their length.
    virtual void ODR_CALL setLength( double ) = 0;

    //! Enumerate lanes from left to right, excluding the center lane.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<Lane*>* ODR_CALL enumerate( void ) = 0;

    //! Enumerate lanes from left ro right, excluding the center lane,
    //! read-only.
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<const Lane*>* ODR_CALL enumerate( void ) const
      = 0;

    //! Return left lane count, not including central lane.
    virtual unsigned int ODR_CALL countLeft( void ) const = 0;

    //! Return right lane count, not including central lane.
    virtual unsigned int ODR_CALL countRight( void ) const = 0;

    //! Return lane at transmitted index.
    //! \arg index    Lane index, <0 for lanes on the right, >0 for left
    //! \return       Pointer to lane, 0 on invalid index
    virtual Lane* ODR_CALL item( int index ) = 0;

    //! Return read-only lane at transmitted index.
    //! \arg index    Lane index, <0 for lanes on the right, >0 for left
    //! \return       Pointer to lane, 0 on invalid index
    virtual const Lane* ODR_CALL item( int index ) const = 0;

    //! Create a new lane and insert it into section.
    //! \arg index    Insertion index, <0 for right, >0 for left, 0 forbidden
    //! \return       Pointer to created lane, 0 on error.
    virtual Lane* ODR_CALL createLane( int index=-1 ) = 0;

  }; // class LaneSection

} // namespace odr

#endif  // #ifndef _ODR_LANE_SECTION_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
