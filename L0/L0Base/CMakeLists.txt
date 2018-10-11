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
# Package: L0Base
################################################################################
gaudi_subdir(L0Base v2r4p1)

gaudi_depends_on_subdirs(Event/DAQEvent
                         DAQ/DAQKernel
                         GaudiAlg)

gaudi_add_library(L0Base
                  src/*.cpp
                  PUBLIC_HEADERS L0Base
                  LINK_LIBRARIES DAQEventLib DAQKernelLib GaudiAlgLib)

