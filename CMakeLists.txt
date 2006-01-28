#Change PROJECT_NAME to the name of your project
PROJECT(watershed)

#include some macros from another file...
INCLUDE(IJMacros.txt)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)

#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
SET(Required_Packages
  ITK
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
  ITKCommon
  ITKBasicFilters
  ITKIO
  ITKAlgorithms
)

OPTION(BUILD_WRAPPERS "Wrap library" OFF)
IF(BUILD_WRAPPERS)
   SUBDIRS(Wrapping)
ENDIF(BUILD_WRAPPERS)


# install devel files
FOREACH(f itkMorphologicalWatershedFromMarkersImageFilter.h itkMorphologicalWatershedImageFilter.h itkMorphologicalWatershedFromMarkersImageFilter.txx itkMorphologicalWatershedImageFilter.txx)
  INSTALL_FILES(/include/InsightToolkit/BasicFilters FILES ${CMAKE_CURRENT_SOURCE_DIR}/${f})
ENDFOREACH(f)

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project
SET(CurrentExe "ws")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "ws3")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "wsm")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "wst")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "wsorig")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "perf3D")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
# ADD_TEST(Testname ExecutableToRun arg1 arg2 arg3)

ADD_TEST(ButtonHoleMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/button-hole.png button-hole-MF.png)
ADD_TEST(ButtonHoleMFCompare ImageCompare button-hole-MF.png ${CMAKE_SOURCE_DIR}/images/button-hole-MF.png)

ADD_TEST(ButtonHolemF ws 0 1 ${CMAKE_SOURCE_DIR}/images/button-hole.png button-hole-mF.png)
ADD_TEST(ButtonHolemFCompare ImageCompare button-hole-mF.png ${CMAKE_SOURCE_DIR}/images/button-hole-mF.png)

ADD_TEST(ButtonHoleMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/button-hole.png button-hole-Mf.png)
ADD_TEST(ButtonHoleMfCompare ImageCompare button-hole-Mf.png ${CMAKE_SOURCE_DIR}/images/button-hole-Mf.png)

ADD_TEST(ButtonHolemf ws 0 0 ${CMAKE_SOURCE_DIR}/images/button-hole.png button-hole-mf.png)
ADD_TEST(ButtonHolemfCompare ImageCompare button-hole-mf.png ${CMAKE_SOURCE_DIR}/images/button-hole-mf.png)

ADD_TEST(PassValueMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/pass-values.png pass-values-MF.png)
ADD_TEST(PassValueMFCompare ImageCompare pass-values-MF.png ${CMAKE_SOURCE_DIR}/images/pass-values-MF.png)

ADD_TEST(PassValuemF ws 0 1 ${CMAKE_SOURCE_DIR}/images/pass-values.png pass-values-mF.png)
ADD_TEST(PassValuemFCompare ImageCompare pass-values-mF.png ${CMAKE_SOURCE_DIR}/images/pass-values-mF.png)

ADD_TEST(PassValueMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/pass-values.png pass-values-Mf.png)
ADD_TEST(PassValueMfCompare ImageCompare pass-values-Mf.png ${CMAKE_SOURCE_DIR}/images/pass-values-Mf.png)

ADD_TEST(PassValuemf ws 0 0 ${CMAKE_SOURCE_DIR}/images/pass-values.png pass-values-mf.png)
ADD_TEST(PassValuemfCompare ImageCompare pass-values-mf.png ${CMAKE_SOURCE_DIR}/images/pass-values-mf.png)

ADD_TEST(ThickLinesMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/thick-lines.png thick-lines-MF.png)
ADD_TEST(ThickLinesMFCompare ImageCompare thick-lines-MF.png ${CMAKE_SOURCE_DIR}/images/thick-lines-MF.png)

ADD_TEST(ThickLinesmF ws 0 1 ${CMAKE_SOURCE_DIR}/images/thick-lines.png thick-lines-mF.png)
ADD_TEST(ThickLinesmFCompare ImageCompare thick-lines-mF.png ${CMAKE_SOURCE_DIR}/images/thick-lines-mF.png)

