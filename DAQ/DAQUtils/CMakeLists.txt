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
# Package: DAQUtils
################################################################################
gaudi_subdir(DAQUtils)

gaudi_depends_on_subdirs(Event/DAQEvent
                         Event/HltEvent
                         Event/RecEvent
                         GaudiAlg
                         GaudiKernel
                         GaudiSvc
                         Phys/LoKiHlt
			 DAQ/DAQKernel)

find_package(RELAX)
find_package(Boost)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})


gaudi_add_module(DAQUtils
                 src/*.cpp
                 INCLUDE_DIRS RELAX
                 LINK_LIBRARIES RELAX DAQEventLib DAQKernelLib HltEvent RecEvent GaudiAlgLib GaudiKernel LoKiHltLib)

gaudi_install_headers(Event)


gaudi_add_test(QMTest QMTEST)
