//----------------------------------------------------------------------------
// libasphalt (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file vecop.h
//! \brief Helper classes for writting generic algorithms on vectors

#ifndef _ASPHALT_VECOP_H_
#define _ASPHALT_VECOP_H_

#include <boost/utility/enable_if.hpp>
#include <asphalt/math.h>

namespace asphalt {

  template<typename T> struct traits;

  template< typename OpTraits,
            typename VT1, typename TR1,
            typename VT2, typename TR2,
            typename Enable = boost::enable_if_c<TR1::size==TR2::size> >
  struct vecop_binary_op;

  template< typename OpTraits, typename ParamType,
            typename VT, typename TR >
  struct vecop_custom_op;

  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            typename Enable = boost::enable_if_c<TR1::size==TR2::size> >
  struct vecop_sum_traits;

  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            typename Enable = boost::enable_if_c<TR1::size==TR2::size> >
  struct vecop_diff_traits;

  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            typename Enable = boost::enable_if_c<TR1::size==TR2::size> >
  struct vecop_cross_traits;

  template< typename VT, typename TR >
  struct vecop_factor_traits;

  template< typename VT, typename TR >
  struct vecop_negate_traits;


  //--------------------------------------------------------------------------
  //! Wrapper class for performing vector operations whatever the layout of
  //! actual data.
  //! Specializing asphalt::traits<T> for user's particular type will make it
  //! usable as a vector operand.
  template < typename VectorType,
             typename Traits=traits<VectorType> >
  class vecop
  {
  public:
    typedef typename vecop<VectorType,Traits> self_type;
    typedef typename VectorType               vector_type;
    typedef typename Traits::scalar_type      scalar_type;

    //------------------------------------------------------------------------
    //! @name Construction
    //@{
    //! Wrap transmitted data as a vector.
    inline vecop( VectorType& data ) : _data(&data) {}

    //! Copy constructor
    inline vecop( const self_type& other ) : _data(other._data){}
    //@}

    //------------------------------------------------------------------------
    //! @name Data access
    //@{
    //! Return the value of the item at index transmitted as template param.
    template<unsigned int index> scalar_type item( void ) const
    { return Traits::item<index>::get( *_data ); }

    //! Return a reference to item at index transmitted as template parameter.
    template<unsigned int index> scalar_type& item( void )
    { return Traits::item<index>::get( *_data ); }
    //@}

    //------------------------------------------------------------------------
    //! @name Unary operators
    //@{

    //! Copy values of transmitted vector to wrapped data.
    template<typename VectorType2, typename Traits2>
    const self_type& operator =( const vecop<VectorType2,Traits2> &other )
    {
      vecop_copy<VectorType,Traits,VectorType2,Traits2>::compute(
        *_data, *other._data );
      return *this;
    }

    //! Copy values of the transmitted vector to wrapped data.
    inline const self_type& operator =( const self_type& other )
    {
      vecop_copy<VectorType,Traits,VectorType,Traits>::compute(
        *_data, *other._data );
      return *this;
    }

    //! Unary addition: add values of transmitted vector to wrapped data.
    template<typename VectorType2, typename Traits2>
    const self_type& operator +=( const vecop<VectorType2,Traits2> &other )
    {
      vecop_add<VectorType,Traits,VectorType2,Traits2>::compute(
        *_data, *other._data );
      return *this;
    }

    //! Unary difference: substract transmitted vector from wrapped data.
    template<typename VectorType2, typename Traits2>
    const self_type& operator -=( const vecop<VectorType2,Traits2> &other )
    {
      vecop_sub<VectorType,Traits,VectorType2,Traits2>::compute(
        *_data, *other._data );
      return *this;
    }

    //! Unary factor: multiply wrapped data by transmitted value.
    inline const self_type& operator *=( scalar_type factor )
    {
      vecop_mult<VectorType,Traits>::compute( *_data, factor );
      return *this;
    }

    //! Return vector's dot product with itself.
    inline scalar_type sqr( void ) const
    { return vecop_sqr<VectorType,Traits>::compute(*_data); }

    //! Return vector's euclidian norm.
    inline scalar_type norm2( void ) const
    { return sqrt( sqr() ); }

    //@}

    //------------------------------------------------------------------------
    //! @name Binary operators
    //@{

    //! Return the dot product of this vector with the transmitted one.
    template<typename VectorType2, typename Traits2>
    scalar_type operator *( vecop<VectorType2,Traits2> other ) const
    {
      return vecop_dot<VectorType,Traits,VectorType2,Traits2>::compute(
        *_data, *other._data );
    }

