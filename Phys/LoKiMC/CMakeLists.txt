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
gaudi_subdir(LoKiMC)

find_package(Graphviz)

gaudi_depends_on_subdirs(Event/MCEvent
                         Kernel/MCInterfaces
                         Phys/LoKiCore
                         Phys/LoKiNumbers)

gaudi_add_library(LoKiMCLib
                  src/*.cpp
                  PUBLIC_HEADERS LoKi
                  INCLUDE_DIRS Kernel/MCInterfaces
                  LINK_LIBRARIES MCEvent LoKiCoreLib)

gaudi_add_module(LoKiMC
                 src/Components/*.cpp
                 INCLUDE_DIRS Kernel/MCInterfaces
                 LINK_LIBRARIES MCEvent LoKiCoreLib LoKiMCLib)

gaudi_add_dictionary(LoKiMC
                     dict/LoKiMCDict.h
                     dict/LoKiMC.xml
                     INCLUDE_DIRS Kernel/MCInterfaces
                     LINK_LIBRARIES MCEvent LoKiCoreLib LoKiMCLib)

if(GAUDI_BUILD_TESTS)
  gaudi_add_executable(MCDecayGrammarTest
                       src/tests/MCDecayGrammarTest.cpp
                       INCLUDE_DIRS Kernel/MCInterfaces
                       LINK_LIBRARIES MCEvent LoKiCoreLib LoKiMCLib)
endif()

gaudi_install_python_modules()


gaudi_add_test(QMTest QMTEST)
