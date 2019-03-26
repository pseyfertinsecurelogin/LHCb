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
'''
Basic consistency checks of a Git CondDB.
'''
import os
import sys
from os.path import exists, dirname, join, normpath

_found_payloads = set()


def check_iovs(path):
    '''
    Check an IOVs file to make sure its lines are in chronological order and the
    paylods it refers to exist.
    '''
    global _found_payloads
    is_ok = True
    last_key = -1
    dirpath = dirname(path)

    for n, l in enumerate(open(path)):
        try:
            key, payload = l.strip().split(None, 1)
            key = int(key)
            if last_key > key:
                print('{0}:{1}: error: non monotonic time {2} > {3}'.format(
                    path, n + 1, last_key, key))
                is_ok = False
            elif last_key == key:
                print('{0}:{1}: warning: repeated time {2}'.format(
                    path, n + 1, key))
            last_key = key
            payload = normpath(join(dirpath, payload))
            if payload not in _found_payloads:
                if exists(payload):
                    _found_payloads.add(payload)
                else:
                    print('{0}:{1}: error: payload not found "{2}"'.format(
                        path, n + 1, payload))
                    is_ok = False
        except:
            print('{0}:{1}: error: invalid line "{2}"'.format(
                path, n + 1, l.strip()))
            is_ok = False
    return is_ok


def process(path):
    '''
    run all the checks
    '''
    is_ok = True
    count = 0
    for dirpath, dirnames, filenames in os.walk(path):
        if 'IOVs' in filenames:
            count += 1
            is_ok = check_iovs(join(dirpath, 'IOVs')) and is_ok
    print('checked {0} files'.format(count))
    return is_ok


def main():
    from optparse import OptionParser
    parser = OptionParser(usage='%prog [options] [path]')

    opts, args = parser.parse_args()
    if len(args) > 1:
        parser.error('can work only on one repository at a time')
    if not process(args[0] if args else os.curdir):
        sys.exit(1)


if __name__ == '__main__':
    main()
