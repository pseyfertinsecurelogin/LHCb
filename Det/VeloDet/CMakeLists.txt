###############################################################################
# (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
gaudi_subdir(VeloDet)

gaudi_depends_on_subdirs(Det/DetDesc
                         Det/DetDescCnv
                         GaudiAlg
                         Kernel/LHCbKernel
                         Kernel/LHCbMath)

find_package(Boost COMPONENTS regex)
find_package(VDT)

gaudi_add_library(VeloDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS VeloDet
                  INCLUDE_DIRS VDT Boost
                  LINK_LIBRARIES VDT Boost DetDescLib DetDescCnvLib GaudiAlgLib LHCbKernel LHCbMathLib)

gaudi_add_module(VeloDet
                 src/component/*.cpp
                 INCLUDE_DIRS VDT Boost
                 LINK_LIBRARIES VDT Boost DetDescLib DetDescCnvLib GaudiAlgLib LHCbKernel LHCbMathLib VeloDetLib)

gaudi_add_dictionary(VeloDet
                     dict/VeloDetDict.h
                     dict/VeloDetDict.xml
                     INCLUDE_DIRS VDT Boost
                     LINK_LIBRARIES VDT Boost DetDescLib DetDescCnvLib GaudiAlgLib LHCbKernel LHCbMathLib VeloDetLib)

# Prepare test Git CondDB overlay
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/test/DB)
file(COPY tests/data/DB/simple/ DESTINATION test/DB/simple/)
file(COPY tests/data/DB/simple/ DESTINATION test/DB/updates/)
file(REMOVE
  ${CMAKE_CURRENT_BINARY_DIR}/test/DB/updates/Conditions/Velo/Alignment/Global.xml
  ${CMAKE_CURRENT_BINARY_DIR}/test/DB/updates/Conditions/online.xml)
file(COPY tests/data/DB/updates/ DESTINATION test/DB/updates/)
execute_process(COMMAND git init -q ${CMAKE_CURRENT_BINARY_DIR}/test/DB/simple)
execute_process(COMMAND git init -q ${CMAKE_CURRENT_BINARY_DIR}/test/DB/updates)

gaudi_add_test(QMTest QMTEST ENVIRONMENT TEST_OVERLAY_ROOT=${CMAKE_CURRENT_BINARY_DIR}/test/DB)
