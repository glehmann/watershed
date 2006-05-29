#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

//#define RECON 

#ifdef RECON
#include "itkHMinimaImageFilter.h"
#else
// filtering instead
#include "itkMedianImageFilter.h"
#endif

#include "itkRegionalMinimaImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkMorphologicalWatershedFromMarkersImageFilter.h"
#include <itkBinaryThresholdImageFilter.h>
#include <itkAddImageFilter.h>
#include <itkGradientRecursiveGaussianImageFilter.h>
#include <itkGradientToMagnitudeImageFilter.h>
#include "itkLabelOverlayImageFilter.h"
#include <itkChangeLabelImageFilter.h>

const int dim = 2;

typedef unsigned char PType;
typedef unsigned short LPType;
typedef float FType;

typedef itk::Image< PType, dim > IType;
typedef itk::Image< LPType, dim > LIType;
typedef itk::Image< FType, dim > FIType;

typedef itk::RGBPixel<unsigned char>   RGBPixelType;
typedef itk::Image<RGBPixelType, dim>    RGBImageType;


int main(int arglen, char * argv[])
{

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

#ifdef RECON
  // suppress insignficant regional minima 
  typedef itk::HMinimaImageFilter< IType, IType > SupressReglMinType;
  SupressReglMinType::Pointer findSigMin = SupressReglMinType::New();
  findSigMin->SetInput(reader->GetOutput());
  findSigMin->SetFullyConnected(true);
  findSigMin->SetHeight(5);
#else
  // median filtering
  typedef itk::MedianImageFilter<IType, IType> MedianType;
  MedianType::Pointer medfilt = MedianType::New();
  MedianType::InputSizeType Sz;
  Sz.Fill(1);
  
  medfilt->SetRadius(Sz);
  medfilt->SetInput(reader->GetOutput());
  
#endif
  // find regional minima - do this outside the watershed, because
  // we'll use the label image twice
  typedef itk::RegionalMinimaImageFilter< IType, IType > RegMinType;
  RegMinType::Pointer findRegMin = RegMinType::New();
  findRegMin->SetFullyConnected(true);
#ifdef RECON
  findRegMin->SetInput(findSigMin->GetOutput());
#else
  findRegMin->SetInput(medfilt->GetOutput());
#endif
  // connected component labelling
  typedef itk::ConnectedComponentImageFilter<IType, LIType> LabelType;
  LabelType::Pointer labeller = LabelType::New();
  labeller->SetInput(findRegMin->GetOutput());
  labeller->SetFullyConnected(true);
  
  // first stage watershed - notice that we are not computing a
  // gradient. The idea is to use the watershed to tesselate the
  // scene, so watershed lines need to end up on the bright parts.
  typedef itk::MorphologicalWatershedFromMarkersImageFilter<IType, LIType> WSType;
  WSType::Pointer wshed = WSType::New();
  wshed->SetFullyConnected(true);
  wshed->SetMarkWatershedLine(true),			
  // could be applied to filtered version - doesn't really matter
  wshed->SetInput(reader->GetOutput());
  wshed->SetMarkerImage(labeller->GetOutput());
  wshed->Update();
  // extract the watershed lines and combine with the orginal markers
  typedef itk::BinaryThresholdImageFilter<LIType, LIType> ThreshType;
  ThreshType::Pointer thresh = ThreshType::New();
  thresh->SetUpperThreshold(0);
  thresh->SetInput(wshed->GetOutput());
  thresh->SetOutsideValue(0);
  // set the outside value to the number of markers + 1
  thresh->SetInsideValue(labeller->GetObjectCount() + 1);
  // Add the marker image to the watershed line image
  typedef itk::AddImageFilter<LIType, LIType, LIType> AddType;
  AddType::Pointer adder = AddType::New();
  adder->SetInput1(thresh->GetOutput());
  adder->SetInput2(labeller->GetOutput());
  // compute a gradient
  typedef itk::Image< itk::CovariantVector< itk::NumericTraits< PType>::RealType, dim >, dim > GradImType;
  typedef itk::GradientRecursiveGaussianImageFilter<IType,GradImType> GradFiltType;
  typedef itk::GradientToMagnitudeImageFilter<GradImType, IType> GradMagType;
  GradFiltType::Pointer grd = GradFiltType::New();
  GradMagType::Pointer grdMag = GradMagType::New();
  grd->SetInput(reader->GetOutput());
  grd->SetSigma(0.25);
  grdMag->SetInput(grd->GetOutput());
  // Now apply a watershed
  WSType::Pointer wshed2 = WSType::New();
  wshed2->SetFullyConnected(true);
  wshed2->SetInput(grdMag->GetOutput());
  wshed2->SetMarkerImage(adder->GetOutput());
  wshed2->SetMarkWatershedLine(false);			
  
  // now for the third stage using second derivatives
  GradFiltType::Pointer grd2 = GradFiltType::New();
  GradMagType::Pointer grdMag2 = GradMagType::New();
  grd2->SetInput(grdMag->GetOutput());
  grd2->SetSigma(0.25);
  grdMag2->SetInput(grd2->GetOutput());

  // delete the background label
  typedef itk::ChangeLabelImageFilter<LIType, LIType> ChangeLabType;
  ChangeLabType::Pointer changer = ChangeLabType::New();
  changer->SetInput(wshed2->GetOutput());
  changer->SetChange(labeller->GetObjectCount() + 1, 0);
  // combine the markers again - could use an eroded version of the
  // final background marker instead of the original to provide more
  // constraints
  AddType::Pointer adder2 = AddType::New();
  adder2->SetInput1(thresh->GetOutput());
  adder2->SetInput2(changer->GetOutput());
  WSType::Pointer wshed3 = WSType::New();
  wshed3->SetFullyConnected(true);
  wshed3->SetInput(grdMag2->GetOutput());
  wshed3->SetMarkerImage(adder2->GetOutput());
  wshed3->SetMarkWatershedLine(false);			

  // file writers
  typedef itk::ImageFileWriter< RGBImageType > RGBWriterType;
  RGBWriterType::Pointer rgbwriter = RGBWriterType::New();

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();

  // generate output images
  typedef itk::LabelOverlayImageFilter< IType, LIType, RGBImageType > LabOverlayType;
  LabOverlayType::Pointer overlay = LabOverlayType::New();
  // regional minima
  overlay->SetInput(reader->GetOutput());
  overlay->SetLabelImage(labeller->GetOutput());
  overlay->SetBackgroundValue(0);
  overlay->SetUseBackground(true);
  overlay->SetOpacity(1.0);

  rgbwriter->SetFileName("gel_minima.png");
  rgbwriter->SetInput(overlay->GetOutput());
  rgbwriter->Update();
  // tesselation
  overlay->SetLabelImage(thresh->GetOutput());
  rgbwriter->SetFileName("gel_tesselation.png");
  rgbwriter->Update();
  // combined markers
  overlay->SetLabelImage(adder->GetOutput());
  rgbwriter->SetFileName("gel_allmarkers.png");
  rgbwriter->Update();
  // combined markers 2
  overlay->SetLabelImage(adder2->GetOutput());
  rgbwriter->SetFileName("gel_allmarkers2.png");
  rgbwriter->Update();
  // result
  overlay->SetBackgroundValue(labeller->GetObjectCount() + 1);
  overlay->SetUseBackground(true);
  overlay->SetOpacity(0.5);
  overlay->SetLabelImage(wshed2->GetOutput());
  rgbwriter->SetFileName("gel_result.png");
  rgbwriter->Update();

  overlay->SetLabelImage(wshed3->GetOutput());
  rgbwriter->SetFileName("gel_result2.png");
  rgbwriter->Update();
  
  writer->SetInput(grdMag->GetOutput());
  writer->SetFileName("gel_gradient.png");
  writer->Update();
  writer->SetInput(grdMag2->GetOutput());
  writer->SetFileName("gel_gradient2.png");
  writer->Update();
}
