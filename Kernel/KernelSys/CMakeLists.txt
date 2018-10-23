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
# Package: KernelSys
################################################################################
gaudi_subdir(KernelSys v12r3)

gaudi_depends_on_subdirs(Calo/CaloKernel
                         Kernel/LHCbKernel
                         Kernel/LHCbMath
                         Kernel/PartProp
                         Kernel/Relations
                         Kernel/XMLSummaryKernel
                         L0/L0MuonKernel
                         L0/ProcessorKernel
                         Muon/MuonKernel
                         Rich/RichKernel
                         ST/STKernel)

