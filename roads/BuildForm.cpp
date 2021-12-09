//----------------------------------------------------------------------------
// roads (c)2008-2010 LIVIC
// developed by Emmanuel Raulo-Kumagai, orange IT&L@bs
// <emmanuel.raulo@orange-ftgroup.com>
//----------------------------------------------------------------------------

//! \file BuildForm.cpp
//! \brief Implementation of roads::BuildForm's methods.

#include "StdAfx.h"
#include "BuildForm.h"
#include <asphalt/asphalt.h>
#include <asphalt/math.h>
#include <asphalt/vecop.h>
#include <vector>
#include <list>
#include <fstream>
#include <misc/DllHelper.h>
#include <misc/WeakRefServerImpl.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::IO;
using namespace System::Drawing;
using namespace misc;
using namespace odr;
using namespace asphalt;
using namespace roads;


//----------------------------------------------------------------------------
#ifdef _MSC_VER 
# pragma warning( push )
# pragma warning( disable : 4250 )
#endif
namespace roads {
  class MyMesh :
    public ::misc::DllImpl<::asphalt::Mesh>,
    public ::misc::WeakRefServerImpl
  {
  public:
    MyMesh( void );
    ~MyMesh();

    //! @name Methods inherited from asphalt::Mesh
    //@{
    unsigned int ASPHALT_CALL nbVertices( void ) const;
    const double* ASPHALT_CALL vertexData( void ) const;
    unsigned int ASPHALT_CALL nbMaterials( void ) const;
    const char* ASPHALT_CALL materialName( unsigned int index ) const;
    unsigned int ASPHALT_CALL nbTriangles( void ) const;
    const unsigned int* ASPHALT_CALL triangleData( void ) const;
    //@}

    std::vector<double>       vertices;
    std::vector<unsigned int> triangles;
    std::vector<std::string>  materials;
  };
}
#ifdef _MSC_VER 
# pragma warning( pop )
#endif


//----------------------------------------------------------------------------
Void BuildForm::Build( Object ^sender, EventArgs ^e )
{
  _optionsGroup->Enabled    = false;
  _outputGroup->Enabled     = false;
  _terrainCheckbox->Enabled = false;
  _terrainGroup->Enabled    = false;
  _buildBtn->Text = L"abort";
  _buildBtn->Click -= gcnew System::EventHandler(this, &BuildForm::Build);
  _buildBtn->Click += gcnew System::EventHandler(this, &BuildForm::Abort);
  _progressBar->Value = 0;
  _progressTimer->Enabled = true;
  _buildThread =
    gcnew Thread( gcnew ThreadStart(this,&BuildForm::BuildThreadStart) );
  _buildThread->Start();
}


//----------------------------------------------------------------------------
Void BuildForm::Abort( Object ^sender, EventArgs ^e )
{
  if( _buildThread ) {
    _builder->abort();
    _buildThread->Join();
    BuildEnd();
  }
}


//----------------------------------------------------------------------------
Void BuildForm::BuildEnd()
{
  delete _buildThread;
  _buildThread=nullptr;
  _optionsGroup->Enabled    = true;
  _outputGroup->Enabled     = true;
  _terrainCheckbox->Enabled = true;
  _terrainGroup->Enabled    = _terrainCheckbox->Checked;
  _buildBtn->Text = L"GO! ";
  _buildBtn->Click -= gcnew System::EventHandler(this, &BuildForm::Abort);
  _buildBtn->Click += gcnew System::EventHandler(this, &BuildForm::Build);
  _progressTimer->Enabled = false;
  _progressBar->Value = 0;
  if( _mesh ) {
    _vertexCount->Text   = String::Format("{0}", _mesh->nbVertices() );
    _triangleCount->Text = String::Format("{0}", _mesh->nbTriangles() );
  }
  else {
    _vertexCount->Text   = "0";
    _triangleCount->Text = "0";
  }
}


//----------------------------------------------------------------------------
Void BuildForm::UpdateProgressBar( Object ^sender, EventArgs ^e )
{
  _progressBar->Value = _builder->progress();
  if( _buildThread!=nullptr && !_buildThread->IsAlive )
    BuildEnd();
}


