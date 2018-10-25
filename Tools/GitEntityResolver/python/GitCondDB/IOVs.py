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
'''
Utils for IOVs manipulations in Git CondDBs.
'''
import os
import re
import logging

from datetime import datetime
from collections import namedtuple

IOV_MIN = 0
IOV_MAX = 0x7fffffffffffffff

# Format YYYY-MM-DD[_HH:MM[:SS.SSS]][UTC] or YYYY-MM-DD[Thh:mm[:ss.sss]][Z]
TIME_STRING_RE = re.compile(
    r'^(?P<year>[0-9]{4})-(?P<month>[0-9]{2})-(?P<day>[0-9]{2})'
    r'(?:[_T ](?P<hour>[0-9]{2}):(?P<minute>[0-9]{2})(?::(?P<second>[0-9]{2})'
    r'(?:\.(?P<decimal>[0-9]*))?)?)?'
    r'(?P<utc>UTC|Z)?$')


Entry = namedtuple('Entry', ['since', 'key'])
NestedEntry = namedtuple('NestedEntry', ['since', 'key', 'entries'])


class IOV(object):
    __slots__ = ('since', 'until')
    def __init__(self, since, until):
        self.since, self.until = since, until
    def ovelap(self, other):
        '''
        Check if two IOVs ovelap.
        '''
        return self.intersection(other).isValid()
    def intersection(self, other):
        return IOV(max(self.since, other.since), min(self.until, other.until))
    def isValid(self):
        return self.since < self.until


def iov_files(path):
    '''
    Get list of IOVs files in a path (recusively).
    '''
    for root, dirs, files in os.walk(path):
        if 'IOVs' in files:
            dirs[:] = []  # do not recurse further
            yield os.path.join(root, 'IOVs')


def parse_iov_line(path, l):
    '''
    Helper function to parse a line in the IOVs file rooted at path.

    Return the start of validity and the path to the payload entry.
    '''
    since, payload = l.strip().split()
    return Entry(int(since), os.path.normpath(os.path.join(path, payload)))


def parse_iovs(path):
    '''
    given the path to a folder with IOVs, return a list of pairs with since
    and either a key or a pair key and the result of the recursive call of this
    function on the nested level
    '''
    def parse_line(l):
        since, key = l.split()
        since = int(since)
        subpath = os.path.join(path, key)
        if os.path.exists(os.path.join(subpath, 'IOVs')):
            return NestedEntry(since, key, parse_iovs(subpath))
        else:
            return Entry(since, key)

    with open(os.path.join(path, 'IOVs')) as iovs:
        return map(parse_line, iovs)


def remove_iovs(path):
    '''
    delete IOVs files from the selected directory
    '''
    for root, dirs, files in os.walk(path, topdown=False):
        if 'IOVs' in files:
            # remove the IOVs file if present
            os.remove(os.path.join(root, 'IOVs'))
            # prune the directory if empty
            # (with topdown == False dirs is set before we recurse and prune)
            dirs = [d for d in dirs if os.path.exists(os.path.join(root, d))]
            if files == ['IOVs'] and not dirs:
                os.rmdir(root)


def flatten(iovs, nested=''):
    from os.path import join, normpath
    for entry in iovs:
        try:
            for item in flatten(entry.entries,join(nested, entry.key)):
                yield item
        except AttributeError:
            # It's a plain Entry instance, no recursion
            yield Entry(entry.since, normpath(join(nested, entry.key)))


def simplify(iovs):
    iovs = flatten(iovs)
    last_entry = iovs.next()
    for entry in iovs:
        if entry.key == last_entry.key or entry.since < last_entry.since:
            # skip this element if same as previous or wrong order
            continue
        if entry.since != last_entry.since:
            # previous is valid only if not empty
            yield last_entry
        last_entry = entry
    yield last_entry


def _deepen_key(entry):
    from os.path import join
    key = join('..', entry.key)
    if isinstance(entry, NestedEntry):
        return NestedEntry(entry.since, key, entry.entries)
    else:
        return Entry(entry.since, key)


def partition(iovs, since2key):
    from itertools import groupby
    iovs = simplify(iovs)
    groups = groupby(iovs, lambda e: since2key(e.since))
    return [NestedEntry(entries[0].since, key, entries)
            for key, entries in [(key, map(_deepen_key, entries))
                                 for key, entries in groups]]


def partition_by_count(iovs, count, keys=None):
    from itertools import islice, count
    if keys is None:
        keys = (str(i) for i in count)
    iovs = simplify(iovs)
    return [NestedEntry(entries[0].since, key, entries)
            for key, entries in [(keys.next(), map(_deepen_key, entries))
                                 for entries in islice(iovs, count)]]

