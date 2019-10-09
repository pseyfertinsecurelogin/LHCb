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
# Package: MCEvent
################################################################################
gaudi_subdir(MCEvent)

# hide warnings from some external projects
find_package(ROOT)
include_directories(SYSTEM ${ROOT_INCLUDE_DIRS})


gaudi_depends_on_subdirs(Event/EventBase
                         Kernel/LHCbKernel
                         Kernel/PartProp)



gaudi_add_library(MCEvent
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  INCLUDE_DIRS Event/EventBase
                  LINK_LIBRARIES LHCbKernel PartPropLib)

gaudi_add_dictionary(MCEvent dict/dictionary.h dict/selection.xml LINK_LIBRARIES LHCbKernel MCEvent PartPropLib INCLUDE_DIRS Event/EventBase)

