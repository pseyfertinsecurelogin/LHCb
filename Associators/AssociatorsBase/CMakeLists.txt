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
# Package: AssociatorsBase
################################################################################
gaudi_subdir(AssociatorsBase v1r0)

gaudi_depends_on_subdirs(Event/LinkerEvent
                         Event/MCEvent
                         Kernel/Relations
                         GaudiKernel)

find_package(Boost)
find_package(ROOT)
find_package(Rangev3)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${RANGEV3_INCLUDE_DIRS})

gaudi_install_headers(Associators)

gaudi_install_python_modules()
