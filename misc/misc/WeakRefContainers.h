//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file WeakRefContainers.h
//! \brief Weak reference container classes definition.

#ifndef _WEAK_REF_CONTAINERS_H_
#define _WEAK_REF_CONTAINERS_H_

#include <misc/WeakRef.h>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <algorithm>

namespace misc
{
  //--------------------------------------------------------------------------
  //| WeakRefList template class
  //!  The WeakRefList template class can be used to manage a list of weak
  //!  references to objects inheriting from WeakRefServer.
  //!  References are removed from the list as soon as pointed object is
  //!  deleted (eg references are not just set to 0, they're removed from the
  //!  container).
  //!  Template argument T is the type of pointed objects and must inherit
  //!  WeakRefServer.
  //!  The resulting container type may mostly be used as an actual instance
  //!  of std::list<T*> that will only contain non-null valid pointers as long
  //!  as the user only fills it with valid non-null pointers.
  //!  Method ptr() has to be called on items to get the actual pointer, while
  //!  operator -> on items just works as one would expect.
  //!  WARNING: This class is not intended to be used in an ABI.
  //!           Using this class in an API will bind the users of your API to
  //!           use the same STL implementation as you.
  template <class T> class WeakRefList :
  public IWeakRef,
  protected std::list< WeakRefCradle<T> >
  {
  public:
    typedef typename std::list< WeakRefCradle<T> >::allocator_type         allocator_type;
    typedef typename std::list< WeakRefCradle<T> >::const_iterator         const_iterator;
    typedef typename std::list< WeakRefCradle<T> >::const_reference        const_reference;
    typedef typename std::list< WeakRefCradle<T> >::const_reverse_iterator const_reverse_iterator;
    typedef typename std::list< WeakRefCradle<T> >::iterator               iterator;
    typedef typename std::list< WeakRefCradle<T> >::reference              reference;
    typedef typename std::list< WeakRefCradle<T> >::reverse_iterator       reverse_iterator;
    typedef typename std::list< WeakRefCradle<T> >::size_type              size_type;
    typedef typename std::list< WeakRefCradle<T> >::value_type             value_type;



    void releaseAddr( WeakRefServer *ptr )
    { remove( WeakRefCradle<T>(ptr) ); }

    template<typename IT> inline void assign( IT first, IT last )
    { clear(); while( first!=last ) push_back( *(first++) ); }

    inline void assign( size_type n, const value_type &val )
    { clear(); while( n-- ) push_back( val ); }

    inline const_reference back( void ) const { return std::list< WeakRefCradle<T> >::back(); }

    inline reference back( void ) { return std::list< WeakRefCradle<T> >::back(); }

    inline const_iterator begin( void ) const { return std::list< WeakRefCradle<T> >::begin(); }

    inline iterator begin( void ) { return std::list< WeakRefCradle<T> >::begin(); }

    inline void clear( void ) { return std::list< WeakRefCradle<T> >::clear(); }

    bool empty( void ) const { return std::list< WeakRefCradle<T> >::empty(); }

    const_iterator end( void ) const { return std::list< WeakRefCradle<T> >::end(); }

    iterator end( void ) { return std::list< WeakRefCradle<T> >::end(); }

    iterator erase( iterator first, iterator last )
    { return std::list< WeakRefCradle<T> >::erase(first,last); }

    iterator erase( iterator pos ) { return std::list< WeakRefCradle<T> >::erase(pos); }

    const_reference front( void ) const { return std::list< WeakRefCradle<T> >::front(); }

    reference front( void ) { return std::list< WeakRefCradle<T> >::front(); }

    allocator_type get_allocator( void ) const
    { return std::list< WeakRefCradle<T> >::get_allocator(); }

    template <typename IT> inline void insert( iterator pos, IT first, IT last )
    { while( first!=last ) insert( pos++, *(first++) ); }

    inline void insert( iterator pos, size_type n, const value_type &x )
    { while( n ) { insert( pos, x ); n--;} }

    inline void insert( iterator pos, const value_type &x )
    { std::list< WeakRefCradle<T> >::insert( pos, WeakRefCradle<T>(x.ptr(),this) ); }

    inline void insert( iterator pos, T *ptr )
    { std::list< WeakRefCradle<T> >::insert( pos, WeakRefCradle<T>(ptr,this) ); }

    template <typename IT> inline WeakRefList( IT first, IT last,
                                                    const allocator_type &a=allocator_type() ) :
      IWeakRef(), std::list< WeakRefCradle<T> >()
    { assign( first, last ); }

    inline WeakRefList( const WeakRefList<T>& other ) :
      IWeakRef(), std::list< WeakRefCradle<T> >()
    { assign( other.begin(), other.end() ); }

    inline WeakRefList( 
      size_type n, const value_type &value=value_type(), const allocator_type &a=allocator_type() ) :
      IWeakRef(), std::list< WeakRefCradle<T> >()
    { assign( n, value ); }

    inline WeakRefList( const allocator_type &a ) :
      IWeakRef(), std::list< WeakRefCradle<T> >() {}

    inline WeakRefList( void ) :
      IWeakRef(), std::list< WeakRefCradle<T> >() {}

    inline size_type max_size( void ) const
    { return std::list< WeakRefCradle<T> >::max_size(); }

  /*
   * template <typename _StrictWeakOrdering> void merge( list &, _StrictWeakOrdering )
   * void merge (list &__x)
   */

    inline WeakRefList& operator =( const WeakRefList& other )
    { assign( other.begin(), other.end() ); return *this; }

    inline void pop_back( void ) { std::list< WeakRefCradle<T> >::pop_back(); }

    inline void pop_front( void ) { std::list< WeakRefCradle<T> >::pop_front(); }

    inline void push_back( const value_type &x )
    { std::list< WeakRefCradle<T> >::push_back( WeakRefCradle<T>(x.ptr(),this) ); }

    inline void push_front( const value_type &x )
    { std::list< WeakRefCradle<T> >::push_front( WeakRefCradle<T>(x.ptr(),this) ); }

    inline const_reverse_iterator rbegin( void ) const
    { return std::list< WeakRefCradle<T> >::rbegin(); }

    inline reverse_iterator rbegin( void )
    { return std::list< WeakRefCradle<T> >::rbegin(); }

    inline void remove( const_reference value )
    { std::list< WeakRefCradle<T> >::remove( value ); }

    template<typename P> inline void remove_if( P predicate )
    { std::list< WeakRefCradle<T> >::remove_if( predicate ); }

    inline const_reverse_iterator rend( void ) const
    { return std::list< WeakRefCradle<T> >::rend(); }

    inline reverse_iterator rend( void )
    { return std::list< WeakRefCradle<T> >::rend(); }

    inline void resize( size_type NewSize, value_type x=value_type() )
    { std::list< WeakRefCradle<T> >::resize( NewSize, x ); }

    inline void reverse( void )
    { std::list< WeakRefCradle<T> >::reverse(); }

    inline size_type size( void ) const
    { return std::list< WeakRefCradle<T> >::size(); }

    template<typename StrictWeakOrdering> inline void sort( StrictWeakOrdering o )
    { std::list< WeakRefCradle<T> >::sort( o ); }

    inline void sort( void )
    { std::list< WeakRefCradle<T> >::sort(); }

  /*
   * void splice (iterator __position, list &__x, iterator __first, iterator __last)
   * void splice (iterator __position, list &__x, iterator __i)
   * void splice (iterator __position, list &__x)
   * void swap (list &__x)
   * template<typename _BinaryPredicate> void unique (_BinaryPredicate)
   * void unique ()
   */

  }; // template <class T> class WeakRefList

