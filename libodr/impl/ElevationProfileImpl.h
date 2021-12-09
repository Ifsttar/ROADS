//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ElevationProfileImpl.h
//! \brief odr::ElevationProfileImpl class declaration

#ifndef _ODR_ELEVATION_PROFILE_IMPL_H_
#define _ODR_ELEVATION_PROFILE_IMPL_H_

#include <odr/ElevationProfile.h>
#include <misc/DllHelper.h>
#include <misc/WeakRefServerImpl.h>
#include <misc/WeakRefContainers.h>
#include "ElevationImpl.h"

namespace odr
{
  class RoadImpl;

  //! Actual ElevationProfile implementation.
  class ElevationProfileImpl:
    public ::misc::DllImpl<ElevationProfile>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    ElevationProfileImpl( RoadImpl *parent );

    //! Destruction: delete elevation records.
    ~ElevationProfileImpl();

    //! @name Methods inherited from ElevationProfile
    //@{
    Road* ODR_CALL parentRoad( void );
    const Road* ODR_CALL parentRoad( void ) const;
    ::misc::Enumerator<Elevation*>* ODR_CALL enumerate( void );
    ::misc::Enumerator<const Elevation*>* ODR_CALL enumerate( void ) const;
    unsigned int ODR_CALL count( void ) const;
    Elevation* ODR_CALL item( unsigned int index );
    const Elevation* ODR_CALL item( unsigned int index ) const;
    Elevation* ODR_CALL elevationAt( double s );
    const Elevation* ODR_CALL elevationAt( double s ) const;
    Elevation* ODR_CALL createRecord( double start );
    //@}

    //! Propagate to document the fact that elevation profile was modified.
    void setModified( void );

    //! Type of the collection of elevation records.
    typedef ::misc::Map2WeakRef<double,ElevationImpl,std::greater<double> >
      elevationsType;

    //! Return a reference to the collection of lane sections.
    inline elevationsType& elevations( void ) { return _elevations; }

    //! Flag the object so that it re-computes the length of elevation records
    inline void setLengthsNeedUpdate( void ) { _updateLengths=true; }

    //! Update the length of elevation records if necessary.
    void updateLengths( void ) const;

  private:
    RoadImpl       *_parent;        //!< Pointer to parent road
    elevationsType _elevations;     //!< Container of elevation records
    mutable bool   _updateLengths;  //!< True when length of records nd update

  }; // class ElevationProfileImpl

} // namespace odr

#endif  // #ifndef _ODR_ELEVATION_PROFILE_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
