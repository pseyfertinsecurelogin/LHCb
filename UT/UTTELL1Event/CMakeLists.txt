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
# Package: UTTELL1Event
################################################################################
gaudi_subdir(UTTELL1Event v1r0)

gaudi_depends_on_subdirs(GaudiObjDesc
                         Kernel/LHCbKernel
                         UT/UTKernel)

include(GaudiObjDesc)

god_build_headers(xml/*.xml)

gaudi_add_library(UTTELL1Event
                  src/*.cpp
                  NO_PUBLIC_HEADERS
                  LINK_LIBRARIES LHCbKernel UTKernelLib)

god_build_dictionary(xml/*.xml
                     LINK_LIBRARIES LHCbKernel UTKernelLib UTTELL1Event)

