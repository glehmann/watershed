#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkRegionalMinimaImageFilter.h"
#include "itkHMinimaImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkMorphologicalWatershedImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkMorphologicalWatershedFromMarkersImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkMinimaImpositionImageFilter.h"
#include <itkImageRegionConstIterator.h>
#include "itkChangeLabelImageFilter.h"

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
  
  typedef unsigned long LPType;
  typedef itk::Image< LPType, dim > LIType;

  // read the input image
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );
  
  // the image is more interesting inverted 
  typedef itk::InvertIntensityImageFilter< IType, IType > InvertType;
  InvertType::Pointer invert = InvertType::New();
  invert->SetInput( reader->GetOutput() );

  typedef itk::MorphologicalWatershedFromMarkersImageFilter< IType, IType > MMWatershedType;
  MMWatershedType::Pointer mmws = MMWatershedType::New();
  mmws->SetInput( invert->GetOutput() );
  mmws->SetMarkerImage( reader2->GetOutput() );

  typedef itk::MinimaImpositionImageFilter< IType, IType > MinimaImpositionType;
  MinimaImpositionType::Pointer minima = MinimaImpositionType::New();
  minima->SetInput(0, invert->GetOutput() );
  minima->SetInput(1, reader2->GetOutput() );

  typedef itk::WatershedImageFilter< IType > WatershedType;
  WatershedType::Pointer ws = WatershedType::New();
  ws->SetInput( minima->GetOutput() );
  
  typedef itk::ChangeLabelImageFilter< LIType, IType > ChangeLabelType;
  ChangeLabelType::Pointer change = ChangeLabelType::New();
  change->SetInput( ws->GetOutput() );


  std::cout << "#F" << "\t" 
            << "M" << "\t" 
            << "min" << "\t" 
            << "itk ws" << "\t" 
            << "match" << "\t" 
            << "change" << "\t" 
            << "mmws" << "\t" 
            << "itk mws" << "\t" 
            << std::endl;

  invert->Update();

  for(int F=0; F<=1; F++ )
    {
    for(int M=0; M<=1; M++ )
      {
      itk::TimeProbe mintime;
      itk::TimeProbe wtime;
      itk::TimeProbe matchtime;
      itk::TimeProbe changetime;
      itk::TimeProbe mmwtime;
  
      minima->SetFullyConnected( F );
      mmws->SetFullyConnected( F );
  
      mmws->SetMarkWatershedLine( M );

      for( int i=0; i<10; i++ )
        {
        if( !M && F )
          {
          mintime.Start();
          minima->Update();
          mintime.Stop();
    
          wtime.Start();
          ws->Update();
          wtime.Stop();
    
          matchtime.Start();
          itk::ImageRegionConstIterator< LIType > wit( ws->GetOutput(),
                                                      ws->GetOutput()->GetLargestPossibleRegion() );
          itk::ImageRegionConstIterator< IType > mit( reader2->GetOutput(),
                                                      ws->GetOutput()->GetLargestPossibleRegion() );
        
          for ( mit.GoToBegin(), wit.GoToBegin();
            !mit.IsAtEnd();
            ++wit, ++mit )
            {
            if( mit.Get() != 0 )
              { change->SetChange( wit.Get(), mit.Get() ); }
            }
          matchtime.Stop();
    
          changetime.Start();
          change->Update();
          changetime.Stop();
          }

        mmwtime.Start();
        mmws->Update();
        mmwtime.Stop();
  
  
        minima->Modified();

        ws->Modified();
        // ws->Modified() is not enough
        ws->SetInput( NULL );
        ws->SetInput( minima->GetOutput() );

        change->Modified();
        change->ClearChangeMap();

        mmws->Modified();
  
        }
        
      std::cout << std::setprecision(3)
                << F << "\t" 
                << M << "\t";
      if( !M && F )
        {
        std::cout
                << mintime.GetMeanTime() << "\t" 
                << wtime.GetMeanTime() << "\t" 
                << matchtime.GetMeanTime() << "\t" 
                << changetime.GetMeanTime() << "\t" 
                << mmwtime.GetMeanTime() << "\t" 
                << mintime.GetMeanTime() + wtime.GetMeanTime() + matchtime.GetMeanTime() + changetime.GetMeanTime() << "\t";
        }
      else 
        {
        std::cout
                << "-" << "\t" 
                << "-" << "\t" 
                << "-" << "\t" 
                << "-" << "\t" 
                << mmwtime.GetMeanTime() << "\t" 
                << "-" << "\t";
        }
      std::cout <<std::endl;
      }
    }
  
  return 0;
}

