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
# Package: DAQEvent
################################################################################
gaudi_subdir(DAQEvent)

gaudi_depends_on_subdirs(GaudiKernel
                         Kernel/LHCbKernel)



gaudi_add_library(DAQEventLib
                  src/*.cpp
                  PUBLIC_HEADERS Event
                  LINK_LIBRARIES GaudiKernel LHCbKernel)

gaudi_add_dictionary(DAQEvent
                     xml/lcgdict/lcg_headers.h
                     xml/lcgdict/lcg_selection.xml
                     LINK_LIBRARIES GaudiKernel DAQEventLib
                     OPTIONS --comments)