def partition_by_month(iovs):
    '''
    partition IOVs per month
    '''
    from itertools import groupby
    from os.path import join

    def month_key(since):
        return datetime.utcfromtimestamp(since * 1e-9).date().strftime('%Y-%m')

    return partition(iovs, month_key)


def write_iovs(path, data):
    '''
    write IOVs data to files
    '''
    if not os.path.exists(path):
        os.mkdir(path)
    logging.debug('writing %s/IOVs', path)
    with open(os.path.join(path, 'IOVs'), 'w') as iovs:
        for entry in data:
            iovs.write('{0.since} {0.key}\n'.format(entry))
            if isinstance(entry, NestedEntry):
                write_iovs(os.path.join(path, entry.key), entry.entries)


def clean_iovs(repo, partitioning=True):
    logging.debug('reducing IOVs...')
    for iovs in iov_files(repo):
        path = os.path.dirname(iovs)
        logging.debug('processing %s', path)
        data = list(simplify(parse_iovs(path)))
        remove_iovs(path)  # at this point we should not need old files
        if partitioning:
            data = partition_by_month(data)
        write_iovs(path, data)


def add_iov(iovs, payload_key, since, until):
    '''
    take a flat IOVs list and override the payloads in the range [since, until)
    with the specified one in the given range
    '''
    from itertools import takewhile, dropwhile, chain
    if not isinstance(iovs, list):
        iovs = list(iovs)
    def last(iterable):
        for x in iterable:
            pass
        return x
    return chain(takewhile(lambda x: x.since < since, iovs),
                 [Entry(since, payload_key)],
                 [] if until == IOV_MAX else [
                     Entry(until,
                           last(takewhile(lambda x: x.since <= until, iovs))
                           .key)],
                 dropwhile(lambda x: x.since <= until, iovs))


def to_iov_key(s, default):
    '''
    Convert a string to an IOV key (nanoseconds).

    The string can be ISO-like (YYYY-MM-DD[_HH:MM[:SS.SSS]][UTC]) or an integer.
    '''
    # it may be an int in nanoseconds
    try:
        return int(s)
    except ValueError:
        pass

    # try to parse the ISO-like time
    m = TIME_STRING_RE.match(s)
    if m:
        # FIXME: check for validity ranges
        tm = tuple([int(n) if n else 0 for n in m.groups()[0:6]] + [0, 0, -1])
        import time
        if m.group('utc'):
            # the user specified UTC
            t = time.timegm(tm)
        else:
            # seconds since epoch UTC, from local time tuple
            t = time.mktime(tm)
        t = int(t) * 1000000000 # to ns
        d = m.group('decimal')
        if d:
            if len(d) < 9:
                # Add the missing 0s to the decimals
                d += '0'*(9-len(d))
            else:
                # truncate decimals
                d = d[:9]
            # add decimals to t
            t += int(d)
        return t
    return default


def overlap(iov1, iov2):
    '''
    Check if two IOVs ovelap.
    '''
    return max(iov1[0], iov2[0]) <= min(iov1[1], iov2[1])


def _get_iovs(tree, repository, path, tag, bounds, for_iov):
    '''
    Internal recursive function to extract all IOVs at a given path in a Git
    repository.
    '''
    if tree.get(path) is None:  # it's a file
        yield path, bounds
    else:
        from GitCondDB.GitAccess import gitOpen
        lines = (parse_iov_line(path, l)
                 for l in gitOpen(repository, path + '/IOVs', tag))
        since, payload = lines.next()
        for until, next_payload in lines:
            if overlap(for_iov, (since, until)):
                # recurse only if we have an overlap
                for entry in _get_iovs(tree, repository, payload, tag,
                                       (max(since, bounds[0]),
                                        min(until, bounds[1])), for_iov):
                    yield entry
            since, payload = until, next_payload
        # handle last line
        if overlap(for_iov, (since, IOV_MAX)):
            for entry in _get_iovs(tree, repository, payload, tag,
                                   (max(since, bounds[0]), bounds[1]), for_iov):
                yield entry

def get_iovs(repository, path, tag='HEAD', for_iov=(IOV_MIN, IOV_MAX),
             bounds=(IOV_MIN, IOV_MAX)):
    '''
    Return a generator over all the entries (payload and IOV) at a given path
    in a Git CondDB.
    '''
    from GitCondDB.GitAccess import Tree, listFiles
    tree = Tree(listFiles(repository, tag, path))
    for entry in _get_iovs(tree, repository, path, tag,
                           bounds, for_iov):
        yield entry
