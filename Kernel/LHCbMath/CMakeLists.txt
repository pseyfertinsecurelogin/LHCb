###############################################################################
# (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
# Package: LHCbMath
###############################################################################
gaudi_subdir(LHCbMath)

gaudi_depends_on_subdirs(GaudiGSL
                         GaudiKernel
                         CPPKernel
                         GaudiPython)

find_package(Boost)
find_package(CLHEP)
find_package(GSL)
find_package(ROOT COMPONENTS MathCore GenVector)
find_package(VDT)
find_package(Eigen)
find_package(pyanalysis)
find_package(Vc)
find_package(VectorClass)

# Runtime requirement since LCG_96 (no longer bundled with ROOT)
find_package(BLAS)
# Runtime requirement since LCG_96ai
# (note that a plain find_package(LAPACK) finds OpenBLAS)
find_library(LAPACK_LIBRARY NAMES lapack)
if(LAPACK_LIBRARY)
  message(STATUS "Found ${LAPACK_LIBRARY}")
  get_filename_component(LAPACK_LIBRARY_DIR "${LAPACK_LIBRARY}" PATH)
  gaudi_env(PREPEND LD_LIBRARY_PATH "${LAPACK_LIBRARY_DIR}")
endif()

# hide warnings from some external projects
include_directories(SYSTEM ${EIGEN_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS}
                           ${Boost_INCLUDE_DIRS} ${CLHEP_INCLUDE_DIRS}
                           ${Vc_INCLUDE_DIR} ${VectorClass_INCLUDE_DIR})

find_path(CPP_GSL_INCLUDE_DIR NAMES gsl/span)
if(NOT CPP_GSL_INCLUDE_DIR)
   message(FATAL "required headers from C++ GSL missing")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

