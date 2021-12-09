//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ProlongateToolPolicy.h
//! \brief Declaration of class roads::ProlongateToolPolicy.

#pragma once

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;

#include "ToolPolicy.h"

namespace roads {

  //! Policies for prolongate tool.
  private ref class ProlongateToolPolicy: public ToolPolicy
  {
  public:
    static property ProlongateToolPolicy^ Instance {
      ProlongateToolPolicy^ get( void ) { return _instance; }
    }

    virtual property Forms::Cursor^ Cursor {
      virtual Forms::Cursor^ get( void ) override { return Cursors::Cross; }
    }
    virtual void OnLMBDown( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBUp( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBDrag( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnAbort( MainForm ^form ) override;
    virtual void OnGLPanelPaint( MainForm ^form ) override;

  private:
    ProlongateToolPolicy(void);

    static ProlongateToolPolicy^ _instance = gcnew ProlongateToolPolicy();

    bool _zeroCurvatureAtEnd;

  }; // class ProlongateToolPolicy

} // namespace roads