    //! Binary addition.
    //! Return a pseudo-vector summing this vector to the transmitted one.
    //! The actual sum algorithm is unravelled when affecting returned vecop
    //! to another.
    template<typename VectorType2, typename Traits2>
    const vecop_binary_op<
      vecop_sum_traits<VectorType,Traits,VectorType2,Traits2>,
      VectorType,Traits,VectorType2,Traits2 >
    operator +( const vecop<VectorType2,Traits2> &other ) const
    {
      return vecop_binary_op<
        vecop_sum_traits<VectorType,Traits,VectorType2,Traits2>,
        VectorType,Traits,VectorType2,Traits2 >( *_data, *other._data );
    }

    //! Binary substraction.
    //! Return a pseudo-vector substracting the transmitted vector from this.
    //! The actual difference algorithm is unravelled when affecting returned
    //! vecop to another.
    template<typename VectorType2, typename Traits2>
    const vecop_binary_op<
      vecop_diff_traits<VectorType,Traits,VectorType2,Traits2>,
      VectorType,Traits,VectorType2,Traits2 >
    operator -( const vecop<VectorType2,Traits2> &other ) const
    {
      return vecop_binary_op<
        vecop_diff_traits<VectorType,Traits,VectorType2,Traits2>,
        VectorType,Traits,VectorType2,Traits2 >( *_data, *other._data );
    }

    //! Cross product.
    //! Return a pseudo-vector computing the cross product of this vector with
    //! the transmitted one. The actual algorithm is unravelled when affecting
    //! returned vecop to another.
    template<typename VectorType2, typename Traits2>
    const vecop_binary_op<
      vecop_cross_traits<VectorType,Traits,VectorType2,Traits2>,
      VectorType,Traits,VectorType2,Traits2 >
    operator ^( const vecop<VectorType2,Traits2> &other ) const
    {
      return vecop_binary_op<
        vecop_cross_traits<VectorType,Traits,VectorType2,Traits2>,
        VectorType,Traits,VectorType2,Traits2 >( *_data, *other._data );
    }

    //! Scalar product.
    const vecop_custom_op< vecop_factor_traits<VectorType,Traits>,
                           scalar_type, VectorType, Traits >
    operator *( scalar_type factor ) const
    {
      return vecop_custom_op<
        vecop_factor_traits<VectorType,Traits>, scalar_type,
        VectorType, Traits >
        ( *_data, factor );
    }
    //@}

  protected:
    //! Let vecop access members of other vecop instances.
    template<typename VT2, typename TR2> friend class vecop;

    //! Neutral constructor to be called by special implementations only.
    vecop( void ) : _data(0) {}

    //! Pointer to wrapped data.
    VectorType *_data;
  };


  //--------------------------------------------------------------------------
  //! Default type traits used by vecop.
  //! Must be specialized for user's own type.
  template<typename T>
  struct traits
  {
    typedef typename T::scalar_type scalar_type;

    enum { size=error_undefined_size };

    template<unsigned int index> struct item
    {
      static inline scalar_type  get( const T& vec ) { error_out_of_bounds; }
      static inline scalar_type& get( T& vec )       { error_out_of_bounds; }
    };
  };


  //--------------------------------------------------------------------------
  // Automatic type traits for constant data from non-constant type traits
  template<typename T>
  struct traits<const T>
  {
    typedef typename traits<T>::scalar_type scalar_type;

    enum { size=traits<T>::size };

    template<unsigned int index> struct item
    {
      static inline scalar_type get( const T &vec )
      { return traits<T>::item<index>::get(vec); }

      static inline scalar_type& get( T& vec )
      { error_write_forbidden; }
    };
  };


  //--------------------------------------------------------------------------
  //! Type traits for pointer.
  //! This one must be used explicitely, with the size of the vector as a
  //! template argument.
  template<typename T, unsigned int SZ>
  struct pointer_traits
  {
    typedef typename T scalar_type;

    enum { size=SZ };

    template< unsigned int index,
              typename Enable=boost::enable_if_c<(index<SZ)> >
    struct item
    {
      static inline scalar_type get( const T* vec ) { return vec[index]; }
      static inline scalar_type& get( T* vec )      { return vec[index]; }
    };

    template< unsigned int index >              
    struct item< index, boost::enable_if_c<(index>=SZ)> >
    {
      static inline scalar_type get( const T* vec ) { error_out_of_bounds; }
      static inline scalar_type& get( T* vec )      { error_out_of_bounds; }
    };
  };


