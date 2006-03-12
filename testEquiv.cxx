// test equivalence of 2 labelled images.

#include <stdlib.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionConstIterator.h>
#include <map>

template <int dimensions, class InputPixelType>
int checkEquiv(std::string infile1, std::string infile2)
{
  typedef typename itk::Image< InputPixelType,  dimensions >    InputImageType;
  typedef typename itk::ImageFileReader< InputImageType >  ReaderType;
  typename ReaderType::Pointer reader1, reader2;

  typename InputImageType::Pointer Im1, Im2;

  reader1 = ReaderType::New();
  reader2 = ReaderType::New();

  reader1->SetFileName(infile1.c_str());
  reader2->SetFileName(infile2.c_str());

  reader1->Update();
  reader2->Update();

  Im1 = reader1->GetOutput();
  Im2 = reader2->GetOutput();

  typedef typename itk::ImageRegionConstIterator<InputImageType> Iterator;

  Iterator it1(Im1, Im1->GetRequestedRegion());
  Iterator it2(Im2, Im2->GetRequestedRegion());
  
  // first check that the image sizes are the same - probably a more
 // general comparison function somewhere to do this

  if (Im1->GetRequestedRegion().GetSize() != Im2->GetRequestedRegion().GetSize())
    {
    std::cerr << "Image dimensions don't match : " << std::endl;
    std::cerr << Im1->GetRequestedRegion().GetSize() << std::endl;
    std::cerr << Im2->GetRequestedRegion().GetSize() << std::endl;
    return EXIT_FAILURE;
    }

  // the map structure we'll use to compare pixel labels
  typedef typename std::map<InputPixelType, InputPixelType> labMapType;
  labMapType LabMap;
  typename labMapType::iterator mIt;
  bool Equiv=true;
  for (it1.GoToBegin(), it2.GoToBegin(); !it1.IsAtEnd(); ++it1, ++it2)
    {
    InputPixelType p1, p2;
    p1 = it1.Get();
    p2 = it2.Get();
    // check to see if p1 is already in the map
    mIt = LabMap.find(p1);
    if (mIt == LabMap.end())
      {
      // haven't seen this label yet
      LabMap[p1] = p2;
      }
    else
      {
      InputPixelType previous = mIt->second;
      if (previous != p2)
	{
	std::cerr << "Found mismatch: (" << p1 << "," << p2 << ")(" << p1 << "," << previous << ")" << std::endl;
	Equiv=false;
	}
      }
    }

  if (Equiv)
    {
    std::cout << "Labelled images are equivalent" << std::endl;
    return(0);
    }
  else
    {
    std::cout << "Images are not equivalent" << std::endl;    
    return (1);
    }

}

int main( int argc, char * argv[] )
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " << argv[0] << " ImageFile1 ImageFile2 dimensions" << std::endl;
    return EXIT_FAILURE;
    }

  // Assume the input type is char
  typedef  int InputPixelType;
  //typedef int OutputPixelType;

  std::string Infile = argv[1];
  std::string Outfile = argv[2];
  int dimensions=2;
  int status = EXIT_FAILURE;
  if (argc > 3) {
    dimensions = atoi(argv[3]);
  }

  switch(dimensions) {
  case 2:
    status = checkEquiv<2, InputPixelType>(Infile, 
					   Outfile);
    break;
  case 3:
    status = checkEquiv<3, InputPixelType>(Infile, 
					   Outfile);
    break;
  default:
    std::cerr << "Unsupported dimensions" << std::endl;
    return EXIT_FAILURE;
  }

  return status;
}
