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
The idea behind the CondDBUI module is to simplify the usage of PyCool for LHCb users.
This means that you will find here many functions which are specific to the way LHCb
is playing with its conditions database. This means as well that these functions are
not supposed to be efficient (and they are not). For efficient manipulations, we
advise you to use directly the PyCool classes and functions, or (even better) to use
the COOL C++ API.
Please note that most of the functions of the CondDBUI module are used in the conddb
browser.
'''

import os, random, sys, re, time, datetime
from multiprocessing.managers import BaseManager
from multiprocessing import Process, cpu_count

LOG_FORMAT = "%(levelname)s: (%(name)s) %(message)s"

ValidityKeyMin, ValidityKeyMax = 0, 0x7fffffffffffffff


def openDB(path, **kwargs):
    '''
    Factory to create a Git CondDB accessor.
    '''
    from CondDBUI import GitBridge
    return GitBridge.CondDB(path, **kwargs)


class Tag:
    '''
    Basic class allowing to manipulate more easily the tags in the tag hierarchy.
    The rule is that a tag has only one child tag and can have many parent tag.
    '''

    def __init__(self, tagName, nodePath):
        '''
        Create a new tag object.
        inputs:
            tagName:  string; name of the tag
            nodePath: string; path to the node which own this tag
        outputs:
            none
        '''
        self.name = tagName
        self.path = nodePath
        self.parents = []
        self.child = None

    def connectChild(self, child):
        '''
        Connect a child tag to the current tag, and update the parent list of the child.
        inputs:
            child: Tag object; the child tag object
        outputs:
            none
        '''
        self.child = child
        child.parents.append(self)

    def printAncestors(self, branche=''):
        '''
        Recursive function printing the relation between the tag and its ancestors.
        inputs:
            branche: string; current status of the ancestor branch. If other ancestors
                     exist, this value is updated. Otherwise, it is printed.
                     -> Default = ''
        outputs:
            none; results are sent to the standard output.
        '''
        s = branche + '/' + self.name
        if self.parents:
            for p in self.parents:
                p.printAncestors(s)
        else:
            print s
        return

    def getAncestorsBranches(self, currentBranche=[], brancheList=None):
        '''
        Recursive function returning the list of ancestor branchs of the tag.
        inputs:
            currentBranche: list of strings; stores the names of the ancestors
                            of the current branch.
                            -> Default = []
            brancheList:    list of lists of strings; variable storing the list
                            of completed ancestor branches.
                            -> Default = None
        outputs:
            brancheList: list of list of strings; each sublist contains a branch of
                         the tag "family".
        '''
        if brancheList == None:
            # Just make sure that brancheList is really a new object.
            brancheList = []

        b = currentBranche[:]
        b.append(self.name)
        if self.parents:
            for p in self.parents:
                p.getAncestorsBranches(b, brancheList)
        else:
            brancheList.append(b)
        return brancheList

    def getAncestors(self):
        '''
        Return the names of the ancestor tags.
        inputs:
            none
        outputs:
            ancestors: list of strings; the names of all the ancestor
                       tags of the current tag. This is equivalent to
                       a list of aliases for this tag.
        '''
        ancestors = []
        branches = self.getAncestorsBranches()
        for b in branches:
            for tagName in b:
                if tagName not in ancestors:
                    ancestors.append(tagName)
        if self.name in ancestors:
            ancestors.remove(self.name)
        return ancestors

    def getAncestorTagsDict(self, currentBranche={}, brancheList=None):
        '''
        Recursive function returning the list of ancestor tags dictionaries.
        inputs:
            currentBranche: dictionary of tags; stores the the ancestors tags,
                            referenced by names, for the current branch.
                            -> Default = {}
            brancheList:    list of dictionaries; variable storing the list
                            of completed ancestor branches.
                            -> Default = None
        outputs:
            brancheList: list of dictionaries; each sublist contains a branch of
                         the tag "family".
        '''
        if brancheList == None:
            # Just make sure that brancheList is really a new object.
            brancheList = []

        b = currentBranche.copy()
        b[self.name] = self
        if self.parents:
            for p in self.parents:
                p.getAncestorTagsDict(b, brancheList)
        else:
            brancheList.append(b)
        return brancheList

    def getAncestorTags(self):
        '''
        Return the ancestor tags as a list of tag objects.
        inputs:
            none
        outputs:
            ancestors: list of tags; all the ancestor tags of the
                       current tag.
        '''
        ancestors = []
        branches = self.getAncestorTagsDict()
        for b in branches:
            for tagName in b.keys():
                if b[tagName] not in ancestors:
                    ancestors.append(b[tagName])
        if self in ancestors:
            ancestors.remove(self)
        return ancestors

    def __str__(self):
        '''
        Standard string conversion. Returns the name of the tag
        '''
        return self.name

    def __repr__(self):
        '''
        Standard object representation. Returns a string representation of all
        the object's attributes, as well as its relations with its ancestors.
        '''
        output = ' name = %s\n path = %s\n child = %s\n' % (
            self.name, self.path, str(self.child))

        # display the parents
        if self.parents:
            output += ' parents = '
            for p in self.parents:
                output += '%s, ' % str(p)
            # Remove the last comma and space. I don't like this,
            # but adding '\b\b' doesn't work.
            output = output[:-2] + '\n'
        else:
            output += ' parents = []\n'

        # display all the ancestors
        output += ' ancestors = %s\n' % str(self.getAncestors())

        # display the relations with ancestors
        output += ' relations = %s\n' % str(self.getAncestorsBranches())

        return output


#########################################################################################
#                                Utility functions                                      #
#########################################################################################
