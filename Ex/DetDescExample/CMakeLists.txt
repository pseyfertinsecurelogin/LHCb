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
# Package: DetDescExample
################################################################################
gaudi_subdir(DetDescExample v7r8p1)

gaudi_depends_on_subdirs(Det/CaloDet
                         Det/CaloDetXmlCnv
                         Det/DDDB
                         Det/DetDescCnv
                         Det/DetDescSvc
                         GaudiAlg
                         GaudiConf
                         GaudiSvc)

gaudi_add_executable(DetDescExample
                     ${CMAKE_SOURCE_DIR}/GaudiConf/src/GaudiMain.cpp
                     src/*.cpp
                     LINK_LIBRARIES CaloDetLib DetDescCnvLib GaudiAlgLib)

