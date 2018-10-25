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

import RawEventFormat
if len(RawEventFormat.Raw_location_db)==0:
    raise AttributeError("Nothing in the database!")

if len(RawEventFormat.Reco_dict_db)==0:
    raise AttributeError("Nothing in the database!")

from Configurables import RawEventFormatConf
RawEventFormatConf().forceLoad()

print "Pass"
