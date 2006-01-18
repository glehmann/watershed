#include <iostream>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkWatershedImageFilter.h"

int main( int argc, char *argv[] )
{
  const int dim=2;

  if (argc < 3) 
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage" << std::endl;
    return 1;
    }

  // pixel and image types
  typedef unsigned char InPixType;
  typedef unsigned long LabType;
  typedef itk::RGBPixel<unsigned char>   RGBPixelType;

  typedef itk::Image<RGBPixelType, dim>    RGBImageType;
  typedef itk::Image<InPixType, dim> InImage;  
  typedef itk::Image<LabType, dim> OutImage;
  typedef itk::Image<unsigned short, dim> ShortImage;

  // colour converter
  typedef itk::Functor::ScalarToRGBPixelFunctor<LabType>
    ColorMapFunctorType;
  typedef itk::UnaryFunctorImageFilter<OutImage, RGBImageType, ColorMapFunctorType> ColorMapFilterType;

  typedef itk::CastImageFilter<OutImage, ShortImage> CasterType;

  // readers and writers
  typedef itk::ImageFileReader<InImage> FileReaderType;
  typedef itk::ImageFileWriter<RGBImageType> FileWriterType;
  //typedef itk::ImageFileWriter<ShortImage> LabFileWriterType;

  // watershed filter
  typedef itk::WatershedImageFilter<InImage> WatershedFilterType;



  // Now set up the processing pipeline
  FileReaderType::Pointer reader = FileReaderType::New();
  FileWriterType::Pointer writer = FileWriterType::New();
  //LabFileWriterType::Pointer labwriter = LabFileWriterType::New();

  WatershedFilterType::Pointer wshed = WatershedFilterType::New();
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();
  CasterType::Pointer caster = CasterType::New();

  wshed->SetLevel(0);
  wshed->SetThreshold(0);

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  //labwriter->SetFileName("labels.tif");

  wshed->SetInput(reader->GetOutput());
  colormapper->SetInput(wshed->GetOutput());
  writer->SetInput(colormapper->GetOutput());
  caster->SetInput(wshed->GetOutput());
  //labwriter->SetInput(caster->GetOutput());
  writer->Update();
  //labwriter->Update();
  return 0;
}
