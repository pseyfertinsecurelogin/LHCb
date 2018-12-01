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
# Package: DAQSys
################################################################################
gaudi_subdir(DAQSys v8r12)

gaudi_depends_on_subdirs(Event/LumiEvent
			 Calo/CaloDAQ
                         L0/L0Calo
                         L0/L0DU
                         L0/L0Muon
                         Muon/MuonDAQ
                         OT/OTDAQ
                         Rich/RichDAQ
                         Si/SiDAQ
                         ST/STDAQ
                         FT/FTDAQ
                         Velo/VeloDAQ
                         VP/VPDAQ
                         Kernel/LHCbAlgs
                         GaudiConf
                         DAQ/RawEventCompat)

gaudi_install_python_modules()

gaudi_add_test(QMTest QMTEST)
