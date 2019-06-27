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
# Package: DetDesc
################################################################################
gaudi_subdir(DetDesc v19r1)

gaudi_depends_on_subdirs(GaudiKernel
                         GaudiUtils
                         Kernel/LHCbMath)

find_package(Boost)
find_package(VDT)
find_package(VectorClass)
find_package(Vc)
find_package(ROOT COMPONENTS MathCore GenVector)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} 
                           ${VectorClass_INCLUDE_DIR} ${Vc_INCLUDE_DIR})

find_path(RANGES_V3_INCLUDE_DIR NAMES range/v3/all.hpp)
if(NOT RANGES_V3_INCLUDE_DIR)
  message(FATAL "required headers from range-v3 missing")
endif()

gaudi_add_library(DetDescLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS DetDesc
                  INCLUDE_DIRS VDT ROOT Boost ${RANGES_V3_INCLUDE_DIR}
                  LINK_LIBRARIES VDT ROOT GaudiKernel GaudiUtilsLib LHCbMathLib)
target_link_libraries( DetDescLib "${Vc_LIB_DIR}/libVc.a" )

gaudi_add_dictionary(DetDesc
                     dict/DetDescDict.h
                     dict/DetDescDict.xml
                     INCLUDE_DIRS ROOT
                     LINK_LIBRARIES ROOT GaudiKernel GaudiUtilsLib LHCbMathLib DetDescLib
                     OPTIONS "-U__MINGW32__")
