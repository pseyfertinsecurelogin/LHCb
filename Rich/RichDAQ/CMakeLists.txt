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
# Package: RichDAQ
################################################################################
gaudi_subdir(RichDAQ v4r0)

gaudi_depends_on_subdirs(Det/RichDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Rich/RichKernel
                         Rich/RichDAQKernel
                         Rich/RichUtils)

find_package(Vc)

include_directories(SYSTEM ${Vc_INCLUDE_DIR})

gaudi_add_module(RichDAQ
                 src/*.cpp
                 INCLUDE_DIRS Event/DAQEvent Event/DAQEvent RichUtils RichDAQKernel
                 LINK_LIBRARIES RichDetLib DAQEventLib RichKernelLib RichDAQKernel)