  //--------------------------------------------------------------------------
  // Type traits for constant pointer.
  // This one must be used explicitely, with the size of the vector as a
  // template argument.
  template<typename T, unsigned int SZ>
  struct pointer_traits<const T,SZ>
  {
    typedef typename T scalar_type;

    enum { size=SZ };

    template< unsigned int index,
              typename Enable=boost::enable_if_c<(index>=0 && index<SZ)> >
    struct item
    {
      static inline scalar_type get( const T* vec ) { return vec[index]; }
      static inline scalar_type& get( T* vec )      { error_write_forbidden; }
    };

    template< unsigned int index >              
    struct item< index, boost::enable_if_c<(index>=SZ)> >
    {
      static inline scalar_type get( const T* vec ) { error_out_of_bounds; }
      static inline scalar_type& get( T* vec )      { error_out_of_bounds; }
    };
  };


  //--------------------------------------------------------------------------
  // Type traits for C array.
  // Performs compile-time bounds checking.
  template<typename T, unsigned int SZ>
  struct traits<T[SZ]>
  {
    typedef T scalar_type;

    enum { size=SZ };

    template< unsigned int index,
              typename Enable=boost::enable_if_c<(index>=0 && index<SZ)> >
    struct item
    {
      static inline scalar_type get( const T* vec ) { return vec[index]; }
      static inline scalar_type& get( T* vec )      { return vec[index]; }
    };

    template< unsigned int index >              
    struct item< index, boost::enable_if_c<(index>=SZ)> >
    {
      static inline scalar_type get( const T* vec ) { error_out_of_bounds; }
      static inline scalar_type& get( T* vec )      { error_out_of_bounds; }
    };
  };


  //--------------------------------------------------------------------------
  // Type traits for constant C array.
  // Performs compile-time bounds checking.
  template<typename T, unsigned int SZ>
  struct traits<const T[SZ]>
  {
    typedef T scalar_type;

    enum { size=SZ };

    template< unsigned int index,
              typename Enable=boost::enable_if_c<(index>=0 && index<SZ)> >
    struct item
    {
      static inline scalar_type get( const T* vec ) { return vec[index]; }
      static inline scalar_type& get( T* vec )      { return vec[index]; }
    };
  };


  //--------------------------------------------------------------------------
  // Type traits for accessing members x and y.
  template<typename ObjType, typename ScalarType>
  struct xy_traits
  {
    typedef typename ScalarType scalar_type;

    enum { size = 2 };

    template<unsigned int index> struct item
    {
      static inline scalar_type get( const ObjType& vec )
      { error_out_of_bounds; }

      static inline scalar_type& get( ObjType& vec )
      { error_out_of_bounds; }
    };

    template<> struct item<0>
    {
      static inline scalar_type  get( const ObjType& vec ) { return vec.x; }
      static inline scalar_type& get( ObjType& vec)        { return vec.x; }
    };

    template<> struct item<1>
    {
      static inline scalar_type  get( const ObjType& vec ) { return vec.y; }
      static inline scalar_type& get( ObjType& vec)        { return vec.y; }
    };
  };


  //--------------------------------------------------------------------------
  //! Provide a vecop for transmitted argument with automatic deduction of
  //! template type argument.
  template<typename VectorType>
  vecop<VectorType> mk_vecop( VectorType& data )
  { return vecop<VectorType>(data); }

  template<unsigned int size, typename T>
  vecop<T*,pointer_traits<T,size>> mk_vecop( T* &data )
  { return vecop<T*,pointer_traits<T,size>>(data); }


  //--------------------------------------------------------------------------
  //! Unary vecop copy
  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            unsigned int SZ=TR1::size,
            unsigned int Index=0 >
  struct vecop_copy {
    static inline void compute( VT1 &target, const VT2 &source )
    {
      TR1::item<Index>::get(target) = TR2::item<Index>::get(source);
      vecop_copy<VT1,TR1,VT2,TR2,SZ,Index+1>::compute(target,source);
    }
  };

  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            unsigned int SZ >
  struct vecop_copy<VT1,TR1,VT2,TR2,SZ,SZ> {
    static inline void compute( VT1 &target, const VT2 &source ) {}
  };


  //--------------------------------------------------------------------------
  //! Unary vecop addition
  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            unsigned int SZ=TR1::size,
            unsigned int Index=0 >
  struct vecop_add {
    static inline void compute( VT1 &target, const VT2 &source )
    {
      TR1::item<Index>::get(target) += TR2::item<Index>::get(source);
      vecop_add<VT1,TR1,VT2,TR2,SZ,Index+1>::compute( target, source );
    }
  };

  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            unsigned int SZ >
  struct vecop_add<VT1,TR1,VT2,TR2,SZ,SZ> {
    static inline void compute( VT1 &target, const VT2 &source ) {}
  };


