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
import RawEventCompat
import RawEventCompat.Configuration as conf

from Configurables import InputCopyStream
writer = InputCopyStream("SplitStream")

from GaudiConf import IOHelper

IOHelper().outStream("crazy-split.dst", writer)

juggle = conf.RawEventJuggler()
juggle.Input = 0.1
juggle.Output = 999
juggle.KillExtraNodes = True
juggle.WriterItemList = writer

from Gaudi.Configuration import *
from Configurables import GaudiSequencer

juggle.Sequencer = GaudiSequencer("SPAM")

from Configurables import LHCbApp
LHCbApp()

ApplicationMgr().TopAlg = [GaudiSequencer("SPAM")]
