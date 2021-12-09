//----------------------------------------------------------------------------
// libodr (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file WeakRef.h
//! \brief misc::WeakRef class definition.

#ifndef _WEAK_REF_H_
#define _WEAK_REF_H_

namespace misc
{
  //--------------------------------------------------------------------------
  // The existence of the class WeakRefServer has to be defined here so as to
  // be able to use pointers to WeakRefServer-s later on...
  class WeakRefServer;

  //--------------------------------------------------------------------------
  //| Abstract class IWeakRef
  //!  Abstract class IWeakRef is the interface a weak reference or a
  //!  collection of weak references must implement to be notified when a
  //!  pointed object is destructed.
  class IWeakRef
  {
  public:
    //! Method called when a pointed object is deleted.
    virtual void releaseAddr( WeakRefServer* ) = 0;
  };

  //--------------------------------------------------------------------------
  //| WeakRefCradle template class
  //!  The WeakRefCradle template class is used to embed one or more pointers
  //!  to objects of type WeakRefServer in an object implementing IWeakRef.
  //!  Its sole purpose is to maintain owner object's subscription to pointed
  //!  objects' deletion.
  //!  Template argument T is the pointed type inheriting WeakRefServer.
  //!  WARNING: WeakRefCradle is not an abstract class and thus can't be used
  //!           in a C++ ABI. Using WeakRefCradle in an API will require its
  //!           users to use the same C++ compiler as you.
  template <class T> class WeakRefCradle
  {
  public:
    //! Default constructor
    inline WeakRefCradle( T *addr=0, IWeakRef *owner=0 );

    //! Copy constructor
    inline WeakRefCradle( const WeakRefCradle<T>& other );

    //! Special constructor to build a cradle that can be used for comparison.
    //! This helps with searching a pointer to a base class in a collection
    //! after destroying the T part of pointed object.
    explicit inline WeakRefCradle( WeakRefServer *server );

    //! Destructor
    inline ~WeakRefCradle();

    //! The member selection operator makes it possible to access members of 
    //! the pointed object just the same way as it would be done with a simple
    //! pointer.
    inline T* operator -> () const;

    //! Assign a normal address to the address cradle.
    inline const WeakRefCradle<T>& operator =( T* addr );

    //! Assign the address stored in another address cradle to the address 
    //! cradle.
    inline const WeakRefCradle<T>& operator =( const WeakRefCradle<T>& other );

    //! Retrieve pointer
    inline T* ptr( void ) const;

    //! Return true if pointer is not null
    inline operator bool () const;

    //! Equality operator
    inline bool operator ==( const WeakRefCradle<T>& other ) const;

    //! Comparison operator
    inline bool operator <( const WeakRefCradle<T>& other ) const;

  private:
    T                   *_addr;   //!< Actual pointer to object
    const WeakRefServer *_server; //!< Pointer to object as a WeakRefServer
    IWeakRef            *_owner;  //!< Pointer to object owning this cradle
  };

  //--------------------------------------------------------------------------
  //| WeakRef template class
  //!  WeakRef template objects can be used in place of pointers when
  //!  manipulating the address of an object that might be deleted somewhere
  //!  else.
  //!  The class of the pointed object has to inherit from WeakRefServer.
  //!  When the pointed object is deleted, the address stored by all clients
  //!  will automatically be reset to 0, so that no pointer to an invalid
  //!  memory segment are kept. This solves issues with pointer ownership.
  //!  WARNING: WeakRef is not an abstract class and thus can't be used in a
  //!           C++ ABI. Using WeakRef in an API will require its users to
  //!           use the same C++ compiler as you.
  template <class T> class WeakRef : public IWeakRef, public WeakRefCradle<T>
  {
  public :
    //! Default constructor
    inline WeakRef( T* addr=0 );

    //! Copy constructor
    inline WeakRef( const WeakRef<T>& other );

    //! Construct from a weak reference cradle
    inline WeakRef( const WeakRefCradle<T>& other );

    //! Release server's address.
    inline void releaseAddr( WeakRefServer* addr );
  };

  //--------------------------------------------------------------------------
  template <class T> 
  WeakRefCradle<T>::WeakRefCradle( T* addr, IWeakRef *owner ) :
  _addr  ( addr ),
  _server( addr ),
  _owner ( owner )
  {
    if( _addr && _owner )
      _server->addRef( _owner );
  }

  template <class T>
  WeakRefCradle<T>::WeakRefCradle( const WeakRefCradle<T>& other ) :
  _addr  ( other._addr ),
  _server( other._server ),
  _owner ( other._owner )
  {
    if( _addr && _owner )
      _addr->addRef( _owner );
  }

  template <class T>
  WeakRefCradle<T>::WeakRefCradle( WeakRefServer *server ):
  _addr  ( _addr = dynamic_cast<T*>(server) ),
  _server( server ),
  _owner ( 0 )
  {}

  template <class T>
  WeakRefCradle<T>::~WeakRefCradle()
  {
    if( _server && _owner )
      _server->removeRef( _owner );
  }

  template <class T>
  T* WeakRefCradle<T>::operator -> () const
  {
    return _addr;
  }

  template <class T>
  const WeakRefCradle<T>& WeakRefCradle<T>::operator = ( T* addr )
  {
    if( _server && _owner )
      _server->removeRef( _owner );

    _addr   = addr;
    _server = addr;

    if( _server && _owner )
      _server->addRef( _owner );

    return static_cast<const WeakRefCradle<T>&>(*this);
  }

  template<class T>
  const WeakRefCradle<T>& 
  WeakRefCradle<T>::operator = (const WeakRefCradle<T>& other)
  {
    if( _server && _owner )
      _server->removeRef( _owner );

    _addr   = other._addr;
    _server = other._server;
    if( !_owner ) _owner=other._owner;

    if( _server && _owner )
      _server->addRef( _owner );

    return static_cast<const WeakRefCradle<T>&>(*this);
  }

  template <class T> T* WeakRefCradle<T>::ptr( void ) const
  {
    return _addr;
  }

  template <class T> WeakRefCradle<T>::operator bool () const
  {
    return _server!=0;
  }

  template <class T>
  bool WeakRefCradle<T>::operator ==( const WeakRefCradle<T>& x ) const
  {
    return _server == x._server;
  }

  template <class T>
  bool WeakRefCradle<T>::operator <( const WeakRefCradle<T>& x ) const
  {
    return _server < x._server;
  }

  template <class T>
  WeakRef<T>::WeakRef( T* addr ) :
  IWeakRef(),
  WeakRefCradle<T>()
  {
    WeakRefCradle<T>::operator = ( WeakRefCradle<T>( addr, this ) );
  }

  template <class T>
  WeakRef<T>::WeakRef( const WeakRef<T>& other ) :
  IWeakRef(),
  WeakRefCradle<T>()
  {
    WeakRefCradle<T>::operator = ( WeakRefCradle<T>( other.ptr(), this ) );
  }

  template <class T>
  WeakRef<T>::WeakRef( const WeakRefCradle<T>& other ) :
  IWeakRef(),
  WeakRefCradle<T>()
  {
    WeakRefCradle<T>::operator = ( WeakRefCradle<T>( other.ptr(), this ) );
  }

  template <class T>
  void WeakRef<T>::releaseAddr ( WeakRefServer* )
  {
    *this = 0;
  }

} // namespace misc

#endif  // #ifndef _WEAK_REF_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
