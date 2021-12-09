//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file SelectToolPolicy.cpp
//! \brief Definition of methods of class roads::SelectToolPolicy.

#include "stdafx.h"
#include "SelectToolPolicy.h"
#include "MainForm.h"

namespace roads {

  void SelectToolPolicy::OnLMBDown( MainForm ^form, MouseEventArgs ^args )
  {
    DomainLogic::Instance->PickNode(
      form->GLPanelX, form->GLPanelY, 10.0 * form->GLPanelMeterPerPixel );
  }

  void SelectToolPolicy::OnLMBUp( MainForm ^form, MouseEventArgs ^args )
  {

  }

  void SelectToolPolicy::OnLMBDrag( MainForm ^form, MouseEventArgs ^args )
  {

  }

  void SelectToolPolicy::OnAbort( MainForm ^form )
  {

  }

  void SelectToolPolicy::OnGLPanelPaint( MainForm ^form )
  {

  }

} // namespace roads
