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
import shutil
import tempfile

from pprint import pprint

import add_files_to_gitconddb as S


def tree(path):
    'helper to recursively list all files and dirs in a directory'
    if not os.path.isdir(path):
        with open(path, 'rb') as f:
            return f.read()
    return dict(
        (name, tree(os.path.join(path, name))) for name in os.listdir(path))


def dumptree(tree_dict, path, empty=False):
    '''
    opposit of tree, take a dictionary and generate the corresponding tree
    on the filesystem
    '''
    if not os.path.exists(path):
        os.makedirs(path)
    elif empty:
        shutil.rmtree(path)
        os.makedirs(path)
    for k, v in tree_dict.items():
        k = os.path.join(path, k)
        if isinstance(v, dict):
            dumptree(v, k)
        else:
            with open(k, 'wb') as f:
                f.write(v)


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


def test_from_scratch():
    expected = {
        'Conditions': {
            'Online': {
                'Cond1': 'cond1\n',
                'Cond2': 'cond2\n'
            }
        }
    }
    dumptree(expected, 'source', True)
    S.git_conddb_extend('source', 'dest')
    # print(tree('dest'))
    assert tree('dest') == expected


def test_add_noop():
    expected = {
        'Conditions': {
            'Online': {
                'Cond1': 'cond1\n',
                'Cond2': 'cond2\n'
            }
        }
    }
    dumptree(expected, 'source', True)
    S.git_conddb_extend('source', 'dest')
    # print(tree('dest'))
    assert tree('dest') == expected


def test_add_one():
    expected = {
        'Conditions': {
            'Online': {
                'Cond1': 'cond1\n',
                'Cond2': 'cond2\n',
                'Cond3': 'dummy\n'
            }
        }
    }
    dumptree(expected, 'source', True)
    S.git_conddb_extend('source', 'dest')
    # print(tree('dest'))
    assert tree('dest') == expected


def test_add_from_partial():
    expected = {
        'Conditions': {
            'Online': {
                'Cond1': 'cond1\n',
                'Cond2': 'cond2\n',
                'Cond3': 'dummy\n',
                'Cond4': 'stuff\n'
            }
        }
    }
    dump = {'Conditions': {'Online': {'Cond4': 'stuff\n'}}}
    dumptree(dump, 'source', True)
    S.git_conddb_extend('source', 'dest')
    # print(tree('dest'))
    assert tree('dest') == expected


def test_add_iov_left():
    src = {'Conditions': {'Online': {'Cond1': 'before\n'}}}
    dumptree(src, 'source', True)

    expected = {
        'Conditions': {
            'Online': {
                'Cond1': {
                    'dcd8448175': 'cond1\n',
                    '7473def722': 'before\n',
                    'IOVs': ('0 7473def722\n'
                             '1490880850000000000 dcd8448175\n')
                },
                'Cond2': 'cond2\n',
                'Cond3': 'dummy\n',
                'Cond4': 'stuff\n'
            }
        }
    }
    S.git_conddb_extend('source', 'dest', until=1490880850000000000)
    pprint(tree('dest'))
    assert tree('dest') == expected


def test_add_iov_right():
    src = {'Conditions': {'Online': {'Cond2': 'after\n'}}}
    dumptree(src, 'source', True)

    expected = {
        'Conditions': {
            'Online': {
                'Cond1': {
                    'dcd8448175': 'cond1\n',
                    '7473def722': 'before\n',
                    'IOVs': ('0 7473def722\n'
                             '1490880850000000000 dcd8448175\n')
                },
                'Cond2': {
                    '454bba8c94': 'cond2\n',
                    'a214ad86e2': 'after\n',
                    'IOVs': ('0 454bba8c94\n'
                             '1490880850000000000 a214ad86e2\n')
                },
                'Cond3': 'dummy\n',
                'Cond4': 'stuff\n'
            }
        }
    }
    S.git_conddb_extend('source', 'dest', since=1490880850000000000)
    pprint(tree('dest'))
    assert tree('dest') == expected


def test_add_iov():
    src = {'Conditions': {'Online': {'Cond3': 'center\n'}}}
    dumptree(src, 'source', True)

    expected = {
        'Conditions': {
            'Online': {
                'Cond1': {
                    'dcd8448175': 'cond1\n',
                    '7473def722': 'before\n',
                    'IOVs': ('0 7473def722\n'
                             '1490880850000000000 dcd8448175\n')
                },
                'Cond2': {
                    '454bba8c94': 'cond2\n',
                    'a214ad86e2': 'after\n',
                    'IOVs': ('0 454bba8c94\n'
                             '1490880850000000000 a214ad86e2\n')
                },
                'Cond3': {
                    'f161ebd296':
                    'dummy\n',
                    'a87b419f4d':
                    'center\n',
                    'IOVs': ('0 f161ebd296\n'
                             '1490880800000000000 a87b419f4d\n'
                             '1490880850000000000 f161ebd296\n')
                },
                'Cond4': 'stuff\n'
            }
        }
    }
    S.git_conddb_extend(
        'source', 'dest', since=1490880800000000000, until=1490880850000000000)
    pprint(tree('dest'))
    assert tree('dest') == expected


