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
# coding=utf-8
from __future__ import print_function

import os
import sys
import shutil
import tempfile
import itertools

from pprint import pprint

from GitCondDB import Payload as P

TMP_DIR = None


def setup():
    'prepare test area'
    global TMP_DIR
    TMP_DIR = tempfile.mkdtemp()
    os.chdir(TMP_DIR)

def teardown():
    'clean up'
    global TMP_DIR
    os.chdir(os.pardir)
    shutil.rmtree(TMP_DIR)
    TMP_DIR = None

def test_clean_system_refs():
    orig = '''<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE DDDB SYSTEM "../../DTD/structure.dtd"[
<!ENTITY ITGeomParams SYSTEM "ITGeomParams.ent">
<!ENTITY BoxesOffsets SYSTEM "conddb:/T/IT/BoxesOffsets.ent">]>
<DDDB>
  <catalog name="dd">
    <catalogref href="Conditions/MainCatalog.xml#Conditions" />
  </catalog>
</DDDB>
'''
    expected = '''<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE DDDB SYSTEM "git:/DTD/structure.dtd"[
<!ENTITY ITGeomParams SYSTEM "git:/T/IT/ITGeomParams.ent">
<!ENTITY BoxesOffsets SYSTEM "git:/T/IT/BoxesOffsets.ent">]>
<DDDB>
  <catalog name="dd">
    <catalogref href="conddb:/Conditions/MainCatalog.xml#Conditions" />
  </catalog>
</DDDB>
'''
    fixed = P.fix_system_refs(orig, '/DDDB', '/DDDB/T/IT/geometry.xml')
    print('fixed:')
    print(fixed)
    print('expected:')
    print(expected)
    assert fixed == expected

def test_clean_endlines():
    orig = '\n'.join(['a  ', 'b', '\r', '    ', ' c ', '', ''])
    fixed = P.fix_lines_ends(orig)
    expected = 'a\nb\n\n\n c\n\n'
    assert fixed == expected

def test_checksum():
    data = 'uheuh934u v09834u9 .\n8th3m9e8fjm[3089u m[308 nt[er]]]\n'
    assert P.checksum(data) == 'd6c2cbffa5b21cd5bf1a874bb72d7b1ba996b296'
    assert P.payload_filename(data) == 'd6c2cbffa5'
