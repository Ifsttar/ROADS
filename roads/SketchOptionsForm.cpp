#include "StdAfx.h"
#include "SketchOptionsForm.h"

namespace roads {

  SketchOptionsForm::SketchOptionsForm( void )
	{
		InitializeComponent();
	}

  SketchOptionsForm::~SketchOptionsForm()
	{
		delete components;
	}

  double SketchOptionsForm::PrimitiveCost::get( void )
  {
    return (double)(200-_primitiveCostBar->Value) * 0.001;
  }

  void SketchOptionsForm::PrimitiveCostChanged::add( EventHandler ^h )
  { _primitiveCostBar->ValueChanged += h; }

  void SketchOptionsForm::PrimitiveCostChanged::remove( EventHandler ^h )
  { _primitiveCostBar->ValueChanged -= h; }

  void SketchOptionsForm::OnButtonClick( Object ^sender, EventArgs ^args )
  {
    Button ^btn = dynamic_cast<Button^>(sender);
    DialogResult = btn->DialogResult;
    Close();
  }

} // namespace roads
