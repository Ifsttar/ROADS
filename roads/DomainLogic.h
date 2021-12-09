//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file DomainLogic.h
//! \brief Declaration of class roads::DomainLogic.

#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::Diagnostics;

namespace roads {

  //--------------------------------------------------------------------------
  //! Implements domain logic for application roads.
	private ref class DomainLogic
	{
	public:
    //! Return reference to unique object instance.
    static property DomainLogic^ Instance {
      DomainLogic^ get( void );
    }

    //! Destructor: delete document if any and reset reference to instance.
		~DomainLogic();

    //! Finalizer: delete unmanaged resource.
    !DomainLogic();

    //------------------------------------------------------------------------
    //! @name Properties
    //@{

    //! Get pointer to working document as read-only.
    //! 0 when no document is open.
    property const odr::Document* Document {
      const odr::Document* get( void );
    };

    //! Get pointer to working document for modification.
    //! When possible, DomainLogic methods should be used instead to modify
    //! the document.
    //! Accessing this property will cause the DocumentModified event to be
    //! triggered at the next message loop iteration.
    //! 0 when no document is open.
    property odr::Document* DocumentRW {
      odr::Document* get( void );
    };

    //! Get or set the path of the file where document is stored.
    property String ^FileName {
      String^ get( void );
      void set( String^ );
    };

    //! Get or set currently selected document node as read-only.
    //! Setting will trigger the SelectionChanged event.
    property const misc::WeakRefServer* SelectedNode {
      const misc::WeakRefServer* get( void );
      void set( const misc::WeakRefServer* );
    };

    //! Get or set currently selected document node for modification.
    //! When possible, DomainLogic methods should be used instead to modify
    //! the document.
    //! Getting will cause the DocumentModified and NodeModified events to be
    //! triggered at the next message loop iteration.
    //! Setting will trigger the SelectionChanged event.
    property misc::WeakRefServer* SelectedNodeRW {
      misc::WeakRefServer* get( void );
      void set( misc::WeakRefServer* );
    };

    //@}

    //------------------------------------------------------------------------
    //! @name Events
    //@{
    
    //! Argument to event delegate concerning a DomainLogic.
    ref class DomainLogicEventArgs : public EventArgs {
    public:
      //! Empty event args.
      static property DomainLogicEventArgs^ Empty {
        DomainLogicEventArgs^ get( void ) { return _empty; }
      }

      //! Construction: event args empty by default.
      DomainLogicEventArgs():
        EventArgs(), _node(0) {}

      //! Pointer to document node associated with the event.
      property const misc::WeakRefServer *Node {
        const misc::WeakRefServer *get( void ) { return _node; }
        void set( const misc::WeakRefServer* n ) { _node=n; }
      };

    private:
      static DomainLogicEventArgs ^_empty = gcnew DomainLogicEventArgs();

      const misc::WeakRefServer *_node;
    };

    //! Delegate for events concerning a DomainLogic.
    //! @param sender is the source of the event
    //! @param args contains the event data
    typedef EventHandler<DomainLogicEventArgs^> DomainLogicEventHandler;

    //! Event triggered after creating a new document.
    event DomainLogicEventHandler ^DocumentCreated {
      void add(DomainLogicEventHandler^);
      void remove(DomainLogicEventHandler^);
      void raise(Object ^sender, DomainLogicEventArgs ^args);
    }

    //! Event triggered after attempting to load or merge a document from file
    event DomainLogicEventHandler ^DocumentLoaded {
      void add(DomainLogicEventHandler^);
      void remove(DomainLogicEventHandler^);
      void raise(Object ^sender, DomainLogicEventArgs ^args);
    }

    //! Event triggered after attempting to save document to a file.
    event DomainLogicEventHandler ^DocumentSaved {
      void add(DomainLogicEventHandler^);
      void remove(DomainLogicEventHandler^);
      void raise(Object ^sender, DomainLogicEventArgs ^args);
    }

    //! Event triggered after closing working document.
    event DomainLogicEventHandler ^DocumentClosed {
      void add(DomainLogicEventHandler^);
      void remove(DomainLogicEventHandler^);
      void raise(Object ^sender, DomainLogicEventArgs ^args);
    }

    //! Event triggered after the document was modified or the Document
    //! property changed.
    event DomainLogicEventHandler ^DocumentModified {
      void add(DomainLogicEventHandler^);
      void remove(DomainLogicEventHandler^);
      void raise(Object ^sender, DomainLogicEventArgs ^args);
    }

    //! Event triggered after a document node was possibly modified.
    //! The Node property of the event args point to modified node.
    event DomainLogicEventHandler ^NodeModified {
      void add(DomainLogicEventHandler^);
      void remove(DomainLogicEventHandler^);
      void raise(Object ^sender, DomainLogicEventArgs ^args);
    }

    //! Event triggered after a document node was deleted.
    //! The Node property of the event args holds the former adress of delete
    //! node.
    event DomainLogicEventHandler ^NodeDeleted {
      void add(DomainLogicEventHandler^);
      void remove(DomainLogicEventHandler^);
      void raise(Object ^sender, DomainLogicEventArgs ^args);
    }

    //! Event triggered after a document node was created through the
    //! DomainLogic instance (e.g. not when loading a document).
    event DomainLogicEventHandler ^NodeCreated {
      void add(DomainLogicEventHandler^);
      void remove(DomainLogicEventHandler^);
      void raise(Object ^sender, DomainLogicEventArgs ^args);
    }

    //! Event triggered when selected document node changed.
    //! The Node property of the event args points to the newly selected node
    //! (possibly 0 for none).
    event DomainLogicEventHandler ^SelectionChanged {
      void add(DomainLogicEventHandler^);
      void remove(DomainLogicEventHandler^);
      void raise(Object ^sender, DomainLogicEventArgs ^args);
    }

    //! Event triggered after the file name was changed.
    event DomainLogicEventHandler ^FileNameChanged {
      void add(DomainLogicEventHandler^);
      void remove(DomainLogicEventHandler^);
      void raise(Object ^sender, DomainLogicEventArgs ^args);
    }

    //@}

    //------------------------------------------------------------------------
    //! @name Operations on document
    //@{

    //! Create a new document.
    void NewDocument( void );

    //! Close current document if any and load file with the transmitted path.
    //! The FileName property is updated accordingly and corresponding events
    //! are triggered.
    //! @return true on success.
    bool OpenDocument( String ^FileName );

    //! Merge content of the file with the transmitted path to current
    //! document. Current value of the FileName property is kept and
    //! corresponding events are triggered.
    //! @return true on success.
    bool MergeDocument( String ^FileName );

    //! Close current document and trigger associated events.
    void CloseDocument( void );

    //! Save document to file with path FileName and trigger DocumentSaved
    //! event.
    //! @return true on success
    bool SaveDocument( void );

    //! Delete transmitted node and trigger associated events.
    //! @return true on success (some elements can't be deleted).
    bool DeleteNode( const misc::WeakRefServer *node );

    //! Create junction node.
    //! This will trigger NodeCreated event.
    //! @return pointer to created road, 0 on failure.
    const odr::Junction* CreateJunction(void);

    //! Create road node.
    //! This will trigger NodeCreated event.
    //! @param[in] junction   Junction where node is created, 0 for none.
    //! @return pointer to created road, 0 on failure.
    const odr::Road* CreateRoad( const odr::Junction* junction );

    //! Create line segment.
    //! This will trigger NodeCreated event.
    //! @param[in] geometry   Parent geometry of created primitive.
    //! @return pointer to create grometric primitive, 0 on failure.
    const odr::Line* CreateLine( const odr::Geometry* geometry );

    //! Create spiral.
    //! This will trigger NodeCreated event.
    //! @param[in] geometry   Parent geometry of created primitive.
    //! @return pointer to create grometric primitive, 0 on failure.
    const odr::Spiral* CreateSpiral( const odr::Geometry* geometry );

    //! Create arc.
    //! This will trigger NodeCreated event.
    //! @param[in] geometry   Parent geometry of created primitive.
    //! @return pointer to create grometric primitive, 0 on failure.
    const odr::Arc* CreateArc( const odr::Geometry* geometry );

    //! Create cubic polynomial curve.
    //! This will trigger NodeCreated event.
    //! @param[in] geometry   Parent geometry of created primitive.
    //! @return pointer to create grometric primitive, 0 on failure.
    const odr::Poly3* CreatePoly3( const odr::Geometry* geometry );

    //! Create lane section in transmitted lanes.
    //! This will trigger NodeCreated event.
    //! @param[in] lanes  Lanes object to be the parent of created section.
    //! @return pointer to created lane section, 0 on failure.
    const odr::LaneSection* CreateLaneSection( const odr::Lanes* lanes );

    //! Create lane in transmitted section at transmitted index.
    //! This will trigger NodeCreated event.
    //! @param[in] section  Lane section in which lane is created.
    //! @param[in] index    Index where lane is created, 0 is forbidden.
    //! @return pointer to created lane, 0 on failure.
    const odr::Lane* CreateLane( const odr::LaneSection* section, int index );

    //! Create lane width in transmitted lane.
    //! This will trigger NodeCreated event.
    //! @param[in] lane   Lane in which width record is added.
    //! @return pointer to created lane width, 0 on failure.
    const odr::LaneWidth* CreateLaneWidth( const odr::Lane* lane );

    //! Create elevation record in transmitted elevation profile.
    //! This will trigger NodeCreated event.
    //! @param[in] profile  Elevation profile in which record is created.
    //! @return pointer to created elevation record.
    const odr::Elevation* CreateElevation(
      const odr::ElevationProfile* profile );

    //! Create crossfall record in transmitted lateral profile.
    //! This will trigger NodeCreated event.
    //! @param[in] profile  Lateral profile in which record is created.
    //! @return pointer to created crossfall record.
    const odr::Crossfall* CreateCrossfall(
      const odr::LateralProfile* profile );

    //! Create superelevation record in transmitted lateral profile.
    //! This will trigger NodeCreated event.
    //! @param[in] profile  Lateral profile in which record is created.
    //! @return pointer to created superelevation record.
    const odr::Superelevation* CreateSuperelevation(
      const odr::LateralProfile* profile );

    //! Return transmitted road's geometric primitive closest to transmitted
    //! position.
    //! @param[in] road Road from which geometric primitive is taken
    //! @param[in] x    X-coordinate of point to test.
    //! @param[in] y    Y-coordinate of point to test.
    //! @param[in] tol  Maximum distance to primitive.
    //! @param[out] s   Receive S-coord offset of closest point along primitive
    //! @param[out] d   Reveive distance to primitive
    //! @return pointer to geometric primitive closest to position.
    const odr::GeometricPrimitive* GetRoadPrimitiveNear(
      const odr::Road *road, double x, double y, double tol,
      double% s, double %d );

    //! Return geometric primitive closest to transmitted position.
    //! @param[in] x   X-coordinate of point to test.
    //! @param[in] y   Y-coordinate of point to test.
    //! @param[in] tol Maximum distance to primitive.
    //! @param[out] s  Receive S-coord offset of closest point along primitive
    //! @return pointer to geometric primitive closest to position.
    const odr::GeometricPrimitive* GetPrimitiveNear(
      double x, double y, double tol, double% s );

    //! Pick node at transmitted coordinates, cycling through possibilities.
    //! @param[in] x    X-coordinate of point to pick.
    //! @param[in] y    Y-coordinate of point to pick.
    //! @param[in] tol  Maximum distance to geometric primitive.
    //! @return pointer to node, 0 if none.
    const misc::WeakRefServer* PickNode( double x, double y, double tol );

    //! Create a road with geometry constructed from a 2D sketch.
    //! @param[in] x      List of X-coordinates of points in sketch
    //! @param[in] y      List of Y-coordinates of points in sketch
    //! @param[in] cost   Cost of adding a single primitive
    //! @return pointer to created road or 0 on error.
    const odr::Road* SketchRoad(
      Collections::Generic::List<double>^ x,
      Collections::Generic::List<double>^ y,
      double cost );

    //! Create a road with geometry strictly passing through a collection of
    //! 2D points.
    //! @param[in] x    List of X-coordinates of passage points
    //! @param[in] y    List of Y-coordinates of passage points
    //! @return pointer to created road or 0 on error.
    const odr::Road* RoadThroughPoints(
      Collections::Generic::List<double>^ x,
      Collections::Generic::List<double>^ y );

  private:
    //! While trying to connect two postures with 3 clothoids of opposed
    //! curvature slopes, evaluate the distance between final position and
    //! target position for guessed total curve length and curvature slope.
    //! This method is internally used by RoadThroughPoints().
    //! @param[in]  Xi        X-coordinate of initial point
    //! @param[in]  Yi        Y-coordinate of initial point
    //! @param[in]  Hi        Initial heading (rad)
    //! @param[in]  Ci        Initial curvature
    //! @param[in]  Xf        X-coordinate of target point
    //! @param[in]  Yf        Y-coordinate of target point
    //! @param[in]  Hf        Final heading (rad)
    //! @param[in]  Cf        Final curvature
    //! @param[in]  S         Guessed total length for the 3 clothoids
    //! @param[in]  k         Guessed curvature slope of 1st and 3rd clothoids
    //! @param[out] spiral1   first clothoid
    //! @param[out] spiral2   second clothoid
    //! @param[out] spiral3   third clothoid
    //! @return distance to target point or QNAN when final heading and
    //!         curvature can't be reached with given S, k.
    double Evaluate3Spirals(
      double Xi, double Yi, double Hi, double Ci,
      double Xf, double Yf, double Hf, double Cf,
      double S, double k,
      odr::Spiral *spiral1, odr::Spiral *spiral2, odr::Spiral *spiral3 );

  public:
    //! Adapt primitive at road's end to reach transmitted coordinates.
    //! @param[out] road          Road to be prolongated
    //! @param[in]  x             Target X coordinate
    //! @param[in]  y             Target Y coordinate
    //! @param[in]  straightEnd   If true, curvature at end is zeroed
    //! @return distance of solution to target point.
    double ProlongateRoad(
      const odr::Road* road, double x, double y, bool straightEnd );

    //! Create connection road goind from one road end to another.
    //! @param[in] aRoad  First road to connect.
    //! @param[in] aEnd   True if contact point for first road is end.
    //! @param[in] aLanes Indices of lanes of road A to connect, index 0 means
    //!                   corresponding lane in road B is collapsed.
    //! @param[in] bRoad  Second road to connect.
    //! @param[in] bEnd   True if contact point for second road is end.
    //! @param[in] bLanes Indices of lanes of road B to connect, index 0 means
    //!                   corresponding lane in road A is collapsed.
    const odr::Road* ConnectRoads(
      const odr::Road* aRoad, bool aEnd, Generic::List<int> ^aLanes,
      const odr::Road* bRoad, bool bEnd, Generic::List<int> ^bLanes );

    //! Create geometric primitives in transmitted road to join transmitted
    //! postures.
    //! @param[in] road               road in which primitives are created
    //! @param[in] initialX           initial X-coordinate
    //! @param[in] initialY           initial Y-coordinate
    //! @param[in] initialHeading     initial heading
    //! @param[in] initialCurvature   initial curvature
    //! @param[in] finalX             final X-coordinate
    //! @param[in] finalY             final Y-coordinate
    //! @param[in] finalHeading       final heading
    //! @param[in] finalCurvature     final curvature
    //! @return true on success
    bool ConnectPosturesInRoad(
      odr::Road* road,
      double initialX, double initialY,
      double initialHeading, double initialCurvature,
      double finalX, double finalY,
      double finalHeading, double finalCurvature );

    //! Split a road in halves.
    //! Transmitted road is shortened to transmitted curve absciss and
    //! a new road is created with the remaining data.
    //! @param[in] road   Road two be split in two
    //! @param[in] s      Curve absciss of splitting point
    //! @return true on success.
    bool SplitRoad( const odr::Road* road, double s );

    //! Signature of node property editing methods.
    delegate void NodeEditDelegate( Object ^arg );

    //! @name Node property editing methods with boxed parameter.
    //@{
    void SetElementName( Object ^value );
    void SetRoadLength( Object ^value );
    void SetLaneSectionStart( Object ^value );
    void SetLaneSectionLength( Object ^value );
    void SetLaneWidthStartOffset( Object ^value );
    void SetLaneWidthLength( Object ^value );
    void SetLaneWidthA( Object ^value );
    void SetLaneWidthB( Object ^value );
    void SetLaneWidthC( Object ^value );
    void SetLaneWidthD( Object ^value );
    void SetGeometricPrimitiveStart( Object ^value );
    void SetGeometricPrimitiveLength( Object ^value );
    void SetGeometricPrimitiveX( Object ^value );
    void SetGeometricPrimitiveY( Object ^value );
    void SetGeometricPrimitiveHeadingDeg( Object ^value );
    void SetArcCurvature( Object ^value );
    void SetSpiralCurvatureStart( Object ^value );
    void SetSpiralCurvatureEnd( Object ^value );
    void SetPoly3A( Object ^value );
    void SetPoly3B( Object ^value );
    void SetPoly3C( Object ^value );
    void SetPoly3D( Object ^value );
    void SetElevationStart( Object ^value );
    void SetElevationLength( Object ^value );
    void SetElevationA( Object ^value );
    void SetElevationB( Object ^value );
    void SetElevationC( Object ^value );
    void SetElevationD( Object ^value );
    void SetCrossfallStart( Object ^value );
    void SetCrossfallLength( Object ^value );
    void SetCrossfallA( Object ^value );
    void SetCrossfallB( Object ^value );
    void SetCrossfallC( Object ^value );
    void SetCrossfallD( Object ^value );
    void SetSuperelevationStart( Object ^value );
    void SetSuperelevationLength( Object ^value );
    void SetSuperelevationA( Object ^value );
    void SetSuperelevationB( Object ^value );
    void SetSuperelevationC( Object ^value );
    void SetSuperelevationD( Object ^value );
    //@}

    //@}

	private:
    //! Reference to unique instance.
    static DomainLogic ^_Instance = nullptr;

    //! Default constructor: document pointer is null by default.
		DomainLogic(void);

    //! Pointer to weak reference to document.
    misc::WeakRef<odr::Document> *_document;

    //! Pointer to weak reference to selected node.
    misc::WeakRef<misc::WeakRefServer> *_selection;

    //! Path to working document file.
    String ^_fileName;

    //! @name Event-backing delegates
    //@{
    DomainLogicEventHandler ^_DocumentCreated;
    DomainLogicEventHandler ^_DocumentLoaded;
    DomainLogicEventHandler ^_DocumentSaved;
    DomainLogicEventHandler ^_DocumentClosed;
    DomainLogicEventHandler ^_DocumentModified;
    DomainLogicEventHandler ^_NodeModified;
    DomainLogicEventHandler ^_NodeDeleted;
    DomainLogicEventHandler ^_NodeCreated;
    DomainLogicEventHandler ^_SelectionChanged;
    DomainLogicEventHandler ^_FileNameChanged;
    //@}

    //! Class for raising events in the UI thread after already-enqueued
    //! events where processed.
    ref class EventRaiser {
    public:
      EventRaiser( DomainLogicEventHandler ^toRaise,
                   DomainLogic ^sender,
                   DomainLogicEventArgs ^args );

    private:
      void DoInvoke( Object ^s, EventArgs ^a );

      EventHandler ^_handler;
      DomainLogicEventHandler ^_toRaise;
      DomainLogic ^_sender;
      DomainLogicEventArgs ^_args;
    };

    //! Cause transmitted event to be raised in the UI thread after
    //! already-enqueued events where processed.
    void AsyncRaise( DomainLogicEventHandler ^toRaise,
                     DomainLogicEventArgs ^args );

    //! Raise a NodeModified and DocumentModified events for selected node.
    void RaiseNodeModified( void );

    //! Re-compute XY coords of transmitted geometric primitives.
    void UpdateGeometricPrimitives( odr::Geometry* geo );

	};

  // Make nested classes accessible in the scope of the roads namespace...
  typedef DomainLogic::DomainLogicEventArgs DomainLogicEventArgs;
  typedef DomainLogic::DomainLogicEventHandler DomainLogicEventHandler;
}
