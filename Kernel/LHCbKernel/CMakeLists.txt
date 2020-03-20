###############################################################################
# (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      #
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
gaudi_subdir(LHCbKernel)

gaudi_depends_on_subdirs(GaudiAlg
                         GaudiKernel
                         Kernel/LHCbMath)

find_package(Boost COMPONENTS system)
find_package(ROOT)
find_package(VDT)
find_package(VectorClass)
find_package(cppgsl)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${VectorClass_INCLUDE_DIR})

gaudi_add_library(LHCbKernel
                  src/*.cpp
		  PUBLIC_HEADERS Kernel
                  INCLUDE_DIRS ROOT Boost VDT ${VectorClass_INCLUDE_DIR} cppgsl
                  LINK_LIBRARIES ROOT Boost VDT GaudiKernel LHCbMathLib GaudiAlgLib)
set_property(SOURCE src/PlatformInfo.cpp APPEND PROPERTY COMPILE_DEFINITIONS BINARY_TAG=${BINARY_TAG})

gaudi_add_dictionary(LHCbKernel dict/dictionary.h dict/selection.xml LINK_LIBRARIES LHCbKernel)

if(GAUDI_BUILD_TESTS)
  gaudi_add_executable(RichSmartIDTest
                       src/Test/RichSmartIDTest/*.cpp
                       LINK_LIBRARIES LHCbKernel)
endif()

gaudi_install_python_modules()

gaudi_add_test(QMTest QMTEST)

gaudi_add_unit_test(test_PartitionPosition tests/src/test_PartitionPosition.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_arenaAllocator tests/src/test_arenaAllocator.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_container tests/src/test_container.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_SynchronizedValue tests/src/test_SynchronizedValue.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_PolymorphicValue tests/src/test_PolymorphicValue.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_STLExtensions tests/src/test_STLExtensions.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_MultiIndexContainer tests/src/test_MultiIndexContainer.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_PlatformInfo tests/src/test_PlatformInfo.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_meta_enum tests/src/test_meta_enum.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_BrokenLineTrajectory tests/src/test_BrokenLineTrajectory.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
gaudi_add_unit_test(test_PiecewiseTrajectory tests/src/test_PiecewiseTrajectory.cpp
                    LINK_LIBRARIES LHCbKernel TYPE Boost)
set_property(SOURCE tests/src/test_PlatformInfo.cpp APPEND PROPERTY COMPILE_DEFINITIONS BINARY_TAG=${BINARY_TAG})

gaudi_add_unit_test(test_allocatorReleaser ../tests/src/test_allocatorReleaser.cpp
                    LINK_LIBRARIES LHCbKernel)
