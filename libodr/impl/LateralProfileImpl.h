//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LateralProfileImpl.h
//! \brief odr::LateralProfileImpl class declaration

#ifndef _ODR_LATERAL_PROFILE_IMPL_H_
#define _ODR_LATERAL_PROFILE_IMPL_H_

#include <odr/LateralProfile.h>
#include <misc/DllHelper.h>
#include <misc/WeakRefServerImpl.h>
#include <misc/WeakRefContainers.h>
#include "SuperelevationImpl.h"
#include "CrossfallImpl.h"

namespace odr
{
  class RoadImpl;

  //! Actual LateralProfile implementation.
  class LateralProfileImpl:
    public ::misc::DllImpl<LateralProfile>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    LateralProfileImpl( RoadImpl *parent );

    //! Destruction: delete elevation records.
    ~LateralProfileImpl();

    //! @name Methods inherited from LateralProfile
    //@{
    Road* ODR_CALL parentRoad( void );
    const Road* ODR_CALL parentRoad( void ) const;
    ::misc::Enumerator<Superelevation*>* ODR_CALL
      enumerateSuperelevations( void );
    ::misc::Enumerator<const Superelevation*>* ODR_CALL
      enumerateSuperelevations( void ) const;
    unsigned int ODR_CALL superelevationCount( void ) const;
    Superelevation* ODR_CALL superelevationItem( unsigned int index );
    const Superelevation* ODR_CALL
      superelevationItem( unsigned int index ) const;
    Superelevation* ODR_CALL superelevationAt( double s );
    const Superelevation* ODR_CALL superelevationAt( double s ) const;
    Superelevation* ODR_CALL createSuperelevation( double start );
    ::misc::Enumerator<Crossfall*>* ODR_CALL enumerateCrossfalls( void );
    ::misc::Enumerator<const Crossfall*>*
      ODR_CALL enumerateCrossfalls( void ) const;
    unsigned int ODR_CALL crossfallCount( void ) const;
    Crossfall* ODR_CALL crossfallItem( unsigned int index );
    const Crossfall* ODR_CALL crossfallItem( unsigned int index ) const;
    Crossfall* ODR_CALL crossfallAt( double s );
    const Crossfall* ODR_CALL crossfallAt( double s ) const;
    Crossfall* ODR_CALL createCrossfall( double start );
    //@}

    //! Propagate to document the fact that elevation profile was modified.
    void setModified( void );

    //! Type of the collection of elevation records.
    typedef ::misc::Map2WeakRef<double,SuperelevationImpl,std::greater<double> >
      superelevationsType;

    //! Type of the collection of crossfall records.
    typedef ::misc::Map2WeakRef<double,CrossfallImpl,std::greater<double> >
      crossfallsType;

    //! Return a reference to the collection of superelevation records.
    inline superelevationsType& superelevations( void )
    { return _superelevations; }

    //! Return a reference to the collection of crossfall records.
    inline crossfallsType& crossfalls( void ) { return _crossfalls; }

    //! Flag the length of records for update.
    inline void setLengthsNeedUpdate( void ) { _updateLengths=true; }

    //! Update the length of records.
    void updateLengths( void ) const;

  private:
    RoadImpl            *_parent;         //!< Pointer to parent road
    superelevationsType _superelevations; //!< Container of superelevations
    crossfallsType      _crossfalls;      //!< Container of crossfall records
    mutable bool        _updateLengths;   //!< true when length of either
                                          //!< crossfall or superelev. recs
                                          //!< need update.

  }; // class LateralProfileImpl

} // namespace odr

#endif  // #ifndef _ODR_LATERAL_PROFILE_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
