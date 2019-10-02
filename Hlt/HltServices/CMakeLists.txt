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
# Package: HltServices
################################################################################
gaudi_subdir(HltServices)

gaudi_depends_on_subdirs(Det/DetDesc
                         Event/DAQEvent
                         Event/HltEvent
                         GaudiAlg
                         GaudiKernel
                         Kernel/HltInterfaces
                         Phys/LoKiHlt)

find_package(AIDA)
find_package(Boost COMPONENTS iostreams filesystem program_options)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

gaudi_add_module(HltServices
                 src/*.cpp
                 INCLUDE_DIRS Boost AIDA
                 LINK_LIBRARIES Boost DetDescLib DAQEventLib HltEvent GaudiAlgLib GaudiKernel HltInterfaces LoKiHltLib)

gaudi_add_executable(hlttck_cdb_listkeys  utilities/main.cpp src/cdb.cpp src/tar.cpp
                     INCLUDE_DIRS Boost
                     LINK_LIBRARIES GaudiKernel Boost HltInterfaces)

gaudi_add_test(QMTest QMTEST)
