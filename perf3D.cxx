#include "itkImageFileReader.h"

#include "itkValuedRegionalMinimaImageFilter.h"
#include "itkHConcaveImageFilter.h"
#include "itkGrayscaleDilateImageFilter.h"

#include "itkTimeProbe.h"
#include <vector>
#include "itkMultiThreader.h"

int main(int, char * argv[])
{
  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);

  const int dim = 3;
  typedef unsigned char PType;
  typedef itk::Image< PType, dim >    IType;
  
  // read the input image
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  typedef itk::HConcaveImageFilter< IType, IType > ConcaveType;
  ConcaveType::Pointer concave = ConcaveType::New();
  concave->SetInput( reader->GetOutput() );
  concave->SetHeight( 1 );
  
  typedef itk::ValuedRegionalMinimaImageFilter< IType, IType > RMinType;
  RMinType::Pointer rmin = RMinType::New();
  rmin->SetInput( reader->GetOutput() );
  
  reader->Update();
  
  
  std::cout << "#F" << "\t" 
            << "concave" << "\t" 
            << "rmin" << "\t" 
            << std::endl;

    itk::TimeProbe ctime;
    itk::TimeProbe rtime;

  for(int F=0; F<=1; F++ )
    {
    concave->SetFullyConnected( F );
    rmin->SetFullyConnected( F );

    for( int i=0; i<5; i++ )
      {
      ctime.Start();
      concave->Update();
      ctime.Stop();
      concave->Modified();

      rtime.Start();
      rmin->Update();
      rtime.Stop();
      rmin->Modified();

      }
      
    std::cout << F << "\t" 
              << ctime.GetMeanTime() << "\t" 
              << rtime.GetMeanTime() << "\t" 
              <<std::endl;
    }
  
  return 0;
}

