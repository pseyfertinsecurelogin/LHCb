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
gaudi_subdir(GaudiGSL)

gaudi_depends_on_subdirs(GaudiAlg GaudiUtils)

find_package(CLHEP)
find_package(GSL)
find_package(ROOT REQUIRED)
find_package(Boost REQUIRED)


# If any of the non-essential externals is missing, give up on building
# these targets.
if( NOT CLHEP_FOUND OR NOT GSL_FOUND )
   message( STATUS "Requirements for GaudiGSL not found. "
      "Not building package" )
   return()
endif()

# Hide some CLHEP/ROOT compile time warnings
include_directories(SYSTEM ${CLHEP_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

#---Libraries---------------------------------------------------------------
gaudi_add_library(GaudiGSLLib src/Lib/*.cpp
                  LINK_LIBRARIES GaudiAlgLib GSL CLHEP
                  INCLUDE_DIRS GSL CLHEP
                  PUBLIC_HEADERS GaudiGSL GaudiMath)
gaudi_add_module(GaudiGSL src/Components/*.cpp
                 LINK_LIBRARIES GaudiGSLLib)

gaudi_add_module(GaudiGSLExamples examples/src/*.cpp
                 LINK_LIBRARIES GaudiGSLLib)

#---Dictionaries------------------------------------------------------------
gaudi_add_dictionary(GaudiGSLMath dict/GaudiGSLMath.h dict/GaudiGSLMath.xml LINK_LIBRARIES GaudiGSLLib)

#---Executables-------------------------------------------------------------
macro(add_gsl_unit_test name)
  gaudi_add_unit_test(${name} src/Tests/${name}.cpp LINK_LIBRARIES GaudiGSLLib GaudiUtilsLib)
endmacro()

foreach(test IntegralInTest DerivativeTest 2DoubleFuncTest GSLAdaptersTest
             PFuncTest ExceptionsTest SimpleFuncTest 3DoubleFuncTest InterpTest
             Integral1Test)
  add_gsl_unit_test(${test})
endforeach()
