//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file SplitToolPolicy.h
//! \brief Declaration of class roads::SplitToolPolicy.

#pragma once

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;

#include "ToolPolicy.h"

namespace roads {

  //! Policies for split tool.
  private ref class SplitToolPolicy : public ToolPolicy {
  public:
    SplitToolPolicy( void );
    ~SplitToolPolicy();
    !SplitToolPolicy();

    virtual property Forms::Cursor^ Cursor {
      virtual Forms::Cursor^ get( void ) override { return Cursors::Cross; }
    }
    virtual void OnLMBDown( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBUp( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBDrag( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnAbort( MainForm ^form ) override;
    virtual void OnGLPanelPaint( MainForm ^form ) override;
    static property SplitToolPolicy^ Instance {
      SplitToolPolicy^ get( void ) { return _instance; }
    }

  private:
    static SplitToolPolicy ^_instance = gcnew SplitToolPolicy();

    misc::WeakRef<const odr::Road> *_road;
    double _s;
  };

} // namespace roads