  //--------------------------------------------------------------------------
  //! Unary vecop substraction
  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            unsigned int SZ=TR1::size,
            unsigned int Index=0 >
  struct vecop_sub {
    static inline void compute( VT1 &target, const VT2 &source )
    {
      TR1::item<Index>::get(target) -= TR2::item<Index>::get(source);
      vecop_sub<VT1,TR1,VT2,TR2,SZ,Index+1>::compute( target, source );
    }
  };

  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            unsigned int SZ >
  struct vecop_sub<VT1,TR1,VT2,TR2,SZ,SZ> {
    static inline void compute( VT1 &target, const VT2 &source ) {}
  };


  //--------------------------------------------------------------------------
  //! Unary vecop multiplication
  template< typename VT, typename TR,
            unsigned int SZ=TR::size, unsigned int Index=0 >
  struct vecop_mult {
    typedef typename TR::scalar_type scalar_type;
    static inline void compute( VT &target, scalar_type factor )
    {
      TR::item<Index>::get(target) *= factor;
      vecop_mult<VT,TR,SZ,Index+1>::compute( target, factor );
    }
  };

  template< typename VT, typename TR, unsigned int SZ >
  struct vecop_mult<VT,TR,SZ,SZ> {
    typedef typename TR::scalar_type scalar_type;
    static inline void compute( VT &target, scalar_type factor ) {}
  };


  //--------------------------------------------------------------------------
  //! Vecop square
  template< typename VT, typename TR,
            unsigned int SZ=TR::size, unsigned int Index=0 >
  struct vecop_sqr {
    typedef typename TR::scalar_type return_type;

    static inline return_type compute( const VT &x )
    {
      return sqr(TR::item<Index>::get(x)) +
             vecop_sqr<VT,TR,SZ,Index+1>::compute(x);
    }
  };

  template< typename VT, typename TR, unsigned int SZ >
  struct vecop_sqr<VT,TR,SZ,SZ> {
    typedef typename TR::scalar_type return_type;

    static inline return_type compute( const VT &x )
    { return return_type(0); }
  };


  //--------------------------------------------------------------------------
  //! Vecop dot product
  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            unsigned int SZ=TR1::size,
            unsigned int Index=0 >
  struct vecop_dot {
    typedef typename TR1::scalar_type return_type;

    static inline return_type compute( const VT1 &a, const VT2 &b )
    {
      return TR1::item<Index>::get(a) * TR2::item<Index>::get(b) +
        vecop_dot<VT1,TR1,VT2,TR2,SZ,Index+1>::compute( a, b );
    }
  };

  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            unsigned int SZ >
  struct vecop_dot<VT1,TR1,VT2,TR2,SZ,SZ> {
    typedef typename TR1::scalar_type return_type;

    static inline return_type compute( const VT1 &a, const VT2 &b )
    { return return_type(0); }
  };


  //--------------------------------------------------------------------------
  //! Binary vecop operation.
  //! Allow a binary operation to be performed when accessing items.
  template< typename OpTraits,
            typename VT1, typename TR1,
            typename VT2, typename TR2,
            typename Enable >
  struct vecop_binary_op :
    public vecop< vecop_binary_op<OpTraits,VT1,TR1,VT2,TR2>, OpTraits >
  {
  public:
    typedef typename vecop_binary_op<OpTraits,VT1,TR1,VT2,TR2> self_type;

    vecop_binary_op( const VT1 &a, const VT2 &b ) :
      vecop< vecop_binary_op<OpTraits,VT1,TR1,VT2,TR2>, OpTraits >(),
      v1(&a), v2(&b)
    { _data=this; }

    vecop_binary_op( const self_type& o ) :
      vecop< vecop_binary_op<OpTraits,VT1,TR1,VT2,TR2>, OpTraits >(),
      v1(o.v1), v2(o.v2)
    { _data=this; }

    const VT1 *v1;
    const VT2 *v2;
  };


  //--------------------------------------------------------------------------
  //! vecop addition traits.
  //! Compute the sum of two vectors when accessing items.
  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            typename Enable >
  struct vecop_sum_traits
  {
    typedef typename vecop_sum_traits<VT1,TR1,VT2,TR2>          self_type;
    typedef typename TR1::scalar_type                           scalar_type;
    typedef typename vecop_binary_op<self_type,VT1,TR1,VT2,TR2> vecop_type;

