//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file MainForm_GridView.cpp
//! \brief Implementation of roads::MainForm's data grid view-related methods.


#include "stdafx.h"
#include "MainForm.h"
#include "MyTextBoxCell.h"

namespace roads {

  bool MainForm::GridViewInit()
  {
    // Bind event handlers...
    DomainLogic::Instance->SelectionChanged += Adapter<DomainLogicEventArgs^>(
      gcnew EventHandler(this,&MainForm::GridViewUpdate) );

    DomainLogic::Instance->NodeModified += Adapter<DomainLogicEventArgs^>(
      gcnew EventHandler(this,&MainForm::GridViewUpdate) );

    return true;
  }

  //--------------------------------------------------------------------------
  void MainForm::GridViewResize( int numRows )
  {
    while( _dataGridView->Rows->Count > numRows )
      _dataGridView->Rows->RemoveAt( _dataGridView->Rows->Count-1 );

    while( _dataGridView->Rows->Count < numRows ) {
      DataGridViewRow ^row = gcnew DataGridViewRow;
      row->Cells->Add( gcnew MyTextBoxCell() );
      row->Cells->Add( gcnew MyTextBoxCell() );
      _dataGridView->Rows->Add(row);
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::GridViewUpdateRow(
    int rowIndex,
    String ^name,
    Object ^value,
    DomainLogic::NodeEditDelegate ^cb )
  {
    if( !_doubleFormatInfo ) {
      _doubleFormatInfo = gcnew System::Globalization::NumberFormatInfo();
      _doubleFormatInfo->NumberDecimalSeparator = ".";
      _doubleFormatInfo->NaNSymbol = "#IND";
      _doubleFormatInfo->PositiveInfinitySymbol = "+INF";
      _doubleFormatInfo->NegativeInfinitySymbol = "-INF";
    }

    DataGridViewRow ^row = _dataGridView->Rows[rowIndex];
    DataGridViewCell ^cell = row->Cells[0];
    cell->Value = name;

    cell = row->Cells[1];
    cell->ValueType = value->GetType();
    cell->Value = value;
    if( value->GetType()==Double::typeid ) {
      cell->Style->FormatProvider = _doubleFormatInfo;
      cell->Style->Format = "0.#########";
    }
    else {
      cell->Style->FormatProvider =
        _dataGridView->DefaultCellStyle->FormatProvider;
      cell->Style->Format = _dataGridView->DefaultCellStyle->Format;
    }
    if( !cb ) {
      cell->ReadOnly = true;
      cell->Style->ForeColor = Drawing::Color::Gray;
      cell->Tag = nullptr;
    }
    else {
      cell->ReadOnly = false;
      cell->Style->ForeColor = _dataGridView->DefaultCellStyle->ForeColor;
      cell->Tag = cb;
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::GridViewUpdate( Object ^sender, EventArgs ^e )
  {
    // Retrieve object bound to selected item...
    const misc::WeakRefServer *object = DomainLogic::Instance->SelectedNode;

    // If no selection, disable grid view and leave...
    if( !object ) {
      _dataGridView->Rows->Clear();
      _dataGridView->Enabled = false;
      return;
    }

    // ...else, enable data grid view...
    _dataGridView->Enabled = true;

    // Fill data grid view depending on selection type...
    if( const odr::Road *road = dynamic_cast<const odr::Road*>(object) ) {
      GridViewResize( 5 );
      GridViewUpdateRow( 0, "ID", gcnew UInt32(road->ID()), nullptr );
      GridViewUpdateRow( 1, "name", gcnew String(road->name()),
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetElementName) );
      GridViewUpdateRow( 2, "length", gcnew Double(road->length()),
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetRoadLength) );
      GridViewUpdateRow(
        3,
        "predecessor",
        road->predecessor() ?
          String::Format( "{0} ({1})", road->predecessor()->ID(), gcnew String(road->predecessor()->name()) ) :
          gcnew String("none"),
        nullptr );
      GridViewUpdateRow(
        4,
        "successor",
        road->successor() ?
          String::Format( "{0} ({1})", road->successor()->ID(), gcnew String(road->successor()->name()) ) :
          gcnew String("none"),
        nullptr );
    }

    else if( const odr::Junction *junction = dynamic_cast<const odr::Junction*>(object) ) {
      GridViewResize( 2 );
      GridViewUpdateRow( 0, "ID", gcnew UInt32(junction->ID()), nullptr );
      GridViewUpdateRow( 1, "name", gcnew String(junction->name()),
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetElementName) );
    }

    else if( const odr::GeometricPrimitive *geo =
              dynamic_cast<const odr::GeometricPrimitive*>(object) )
    {
      GridViewResize( 9 );
      double s = geo->s();
      GridViewUpdateRow( 0, "s", gcnew Double(s),
        s!=0.0 ? gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetGeometricPrimitiveStart) : nullptr );
      GridViewUpdateRow( 1, "x", gcnew Double(geo->x()),
        s==0.0 ? gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetGeometricPrimitiveX) : nullptr );
      GridViewUpdateRow( 2, "y", gcnew Double(geo->y()),
        s==0.0 ? gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetGeometricPrimitiveY) : nullptr );
      GridViewUpdateRow( 3, "heading", gcnew Double(geo->heading()*180.0/3.141592654),
        s==0.0 ? gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetGeometricPrimitiveHeadingDeg) : nullptr );
      GridViewUpdateRow( 4, "length", gcnew Double(geo->length()),
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetGeometricPrimitiveLength) );
      
      if( const odr::Arc *arc = dynamic_cast<const odr::Arc*>(geo) ) {
        GridViewResize( 6 );
        GridViewUpdateRow( 5, "curvature", gcnew Double(arc->curvature()), 
          gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetArcCurvature) );
      }

      else if( const odr::Spiral *spiral = dynamic_cast<const odr::Spiral*>(geo) ) {
        GridViewResize( 7 );
        GridViewUpdateRow( 5, "curvature start", gcnew Double(spiral->curvatureStart()), 
          gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetSpiralCurvatureStart) );
        GridViewUpdateRow( 6, "curvature end", gcnew Double(spiral->curvatureEnd()), 
          gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetSpiralCurvatureEnd) );
      }

      else if( const odr::Poly3 *poly3 = dynamic_cast<const odr::Poly3*>(geo) ) {
        GridViewUpdateRow( 5, "a", gcnew Double(poly3->a()),
          gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetPoly3A) );
        GridViewUpdateRow( 6, "b", gcnew Double(poly3->b()), 
          gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetPoly3B) );
        GridViewUpdateRow( 7, "c", gcnew Double(poly3->c()), 
          gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetPoly3C) );
        GridViewUpdateRow( 8, "d", gcnew Double(poly3->d()), 
          gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetPoly3D) );
      }

      else
        GridViewResize( 5 );
    }

    else if( const odr::Elevation *elev = dynamic_cast<const odr::Elevation*>(object) ) {
      GridViewResize( 6 );
      GridViewUpdateRow( 0, "start", gcnew Double(elev->start()),
        elev->start()!=0.0 ? gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetElevationStart) : nullptr );
      GridViewUpdateRow( 1, "length", gcnew Double(elev->length()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetElevationLength) );
      GridViewUpdateRow( 2, "a", gcnew Double(elev->a()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetElevationA) );
      GridViewUpdateRow( 3, "b", gcnew Double(elev->b()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetElevationB) );
      GridViewUpdateRow( 4, "c", gcnew Double(elev->c()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetElevationC) );
      GridViewUpdateRow( 5, "d", gcnew Double(elev->d()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetElevationD) );
    }

    else if( const odr::Crossfall *cross = dynamic_cast<const odr::Crossfall*>(object) ) {
      GridViewResize( 6 );
      GridViewUpdateRow( 0, "start", gcnew Double(cross->start()),
        cross->start()!=0.0 ? gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetCrossfallStart) : nullptr );
      GridViewUpdateRow( 1, "length", gcnew Double(cross->length()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetCrossfallLength) );
      GridViewUpdateRow( 2, "a", gcnew Double(cross->a()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetCrossfallA) );
      GridViewUpdateRow( 3, "b", gcnew Double(cross->b()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetCrossfallB) );
      GridViewUpdateRow( 4, "c", gcnew Double(cross->c()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetCrossfallC) );
      GridViewUpdateRow( 5, "d", gcnew Double(cross->d()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetCrossfallD) );
    }

    else if( const odr::Superelevation *super =
              dynamic_cast<const odr::Superelevation*>(object) )
    {
      GridViewResize( 6 );
      GridViewUpdateRow( 0, "start", gcnew Double(super->start()),
        super->start()!=0.0 ? gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetSuperelevationStart) : nullptr );
      GridViewUpdateRow( 1, "length", gcnew Double(super->length()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetSuperelevationLength) );
      GridViewUpdateRow( 2, "a", gcnew Double(super->a()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetSuperelevationA) );
      GridViewUpdateRow( 3, "b", gcnew Double(super->b()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetSuperelevationB) );
      GridViewUpdateRow( 4, "c", gcnew Double(super->c()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetSuperelevationC) );
      GridViewUpdateRow( 5, "d", gcnew Double(super->d()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetSuperelevationD) );
    }

    else if( const odr::LaneSection *section =
      dynamic_cast<const odr::LaneSection*>(object) )
    {
      GridViewResize( 2 );
      GridViewUpdateRow( 0, "start", gcnew Double(section->start()),
        section->start()!=0.0 ? gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetLaneSectionStart) : nullptr );
      GridViewUpdateRow( 1, "length", gcnew Double(section->length()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetLaneSectionLength) );
    }

    else if( const odr::LaneWidth *width =
              dynamic_cast<const odr::LaneWidth*>(object) )
    {
      GridViewResize( 6 );
      GridViewUpdateRow( 0, "start offset", gcnew Double(width->startOffset()),
        width->startOffset()!=0.0 ? gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetLaneWidthStartOffset) : nullptr );
      GridViewUpdateRow( 1, "length", gcnew Double(width->length()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetLaneWidthLength) );
      GridViewUpdateRow( 2, "a", gcnew Double(width->a()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetLaneWidthA) );
      GridViewUpdateRow( 3, "b", gcnew Double(width->b()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetLaneWidthB) );
      GridViewUpdateRow( 4, "c", gcnew Double(width->c()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetLaneWidthC) );
      GridViewUpdateRow( 5, "d", gcnew Double(width->d()), 
        gcnew DomainLogic::NodeEditDelegate(DomainLogic::Instance,&DomainLogic::SetLaneWidthD) );
    }

    else {
      GridViewResize( 0 );
      _dataGridView->Enabled = false;
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::GridViewEdited(
    Object ^sender, DataGridViewCellEventArgs ^e )
  {
    DataGridViewCell ^cell =
      _dataGridView->Rows[e->RowIndex]->Cells[e->ColumnIndex];

    DomainLogic::NodeEditDelegate ^cb =
      dynamic_cast<DomainLogic::NodeEditDelegate^>(cell->Tag);

    if( cb ) cb(cell->Value);
  }

  //--------------------------------------------------------------------------
  void MainForm::GridViewError(
    Object ^sender, DataGridViewDataErrorEventArgs ^args )
  {
    DataGridViewRow ^row = _dataGridView->Rows[args->RowIndex];
    MessageBox::Show(
      String::Format("Invalid input for {0}", row->Cells[0]->Value),
      "Invalid input", MessageBoxButtons::OK, MessageBoxIcon::Warning );
    DataGridViewCell ^cell = row->Cells[args->ColumnIndex];
    args->Cancel = true;
    args->ThrowException = false;
  }

} // namespace roads
