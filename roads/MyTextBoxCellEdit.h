#pragma once

using namespace System;
using namespace System::Windows::Forms;

namespace roads {

  private ref class MyTextBoxCellEdit:
    public DataGridViewTextBoxEditingControl
  {
  public:
    virtual bool EditingControlWantsInputKey(
      Keys keyData, bool dataGridViewWantsInputKey ) override
    {
      switch( keyData & Keys::KeyCode )
      {
      case Keys::PageUp:
      case Keys::PageDown:
      case Keys::End:
      case Keys::Home:
      case Keys::Left:
      case Keys::Right:
      case Keys::Delete:
        return true;
      }
      return DataGridViewTextBoxEditingControl::EditingControlWantsInputKey(
        keyData, dataGridViewWantsInputKey );
    }

  protected:
    virtual bool ProcessCmdKey( Message %m, Keys k ) override
    {
      if( m.Msg==256 && k==Keys::Enter ) {
        EditingControlDataGridView->EndEdit();
        Forms::SendKeys::Send("\t");
        return true;
      }
      return DataGridViewTextBoxEditingControl::ProcessCmdKey(m,k);
    }

  }; // class MyCellTextBoxEdit

} // namespace roads
