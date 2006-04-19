#Change PROJECT_NAME to the name of your project
PROJECT(watershed)

#include some macros from another file...
INCLUDE(IJMacros.txt)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)
FIND_PROGRAM(IMAGE_COMPARE ImageCompare)

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
OPTION(INSTALL_DEVEL_FILES "Install C++ headers" ON)
IF(INSTALL_DEVEL_FILES)
FOREACH(f itkMorphologicalWatershedFromMarkersImageFilter.h itkMorphologicalWatershedImageFilter.h itkMorphologicalWatershedFromMarkersImageFilter.txx itkMorphologicalWatershedImageFilter.txx)
  INSTALL_FILES(/include/InsightToolkit/BasicFilters FILES ${CMAKE_CURRENT_SOURCE_DIR}/${f})
ENDFOREACH(f)
ENDIF(INSTALL_DEVEL_FILES)

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project

IF(BUILD_TESTING)

SET(CurrentExe "ws")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "iws")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "ws3")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "wsm")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "iwsm")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "wst")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "iwst")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "wsorig")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "perf3D")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "testEquiv")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "lt")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

ENDIF(BUILD_TESTING)

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
# ADD_TEST(Testname ExecutableToRun arg1 arg2 arg3)

ADD_TEST(ButtonHoleMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/button-hole.png button-hole-MF.png button-hole-MF-rgb.png 1)
ADD_TEST(ButtonHoleMFCompare testEquiv button-hole-MF.png ${CMAKE_SOURCE_DIR}/images/button-hole-MF.png)
ADD_TEST(ButtonHoleMFRGBCompare ${IMAGE_COMPARE} button-hole-MF-rgb.png ${CMAKE_SOURCE_DIR}/images/button-hole-MF-rgb.png)

ADD_TEST(ButtonHolemF ws 0 1 ${CMAKE_SOURCE_DIR}/images/button-hole.png button-hole-mF.png button-hole-mF-rgb.png 1)
ADD_TEST(ButtonHolemFCompare testEquiv button-hole-mF.png ${CMAKE_SOURCE_DIR}/images/button-hole-mF.png)
ADD_TEST(ButtonHolemFRGBCompare ${IMAGE_COMPARE} button-hole-mF-rgb.png ${CMAKE_SOURCE_DIR}/images/button-hole-mF-rgb.png)

ADD_TEST(ButtonHoleMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/button-hole.png button-hole-Mf.png button-hole-Mf-rgb.png 1)
ADD_TEST(ButtonHoleMfCompare testEquiv button-hole-Mf.png ${CMAKE_SOURCE_DIR}/images/button-hole-Mf.png)
ADD_TEST(ButtonHoleMfRGBCompare ${IMAGE_COMPARE} button-hole-Mf-rgb.png ${CMAKE_SOURCE_DIR}/images/button-hole-Mf-rgb.png)

ADD_TEST(ButtonHolemf ws 0 0 ${CMAKE_SOURCE_DIR}/images/button-hole.png button-hole-mf.png button-hole-mf-rgb.png 1)
ADD_TEST(ButtonHolemfCompare testEquiv button-hole-mf.png ${CMAKE_SOURCE_DIR}/images/button-hole-mf.png)
ADD_TEST(ButtonHolemfRGBCompare ${IMAGE_COMPARE} button-hole-mf-rgb.png ${CMAKE_SOURCE_DIR}/images/button-hole-mf-rgb.png)

ADD_TEST(ButtonHoleITK iws 0 0 ${CMAKE_SOURCE_DIR}/images/button-hole.png button-hole-itk.png button-hole-itk-rgb.png 1)
ADD_TEST(ButtonHoleITKCompare testEquiv button-hole-itk.png ${CMAKE_SOURCE_DIR}/images/button-hole-itk.png)
ADD_TEST(ButtonHoleITKRGBCompare ${IMAGE_COMPARE} button-hole-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/button-hole-itk-rgb.png)



