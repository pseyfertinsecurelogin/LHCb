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

import unittest

import sys

#import string
#import random

class DetCondReadingTest(unittest.TestCase):

#    def assertEqualsConfig(self, lhs, rhs):
#        self.assertEquals(lhs.getFullName(), rhs.getFullName())

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.testDB = 'sqlite_file:../data/TESTDB3.db/TESTDB3'
        self.testFolder = '/lhcb.xml'
        self.testout = '<?xml version="1.0" encoding="ISO-8859-1"?>\n<!DOCTYPE DDDB SYSTEM "conddb:/DTD/structure.dtd">\n<DDDB>\n  <catalog name="dd">\n     <catalogref href="AutoMap" />\n  </catalog> \n</DDDB> \n'
        self.Nmethods = 1 # number of methods

    def readDB(self):
        from CondDBUI import CondDB
        db = CondDB(self.testDB)
        s = self.testFolder
        if not db.existsFolder(s): return ""
        data = db.getPayload(s,0,0)
        if 'data' not in data: return ""
        return data['data']

#    def tearDown(self):
#        unittest.TestCase.tearDown(self)

    def test_reading(self):
        """Check CondDB reading """
        ret = self.readDB()
        self.assertEquals(self.testout, ret)

if __name__ == '__main__':
    unittest.main(testRunner = unittest.TextTestRunner(stream=sys.stdout,verbosity=2))
