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
# -*- coding: utf-8 -*-
'''
Prepare the Git repository to test GitEntityResolver.
'''

import sys
import os
from datetime import datetime
from subprocess import check_call, check_output, STDOUT
from os.path import join, isdir, dirname, exists
from shutil import copytree, rmtree, copy

EPOCH = datetime(1970, 1, 1)


def echo_cmd(func):
    def wrap(*args, **kwargs):
        print args, kwargs
        return func(*args, **kwargs)
    return wrap


check_call = echo_cmd(check_call)
check_output = echo_cmd(check_output)


def makedirs(*args):
    '''
    Create requested directories, if needed.
    '''
    for path in args:
        if not isdir(path):
            os.makedirs(path)


def to_ts(dt):
    return int((dt - EPOCH).total_seconds() * 1e9)


def write_IOVs(iovs, path):
    '''
    write a list of (datetime, key) pairs to the IOVs file in path.
    '''
    with open(join(path, 'IOVs'), 'w') as IOVs:
        IOVs.write(''.join('{0} {1}\n'.format(to_ts(dt), key)
                           for dt, key in iovs))


# first argument is destination directory
path = sys.argv[1] if len(sys.argv) > 1 else os.environ['GIT_TEST_REPOSITORY']

if exists(path):
    rmtree(path)

# initialize repository from template (tag 'v0')
src_data = join(dirname(__file__), 'data', 'test_repo')
copytree(src_data, path)

check_call(['git', 'init', path])
check_call(['git', 'config', '-f', '.git/config', 'user.name', 'Test User'], cwd=path)
check_call(['git', 'config', '-f', '.git/config', 'user.email', 'test.user@no.where'],
           cwd=path)
check_call(['git', 'add', '.'], cwd=path)
env = dict(os.environ)
env['GIT_COMMITTER_DATE'] = env['GIT_AUTHOR_DATE'] = '1483225200'
check_call(['git', 'commit', '-m', 'initial version'], cwd=path, env=env)
check_call(['git', 'tag', 'v0'], cwd=path)

# change values for tag 'v1'
with open(join(src_data, 'values.xml')) as in_file:
    with open(join(path, 'values.xml'), 'w') as out_file:
        out_file.write(in_file.read().replace('42', '2016'))

# modify changing.xml dir to use partitioning
dirInit = join(path, 'changing.xml', 'initial')
dir2016 = join(path, 'changing.xml', '2016')
dir2017 = join(path, 'changing.xml', '2017')

makedirs(dirInit, dir2016, dir2017)
write_IOVs([(EPOCH, 'initial'),
            (datetime(2016, 1, 1), 2016),
            (datetime(2017, 1, 1), 2017)],
           join(path, 'changing.xml'))

copy(join(path, 'changing.xml', 'v0.xml'),
     join(dirInit, 'v0'))
write_IOVs([(EPOCH, 'v0')], dirInit)

copy(join(path, 'changing.xml', 'v1.xml'),
     join(dir2016, 'v1'))
write_IOVs([(EPOCH, '../initial/v0'),
            (datetime(2016, 7, 1), 'v1')], dir2016)

write_IOVs([(EPOCH, '../2016/v1')], dir2017)

os.remove(join(path, 'changing.xml', 'v0.xml'))
os.remove(join(path, 'changing.xml', 'v1.xml'))

check_call(['git', 'add', '--all', 'changing.xml'], cwd=path)
check_call(['git', 'commit', '-am', 'v1 data'], cwd=path)
check_call(['git', 'tag', 'v1'], cwd=path)

# changes for HEAD version (no tag)
with open(join(src_data, 'values.xml')) as in_file:
    with open(join(path, 'values.xml'), 'w') as out_file:
        out_file.write(in_file.read().replace('42', '0'))
check_call(['git', 'commit', '-am', 'new data'], cwd=path)

# changes for local files
with open(join(src_data, 'values.xml')) as in_file:
    with open(join(path, 'values.xml'), 'w') as out_file:
        out_file.write(in_file.read().replace('42', '-123'))

# make a "bare" clone of the repository
if exists(path + '-bare.git'):
    rmtree(path + '-bare.git')
print check_output(['git', 'clone', '--mirror', path, path + '-bare.git'],
                   stderr=STDOUT)

# prepare an overlay directory
if exists(path + '-overlay'):
    rmtree(path + '-overlay')
os.makedirs(path + '-overlay')
with open(join(src_data, 'values.xml')) as in_file:
    with open(join(path + '-overlay', 'values.xml'), 'w') as out_file:
        out_file.write(in_file.read().replace('42', '777'))
print check_output(['git', 'init'], cwd=path + '-overlay',
                   stderr=STDOUT)
