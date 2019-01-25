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
# Package: TrackEvent
################################################################################
gaudi_subdir(TrackEvent v6r7)

gaudi_depends_on_subdirs(GaudiObjDesc
                         Kernel/LHCbKernel
                         Event/DigiEvent)

find_package(GSL)
find_package(ROOT)
find_package(Rangev3 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

include(GaudiObjDesc)

gaudi_install_headers(Event)