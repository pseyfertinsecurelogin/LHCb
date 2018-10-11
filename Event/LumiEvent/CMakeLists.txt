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
# Package: LumiEvent
################################################################################
gaudi_subdir(LumiEvent v3r10)

gaudi_depends_on_subdirs(Event/DAQEvent
                         Event/EventBase
                         GaudiAlg
                         GaudiKernel
                         GaudiObjDesc
			 DAQ/DAQKernel
                         Kernel/LHCbKernel)

include(GaudiObjDesc)

god_build_headers(xml/*.xml)

gaudi_add_module(LumiEvent
                 src/*.cpp
                 INCLUDE_DIRS Event/EventBase
                 LINK_LIBRARIES DAQEventLib DAQKernelLib GaudiAlgLib GaudiKernel LHCbKernel)

god_build_dictionary(xml/*.xml
                     EXTEND dict/lcgDict.h
                     INCLUDE_DIRS Event/EventBase
                     LINK_LIBRARIES DAQEventLib GaudiAlgLib GaudiKernel LHCbKernel)

