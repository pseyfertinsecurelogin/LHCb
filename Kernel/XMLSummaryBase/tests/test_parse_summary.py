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
# =============================================================================
# =============================================================================
"""
*******************************************************************************
*      test the summary parsing.                                              *
*      python  test_parse_summary.py                                          *
*******************************************************************************
"""
# =============================================================================
__author__ = 'Rob Lambert Rob.Lambert@cern.ch'
# =============================================================================

#test I can import it!
from XMLSummaryBase import summary

topdir = '$XMLSUMMARYBASEROOT/'
xsdfile = 'xml/XMLSummary.xsd'
xmlfile1 = 'xml/Summary_Minimal.xml'
xmlfile2 = 'xml/Summary_Example.xml'

#test I can make the helper classes
sum = summary.Summary(topdir + xsdfile)
sum1 = summary.Summary(topdir + xsdfile)
print 'summary class successful'

#test I can parse into the helper classes
sum1.parse(topdir + xmlfile1)
sum.parse(topdir + xmlfile2)
print 'parsed example xml OK'
sum.dump()

sum1.fill_counter('test/test', 1)
sum1.fill_counter('test/test', 1)

sum1.fill_counter('test/testStat', 100, 100, 100, 100, 100)
sum1.fill_counter('test/testStat', 100, 100, 100, 0, 0)

sum1.fill_input('test/test.dst', addevents=1)
sum1.fill_input('test/test.dst', addevents=1)

sum1.fill_output('test/testoutput.dst', addevents=1)
sum1.fill_output('test/testoutput.dst', addevents=1)

print 'extra methods tested'
sum1.dump()
sum1.write('testtest.xml')
print 'printing/writing tested'

#test I can merge different files
merge1 = summary.Merge([topdir + xmlfile1, topdir + xmlfile2])
merge2 = summary.Merge([topdir + xmlfile2, topdir + xmlfile2])
sum.fill_output('test/testoutput2.dst', addevents=100)
merge3 = summary.Merge([sum, sum])
print 'merge OK'
merge3.dump()
