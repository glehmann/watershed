#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkRegionalMinimaImageFilter.h"
#include "itkHMinimaImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkMorphologicalWatershedImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkMorphologicalWatershedFromMarkersImageFilter.h"
#include "itkInvertIntensityImageFilter.h"

#include "itkTimeProbe.h"
#include <vector>
#include "itkMultiThreader.h"
#include <iomanip>

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
  
  // the image is more interesting inverted 
  typedef itk::InvertIntensityImageFilter< IType, IType > InvertType;
  InvertType::Pointer invert = InvertType::New();
  invert->SetInput( reader->GetOutput() );

  // remove some minima
  typedef itk::HMinimaImageFilter< IType, IType > MinimaType;
  MinimaType::Pointer minima = MinimaType::New();
  minima->SetInput( invert->GetOutput() );
  minima->SetHeight( 30 );

//  typedef itk::ImageFileWriter<IType> WriterType;
//  WriterType::Pointer writer = WriterType::New();
//  writer->SetInput(minima->GetOutput());
//  writer->SetFileName("esmod.img");
//  writer->Update();
    


  typedef itk::RegionalMinimaImageFilter< IType, IType > RMinType;
  RMinType::Pointer rmin = RMinType::New();
  rmin->SetInput( minima->GetOutput() );
  
  typedef itk::ConnectedComponentImageFilter< IType, IType > ConnectedCompType;
  ConnectedCompType::Pointer label = ConnectedCompType::New();
  label->SetInput( rmin->GetOutput() );

  typedef itk::MorphologicalWatershedFromMarkersImageFilter< IType, IType > MMWatershedType;
  MMWatershedType::Pointer mmws = MMWatershedType::New();
  mmws->SetInput( reader->GetOutput() );
  mmws->SetMarkerImage( label->GetOutput() );



  typedef itk::WatershedImageFilter< IType > WatershedType;
  WatershedType::Pointer ws = WatershedType::New();
  ws->SetInput( minima->GetOutput() );


  
  typedef itk::MorphologicalWatershedImageFilter< IType, IType > MWatershedType;
  MWatershedType::Pointer mws = MWatershedType::New();
  mws->SetInput( minima->GetOutput() );
  

  // should take about 20 sec
  minima->Update();
  
  
  std::cout << "#F" << "\t" 
            << "M" << "\t" 
            << "rmin" << "\t" 
            << "label" << "\t" 
            << "mmw" << "\t" 
/*            << "+" << "\t" */
            << "mw" << "\t" 
            << "w" << "\t" 
            << std::endl;

  for(int F=0; F<=1; F++ )
    {
    for(int M=0; M<=1; M++ )
      {
      itk::TimeProbe rtime;
      itk::TimeProbe wtime;
      itk::TimeProbe mwtime;
      itk::TimeProbe mmwtime;
      itk::TimeProbe ltime;
  
      rmin->SetFullyConnected( F );
      mws->SetFullyConnected( F );
      mmws->SetFullyConnected( F );
      label->SetFullyConnected( F );
  
      mws->SetMarkWatershedLine( M );
      mmws->SetMarkWatershedLine( M );

      for( int i=0; i<10; i++ )
        {
        if( !M && !F )
          {
          wtime.Start();
          ws->Update();
          wtime.Stop();
          }
  
        mwtime.Start();
        mws->Update();
        mwtime.Stop();
  
        rtime.Start();
        rmin->Update();
        rtime.Stop();
  
        ltime.Start();
        label->Update();
        ltime.Stop();
  
        mmwtime.Start();
        mmws->Update();
        mmwtime.Stop();
  
  
        rmin->Modified();
        mmws->Modified();
        mws->Modified();
        label->Modified();
        // ws->Modified() is not enough
        ws->SetInput( NULL );
        ws->SetInput( minima->GetOutput() );
  
        }
        
      std::cout << std::setprecision(3)
                << F << "\t" 
                << M << "\t" 
                << rtime.GetMeanTime() << "\t" 
                << ltime.GetMeanTime() << "\t" 
                << mmwtime.GetMeanTime() << "\t" 
/*                << mmwtime.GetMeanTime() + ltime.GetMeanTime() + rtime.GetMeanTime() << "\t" */
                << mwtime.GetMeanTime() << "\t";
      if( !M && !F )
        {
        std::cout
                << wtime.GetMeanTime() << "\t";
        }
      else 
        {
        std::cout
                << "-" << "\t";
        }
      std::cout <<std::endl;
      }
    }
  
  return 0;
}

