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
gaudi_subdir(XmlTools)

gaudi_depends_on_subdirs(GaudiKernel
                         Kernel/LHCbKernel)

find_package(Boost)
find_package(ROOT)
find_package(XercesC)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS}
                           ${XERCESC_INCLUDE_DIRS} ${XercesC_INCLUDE_DIRS})

gaudi_add_library(XmlToolsLib
                  src/Lib/*.cpp
                  PUBLIC_HEADERS XmlTools
                  INCLUDE_DIRS XercesC
                  LINK_LIBRARIES XercesC GaudiKernel)

gaudi_add_module(XmlTools
                 src/component/*.cpp
                 INCLUDE_DIRS XercesC
                 LINK_LIBRARIES XercesC GaudiKernel XmlToolsLib)

gaudi_add_dictionary(XmlTools
                     dict/XmlToolsDict.h
                     dict/XmlToolsDict.xml
                     INCLUDE_DIRS XercesC
                     LINK_LIBRARIES XercesC GaudiKernel XmlToolsLib)
