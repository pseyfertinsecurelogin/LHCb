###############################################################################
# (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
################################################################################
# Package: SOAExtensions
################################################################################
gaudi_subdir(SOAExtensions)

gaudi_depends_on_subdirs(
  Kernel/SOAContainer
  Kernel/LHCbKernel
  Kernel/LHCbMath
  GaudiAlg)

gaudi_install_headers(SOAExtensions)
# as transitive dependency, projects that include SOAExtensions must also include Kernel/SOAContainer

find_package(Boost)
find_package(Rangev3 REQUIRED)
find_package(cppgsl)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${RANGEV3_INCLUDE_DIR})

gaudi_add_unit_test(TestZipSelection
                    tests/src/TestSelection.cpp
                    INCLUDE_DIRS Kernel/SOAContainer cppgsl
                    LINK_LIBRARIES GaudiKernel
                    TYPE Boost)

gaudi_add_unit_test(TestHanaAccessor
                    tests/src/hana_soa_access.cpp
                    INCLUDE_DIRS Kernel/LHCbMath
                    Link_LIBRARIES LHCbMathLib
                    TYPE Boost)

gaudi_add_module( SOAExtensionExamples examples/src/*.cpp
  INCLUDE_DIRS GaudiAlg Kernel/SOAContainer Kernel/SOAExtensions cppgsl
  LINK_LIBRARIES GaudiAlgLib
  )

gaudi_add_module( ZipBarrier src/*.cpp
  INCLUDE_DIRS Kernel/SOAContainer Kernel/SOAExtensions GaudiAlg
  LINK_LIBRARIES GaudiAlgLib
  )



gaudi_add_test(QMTest QMTEST
  ENVIRONMENT
  BINARY_TAG=${BINARY_TAG}
  STDOPTS=${CMAKE_CURRENT_SOURCE_DIR}/options
  JOBOPTSEARCHPATH+=${CMAKE_CURRENT_SOURCE_DIR}/options
  PYTHONPATH+=${CMAKE_CURRENT_SOURCE_DIR}/tests/qmtest
  )