gaudi_add_library(LHCbMathLib
                  src/*.cpp
                  PUBLIC_HEADERS LHCbMath
                  INCLUDE_DIRS Vc VDT Boost GSL ROOT Kernel/CPPKernel ${CPP_GSL_INCLUDE_DIR} ${VectorClass_INCLUDE_DIR}
                  LINK_LIBRARIES VDT Boost GSL ROOT VectorClass GaudiGSLLib GaudiKernel)

target_link_libraries( LHCbMathLib "${Vc_LIB_DIR}/libVc.a" )

gaudi_add_dictionary(LHCbMath
                     dict/LHCbMathDict.h
                     dict/LHCbMathDict.xml
                     INCLUDE_DIRS VDT Vc Boost GSL ROOT
                     LINK_LIBRARIES VDT Boost GSL ROOT VectorClass GaudiKernel LHCbMathLib
                     OPTIONS " -Wno-undefined-var-template -U__MINGW32__ ")

# Note - Above -Wno-undefined-var-template is to work around an issue in Vc 1.3.0.
#        To be removed once Vc is updated.

# flags to turn off AVX512.
set(NO_AVX512_FLAGS     "-mno-avx512bw -mno-avx512cd -mno-avx512dq -mno-avx512er -mno-avx512f -mno-avx512ifma -mno-avx512pf -mno-avx512vbmi -mno-avx512vl -mno-avx512vpopcntdq ")
if( BINARY_TAG_COMP_NAME STREQUAL "gcc" AND BINARY_TAG_COMP_VERSION VERSION_GREATER "7.99")
  set(NO_AVX512_FLAGS "${NO_AVX512_FLAGS} -mno-avx512vbmi2 -mno-avx512vnni ")
endif()

# Determine compiler flags for each build target
set(SSE_BUILD_FLAGS     " -msse4.2 -mno-avx -mno-avx2 -mno-fma ${NO_AVX512_FLAGS}" )
set(AVX_BUILD_FLAGS     " -mavx -mno-avx2 -mno-fma ${NO_AVX512_FLAGS}" )
set(AVX2_BUILD_FLAGS    " -mavx2 -mno-fma ${NO_AVX512_FLAGS}")
set(AVX2FMA_BUILD_FLAGS " -mavx2 -mfma ${NO_AVX512_FLAGS}")
# only use 'basic' avx512 options here..
set(AVX512_BUILD_FLAGS  " -mavx512f -mavx512cd -mavx512dq ")
exec_program(${CMAKE_CXX_COMPILER} ARGS -print-prog-name=as OUTPUT_VARIABLE _as)
if(NOT _as)
  message(ERROR "Could not find the 'as' assembler...")
else()
  exec_program(${_as} ARGS --version OUTPUT_VARIABLE _as_version)
  string(REGEX REPLACE "\\([^\\)]*\\)" "" _as_version "${_as_version}")
  string(REGEX MATCH "[1-9]\\.[0-9]+(\\.[0-9]+)?" _as_version "${_as_version}")
  if(_as_version VERSION_LESS "2.21.0")
     message(WARNING "binutils is too old to support AVX2+FMA... Falling back to AVX only.")
     set(AVX2_BUILD_FLAGS    " -mavx -mno-avx2 -mno-fma ${NO_AVX512_FLAGS}" )
     set(AVX2FMA_BUILD_FLAGS " -mavx -mno-avx2 -mno-fma ${NO_AVX512_FLAGS}" )
     set(AVX512_BUILD_FLAGS  " -mavx -mno-avx2 -mno-fma ${NO_AVX512_FLAGS}" )
  endif()
endif()

# suppress AVX512 warnings from vectorclass
# to be removed when vectorclass updated (currently 1.30) and fixes it.
set(AVX512_BUILD_FLAGS " -Wno-strict-aliasing ${AVX512_BUILD_FLAGS}" )

if(GAUDI_BUILD_TESTS)
  gaudi_add_executable(TestSimilarity
                       tests/Similarity.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestPow
                       tests/pow.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestBit
                       tests/bit.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestDigit
                       tests/digit.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestKine
                       tests/kinematics.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestTruncate
                       tests/truncate.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestLomont
                       tests/TestLomontCompare.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestErrors
                       tests/withErrors.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestRound
                       tests/TestRound.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestLomont2
                       tests/TestLomontCPU.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestEigen
                       tests/TestEigen.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestBlind
                       tests/blind.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestAbs
                       tests/testAbs.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestChi2
                       tests/testChi2.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestHesse
                       tests/TestHesse.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestChi2Fit
                       tests/TestChi2Fit.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestLABug
                       tests/TestLABug.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestBloomFilter
                       tests/TestBloomFilter.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestEigenGeometry
                       tests/TestEigenGeometry.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestXYZTypes
                       tests/TestXYZTypes.cpp
                       LINK_LIBRARIES LHCbMathLib)
  gaudi_add_executable(TestPolynomials
                       tests/poly.cpp
                       LINK_LIBRARIES LHCbMathLib)
  if(${BINARY_TAG_ARCH} IN_LIST BTU_KNOWN_x86_ARCHS)
  set_property(SOURCE tests/TestVDTMathSSE4.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${SSE_BUILD_FLAGS} )
  gaudi_add_executable(TestVDTMathSSE4
                       tests/TestVDTMathSSE4.cpp
                       LINK_LIBRARIES LHCbMathLib)
  set_property(SOURCE tests/TestVDTMathAVX.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${AVX_BUILD_FLAGS} )
  gaudi_add_executable(TestVDTMathAVX
                       tests/TestVDTMathAVX.cpp
                       LINK_LIBRARIES LHCbMathLib)
  set_property(SOURCE tests/TestVDTMathAVX2.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${AVX2_BUILD_FLAGS} )
  gaudi_add_executable(TestVDTMathAVX2
                       tests/TestVDTMathAVX2.cpp
                       LINK_LIBRARIES LHCbMathLib)
  set_property(SOURCE tests/TestVDTMathAVX2FMA.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${AVX2FMA_BUILD_FLAGS} )
  gaudi_add_executable(TestVDTMathAVX2FMA
                       tests/TestVDTMathAVX2FMA.cpp
                       LINK_LIBRARIES LHCbMathLib)
  set_property(SOURCE tests/TestVDTMathAVX512.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${AVX512_BUILD_FLAGS} )
  gaudi_add_executable(TestVDTMathAVX512
                       tests/TestVDTMathAVX512.cpp
                       LINK_LIBRARIES LHCbMathLib)

  gaudi_add_executable(TestMathSpeedSSE4
                       tests/MathSpeedTests/main_sse4.cpp
                       LINK_LIBRARIES LHCbMathLib )
  target_link_libraries( TestMathSpeedSSE4 -lrt )
  set_property(SOURCE tests/MathSpeedTests/main_sse4.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-ignored-attributes ${SSE_BUILD_FLAGS}" )

  gaudi_add_executable(TestMathSpeedAVX
                       tests/MathSpeedTests/main_avx.cpp
                       LINK_LIBRARIES LHCbMathLib )
  target_link_libraries( TestMathSpeedAVX -lrt )
  set_property(SOURCE tests/MathSpeedTests/main_avx.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-ignored-attributes ${AVX_BUILD_FLAGS} " )

  gaudi_add_executable(TestMathSpeedAVX2
                       tests/MathSpeedTests/main_avx2.cpp
                       LINK_LIBRARIES LHCbMathLib )
  target_link_libraries( TestMathSpeedAVX2 -lrt )
  set_property(SOURCE tests/MathSpeedTests/main_avx2.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${AVX2_BUILD_FLAGS} )

  gaudi_add_executable(TestMathSpeedAVX2FMA
                       tests/MathSpeedTests/main_avx2fma.cpp
                       LINK_LIBRARIES LHCbMathLib )
  target_link_libraries( TestMathSpeedAVX2FMA -lrt )
  set_property(SOURCE tests/MathSpeedTests/main_avx2fma.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${AVX2FMA_BUILD_FLAGS} )

  gaudi_add_executable(TestMathSpeedAVX512
                       tests/MathSpeedTests/main_avx512.cpp
                       LINK_LIBRARIES LHCbMathLib )
  target_link_libraries( TestMathSpeedAVX512 -lrt )
  set_property(SOURCE tests/MathSpeedTests/main_avx512.cpp APPEND_STRING PROPERTY COMPILE_FLAGS ${AVX512_BUILD_FLAGS} )


  # Fixes for GCC7.
  if( BINARY_TAG_COMP_NAME STREQUAL "gcc" AND BINARY_TAG_COMP_VERSION VERSION_GREATER "6.99")
    set_property(TARGET TestMathSpeedAVX     APPEND PROPERTY COMPILE_FLAGS " -faligned-new ")
    set_property(TARGET TestMathSpeedAVX2    APPEND PROPERTY COMPILE_FLAGS " -faligned-new ")
    set_property(TARGET TestMathSpeedAVX2FMA APPEND PROPERTY COMPILE_FLAGS " -faligned-new ")
  endif()

  endif() # x86_64
endif()

gaudi_install_python_modules()

gaudi_add_test(QMTest QMTEST)

gaudi_add_unit_test(test_md5 tests/test_md5.cpp LINK_LIBRARIES LHCbMathLib TYPE Boost)
gaudi_add_unit_test(test_matvec tests/test_matvec.cpp LINK_LIBRARIES LHCbMathLib TYPE Boost)
