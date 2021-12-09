//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LaneSectionImpl.h
//! \brief odr::LaneSectionImpl class declaration

#ifndef _ODR_LANE_SECTION_IMPL_H_
#define _ODR_LANE_SECTION_IMPL_H_

#include <odr/LaneSection.h>
#include <misc/DllHelper.h>
#include <misc/WeakRefServerImpl.h>
#include <misc/WeakRefContainers.h>
#include "LaneImpl.h"

namespace odr
{
  class LanesImpl;

  //! Actual LaneSection implementation.
  class LaneSectionImpl:
    public ::misc::DllImpl<LaneSection>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    LaneSectionImpl( LanesImpl *parent, double nStart );

    //! Destruction: delete lanes.
    ~LaneSectionImpl();

    //! @name Methods inherited from LaneSection
    //@{
    Lanes* ODR_CALL parentLanes( void );
    const Lanes* ODR_CALL parentLanes( void ) const;
    double ODR_CALL start( void ) const;
    bool ODR_CALL setStart( double nStart );
    double ODR_CALL length( void ) const;
    void ODR_CALL setLength( double );
    ::misc::Enumerator<Lane*>* ODR_CALL enumerate( void );
    ::misc::Enumerator<const Lane*>* ODR_CALL enumerate( void ) const;
    unsigned int ODR_CALL countLeft( void ) const;
    unsigned int ODR_CALL countRight( void ) const;
    Lane* ODR_CALL item( int index );
    const Lane* ODR_CALL item( int index ) const;
    Lane* ODR_CALL createLane( int index=1 );
    //@}

    //! Propagate to document the fact that the section was modified.
    void setModified( void );

    //! Type of the collection of lanes.
    typedef ::misc::WeakRefVector<LaneImpl> lanesType;

    //! Return reference to lane collection.
    inline lanesType& lanes( void ) { return _lanes; }

    //! Modify cached section length.
    inline void setCachedLength( double d ) const { _length=d; }

  protected:
    //! Protected methods are to be called by LaneImpl only.
    friend class LaneImpl;

    //! Recompute index of every lanes.
    void reindexLanes( void ) const;

    //! Notify that lanes must be reindexed.
    inline void setReindexNeeded( void ) { _reindexNeeded=true; }


  private:
    //! Type of collections of lanes.
    typedef ::misc::WeakRefVector<LaneImpl> lanesType;

    LanesImpl      *_parentLanes;  //!< Pointer to parent
    mutable bool   _reindexNeeded; //!< True when indices must be re-computed
    double         _start;         //!< Start s-coord of section on road
    lanesType      _lanes;         //!< All lanes from left to right
    lanesType      _lanesLeft;     //!< Lanes on the left including center
    lanesType      _lanesRight;    //!< Lanes on the right including center
    mutable double _length;        //!< Cached section length

  }; // class LaneSectionImpl

} // namespace odr

#endif  // #ifndef _ODR_LANE_SECTION_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
