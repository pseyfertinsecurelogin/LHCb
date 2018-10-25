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
# Package: VeloDAQ
################################################################################
gaudi_subdir(VeloDAQ v6r13)

gaudi_depends_on_subdirs(DAQ/Tell1Kernel
                         Det/VeloDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Event/RecEvent
                         Event/VeloEvent
			 DAQ/DAQKernel
                         Si/SiDAQ)

find_package(Boost)
find_package(ROOT)
# hide warnings from some external projects
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})

gaudi_add_module(VeloDAQ
                 src/*.cpp
                 INCLUDE_DIRS DAQ/Tell1Kernel Event/DigiEvent Event/VeloEvent Si/SiDAQ DAQ/DAQKernel
                 LINK_LIBRARIES VeloDetLib DAQEventLib DAQKernelLib RecEvent)

gaudi_install_python_modules()

