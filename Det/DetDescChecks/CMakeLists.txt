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
# Package: DetDescChecks
################################################################################
gaudi_subdir(DetDescChecks)

gaudi_depends_on_subdirs(Det/DDDB
                         Det/DetDesc
                         Det/DetDescCnv
                         Det/DetDescSvc
                         Det/Magnet
                         GaudiAlg
                         GaudiConf
                         GaudiPython
                         GaudiSvc
                         RootHistCnv)

find_package(ROOT)
find_package(Boost)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(DetDescChecks
                 src/*.cpp
                 LINK_LIBRARIES Boost DetDescLib DetDescCnvLib GaudiAlgLib)

gaudi_install_python_modules()


gaudi_add_test(QMTest QMTEST)
