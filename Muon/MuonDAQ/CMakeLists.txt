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
# Package: MuonDAQ
################################################################################
gaudi_subdir(MuonDAQ v4r22)

gaudi_depends_on_subdirs(DAQ/DAQUtils
                         Det/MuonDet
                         Event/DAQEvent
                         Event/DigiEvent
                         Event/RecEvent
                         GaudiAlg
                         DAQ/DAQKernel
                         Muon/MuonKernel)

gaudi_add_module(MuonDAQ
                 src/*.cpp
                 INCLUDE_DIRS DAQ/DAQUtils Event/DigiEvent
                 LINK_LIBRARIES MuonDetLib DAQEventLib DAQKernelLib RecEvent GaudiAlgLib MuonKernelLib)

gaudi_add_dictionary(MuonDAQ
                     dict/MuonDAQDict.h
                     dict/MuonDAQDict.xml
                     INCLUDE_DIRS DAQ/DAQUtils Event/DigiEvent
                     LINK_LIBRARIES MuonDetLib DAQEventLib RecEvent GaudiAlgLib MuonKernelLib
                     OPTIONS "-U__MINGW32__ -DBOOST_DISABLE_ASSERTS")

gaudi_install_headers(Event MuonDAQ)

