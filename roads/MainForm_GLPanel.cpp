//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file MainForm_GLPanel.cpp
//! \brief Implementation of roads::MainForm's OpenGL panel-related methods.

#include "stdafx.h"
#include "MainForm.h"
#include "Render.h"
#include "ToolPolicy.h"

namespace roads {

  bool MainForm::GLPanelInit()
  {
    _glHDC = NULL;
    _glHGLRC = NULL;

    // Retrieve handle of device context...
    _glHDC = ::GetDC( (HWND)_glPanel->Handle.ToPointer() );

    PIXELFORMATDESCRIPTOR pfd = { 
      sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd 
      1,                             // version number 
      PFD_DRAW_TO_WINDOW |           // support window 
      PFD_SUPPORT_OPENGL |           // support OpenGL 
      PFD_DOUBLEBUFFER,              // double buffered 
      PFD_TYPE_RGBA,                 // RGBA type 
      24,                            // 24-bit color depth 
      0, 0, 0, 0, 0, 0,              // color bits ignored 
      0,                             // no alpha buffer 
      0,                             // shift bit ignored 
      0,                             // no accumulation buffer 
      0, 0, 0, 0,                    // accum bits ignored 
      24,                            // 24-bit z-buffer     
      0,                             // no stencil buffer 
      0,                             // no auxiliary buffer 
      PFD_MAIN_PLANE,                // main layer 
      0,                             // reserved 
      0, 0, 0                        // layer masks ignored 
    }; 
    
    GLint  iPixelFormat; 
     
    // Get the device context's best, available pixel format match...
    if( (iPixelFormat = ::ChoosePixelFormat( _glHDC, &pfd ) ) == 0 ) {
      MessageBox::Show("ChoosePixelFormat Failed");
      return false;
    }
     
    // Make that match the device context's current pixel format...
    if( ::SetPixelFormat( _glHDC, iPixelFormat, &pfd ) == FALSE ) {
      MessageBox::Show("SetPixelFormat Failed");
      return false;
    }

    // Create OpenGL context...
    if( (_glHGLRC = wglCreateContext(_glHDC)) == NULL ) {
      MessageBox::Show("wglCreateContext Failed");
      return false;
    }
    
    // Make it the current context...
    if( ::wglMakeCurrent(_glHDC, _glHGLRC) == NULL ) {
      MessageBox::Show("wglMakeCurrent Failed");
      return false;
    }

    // Set up OpenGL renderer state...
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );
    glEnable( GL_SMOOTH );
    glDisable( GL_DITHER );
    _glCenterX = 0.0;
    _glCenterY = 0.0;
    _glSpreadX = 100.0;
    _glSpreadY = _glSpreadX * (double)_glPanel->Height
                  / (double)_glPanel->Width;
    _glListID = glGenLists( 1 );
    _glSelListID = glGenLists( 1 );
    _glListNdUpdate = true;
    _glSelListNdUpdate = true;

    // Bind event handlers...
    _glPanel->Paint += Adapter<PaintEventArgs^,PaintEventHandler>(
      gcnew EventHandler(this, &MainForm::GLPanelPaint) );

    DomainLogic::Instance->DocumentCreated += Adapter<DomainLogicEventArgs^>(
      gcnew EventHandler(this,&MainForm::GLPanelReset) );

    DomainLogic::Instance->DocumentCreated += gcnew DomainLogicEventHandler(
      this, &MainForm::GLPanelDocumentModified );

    DomainLogic::Instance->DocumentLoaded += Adapter<DomainLogicEventArgs^>(
      gcnew EventHandler(this,&MainForm::GLPanelReset) );

    DomainLogic::Instance->DocumentLoaded += gcnew DomainLogicEventHandler(
      this, &MainForm::GLPanelDocumentModified );

    DomainLogic::Instance->DocumentModified += gcnew DomainLogicEventHandler(
      this, &MainForm::GLPanelDocumentModified );

    DomainLogic::Instance->SelectionChanged += gcnew DomainLogicEventHandler(
      this, &MainForm::GLPanelSelectionChanged );

