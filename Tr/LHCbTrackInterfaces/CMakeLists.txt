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
# Package: LHCbTrackInterfaces
################################################################################
gaudi_subdir(LHCbTrackInterfaces v2r3)

gaudi_depends_on_subdirs(Det/DetDesc
                         Event/RecEvent
                         Event/TrackEvent
                         GaudiKernel
                         Kernel/LHCbKernel)

gaudi_add_dictionary(LHCbTrackInterfaces
                     dict/LHCbTrackInterfacesDict.h
                     dict/LHCbTrackInterfacesDict.xml
                     LINK_LIBRARIES DetDescLib RecEvent TrackEvent GaudiKernel LHCbKernel
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(TrackInterfaces)

