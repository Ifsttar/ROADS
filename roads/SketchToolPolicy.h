//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file SketchToolPolicy.h
//! \brief Declaration of class roads::SketchToolPolicy.

#pragma once

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;

#include "ToolPolicy.h"

namespace roads {

  ref class SketchOptionsForm;

  //! Policies for sketch tool.
  private ref class SketchToolPolicy : public ToolPolicy {
  public:
    SketchToolPolicy( void );
    ~SketchToolPolicy();
    !SketchToolPolicy();

    virtual property Forms::Cursor^ Cursor {
      virtual Forms::Cursor^ get( void ) override { return Cursors::Cross; }
    }
    virtual void OnLMBDown( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBUp( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBDrag( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnAbort( MainForm ^form ) override;
    virtual void OnGLPanelPaint( MainForm ^form ) override;
    static property SketchToolPolicy^ Instance {
      SketchToolPolicy^ get( void ) { return _instance; }
    }
    void OnPrimitiveCostChanged( Object ^sender, EventArgs ^args );
    void OnOptionsClosed( Object ^sender, EventArgs ^args );

  private:
    static SketchToolPolicy ^_instance = gcnew SketchToolPolicy();

    Collections::Generic::List<double> ^_x, ^_y;
    SketchOptionsForm ^_optionsForm;
    misc::WeakRef<const odr::Road> *_createdRoad;
  }; // ref class SketchToolPolicy

} // namespace roads
