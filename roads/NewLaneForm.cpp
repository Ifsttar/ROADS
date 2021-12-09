#include "StdAfx.h"
#include "NewLaneForm.h"

using namespace roads;

//----------------------------------------------------------------------------
NewLaneForm::NewLaneForm( int min, int max )
{
	InitializeComponent();
	_prevValue = min;
  _indexBox->Minimum = Decimal(min);
  _indexBox->Maximum = Decimal(max);
  _indexBox->Value   = Decimal(min);
}

//----------------------------------------------------------------------------
int NewLaneForm::LaneIndex::get(void) { return (int)_indexBox->Value; }

//----------------------------------------------------------------------------
NewLaneForm::~NewLaneForm()
{
	if (components)
	{
		delete components;
	}
}

//----------------------------------------------------------------------------
void NewLaneForm::ValueChanged( Object ^sender, EventArgs ^e ) {
  if( _indexBox->Value==Decimal(0) ) {
    EventHandler ^handler = gcnew EventHandler( this, &NewLaneForm::ValueChanged );
    _indexBox->ValueChanged -= handler;
    if( _prevValue<0 ) _indexBox->Value = Decimal(1);
    else               _indexBox->Value = Decimal(-1);
    _indexBox->ValueChanged += handler;
    _prevValue = (int)_indexBox->Value;
  }
}
