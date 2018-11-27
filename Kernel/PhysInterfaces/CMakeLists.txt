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
# Package: PhysInterfaces
################################################################################
gaudi_subdir(PhysInterfaces)

gaudi_depends_on_subdirs(GaudiAlg
                         Event/PhysEvent
                         GaudiKernel)

find_package(ROOT)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})

gaudi_add_library(PhysInterfacesLib
                  src/*.cpp
                  PUBLIC_HEADERS Kernel
                  LINK_LIBRARIES PhysEvent GaudiKernel GaudiAlgLib)

gaudi_add_dictionary(PhysInterfaces
                     dict/PhysInterfacesDict.h
                     dict/PhysInterfacesDict.xml
                     LINK_LIBRARIES PhysEvent GaudiKernel PhysInterfacesLib
                     OPTIONS "-U__MINGW32__")

