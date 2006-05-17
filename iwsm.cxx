#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkLabelOverlayImageFilter.h"
#include "itkMinimaImpositionImageFilter.h"
#include <itkIntensityWindowingImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkImageRegionConstIterator.h>

#include "itkWatershedImageFilter.h"
#include "itkSimpleFilterWatcher.h"
#include "itkChangeLabelImageFilter.h"

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
//   filter->SetMarkWatershedLine( atoi( argv[1] ) );
//   filter->SetFullyConnected( atoi( argv[2] ) );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  filter->Update();

  typedef itk::ChangeLabelImageFilter< LIType, IType > ChangeLabelType;
  ChangeLabelType::Pointer changeLabel = ChangeLabelType::New();
  changeLabel->SetInput( filter->GetOutput() );
  
  itk::ImageRegionConstIterator< LIType > wit( filter->GetOutput(), filter->GetOutput()->GetLargestPossibleRegion() );
  itk::ImageRegionConstIterator< IType > mit( reader2->GetOutput(), filter->GetOutput()->GetLargestPossibleRegion() );
  for ( mit.GoToBegin(), wit.GoToBegin();
    !mit.IsAtEnd();
    ++wit, ++mit )
    {
    if( mit.Get() != 0 )
      {
      changeLabel->SetChange( wit.Get(), mit.Get() );
      }
    }

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( changeLabel->GetOutput() );
  writer->SetFileName( argv[5] );
  writer->Update();

  if( arglen > 6 )
    {
    typedef itk::RGBPixel<unsigned char>   RGBPixelType;
    typedef itk::Image<RGBPixelType, dim>    RGBImageType;
    
    typedef itk::LabelOverlayImageFilter<IType, IType, RGBImageType> OverlayType;
    OverlayType::Pointer overlay = OverlayType::New();
    overlay->SetInput( reader->GetOutput() );
    overlay->SetLabelImage( changeLabel->GetOutput() );

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

