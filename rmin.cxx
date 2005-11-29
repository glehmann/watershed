// a test routine for regional extrema using flooding
#include "itkRegionalMinimaImageFilter.h"
#include "itkHConcaveImageFilter.h"
#include "itkMaximumImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include <itkRescaleIntensityImageFilter.h>
#include <itkAndImageFilter.h>
#include "itkCommand.h"

template < class TFilter >
class ProgressCallback : public itk::Command
{
public:
  typedef ProgressCallback   Self;
  typedef itk::Command  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  itkTypeMacro( IterationCallback, Superclass );
  itkNewMacro( Self );

  /** Type defining the optimizer. */
  typedef    TFilter     FilterType;

  /** Method to specify the optimizer. */
  void SetFilter( FilterType * filter )
    {
    m_Filter = filter;
    m_Filter->AddObserver( itk::ProgressEvent(), this );
    }

  /** Execute method will print data at each iteration */
  void Execute(itk::Object *caller, const itk::EventObject & event)
    {
    Execute( (const itk::Object *)caller, event);
    }

  void Execute(const itk::Object *, const itk::EventObject & event)
    {
    std::cout << m_Filter->GetNameOfClass() << ": " << m_Filter->GetProgress() << std::endl;
    }

protected:
  ProgressCallback() {};
  itk::WeakPointer<FilterType>   m_Filter;
};





int main(int, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[2] );

  typedef itk::RegionalMinimaImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetFullyConnected( atoi(argv[1]) );

  typedef ProgressCallback< FilterType > ProgressType;
  ProgressType::Pointer progress = ProgressType::New();
  progress->SetFilter( filter );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();


  // produce the same output with other filters
  typedef itk::HConcaveImageFilter< IType, IType > ConcaveType;
  ConcaveType::Pointer concave = ConcaveType::New();
  concave->SetInput( reader->GetOutput() );
  concave->SetFullyConnected( atoi(argv[1]) );
  concave->SetHeight( 1 );

  // concave gives minima with value=1 and others with value=0
  // rescale the image so we have minima=255 other=0
  typedef itk::RescaleIntensityImageFilter< IType, IType > RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( concave->GetOutput() );
  rescale->SetOutputMaximum( 255 );
  rescale->SetOutputMinimum( 0 );

  // in the input image, select the values of the pixel at the minima
  typedef itk::AndImageFilter< IType, IType, IType > AndType;
  AndType::Pointer a = AndType::New();
  a->SetInput(0, rescale->GetOutput() );
  a->SetInput(1, reader->GetOutput() );

  // all pixel which are not minima must have value=255
  // get the non minima pixel by inverting the rescaled image
  // minima have value=0 and non minima value=255
  typedef itk::InvertIntensityImageFilter< IType, IType > InvertType;
  InvertType::Pointer invert = InvertType::New();
  invert->SetInput( rescale->GetOutput() );

  // get the highest value from "and" and from invert. The minima have value>=0 in and image
  // and the non minima have a value =0. In invert, the non minima have a value=255 and the minima
  // a value=0
  typedef itk::MaximumImageFilter< IType, IType, IType > MaxType;
  MaxType::Pointer max = MaxType::New();
  max->SetInput(0, invert->GetOutput() );
  max->SetInput(1, a->GetOutput() );

  WriterType::Pointer writer2 = WriterType::New();
  writer2->SetInput( max->GetOutput() );
  writer2->SetFileName( argv[4] );
  writer2->Update();

  // to verify if the image is flat or not
  filter->Print( std::cout );

  return 0;
}

