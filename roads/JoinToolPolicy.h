//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file JoinToolPolicy.h
//! \brief Declaration of class roads::JoinToolPolicy.

#pragma once

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Collections;

#include "ToolPolicy.h"

namespace roads {

  ref class LaneConnectionOptionsForm;

  //! Policies for join tool.
  private ref class JoinToolPolicy : public ToolPolicy {
  public:
    JoinToolPolicy( void );
    ~JoinToolPolicy();
    !JoinToolPolicy();

    virtual property Forms::Cursor^ Cursor {
      virtual Forms::Cursor^ get( void ) override { return Cursors::Default; }
    }
    virtual void OnLMBDown( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBUp( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBDrag( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnAbort( MainForm ^form ) override;
    virtual void OnGLPanelPaint( MainForm ^form ) override;
    static property JoinToolPolicy^ Instance {
      JoinToolPolicy^ get( void ) { return _instance; }
    }

  private:
    static JoinToolPolicy ^_instance = gcnew JoinToolPolicy();

    misc::WeakRef<const odr::Road> *_createdRoad;
    value class RoadEnd {
    public:
      const odr::Road* road;
      bool end;
    };
    Generic::List<RoadEnd> ^_toConnect;

    LaneConnectionOptionsForm ^_optionsForm;

    void OnOptionsClosed( Object ^sender, EventArgs ^args );
    void UpdateJunction( Object ^sender, EventArgs ^args );
  };

} // namespace roads
