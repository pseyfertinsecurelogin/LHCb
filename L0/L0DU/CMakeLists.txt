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
# Package: L0DU
################################################################################
gaudi_subdir(L0DU v10r35p1)

gaudi_depends_on_subdirs(Calo/CaloUtils
			 Calo/CaloDAQ
                         Det/CaloDet
                         Event/DAQEvent
                         Event/L0Event
                         L0/L0Base
                         DAQ/DAQKernel
                         L0/L0Interfaces)

find_package(AIDA)
find_package(Boost)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})


gaudi_add_module(L0DU
                 src/*.cpp
                 INCLUDE_DIRS AIDA L0/L0Interfaces Calo/CaloDAQ
                 LINK_LIBRARIES CaloUtils CaloDetLib DAQKernelLib DAQEventLib L0Event L0Base)

gaudi_install_python_modules()

gaudi_env(SET L0DUOPTS \${L0DUROOT}/options)

gaudi_add_test(QMTest QMTEST)
