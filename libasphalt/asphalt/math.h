//----------------------------------------------------------------------------
// libasphalt (c)2008-2009 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file math.h
//! \brief Math miscellaneous functions

#ifndef _ASPHALT_MATH_H_
#define _ASPHALT_MATH_H_

#ifdef max
# undef max
#endif
#include <cmath>
#include <limits>
#include <list>


namespace asphalt
{
  template<typename T> struct math
  {
    static const T zero;
    static const T one;
    static const T two;
    static const T three;
    static const T four;
    static const T half;
    static const T quarter;
    static const T pi;
    static const T pi_by_two;
    static const T pi_by_180;
    static const T sqrt_two;
    static const T sqrt_three;
    static const T sqrt_pi_by_two;
    static const T _180;

    template<unsigned int P> static T pow( T x ) { return x*pow<P-1>(x); }
    template<> static T pow<1>( T x ) { return x; }
    template<> static T pow<0>( T x ) { return T(1); }
    inline static T sqr( T x ) { return x*x; }
  };

  const float math<float>::zero           = 0.0f;
  const float math<float>::one            = 1.0f;
  const float math<float>::two            = 2.0f;
  const float math<float>::three          = 3.0f;
  const float math<float>::four           = 4.0f;
  const float math<float>::half           = 0.5f;
  const float math<float>::quarter        = 0.5f;
  const float math<float>::pi             = 3.141592654f;
  const float math<float>::pi_by_180      = 3.141592654f/180.0f;
  const float math<float>::pi_by_two      = 1.570796327f;
  const float math<float>::sqrt_two       = sqrtf(2.0f);
  const float math<float>::sqrt_three     = sqrtf(3.0f);
  const float math<float>::sqrt_pi_by_two = sqrtf(1.570796327f);
  const float math<float>::_180           = 180.0f;

  const double math<double>::zero           = 0.0;
  const double math<double>::one            = 1.0;
  const double math<double>::two            = 2.0;
  const double math<double>::three          = 3.0;
  const double math<double>::four           = 4.0;
  const double math<double>::half           = 0.5;
  const double math<double>::quarter        = 0.5;
  const double math<double>::pi             = 3.141592654;
  const double math<double>::pi_by_180      = 3.141592654/180.0;
  const double math<double>::pi_by_two      = 1.570796327;
  const double math<double>::sqrt_two       = sqrt(2.0);
  const double math<double>::sqrt_three     = sqrt(3.0);
  const double math<double>::sqrt_pi_by_two = sqrt(1.570796327);
  const double math<double>::_180           = 180.0;

  template<unsigned int P, typename T> T pow( T x )
  { return math<T>::pow<P>(x); }

  template<typename T> T sqr( T x )
  { return math<T>::sqr(x); }



  //--------------------------------------------------------------------------
  template<typename T=double> struct Complex {
    T r,  //!< Real component
      i;  //!< Imaginary component

    inline Complex( T nR=0, T nI=0 ) : r(nR), i(nI) {}
    inline Complex( const Complex& other ) : r(other.r), i(other.i) {}

    inline const Complex& operator =( const Complex& other )
    { r=other.r; i=other.i; return *this; }

    inline const Complex& operator -=( const Complex& other )
    { r-=other.r; i-=other.i; return *this; }

    inline const Complex& operator +=( const Complex& other )
    { r+=other.r; i+=other.i; return *this; }

    inline const Complex& operator *=( T x ) { r*=x; i*=x; return *this; }

    inline const Complex& operator /=( T x ) { r/=x; i/=x; return *this; }

    inline const Complex& operator *=( const Complex& other )
    {
      T tr = r*other.r - i*other.i, ti = r*other.i + i*other.r;
      r=tr; i=ti;
      return *this;
    }

    inline const Complex& operator /=( const Complex& other )
    { return this->operator*=( other.conjugate() ); }

    inline Complex operator +( const Complex& other ) const
    { return Complex( r+other.r, i+other.i ); }

    inline Complex operator -( const Complex& other ) const
    { return Complex( r-other.r, i-other.i ); }

    inline Complex operator *( T x ) const { return Complex( r*x, i*x ); }

    inline Complex operator /( T x ) const { return Complex( r/x, i/x ); }

    inline Complex operator *( const Complex& other ) const
    { return Complex( r*other.r - i*other.i, i*other.r + r*other.i ); }

    inline Complex conjugate( void ) const
    { T div = r*r + i*i; return Complex( r/div, -i/div ); }

    inline Complex operator /( const Complex& other ) const
    { return (*this) * other.conjugate(); }
  };



