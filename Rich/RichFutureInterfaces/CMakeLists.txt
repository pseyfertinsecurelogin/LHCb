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
# Package: RichFutureInterfaces
################################################################################
gaudi_subdir(RichFutureInterfaces)

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         Rich/RichUtils
                         Rich/RichFutureUtils)

find_package(Vc)

include_directories(SYSTEM ${Vc_INCLUDE_DIR})

gaudi_add_dictionary(RichFutureInterfaces
                     dict/RichFutureInterfacesDict.h
                     dict/RichFutureInterfacesDict.xml
                     INCLUDE_DIRS AIDA Boost
                     LINK_LIBRARIES LHCbKernel RichFutureUtils RichUtils
                     OPTIONS " -U__MINGW32__ -Wno-undefined-var-template ")

# Note - Above -Wno-undefined-var-template is to work around an issue in Vc 1.3.0.
#        To be removed once Vc is updated.

gaudi_install_headers(RichFutureInterfaces)
