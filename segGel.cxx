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

const int dim = 2;

typedef unsigned char PType;
typedef unsigned short LPType;
typedef float FType;

typedef itk::Image< PType, dim > IType;
typedef itk::Image< LPType, dim > LIType;
typedef itk::Image< FType, dim > FIType;


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
  grd->SetSigma(0.5);
  grdMag->SetInput(grd->GetOutput());
  // Now apply a watershed
  WSType::Pointer wshed2 = WSType::New();
  wshed2->SetFullyConnected(true);
  wshed2->SetInput(grdMag->GetOutput());
  wshed2->SetMarkerImage(adder->GetOutput());
  
  // file writer
  typedef itk::ImageFileWriter< LIType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(wshed2->GetOutput());
  writer->SetFileName("recon.tif");
  writer->Update();
}
