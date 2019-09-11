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
# Package: LoKiCore
################################################################################
gaudi_subdir(LoKiCore)

gaudi_depends_on_subdirs(Event/RecEvent
                         GaudiAlg
                         Kernel/LHCbKernel
                         Kernel/LHCbMath
                         Kernel/PartProp
                         Kernel/PhysInterfaces)

find_package(AIDA)
find_package(Boost)
find_package(PythonLibs)
find_package(RELAX)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}
                           ${PYTHON_INCLUDE_DIRS})

gaudi_add_library(LoKiCoreLib
                  src/*.cpp
                  PUBLIC_HEADERS LoKi
                  INCLUDE_DIRS ROOT AIDA Boost RELAX ROOT PythonLibs
                  LINK_LIBRARIES ROOT Boost RELAX ROOT PythonLibs GaudiAlgLib LHCbKernel LHCbMathLib PartPropLib PhysInterfacesLib RecEvent)

gaudi_add_module(LoKiCore
                 src/Components/*.cpp
                 INCLUDE_DIRS ROOT AIDA Boost RELAX ROOT PythonLibs
                 LINK_LIBRARIES ROOT Boost RELAX ROOT PythonLibs GaudiAlgLib LHCbKernel LHCbMathLib PartPropLib PhysInterfacesLib LoKiCoreLib)

gaudi_add_dictionary(LoKiCore
                     dict/LoKiCoreDict.h
                     dict/LoKiCore.xml
                     INCLUDE_DIRS ROOT AIDA Boost RELAX ROOT PythonLibs
                     LINK_LIBRARIES ROOT Boost RELAX ROOT PythonLibs GaudiAlgLib LHCbKernel LHCbMathLib PhysInterfacesLib PartPropLib LoKiCoreLib
                     OPTIONS " -U__MINGW32__ ")

if(GAUDI_BUILD_TESTS)
  gaudi_add_executable(NodeGrammarTest
                       src/tests/NodeGrammarTest.cpp
                       INCLUDE_DIRS ROOT AIDA Boost RELAX ROOT PythonLibs
                       LINK_LIBRARIES ROOT Boost RELAX ROOT PythonLibs GaudiAlgLib LHCbKernel LHCbMathLib PartPropLib LoKiCoreLib)
  gaudi_add_executable(TreeGrammarTest
                       src/tests/TreeGrammarTest.cpp
                       INCLUDE_DIRS ROOT AIDA Boost RELAX ROOT PythonLibs
                       LINK_LIBRARIES ROOT Boost RELAX ROOT PythonLibs GaudiAlgLib LHCbKernel LHCbMathLib PartPropLib LoKiCoreLib)
  gaudi_add_executable(CCGrammarTest
                       src/tests/CCGrammarTest.cpp
                       INCLUDE_DIRS ROOT AIDA Boost RELAX ROOT PythonLibs
                       LINK_LIBRARIES ROOT Boost RELAX ROOT PythonLibs GaudiAlgLib LHCbKernel LHCbMathLib PartPropLib LoKiCoreLib)
endif()

gaudi_install_python_modules()

# Install CMake modules
gaudi_install_cmake_modules()
# note: gaudi_install_cmake_modules only install .cmake files
install(FILES cmake/LoKiFunctorsCachePostActionOpts.py
        DESTINATION cmake)

gaudi_add_test(QMTest QMTEST)
