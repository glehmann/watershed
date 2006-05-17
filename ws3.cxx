#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkNumericTraits.h"
#include <itkIntensityWindowingImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include "itkMorphologicalWatershedImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkSimpleFilterWatcher.h"



int main(int, char * argv[])
{
  const int dim = 3;
  
  typedef unsigned short PType;
  typedef itk::Image< PType, dim > IType;

  typedef unsigned char PType2;
  typedef itk::Image< PType2, dim > IType2;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[4] );

  typedef itk::InvertIntensityImageFilter< IType, IType > InvertType;
  InvertType::Pointer invert = InvertType::New();
  invert->SetInput( reader->GetOutput() );

  typedef itk::MorphologicalWatershedImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( invert->GetOutput() );
  filter->SetMarkWatershedLine( atoi( argv[1] ) );
  filter->SetFullyConnected( atoi( argv[2] ) );
  filter->SetLevel( atoi( argv[3] ) );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::RelabelComponentImageFilter< IType, IType > RelabelType;
  RelabelType::Pointer relabel = RelabelType::New();
  relabel->SetInput( filter->GetOutput() );

  relabel->Update();

  // rescale the output to have a better display
  typedef itk::MinimumMaximumImageCalculator< IType > MaxCalculatorType;
  MaxCalculatorType::Pointer max = MaxCalculatorType::New();
  max->SetImage( relabel->GetOutput() );
  max->Compute();

  typedef itk::IntensityWindowingImageFilter< IType, IType2 > RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( relabel->GetOutput() );
  rescale->SetWindowMinimum( itk::NumericTraits< PType >::Zero );
  rescale->SetWindowMaximum( max->GetMaximum() );
  rescale->SetOutputMaximum( itk::NumericTraits< PType2 >::max() );
  rescale->SetOutputMinimum( itk::NumericTraits< PType2 >::Zero );

  typedef itk::ImageFileWriter< IType2 > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( rescale->GetOutput() );
  writer->SetFileName( argv[5] );
  writer->Update();

  return 0;
}

