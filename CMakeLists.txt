#Change PROJECT_NAME to the name of your project
PROJECT(watershed)

# set the name of the input image used to run the test
SET(INPUT_IMAGE "")

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
)

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project
SET(CurrentExe "ws")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "wsm")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "wst")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "rmin")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
# ADD_TEST(Testname ExecutableToRun arg1 arg2 arg3)

ADD_TEST(ButtonHoleMF ws 1 1 ${CMAKE_SOURCE_DIR}/button-hole.png button-hole-MF.png)
ADD_TEST(ButtonHoleMFCompare ImageCompare button-hole-MF.png ${CMAKE_SOURCE_DIR}/button-hole-MF.png)

ADD_TEST(ButtonHolemF ws 0 1 ${CMAKE_SOURCE_DIR}/button-hole.png button-hole-mF.png)
ADD_TEST(ButtonHolemFCompare ImageCompare button-hole-mF.png ${CMAKE_SOURCE_DIR}/button-hole-mF.png)

ADD_TEST(ButtonHoleMf ws 1 0 ${CMAKE_SOURCE_DIR}/button-hole.png button-hole-Mf.png)
ADD_TEST(ButtonHoleMfCompare ImageCompare button-hole-Mf.png ${CMAKE_SOURCE_DIR}/button-hole-Mf.png)

ADD_TEST(ButtonHolemf ws 0 0 ${CMAKE_SOURCE_DIR}/button-hole.png button-hole-mf.png)
ADD_TEST(ButtonHolemfCompare ImageCompare button-hole-mf.png ${CMAKE_SOURCE_DIR}/button-hole-mf.png)

ADD_TEST(PassValueMF ws 1 1 ${CMAKE_SOURCE_DIR}/pass-values.png pass-values-MF.png)
ADD_TEST(PassValueMFCompare ImageCompare pass-values-MF.png ${CMAKE_SOURCE_DIR}/pass-values-MF.png)

ADD_TEST(PassValuemF ws 0 1 ${CMAKE_SOURCE_DIR}/pass-values.png pass-values-mF.png)
ADD_TEST(PassValuemFCompare ImageCompare pass-values-mF.png ${CMAKE_SOURCE_DIR}/pass-values-mF.png)

ADD_TEST(PassValueMf ws 1 0 ${CMAKE_SOURCE_DIR}/pass-values.png pass-values-Mf.png)
ADD_TEST(PassValueMfCompare ImageCompare pass-values-Mf.png ${CMAKE_SOURCE_DIR}/pass-values-Mf.png)

ADD_TEST(PassValuemf ws 0 0 ${CMAKE_SOURCE_DIR}/pass-values.png pass-values-mf.png)
ADD_TEST(PassValuemfCompare ImageCompare pass-values-mf.png ${CMAKE_SOURCE_DIR}/pass-values-mf.png)

ADD_TEST(ThickLinesMF ws 1 1 ${CMAKE_SOURCE_DIR}/thick-lines.png thick-lines-MF.png)
ADD_TEST(ThickLinesMFCompare ImageCompare thick-lines-MF.png ${CMAKE_SOURCE_DIR}/thick-lines-MF.png)

ADD_TEST(ThickLinesmF ws 0 1 ${CMAKE_SOURCE_DIR}/thick-lines.png thick-lines-mF.png)
ADD_TEST(ThickLinesmFCompare ImageCompare thick-lines-mF.png ${CMAKE_SOURCE_DIR}/thick-lines-mF.png)

ADD_TEST(ThickLinesMf ws 1 0 ${CMAKE_SOURCE_DIR}/thick-lines.png thick-lines-Mf.png)
ADD_TEST(ThickLinesMfCompare ImageCompare thick-lines-Mf.png ${CMAKE_SOURCE_DIR}/thick-lines-Mf.png)

ADD_TEST(ThickLinesmf ws 0 0 ${CMAKE_SOURCE_DIR}/thick-lines.png thick-lines-mf.png)
ADD_TEST(ThickLinesmfCompare ImageCompare thick-lines-mf.png ${CMAKE_SOURCE_DIR}/thick-lines-mf.png)

ADD_TEST(PlateauMF ws 1 1 ${CMAKE_SOURCE_DIR}/plateau.png plateau-MF.png)
ADD_TEST(PlateauMFCompare ImageCompare plateau-MF.png ${CMAKE_SOURCE_DIR}/plateau-MF.png)

ADD_TEST(PlateaumF ws 0 1 ${CMAKE_SOURCE_DIR}/plateau.png plateau-mF.png)
ADD_TEST(PlateaumFCompare ImageCompare plateau-mF.png ${CMAKE_SOURCE_DIR}/plateau-mF.png)