  //--------------------------------------------------------------------------
  //! Computes the Fresnel integrals S(x) and C(x) for all real x.
  //! @param[in]  x   value for which to compute fresnel integrals
  //! @param[out] s   pointer where S(x) will be stored
  //! @param[out] c   pointer where C(x) will be stored
  //! @return         true upon successful computation
  template<typename T> bool fresnel(
    T x, T *s, T *c,
    unsigned int max_iter=100, T epsilon=T(6.0e-8f), T divide_line=T(1.5f) )
  {
    bool result=true;
    int n;
    bool odd;
    T a,ax,fact,pix2,sign,sum,sumc,sums,term,test;
    Complex<T> b,cc,d,h,del,cs;
    ax=fabs(x);
    if( ax < sqrt(std::numeric_limits<T>::epsilon()) ) {
      // Special case: avoid failure of convergence test because of underflow.
      *s=T(0);
      *c=ax;
    }
    else if( ax <= divide_line ) {
      // Evaluate both series simultaneously.
      sum=sums=T(0);
      sumc=ax;
      sign=T(1);
      fact=math<T>::pi_by_two*ax*ax;
      odd=true;
      term=ax;
      n=3;
      unsigned int k;
      for( k=1 ; k<=max_iter ; k++ ) {
        term *= fact/k;
        sum += sign*term/n;
        test= fabs(sum)*epsilon;
        if (odd) {
          sign = -sign;
          sums=sum;
          sum=sumc;
        }
        else {
          sumc=sum;
          sum=sums;
        }
        if( term < test ) break;
        odd=!odd;
        n+=2;
      }
      if( k > max_iter ) result=false;
      *s=sums;
      *c=sumc;
    }
    else {
      // Evaluate continued fraction by modified Lentz's method.
      pix2=math<T>::pi*ax*ax;
      b=Complex<T>(1.0f,-pix2);
      cc=Complex<T>(std::numeric_limits<T>::max(),T(0));
      d=h= Complex<T>(1.0f,0.0f) / b;
      n = -1;
      unsigned int k;
      for( k=2 ; k<=max_iter ; k++ ) {
        n+=2;
        a = (float)( -n*(n+1) );
        b += Complex<T>(4.0f,0.0f);
        d = ( d*a + b ).conjugate(); // Denominators cannot be zero
        cc = b + (Complex<T>(a,0.0f) / cc);
        del = cc * d;
        h = h * del;
        if( fabs(del.r-1.0)+fabs(del.i) < epsilon ) break;
      }
      if( k > max_iter ) result=false;
      h*=Complex<T>(ax,-ax);
      cs=Complex<T>(T(0.5f),T(0.5f)) *
        ( Complex<T>(T(1.0f)-cos(T(0.5f)*pix2),-sin(T(0.5f)*pix2)) * h );
      *c=cs.r;
      *s=cs.i;
    }
    if (x < 0.0) { //Use antisymmetry.
      *c = -(*c);
      *s = -(*s);
    }
    return result;
  }

  //--------------------------------------------------------------------------
  template<typename T> T poly3length2param(
    T a, T b, T c, T d,
    T length,
    unsigned int min_subdiv=1024,
    unsigned int max_subdiv=16384,
    T tolerance=T(0.0001) )
  {
    // Find interest points of function...
    T da=T(3)*d, db=T(2)*c, dc=b;
    T delta = db*db - T(4)*da*dc;
    std::list<double> interest_points;
    if( fabs(delta) < std::numeric_limits<T>::epsilon() )
      interest_points.push_back( T(-0.5f) * db / da );
    else if( delta>0.0 ) {
      interest_points.push_back( T(-0.5f) * (db - sqrt(delta)) / da );
      interest_points.push_back( T(-0.5f) * (db + sqrt(delta)) / da );
    }
    
    // Iterate until a satisfying solution is found...
    T min_result = T(0);
    T max_result = length;
    do {
      T min_length = T(0);
      T max_length = T(0);
      T x=T(0), y=a, dx=max_result/min_subdiv;
      while( min_length<length ) {
        double ox=x, oy=y;
        x += dx;
        for( std::list<double>::const_iterator it=interest_points.begin() ;
             it!=interest_points.end() ; ++it )
          if( ox<(*it) && x>(*it) ) {
            y = a + b*(*it) + c*pow<2>(*it) + d*pow<3>(*it);
            min_length += sqrt( sqr(ox-*it) + sqr(oy-y) );
            max_length += fabs( ox-*it ) + fabs( oy-y );
            ox=*it;
            oy=y;
          }
        y = a + b*x + c*x*x + d*x*x*x;
        min_length += sqrt( sqr(ox-x) + sqr(oy-y) );
        max_length += fabs( ox-x ) + fabs( oy-y );
        if( max_length<=length ) min_result = x;
        if( min_length<=length ) max_result = x;
      }

      min_subdiv *= 2;
    }
    while( max_result-min_result > tolerance && min_subdiv <= max_subdiv );
    return max_result;
  } // poly3length2param
} // namespace asphalt  

#endif  // #ifndef _ASPHALT_MATH_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 80
 * End:
 */
