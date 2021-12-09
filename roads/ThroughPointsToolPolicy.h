//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file ThroughPointsToolPolicy.h
//! \brief Declaration of class roads::ThroughPointsToolPolicy.

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Collections;

#include "ToolPolicy.h"

namespace roads {

  //! Policies for road-through-points tool.
  private ref class ThroughPointsToolPolicy : public ToolPolicy {
  public:
    virtual property Forms::Cursor^ Cursor {
      virtual Forms::Cursor^ get( void ) override { return Cursors::Cross; }
    }
    virtual void OnLMBDown( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBUp( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnLMBDrag( MainForm ^form, MouseEventArgs ^args ) override;
    virtual void OnAbort( MainForm ^form ) override;
    virtual void OnGLPanelPaint( MainForm ^form ) override;
    static property ThroughPointsToolPolicy^ Instance {
      ThroughPointsToolPolicy^ get( void ) { return _instance; }
    }

  protected:
    //! Destructor/releaser:
    //! release allocated resources and make object ready for recycle.
    ~ThroughPointsToolPolicy();

    //! Finalizer: release resources still referenced.
    !ThroughPointsToolPolicy();

  private:
    //! Constructor: allocate necessary resources.
    ThroughPointsToolPolicy( void );

    static ThroughPointsToolPolicy^ _instance = gcnew ThroughPointsToolPolicy();

    Generic::List<double>
        ^_x, ^_y;                   //!< Coordinates of points to connect
    misc::WeakRef<const odr::Road>
        *_createdRoad;              //!< Weak reference to created road
    int _dragIndex;                 //!< Index of dragged point, -1 for none

  }; // class ThroughPointsToolPolicy

} // namespace roads
