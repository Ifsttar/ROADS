//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LaneImpl.h
//! \brief odr::LaneImpl class definition

#ifndef _ODR_LANE_IMPL_H_
#define _ODR_LANE_IMPL_H_

#include <odr/Lane.h>
#include "LaneWidthImpl.h"
#include <misc/DllHelper.h>
#include <misc/WeakRefServerImpl.h>
#include <misc/WeakRefContainers.h>
#include <map>
#include <string>

namespace odr
{
  class LaneSectionImpl;

  class LaneImpl:
    public ::misc::DllImpl<Lane>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    LaneImpl( LaneSectionImpl *parent, int index );

    //! Destruction: delete lane width records and update lane linkage.
    ODR_CALL ~LaneImpl();

    //! @name Methods inherited from odr::Lane
    //@{
    LaneSection* ODR_CALL parentSection( void );
    const LaneSection* ODR_CALL parentSection( void ) const;
    int index( void ) const;
    const char * ODR_CALL type( void ) const;
    void ODR_CALL setType( const char *nType );
    bool ODR_CALL level( void ) const;
    void ODR_CALL setLevel( bool nLevel );
    Lane* ODR_CALL predecessor( void );
    const Lane* ODR_CALL predecessor( void ) const;
    bool ODR_CALL setPredecessor( Lane* nPredecessor );
    Lane* ODR_CALL successor( void );
    const Lane* ODR_CALL successor( void ) const;
    bool ODR_CALL setSuccessor( Lane* nSuccessor );
    ::misc::Enumerator<LaneWidth*>* ODR_CALL
      enumerateWidths( void );
    ::misc::Enumerator<const LaneWidth*>* ODR_CALL
      enumerateWidths( void ) const;
    unsigned int ODR_CALL widthCount( void ) const;
    LaneWidth* ODR_CALL widthItem( unsigned int index );
    const LaneWidth* ODR_CALL widthItem( unsigned int index ) const;
    LaneWidth* ODR_CALL widthAt( double sOffset );
    const LaneWidth* ODR_CALL widthAt( double sOffset ) const;
    LaneWidth* ODR_CALL createWidth( double sOffset );
    //@}

    //! Propagate to document the fact that the lane was modified.
    void setModified( void );

    //! Type of the collection of lane width records.
    typedef ::misc::Map2WeakRef<double,LaneWidthImpl,std::greater<double> >
      laneWidthsType;

    //! Return the collection of lane width.
    inline laneWidthsType& laneWidths( void ) { return _laneWidths; }

    //! Set lane index. Called by parent when re-indexing lanes.
    inline void setIndex( int nIndex ) const { _index=nIndex; }

    //! Notify object that length of width records must be recomputed.
    void setLengthsNeedUpdate( void ) { _updateLengths=true; }

    //! Re-compute length of width records if necessary.
    void updateLengths( void ) const;

  private:
    LaneSectionImpl           *_parentSection;  //!< Pointer to parent section
    mutable int               _index;           //!< Cached lane index
    ::std::string             _type;            //!< Lane type
    bool                      _level;           //!< true to ignore superelev.
    ::misc::WeakRef<LaneImpl> _predecessor;     //!< Reference to predecessor
    ::misc::WeakRef<LaneImpl> _successor;       //!< Pointer to successor
    laneWidthsType            _laneWidths;      //!< Map s-coords to width rec
    mutable bool              _updateLengths;   //!< true when length of width
                                                //!< rec. need update

  }; // class LaneImpl

} // namespace odr

#endif  // _ODR_LANE_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
