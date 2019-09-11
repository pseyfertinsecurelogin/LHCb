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
# Package: L0Calo
################################################################################
gaudi_subdir(L0Calo)

# hide warnings from some external projects
find_package(Boost)
find_package(ROOT)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_depends_on_subdirs(Calo/CaloDAQ
                         Calo/CaloUtils
                         Event/LinkerEvent
                         DAQ/DAQKernel
                         L0/L0Base)

find_package(AIDA)

gaudi_add_module(L0Calo
                 src/*.cpp
                 INCLUDE_DIRS AIDA Calo/CaloDAQ
                 LINK_LIBRARIES CaloUtils LinkerEvent L0Base DAQKernelLib)

gaudi_install_python_modules()

gaudi_env(SET L0CALOOPTS \${L0CALOROOT}/options)

gaudi_add_test(QMTest QMTEST)
