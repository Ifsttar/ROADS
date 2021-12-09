//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LaneWidthImpl.h
//! \brief odr::LaneWidthImpl class definition

#ifndef _ODR_LANE_WIDTH_IMPL_H_
#define _ODR_LANE_WIDTH_IMPL_H_

#include <odr/LaneWidth.h>
#include <misc/DllHelper.h>
#include <misc/WeakRefServerImpl.h>

namespace odr
{
  class LaneImpl;

  class LaneWidthImpl :
    public ::misc::DllImpl<LaneWidth>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    LaneWidthImpl( LaneImpl *parent, double nStartOffset );

    //! Destruction: nothing special.
    ODR_CALL ~LaneWidthImpl();

    //! @name Methods inherited from LaneWidth
    //@{
    Lane* ODR_CALL parentLane( void );
    const Lane* ODR_CALL parentLane( void ) const;
    double ODR_CALL startOffset( void ) const;
    double ODR_CALL length( void ) const;
    double ODR_CALL a( void ) const;
    double ODR_CALL b( void ) const;
    double ODR_CALL c( void ) const;
    double ODR_CALL d( void ) const;
    bool ODR_CALL setStartOffset( double nStartOffset );
    void ODR_CALL setLength( double nStart );
    void ODR_CALL setA( double v );
    void ODR_CALL setB( double v );
    void ODR_CALL setC( double v );
    void ODR_CALL setD( double v );
    //@}

    //! Propagate to document the fact that the width was modified.
    void setModified( void );

    //! Set length along S-coord for which width record is applicable.
    //! Used by LaneImpl::updateLengths().
    inline void setCachedLength( double d ) const { _length=d; }

  private:
    LaneImpl  *_parentLane;  //!< Parent lane
    double    _startOffset,  //!< S-coord start offset
              _a,            //!< Width curve parameter A (m)
              _b,            //!< Width curve parameter B
              _c,            //!< Width curve parameter C
              _d;            //!< Width curve parameter D
    mutable double _length;  //!< Length of record along S-coord

  }; // class LaneWidth

} // namespace odr

#endif  // #ifndef _ODR_LANE_WIDTH_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
