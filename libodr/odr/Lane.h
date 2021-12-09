//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Lane.h
//! \brief odr::Lane class definition

#ifndef _ODR_LANE_H_
#define _ODR_LANE_H_

#include <misc/WeakRefServer.h>
#include <misc/Enumerator.h>
#include <odr/Factories.h>

namespace odr
{
  class LaneSection;
  class LaneWidth;

  //! Class storing information on one lane of a lane section.
  //! Wraps one OpenDRIVE lane record. Container of lane width records.
  class Lane :
    public virtual ::misc::WeakRefServer
  {
  public:
    //! Destroy object and deallocate memory with DLL's deallocator.
    inline void ODR_CALL operator delete( void *p )
    { if(p) static_cast<Lane*>(p)->destroy(); }

    //! Return the lane section owning this lane.
    virtual LaneSection* ODR_CALL parentSection( void ) = 0;

    //! Return the lane section owning this lane as read-only.
    virtual const LaneSection* ODR_CALL parentSection( void ) const = 0;

    //! Return lane index, according to convention.
    virtual int index( void ) const = 0;

    //! Return lane type as a string.
    virtual const char * ODR_CALL type( void ) const = 0;

    //! Set lane type.
    virtual void ODR_CALL setType( const char *nType ) = 0;

    //! Return whether to keep level and ignore superelevation or not.
    virtual bool ODR_CALL level( void ) const = 0;

    //! Set whether to keep level and ignore superelevation or not.
    virtual void ODR_CALL setLevel( bool nLevel ) = 0;

    //------------------------------------------------------------------------
    //! @name Linkage management
    //@{

    //! Return preceding lane.
    virtual Lane* ODR_CALL predecessor( void ) = 0;

    //! Return preceding lane as read-only.
    virtual const Lane* ODR_CALL predecessor( void ) const = 0;

    //! Set preceding lane.
    //! On road's ends, transmitted lane must actually match road linkage.
    //! This method does not perform 2-way linkage.
    //! \arg lane   Lane to be linked before this one, 0 to unlink.
    //! \return     Return true on success.
    virtual bool ODR_CALL setPredecessor( Lane* nPredecessor ) = 0;

    //! Return next lane.
    virtual Lane* ODR_CALL successor( void ) = 0;

    //! Return next lane as read-only.
    virtual const Lane* ODR_CALL successor( void ) const = 0;

    //! Set next lane.
    //! On road ends, transmitted lane must actually match road linkage.
    //! This method does not perform 2-way linkage.
    //! \arg lane   Lane to be linked after this one, 0 to unlink.
    //! \return     Return true on success.
    virtual bool ODR_CALL setSuccessor( Lane* nSuccessor ) = 0;

    //@}

    //------------------------------------------------------------------------
    //! @name Width management
    //@{

    //! Enumerate lane width records.
    //! Records are ordered in ascending start offset (s-coord).
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<LaneWidth*>* ODR_CALL
      enumerateWidths( void ) = 0;

    //! Enumerate lane width records as constants.
    //! Records are ordered in ascending start offset (s-coord).
    //! The user is responsible for deleting returned object.
    virtual ::misc::Enumerator<const LaneWidth*>* ODR_CALL
      enumerateWidths( void ) const = 0;

    //! Return the width records count.
    virtual unsigned int ODR_CALL widthCount( void ) const = 0;

    //! Return width record at transmitted index or 0 on error.
    virtual LaneWidth* ODR_CALL widthItem( unsigned int index ) = 0;

    //! Return width record at transmitted index as a constant or 0 on error.
    virtual const LaneWidth* ODR_CALL widthItem( unsigned int index ) const
      = 0;

    //! Return width record applicable for transmitted S-coordinate offset.
    virtual LaneWidth* ODR_CALL widthAt( double sOffset ) = 0;

    //! Return width record applicable for transmitted S-coordinate offset,
    //! read-only.
    virtual const LaneWidth* ODR_CALL widthAt( double sOffset ) const = 0;

    //! Create a new width record at transmitted offset.
    //! \arg sOffset    Start offset (s-coord) of new record. Must be valid.
    //! \return         Return pointer to created record, 0 on error.
    virtual LaneWidth* ODR_CALL createWidth( double sOffset ) = 0;

    //@}

  }; // class Lane

} // namespace odr

#endif  // #ifndef _ODR_LANE_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
