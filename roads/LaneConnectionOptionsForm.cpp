#include "StdAfx.h"
#include "LaneConnectionOptionsForm.h"
#include "DomainLogic.h"

namespace roads {

  LaneConnectionOptionsForm::LaneConnectionOptionsForm()
	{
		InitializeComponent();
    _roadA = new misc::WeakRef<const odr::Road>(0);
    _roadB = new misc::WeakRef<const odr::Road>(0);
	}

  LaneConnectionOptionsForm::~LaneConnectionOptionsForm()
  {
    delete _roadA;
    _roadA = 0;
    delete _roadB;
    _roadB = 0;
		delete components;
  }

  LaneConnectionOptionsForm::!LaneConnectionOptionsForm()
  {
    delete _roadA;
    delete _roadB;
  }

  void LaneConnectionOptionsForm::SetRoads(
    const odr::Road *roadA, RoadEnd endA,
    const odr::Road *roadB, RoadEnd endB )
  {
    if( _roadA->ptr()==roadA && _roadB->ptr()==roadB
        && endA==_endA && endB==_endB )
      return;

    Enabled = false;

    *_roadA = roadA;
    _endA   = endA;
    *_roadB = roadB;
    _endB   = endB;

    // Update UI for road A...
    if( roadA ) {
      _roadALabelBox->Text =
        String::Format( "Road {0} ({1}) {2}",
                        UInt32(roadA->ID()).ToString(),
                        gcnew String(roadA->name()),
                        gcnew String(endA==RoadEnd::End ? "end" : "start") );

      int firstLane, lastLane, laneIncrement;
      const odr::LaneSection *section=0;
      if( endA==RoadEnd::Start ) {
        section = roadA->lanes()->sectionAt( 0.0 );
        firstLane = (int)section->countLeft();
        lastLane = -(int)section->countRight();
        if( lastLane && !firstLane ) firstLane = -1;
        laneIncrement = -1;
      }
      else {
        section = roadA->lanes()->sectionAt( roadA->length() );
        firstLane = -(int)section->countRight();
        lastLane = (int)section->countLeft();
        if( lastLane && !firstLane ) firstLane = 1;
        laneIncrement = 1;
      }
      int count = (int)( section->countLeft()+section->countRight() );
      while( _roadALanesBox->Rows->Count > count )
        _roadALanesBox->Rows->RemoveAt( _roadALanesBox->Rows->Count-1 );
      cli::array<Object^> ^defaultRowData =
        { gcnew Int32(0), gcnew Boolean(true), gcnew Boolean(false) };
      while( _roadALanesBox->Rows->Count < count )
        _roadALanesBox->Rows->Add( defaultRowData );

      int laneIndex = firstLane;
      for( int i=0 ; i<count ; ++i ) {
        DataGridViewRow ^row = _roadALanesBox->Rows[i];        
        row->Cells[0]->Value = gcnew Int32( laneIndex );
        laneIndex += laneIncrement;
        if( laneIndex==0 ) laneIndex += laneIncrement;
      }
    }
    else {
      _roadALabelBox->Text = "(no road end selected)";
      _roadALanesBox->Rows->Clear();
    }

    // Update UI for road B...
    if( roadB ) {
      _roadBLabelBox->Text =
        String::Format( "Road {0} ({1}) {2}",
                        UInt32(roadB->ID()).ToString(),
                        gcnew String(roadB->name()),
                        gcnew String(endB==RoadEnd::End ? "end" : "start") );

      int firstLane, lastLane, laneIncrement;
      const odr::LaneSection *section=0;
      if( endB==RoadEnd::Start ) {
        section = roadB->lanes()->sectionAt( 0.0 );
        firstLane = -(int)section->countRight();
        lastLane = (int)section->countLeft();
        if( lastLane && !firstLane ) firstLane = 1;
        laneIncrement = 1;
      }
      else {
        section = roadB->lanes()->sectionAt( roadA->length() );
        firstLane = (int)section->countLeft();
        lastLane = -(int)section->countRight();
        if( lastLane && !firstLane ) firstLane = -1;
        laneIncrement = -1;
      }
      int count = (int)( section->countLeft()+section->countRight() );
      while( _roadBLanesBox->Rows->Count > count )
        _roadBLanesBox->Rows->RemoveAt( _roadBLanesBox->Rows->Count-1 );
      cli::array<Object^> ^defaultRowData =
        { gcnew Int32(0), gcnew Boolean(true), gcnew Boolean(false) };
      while( _roadBLanesBox->Rows->Count < count )
        _roadBLanesBox->Rows->Add( defaultRowData );

      int laneIndex = firstLane;
      for( int i=0 ; i<count ; ++i ) {
        DataGridViewRow ^row = _roadBLanesBox->Rows[i];
        row->Cells[0]->Value = gcnew Int32( laneIndex );
        laneIndex += laneIncrement;
        if( laneIndex==0 ) laneIndex += laneIncrement;
      }
    }
    else {
      _roadBLabelBox->Text = "(no road end selected)";
      _roadBLanesBox->Rows->Clear();
    }

    Enabled = true;
  }

