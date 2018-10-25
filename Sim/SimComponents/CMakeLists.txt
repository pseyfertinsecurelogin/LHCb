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
# Package: SimComponents
################################################################################
gaudi_subdir(SimComponents v4r0)

gaudi_depends_on_subdirs(Det/CaloDet
                         Event/GenEvent
                         Event/MCEvent
			 Event/RecEvent
			 Event/TrackEvent
                         GaudiAlg
                         Kernel/MCInterfaces
			 Kernel/FSRAlgs)

find_package(AIDA)
find_package(Boost)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(SimComponents
                 src/*.cpp
                 INCLUDE_DIRS Boost AIDA Kernel/MCInterfaces
                 LINK_LIBRARIES Boost CaloDetLib GenEvent MCEvent RecEvent TrackEvent GaudiAlgLib)

