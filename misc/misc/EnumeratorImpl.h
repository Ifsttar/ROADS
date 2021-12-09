//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file EnumeratorImpl.h
//! \brief misc::Enumerator abstract template class implementation.

#ifndef _ENUMERATOR_IMPL_H_
#define _ENUMERATOR_IMPL_H_

#include <misc/Enumerator.h>

namespace misc
{
  //--------------------------------------------------------------------------
  //! Identity object adapter: convert an object into oneself.
  //! An implicit cast is applied when necessary.
  template<typename T> struct Identity
  {
    typedef typename T output_type;

    template<typename TIN>
    output_type operator()( TIN in )
    { return in; }
  };


  //--------------------------------------------------------------------------
  //! Weak reference adapter: extract pointer from weak reference.
  template<typename T> struct WeakRef2Ptr
  {
    typedef typename T* output_type;

    template<typename TIN>
    output_type operator()( const WeakRefCradle<TIN>& in )
    { return in.ptr(); }
  };


  //--------------------------------------------------------------------------
  //! Static type cast adapter: explicit static cast from one type to another.
  template<typename T> struct StaticCast
  {
    typedef typename T output_type;

    template<typename TIN>
    output_type operator()( TIN in )
    { return static_cast<T>(in); }
  };


  //--------------------------------------------------------------------------
  //! Dynamic type cast adapter: dynamic cast from one type to another.
  template<typename T> struct DynamicCast
  {
    typedef typename T output_type;

    template<typename TIN>
    output_type operator()( TIN in )
    { return dynamic_cast<T>(in); }
  };


  //--------------------------------------------------------------------------
  //! Pair first element adapter: extract first member of transmitted pair.
  //! An implicit cast is applied when necessary.
  template<typename T> struct ExtractFirst
  {
    typedef typename T output_type;

    template<typename TIN>
    output_type operator()( TIN in )
    { return in.first; }
  };


  //--------------------------------------------------------------------------
  //! Pair first element adapter: extract second member of transmitted pair.
  //! An implicit cast is applied when necessary.
  template<typename T> struct ExtractSecond
  {
    typedef typename T output_type;

    template<typename TIN>
    output_type operator()( TIN in )
    { return in.second; }
  };


  //--------------------------------------------------------------------------
  //! Composition adapter: compose two functors.
  template<class F1, class F2> struct Compose
  {
    typedef typename F1::output_type output_type;

    template<typename TIN>
    output_type operator()( TIN in )
    { F1 f1; F2 f2; return f1( f2(in) ); }
  };


  //--------------------------------------------------------------------------
  //! Provide a misc::Enumerator implementation for STL containers.
  template< class ITERATOR, class ADAPTER=Identity<ITERATOR::value_type> >
  class ContainerEnumerator :
    public DllImpl< Enumerator<typename ADAPTER::output_type> >
  {
  public:
    typedef typename Enumerator<typename ADAPTER::output_type>::output_type
      output_type;

    //! Construction: enumerate items between transmitted iterators.
    ContainerEnumerator(
      ITERATOR first, ITERATOR last, const ADAPTER& a=ADAPTER() ) :
    _it( first ), _last( last ), _adapter( a ) {}

    //! Return current element.
    output_type MISC_CALL item( void )
    { return (_it!=_last) ? _adapter(*_it) : 0; }

    //! Move to next element.
    void MISC_CALL moveNext( void )
    { if(_it!=_last) ++_it; }

  private:
    ITERATOR _it;
    ITERATOR _last;
    ADAPTER  _adapter;
  };


  //--------------------------------------------------------------------------
  //! Return a newly constructed ContainerEnumerator.
  //! Allows automatic types deduction.
  template<class IT, class AD>
  ContainerEnumerator<IT,AD>* newContainerEnumerator( IT first,IT last,AD ad )
  { return new ContainerEnumerator<IT,AD>( first, last, ad ); }

} // namespace misc

#endif  // #ifndef _ENUMERATOR_IMPL_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
