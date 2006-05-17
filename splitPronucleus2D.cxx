#include "itkImageFileReader.h"
#include "itkMedianImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkOpeningByReconstructionImageFilter.h"
#include "itkGrayscaleFillholeImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkDanielssonDistanceMapImageFilter.h"
#include "itkMorphologicalWatershedImageFilter.h"
#include "itkMaskImageFilter.h"
#include "itkLabelOverlayImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkImageFileWriter.h"

int main(int arglen, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef unsigned long LPType;
  typedef itk::Image< LPType, dim > LIType;

  typedef itk::RGBPixel<unsigned char>   RGBPixelType;
  typedef itk::Image<RGBPixelType, dim>    RGBImageType;
    
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::MedianImageFilter< IType, IType > MedianType;
  MedianType::Pointer median = MedianType::New();
  median->SetInput( reader->GetOutput() );

  typedef itk::BinaryThresholdImageFilter< IType, IType > ThresholdType;
  ThresholdType::Pointer th = ThresholdType::New();
  th->SetInput( median->GetOutput() );
  th->SetLowerThreshold( 40 );

  typedef itk::BinaryBallStructuringElement< bool, dim > KernelType;
  KernelType kernel;
  kernel.SetRadius( 30 );
  kernel.CreateStructuringElement();

  typedef itk::OpeningByReconstructionImageFilter< IType, IType, KernelType > OpeningType;
  OpeningType::Pointer open = OpeningType::New();
  open->SetInput( th->GetOutput() );
  open->SetKernel( kernel );

  typedef itk::GrayscaleFillholeImageFilter< IType, IType > FillType;
  FillType::Pointer fill = FillType::New();
  fill->SetInput( open->GetOutput() );
  
  typedef itk::InvertIntensityImageFilter< IType, IType > InvertType;
  InvertType::Pointer invert = InvertType::New();
  invert->SetInput( fill->GetOutput() );
  
  typedef itk::DanielssonDistanceMapImageFilter< IType, IType > DistanceType;
  DistanceType::Pointer distance = DistanceType::New();
  distance->SetInput( invert->GetOutput() );

  InvertType::Pointer idistance = InvertType::New();
  idistance->SetInput( distance->GetOutput() );
  
  typedef itk::MorphologicalWatershedImageFilter< IType, IType > MWatershedType;
  MWatershedType::Pointer ws = MWatershedType::New();
  ws->SetInput( idistance->GetOutput() );
  ws->SetMarkWatershedLine( false );
  ws->SetLevel( 10 );

  typedef itk::MaskImageFilter< IType, IType, IType > MaskType;
  MaskType::Pointer mask = MaskType::New();
  mask->SetInput( 0, ws->GetOutput() );
  mask->SetInput( 1, fill->GetOutput() );

  typedef itk::LabelOverlayImageFilter< IType, IType, RGBImageType > OverlayType;
  OverlayType::Pointer overlay = OverlayType::New();
  overlay->SetInput( reader->GetOutput() );
  overlay->SetLabelImage( mask->GetOutput() );
  overlay->SetUseBackground( true );



  typedef itk::WatershedImageFilter< IType > WatershedType;
  WatershedType::Pointer ws2 = WatershedType::New();
  ws2->SetInput( idistance->GetOutput() );
  ws2->SetLevel( 0.5 );

  // relabel to get the same result as with python
  typedef itk::RelabelComponentImageFilter< LIType, IType > RelabelType;
  RelabelType::Pointer relabel = RelabelType::New();
  relabel->SetInput( ws2->GetOutput() );

  MaskType::Pointer mask2 = MaskType::New();
  mask2->SetInput( 0, relabel->GetOutput() );
  mask2->SetInput( 1, fill->GetOutput() );

  OverlayType::Pointer overlay2 = OverlayType::New();
  overlay2->SetInput( reader->GetOutput() );
  overlay2->SetLabelImage( mask2->GetOutput() );
  overlay2->SetUseBackground( true );


  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();

  typedef itk::ImageFileWriter< RGBImageType > RGBWriterType;
  RGBWriterType::Pointer rgbwriter = RGBWriterType::New();


  writer->SetInput( median->GetOutput() );
  writer->SetFileName( "embryo-median.png" );
  writer->Update();

  writer->SetInput( th->GetOutput() );
  writer->SetFileName( "embryo-th.png" );
  writer->Update();

  writer->SetInput( open->GetOutput() );
  writer->SetFileName( "embryo-open.png" );
  writer->Update();

  writer->SetInput( fill->GetOutput() );
  writer->SetFileName( "embryo-fill.png" );
  writer->Update();

  writer->SetInput( invert->GetOutput() );
  writer->SetFileName( "embryo-invert.png" );
  writer->Update();

  writer->SetInput( distance->GetOutput() );
  writer->SetFileName( "embryo-distance.png" );
  writer->Update();

  writer->SetInput( idistance->GetOutput() );
  writer->SetFileName( "embryo-idistance.png" );
  writer->Update();

  rgbwriter->SetInput( overlay->GetOutput() );
  rgbwriter->SetFileName( "embryo-overlay.png" );
  rgbwriter->Update();

  rgbwriter->SetInput( overlay2->GetOutput() );
  rgbwriter->SetFileName( "embryo-overlay2.png" );
  rgbwriter->Update();

  return 0;
}