  const odr::Road* LaneConnectionOptionsForm::RoadA::get( void )
  { return _roadA->ptr(); }

  LaneConnectionOptionsForm::RoadEnd
    LaneConnectionOptionsForm::EndA::get( void )
  { return _endA; }

  const odr::Road* LaneConnectionOptionsForm::RoadB::get( void )
  { return _roadB->ptr(); }

  LaneConnectionOptionsForm::RoadEnd
    LaneConnectionOptionsForm::EndB::get( void )
  { return _endB; }
  
  void LaneConnectionOptionsForm::OptionsChanged::add( EventHandler ^handler )
  { _optionsChanged += handler; }

  void LaneConnectionOptionsForm::OptionsChanged::remove( EventHandler ^handler )
  { _optionsChanged -= handler; }

  Generic::List<int>^ LaneConnectionOptionsForm::LanesA::get( void )
  {
    Generic::List<int> ^lanesA = gcnew Generic::List<int>;

    // Find index of first used lane in road A...
    int aRowIndex=0, aLaneIndex=0;
    bool aUse=false, aCollapse=false;
    while( aRowIndex<_roadALanesBox->Rows->Count && !aUse ) {
      DataGridViewRow ^row = _roadALanesBox->Rows[aRowIndex];
      aLaneIndex = *safe_cast<Int32^>  ( row->Cells[0]->Value );
      aUse       = *safe_cast<Boolean^>( row->Cells[1]->Value );
      aCollapse  = *safe_cast<Boolean^>( row->Cells[2]->Value );
      if(!aUse) ++aRowIndex;
    }

    // Find index of first used lane in road B...
    int bRowIndex=0, bLaneIndex=0;
    bool bUse=false, bCollapse=false;
    while( bRowIndex<_roadBLanesBox->Rows->Count && !bUse ) {
      DataGridViewRow ^row = _roadBLanesBox->Rows[bRowIndex];
      bLaneIndex = *safe_cast<Int32^>  ( row->Cells[0]->Value );
      bUse       = *safe_cast<Boolean^>( row->Cells[1]->Value );
      bCollapse  = *safe_cast<Boolean^>( row->Cells[2]->Value );
      if(!bUse) ++bRowIndex;
    }

    // Fill result...
    while( aUse || bUse )
    {
      if( aRowIndex < _roadALanesBox->Rows->Count ) {
        DataGridViewRow ^row = _roadALanesBox->Rows[aRowIndex];
        aLaneIndex = *safe_cast<Int32^>  ( row->Cells[0]->Value );
        aUse       = *safe_cast<Boolean^>( row->Cells[1]->Value );
        aCollapse  = *safe_cast<Boolean^>( row->Cells[2]->Value );
      }
      else
        aUse = false;

      if( bRowIndex < _roadBLanesBox->Rows->Count ) {
        DataGridViewRow ^row = _roadBLanesBox->Rows[bRowIndex];
        bLaneIndex = *safe_cast<Int32^>  ( row->Cells[0]->Value );
        bUse       = *safe_cast<Boolean^>( row->Cells[1]->Value );
        bCollapse  = *safe_cast<Boolean^>( row->Cells[2]->Value );
      }
      else
        bUse = false;

      if( aUse && (aCollapse || !bUse) ) {
        lanesA->Add( aLaneIndex );
        ++aRowIndex;
      }

      else if( bUse && (bCollapse || !aUse) ) {
        lanesA->Add( 0 );
        ++bRowIndex;
      }

      else if( aUse && bUse ) {
        lanesA->Add( aLaneIndex );
        ++aRowIndex;
        ++bRowIndex;
      }
    }

    return lanesA;
  }

