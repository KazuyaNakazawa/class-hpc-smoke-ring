#include "vis_interface.h"
#include <kvs/StructuredVolumeObject>
#include <kvs/Isosurface>
#include <kvs/SlicePlane>
#include <kvs/Bounds>
#include <kvs/RayCastingRenderer>
#include <kvs/ColorImage>
#include <KVS.osmesa/Lib/Screen.h>
#include <kvs/String>

extern "C" void Isosurface( float values[], int size, int dimx, int dimy, int dimz ,int visstep, float isolevel)
{
  std::cout << "Vis_interface Start" << std::endl;
  kvs::StructuredVolumeObject* volume = new kvs::StructuredVolumeObject();
  volume->setGridTypeToUniform();
  volume->setVeclen( 1 );
  volume->setResolution( kvs::Vec3u( dimx, dimy, dimz ) );
  volume->setValues( kvs::ValueArray<float>( values, size )  );
  volume->updateMinMaxValues();
  volume->updateMinMaxCoords();
  volume->print(std::cout);

  //kvs::PolygonObject* object = new kvs::ExternalFaces( volume );
  //const double i = ( volume->maxValue() + volume->minValue() ) * 0.5;
  const double i = ( volume->maxValue() - volume->minValue() ) * isolevel + volume->minValue();
  const kvs::PolygonObject::NormalType n = kvs::PolygonObject::VertexNormal;
  const bool d = false;
  const kvs::TransferFunction t( 256 );
  kvs::PolygonObject* object = new kvs::Isosurface( volume , i, n, d, t );
  delete volume;
  object->print(std::cout);
  const kvs::Mat3 R = kvs::Mat3::RotationX( 30 ) * kvs::Mat3::RotationY( 30 );
  object->multiplyXform( kvs::Xform::Rotation( R ) );
  
  kvs::osmesa::Screen screen;
  screen.registerObject( object );
  screen.registerObject( object, new kvs::Bounds );
  screen.draw();
  
  kvs::ColorImage image = screen.capture();
  
  std::string filename = "iso_image_"+ kvs::String::ToString( visstep) + ".bmp" ;
  //std::string filename = "iso_image.bmp" ;
  image.write( filename );
}

extern "C" void SlicePlane( float values[], int size, int dimx, int dimy, int dimz, int visstep )
{
  std::cout << "Vis_interface Start" << std::endl;
  kvs::StructuredVolumeObject* volume = new kvs::StructuredVolumeObject();
  volume->setGridTypeToUniform();
  volume->setVeclen( 1 );
  volume->setResolution( kvs::Vec3u( dimx, dimy, dimz ) );
  volume->setValues( kvs::ValueArray<float>( values, size )  );
  volume->updateMinMaxValues();
  volume->updateMinMaxCoords();
  volume->print(std::cout);
  
  const kvs::Vector3f c( ( volume->maxObjectCoord() + volume->minObjectCoord() ) * 0.4f );
  const kvs::Vector3f p( c );
  const kvs::Vector3f n( 1.0, 0.8, 1.0 );
  const kvs::TransferFunction t( 256 );

  kvs::PolygonObject* object = new kvs::SlicePlane( volume, p, n, t );
  delete volume;
  object->print(std::cout);
  const kvs::Mat3 R = kvs::Mat3::RotationX( 30 ) * kvs::Mat3::RotationY( 30 );
  object->multiplyXform( kvs::Xform::Rotation( R ) );
  
  kvs::osmesa::Screen screen;
  screen.registerObject( object );
  screen.registerObject( object, new kvs::Bounds );
  screen.draw();
  kvs::ColorImage image = screen.capture();

  std::string filename = "slice_image_"+ kvs::String::ToString( visstep) + ".bmp" ;
  image.write( filename );
  //  image.write( "slice_image.bmp" );
}

extern "C" void RayCasting( float values[], int size, int dimx, int dimy, int dimz, int visstep )
{
  std::cout << "Vis_interface Start" << std::endl;
  kvs::StructuredVolumeObject* volume = new kvs::StructuredVolumeObject();
  volume->setGridTypeToUniform();
  volume->setVeclen( 1 );
  volume->setResolution( kvs::Vec3u( dimx, dimy, dimz ) );
  volume->setValues( kvs::ValueArray<float>( values, size )  );
  volume->updateMinMaxValues();
  volume->updateMinMaxCoords();
  volume->print(std::cout);
  kvs::RayCastingRenderer* renderer = new kvs::RayCastingRenderer();
  renderer->setSamplingStep( 0.5f );
  renderer->setOpaqueValue( 0.90f );

  const kvs::Mat3 R = kvs::Mat3::RotationX( 30 ) * kvs::Mat3::RotationY( 30 );
  volume->multiplyXform( kvs::Xform::Rotation( R ) );
  
  kvs::osmesa::Screen screen;
  screen.registerObject( volume, renderer );
  screen.registerObject( volume, new kvs::Bounds );
  screen.draw();
  kvs::ColorImage image = screen.capture();

  std::string filename = "raycast_image_"+ kvs::String::ToString( visstep) + ".bmp" ;
  image.write( filename );
  //  image.write( "raycast_image.bmp" );
}
