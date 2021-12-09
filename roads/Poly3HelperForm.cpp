//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file Poly3HelperForm.cpp
//! \brief Definition of methods of class roads::Poly3HelperForm.

#include "StdAfx.h"
#include "Poly3HelperForm.h"

namespace roads {

  Poly3HelperForm::Poly3HelperForm( void )
	{
		InitializeComponent();
    _editedValueName = "value";
    _length = 1.0;
	}

  Poly3HelperForm::~Poly3HelperForm()
	{
		delete components;
	}

  String^ Poly3HelperForm::EditedValueName::get( void )
  {
    return _editedValueName;
  }

  void Poly3HelperForm::EditedValueName::set( String ^s )
  {
    _editedValueName = s;
    _initialValueLbl->Text = "Initial " + s;
    _finalValueLbl->Text = "Final " + s;
  }

  //--------------------------------------------------------------------------
  double Poly3HelperForm::Length::get(void)
  {
    return _length;
  }

  void Poly3HelperForm::Length::set( double d )
  {
    _length = d;
  }

  double Poly3HelperForm::InitialValue::get( void )
  {
    return (double)_initialValueBox->Value;
  }

  void Poly3HelperForm::InitialValue::set( double d )
  {
    _initialValueBox->Value = Decimal(d);
  }

  double Poly3HelperForm::InitialDerivative::get( void )
  {
    return (double)_initialDerivativeBox->Value;
  }

  void Poly3HelperForm::InitialDerivative::set( double d )
  {
    _initialDerivativeBox->Value = Decimal(d);
  }

  double Poly3HelperForm::FinalValue::get( void )
  {
    return (double)_finalValueBox->Value;
  }

  void Poly3HelperForm::FinalValue::set( double d )
  {
    _finalValueBox->Value = Decimal(d);
  }

  double Poly3HelperForm::FinalDerivative::get( void )
  {
    return (double)_finalDerivativeBox->Value;
  }

  void Poly3HelperForm::FinalDerivative::set( double d )
  {
    _finalDerivativeBox->Value = Decimal(d);
  }

  //--------------------------------------------------------------------------
  double Poly3HelperForm::A::get(void)
  {
    return (double)_initialValueBox->Value;
  }

  double Poly3HelperForm::B::get(void)
  {
    return (double)_initialDerivativeBox->Value;
  }

  double Poly3HelperForm::C::get(void)
  {
    return
      ( 3.0*(double)_finalValueBox->Value
        - Length*(double)_finalDerivativeBox->Value
        - 3.0*A - 2.0*B*Length ) / (Length*Length);
  }

  double Poly3HelperForm::D::get(void)
  {
    return
      ( 2.0*A + B*Length + Length*(double)_finalDerivativeBox->Value
        - 2.0*(double)_finalValueBox->Value ) / (Length*Length*Length);
  }

} // namespace roads
