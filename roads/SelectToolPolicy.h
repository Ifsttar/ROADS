//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file SelectToolPolicy.h
//! \brief Declaration of class roads::SelectToolPolicy.

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;

#include "ToolPolicy.h"

namespace roads {

  //! Policies for select tool.
  private ref class SelectToolPolicy : public ToolPolicy {
  public:
    virtual property Forms::Cursor^ Cursor {
      virtual Forms::Cursor^ get( void ) override { return Cursors::Arrow; }
    }
    virtual void OnLMBDown( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBUp( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBDrag( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnAbort( MainForm ^form ) override;
    virtual void OnGLPanelPaint( MainForm ^form ) override;
    static property SelectToolPolicy^ Instance {
      SelectToolPolicy^ get( void ) { return _instance; }
    }

  private:
    static SelectToolPolicy^ _instance = gcnew SelectToolPolicy();
  };

}
