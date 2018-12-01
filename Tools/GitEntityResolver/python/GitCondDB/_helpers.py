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
import os
import logging
import shutil
from GitCondDB.IOVs import (IOV_MIN, IOV_MAX,
                            parse_iovs, simplify,
                            add_iov, remove_iovs, write_iovs,
                            partition_by_month)
from GitCondDB.Payload import payload_filename


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


def _add_file(data, dest, iov):
    'helper for a file with a limited validity'
    if iov == (IOV_MIN, IOV_MAX):
        return _add_file_no_iov(data, dest)
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
            data = []
            if iov[0] > IOV_MIN:
                data.append((IOV_MIN, old_payload_name))
            data.append((iov[0], new_payload_name))
            if iov[1] < IOV_MAX:
                data.append((iov[1], old_payload_name))
            _write_file(os.path.join(dest, 'IOVs'), '\n'.join(
                '{0} {1}'.format(since, payload)
                for since, payload in data
            ) + '\n')
    else:
        orig_iovs = parse_iovs(dest)
        key = payload_filename(data)

        iovs = list(add_iov(simplify(orig_iovs), key, *iov))
        referenced_payloads = set([e.key for e in iovs])

        iovs = partition_by_month(iovs)

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
