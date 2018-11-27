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
# Package: DetDescCnv
################################################################################
gaudi_subdir(DetDescCnv v4r19)

gaudi_depends_on_subdirs(Det/DetDesc
                         GaudiKernel
                         Tools/XmlTools)

find_package(XercesC)
find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS}
                           ${ROOT_INCLUDE_DIRS}
                           ${XERCESC_INCLUDE_DIRS})

gaudi_add_library(DetDescCnvLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS DetDescCnv
                  INCLUDE_DIRS XercesC
                  LINK_LIBRARIES XercesC DetDescLib GaudiKernel XmlToolsLib)

gaudi_add_module(DetDescCnv
                 src/component/*.cpp
                 INCLUDE_DIRS XercesC
                 LINK_LIBRARIES XercesC DetDescLib GaudiKernel XmlToolsLib DetDescCnvLib)