ADD_TEST(PlateauMf ws 1 0 ${CMAKE_SOURCE_DIR}/plateau.png plateau-Mf.png)
ADD_TEST(PlateauMfCompare ImageCompare plateau-Mf.png ${CMAKE_SOURCE_DIR}/plateau-Mf.png)

ADD_TEST(Plateaumf ws 0 0 ${CMAKE_SOURCE_DIR}/plateau.png plateau-mf.png)
ADD_TEST(PlateaumfCompare ImageCompare plateau-mf.png ${CMAKE_SOURCE_DIR}/plateau-mf.png)

ADD_TEST(3x3MF wsm 1 1 ${CMAKE_SOURCE_DIR}/3x3-input.png ${CMAKE_SOURCE_DIR}/3x3-markers.png 3x3MF.png)
ADD_TEST(3x3MFCompare ImageCompare 3x3MF.png ${CMAKE_SOURCE_DIR}/3x3-markers.png)

ADD_TEST(3x3Mf wsm 1 0 ${CMAKE_SOURCE_DIR}/3x3-input.png ${CMAKE_SOURCE_DIR}/3x3-markers.png 3x3Mf.png)
ADD_TEST(3x3MfCompare ImageCompare 3x3Mf.png ${CMAKE_SOURCE_DIR}/3x3-markers.png)

ADD_TEST(3x3mF wsm 0 1 ${CMAKE_SOURCE_DIR}/3x3-input.png ${CMAKE_SOURCE_DIR}/3x3-markers.png 3x3mF.png)
ADD_TEST(3x3mFCompare ImageCompare 3x3mF.png ${CMAKE_SOURCE_DIR}/3x3m.png)

ADD_TEST(3x3mf wsm 0 0 ${CMAKE_SOURCE_DIR}/3x3-input.png ${CMAKE_SOURCE_DIR}/3x3-markers.png 3x3mf.png)
ADD_TEST(3x3mfCompare ImageCompare 3x3mf.png ${CMAKE_SOURCE_DIR}/3x3m.png)

ADD_TEST(Marker wsm 1 0 ${CMAKE_SOURCE_DIR}/threshold.png ${CMAKE_SOURCE_DIR}/threshold-markers.png threshold-markersMf.png)
ADD_TEST(MarkerCompare ImageCompare threshold-markersMf.png ${CMAKE_SOURCE_DIR}/threshold-markersMf.png)

ADD_TEST(Threshold10 wst 10 ${CMAKE_SOURCE_DIR}/threshold.png threshold10.png)
ADD_TEST(Threshold10Compare ImageCompare threshold10.png ${CMAKE_SOURCE_DIR}/threshold10.png)

ADD_TEST(Threshold20 wst 20 ${CMAKE_SOURCE_DIR}/threshold.png threshold20.png)
ADD_TEST(Threshold20Compare ImageCompare threshold20.png ${CMAKE_SOURCE_DIR}/threshold20.png)

ADD_TEST(Threshold30 wst 30 ${CMAKE_SOURCE_DIR}/threshold.png threshold30.png)
ADD_TEST(Threshold30Compare ImageCompare threshold30.png ${CMAKE_SOURCE_DIR}/threshold30.png)

ADD_TEST(Threshold40 wst 40 ${CMAKE_SOURCE_DIR}/threshold.png threshold40.png)
ADD_TEST(Threshold40Compare ImageCompare threshold40.png ${CMAKE_SOURCE_DIR}/threshold40.png)

ADD_TEST(Threshold50 wst 50 ${CMAKE_SOURCE_DIR}/threshold.png threshold50.png)
ADD_TEST(Threshold50Compare ImageCompare threshold50.png ${CMAKE_SOURCE_DIR}/threshold50.png)

ADD_TEST(RMinF rmin 1 ${CMAKE_SOURCE_DIR}/threshold.png threshold-rminF.png threshold-rminF-ref.png)
ADD_TEST(RMinFCompare ImageCompare threshold-rminF.png threshold-rminF-ref.png)

ADD_TEST(RMinf rmin 0 ${CMAKE_SOURCE_DIR}/threshold.png threshold-rminf.png threshold-rminf-ref.png)
ADD_TEST(RMinfCompare ImageCompare threshold-rminf.png threshold-rminf-ref.png)

ADD_TEST(RMinBlack rmin 0 ${CMAKE_SOURCE_DIR}/black.png black-rminf.png black-rminf-ref.png)
ADD_TEST(RMinBlackCompare ImageCompare black-rminf.png black-rminf-ref.png)

ADD_TEST(RMinWhite rmin 0 ${CMAKE_SOURCE_DIR}/white.png white-rminf.png white-rminf-ref.png)
ADD_TEST(RMinWhiteCompare ImageCompare white-rminf.png white-rminf-ref.png)