    MouseWheel += gcnew MouseEventHandler(this,&MainForm::GLPanelMouseWheel);

    return true;
  }

  //--------------------------------------------------------------------------
  void MainForm::GLPanelResize( Object ^sender, EventArgs ^e )
  {
    _glSpreadY = _glSpreadX * (double)_glPanel->Height 
                  / (double)_glPanel->Width;
    GLPanelPaint(this,EventArgs::Empty);
  }

  //--------------------------------------------------------------------------
  void MainForm::GLPanelDocumentModified(
    Object ^sender, DomainLogicEventArgs ^args )
  {
    _glListNdUpdate = true;
    _glSelListNdUpdate = true;
    GLPanelPaint( sender, args );
  }

  //--------------------------------------------------------------------------
  void MainForm::GLPanelSelectionChanged(
    Object ^sender, DomainLogicEventArgs ^args )
  {
    _glSelListNdUpdate = true;
    GLPanelPaint( sender, args );
  }

  //--------------------------------------------------------------------------
  void MainForm::GLPanelPaint( Object ^sender, EventArgs ^e )
  {
    if( !_glHGLRC ) return;

    // Switch to appropriate render context...
    ::wglMakeCurrent( _glHDC, _glHGLRC );

    // Clear viewport...
    glViewport( 0, 0,
                _glPanel->Width,
                _glPanel->Height );
    glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Leave if no document...
    if( !DomainLogic::Instance->Document ) {
      ::SwapBuffers(_glHDC);
      return;
    }

    // Set up projection...
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( _glCenterX-_glSpreadX, _glCenterX+_glSpreadX,
             _glCenterY-_glSpreadY, _glCenterY+_glSpreadY,
             -1.0, 1.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    // Draw grid...
    double MaxScaleLog = log( _glSpreadY) / log(10.0);
    double MinScaleLog =
      log(_glSpreadX*3.0/_glPanel->Width) / log(10.0);

    glLineWidth( 1.0f );
    glBegin( GL_LINES );
    for( int i=(int)ceil(MinScaleLog) ; i<=(int)ceil(MaxScaleLog) ; ++i ) {
      double scale = pow( 10.0, double(i) );
      if( scale<0.001 ) continue;
      float intensity =
        0.2f + 0.6f * float(i-MinScaleLog) / float(MaxScaleLog-MinScaleLog);
      glColor3f( intensity, intensity, intensity );
      double x_min = scale * ceil( (_glCenterX-_glSpreadX)/scale );
      double x_max = scale * floor( (_glCenterX+_glSpreadX)/scale );
      for( double x=x_min ; x<=x_max ; x+=scale ) {
        glVertex2d( x, _glCenterY-_glSpreadY );
        glVertex2d( x, _glCenterY+_glSpreadY );
      }
      double y_min = scale * ceil( (_glCenterY-_glSpreadY)/scale );
      double y_max = scale * floor( (_glCenterY+_glSpreadY)/scale );
      for( double y=y_min ; y<=y_max ; y+=scale ) {
        glVertex2d( _glCenterX-_glSpreadX, y );
        glVertex2d( _glCenterX+_glSpreadX, y );
      }
    }
    glEnd();
    glLineWidth( 2.0f );
    glBegin( GL_LINES );
      glColor4f( 1.0f, 0.0f, 0.0f, 0.7f );
      glVertex2d( _glCenterX-_glSpreadX, 0.0 );
      glVertex2d( _glCenterX+_glSpreadX, 0.0 );
      glColor4f( 0.0f, 1.0f, 0.0f, 0.7f );
      glVertex2d( 0.0, _glCenterY-_glSpreadY );
      glVertex2d( 0.0, _glCenterY+_glSpreadY );
    glEnd();

    // Draw backdrops...
    glEnable( GL_TEXTURE_2D );
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glEnable( GL_BLEND );
    glColor3f( 1.0f, 1.0f, 1.0f );
    for each( Backdrop ^bd in _backdrops ) {
      if( bd->Visible ) {
        glBindTexture( GL_TEXTURE_2D, bd->TextureID );
        glBegin( GL_QUADS );
        glTexCoord2f( 0.0f, 0.0f );
        glVertex2d( bd->Left, bd->Top );
        glTexCoord2f( 0.0f, 1.0f );
        glVertex2d( bd->Left, bd->Bottom );
        glTexCoord2f( 1.0f, 1.0f );
        glVertex2d( bd->Right, bd->Bottom );
        glTexCoord2f( 1.0f, 0.0f );
        glVertex2d( bd->Right, bd->Top );
        glEnd();
      }
    }
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable( GL_TEXTURE_2D );

    // Draw mesh...
    if( _mesh ) {
      glLineWidth( 1.0f );
      glPointSize( 4.0f );
      unsigned int nbVertices = _mesh->nbVertices();
      const double *vertexData = _mesh->vertexData();
      //glEnableClientState( GL_VERTEX_ARRAY );
      //glVertexPointer( _mesh->nbVertices(), GL_DOUBLE, 16, vertexData );
      unsigned int nbTriangles = _mesh->nbTriangles();
      const unsigned int *triangleData = _mesh->triangleData();
      const unsigned int vsize = asphalt::Mesh::VERTEX_SIZE;
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glEnable( GL_BLEND );
      glColor4f( 0.0f, 0.5f, 1.0f, 0.25f );
      glBegin( GL_TRIANGLES );
        for( unsigned int i=0 ; i<nbTriangles ; ++i ) {
          glVertex2dv( vertexData + triangleData[(i<<2)+1]*vsize );
          glVertex2dv( vertexData + triangleData[(i<<2)+2]*vsize );
          glVertex2dv( vertexData + triangleData[(i<<2)+3]*vsize );
        }
      glEnd();
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      glDisable( GL_BLEND );
      glColor3f( 0.0f, 0.0f, 0.0f );
      glBegin( GL_TRIANGLES );
        for( unsigned int i=0 ; i<nbTriangles ; ++i ) {
          glVertex2dv( vertexData + triangleData[(i<<2)+1]*vsize );
          glVertex2dv( vertexData + triangleData[(i<<2)+2]*vsize );
          glVertex2dv( vertexData + triangleData[(i<<2)+3]*vsize );
        }
      glEnd();
      glColor3f( 1.0f, 0.0f, 0.0f );
      glBegin( GL_LINES );
        for( unsigned int i=0 ; i<nbVertices ; ++i ) {
          glVertex2dv( vertexData + i*vsize );
          glVertex2d( vertexData[i*vsize+0] + vertexData[i*vsize+10],
                      vertexData[i*vsize+1] + vertexData[i*vsize+11] );
        }
      glEnd();
      glColor3f( 0.0f, 1.0f, 0.0f );
      glBegin( GL_LINES );
        for( unsigned int i=0 ; i<nbVertices ; ++i ) {
          glVertex2dv( vertexData + i*vsize );
          glVertex2d( vertexData[i*vsize+0] + vertexData[i*vsize+13],
                      vertexData[i*vsize+1] + vertexData[i*vsize+14] );
        }
      glEnd();
      glColor3f( 0.0f, 0.0f, 0.0f );
      glBegin( GL_POINTS );
        for( unsigned int i=0 ; i<nbVertices ; ++i )
          glVertex2dv( vertexData + i*vsize );
      glEnd();
      //glDisableClientState( GL_VERTEX_ARRAY );
    }

    // Draw roads...
    if( _glListNdUpdate ) {
      _glListNdUpdate = false;
      glNewList( _glListID, GL_COMPILE_AND_EXECUTE );

      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glEnable( GL_BLEND );
      glDisable( GL_CULL_FACE );
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      glColor4f( 1.0f, 0.6f, 0.2f, 0.5f );
      
      misc::Enumerator<const odr::Element*> *elements =
        DomainLogic::Instance->Document->enumerate();
      while( const odr::Element *el = elements->item() ) {
        elements->moveNext();
        const odr::Road *road = dynamic_cast<const odr::Road*>(el);
        if(!road) continue;

        Render::FilledRoad(road);
      }
      delete elements;

      // Draw lane separations...
      glColor4f( 1.0f, 1.0f, 1.0f, 0.8f );
      glLineStipple( 1, 0XCCCC );
      glLineWidth( 0.7f );
      glEnable( GL_LINE_STIPPLE );
      elements = DomainLogic::Instance->Document->enumerate();
      while( const odr::Element *el = elements->item() ) {
        elements->moveNext();
        const odr::Road *road = dynamic_cast<const odr::Road*>(el);
        if( !road ) continue;

        misc::Enumerator<const odr::LaneSection*> *sectionEnum =
          road->lanes()->enumerate();
        const odr::LaneSection *section = 0;
        while( sectionEnum && (section=sectionEnum->item()) ) {
          sectionEnum->moveNext();
          for( int i=1-(int)section->countRight() ;
               i<(int)section->countLeft() ; ++i )
            if( i )
              Render::LaneOuterBorder( section->item(i) );
        }
        delete sectionEnum;
      }
      delete elements;
      glDisable( GL_LINE_STIPPLE );

      // Draw road-bearing geometric primitives...
      glLineWidth( 1.0f );
      glPointSize( 4.0f );
      glColor4f( 0.0f, 0.7f, 1.0f, 0.8f );
      elements = DomainLogic::Instance->Document->enumerate();
      while( const odr::Element *el = elements->item() ) {
        if( el->type()==odr::Element::T_ROAD ) {
          const odr::Road *road = dynamic_cast<const odr::Road*>(el);
          misc::Enumerator<const odr::GeometricPrimitive*> *primitives =
            road->geometry()->enumerate();
          while( const odr::GeometricPrimitive *prim = primitives->item() ) {
            primitives->moveNext();
            Render::GeometricPrimitive( prim );
          }
          delete primitives;
        }

        elements->moveNext();
      }
      delete elements;
      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

      glEndList();
    }
    else
      glCallList( _glListID );

    // Draw selected element...
    if( _glSelListNdUpdate ) {
      _glSelListNdUpdate = false;
      glNewList( _glSelListID, GL_COMPILE_AND_EXECUTE );

      glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
      glEnable( GL_BLEND );
      glDisable( GL_CULL_FACE );
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      glEnable( GL_BLEND );
      glColor4f( 1.0f, 1.0f, 0.0f, 0.5f );
      glLineWidth( 0.8f );

      const misc::WeakRefServer *selected = DomainLogic::Instance->SelectedNode;

      if( const odr::Road *road=dynamic_cast<const odr::Road*>(selected) ) {
        Render::FilledOutlinedRoad(road);
      }

      if( const odr::Lanes *lanes=dynamic_cast<const odr::Lanes*>(selected) ) {
        Render::FilledOutlinedRoad(lanes->parentRoad());
      }

      if( const odr::LateralProfile *lat=dynamic_cast<const odr::LateralProfile*>(selected) ) {
        Render::FilledOutlinedRoad(lat->parentRoad());
      }

      if( const odr::ElevationProfile *elev=dynamic_cast<const odr::ElevationProfile*>(selected) ) {
        Render::FilledOutlinedRoad(elev->parentRoad());
      }

      else if( const odr::LaneSection *section=dynamic_cast<const odr::LaneSection*>(selected) ) {
        Render::FilledOutlinedRoad( section->parentLanes()->parentRoad(),
                                    section->start(),
                                    section->start()+section->length() );
      }

      else if( const odr::Lane *lane=dynamic_cast<const odr::Lane*>(selected) ) {
        Render::FilledOutlinedLane(lane);
      }

      else if( const odr::LaneWidth *width=dynamic_cast<const odr::LaneWidth*>(selected) ) {
        Render::FilledOutlinedLane( width->parentLane(),
                                    width->startOffset(),
                                    width->startOffset()+width->length() );
      }

      else if( const odr::Elevation *elev=dynamic_cast<const odr::Elevation*>(selected) ) {
        Render::FilledOutlinedRoad( elev->parentProfile()->parentRoad(),
                                    elev->start(),
                                    elev->start()+elev->length() );
      }

      else if( const odr::Crossfall *cross=dynamic_cast<const odr::Crossfall*>(selected) ) {
        Render::FilledOutlinedRoad( cross->parentProfile()->parentRoad(),
                                    cross->start(),
                                    cross->start()+cross->length() );
      }

      else if( const odr::Superelevation *super=dynamic_cast<const odr::Superelevation*>(selected) ) {
        Render::FilledOutlinedRoad( super->parentProfile()->parentRoad(),
                                    super->start(),
                                    super->start()+super->length() );
      }

      else if( const odr::Geometry *geometry=dynamic_cast<const odr::Geometry*>(selected) ) {
        glLineWidth( 2.0f );
        glColor4f( 1.0f, 1.0f, 0.0f, 1.0f );
        misc::Enumerator<const odr::GeometricPrimitive*> *primitives = geometry->enumerate();
        while( const odr::GeometricPrimitive* prim=primitives->item() ) {
          Render::GeometricPrimitive( prim );
          primitives->moveNext();
        }
        delete primitives;
      }

      else if( const odr::GeometricPrimitive *prim=dynamic_cast<const odr::GeometricPrimitive*>(selected) ) {
        glLineWidth( 2.0f );
        glColor4f( 1.0f, 1.0f, 0.0f, 1.0f );
        Render::GeometricPrimitive( prim );
      }

      glEndList();
    }
    else
      glCallList( _glSelListID );

    // Draw tool-specific stuff...
    _currentToolPolicy->OnGLPanelPaint( this );

    // Swap buffers...
    ::SwapBuffers(_glHDC);
  }

  //--------------------------------------------------------------------------
  void MainForm::GLPanelReset( Object ^sender, EventArgs ^args )
  {
    if( DomainLogic::Instance->Document ) {
      const odr::Header *header = DomainLogic::Instance->Document->header();
      _glCenterX = 0.5*( header->east()+header->west() );
      _glCenterY = 0.5*( header->north()+header->south() );
      _glSpreadX = 0.5*( header->east()-header->west() );
      _glSpreadY = _glSpreadX * (double)_glPanel->Height
                    / (double)_glPanel->Width;
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::GLPanelMouseDown( Object ^sender, MouseEventArgs ^e )
  {
    if( !DomainLogic::Instance->Document ) return;

    GLPanelX = _glCenterX + 2.0 * _glSpreadX *
      ( double(e->Location.X)/_glPanel->Width - 0.5 );
    GLPanelY = _glCenterY + 2.0 * _glSpreadY *
      ( 0.5 - double(e->Location.Y)/_glPanel->Height );

    _glPanel->Focus();
    switch( e->Button ) {
    case Forms::MouseButtons::Left:
      _currentToolPolicy->OnLMBDown( this, e );
      break;

    case Forms::MouseButtons::Right:
      _glPanel->Capture = true;
      _glPanel->Cursor = Windows::Forms::Cursors::NoMove2D;
      break;

    case Forms::MouseButtons::Middle:
      _glPanel->Capture = true;
      _glPanel->Cursor = Windows::Forms::Cursors::NoMoveVert;
      break;
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::GLPanelMouseMove( Object ^sender, MouseEventArgs ^e )
  {
    if( !DomainLogic::Instance->Document ) return;

    static Drawing::Size lastPos;
    Drawing::Point move = e->Location - lastPos;
    lastPos = Drawing::Size(e->Location);

    GLPanelX = _glCenterX + 2.0 * _glSpreadX *
      ( double(e->Location.X)/_glPanel->Width - 0.5 );
    GLPanelY = _glCenterY + 2.0 * _glSpreadY *
      ( 0.5 - double(e->Location.Y)/_glPanel->Height );

    if( MouseButtons == Windows::Forms::MouseButtons::Left ) {
      _currentToolPolicy->OnLMBDrag( this, e );
    }

    else if( MouseButtons == Windows::Forms::MouseButtons::Right ) {
      _glCenterX -= move.X * 2.0f * _glSpreadX / _glPanel->Width;
      _glCenterY += move.Y * 2.0f * _glSpreadX / _glPanel->Width;
      Drawing::Point pos = Cursor->Position;
      if( e->X < 0 ) {
        pos.X += _glPanel->Width;
        lastPos.Width += _glPanel->Width;
      }
      else if( e->X >= _glPanel->Width ) {
        pos.X -= _glPanel->Width;
        lastPos.Width -= _glPanel->Width;
      }
      if( e->Y < 0 ) {
        pos.Y += _glPanel->Height;
        lastPos.Height += _glPanel->Height;
      }
      else if( e->Y >= _glPanel->Height ) {
        pos.Y -= _glPanel->Height;
        lastPos.Height -= _glPanel->Height;
      }
      Cursor->Position = pos;
      GLPanelPaint(this,EventArgs::Empty);
    }

    else if( MouseButtons == Windows::Forms::MouseButtons::Middle ) {
      _glSpreadX *= expf( move.Y * 4.0f / _glPanel->Height );
      if( _glSpreadX < 0.001f ) _glSpreadX=0.001f;
      if( _glSpreadX > 10000.0f ) _glSpreadX=10000.0f;
      _glSpreadY = _glSpreadX * (double)_glPanel->Height
                    / (double)_glPanel->Width;
      Drawing::Point pos = Cursor->Position;
      if( e->Y < 0 ) {
        pos.Y += _glPanel->Height;
        lastPos.Height += _glPanel->Height;
      }
      else if( e->Y >= _glPanel->Height ) {
        pos.Y -= _glPanel->Height;
        lastPos.Height -= _glPanel->Height;
      }
      Cursor->Position = pos;
      GLPanelPaint(this,EventArgs::Empty);
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::GLPanelMouseUp( Object ^sender, MouseEventArgs ^e )
  {
    if( !DomainLogic::Instance->Document ) return;

    GLPanelX = _glCenterX + 2.0 * _glSpreadX *
      ( double(e->Location.X)/_glPanel->Width - 0.5 );
    GLPanelY = _glCenterY + 2.0 * _glSpreadY *
      ( 0.5 - double(e->Location.Y)/_glPanel->Height );

    if( e->Button == Forms::MouseButtons::Left )
      _currentToolPolicy->OnLMBUp( this, e );

    if( MouseButtons == Windows::Forms::MouseButtons::None ) {
      _glPanel->Capture = false;
      _glPanel->Cursor = _currentToolPolicy->Cursor;
    }
  }

  //--------------------------------------------------------------------------
  void MainForm::GLPanelMouseWheel( Object ^sender, MouseEventArgs ^e )
  {
    if( !DomainLogic::Instance->Document ) return;

    if( e->Delta > 0 )
      _glSpreadX *= 0.8f;
    else
      _glSpreadX *= 1.25f;
    if( _glSpreadX < 0.001f ) _glSpreadX=0.001f;
    if( _glSpreadX > 10000.0f ) _glSpreadX=10000.0f;
    _glSpreadY = _glSpreadX * (double)_glPanel->Height
                  / (double)_glPanel->Width;

    GLPanelX = _glCenterX + 2.0 * _glSpreadX *
      ( double(e->Location.X)/_glPanel->Width - 0.5 );
    GLPanelY = _glCenterY + 2.0 * _glSpreadY *
      ( 0.5 - double(e->Location.Y)/_glPanel->Height );

    GLPanelPaint(this,EventArgs::Empty);
  }

  //--------------------------------------------------------------------------
  void MainForm::GLPanelKeyDown( Object ^sender, PreviewKeyDownEventArgs ^e )
  {
    if( !DomainLogic::Instance->Document ) return;

    const misc::WeakRefServer* node = DomainLogic::Instance->SelectedNode;
    if( node && e->KeyData==Keys::Delete )
      DomainLogic::Instance->DeleteNode( node );
  }

} // namespace roads