def test_add_iov_new():
    src = {'Conditions': {'Online': {'Cond5': 'unlimited\n'}}}
    dumptree(src, 'source', True)

    expected = {
        'Conditions': {
            'Online': {
                'Cond1': {
                    'dcd8448175': 'cond1\n',
                    '7473def722': 'before\n',
                    'IOVs': ('0 7473def722\n'
                             '1490880850000000000 dcd8448175\n')
                },
                'Cond2': {
                    '454bba8c94': 'cond2\n',
                    'a214ad86e2': 'after\n',
                    'IOVs': ('0 454bba8c94\n'
                             '1490880850000000000 a214ad86e2\n')
                },
                'Cond3': {
                    'f161ebd296':
                    'dummy\n',
                    'a87b419f4d':
                    'center\n',
                    'IOVs': ('0 f161ebd296\n'
                             '1490880800000000000 a87b419f4d\n'
                             '1490880850000000000 f161ebd296\n')
                },
                'Cond4': 'stuff\n',
                'Cond5': 'unlimited\n'
            }
        }
    }
    S.git_conddb_extend(
        'source', 'dest', since=1490880800000000000, until=1490880850000000000)
    pprint(tree('dest'))
    assert tree('dest') == expected


def test_add_iov_to_iov():
    src = {'Conditions': {'Cond1': 'new value'}}
    dumptree(src, 'source', True)

    dst = {
        'Conditions': {
            'Cond1': {
                'dcd8448175': 'cond1\n',
                'IOVs': '0 dcd8448175\n'
            }
        }
    }
    dumptree(dst, 'dest', True)

    expected = {
        'Conditions': {
            'Cond1': {
                '1970-01': {
                    'IOVs': '0 ../dcd8448175\n'
                },
                '2017-03': {
                    'IOVs': ('1490880800000000000 ../4d9f5cf4b2\n'
                             '1490880850000000000 ../dcd8448175\n')
                },
                'dcd8448175': 'cond1\n',
                '1970-01': {
                    'IOVs': '0 ../dcd8448175\n'
                },
                '4d9f5cf4b2': 'new value\n',
                'IOVs': ('0 1970-01\n'
                         '1490880800000000000 2017-03\n')
            }
        }
    }
    S.git_conddb_extend(
        'source', 'dest', since=1490880800000000000, until=1490880850000000000)
    pprint(tree('dest'))
    assert tree('dest') == expected


def test_add_iov_hide_payload():
    src = {'Conditions': {'Cond1': 'new value 2'}}
    dumptree(src, 'source', True)

    dst = {
        'Conditions': {
            'Cond1': {
                '1970-01': {
                    'IOVs': '0 ../dcd8448175\n'
                },
                '2017-03': {
                    'IOVs': ('1490880800000000000 ../4d9f5cf4b2\n'
                             '1490880850000000000 ../dcd8448175\n')
                },
                'dcd8448175': 'cond1\n',
                '4d9f5cf4b2': 'new value\n',
                'IOVs': ('0 1970-01\n'
                         '1490880800000000000 2017-03\n')
            }
        }
    }
    dumptree(dst, 'dest', True)

    expected = {
        'Conditions': {
            'Cond1': {
                '1970-01': {
                    'IOVs': '0 ../16a605d2d0\n'
                },
                '2017-03': {
                    'IOVs': '1490880900000000000 ../dcd8448175\n'
                },
                'dcd8448175': 'cond1\n',
                '16a605d2d0': 'new value 2\n',
                'IOVs': ('0 1970-01\n'
                         '1490880900000000000 2017-03\n')
            }
        }
    }
    S.git_conddb_extend('source', 'dest', until=1490880900000000000)
    pprint(tree('dest'))
    assert tree('dest') == expected


def test_add_remove_iov():
    src = {'Conditions': {'Cond1': 'unlimited\n'}}
    dumptree(src, 'source', True)

    dst = {
        'Conditions': {
            'Cond1': {
                '1970-01': {
                    'IOVs': '0 ../dcd8448175\n'
                },
                '2017-03': {
                    'IOVs': ('1490880800000000000 ../4d9f5cf4b2\n'
                             '1490880850000000000 ../dcd8448175\n')
                },
                'dcd8448175': 'cond1\n',
                '4d9f5cf4b2': 'new value\n',
                'IOVs': ('0 1970-01\n'
                         '1490880800000000000 2017-03\n')
            }
        }
    }
    dumptree(dst, 'dest', True)

    S.git_conddb_extend('source', 'dest')
    pprint(tree('dest'))
    assert tree('dest') == src
