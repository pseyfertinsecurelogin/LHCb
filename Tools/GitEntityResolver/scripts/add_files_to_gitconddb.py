#!/usr/bin/env python
'''
Script to add a set of files to Git CondDB (for an optional IOV).
'''
import os
import shutil
import logging
from GitCondDB.IOVs import (IOV_MIN, IOV_MAX,
                            parse_iovs, flatten_iovs, remove_dummy_entries,
                            add_iov, remove_iovs, write_iovs, partition_iovs)
from GitCondDB.Payload import fix_system_refs, fix_lines_ends, payload_filename


def _write_file(path, data):
    'helper to write files'
    dirname = os.path.dirname(path)
    if not os.path.exists(dirname):
        logging.debug('creating directory %s', dirname)
        os.makedirs(dirname)
    logging.debug('writing %s', path)
    with open(path, 'wb') as f:
        f.write(data)


def _add_file_no_iov(data, dest):
    'helper for a file covering the whole timespan'
    # wipe out the destination
    if os.path.isdir(dest):
        logging.debug('remove tree %s', dest)
        shutil.rmtree(dest)
    elif os.path.exists(dest):
        logging.debug('remove file %s', dest)
        os.remove(dest)
    _write_file(dest, data)

def _add_file_with_iov(data, dest, iov):
    'helper for a file with a limited validity'
    if not os.path.exists(dest):
        logging.warning('limited IOV on new entries not allowed, use full IOV '
                        'for %s', dest)
        _add_file_no_iov(data, dest)
    elif not os.path.isdir(dest):
        # the existing entry covers whole timespan
        with open(dest) as f:
            old_data = f.read()
        if old_data == data:
            logging.warning('same data: not changing %s', dest)
        else:
            logging.debug('remove file %s', dest)
            os.remove(dest)
            logging.debug('create directory %s', dest)
            os.makedirs(dest)
            old_payload_name = payload_filename(old_data)
            _write_file(os.path.join(dest, old_payload_name), old_data)
            new_payload_name = payload_filename(data)
            _write_file(os.path.join(dest, new_payload_name), data)
            _write_file(os.path.join(dest, 'IOVs'), '\n'.join(
                '{0} {1}'.format(since, payload)
                for since, payload in [(IOV_MIN, old_payload_name),
                                       (iov[0], new_payload_name),
                                       (iov[1], old_payload_name)]
                if since < IOV_MAX
            ) + '\n')
    else:
        orig_iovs = parse_iovs(dest)
        key = payload_filename(data)

        iovs = list(add_iov(remove_dummy_entries(flatten_iovs(orig_iovs)),
                            key, *iov))
        referenced_payloads = set([p for _, p in iovs])

        iovs = partition_iovs(iovs)

        if iovs == orig_iovs:
            logging.warning('no change needed for %s', dest)
        else:
            data_file_name = os.path.join(dest, key)
            if os.path.exists(data_file_name):
                # just to check
                if open(data_file_name).read() != data:
                    logging.error('hash clash on %s', data_file_name)
            else:
                _write_file(data_file_name, data)
            remove_iovs(dest)
            for f in set(os.listdir(dest)) - referenced_payloads:
                logging.debug('remove unreferenced payload %s',
                              os.path.join(dest, f))
                os.remove(os.path.join(dest, f))
            write_iovs(dest, iovs)

def git_conddb_add_file(source, dest, root, since=IOV_MIN, until=IOV_MAX):
    '''
    copy a file to a Git CondDB location for the given IOV
    '''
    logging.debug('adding %s', os.path.relpath(source, root))
    data = fix_lines_ends(fix_system_refs(open(source).read(), root, dest))
    iov = (since, until)
    if iov == (IOV_MIN, IOV_MAX):
        _add_file_no_iov(data, dest)
    else:
        _add_file_with_iov(data, dest, iov)


def git_conddb_extend(source, dest, since=IOV_MIN, until=IOV_MAX):
    logging.debug('adding %s to %s, from %d to %d', source, dest, since, until)
    for root, _, filenames in os.walk(source):
        for f in filenames:
            src_file = os.path.join(root, f)
            dst_file = os.path.join(dest, os.path.relpath(src_file, source))
            git_conddb_add_file(src_file, dst_file, source, since, until)


def main():
    from optparse import OptionParser
    parser = OptionParser(usage='%prog [options] source destination')
    parser.add_option('--since',
                      help='start of validity for the files')
    parser.add_option('--until',
                      help='end of validity for the files')

    parser.add_option('--quiet',
                      action='store_const', const=logging.WARNING,
                      dest='log_level',
                      help='minimal output messages [default]')
    parser.add_option('--verbose',
                      action='store_const', const=logging.INFO,
                      dest='log_level',
                      help='print more details')
    parser.add_option('--debug',
                      action='store_const', const=logging.DEBUG,
                      dest='log_level',
                      help='debug printout')

    parser.set_defaults(since=IOV_MIN,
                        until=IOV_MAX,
                        log_level=logging.WARNING)

    opts, args = parser.parse_args()
    logging.basicConfig(level=opts.log_level)

    if len(args) == 2:
        source, dest = args
    else:
        parser.error('wrong number of arguments')

    git_conddb_extend(source, dest, int(opts.since), int(opts.until))


if __name__ == '__main__':
    main()