ADD_TEST(PassValueMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/pass-values.png pass-values-MF.png pass-values-MF-rgb.png 1)
ADD_TEST(PassValueMFCompare testEquiv pass-values-MF.png ${CMAKE_SOURCE_DIR}/images/pass-values-MF.png)
ADD_TEST(PassValueMFRGBCompare ${IMAGE_COMPARE} pass-values-MF-rgb.png ${CMAKE_SOURCE_DIR}/images/pass-values-MF-rgb.png)

ADD_TEST(PassValuemF ws 0 1 ${CMAKE_SOURCE_DIR}/images/pass-values.png pass-values-mF.png pass-values-mF-rgb.png 1)
ADD_TEST(PassValuemFCompare testEquiv pass-values-mF.png ${CMAKE_SOURCE_DIR}/images/pass-values-mF.png)
ADD_TEST(PassValuemFRGBCompare ${IMAGE_COMPARE} pass-values-mF-rgb.png ${CMAKE_SOURCE_DIR}/images/pass-values-mF-rgb.png)

ADD_TEST(PassValueMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/pass-values.png pass-values-Mf.png pass-values-Mf-rgb.png 1)
ADD_TEST(PassValueMfCompare testEquiv pass-values-Mf.png ${CMAKE_SOURCE_DIR}/images/pass-values-Mf.png)
ADD_TEST(PassValueMfRGBCompare ${IMAGE_COMPARE} pass-values-Mf-rgb.png ${CMAKE_SOURCE_DIR}/images/pass-values-Mf-rgb.png)

ADD_TEST(PassValuemf ws 0 0 ${CMAKE_SOURCE_DIR}/images/pass-values.png pass-values-mf.png pass-values-mf-rgb.png 1)
ADD_TEST(PassValuemfCompare testEquiv pass-values-mf.png ${CMAKE_SOURCE_DIR}/images/pass-values-mf.png)
ADD_TEST(PassValuemfRGBCompare ${IMAGE_COMPARE} pass-values-mf-rgb.png ${CMAKE_SOURCE_DIR}/images/pass-values-mf-rgb.png)

ADD_TEST(PassValueITK iws 0 0 ${CMAKE_SOURCE_DIR}/images/pass-values.png pass-values-itk.png pass-values-itk-rgb.png 1)
ADD_TEST(PassValueITKCompare testEquiv pass-values-itk.png ${CMAKE_SOURCE_DIR}/images/pass-values-itk.png)
ADD_TEST(PassValueITKRGBCompare ${IMAGE_COMPARE} pass-values-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/pass-values-itk-rgb.png)



ADD_TEST(ThickLinesMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/thick-lines.png thick-lines-MF.png thick-lines-MF-rgb.png 1)
ADD_TEST(ThickLinesMFCompare testEquiv thick-lines-MF.png ${CMAKE_SOURCE_DIR}/images/thick-lines-MF.png)
ADD_TEST(ThickLinesMFRGBCompare ${IMAGE_COMPARE} thick-lines-MF-rgb.png ${CMAKE_SOURCE_DIR}/images/thick-lines-MF-rgb.png)

ADD_TEST(ThickLinesmF ws 0 1 ${CMAKE_SOURCE_DIR}/images/thick-lines.png thick-lines-mF.png thick-lines-mF-rgb.png 1)
ADD_TEST(ThickLinesmFCompare testEquiv thick-lines-mF.png ${CMAKE_SOURCE_DIR}/images/thick-lines-mF.png)
ADD_TEST(ThickLinesmFRGBCompare ${IMAGE_COMPARE} thick-lines-mF-rgb.png ${CMAKE_SOURCE_DIR}/images/thick-lines-mF-rgb.png)

ADD_TEST(ThickLinesMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/thick-lines.png thick-lines-Mf.png thick-lines-Mf-rgb.png 1)
ADD_TEST(ThickLinesMfCompare testEquiv thick-lines-Mf.png ${CMAKE_SOURCE_DIR}/images/thick-lines-Mf.png)
ADD_TEST(ThickLinesMfRGBCompare ${IMAGE_COMPARE} thick-lines-Mf-rgb.png ${CMAKE_SOURCE_DIR}/images/thick-lines-Mf-rgb.png)

