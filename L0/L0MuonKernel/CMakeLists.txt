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
# Package: L0MuonKernel
################################################################################
gaudi_subdir(L0MuonKernel v9r10)

gaudi_depends_on_subdirs(L0/ProcessorKernel)

gaudi_add_library(L0MuonKernel
                  src/*.cpp
                  PUBLIC_HEADERS L0MuonKernel
                  LINK_LIBRARIES ProcessorKernel)

