'''
Utils for IOVs manipulations in Git CondDBs.
'''
import os
from datetime import datetime


IOV_MIN = 0
IOV_MAX = 0x7fffffffffffffff


def iov_files(path):
    '''
    Get list of IOVs files in a path (recusively).
    '''
    for root, dirs, files in os.walk(path):
        if 'IOVs' in files:
            dirs[:] = []  # do not recurse further
            yield os.path.join(root, 'IOVs')


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
        return (since,
                (key, parse_iovs(subpath))
                if os.path.exists(os.path.join(subpath, 'IOVs')) else key)

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


def flatten_iovs(iovs, nested=''):
    from os.path import join, normpath
    for since, key in iovs:
        if isinstance(key, basestring):
            yield (since, normpath(join(nested, key)))
        else:
            for item in flatten_iovs(key[1], join(nested, key[0])):
                yield item


def partition_iovs(data):
    '''
    partition IOVs per month
    '''
    from itertools import groupby
    from os.path import join

    def to_ts(d):
        return int((d - datetime.utcfromtimestamp(0).date()).total_seconds() * 1e9)

    def to_key(d):
        return d.strftime('%Y-%m')

    def from_key(key):
        return datetime.strptime(key, '%Y-%m').date()

    def month_key(since):
        if since == 0:
            return None
        return to_key(datetime.utcfromtimestamp(since * 1e-9).date())

    top_iovs = []
    nested_iovs = {}
    for month, entries in groupby(data, lambda d: month_key(d[0])):
        if month is None:
            top_iovs.extend(entries)
        else:
            nested_iovs[month] = []
            if top_iovs:
                if top_iovs[-1][0] == 0:
                    nested_iovs[month].append((0, top_iovs[-1][1]))
                else:
                    # new month should start from the last entry of the
                    # previous month
                    nested_iovs[month].append(nested_iovs[top_iovs[-1][1]][-1])
            nested_iovs[month].extend(entries)
            top_iovs.append((to_ts(from_key(month)), month))

    return [(since,
             (key, map(lambda e: (e[0], join('..', e[1])), nested_iovs[key]))
             if key in nested_iovs else key)
            for since, key in top_iovs]


def write_iovs(path, data):
    '''
    write IOVs data to files
    '''
    if not os.path.exists(path):
        os.mkdir(path)
    print '  writing %s/IOVs' % path
    with open(os.path.join(path, 'IOVs'), 'w') as iovs:
        for since, key in data:
            if not isinstance(key, basestring):
                key, subdata = key
                write_iovs(os.path.join(path, key), subdata)
            iovs.write('%d %s\n' % (since, key))


def remove_dummy_entries(data):
    # remove duplicated since values and fix order
    new_data = []
    for entry in sorted(dict(data).items()):
        if new_data and entry[1] == new_data[-1][1]:
            continue  # skip fake IOV boundaries
        new_data.append(entry)
    return new_data


def clean_iovs(repo, partition=True):
    print 'reducing IOVs...'
    for iovs in iov_files(repo):
        path = os.path.dirname(iovs)
        print '  processing', path
        data = remove_dummy_entries(flatten_iovs(parse_iovs(path)))
        remove_iovs(path)  # at this point we should not need old files
        if partition:
            data = partition_iovs(data)
        write_iovs(path, data)


def add_iov(iovs, payload_key, since, until):
    '''
    take a flat IOVs list and override the payloads in the range [since, until)
    with the specified one in the given range
    '''
    from itertools import takewhile, dropwhile, chain
    def last(iterable):
        for x in iterable:
            pass
        return x
    return chain(takewhile(lambda x: x[0] < since, iovs),
                 [(since, payload_key)],
                 [(until, last(takewhile(lambda x: x[0] <= until, iovs))[1])] if until != IOV_MAX else [],
                 dropwhile(lambda x: x[0] <= until, iovs))
