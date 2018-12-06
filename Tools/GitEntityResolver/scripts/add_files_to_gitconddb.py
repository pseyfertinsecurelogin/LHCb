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
Script to add a set of files to Git CondDB (for an optional IOV).
'''
import os
import logging
from subprocess import check_output, STDOUT, CalledProcessError
from GitCondDB.IOVs import IOV_MIN, IOV_MAX
from GitCondDB.Payload import fix_system_refs, fix_lines_ends
from GitCondDB._helpers import _add_file


def git_conddb_add_file(source, dest, root, since=IOV_MIN, until=IOV_MAX):
    '''
    copy a file to a Git CondDB location for the given IOV
    '''
    logging.debug('adding %s', os.path.relpath(dest, root))
    data = fix_lines_ends(fix_system_refs(open(source).read(), root, dest))
    _add_file(data, dest, (since, until))


def git_conddb_extend(source,
                      dest,
                      since=IOV_MIN,
                      until=IOV_MAX,
                      dest_root=None):
    logging.debug('adding %s to %s, from %d to %d', source, dest, since, until)
    if dest_root is None:
        dest_root = dest
    if os.path.isfile(source):
        git_conddb_add_file(source, dest, dest_root, since, until)
    else:
        for root, _, filenames in os.walk(source):
            for f in filenames:
                src_file = os.path.join(root, f)
                dst_file = os.path.join(dest, os.path.relpath(
                    src_file, source))
                git_conddb_add_file(src_file, dst_file, dest_root, since,
                                    until)


def guess_root(path):
    logging.debug('looking for DB root of %s', path)
    while not os.path.isdir(path):
        path = os.path.dirname(path)
    return check_output(['git', 'rev-parse', '--show-toplevel'],
                        cwd=path,
                        stderr=STDOUT).strip()


def main():
    from argparse import ArgumentParser
    parser = ArgumentParser()
    parser.add_argument('source')
    parser.add_argument('destination')

    parser.add_argument(
        '--since',
        type=int,
        default=IOV_MIN,
        help='start of validity for the files')
    parser.add_argument(
        '--until',
        type=int,
        default=IOV_MAX,
        help='end of validity for the files')

    parser.add_argument(
        '--dest-root',
        help='root directory of the destination database, required only if it'
        ' cannot be guessed')

    parser.add_argument(
        '--quiet',
        action='store_const',
        const=logging.WARNING,
        dest='log_level',
        help='minimal output messages [default]')
    parser.add_argument(
        '--verbose',
        action='store_const',
        const=logging.INFO,
        dest='log_level',
        help='print more details')
    parser.add_argument(
        '--debug',
        action='store_const',
        const=logging.DEBUG,
        dest='log_level',
        help='debug printout')

    parser.set_defaults(log_level=logging.WARNING)

    args = parser.parse_args()
    logging.basicConfig(level=args.log_level)

    if args.dest_root is None:
        try:
            args.dest_root = guess_root(args.destination)
        except CalledProcessError:
            parser.error('destination %s does not seem in a Git repository, '
                         'use --dest-root' % args.destination)

    git_conddb_extend(args.source, args.destination, args.since, args.until,
                      args.dest_root)


if __name__ == '__main__':
    main()
