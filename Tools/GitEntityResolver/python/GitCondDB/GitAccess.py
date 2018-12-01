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
Helper functions to query a Git repository.
'''
import os
from os.path import join, isdir, relpath, abspath
from subprocess import check_output, PIPE, CalledProcessError

class Tree(object):
    '''
    Manage and query a hierarchy of entries.

    >>> t = Tree(['a/b', 'a/c/d'])
    >>> for r, d, f in t.walk():
    ...     print '{}: {} {}'.format(r, d, f)
    ...
    : ['a'] []
    a: ['c'] ['b']
    a/c: [] ['d']
    '''
    def __init__(self, iterable=None):
        '''
        Initialize a tree, possibly from a list of path to leaves.
        '''
        self._entries = {}
        if iterable:
            for item in iterable:
                self.add(item)
    def add(self, item, value=None):
        '''
        Add a new leaf and the subtrees up to it to the Tree,
        '''
        if '/' not in item:
            self._entries[item] = value  # it's a leaf
        else:
            head, tail = item.split('/', 1)
            if head not in self._entries:
                self._entries[head] = Tree()
            self._entries[head].add(tail, value)
    def get(self, item):
        '''
        Get the payload attached to a path. Either a Tree instance or None,
        unless a value was given to the call to Tree.add.
        '''
        if item == '':
            return self
        level = self
        for key in item.split('/'):
            level = level._entries[key]
        return level
    def remove(self, item):
        '''
        Remove an entry from the Tree.
        '''
        if '/' not in item:
            del self._entries[item]
        else:
            head, tail = item.split('/', 1)
            self._entries[head].remove(tail)
    def __contains__(self, item):
        '''
        Implementation of the "in" callback.
        '''
        try:
            self.get(item)
            return True
        except KeyError:
            return False
    def entries(self):
        '''
        Return the name of the first children of the Tree.
        '''
        return sorted(self._entries)
    def leaves(self):
        '''
        Return the name of the first non Tree children of the Tree.
        '''
        return sorted(e for e in self._entries
                      if not isinstance(self._entries[e], Tree))
    def walk(self, root=''):
        '''
        Iterate recursively over all the entries in the tree, similar to os.walk.
        '''
        entries = []
        leaves = []
        for e in self._entries:
            if isinstance(self._entries[e], Tree):
                entries.append(e)
            else:
                leaves.append(e)
        yield root, entries, leaves
        for e in entries:
            if root:
                nextroot = '/'.join((root, e))
            else:
                nextroot = e
            for i in self._entries[e].walk(nextroot):
                yield i

def _git(repository, cmd, *args, **kwargs):
    '''
    Helper to call the git command.
    '''
    kwargs['cwd'] = repository
    return check_output(['git'] + cmd, *args, **kwargs)


def listTags(repository, sort=False):
    '''
    List the tags in a Git repository.
    '''
    cmd = ['for-each-ref', '--python', '--format=%(refname:short),']
    if sort:
        cmd.append('--sort=-creatordate')
    cmd.append('refs/tags/')
    return eval('[{}]'.format(_git(repository, cmd)))


def _useFiles(repository, tag):
    '''
    Helper to ease the handling of the special empty tag to access files.
    '''
    # empty tag and checked out repository means access files, but 'HEAD' on a
    # bare repository
    def is_not_bare():
        return (isdir(join(repository, '.git')) or
                not (isdir(join(repository, 'object')) and
                     isdir(join(repository, 'refs'))))
    return (not tag or tag == '<files>') and is_not_bare()


def listFiles(repository, tag='HEAD', path='.'):
    '''
    Get the list of files in a Git repository.
    '''
    if _useFiles(repository, tag):
        path = abspath(join(repository, path))
        def all_files():
            'get all plain files in a directory (except .git content)'
            for root, dirs, files in os.walk(path):
                if '.git' in dirs:
                    dirs.remove('.git')
                for f in files:
                    yield relpath(join(root, f), repository)
        if not isdir(path):
            out = [relpath(path, repository)]
        else:
            out = list(all_files())
    else:
        out = _git(repository,
                   ['ls-tree', '-r', '--name-only', '-z', tag or 'HEAD',
                    '--', path]
                  ).rstrip('\0').split('\0')
    out.sort()
    return out


def isFile(repository, path, tag='HEAD'):
    '''
    True if path under repository points to a file.
    '''
    if _useFiles(repository, tag):
        os.path.isfile(join(repository, path))
    else:
        return listFiles(repository, tag or 'HEAD', path) == [path]


def gitOpen(repository, path, tag='HEAD'):
    '''
    Return the content of a file in a Git repository as a file-like object.
    '''
    if _useFiles(repository, tag):
        return open(join(repository, path))
    else:
        from StringIO import StringIO
        return StringIO(_git(repository,
                             ['cat-file', 'blob',
                              (tag or 'HEAD') + ':' + path]))


def rev_parse(repository, *args):
    '''
    Shortcut to git-rev-parse.
    '''
    try:
        return _git(repository, ['rev-parse'] + list(args), stderr=PIPE).strip()
    except CalledProcessError:
        return None
