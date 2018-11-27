#!/usr/bin/env python
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

# $ Id: $
#
# Marco Clemencic <Marco.Clemencic@cern.ch>
#
# Script to check when a tag becomes available on a database.

from sys import argv, exit
from time import sleep, asctime

import CondDBUI

if len(argv) < 3:
    print "Usage: %s <tag> <connection_string> [connection_string ...]"%argv[0]
    exit(1)

tag = argv[1]
# Connect to the databases
DBs = []
for conn in argv[2:] :
    DBs.append(CondDBUI.CondDB(conn))

# retry every minute for 13h
sleeptime = 60
trials = 13 * 60
while DBs and trials > 0:
    to_retry = []
    for db in DBs:
        if db.isTagReady(tag):
            print "======== Found TAG '%s' in DB '%s' at %s"%(tag,db.db.databaseId(),asctime())
        else:
            to_retry.append(db)
    DBs = to_retry
    trials -= 1
    if DBs and trials > 0 :
        sleep(sleeptime)

if DBs:
    print "======== Could not find TAG '%s' in:"%tag
    for db in DBs:
        print "========\t%s"%db.db.databaseId()
