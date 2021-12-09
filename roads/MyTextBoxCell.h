#pragma once

using namespace System;
using namespace System::Windows::Forms;

#include "MyTextBoxCellEdit.h"

namespace roads {

  private ref class MyTextBoxCell :
    public DataGridViewTextBoxCell
  {
  public:
    virtual property Type^ EditType {
      Type^ get( void ) override { return MyTextBoxCellEdit::typeid; }
    }

  }; // class MyCellTextBox

} // namespace roads
