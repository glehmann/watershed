#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkLabelOverlayImageFilter.h"

#include "itkMorphologicalWatershedFromMarkersImageFilter.h"
#include "itkSimpleFilterWatcher.h"


int main(int arglen, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[3] );

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[4] );

  typedef itk::MorphologicalWatershedFromMarkersImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetMarkerImage( reader2->GetOutput() );
  filter->SetMarkWatershedLine( atoi( argv[1] ) );
  filter->SetFullyConnected( atoi( argv[2] ) );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[5] );
  writer->Update();

  if( arglen > 6 )
    {
    typedef itk::RGBPixel<unsigned char>   RGBPixelType;
    typedef itk::Image<RGBPixelType, dim>    RGBImageType;
    
    typedef itk::LabelOverlayImageFilter<IType, IType, RGBImageType> OverlayType;
    OverlayType::Pointer overlay = OverlayType::New();
    overlay->SetInput( reader->GetOutput() );
    overlay->SetLabelImage( filter->GetOutput() );

    typedef itk::ImageFileWriter< RGBImageType > RGBWriterType;
    RGBWriterType::Pointer rgbwriter = RGBWriterType::New();
    rgbwriter->SetInput( overlay->GetOutput() );
    rgbwriter->SetFileName( argv[6] );
    rgbwriter->Update();

    if( arglen > 7 )
      {
      overlay->SetOpacity( atof( argv[7] ) );
      }

    rgbwriter->Update();

    }

  return 0;
}

