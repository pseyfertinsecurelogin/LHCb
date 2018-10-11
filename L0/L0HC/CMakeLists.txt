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
# Package: L0HC
################################################################################
gaudi_subdir(L0HC v1r0)

gaudi_depends_on_subdirs(DAQ/DAQUtils
			 DAQ/DAQKernel	
                         Det/DetDesc
                         Event/DAQEvent
                         Event/DigiEvent
                         Kernel/LHCbKernel

#Calo/CaloDAQ
#Calo/CaloUtils
                         Event/LinkerEvent
                         Event/L0Event
                         DAQ/DAQKernel
                         L0/L0Base)

find_package(AIDA)

gaudi_add_module(L0HC
                 src/*.cpp
                 INCLUDE_DIRS AIDA HC/HCDAQ
                 LINK_LIBRARIES DetDescLib LinkerEvent L0Base DAQKernelLib)

gaudi_env(SET L0HCOPTS \${L0HCROOT}/options)