  //--------------------------------------------------------------------------
  //| WeakRefSet template class
  //!  The WeakRefSet template class can be used to manage a set of weak
  //!  references to objects inheriting from WeakRefServer.
  //!  References are removed from the set as soon as a pointed object is
  //!  deleted (eg references are not just set to 0, they're removed from the
  //!  container).
  //!  Template argument T is the type of pointed objects and must inherit
  //!  WeakRefServer.
  //!  The resulting container type may mostly be used as an actual instance
  //!  of std::set<T*> that will only contain non-null valid pointers as long
  //!  as the user only fills it with valid non-null pointers.
  //!  Method ptr() has to be called on items to get the actual pointer, while
  //!  operator -> on items just works as one would expect.
  //!  WARNING: This class is not intended to be used in an ABI.
  //!           Using this class in an API will bind the users of your API to
  //!           use the same STL implementation as you.
  template <class T> class WeakRefSet :
  public IWeakRef,
  protected std::set< WeakRefCradle<T> >
  {
  public:
    typedef typename std::set< WeakRefCradle<T> >::allocator_type         allocator_type;
    typedef typename std::set< WeakRefCradle<T> >::key_compare            key_compare;
    typedef typename std::set< WeakRefCradle<T> >::key_type               key_type;
    typedef typename std::set< WeakRefCradle<T> >::value_compare          value_compare;
    typedef typename std::set< WeakRefCradle<T> >::value_type             value_type;

    typedef typename std::set< WeakRefCradle<T> >::iterator               iterator;
    typedef typename std::set< WeakRefCradle<T> >::reverse_iterator       reverse_iterator;
    typedef typename std::set< WeakRefCradle<T> >::const_iterator         const_iterator;
    typedef typename std::set< WeakRefCradle<T> >::const_reverse_iterator const_reverse_iterator;
    typedef typename std::set< WeakRefCradle<T> >::reference              reference;
    typedef typename std::set< WeakRefCradle<T> >::const_reference        const_reference;
    typedef typename std::set< WeakRefCradle<T> >::size_type              size_type;



    void releaseAddr( WeakRefServer *ptr )
    { erase( ptr ); }

    inline iterator begin( void ) const
    { return std::set< WeakRefCradle<T> >::begin(); }

    inline void clear( void )
    { std::set< WeakRefCradle<T> >::clear(); }

    inline size_type count( const key_type &k ) const
    { return std::set< WeakRefCradle<T> >::count(k); }

    inline bool empty( void ) const
    { return std::set< WeakRefCradle<T> >::empty(); }

    inline iterator end( void ) const
    { return std::set< WeakRefCradle<T> >::end(); }

    inline void erase( iterator first, iterator last )
    { std::set< WeakRefCradle<T> >::erase(first,last); }

    inline size_type erase( const key_type &k )
    { return std::set< WeakRefCradle<T> >::erase(k); }

    inline void erase( iterator position )
    { std::set< WeakRefCradle<T> >::erase( position ); }

    inline allocator_type get_allocator( void ) const
    { return std::set< WeakRefCradle<T> >::get_allocator(); }

    template<typename IT> inline void insert( IT first, IT last )
    { while( first!=last ) insert( *(first++) ); }

    inline iterator insert( iterator position, const value_type &x )
    { std::set< WeakRefCradle<T> >::insert( position, WeakRefCradle<T>(x.ptr(),this) ); }

    inline std::pair< iterator, bool > insert( const value_type &x )
    { return std::set< WeakRefCradle<T> >::insert( WeakRefCradle<T>(x.ptr(),this) ); }

    inline key_compare key_comp( void ) const
    { return std::set< WeakRefCradle<T> >::key_compare(); }

    inline size_type max_size( void ) const
    { return std::set< WeakRefCradle<T> >::max_size(); }

    inline WeakRefSet& operator =( const std::set< WeakRefCradle<T> > &x )
    { clear(); for( const_iterator it=x.begin() ; it!=x.end() ; ++it ) insert(*it); }

    inline reverse_iterator rbegin( void ) const
    { return std::set< WeakRefCradle<T> >::rbegin(); }

    inline reverse_iterator rend( void ) const
    { return std::set< WeakRefCradle<T> >::rend(); }

    inline WeakRefSet( const WeakRefSet &other ) :
    IWeakRef(), std::set< WeakRefCradle<T> >()
    { *this = other; }

    template<typename IT> inline WeakRefSet( 
      IT first, IT last, const key_compare &comp, const allocator_type &a=allocator_type() ) :
    IWeakRef(), std::set< WeakRefCradle<T> >()
    { insert( first, last ); }

    template <typename IT> inline WeakRefSet( IT first, IT last ) :
    IWeakRef(), std::set< WeakRefCradle<T> >()
    { insert( first, last ); }

    inline WeakRefSet( const key_compare &comp, const allocator_type &a=allocator_type() ) :
    IWeakRef(), std::set< WeakRefCradle<T> >() {}

    inline WeakRefSet( void ) :
    IWeakRef(), std::set< WeakRefCradle<T> >() {}

    inline size_type size( void ) const
    { return std::set< WeakRefCradle<T> >::size(); }

  /*
   * void swap (set &__x)
   */

    inline value_compare value_comp( void ) const
    { return std::set< WeakRefCradle<T> >::value_compare(); }

    inline std::pair< const_iterator, const_iterator > equal_range( const key_type &k ) const
    { return std::set< WeakRefCradle<T> >::equal_range( k ); }

    inline std::pair< iterator, iterator > equal_range( const key_type &k )
    { return std::set< WeakRefCradle<T> >::equal_range( k ); }

    inline const_iterator find( const key_type &k ) const
    { return std::set< WeakRefCradle<T> >::find(k); }

    inline iterator find( const key_type &k )
    { return std::set< WeakRefCradle<T> >::find(k); }

    inline const_iterator lower_bound( const key_type &k ) const
    { return std::set< WeakRefCradle<T> >::lower_bound(k); }

    inline iterator lower_bound( const key_type &k )
    { return std::set< WeakRefCradle<T> >::lower_bound(k); }

    inline const_iterator upper_bound( const key_type &k ) const
    { return std::set< WeakRefCradle<T> >::upper_bound(k); }

    inline iterator upper_bound( const key_type &k )
    { return std::set< WeakRefCradle<T> >::upper_bound(k); }

  }; // template <class T> class WeakRefSet