ADD_TEST(ThickLinesMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/thick-lines.png thick-lines-Mf.png)
ADD_TEST(ThickLinesMfCompare ImageCompare thick-lines-Mf.png ${CMAKE_SOURCE_DIR}/images/thick-lines-Mf.png)

ADD_TEST(ThickLinesmf ws 0 0 ${CMAKE_SOURCE_DIR}/images/thick-lines.png thick-lines-mf.png)
ADD_TEST(ThickLinesmfCompare ImageCompare thick-lines-mf.png ${CMAKE_SOURCE_DIR}/images/thick-lines-mf.png)

ADD_TEST(PlateauMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/plateau.png plateau-MF.png)
ADD_TEST(PlateauMFCompare ImageCompare plateau-MF.png ${CMAKE_SOURCE_DIR}/images/plateau-MF.png)

ADD_TEST(PlateaumF ws 0 1 ${CMAKE_SOURCE_DIR}/images/plateau.png plateau-mF.png)
ADD_TEST(PlateaumFCompare ImageCompare plateau-mF.png ${CMAKE_SOURCE_DIR}/images/plateau-mF.png)

ADD_TEST(PlateauMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/plateau.png plateau-Mf.png)
ADD_TEST(PlateauMfCompare ImageCompare plateau-Mf.png ${CMAKE_SOURCE_DIR}/images/plateau-Mf.png)

ADD_TEST(Plateaumf ws 0 0 ${CMAKE_SOURCE_DIR}/images/plateau.png plateau-mf.png)
ADD_TEST(PlateaumfCompare ImageCompare plateau-mf.png ${CMAKE_SOURCE_DIR}/images/plateau-mf.png)


