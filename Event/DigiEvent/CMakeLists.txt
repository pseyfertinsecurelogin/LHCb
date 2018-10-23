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
# Package: DigiEvent
################################################################################
gaudi_subdir(DigiEvent v4r5)

gaudi_depends_on_subdirs(GaudiObjDesc
                         Kernel/LHCbKernel
                         Kernel/LHCbMath)

find_package(Boost)

include(GaudiObjDesc)

god_build_headers(xml/*.xml)

god_build_dictionary(xml/*.xml
                     EXTEND dict/lcgDict.h
                     INCLUDE_DIRS Boost
                     LINK_LIBRARIES Boost LHCbKernel LHCbMathLib)

gaudi_install_headers(Event)

