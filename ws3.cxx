#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkNumericTraits.h"
#include <itkIntensityWindowingImageFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include "itkMorphologicalWatershedImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkInvertIntensityImageFilter.h"

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
  const int dim = 3;
  
  typedef unsigned short PType;
  typedef itk::Image< PType, dim > IType;

  typedef unsigned char PType2;
  typedef itk::Image< PType2, dim > IType2;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[4] );

  typedef itk::InvertIntensityImageFilter< IType, IType > InvertType;
  InvertType::Pointer invert = InvertType::New();
  invert->SetInput( reader->GetOutput() );

  typedef itk::MorphologicalWatershedImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( invert->GetOutput() );
  filter->SetMarkWatershed( atoi( argv[1] ) );
  filter->SetFullyConnected( atoi( argv[2] ) );
  filter->SetThreshold( atoi( argv[3] ) );

  typedef ProgressCallback< FilterType > ProgressType;
  ProgressType::Pointer progress = ProgressType::New();
  progress->SetFilter( filter );

  typedef itk::RelabelComponentImageFilter< IType, IType > RelabelType;
  RelabelType::Pointer relabel = RelabelType::New();
  relabel->SetInput( filter->GetOutput() );

  relabel->Update();

  // rescale the output to have a better display
  typedef itk::MinimumMaximumImageCalculator< IType > MaxCalculatorType;
  MaxCalculatorType::Pointer max = MaxCalculatorType::New();
  max->SetImage( relabel->GetOutput() );
  max->Compute();

std::cout << max->GetMinimum() << " " << max->GetMaximum() << std::endl;

  typedef itk::IntensityWindowingImageFilter< IType, IType2 > RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( relabel->GetOutput() );
  rescale->SetWindowMinimum( itk::NumericTraits< PType >::Zero );
  rescale->SetWindowMaximum( max->GetMaximum() );
  rescale->SetOutputMaximum( itk::NumericTraits< PType2 >::max() );
  rescale->SetOutputMinimum( itk::NumericTraits< PType2 >::Zero );

  typedef itk::ImageFileWriter< IType2 > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( rescale->GetOutput() );
  writer->SetFileName( argv[5] );
  writer->Update();

  return 0;
}
