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
# Package: RichInterfaces
################################################################################
gaudi_subdir(RichInterfaces v1r0)

gaudi_depends_on_subdirs(Kernel/LHCbKernel
                         Det/RichDet
                         Rich/RichUtils)

gaudi_add_dictionary(RichInterfaces
                     dict/RichInterfacesDict.h
                     dict/RichInterfacesDict.xml
                     INCLUDE_DIRS AIDA Boost RichDet
                     LINK_LIBRARIES LHCbKernel
                     OPTIONS "-U__MINGW32__")

find_package(Vc)
include_directories(SYSTEM ${Vc_INCLUDE_DIR})

gaudi_install_headers(RichInterfaces)
