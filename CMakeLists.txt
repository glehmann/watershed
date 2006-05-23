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

SET(CurrentExe "wsl")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "iwsl")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "perf3D")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "mperf3D")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "testEquiv")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "color")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "splitCells")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "splitPronucleus2D")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "segGel")
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



ADD_TEST(Marker wsm 1 0 ${CMAKE_SOURCE_DIR}/images/level.png ${CMAKE_SOURCE_DIR}/images/level-markers.png level-markersMf.png level-markersMf-rgb.png 0.5)
ADD_TEST(MarkerCompare testEquiv level-markersMf.png ${CMAKE_SOURCE_DIR}/images/level-markersMf.png)
ADD_TEST(MarkerRGBCompare ${IMAGE_COMPARE} level-markersMf-rgb.png ${CMAKE_SOURCE_DIR}/images/level-markersMf-rgb.png)

ADD_TEST(MarkerITK iwsm 1 0 ${CMAKE_SOURCE_DIR}/images/level.png ${CMAKE_SOURCE_DIR}/images/level-markers.png level-markers-itk.png level-markers-itk-rgb.png 0.5)
ADD_TEST(MarkerITKCompare testEquiv level-markers-itk.png ${CMAKE_SOURCE_DIR}/images/level-markers-itk.png)
ADD_TEST(MarkerITKRGBCompare ${IMAGE_COMPARE} level-markers-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/level-markers-itk-rgb.png)



ADD_TEST(Level0 wsl 0 ${CMAKE_SOURCE_DIR}/images/level.png level00.png level00-rgb.png 0.5)
ADD_TEST(Level0Compare testEquiv level00.png ${CMAKE_SOURCE_DIR}/images/level00.png)
ADD_TEST(Level0RGBCompare ${IMAGE_COMPARE} level00-rgb.png ${CMAKE_SOURCE_DIR}/images/level00-rgb.png)

ADD_TEST(Level10 wsl 10 ${CMAKE_SOURCE_DIR}/images/level.png level10.png level10-rgb.png 0.5)
ADD_TEST(Level10Compare testEquiv level10.png ${CMAKE_SOURCE_DIR}/images/level10.png)
ADD_TEST(Level10RGBCompare ${IMAGE_COMPARE} level10-rgb.png ${CMAKE_SOURCE_DIR}/images/level10-rgb.png)

ADD_TEST(Level20 wsl 20 ${CMAKE_SOURCE_DIR}/images/level.png level20.png level20-rgb.png 0.5)
ADD_TEST(Level20Compare testEquiv level20.png ${CMAKE_SOURCE_DIR}/images/level20.png)
ADD_TEST(Level20RGBCompare ${IMAGE_COMPARE} level20-rgb.png ${CMAKE_SOURCE_DIR}/images/level20-rgb.png)

ADD_TEST(Level30 wsl 30 ${CMAKE_SOURCE_DIR}/images/level.png level30.png level30-rgb.png 0.5)
ADD_TEST(Level30Compare testEquiv level30.png ${CMAKE_SOURCE_DIR}/images/level30.png)
ADD_TEST(Level30RGBCompare ${IMAGE_COMPARE} level30-rgb.png ${CMAKE_SOURCE_DIR}/images/level30-rgb.png)

ADD_TEST(Level40 wsl 40 ${CMAKE_SOURCE_DIR}/images/level.png level40.png level40-rgb.png 0.5)
ADD_TEST(Level40Compare testEquiv level40.png ${CMAKE_SOURCE_DIR}/images/level40.png)
ADD_TEST(Level40RGBCompare ${IMAGE_COMPARE} level40-rgb.png ${CMAKE_SOURCE_DIR}/images/level40-rgb.png)

ADD_TEST(Level50 wsl 50 ${CMAKE_SOURCE_DIR}/images/level.png level50.png level50-rgb.png 0.5)
ADD_TEST(Level50Compare testEquiv level50.png ${CMAKE_SOURCE_DIR}/images/level50.png)
ADD_TEST(Level50RGBCompare ${IMAGE_COMPARE} level50-rgb.png ${CMAKE_SOURCE_DIR}/images/level50-rgb.png)



ADD_TEST(LevelITK0 iwsl 0 ${CMAKE_SOURCE_DIR}/images/level.png level00-itk.png level00-itk-rgb.png 0.5)
ADD_TEST(LevelITK0Compare testEquiv level00-itk.png ${CMAKE_SOURCE_DIR}/images/level00-itk.png)
ADD_TEST(LevelITK0RGBCompare ${IMAGE_COMPARE} level00-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/level00-itk-rgb.png)

ADD_TEST(LevelITK10 iwsl 0.10 ${CMAKE_SOURCE_DIR}/images/level.png level10-itk.png level10-itk-rgb.png 0.5)
ADD_TEST(LevelITK10Compare testEquiv level10-itk.png ${CMAKE_SOURCE_DIR}/images/level10-itk.png)
ADD_TEST(LevelITK10RGBCompare ${IMAGE_COMPARE} level10-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/level10-itk-rgb.png)

ADD_TEST(LevelITK20 iwsl 0.20 ${CMAKE_SOURCE_DIR}/images/level.png level20-itk.png level20-itk-rgb.png 0.5)
ADD_TEST(LevelITK20Compare testEquiv level20-itk.png ${CMAKE_SOURCE_DIR}/images/level20-itk.png)
ADD_TEST(LevelITK20RGBCompare ${IMAGE_COMPARE} level20-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/level20-itk-rgb.png)

