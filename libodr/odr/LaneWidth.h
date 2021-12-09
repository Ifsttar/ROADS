//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LaneWidth.h
//! \brief odr::LaneWidth class declaration

#ifndef _ODR_LANE_WIDTH_H_
#define _ODR_LANE_WIDTH_H_

#include <odr/Factories.h>
#include <misc/WeakRefServer.h>

namespace odr
{
  class Lane;

  //! Store width information for a section of a lane.
  //! Wraps one OpenDRIVE width record.
  class LaneWidth :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<LaneWidth*>(p)->destroy(); }

    //! Return lane containing this width record.
    virtual Lane* ODR_CALL parentLane( void ) = 0;

    //! Return lane containing this width record as a constant.
    virtual const Lane* ODR_CALL parentLane( void ) const = 0;

    //! @name Read-only accessors
    //@{

    //! Return start position (s-coord) of this record relative to lane
    //! section's start.
    virtual double ODR_CALL startOffset( void ) const = 0;

    //! Return length for which width record is applicable along S-coordinate.
    virtual double ODR_CALL length( void ) const = 0;

    //! Return width curve parameter A (in m).
    virtual double ODR_CALL a( void ) const = 0;

    //! Return width curve parameter B.
    virtual double ODR_CALL b( void ) const = 0;

    //! Return width curve parameter C.
    virtual double ODR_CALL c( void ) const = 0;

    //! Return width curve parameter D.
    virtual double ODR_CALL d( void ) const = 0;

    //@}

    //! @name Write accessors
    //@{

    //! Set start position (s-coord) of this record relative to lane section's
    //! start.
    //! This does not move other width records, nor does it change parent
    //! lane section's length.
    //! Length of every width record in lane is re-computed accordingly.
    //! \arg nStartOffset   New start offset of width record, must be valid.
    //! \return             Return true on success.
    virtual bool ODR_CALL setStartOffset( double nStartOffset ) = 0;

    //! Modify record's length. Lane section's length is modified accordingly.
    //! Other width records in lane are moved in order to keep their length.
    virtual void ODR_CALL setLength( double v ) = 0;

    //! Set width curve parameter A (in m).
    virtual void ODR_CALL setA( double v ) = 0;

    //! Set width curve parameter B.
    virtual void ODR_CALL setB( double v ) = 0;

    //! Set width curve parameter C.
    virtual void ODR_CALL setC( double v ) = 0;

    //! Set width curve parameter D.
    virtual void ODR_CALL setD( double v ) = 0;

    //@}

  }; // class LaneWidth

} // namespace odr

#endif  // #ifndef _ODR_LANE_WIDTH_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
