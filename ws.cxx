#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkNumericTraits.h"
#include <itkIntensityWindowingImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include "itkMorphologicalWatershedImageFilter.h"
#include "itkSimpleFilterWatcher.h"
#include "itkLabelOverlay.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"



int main(int, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned short PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[3] );

  typedef itk::MorphologicalWatershedImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetMarkWatershed( atoi( argv[1] ) );
  filter->SetFullyConnected( atoi( argv[2] ) );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  filter->Update();

  // A thresholder to highlight the borders
  typedef itk::BinaryThresholdImageFilter< IType, IType >  ThresholdFilterType;
  ThresholdFilterType::Pointer threshfilter = ThresholdFilterType::New();

  threshfilter->SetInput(filter->GetOutput());
  threshfilter->SetOutsideValue(0);
  threshfilter->SetInsideValue(1);
  threshfilter->SetUpperThreshold( 0 );
  threshfilter->SetLowerThreshold( 0 );


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
  writer->SetFileName( argv[4] );
  writer->Update();

  typedef itk::RGBPixel<unsigned char> CPType;
  typedef itk::Image< CPType, dim > CIType;

  // create an overlay image - basically mark the watershed lines
  typedef LabelOverlay< PType, PType, CPType > LabelOverlayType;
  typedef itk::BinaryFunctorImageFilter< IType, IType, CIType, LabelOverlayType > ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  colormapper->SetInput1( reader->GetOutput() );
  colormapper->SetInput2( threshfilter->GetOutput() );

  typedef itk::ImageFileWriter< CIType > OVWriterType;
  OVWriterType::Pointer ovwriter = OVWriterType::New();
  ovwriter->SetInput( colormapper->GetOutput() );
  ovwriter->SetFileName( "overlay.tif");
  ovwriter->Update();


  return 0;
}

