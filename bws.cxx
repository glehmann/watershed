#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkNumericTraits.h"
#include "itkBinaryWatershedImageFilter.h"

#include "itkSimpleFilterWatcher.h"



int main(int argc, char * argv[])
{
  if( argc != 8 )
    {
    std::cerr << "usage: " << argv[0] << " input output binary fullyConnected fg bg level" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::BinaryWatershedImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetBinaryOutput( atoi( argv[3] ) );
  filter->SetFullyConnected( atoi( argv[4] ) );
  filter->SetForegroundValue( atoi( argv[5] ) );
  filter->SetBackgroundValue( atoi( argv[6] ) );
  filter->SetLevel( atoi( argv[7] ) );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  filter->Update();

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