ADD_TEST(ThickLinesmf ws 0 0 ${CMAKE_SOURCE_DIR}/images/thick-lines.png thick-lines-mf.png thick-lines-mf-rgb.png 1)
ADD_TEST(ThickLinesmfCompare testEquiv thick-lines-mf.png ${CMAKE_SOURCE_DIR}/images/thick-lines-mf.png)
ADD_TEST(ThickLinesmfRGBCompare ${IMAGE_COMPARE} thick-lines-mf-rgb.png ${CMAKE_SOURCE_DIR}/images/thick-lines-mf-rgb.png)

ADD_TEST(ThickLinesITK iws 0 0 ${CMAKE_SOURCE_DIR}/images/thick-lines.png thick-lines-itk.png thick-lines-itk-rgb.png 1)
ADD_TEST(ThickLinesITKCompare testEquiv thick-lines-itk.png ${CMAKE_SOURCE_DIR}/images/thick-lines-itk.png)
ADD_TEST(ThickLinesITKRGBCompare ${IMAGE_COMPARE} thick-lines-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/thick-lines-itk-rgb.png)



ADD_TEST(PlateauMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/plateau.png plateau-MF.png plateau-MF-rgb.png 1)
ADD_TEST(PlateauMFCompare testEquiv plateau-MF.png ${CMAKE_SOURCE_DIR}/images/plateau-MF.png)
ADD_TEST(PlateauMFRGBCompare ${IMAGE_COMPARE} plateau-MF-rgb.png ${CMAKE_SOURCE_DIR}/images/plateau-MF-rgb.png)

ADD_TEST(PlateaumF ws 0 1 ${CMAKE_SOURCE_DIR}/images/plateau.png plateau-mF.png plateau-mF-rgb.png 1)
ADD_TEST(PlateaumFCompare testEquiv plateau-mF.png ${CMAKE_SOURCE_DIR}/images/plateau-mF.png)
ADD_TEST(PlateaumFRGBCompare ${IMAGE_COMPARE} plateau-mF-rgb.png ${CMAKE_SOURCE_DIR}/images/plateau-mF-rgb.png)

ADD_TEST(PlateauMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/plateau.png plateau-Mf.png plateau-Mf-rgb.png 1)
ADD_TEST(PlateauMfCompare testEquiv plateau-Mf.png ${CMAKE_SOURCE_DIR}/images/plateau-Mf.png)
ADD_TEST(PlateauMfRGBCompare ${IMAGE_COMPARE} plateau-Mf-rgb.png ${CMAKE_SOURCE_DIR}/images/plateau-Mf-rgb.png)

ADD_TEST(Plateaumf ws 0 0 ${CMAKE_SOURCE_DIR}/images/plateau.png plateau-mf.png plateau-mf-rgb.png 1)
ADD_TEST(PlateaumfCompare testEquiv plateau-mf.png ${CMAKE_SOURCE_DIR}/images/plateau-mf.png)
ADD_TEST(PlateaumfRGBCompare ${IMAGE_COMPARE} plateau-mf-rgb.png ${CMAKE_SOURCE_DIR}/images/plateau-mf-rgb.png)

ADD_TEST(PlateauITK iws 0 0 ${CMAKE_SOURCE_DIR}/images/plateau.png plateau-itk.png plateau-itk-rgb.png 1)
ADD_TEST(PlateauITKCompare testEquiv plateau-itk.png ${CMAKE_SOURCE_DIR}/images/plateau-itk.png)
ADD_TEST(PlateauITKRGBCompare ${IMAGE_COMPARE} plateau-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/plateau-itk-rgb.png)