ADD_TEST(BlackMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/black.png black-MF.png)
ADD_TEST(BlackMFCompare ImageCompare black-MF.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(BlackmF ws 0 1 ${CMAKE_SOURCE_DIR}/images/black.png black-mF.png)
ADD_TEST(BlackmFCompare ImageCompare black-mF.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(BlackMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/black.png black-Mf.png)
ADD_TEST(BlackMfCompare ImageCompare black-Mf.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(Blackmf ws 0 0 ${CMAKE_SOURCE_DIR}/images/black.png black-mf.png)
ADD_TEST(BlackmfCompare ImageCompare black-mf.png ${CMAKE_SOURCE_DIR}/images/white.png)


ADD_TEST(WhiteMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/white.png white-MF.png)
ADD_TEST(WhiteMFCompare ImageCompare white-MF.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(WhitemF ws 0 1 ${CMAKE_SOURCE_DIR}/images/white.png white-mF.png)
ADD_TEST(WhitemFCompare ImageCompare white-mF.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(WhiteMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/white.png white-Mf.png)
ADD_TEST(WhiteMfCompare ImageCompare white-Mf.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(Whitemf ws 0 0 ${CMAKE_SOURCE_DIR}/images/white.png white-mf.png)
ADD_TEST(WhitemfCompare ImageCompare white-mf.png ${CMAKE_SOURCE_DIR}/images/white.png)


ADD_TEST(3x3MF wsm 1 1 ${CMAKE_SOURCE_DIR}/images/3x3-input.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png 3x3MF.png)
ADD_TEST(3x3MFCompare ImageCompare 3x3MF.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png)

ADD_TEST(3x3Mf wsm 1 0 ${CMAKE_SOURCE_DIR}/images/3x3-input.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png 3x3Mf.png)
ADD_TEST(3x3MfCompare ImageCompare 3x3Mf.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png)

ADD_TEST(3x3mF wsm 0 1 ${CMAKE_SOURCE_DIR}/images/3x3-input.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png 3x3mF.png)
ADD_TEST(3x3mFCompare ImageCompare 3x3mF.png ${CMAKE_SOURCE_DIR}/images/3x3m.png)

ADD_TEST(3x3mf wsm 0 0 ${CMAKE_SOURCE_DIR}/images/3x3-input.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png 3x3mf.png)
ADD_TEST(3x3mfCompare ImageCompare 3x3mf.png ${CMAKE_SOURCE_DIR}/images/3x3m.png)


ADD_TEST(Cthead1MF wsm 1 1 ${CMAKE_SOURCE_DIR}/images/cthead1.png ${CMAKE_SOURCE_DIR}/images/cthead1-markers.png cthead1MF.png)
ADD_TEST(Cthead1MFCompare ImageCompare cthead1MF.png ${CMAKE_SOURCE_DIR}/images/cthead1MF.png)

ADD_TEST(Cthead1Mf wsm 1 0 ${CMAKE_SOURCE_DIR}/images/cthead1.png ${CMAKE_SOURCE_DIR}/images/cthead1-markers.png cthead1Mf.png)
ADD_TEST(Cthead1MfCompare ImageCompare cthead1Mf.png ${CMAKE_SOURCE_DIR}/images/cthead1Mf.png)

ADD_TEST(Cthead1mF wsm 0 1 ${CMAKE_SOURCE_DIR}/images/cthead1.png ${CMAKE_SOURCE_DIR}/images/cthead1-markers.png cthead1mF.png)
ADD_TEST(Cthead1mFCompare ImageCompare cthead1mF.png ${CMAKE_SOURCE_DIR}/images/cthead1mF.png)

ADD_TEST(Cthead1mf wsm 0 0 ${CMAKE_SOURCE_DIR}/images/cthead1.png ${CMAKE_SOURCE_DIR}/images/cthead1-markers.png cthead1mf.png)
ADD_TEST(Cthead1mfCompare ImageCompare cthead1mf.png ${CMAKE_SOURCE_DIR}/images/cthead1mf.png)


ADD_TEST(Marker wsm 1 0 ${CMAKE_SOURCE_DIR}/images/threshold.png ${CMAKE_SOURCE_DIR}/images/threshold-markers.png threshold-markersMf.png)
ADD_TEST(MarkerCompare ImageCompare threshold-markersMf.png ${CMAKE_SOURCE_DIR}/images/threshold-markersMf.png)


ADD_TEST(Threshold10 wst 10 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold10.png)
ADD_TEST(Threshold10Compare ImageCompare threshold10.png ${CMAKE_SOURCE_DIR}/images/threshold10.png)

ADD_TEST(Threshold20 wst 20 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold20.png)
ADD_TEST(Threshold20Compare ImageCompare threshold20.png ${CMAKE_SOURCE_DIR}/images/threshold20.png)

ADD_TEST(Threshold30 wst 30 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold30.png)
ADD_TEST(Threshold30Compare ImageCompare threshold30.png ${CMAKE_SOURCE_DIR}/images/threshold30.png)

ADD_TEST(Threshold40 wst 40 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold40.png)
ADD_TEST(Threshold40Compare ImageCompare threshold40.png ${CMAKE_SOURCE_DIR}/images/threshold40.png)

ADD_TEST(Threshold50 wst 50 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold50.png)
ADD_TEST(Threshold50Compare ImageCompare threshold50.png ${CMAKE_SOURCE_DIR}/images/threshold50.png)


ADD_TEST(ESCellsMf30 ws3 1 0 30 ${CMAKE_SOURCE_DIR}/images/ESCells.img ESCellsMf30.tif)
ADD_TEST(ESCellsMf30Compare ImageCompare ESCellsMf30.tif ${CMAKE_SOURCE_DIR}/images/ESCellsMf30.tif)

ADD_TEST(ESCellsmf30 ws3 0 0 30 ${CMAKE_SOURCE_DIR}/images/ESCells.img ESCellsmf30.tif)
ADD_TEST(ESCellsmf30Compare ImageCompare ESCellsmf30.tif ${CMAKE_SOURCE_DIR}/images/ESCellsmf30.tif)
