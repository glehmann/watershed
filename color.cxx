#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkLabelOverlayImageFilter.h"

#include "itkSimpleFilterWatcher.h"



int main(int arglen, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::RGBPixel<unsigned char>   RGBPixelType;
  typedef itk::Image<RGBPixelType, dim>    RGBImageType;
  
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::LabelOverlayImageFilter<IType, IType, RGBImageType> OverlayType;
  OverlayType::Pointer overlay = OverlayType::New();
  overlay->SetInput( reader->GetOutput() );
  overlay->SetLabelImage( reader->GetOutput() );

  typedef itk::ImageFileWriter< RGBImageType > RGBWriterType;
  RGBWriterType::Pointer rgbwriter = RGBWriterType::New();
  rgbwriter->SetInput( overlay->GetOutput() );
  rgbwriter->SetFileName( argv[2] );
  rgbwriter->Update();

  if( arglen > 3 )
    {
    overlay->SetOpacity( atof( argv[3] ) );
    }

  if( arglen > 4 )
    {
    overlay->SetUseBackground( true );
    overlay->SetBackgroundValue( atoi( argv[4] ) );
    }

    rgbwriter->Update();

  return 0;
}