ADD_TEST(BlackMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/black.png black-MF.png)
ADD_TEST(BlackMFCompare testEquiv black-MF.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(BlackmF ws 0 1 ${CMAKE_SOURCE_DIR}/images/black.png black-mF.png)
ADD_TEST(BlackmFCompare testEquiv black-mF.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(BlackMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/black.png black-Mf.png)
ADD_TEST(BlackMfCompare testEquiv black-Mf.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(Blackmf ws 0 0 ${CMAKE_SOURCE_DIR}/images/black.png black-mf.png)
ADD_TEST(BlackmfCompare testEquiv black-mf.png ${CMAKE_SOURCE_DIR}/images/white.png)



ADD_TEST(WhiteMF ws 1 1 ${CMAKE_SOURCE_DIR}/images/white.png white-MF.png)
ADD_TEST(WhiteMFCompare testEquiv white-MF.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(WhitemF ws 0 1 ${CMAKE_SOURCE_DIR}/images/white.png white-mF.png)
ADD_TEST(WhitemFCompare testEquiv white-mF.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(WhiteMf ws 1 0 ${CMAKE_SOURCE_DIR}/images/white.png white-Mf.png)
ADD_TEST(WhiteMfCompare testEquiv white-Mf.png ${CMAKE_SOURCE_DIR}/images/white.png)

ADD_TEST(Whitemf ws 0 0 ${CMAKE_SOURCE_DIR}/images/white.png white-mf.png)
ADD_TEST(WhitemfCompare testEquiv white-mf.png ${CMAKE_SOURCE_DIR}/images/white.png)



ADD_TEST(3x3MF wsm 1 1 ${CMAKE_SOURCE_DIR}/images/3x3-input.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png 3x3MF.png)
ADD_TEST(3x3MFCompare testEquiv 3x3MF.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png)

ADD_TEST(3x3Mf wsm 1 0 ${CMAKE_SOURCE_DIR}/images/3x3-input.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png 3x3Mf.png)
ADD_TEST(3x3MfCompare testEquiv 3x3Mf.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png)

ADD_TEST(3x3mF wsm 0 1 ${CMAKE_SOURCE_DIR}/images/3x3-input.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png 3x3mF.png)
ADD_TEST(3x3mFCompare testEquiv 3x3mF.png ${CMAKE_SOURCE_DIR}/images/3x3m.png)

ADD_TEST(3x3mf wsm 0 0 ${CMAKE_SOURCE_DIR}/images/3x3-input.png ${CMAKE_SOURCE_DIR}/images/3x3-markers.png 3x3mf.png)
ADD_TEST(3x3mfCompare testEquiv 3x3mf.png ${CMAKE_SOURCE_DIR}/images/3x3m.png)



ADD_TEST(Cthead1MF wsm 1 1 ${CMAKE_SOURCE_DIR}/images/cthead1.png ${CMAKE_SOURCE_DIR}/images/cthead1-markers.png cthead1MF.png cthead1MF-rgb.png 0.5)
ADD_TEST(Cthead1MFCompare testEquiv cthead1MF.png ${CMAKE_SOURCE_DIR}/images/cthead1MF.png)
ADD_TEST(Cthead1MFRGBCompare ${IMAGE_COMPARE} cthead1MF.png ${CMAKE_SOURCE_DIR}/images/cthead1MF.png)

ADD_TEST(Cthead1Mf wsm 1 0 ${CMAKE_SOURCE_DIR}/images/cthead1.png ${CMAKE_SOURCE_DIR}/images/cthead1-markers.png cthead1Mf.png cthead1Mf-rgb.png 0.5)
ADD_TEST(Cthead1MfCompare testEquiv cthead1Mf.png ${CMAKE_SOURCE_DIR}/images/cthead1Mf.png)
ADD_TEST(Cthead1MfRGBCompare ${IMAGE_COMPARE} cthead1Mf.png ${CMAKE_SOURCE_DIR}/images/cthead1Mf.png)

ADD_TEST(Cthead1mF wsm 0 1 ${CMAKE_SOURCE_DIR}/images/cthead1.png ${CMAKE_SOURCE_DIR}/images/cthead1-markers.png cthead1mF.png cthead1mF-rgb.png 0.5)
ADD_TEST(Cthead1mFCompare testEquiv cthead1mF.png ${CMAKE_SOURCE_DIR}/images/cthead1mF.png)
ADD_TEST(Cthead1mFRGBCompare ${IMAGE_COMPARE} cthead1mF.png ${CMAKE_SOURCE_DIR}/images/cthead1mF.png)

ADD_TEST(Cthead1mf wsm 0 0 ${CMAKE_SOURCE_DIR}/images/cthead1.png ${CMAKE_SOURCE_DIR}/images/cthead1-markers.png cthead1mf.png cthead1mf-rgb.png 0.5)
ADD_TEST(Cthead1mfCompare testEquiv cthead1mf.png ${CMAKE_SOURCE_DIR}/images/cthead1mf.png)
ADD_TEST(Cthead1mfRGBCompare ${IMAGE_COMPARE} cthead1mf.png ${CMAKE_SOURCE_DIR}/images/cthead1mf.png)

ADD_TEST(Cthead1ITK iwsm 0 0 ${CMAKE_SOURCE_DIR}/images/cthead1.png ${CMAKE_SOURCE_DIR}/images/cthead1-markers.png cthead1itk.png cthead1itk-rgb.png 0.5)
ADD_TEST(Cthead1ITKCompare testEquiv cthead1itk.png ${CMAKE_SOURCE_DIR}/images/cthead1itk.png)
ADD_TEST(Cthead1ITKRGBCompare ${IMAGE_COMPARE} cthead1itk.png ${CMAKE_SOURCE_DIR}/images/cthead1itk.png)



ADD_TEST(Marker wsm 1 0 ${CMAKE_SOURCE_DIR}/images/threshold.png ${CMAKE_SOURCE_DIR}/images/threshold-markers.png threshold-markersMf.png threshold-markersMf-rgb.png 0.5)
ADD_TEST(MarkerCompare testEquiv threshold-markersMf.png ${CMAKE_SOURCE_DIR}/images/threshold-markersMf.png)
ADD_TEST(MarkerRGBCompare ${IMAGE_COMPARE} threshold-markersMf-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold-markersMf-rgb.png)

ADD_TEST(MarkerITK iwsm 1 0 ${CMAKE_SOURCE_DIR}/images/threshold.png ${CMAKE_SOURCE_DIR}/images/threshold-markers.png threshold-markers-itk.png threshold-markers-itk-rgb.png 0.5)
ADD_TEST(MarkerITKCompare testEquiv threshold-markers-itk.png ${CMAKE_SOURCE_DIR}/images/threshold-markers-itk.png)
ADD_TEST(MarkerITKRGBCompare ${IMAGE_COMPARE} threshold-markers-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold-markers-itk-rgb.png)



ADD_TEST(Threshold0 wst 0 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold00.png threshold00-rgb.png 0.5)
ADD_TEST(Threshold0Compare testEquiv threshold00.png ${CMAKE_SOURCE_DIR}/images/threshold00.png)
ADD_TEST(Threshold0RGBCompare ${IMAGE_COMPARE} threshold00-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold00-rgb.png)

ADD_TEST(Threshold10 wst 10 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold10.png threshold10-rgb.png 0.5)
ADD_TEST(Threshold10Compare testEquiv threshold10.png ${CMAKE_SOURCE_DIR}/images/threshold10.png)
ADD_TEST(Threshold10RGBCompare ${IMAGE_COMPARE} threshold10-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold10-rgb.png)

ADD_TEST(Threshold20 wst 20 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold20.png threshold20-rgb.png 0.5)
ADD_TEST(Threshold20Compare testEquiv threshold20.png ${CMAKE_SOURCE_DIR}/images/threshold20.png)
ADD_TEST(Threshold20RGBCompare ${IMAGE_COMPARE} threshold20-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold20-rgb.png)

ADD_TEST(Threshold30 wst 30 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold30.png threshold30-rgb.png 0.5)
ADD_TEST(Threshold30Compare testEquiv threshold30.png ${CMAKE_SOURCE_DIR}/images/threshold30.png)
ADD_TEST(Threshold30RGBCompare ${IMAGE_COMPARE} threshold30-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold30-rgb.png)

ADD_TEST(Threshold40 wst 40 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold40.png threshold40-rgb.png 0.5)
ADD_TEST(Threshold40Compare testEquiv threshold40.png ${CMAKE_SOURCE_DIR}/images/threshold40.png)
ADD_TEST(Threshold40RGBCompare ${IMAGE_COMPARE} threshold40-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold40-rgb.png)

ADD_TEST(Threshold50 wst 50 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold50.png threshold50-rgb.png 0.5)
ADD_TEST(Threshold50Compare testEquiv threshold50.png ${CMAKE_SOURCE_DIR}/images/threshold50.png)
ADD_TEST(Threshold50RGBCompare ${IMAGE_COMPARE} threshold50-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold50-rgb.png)



ADD_TEST(ThresholdITK0 iwst 0 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold00-itk.png threshold00-itk-rgb.png 0.5)
ADD_TEST(ThresholdITK0Compare testEquiv threshold00-itk.png ${CMAKE_SOURCE_DIR}/images/threshold00-itk.png)
ADD_TEST(ThresholdITK0RGBCompare ${IMAGE_COMPARE} threshold00-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold00-itk-rgb.png)

ADD_TEST(ThresholdITK10 iwst 0.10 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold10-itk.png threshold10-itk-rgb.png 0.5)
ADD_TEST(ThresholdITK10Compare testEquiv threshold10-itk.png ${CMAKE_SOURCE_DIR}/images/threshold10-itk.png)
ADD_TEST(ThresholdITK10RGBCompare ${IMAGE_COMPARE} threshold10-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold10-itk-rgb.png)

ADD_TEST(ThresholdITK20 iwst 0.20 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold20-itk.png threshold20-itk-rgb.png 0.5)
ADD_TEST(ThresholdITK20Compare testEquiv threshold20-itk.png ${CMAKE_SOURCE_DIR}/images/threshold20-itk.png)
ADD_TEST(ThresholdITK20RGBCompare ${IMAGE_COMPARE} threshold20-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold20-itk-rgb.png)

ADD_TEST(ThresholdITK40 iwst 0.40 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold40-itk.png threshold40-itk-rgb.png 0.5)
ADD_TEST(ThresholdITK40Compare testEquiv threshold40-itk.png ${CMAKE_SOURCE_DIR}/images/threshold40-itk.png)
ADD_TEST(ThresholdITK40RGBCompare ${IMAGE_COMPARE} threshold40-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold40-itk-rgb.png)

ADD_TEST(ThresholdITK63 iwst 0.63 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold63-itk.png threshold63-itk-rgb.png 0.5)
ADD_TEST(ThresholdITK63Compare testEquiv threshold63-itk.png ${CMAKE_SOURCE_DIR}/images/threshold63-itk.png)
ADD_TEST(ThresholdITK63RGBCompare ${IMAGE_COMPARE} threshold63-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold63-itk-rgb.png)

ADD_TEST(ThresholdITK65 iwst 0.65 ${CMAKE_SOURCE_DIR}/images/threshold.png threshold65-itk.png threshold65-itk-rgb.png 0.5)
ADD_TEST(ThresholdITK65Compare testEquiv threshold65-itk.png ${CMAKE_SOURCE_DIR}/images/threshold65-itk.png)
ADD_TEST(ThresholdITK65RGBCompare ${IMAGE_COMPARE} threshold65-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/threshold65-itk-rgb.png)



ADD_TEST(ESCellsMf30 ws3 1 0 30 ${CMAKE_SOURCE_DIR}/images/ESCells.img ESCellsMf30.tif)
ADD_TEST(ESCellsMf30Compare testEquiv ESCellsMf30.tif ${CMAKE_SOURCE_DIR}/images/ESCellsMf30.tif)

ADD_TEST(ESCellsmf30 ws3 0 0 30 ${CMAKE_SOURCE_DIR}/images/ESCells.img ESCellsmf30.tif)
ADD_TEST(ESCellsmf30Compare testEquiv ESCellsmf30.tif ${CMAKE_SOURCE_DIR}/images/ESCellsmf30.tif)
