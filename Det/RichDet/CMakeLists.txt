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
gaudi_subdir(RichDet)

gaudi_depends_on_subdirs(Det/DetDescCnv
                         Rich/RichUtils)

find_package(Boost)
find_package(ROOT)
find_package(GSL)
find_package(Vc)
find_package(XercesC)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS}
                           ${ROOT_INCLUDE_DIRS} ${Vc_INCLUDE_DIR}
                           ${XERCESC_INCLUDE_DIRS} ${XercesC_INCLUDE_DIRS})

gaudi_add_library(RichDetLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS RichDet
                  INCLUDE_DIRS GSL Boost
                  LINK_LIBRARIES GSL Boost DetDescCnvLib RichUtils)

gaudi_add_module(RichDet
                 src/component/*.cpp
                 INCLUDE_DIRS GSL Boost
                 LINK_LIBRARIES GSL Boost DetDescCnvLib RichUtils RichDetLib)

gaudi_add_dictionary(RichDet
                     dict/RichDetDict.h
                     dict/RichDetDict.xml
                     INCLUDE_DIRS GSL Boost
                     LINK_LIBRARIES GSL Boost DetDescCnvLib RichUtils RichDetLib
                     OPTIONS " -Wno-undefined-var-template")

# Note - Above -Wno-undefined-var-template is to work around an issue in Vc 1.3.0.
#        To be removed once Vc is updated.

# Fixes for GCC7.
if( BINARY_TAG_COMP_NAME STREQUAL "gcc" AND BINARY_TAG_COMP_VERSION VERSION_GREATER "6.99")
  set_property(TARGET RichDetLib APPEND PROPERTY COMPILE_FLAGS " -faligned-new ")
  set_property(TARGET RichDet    APPEND PROPERTY COMPILE_FLAGS " -faligned-new ")
endif()

