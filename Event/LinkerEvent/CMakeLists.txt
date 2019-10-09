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
# Package: LinkerEvent
################################################################################
gaudi_subdir(LinkerEvent)

gaudi_depends_on_subdirs(Kernel/LHCbKernel)

gaudi_add_library(LinkerEvent
                  src/*.cpp
                  PUBLIC_HEADERS Linker Event
                  LINK_LIBRARIES LHCbKernel)

gaudi_add_dictionary(LinkerEvent dict/lcgDict.h dict/lcg_selection.xml LINK_LIBRARIES LinkerEvent)
