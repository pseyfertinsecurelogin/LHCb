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
# Package: OTDAQ
################################################################################
gaudi_subdir(OTDAQ)

gaudi_depends_on_subdirs(Det/OTDet
                         Event/DAQEvent
                         Event/DigiEvent
			 DAQ/DAQKernel
                         GaudiAlg)


gaudi_add_module(OTDAQ
                 src/*.cpp
                 INCLUDE_DIRS Event/DigiEvent
                 LINK_LIBRARIES OTDetLib DAQKernelLib DAQEventLib GaudiAlgLib)

gaudi_add_dictionary(OTDAQ
                     dict/OTDAQDict.h
                     dict/OTDAQDict.xml
                     INCLUDE_DIRS Event/DigiEvent
                     LINK_LIBRARIES OTDetLib DAQEventLib GaudiAlgLib
                     OPTIONS "-U__MINGW32__")

gaudi_install_headers(Event OTDAQ)

