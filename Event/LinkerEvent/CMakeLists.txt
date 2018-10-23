###############################################################################
# (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           #
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
gaudi_subdir(LinkerEvent v3r11)

gaudi_depends_on_subdirs(GaudiObjDesc
                         Kernel/LHCbKernel)

include(GaudiObjDesc)

god_build_headers(xml/*.xml)

gaudi_add_library(LinkerEvent
                  src/*.cpp
                  PUBLIC_HEADERS Linker
                  LINK_LIBRARIES LHCbKernel)

god_build_dictionary(xml/*.xml
                     LINK_LIBRARIES LHCbKernel LinkerEvent)