  //--------------------------------------------------------------------------
  //| WeakRefVector template class
  //!  The WeakRefVector template class can be used to manage a vector of weak
  //!  references to objects inheriting from WeakRefServer.
  //!  References are removed from the vector and the vector is shrinked as
  //!  soon as pointed object is deleted (eg references are not just set to 0,
  //!  they're removed from the container).
  //!  Template argument T is the type of pointed objects and must inherit
  //!  WeakRefServer.
  //!  The resulting container type may mostly be used as an actual instance
  //!  of std::vector<T*> that will only contain non-null valid pointers as
  //!  long as the user only fills it with non-null valid pointers.
  //!  Method ptr() has to be called on items to get the actual pointer, while
  //!  operator -> on items just works as one would expect.
  //!  WARNING: This class is not intended to be used in an ABI.
  //!           Using this class in an API will bind the users of your API to
  //!           use the same STL implementation as you.
  template <class T> class WeakRefVector :
  public IWeakRef,
  protected std::vector< WeakRefCradle<T> >
  {
  public:
    typedef typename std::vector< WeakRefCradle<T> >::value_type             value_type;
    typedef typename std::vector< WeakRefCradle<T> >::pointer                pointer;
    typedef typename std::vector< WeakRefCradle<T> >::reference              reference;
    typedef typename std::vector< WeakRefCradle<T> >::const_reference        const_reference;
    typedef typename std::vector< WeakRefCradle<T> >::size_type              size_type;
    typedef typename std::vector< WeakRefCradle<T> >::difference_type        difference_type;

    typedef typename std::vector< WeakRefCradle<T> >::iterator               iterator;
    typedef typename std::vector< WeakRefCradle<T> >::const_iterator         const_iterator;
    typedef typename std::vector< WeakRefCradle<T> >::reverse_iterator       reverse_iterator;
    typedef typename std::vector< WeakRefCradle<T> >::const_reverse_iterator const_reverse_iterator;
    
    
    
    void releaseAddr( WeakRefServer *ptr )
    {
      iterator it=begin();
      while( it!=end() ) {
        if( *it==WeakRefCradle<T>(ptr) )
          it=erase(it);
        else
          ++it;
      }
    }

    inline iterator begin( void )
    { return std::vector< WeakRefCradle<T> >::begin(); }

    inline iterator end( void )
    { return std::vector< WeakRefCradle<T> >::end(); }

    inline const_iterator begin( void ) const
    { return std::vector< WeakRefCradle<T> >::begin(); }

    inline const_iterator end( void ) const
    { return std::vector< WeakRefCradle<T> >::end(); }

    inline reverse_iterator rbegin( void )
    { return std::vector< WeakRefCradle<T> >::rbegin(); }

    inline reverse_iterator rend( void )
    { return std::vector< WeakRefCradle<T> >::rend(); }

    inline const_reverse_iterator rbegin( void ) const
    { return std::vector< WeakRefCradle<T> >::rbegin(); }

    inline const_reverse_iterator rend( void ) const
    { return std::vector< WeakRefCradle<T> >::rend(); }

    inline size_type size( void ) const
    { return std::vector< WeakRefCradle<T> >::size(); }

    inline size_type max_size( void ) const
    { return std::vector< WeakRefCradle<T> >::max_size(); }

    inline size_type capacity( void ) const
    { return std::vector< WeakRefCradle<T> >::capacity(); }

    inline bool empty( void ) const
    { return std::vector< WeakRefCradle<T> >::empty(); }

    inline reference operator[]( size_type pos )
    { return std::vector< WeakRefCradle<T> >::operator[](pos); }

    inline const_reference operator[]( size_type pos ) const
    { return std::vector< WeakRefCradle<T> >::operator[](pos); }

    inline WeakRefVector( void ) : std::vector< WeakRefCradle<T> >::vector() {}

    inline WeakRefVector( const WeakRefVector<T> &other ) :
    std::vector< WeakRefCradle<T> >::vector()
    {
      reserve( other.size() );
      for( const_iterator it=other.begin() ; it!=other.end() ; ++it )
        push_back( *it );
    }

    template <class InputIterator> WeakRefVector( InputIterator f,
                                                  InputIterator l ) :
    std::vector< WeakRefCradle<T> >::vector()
    { while( f!=l ) push_back( *(f++) ); }
    
    inline ~WeakRefVector() {}

    inline const WeakRefVector<T>& operator =( const WeakRefVector<T> &other )
    {
      clear(); reserve(other.size()); 
      for( const_iterator it=other.begin() ; it!=other.end() ; ++it )
        push_back( it->ptr() );
      return *this;
    }

    inline void reserve( size_type sz )
    { std::vector< WeakRefCradle<T> >::reserve(sz); }

    inline reference front( void )
    { return std::vector< WeakRefCradle<T> >::front(); }

    inline const_reference front( void ) const
    { return std::vector< WeakRefCradle<T> >::front(); }

    inline reference back( void )
    { return std::vector< WeakRefCradle<T> >::back(); }

    inline const_reference back( void ) const 
    { return std::vector< WeakRefCradle<T> >::back(); }

    inline void push_back( const value_type& v )
    {
      std::vector< WeakRefCradle<T> >::push_back(
        WeakRefCradle<T>(v.ptr(),this) );
    }

    inline void pop_back( void )
    { std::vector< WeakRefCradle<T> >::pop_back(); }

    inline void swap( void )
    { std::vector< WeakRefCradle<T> >::swap(); }

    inline iterator insert( iterator pos, const value_type& v )
    {
      return std::vector< WeakRefCradle<T> >::insert(
        pos, WeakRefCradle<T>(v.ptr(),this) );
    }

    template<typename IT> void insert( iterator pos, IT first, IT last)
    { while(first!=last) insert( pos++, *(first++) ); }

    inline void insert( iterator pos, size_type n, const value_type& v )
    { reserve(size()+n); while( n-- ) insert( pos++, v ); }

    inline iterator erase( iterator pos )
    { return std::vector< WeakRefCradle<T> >::erase(pos); }

    inline iterator erase( iterator first, iterator last )
    { iterator res=first; while( res!=last ) res=erase(res); }

    inline void clear( void )
    { std::vector< WeakRefCradle<T> >::clear(); }

    inline bool operator ==( const WeakRefVector<T>& other )
    { return std::vector< WeakRefCradle<T> >::operator ==(other); }

    inline bool operator <( const WeakRefVector<T>& other )
    { return std::vector< WeakRefCradle<T> >::operator <(other); }

  }; // template <class T> class WeakRefVector

