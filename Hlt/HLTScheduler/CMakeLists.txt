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
# Package: HLTScheduler
################################################################################
gaudi_subdir(HLTScheduler)

gaudi_depends_on_subdirs(
                         GaudiCommonSvc
                         GaudiCoreSvc
                         GaudiHive
                         GaudiAlg
                         GaudiKernel
                         Event/HltEvent)

find_package(Boost)
find_package(TBB)
find_package(cppgsl)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${TBB_INCLUDE_DIRS})

gaudi_add_module(HLTScheduler
                 src/*.cpp
                 INCLUDE_DIRS Boost HLTScheduler cppgsl
                 LINK_LIBRARIES Boost GaudiAlgLib GaudiKernel HltEvent)

gaudi_add_test(QMTest QMTEST)
