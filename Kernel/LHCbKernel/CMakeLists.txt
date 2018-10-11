###############################################################################
# (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
################################################################################
# Package: LHCbKernel
################################################################################
gaudi_subdir(LHCbKernel v16r1)

gaudi_depends_on_subdirs(GaudiAlg
                         GaudiKernel
                         GaudiObjDesc
                         Kernel/LHCbMath)

find_package(Boost COMPONENTS system)
find_package(ROOT)
find_package(VDT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

find_path(CPP_GSL_INCLUDE_DIR NAMES gsl/span)
if(NOT CPP_GSL_INCLUDE_DIR)
       message(FATAL "required headers from C++ GSL missing")
endif()


include(GaudiObjDesc)

god_build_headers(xml/*.xml
                  DESTINATION Kernel)

gaudi_add_library(LHCbKernel
                  src/*.cpp
                  PUBLIC_HEADERS Kernel
                  INCLUDE_DIRS ROOT Boost VDT
                  LINK_LIBRARIES ROOT Boost VDT GaudiKernel GaudiObjDescLib LHCbMathLib GaudiAlgLib)
set_property(SOURCE src/PlatformInfo.cpp APPEND PROPERTY COMPILE_DEFINITIONS BINARY_TAG=${BINARY_TAG})

god_build_dictionary(xml/*.xml
                     HEADERS_DESTINATION Kernel
                     EXTEND dict/LHCbKernelDict.h
                     INCLUDE_DIRS ROOT Boost
                     LINK_LIBRARIES ROOT Boost GaudiKernel GaudiObjDescLib LHCbMathLib LHCbKernel)

if(GAUDI_BUILD_TESTS)
  gaudi_add_executable(RichSmartIDTest
                       src/Test/RichSmartIDTest/*.cpp
                       INCLUDE_DIRS ROOT Boost
                       LINK_LIBRARIES ROOT Boost GaudiKernel GaudiObjDescLib LHCbMathLib LHCbKernel)
endif()

gaudi_install_python_modules()

gaudi_add_test(QMTest QMTEST)

gaudi_add_unit_test(test_PolymorphicValue tests/src/test_PolymorphicValue.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_STLExtensions tests/src/test_STLExtensions.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_MultiIndexContainer tests/src/test_MultiIndexContainer.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_PlatformInfo tests/src/test_PlatformInfo.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
set_property(SOURCE tests/src/test_PlatformInfo.cpp APPEND PROPERTY COMPILE_DEFINITIONS BINARY_TAG=${BINARY_TAG})
