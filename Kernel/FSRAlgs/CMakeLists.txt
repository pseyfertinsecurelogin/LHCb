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
# Package: FSRAlgs
################################################################################
gaudi_subdir(FSRAlgs)

gaudi_depends_on_subdirs(Det/DetDesc
                         Event/DAQEvent
                         Event/EventBase
                         Event/FSREvent
                         Event/LumiEvent
                         Event/RecEvent
                         Event/GenEvent
                         GaudiAlg
                         GaudiConf
                         Kernel/HltInterfaces
                         Kernel/LHCbAlgs
                         Kernel/LHCbKernel)

find_package(Boost COMPONENTS iostreams)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

find_package(RELAX)

gaudi_add_module(FSRAlgs
                 src/*.cpp
                 INCLUDE_DIRS Boost RELAX Event/EventBase Event/LumiEvent
                 LINK_LIBRARIES Boost RELAX DetDescLib DAQEventLib RecEvent GaudiAlgLib HltInterfaces LHCbKernel)

gaudi_install_headers(FSRAlgs)
gaudi_install_python_modules()


gaudi_add_test(QMTest QMTEST)
