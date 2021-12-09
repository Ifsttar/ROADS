//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ToolPolicy.h
//! \brief Declaration of abstract class roads::ToolPolicy.

#pragma once

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;

namespace roads {
  ref class MainForm;

  //! Policies associated with an edition tool.
  private ref class ToolPolicy abstract {
  public:
    //! Cursor associated with tool in OpenGL view.
    virtual property Forms::Cursor^ Cursor {
      virtual Forms::Cursor^ get( void ) = 0;
    }

    //! Handler for left mouse button press.
    virtual void OnLMBDown( MainForm ^form, MouseEventArgs ^args ) = 0;

    //! Handler for left mouse button release.
    virtual void OnLMBUp( MainForm ^form, MouseEventArgs ^args ) = 0;

    //! Handler for mouse move while pressing LMB.
    virtual void OnLMBDrag( MainForm ^form, MouseEventArgs ^args ) = 0;

    //! Handler for aborting use of tool.
    //! Called when switching to another tool.
    virtual void OnAbort( MainForm ^form ) = 0;

    //! Method called while repainting OpenGL view.
    virtual void OnGLPanelPaint( MainForm ^form ) = 0;

  }; // class ToolPolicy

} // namespace roads
