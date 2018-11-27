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

from RawEventFormat import Raw_location_db

banks=[]
for v in Raw_location_db:
    banks=banks+[b for b in Raw_location_db[v] if b not in banks]

Raw_location_db[0.1]={}
Raw_location_db[999]={}
for b in banks:
    Raw_location_db[0.1][b]="DAQ/RawEvent"
    Raw_location_db[999][b]="Crazy/RawEvent"



conf.RawEventFormatConf().loadIfRequired()
