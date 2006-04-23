#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkLabelOverlayImageFilter.h"
#include "itkMinimaImpositionImageFilter.h"
#include <itkIntensityWindowingImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>

#include "itkWatershedImageFilter.h"
#include "itkSimpleFilterWatcher.h"


int main(int arglen, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef unsigned long LPType;
  typedef itk::Image< LPType, dim > LIType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[3] );

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[4] );

  typedef itk::MinimaImpositionImageFilter< IType, IType > MinimaImpositionType;
  MinimaImpositionType::Pointer minima = MinimaImpositionType::New();
  minima->SetInput(0, reader->GetOutput() );
  minima->SetInput(1, reader2->GetOutput() );

  typedef itk::WatershedImageFilter< IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( minima->GetOutput() );
//   filter->SetMarkerImage( reader2->GetOutput() );
//   filter->SetMarkWatershed( atoi( argv[1] ) );
//   filter->SetFullyConnected( atoi( argv[2] ) );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  filter->Update();

  // rescale the output to have a better display
  typedef itk::MinimumMaximumImageCalculator< LIType > MaxCalculatorType;
  MaxCalculatorType::Pointer max = MaxCalculatorType::New();
  max->SetImage( filter->GetOutput() );
  max->Compute();

  typedef itk::IntensityWindowingImageFilter< LIType, IType > RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( filter->GetOutput() );
  rescale->SetWindowMinimum( itk::NumericTraits< PType >::Zero );
  rescale->SetWindowMaximum( max->GetMaximum() );
  rescale->SetOutputMaximum( itk::NumericTraits< PType >::max() );
  rescale->SetOutputMinimum( itk::NumericTraits< PType >::Zero );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( rescale->GetOutput() );
  writer->SetFileName( argv[5] );
  writer->Update();

  if( arglen > 6 )
    {
    typedef itk::RGBPixel<unsigned char>   RGBPixelType;
    typedef itk::Image<RGBPixelType, dim>    RGBImageType;
    
    typedef itk::LabelOverlayImageFilter<IType, LIType, RGBImageType> OverlayType;
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