    enum { size=TR1::size };

    template<unsigned int index> struct item {
      static inline scalar_type get( const vecop_type &vec )
      {
        return TR1::item<index>::get(*vec.v1) +
               TR2::item<index>::get(*vec.v2);
      }
    };
  };


  //--------------------------------------------------------------------------
  //! vecop difference traits.
  //! Compute the difference between two vectors when accessing items.
  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            typename Enable >
  struct vecop_diff_traits
  {
    typedef typename vecop_diff_traits<VT1,TR1,VT2,TR2>         self_type;
    typedef typename TR1::scalar_type                           scalar_type;
    typedef typename vecop_binary_op<self_type,VT1,TR1,VT2,TR2> vecop_type;

    enum { size=TR1::size };

    template<unsigned int index> struct item {
      static inline scalar_type get( const vecop_type &vec )
      {
        return TR1::item<index>::get(*vec.v1) -
               TR2::item<index>::get(*vec.v2);
      }
    };
  };


  //--------------------------------------------------------------------------
  //! vecop cross product traits.
  //! Compute the cross product of two vectors when accessing items.
  template< typename VT1, typename TR1,
            typename VT2, typename TR2,
            typename Enable >
  struct vecop_cross_traits
  {
    typedef typename vecop_cross_traits<VT1,TR1,VT2,TR2>        self_type;
    typedef typename TR1::scalar_type                           scalar_type;
    typedef typename vecop_binary_op<self_type,VT1,TR1,VT2,TR2> vecop_type;

    enum { size=TR1::size };

    template<unsigned int index> struct item {
      static inline scalar_type get( const vecop_type &vec )
      {
        return 
          TR1::item<(index+TR1::size-2)%TR1::size>::get(*vec.v1)
          * TR2::item<(index+TR1::size-1)%TR1::size>::get(*vec.v2) -
          TR1::item<(index+TR1::size-1)%TR1::size>::get(*vec.v1)
          * TR2::item<(index+TR1::size-2)%TR1::size>::get(*vec.v2);
      }
    };
  };


  //--------------------------------------------------------------------------
  //! Custom vecop operation.
  //! Allow some operation to be performed when accessing items.
  template< typename OpTraits, typename ParamType,
            typename VT, typename TR >
  struct vecop_custom_op :
    public vecop< vecop_custom_op<OpTraits,ParamType,VT,TR>, OpTraits >
  {
  public:
    typedef typename vecop_custom_op<OpTraits,ParamType,VT,TR> self_type;

    vecop_custom_op( const VT &nData, ParamType nParam ) :
      vecop< vecop_custom_op<OpTraits,ParamType,VT,TR>, OpTraits >(),
      data( &nData ), param( nParam )
    { _data=this; }

    vecop_custom_op( const self_type& o ) :
      vecop< vecop_custom_op<OpTraits,ParamType,VT,TR>, OpTraits >(),
      data(o.data), param(o.param)
    { _data=this; }

    const VT *data;
    ParamType param;
  };


  //--------------------------------------------------------------------------
  //! Custom traits negating vector content on access
  template< typename VT, typename TR >
  struct vecop_negate_traits
  {
    typedef typename TR::scalar_type scalar_type;

    enum { size=TR1::size };

    template<unsigned int index> struct item {
      static inline scalar_type get( const VT& vec )
      { return -TR::item<index>::get(vec); }

      static inline scalar_type& get( VT& vec )
      { error_write_forbidden; }
    };
  };


  //--------------------------------------------------------------------------
  //! Custom traits applying a factor to vector content on access
  template< typename VT, typename TR >
  struct vecop_factor_traits
  {
    typedef typename vecop_factor_traits<VT,TR> self_type;
    typedef typename TR::scalar_type            scalar_type;

    enum { size=TR::size };

    template<unsigned int index> struct item {
      static inline scalar_type get(
        const vecop_custom_op<self_type,scalar_type,VT,TR>& vec )
      { return TR::item<index>::get(*vec.data) * vec.param; }

      static inline scalar_type& get(
        vecop_custom_op<self_type,scalar_type,VT,TR>& vec )
      { error_write_forbidden; }
    };
  };

  template< typename VT, typename TR >
  vecop_custom_op< vecop_factor_traits<VT,TR>,
                   typename vecop<VT,TR>::scalar_type,
                   VT, TR >
  operator *( typename vecop<VT,TR>::scalar_type a,
              const vecop<VT,TR> &b )
  { return b*a; }

} // namespace asphalt

#endif  // #ifndef _ASPHALT_VECOP_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
