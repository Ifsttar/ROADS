//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file LanesImpl.h
//! \brief odr::LanesImpl class declaration

#ifndef _ODR_LANES_IMPL_H_
#define _ODR_LANES_IMPL_H_

#include <odr/Lanes.h>
#include <misc/DllHelper.h>
#include <misc/WeakRefServerImpl.h>
#include <misc/WeakRefContainers.h>
#include "LaneSectionImpl.h"

namespace odr
{
  class RoadImpl;

  //! Actual odr::Lanes implementation.
  class LanesImpl:
    public ::misc::DllImpl<Lanes>,
    public ::misc::WeakRefServerImpl
  {
  public:
    //! Construction.
    LanesImpl( RoadImpl *parent );

    //! Destruction: delete sections making this lanes collection.
    ODR_CALL ~LanesImpl();

    //! @name Methods inherited from odr::Lanes
    //@{
    Road* ODR_CALL parentRoad( void );
    const Road* ODR_CALL parentRoad( void ) const;
    ::misc::Enumerator<LaneSection*>* ODR_CALL enumerate( void );
    ::misc::Enumerator<const LaneSection*>* ODR_CALL enumerate( void ) const;
    unsigned int ODR_CALL count( void ) const;
    LaneSection* ODR_CALL item( unsigned int index );
    const LaneSection* ODR_CALL item( unsigned int index ) const;
    LaneSection* ODR_CALL sectionAt( double s );
    const LaneSection* ODR_CALL sectionAt( double s ) const;
    LaneSection* ODR_CALL createSection( double start );
    //@}

    //! Propagate to document the fact that the object was modified.
    void setModified( void );

    //! Recompute the length of sections if necessary.
    void updateLengths( void ) const;

    //! Notify object that length of sections must be recomputed.
    void setLengthsNeedUpdate( void );

    //! Type of the collection of sections.
    typedef ::misc::Map2WeakRef<double,LaneSectionImpl,std::greater<double> >
      sectionsType;

    //! Return a reference to the collection of lane sections.
    inline sectionsType& sections( void ) { return _sections; }

  private:
    RoadImpl     *_parent;       //!< Pointer to parent road
    sectionsType _sections;      //!< Container of lane sections
    mutable bool _updateLengths; //!< True when section lengths need update

  }; // class LanesImpl

} // namespace odr

#endif  // #ifndef _ODR_LANES_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
