################################################################################
# Package: LHCbMath
################################################################################
gaudi_subdir(LHCbMath v3r62)

gaudi_depends_on_subdirs(GaudiGSL
                         GaudiKernel
                         GaudiPython
                         Kernel/VectorClass)

find_package(Boost)
find_package(CLHEP)
find_package(GSL)
find_package(ROOT COMPONENTS MathCore GenVector)
find_package(VDT)
find_package(Eigen)
find_package(pyanalysis)
# hide warnings from some external projects
include_directories(SYSTEM ${EIGEN_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS}
                           ${Boost_INCLUDE_DIRS} ${CLHEP_INCLUDE_DIRS})

gaudi_add_library(LHCbMathLib
                  src/*.cpp
                  PUBLIC_HEADERS LHCbMath
                  INCLUDE_DIRS Boost GSL ROOT
                  LINK_LIBRARIES Boost GSL ROOT GaudiGSLLib GaudiKernel)

gaudi_add_dictionary(LHCbMath
                     dict/LHCbMathDict.h
                     dict/LHCbMathDict.xml
                     INCLUDE_DIRS Boost GSL ROOT
                     LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib
                     OPTIONS "-U__MINGW32__")


# Determine if AVX2 is supported
set(AVX2_BUILD_FLAGS " -mavx ")
set(AVX2FMA_BUILD_FLAGS " -mavx ")
exec_program(${CMAKE_CXX_COMPILER} ARGS -print-prog-name=as OUTPUT_VARIABLE _as)
if(NOT _as)
  message(ERROR "Could not find the 'as' assembler...")
else()
  exec_program(${_as} ARGS --version OUTPUT_VARIABLE _as_version)
  string(REGEX REPLACE "\\([^\\)]*\\)" "" _as_version "${_as_version}")
  string(REGEX MATCH "[1-9]\\.[0-9]+(\\.[0-9]+)?" _as_version "${_as_version}")
  if(_as_version VERSION_LESS "2.21.0")
     message(WARNING "binutils is too old to support AVX2+FMA... Falling back to AVX only.")
  else()
     set(AVX2_BUILD_FLAGS " -mavx2 ")
     set(AVX2FMA_BUILD_FLAGS " -mavx2 -mfma ")
  endif()
endif()
message(STATUS "AVX2FMA build flags = " ${AVX2FMA_BUILD_FLAGS})
set_property(SOURCE src/Similarity_SSE3.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -msse3 " )
set_property(SOURCE src/Similarity_AVX.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -mavx " )
set_property(SOURCE src/Similarity_AVX2.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${AVX2FMA_BUILD_FLAGS} )
if(LCG_COMP STREQUAL "gcc" OR
    (BINARY_TAG_COMP_NAME STREQUAL "gcc" AND BINARY_TAG_COMP_VERSION VERSION_LESS "5"))
  set_property(SOURCE src/Similarity_AVX.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -fabi-version=0 " )
  set_property(SOURCE src/Similarity_AVX2.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -fabi-version=0 " )
endif()

if(GAUDI_BUILD_TESTS)
  gaudi_add_executable(TestPow
                       tests/pow.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestBit
                       tests/bit.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestDigit
                       tests/digit.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestKine
                       tests/kinematics.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestLomont
                       tests/TestLomontCompare.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestErrors
                       tests/withErrors.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestRound
                       tests/TestRound.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestLomont2
                       tests/TestLomontCPU.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestEigen
                       tests/TestEigen.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestBlind
                       tests/blind.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestAbs
                       tests/testAbs.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestChi2
                       tests/testChi2.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestHesse
                       tests/TestHesse.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestChi2Fit
                       tests/TestChi2Fit.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestLABug
                       tests/TestLABug.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestBloomFilter
                       tests/TestBloomFilter.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestSimilarity
                       tests/TestSimilarity.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestSimilarityTiming
                       tests/TestSimilarityTiming.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestEigenGeometry
                       tests/TestEigenGeometry.cpp
                       INCLUDE_DIRS Boost GSL ROOT VDT Eigen
                       LINK_LIBRARIES Boost GSL ROOT VDT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestXYZTypes
                       tests/TestXYZTypes.cpp
                       INCLUDE_DIRS Boost GSL ROOT VDT Eigen
                       LINK_LIBRARIES Boost GSL ROOT VDT GaudiKernel LHCbMathLib)
  gaudi_add_executable(TestPolynomials
                       tests/poly.cpp
                       INCLUDE_DIRS Boost GSL ROOT
                       LINK_LIBRARIES Boost GSL ROOT GaudiKernel LHCbMathLib)
  set_property(SOURCE tests/TestVDTMathSSE4.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -msse4.2 " )
  gaudi_add_executable(TestVDTMathSSE4
                       tests/TestVDTMathSSE4.cpp
                       INCLUDE_DIRS VDT Kernel/VectorClass
                       LINK_LIBRARIES VDT GaudiKernel LHCbMathLib)
  set_property(SOURCE tests/TestVDTMathAVX.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -mavx " )
  gaudi_add_executable(TestVDTMathAVX
                       tests/TestVDTMathAVX.cpp
                       INCLUDE_DIRS VDT Kernel/VectorClass
                       LINK_LIBRARIES VDT GaudiKernel LHCbMathLib)
  set_property(SOURCE tests/TestVDTMathAVX2.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${AVX2_BUILD_FLAGS} )
  gaudi_add_executable(TestVDTMathAVX2
                       tests/TestVDTMathAVX2.cpp
                       INCLUDE_DIRS VDT Kernel/VectorClass
                       LINK_LIBRARIES VDT GaudiKernel LHCbMathLib)
  set_property(SOURCE tests/TestVDTMathAVX2FMA.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${AVX2FMA_BUILD_FLAGS} )
  gaudi_add_executable(TestVDTMathAVX2FMA
                       tests/TestVDTMathAVX2FMA.cpp
                       INCLUDE_DIRS VDT Kernel/VectorClass
                       LINK_LIBRARIES VDT GaudiKernel LHCbMathLib)

endif()

gaudi_install_python_modules()

gaudi_add_test(QMTest QMTEST)