//----------------------------------------------------------------------------
Void BuildForm::Closing( Object ^sender, FormClosingEventArgs ^e )
{
  if( _buildThread ) {
    if( MessageBox::Show( "Abort?",
                          "Operation in progress. Do you wish to abort?",
                          MessageBoxButtons::YesNo,
                          MessageBoxIcon::Asterisk,
                          MessageBoxDefaultButton::Button2 )
          == System::Windows::Forms::DialogResult::Yes )
    {
      Abort(nullptr,nullptr);
    }
    else
      e->Cancel = true;
  }
}


//----------------------------------------------------------------------------
Void BuildForm::BuildThreadStart()
{
  if( _mesh ) delete _mesh;
  _mesh = 0;

  // If terrain is enabled, build terrain mesh...
  MyMesh terrain;
  if( _terrainCheckbox->Checked ) try {
    // Retrieve terrain parameters...
    double north = (double)_northBox->Value,
           south = (double)_southBox->Value,
           east  = (double)_eastBox->Value,
           west  = (double)_westBox->Value,
           grid  = (double)_gridBox->Value,
           hmin  = (double)_minHeightBox->Value,
           hmax  = (double)_maxHeightBox->Value;

    // Calculate number of vertices in both directions...
    unsigned int
      nbVerticesNS = 1 + (unsigned int)ceil( abs(north-south) / grid ),
      nbVerticesEW = 1 + (unsigned int)ceil( abs(east-west) / grid );

    // Open image file...
    Image ^img = Image::FromFile(_heightPathBox->Text);
    Bitmap ^bmp = safe_cast<Bitmap^>(
      img->GetThumbnailImage(nbVerticesEW,nbVerticesNS,nullptr,IntPtr(0)) );
    if( !bmp ) return;

    // Allocate data...
    terrain.vertices.resize( 16*nbVerticesNS*nbVerticesEW );
    terrain.triangles.resize( 8*(nbVerticesNS-1)*(nbVerticesEW-1) );
    terrain.materials.push_back( "terrain" );

    // Setup vertex and faces...
    for( unsigned int vindex=0, findex=0, j=0 ; j<nbVerticesNS ; ++j ) {
      for( unsigned int i=0 ; i<nbVerticesEW ; ++i, ++vindex, findex+=2 ) {
        double *v =
          &(*terrain.vertices.begin()) + vindex*asphalt::Mesh::VERTEX_SIZE;
        // Vertex coordinates...
        v[0] = west  + (east-west)   * (double)i / (nbVerticesEW-1);
        v[1] = south + (north-south) * (double)j / (nbVerticesNS-1);
        Color col = bmp->GetPixel(i,j);
        v[2] = hmin + (hmax-hmin) * ( (double)col.R + col.G + col.B ) / 765.0;
        // Texture coordinates...
        v[6] = v[0];
        v[7] = v[1];
        // Lightmap coordinates...
        v[8] = (double)i / (nbVerticesEW-1);
        v[9] = (double)i / (nbVerticesNS-1);
        // Faces...
        if( i<nbVerticesEW-1 && j<nbVerticesNS-1 ) {
          unsigned int *f = &(*terrain.triangles.begin()) + (findex<<2);
          f[0] = 0;
          f[1] = vindex;
          f[2] = vindex+1;
          f[3] = vindex+nbVerticesEW;
          f[4] = 0;
          f[5] = vindex+1;
          f[6] = vindex+nbVerticesEW+1;
          f[7] = vindex+nbVerticesEW;
        }
      }
      findex -= 2;
    }

    // Setup vertex normals and U-V tangents...
    for( unsigned int vindex=0, j=0 ; j<nbVerticesNS ; ++j )
      for( unsigned int i=0 ; i<nbVerticesEW ; ++i, ++vindex ) {
        const unsigned int vsize = asphalt::Mesh::VERTEX_SIZE;
        double *v = &(*terrain.vertices.begin()) + vindex*vsize;
        double *vni = ( i<nbVerticesEW-1 ) ? v+16 : v,
               *vpi = ( i>0 ) ? v-16 : v,
               *vnj = ( j<nbVerticesNS-1 ) ? v+nbVerticesEW*vsize : v,
               *vpj = ( j>0 ) ? v-nbVerticesEW*vsize : v;

        double *tu = v+10;
        tu[0] = 1.0;
        tu[1] = 0.0;
        tu[2] = (vni[2]-vpi[2]) / (vni[0]-vpi[0]);
        asphalt::mk_vecop<3>(tu) *= 1.0 / asphalt::mk_vecop<3>(tu).norm2();

        double *tv = v+13;
        tv[0] = 0.0;
        tv[1] = 1.0;
        tv[2] = (vnj[2]-vpj[2]) / (vnj[1]-vpj[1]);
        asphalt::mk_vecop<3>(tv) *= 1.0 / asphalt::mk_vecop<3>(tv).norm2();

        double *n = v+3;
        asphalt::mk_vecop<3>(n) =
          asphalt::mk_vecop<3>(tu) ^ asphalt::mk_vecop<3>(tv);
      }
  }
  catch(...) {
    return;
  }

  // Build road mesh...
  _mesh = _builder->build(
    _odrDocument,
    (double)_maxEdgeAngleBox->Value * asphalt::math<double>::pi_by_180,
    (double)_maxSurfAngleBox->Value * asphalt::math<double>::pi_by_180,
    (double)_minEdgeLengthBox->Value,
    (double)_maxEdgeLengthBox->Value,
    (double)_maxRatioBox->Value,
    _terrainCheckbox->Checked ? &terrain : 0,
    (double)_terrainDistanceBox->Value );

  // If build was successful, save file...
  if( _mesh ) {
    using namespace System::Runtime::InteropServices;
    char *tmp = (char*)(void*)Marshal::StringToHGlobalAnsi(_pathBox->Text);
    std::ofstream out( tmp );
    Marshal::FreeHGlobal( IntPtr((void*)tmp) );

    if( !(out.bad() || out.fail()) ) {
      unsigned int nbVertices = _mesh->nbVertices();
      const double *vertexData = _mesh->vertexData();
      for( unsigned int i=0 ; i<nbVertices ; ++i, vertexData+=16 )
        out
          << "v " << vertexData[0] << " " << vertexData[1] << " "
          << vertexData[2] << std::endl
          << "vn " << vertexData[3] << " " << vertexData[4] << " "
          << vertexData[5] << std::endl
          << "vt " << vertexData[6] << " " << vertexData[7] << std::endl;

      unsigned int curMaterial = std::numeric_limits<unsigned int>::max();
      unsigned int nbTriangles = _mesh->nbTriangles();
      const unsigned int *triangleData = _mesh->triangleData();
      for( unsigned int i=0 ; i<nbTriangles ; ++i, triangleData+=4 ) {
        unsigned int m=triangleData[0],
                     a=triangleData[1]+1,
                     b=triangleData[2]+1,
                     c=triangleData[3]+1;
        if( m!=curMaterial ) {
          out << "g " << _mesh->materialName(m) << std::endl;
          curMaterial=m;
        }
        out << "f "
            << a << '/' << a << '/' << a << '\t'
            << b << '/' << b << '/' << b << '\t'
            << c << '/' << c << '/' << c << '\t' << std::endl;
      }
      out.close();
    }
  }
}


//----------------------------------------------------------------------------
MyMesh::MyMesh( void ) : vertices(), triangles(), materials() {}

MyMesh::~MyMesh() {}

unsigned int MyMesh::nbVertices( void ) const
{ return vertices.size()>>4; }

const double* MyMesh::vertexData( void ) const
{ return vertices.empty() ? 0 : &(*vertices.begin()); }

unsigned int MyMesh::nbMaterials( void ) const
{ return materials.size(); }

const char* MyMesh::materialName( unsigned int index ) const
{ return materials[index].c_str(); }

unsigned int MyMesh::nbTriangles( void ) const
{ return triangles.size()>>2; }

const unsigned int* MyMesh::triangleData( void ) const
{ return triangles.empty() ? 0 : &(*triangles.begin()); }
