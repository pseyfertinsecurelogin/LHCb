#!/usr/bin/env python
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
# coding=utf-8
from __future__ import print_function

import os
import shutil
import tempfile
import itertools

from pprint import pprint

from GitCondDB import IOVs as I


def make_entries(iterable):
    output = []
    for data in iterable:
        if isinstance(data[1], tuple):
            output.append(I.NestedEntry(data[0], data[1][0],
                                        make_entries(data[1][1])))
        else:
            output.append(I.Entry(*data))
    return output


TMP_DIR = None
Cond1_IOVs = make_entries([(0, 'zero')] +
                          zip(range(1270049171000000000,
                                    1270116165000000000,
                                    15000000000000),
                              ('abc{:03d}'.format(i)
                               for i in itertools.count())))


def tree(path):
    'helper to recursively list all files and dirs in a directory'
    if not os.path.isdir(path):
        return None
    return dict((name, tree(os.path.join(path, name)))
                for name in os.listdir(path))


def setup():
    'prepare test area'
    global TMP_DIR
    TMP_DIR = tempfile.mkdtemp()
    os.chdir(TMP_DIR)
    os.makedirs('repo1')

def teardown():
    'clean up'
    global TMP_DIR
    os.chdir(os.pardir)
    shutil.rmtree(TMP_DIR)
    TMP_DIR = None


def test_write_flat():
    I.write_iovs('repo1/Cond1', Cond1_IOVs)
    assert tree('repo1/Cond1') == {'IOVs': None}
    expected = '\n'.join('{} {}'.format(since, payload)
                         for since, payload in Cond1_IOVs) + '\n'
    with open('repo1/Cond1/IOVs') as iovs:
        assert iovs.read() == expected

def test_parse_flat():
    data = I.parse_iovs('repo1/Cond1')
    assert data == Cond1_IOVs

def test_partition():
    data = I.partition_by_month(Cond1_IOVs)
    expected = make_entries(
        [(0, ('1970-01', [(0, '../zero')])),
         (1270049171000000000, ('2010-03', [(1270049171000000000, '../abc000'),
                                            (1270064171000000000, '../abc001'),
                                            (1270079171000000000, '../abc002')])),
         (1270094171000000000, ('2010-04', [(1270094171000000000, '../abc003'),
                                            (1270109171000000000, '../abc004')]))])
    print('expected:')
    pprint(expected)
    print('data:')
    pprint(data)
    assert data == expected

def test_remove_flat():
    assert tree('repo1/Cond1') == {'IOVs': None}
    I.remove_iovs('repo1/Cond1')
    assert not os.path.exists('repo1/Cond1')

def test_write_partitions():
    data = I.partition_by_month(Cond1_IOVs)
    I.write_iovs('repo1/Cond1', data)
    expected = {'IOVs': None,
                '1970-01': {'IOVs': None},
                '2010-03': {'IOVs': None},
                '2010-04': {'IOVs': None}}
    print(expected)
    print(tree('repo1/Cond1'))
    assert tree('repo1/Cond1') == expected

def test_parse_partitions():
    print(open('repo1/Cond1/IOVs').read())
    data = I.parse_iovs('repo1/Cond1')
    expected = I.partition_by_month(Cond1_IOVs)
    print(data)
    print(expected)
    assert data == expected

def test_simplify():
    data = I.partition_by_month(Cond1_IOVs)
    data = list(I.simplify(data))
    assert data == Cond1_IOVs

def test_remove_partitions():
    before = {'IOVs': None,
              '1970-01': {'IOVs': None},
              '2010-03': {'IOVs': None},
              '2010-04': {'IOVs': None}}
    assert tree('repo1/Cond1') == before
    I.remove_iovs('repo1/Cond1')
    assert not os.path.exists('repo1/Cond1')

def test_clean():
    I.write_iovs('repo1/Cond1', Cond1_IOVs)
    I.write_iovs('repo1/Cond2', Cond1_IOVs)

    I.clean_iovs('repo1')

    expected = I.partition_by_month(Cond1_IOVs)
    data1 = I.parse_iovs('repo1/Cond1')
    data2 = I.parse_iovs('repo1/Cond2')

    assert data1 == expected
    assert data2 == expected

def test_add_iov():
    orig = make_entries([(0, '0x0'),
                         (10, '0xa'),
                         (20, '0x14'),
                         (30, '0x1e'),
                         (40, '0x28'),
                         (50, '0x32'),
                         (60, '0x3c'),
                         (70, '0x46'),
                         (80, '0x50'),
                         (90, '0x5a')])

    # starting from 0
    new = list(I.add_iov(orig, 'added', 0, 55))
    expected = make_entries([(0, 'added'), (55, '0x32'),
                             (60, '0x3c'), (70, '0x46'),
                             (80, '0x50'), (90, '0x5a')])
    # pprint(new)
    assert new == expected

    # in the middle
    new = list(I.add_iov(orig, 'added', 15, 75))
    expected = make_entries([(0, '0x0'), (10, '0xa'),
                             (15, 'added'), (75, '0x46'),
                             (80, '0x50'), (90, '0x5a')])
    # pprint(new)
    assert new == expected

    # at the end
    new = list(I.add_iov(orig, 'added', 15, I.IOV_MAX))
    expected = make_entries([(0, '0x0'), (10, '0xa'),
                             (15, 'added')])
    # pprint(new)
    assert new == expected

    # small
    new = list(I.add_iov(orig, 'added', 12, 18))
    expected = make_entries([(0, '0x0'), (10, '0xa'),
                             (12, 'added'), (18, '0xa'),
                             (20, '0x14'), (30, '0x1e'),
                             (40, '0x28'), (50, '0x32'),
                             (60, '0x3c'), (70, '0x46'),
                             (80, '0x50'), (90, '0x5a')])
    # pprint(new)
    assert new == expected

    # with matching boundaries
    new = list(I.add_iov(orig, 'added', 20, 80))
    expected = make_entries([(0, '0x0'),
                             (10, '0xa'),
                             (20, 'added'),
                             (80, '0x50'),
                             (90, '0x5a')])
    # pprint(new)
    assert new == expected
