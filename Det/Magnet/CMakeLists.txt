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
# Package: Magnet
################################################################################
gaudi_subdir(Magnet)

gaudi_depends_on_subdirs(Det/DDDB
                         Det/DetDesc
                         GaudiAlg
                         GaudiConf
                         GaudiKernel
                         Kernel/LHCbKernel)

find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(Magnet
                 src/*.cpp
                 LINK_LIBRARIES DetDescLib GaudiKernel LHCbKernel GaudiAlgLib)

