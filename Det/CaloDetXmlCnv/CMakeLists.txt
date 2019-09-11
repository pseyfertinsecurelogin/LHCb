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
# Package: CaloDetXmlCnv
################################################################################
gaudi_subdir(CaloDetXmlCnv)

find_package(Boost)
find_package(XercesC)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${XERCESC_INCLUDE_DIRS})

gaudi_depends_on_subdirs(Det/CaloDet
                         Det/DetDescCnv)

gaudi_add_module(CaloDetXmlCnv
                 src/*.cpp
                 LINK_LIBRARIES CaloDetLib DetDescCnvLib)

