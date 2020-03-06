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
gaudi_subdir(CaloInterfaces)

gaudi_depends_on_subdirs(Det/CaloDet
                         Event/DigiEvent
                         Event/L0Event
                         Event/RecEvent
                         GaudiKernel
                         Kernel/LHCbKernel)

find_package(ROOT)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})

gaudi_add_dictionary(CaloInterfaces
                     dict/CaloInterfacesDict.h
                     dict/CaloInterfacesDict.xml
                     INCLUDE_DIRS Event/DigiEvent
                     LINK_LIBRARIES CaloDetLib L0Event RecEvent GaudiKernel LHCbKernel)

gaudi_install_headers(CaloInterfaces)