ADD_TEST(LevelITK40 iwsl 0.40 ${CMAKE_SOURCE_DIR}/images/level.png level40-itk.png level40-itk-rgb.png 0.5)
ADD_TEST(LevelITK40Compare testEquiv level40-itk.png ${CMAKE_SOURCE_DIR}/images/level40-itk.png)
ADD_TEST(LevelITK40RGBCompare ${IMAGE_COMPARE} level40-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/level40-itk-rgb.png)

ADD_TEST(LevelITK63 iwsl 0.63 ${CMAKE_SOURCE_DIR}/images/level.png level63-itk.png level63-itk-rgb.png 0.5)
ADD_TEST(LevelITK63Compare testEquiv level63-itk.png ${CMAKE_SOURCE_DIR}/images/level63-itk.png)
ADD_TEST(LevelITK63RGBCompare ${IMAGE_COMPARE} level63-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/level63-itk-rgb.png)

ADD_TEST(LevelITK65 iwsl 0.65 ${CMAKE_SOURCE_DIR}/images/level.png level65-itk.png level65-itk-rgb.png 0.5)
ADD_TEST(LevelITK65Compare testEquiv level65-itk.png ${CMAKE_SOURCE_DIR}/images/level65-itk.png)
ADD_TEST(LevelITK65RGBCompare ${IMAGE_COMPARE} level65-itk-rgb.png ${CMAKE_SOURCE_DIR}/images/level65-itk-rgb.png)



ADD_TEST(ESCellsMf30 ws3 1 0 30 ${CMAKE_SOURCE_DIR}/images/ESCells.img ESCellsMf30.tif)
ADD_TEST(ESCellsMf30Compare testEquiv ESCellsMf30.tif ${CMAKE_SOURCE_DIR}/images/ESCellsMf30.tif)

ADD_TEST(ESCellsmf30 ws3 0 0 30 ${CMAKE_SOURCE_DIR}/images/ESCells.img ESCellsmf30.tif)
ADD_TEST(ESCellsmf30Compare testEquiv ESCellsmf30.tif ${CMAKE_SOURCE_DIR}/images/ESCellsmf30.tif)



ADD_TEST(ColoredLevelMarkers color ${CMAKE_SOURCE_DIR}/images/level-markers.png level-markers-rgb.png 1 0)
ADD_TEST(ColoredLevelMarkersCompare ${IMAGE_COMPARE} level-markers-rgb.png ${CMAKE_SOURCE_DIR}/images/level-markers-rgb.png)

ADD_TEST(ColoredCthead1Markers color ${CMAKE_SOURCE_DIR}/images/cthead1-markers.png cthead1-markers-rgb.png 1 0)
ADD_TEST(ColoredCthead1MarkersCompare ${IMAGE_COMPARE} cthead1-markers-rgb.png ${CMAKE_SOURCE_DIR}/images/cthead1-markers-rgb.png)

ADD_TEST(ColoredOverSegmentation iws 0 0 ${CMAKE_SOURCE_DIR}/images/cthead1.png dummy.png cthead1-over-segmentation-rgb.png 0.5)
ADD_TEST(ColoredOverSegmentationCompare ${IMAGE_COMPARE} cthead1-over-segmentation-rgb.png ${CMAKE_SOURCE_DIR}/images/cthead1-over-segmentation-rgb.png)


ADD_TEST(ColoredCircle1 iws 0 0 ${CMAKE_SOURCE_DIR}/images/circle_plateau.tif dummy.png circle_rgb1.png)
ADD_TEST(ColoredCircle1Compare ${IMAGE_COMPARE} circle_rgb1.png ${CMAKE_SOURCE_DIR}/images/circle_rgb1.png)

ADD_TEST(ColoredCircle2 ws 0 0 ${CMAKE_SOURCE_DIR}/images/circle_plateau.tif dummy.png circle_rgb2.png)
ADD_TEST(ColoredCircle2Compare ${IMAGE_COMPARE} circle_rgb2.png ${CMAKE_SOURCE_DIR}/images/circle_rgb2.png)

ADD_TEST(SplitPronucleus2D splitPronucleus2D ${CMAKE_SOURCE_DIR}/images/embryo.png)
ADD_TEST(SplitPronucleus2DdistanceCompare ${IMAGE_COMPARE} embryo-distance.png ${CMAKE_SOURCE_DIR}/images/embryo-distance.png)
ADD_TEST(SplitPronucleus2DfillCompare ${IMAGE_COMPARE} embryo-fill.png ${CMAKE_SOURCE_DIR}/images/embryo-fill.png)
ADD_TEST(SplitPronucleus2DidistanceCompare ${IMAGE_COMPARE} embryo-idistance.png ${CMAKE_SOURCE_DIR}/images/embryo-idistance.png)
ADD_TEST(SplitPronucleus2DinvertCompare ${IMAGE_COMPARE} embryo-invert.png ${CMAKE_SOURCE_DIR}/images/embryo-invert.png)
ADD_TEST(SplitPronucleus2DmedianCompare ${IMAGE_COMPARE} embryo-median.png ${CMAKE_SOURCE_DIR}/images/embryo-median.png)
ADD_TEST(SplitPronucleus2DopenCompare ${IMAGE_COMPARE} embryo-open.png ${CMAKE_SOURCE_DIR}/images/embryo-open.png)
ADD_TEST(SplitPronucleus2Doverlay2Compare ${IMAGE_COMPARE} embryo-overlay2.png ${CMAKE_SOURCE_DIR}/images/embryo-overlay2.png)
ADD_TEST(SplitPronucleus2DoverlayCompare ${IMAGE_COMPARE} embryo-overlay.png ${CMAKE_SOURCE_DIR}/images/embryo-overlay.png)
ADD_TEST(SplitPronucleus2DthCompare ${IMAGE_COMPARE} embryo-th.png ${CMAKE_SOURCE_DIR}/images/embryo-th.png)