  Generic::List<int>^ LaneConnectionOptionsForm::LanesB::get( void )
  {
    Generic::List<int> ^lanesB = gcnew Generic::List<int>;

    // Find index of first used lane in road A...
    int aRowIndex=0, aLaneIndex=0;
    bool aUse=false, aCollapse=false;
    while( aRowIndex<_roadALanesBox->Rows->Count && !aUse ) {
      DataGridViewRow ^row = _roadALanesBox->Rows[aRowIndex];
      aLaneIndex = *safe_cast<Int32^>  ( row->Cells[0]->Value );
      aUse       = *safe_cast<Boolean^>( row->Cells[1]->Value );
      aCollapse  = *safe_cast<Boolean^>( row->Cells[2]->Value );
      if(!aUse) ++aRowIndex;
    }

    // Find index of first used lane in road B...
    int bRowIndex=0, bLaneIndex=0;
    bool bUse=false, bCollapse=false;
    while( bRowIndex<_roadBLanesBox->Rows->Count && !bUse ) {
      DataGridViewRow ^row = _roadBLanesBox->Rows[bRowIndex];
      bLaneIndex = *safe_cast<Int32^>  ( row->Cells[0]->Value );
      bUse       = *safe_cast<Boolean^>( row->Cells[1]->Value );
      bCollapse  = *safe_cast<Boolean^>( row->Cells[2]->Value );
      if(!bUse) ++bRowIndex;
    }

    // Fill result...
    while( aUse || bUse )
    {
      if( aRowIndex < _roadALanesBox->Rows->Count ) {
        DataGridViewRow ^row = _roadALanesBox->Rows[aRowIndex];
        aLaneIndex = *safe_cast<Int32^>  ( row->Cells[0]->Value );
        aUse       = *safe_cast<Boolean^>( row->Cells[1]->Value );
        aCollapse  = *safe_cast<Boolean^>( row->Cells[2]->Value );
      }
      else
        aUse = false;

      if( bRowIndex < _roadBLanesBox->Rows->Count ) {
        DataGridViewRow ^row = _roadBLanesBox->Rows[bRowIndex];
        bLaneIndex = *safe_cast<Int32^>  ( row->Cells[0]->Value );
        bUse       = *safe_cast<Boolean^>( row->Cells[1]->Value );
        bCollapse  = *safe_cast<Boolean^>( row->Cells[2]->Value );
      }
      else
        bUse = false;

      if( aUse && (aCollapse || !bUse) ) {
        lanesB->Add( 0 );
        ++aRowIndex;
      }

      else if( bUse && (bCollapse || !aUse) ) {
        lanesB->Add( bLaneIndex );
        ++bRowIndex;
      }

      else if( aUse && bUse ) {
        lanesB->Add( bLaneIndex );
        ++aRowIndex;
        ++bRowIndex;
      }
    }

    return lanesB;
  }

  void LaneConnectionOptionsForm::OnButtonClick(
    Object ^sender, EventArgs ^e )
  {
    if( sender==_okBtn ) DialogResult = Windows::Forms::DialogResult::OK;
    else                 DialogResult = Windows::Forms::DialogResult::Cancel;
    Close();
  }

  void LaneConnectionOptionsForm::OnCellEnter(
    Object ^sender, DataGridViewCellEventArgs ^e )
  {
    DataGridView ^grid = dynamic_cast<DataGridView^>(sender);
    if( !grid ) return;

    const odr::Road *road = 0;
    RoadEnd end;
    if( grid==_roadALanesBox ) {
      road = _roadA->ptr();
      end  = _endA;
    }
    else {
      road = _roadB->ptr();
      end  = _endB;
    }
    if( !road ) return;

    const odr::LaneSection *section = 0;
    if( end==RoadEnd::Start )
      section = road->lanes()->sectionAt( 0.0 );
    else
      section = road->lanes()->sectionAt( road->length() );
    if( !section ) return;

    DataGridViewRow ^row = grid->Rows[ e->RowIndex ];
    int laneIndex = *dynamic_cast<Int32^>( row->Cells[0]->Value );
    const odr::Lane *lane = section->item( laneIndex );
    if( !lane ) return;

    DomainLogic::Instance->SelectedNode = lane;
  }

  void LaneConnectionOptionsForm::OnCellEdit(
    Object ^sender, DataGridViewCellEventArgs ^e )
  {
    if( Enabled && _optionsChanged )
      _optionsChanged( this, EventArgs::Empty );
  }

  void LaneConnectionOptionsForm::OnDirtyCell( Object ^sender, EventArgs ^e )
  {
    if( _roadALanesBox->Focused ) _roadALanesBox->CommitEdit(DataGridViewDataErrorContexts::Commit);
    if( _roadBLanesBox->Focused ) _roadBLanesBox->CommitEdit(DataGridViewDataErrorContexts::Commit);
  }
}
