#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Prepare the Git repository to test GitEntityResolver.
'''

import sys
import os
from subprocess import check_call, check_output, STDOUT
from os.path import join, isdir, dirname, exists
from shutil import copytree, rmtree


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
        if not isdir(name):
            os.makedirs(path)

# first argument is destination directory
path = sys.argv[1] if len(sys.argv) > 1 else os.environ['GIT_TEST_REPOSITORY']

if exists(path):
    rmtree(path)

src_data = join(dirname(__file__), 'data', 'test_repo')
copytree(src_data, path)

check_call(['git', 'init', path])
check_call(['git', 'add', '.'], cwd=path)
check_call(['git', 'commit', '-m', 'initial version'], cwd=path)
check_call(['git', 'tag', 'v0'], cwd=path)

with open(join(src_data, 'values.xml')) as in_file:
    with open(join(path, 'values.xml'), 'w') as out_file:
        out_file.write(in_file.read().replace('42', '2016'))
check_call(['git', 'commit', '-am', 'v1 data'], cwd=path)
check_call(['git', 'tag', 'v1'], cwd=path)

with open(join(src_data, 'values.xml')) as in_file:
    with open(join(path, 'values.xml'), 'w') as out_file:
        out_file.write(in_file.read().replace('42', '0'))
check_call(['git', 'commit', '-am', 'new data'], cwd=path)

with open(join(src_data, 'values.xml')) as in_file:
    with open(join(path, 'values.xml'), 'w') as out_file:
        out_file.write(in_file.read().replace('42', '-123'))

if exists(path + '-bare.git'):
    rmtree(path + '-bare.git')
print check_output(['git', 'clone', '--mirror', path, path + '-bare.git'], stderr=STDOUT)
