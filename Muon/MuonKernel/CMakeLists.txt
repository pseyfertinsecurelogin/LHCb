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
# Package: MuonKernel
################################################################################
gaudi_subdir(MuonKernel v5r0)

gaudi_depends_on_subdirs(Kernel/LHCbKernel)

gaudi_add_library(MuonKernelLib
                  src/*.cpp
                  PUBLIC_HEADERS MuonKernel
                  LINK_LIBRARIES LHCbKernel)

