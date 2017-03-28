#!/usr/bin/env python
# coding=utf-8
from __future__ import print_function

import os
import sys
import shutil
import tempfile
import itertools

from pprint import pprint

import GitCondDB.IOVs

TMP_DIR = None
Cond1_IOVs = [(0, 'zero')] + zip(range(1270049171000000000,
                                       1270116165000000000,
                                       15000000000000),
                                 ('abc{:03d}'.format(i)
                                  for i in itertools.count()))


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
    GitCondDB.IOVs.write_iovs('repo1/Cond1', Cond1_IOVs)
    assert tree('repo1/Cond1') == {'IOVs': None}
    expected = '\n'.join('{} {}'.format(since, payload)
                         for since, payload in Cond1_IOVs) + '\n'
    with open('repo1/Cond1/IOVs') as iovs:
        assert iovs.read() == expected

def test_parse_flat():
    data = GitCondDB.IOVs.parse_iovs('repo1/Cond1')
    assert data == Cond1_IOVs

def test_partition():
    data = GitCondDB.IOVs.partition_iovs(Cond1_IOVs)
    expected = [(0, 'zero'),
                (1267401600000000000, ('2010-03', [(0, '../zero'),
                                                   (1270049171000000000, '../abc000'),
                                                   (1270064171000000000, '../abc001'),
                                                   (1270079171000000000, '../abc002')])),
                (1270080000000000000, ('2010-04', [(1270079171000000000, '../abc002'),
                                                   (1270094171000000000, '../abc003'),
                                                   (1270109171000000000, '../abc004')]))]
    print('expected:')
    pprint(expected)
    print('data:')
    pprint(data)
    assert data == expected

def test_remove_flat():
    assert tree('repo1/Cond1') == {'IOVs': None}
    GitCondDB.IOVs.remove_iovs('repo1/Cond1')
    assert not os.path.exists('repo1/Cond1')

def test_write_partitions():
    data = GitCondDB.IOVs.partition_iovs(Cond1_IOVs)
    GitCondDB.IOVs.write_iovs('repo1/Cond1', data)
    expected = {'IOVs': None,
                '2010-03': {'IOVs': None},
                '2010-04': {'IOVs': None}}
    print(expected)
    print(tree('repo1/Cond1'))
    assert tree('repo1/Cond1') == expected

def test_parse_partitions():
    data = GitCondDB.IOVs.parse_iovs('repo1/Cond1')
    expected = GitCondDB.IOVs.partition_iovs(Cond1_IOVs)
    assert data == expected

def test_flatten_and_clean():
    data = GitCondDB.IOVs.partition_iovs(Cond1_IOVs)
    data = GitCondDB.IOVs.flatten_iovs(data)
    data = GitCondDB.IOVs.remove_dummy_entries(data)
    assert data == Cond1_IOVs

def test_remove_partitions():
    before = {'IOVs': None,
              '2010-03': {'IOVs': None},
              '2010-04': {'IOVs': None}}
    assert tree('repo1/Cond1') == before
    GitCondDB.IOVs.remove_iovs('repo1/Cond1')
    assert not os.path.exists('repo1/Cond1')

def test_clean():
    GitCondDB.IOVs.write_iovs('repo1/Cond1', Cond1_IOVs)
    GitCondDB.IOVs.write_iovs('repo1/Cond2', Cond1_IOVs)

    GitCondDB.IOVs.clean_iovs('repo1')

    expected = GitCondDB.IOVs.partition_iovs(Cond1_IOVs)
    data1 = GitCondDB.IOVs.parse_iovs('repo1/Cond1')
    data2 = GitCondDB.IOVs.parse_iovs('repo1/Cond2')

    assert data1 == expected
    assert data2 == expected
