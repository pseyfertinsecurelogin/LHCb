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
# Package: HCDAQ
################################################################################
gaudi_subdir(HCDAQ v1r5)

gaudi_depends_on_subdirs(DAQ/DAQUtils
			 DAQ/DAQKernel	
                         Det/DetDesc
                         Event/DAQEvent
                         Event/DigiEvent
                         GaudiAlg
                         Kernel/LHCbKernel)

find_package(AIDA)

gaudi_add_module(HCDAQ
                 src/*.cpp
                 INCLUDE_DIRS AIDA Event/DigiEvent
                 LINK_LIBRARIES DAQEventLib DAQKernelLib DetDescLib GaudiAlgLib LHCbKernel)

