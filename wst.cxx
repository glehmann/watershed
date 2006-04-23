#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkNumericTraits.h"
#include <itkIntensityWindowingImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include "itkLabelOverlayImageFilter.h"

#include "itkMorphologicalWatershedImageFilter.h"
#include "itkSimpleFilterWatcher.h"


int main(int arglen, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[2] );

  typedef itk::MorphologicalWatershedImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetMarkWatershed( true );
  filter->SetFullyConnected( false );
  filter->SetThreshold( atoi( argv[1] ) );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  filter->Update();

  // rescale the output to have a better display
  typedef itk::MinimumMaximumImageCalculator< IType > MaxCalculatorType;
  MaxCalculatorType::Pointer max = MaxCalculatorType::New();
  max->SetImage( filter->GetOutput() );
  max->Compute();

  typedef itk::IntensityWindowingImageFilter< IType, IType > RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( filter->GetOutput() );
  rescale->SetWindowMinimum( itk::NumericTraits< PType >::Zero );
  rescale->SetWindowMaximum( max->GetMaximum() );
  rescale->SetOutputMaximum( itk::NumericTraits< PType >::max() );
  rescale->SetOutputMinimum( itk::NumericTraits< PType >::Zero );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( rescale->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();

  if( arglen > 4 )
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
    rgbwriter->SetFileName( argv[4] );
    rgbwriter->Update();

    if( arglen > 5 )
      {
      overlay->SetOpacity( atof( argv[5] ) );
      }

    rgbwriter->Update();

    }

  return 0;
}