  //--------------------------------------------------------------------------
  //| Map2WeakRef template class
  //!  The Map2WeakRef template class can be used to manage a map where values
  //!  are weak references to objects inheriting from WeakRefServer.
  //!  Pairs are removed from the map and the map is shrinked as soon as a
  //!  referenced object is deleted (eg references are not just set to 0,
  //!  they're removed from the container).
  //!  Template argument KEY is the type of key. DATA is the type of pointed
  //!  objects and must inherit WeakRefServer.
  //!  The resulting container type may mostly be used as an actual instance
  //!  of std::map<KEY,DATA*> that will only contain pairs with non-null valid
  //!  pointers for the second element of every pairs as long as the user only
  //!  fills it with non-null valid pointers.
  //!  Method ptr() has to be called on items' second element to get the
  //!  actual pointer, while operator -> just works as one would expect.
  //!  WARNING: This class is not intended to be used in an ABI.
  //!           Using this class in an API will bind the users of your API to
  //!           use the same STL implementation as you.
  template <typename KEY, class DATA, typename COMPARE=std::less<KEY> >
  class Map2WeakRef :
  public IWeakRef,
  protected std::map< KEY, WeakRefCradle<DATA>, COMPARE >
  {
  public:
    typedef typename Map2WeakRef<KEY,DATA,COMPARE>                _self;
    typedef typename ::std::map<KEY,WeakRefCradle<DATA>,COMPARE>  _parent;
    typedef typename _parent::key_type                key_type;
    typedef typename _parent::mapped_type             mapped_type;
    typedef typename _parent::value_type              value_type;
    typedef typename _parent::key_compare             key_compare;
    typedef typename _parent::value_compare           value_compare;
    typedef typename _parent::pointer                 pointer;
    typedef typename _parent::reference               reference;
    typedef typename _parent::const_reference         const_reference;
    typedef typename _parent::size_type               size_type;
    typedef typename _parent::difference_type         difference_type;
    typedef typename _parent::iterator                iterator;
    typedef typename _parent::const_iterator          const_iterator;
    typedef typename _parent::reverse_iterator        reverse_iterator;
    typedef typename _parent::const_reverse_iterator  const_reverse_iterator;



    void releaseAddr( WeakRefServer *addr )
    {
      mapped_type toDelete = mapped_type(addr);
      iterator it=begin();
      while( it!=end() ) {
        iterator cur = it++;
        if( cur->second == toDelete )
          erase(cur);
      }
    }

    inline iterator begin( void )
    { return _parent::begin(); }

    inline iterator end( void )
    { return _parent::end(); }

    inline const_iterator begin( void ) const
    { return _parent::begin(); }

    inline const_iterator end( void ) const
    { return _parent::end(); }

    inline reverse_iterator rbegin( void )
    { return _parent::rbegin(); }

    inline reverse_iterator rend( void )
    { return _parent::rend(); }

    inline const_reverse_iterator rbegin( void ) const
    { return _parent::rbegin(); }

    inline const_reverse_iterator rend( void ) const
    { return _parent::rend(); }

    inline size_type size( void ) const
    { return _parent::size(); }

    inline size_type max_size( void ) const
    { return _parent::max_size(); }

    inline bool empty( void ) const
    { return _parent::empty(); }

    inline key_compare key_comp( void ) const
    { return _parent::key_comp(); }

    inline value_compare value_comp( void ) const
    { return _parent::value_comp(); }

    inline Map2WeakRef( void ):
    IWeakRef(), _parent::map() {}

    inline Map2WeakRef( const key_compare& comp ):
    IWeakRef(), _parent::map(comp) {}

    template <class InputIterator>
    Map2WeakRef( InputIterator first, InputIterator last ):
    IWeakRef(), _parent::map()
    { while( first!=last ) insert( end(), (*first)++ ); }

    template <class InputIterator>
    Map2WeakRef(
      InputIterator first, InputIterator last, const key_compare &comp ):
    IWeakRef(), _parent::map(comp)
    { insert( first, last ); }

    inline Map2WeakRef( const _self& other ):
    IWeakRef(), _parent::map()
    { this->operator=( other ); }

    inline _self& operator=( const _self& other )
    {
      clear();
      insert( other.begin(), other.end() );
    }

    //inline void swap( Map2WeakRef<KEY,DATA,COMPARE>& other );

    inline ::std::pair<iterator,bool> insert( const value_type& x )
    {
      return _parent::insert(
        value_type( x.first, mapped_type(x.second.ptr(),this) )  );
    }

    inline iterator insert( iterator pos, const value_type& x )
    {
      return _parent::insert(
        pos, value_type( x.first, mapped_type(x.second.ptr(),this) )  );
    }

    template <class InputIterator>
    void insert( InputIterator first, InputIterator last )
    { while( first!=last ) insert( end(), (*first)++ ); }

    inline void erase( iterator pos )
    { _parent::erase(pos); }

    inline size_type erase( const key_type& k )
    { return _parent::erase(k); }

    inline void erase( iterator first, iterator last )
    { _parent::erase(first,last); }

    inline void clear( void )
    { _parent::clear(); }

    inline iterator find( const key_type& k )
    { return _parent::find(k); }

    inline const_iterator find( const key_type& k ) const
    { return _parent::find(k); }

    inline size_type count( const key_type& k )
    { return _parent::count(k); }

    inline iterator lower_bound( const key_type& k )
    { return _parent::lower_bound(k); }

    inline const_iterator lower_bound( const key_type& k ) const
    { return _parent::lower_bound(k); }

    inline iterator upper_bound( const key_type& k )
    { return _parent::upper_bound(k); }

    inline const_iterator upper_bound( const key_type& k ) const
    { return _parent::upper_bound(k); }

    inline ::std::pair<iterator,iterator> equal_range( const key_type& k )
    { return _parent::equal_range(k); }

    inline ::std::pair<const_iterator,const_iterator> equal_range(
      const key_type& k ) const
    { return _parent::equal_range(k); }

    inline mapped_type& operator[]( const key_type& k )
    {
      iterator it=find(k);
      if( it==end() ) {
        ::std::pair<iterator,bool> res = 
          insert( value_type( k, mapped_type(0,this) ) );
        it = res.first;
      }
      return it->second;
    }

    inline bool operator==( const _self& other )
    { return (_parent&)(*this) == (_parent&)(other); }

    inline bool operator<( const _self& other )
    { return (_parent&)(*this) < (_parent&)(other); }

  }; // template <...> class Map2WeakRef

} // namespace misc

#endif // #ifndef _MG_ADDR_CLIENT_CONTAINERS_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
