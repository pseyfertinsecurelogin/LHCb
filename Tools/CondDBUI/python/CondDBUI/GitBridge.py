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
Helper to access Git-based CondDB via old interfaces for CondDBUI CondDB
'''
import os
import GitCondDB
from subprocess import check_output
from CondDBUI import Tag
from GitCondDB.GitAccess import Tree


class COOLNodeFacade(object):
    '''
    Helper class for the cases where we had to use directly COOL Folder or
    FolderSet instances rather than the CondDB accessor.
    '''

    def __init__(self, db, path):
        '''
        Initialize a node from a CondDB and a path.
        '''
        self.db = db
        self.path = path

    def listFolders(self):
        '''
        Get list of sub-folders of a node.
        '''
        for n in self.db._tree.get(self.path).leaves():
            yield '/'.join(['', self.path, n])

    def listFolderSets(self):
        '''
        Get list of sub-foldersets of a node.
        '''
        subtree = self.db._tree.get(self.path)
        leaves = set(subtree.leaves())
        for n in [n for n in subtree.entries() if n not in leaves]:
            yield '/'.join(['', self.path, n])

    def isLeaf(self):
        '''
        True for leaf nodes, False otherwise.
        '''
        return self.db._tree.get(self.path) is None

    def versioningMode(self):
        '''
        On Git CondDB everything is MULTI_VERSION.
        '''
        return 1  # cool.FolderVersioning.MULTI_VERSION

    def fullPath(self):
        '''
        Full path to the current node (with leading '/' to match COOL
        identifiers).
        '''
        return '/' + self.path


class CondDB(object):
    '''
    Object allowing to manipulate a COOL database object in an LHCb way.
    This object contains a functions to open or create a database. It can then be
    manipulated either directly through the attribute 'db', or via a set of
    functions simplifying some operations, like creation and deletion of nodes, storage
    and retrieval of XML strings, etc.
    '''

    def __init__(self,
                 connectionString='',
                 create_new_db=False,
                 defaultTag=None,
                 readOnly=True):
        """
        Establishes the connection to the COOL database and store the object.
        inputs:
            connectionString: string; path to a Git repository (non-bare if readOnly==False).
                              -> Default = ''
            create_new_db:    boolean; tells the constructor what to do if the connectionString
                              doesn't open an existing database.
                              -> Default = False
            defaultTag:       string; tag which will be used by default if no other is precised.
                              -> Default = 'HEAD'
            readOnly:         boolean; open the conddb in read only mode if True, or read/write
                              mode if False.
                              -> Default = True (or False if the database is created)
        outputs:
            none
        """
        self._path = None
        self._is_bare = True
        self.connectionString = os.path.expandvars(connectionString)
        self.readOnly = True
        self._folders = self._foldersets = None
        if self.connectionString == '':
            print "CONDDBUI: WARNING: no database opened"
        else:
            self.openDatabase(self.connectionString, create_new_db,
                              self.readOnly)
        if defaultTag is None:
            self.defaultTag = 'HEAD' if self._is_bare else '<files>'

    ## Return representation string for the CondDB instance.
    def __repr__(self):
        rep_str = self.__class__.__name__ + "(%r" % self.connectionString
        if self.defaultTag != "HEAD":
            rep_str += ", defaultTag=%r" % self.defaultTag
        if not self.readOnly:
            rep_str += ", readOnly=False"
        return rep_str + ")"

    #---------------------------------------------------------------------------------#

    #=================#
    # Database Access #
    #=================#

    def openDatabase(self,
                     connectionString,
                     create_new_db=False,
                     readOnly=True):
        '''
        Closes the current database and connects to a new one. Creates it if asked to.
        inputs:
            connectionString: string; standard COOL connection string.
            create_new_db:    boolean; if True, creates a new database on failure to
                              connect.
                              -> Default = False
            readOnly:         boolean; open the conddb in read only mode if True, or read/write
                              mode if False.
                              -> Default = True
        outputs:
            none
        '''
        self.connectionString = connectionString
        self.readOnly = True
        self._path = None
        self._tree_ = None
        self._is_bare = True

        # check if we have a non-bare repository
        if os.path.isdir(os.path.join(self.connectionString, '.git')):
            self._path = self.connectionString
            self._is_bare = False
        else:
            # non bare, so try for bare if readOnly
            if (self.readOnly and os.path.isdir(
                    os.path.join(self.connectionString, 'objects'))):
                self._path = self.connectionString
                self._is_bare = True
        if self._path is None:
            if create_new_db:
                self.createDatabase(self.connectionString)
            else:
                raise RuntimeError('Database not found: cannot open '
                                   'Git CondDB at %s' % self.connectionString)

    def closeDatabase(self):
        '''
        Close the connection to the opened database
        inputs:
            none
        outputs:
            none
        '''
        self._path = None

    def createDatabase(self, connectionString):
        '''
        Create a new database and connect to it.
        inputs:
            connectionString: string; standard COOL connection string.
        outputs:
            none
        '''
        raise NotImplementedError()

    #---------------------------------------------------------------------------------#

    #=============================#
    # Database Reading Operations #
    #=============================#

    def isSingleVersionFolder(self, path):
        '''
        Check if path corresponds to a single version folder
        inputs:
            path: string; path to the node to check
        outputs:
            boolean; True if the node is a single version folder, False in all other cases
           (i.e. if the node is a multi version folder OR if it is a folderset or does not
           exist).
        '''
        # in Git all folders are MultiVersion
        return False

    def isMultiVersionFolder(self, path):
        '''
        Check if path corresponds to a multi version folder
        inputs:
            path: string; path to the node to check
        outputs:
            boolean; True if the node is a multi version folder, False in all other cases
           (i.e. if the node is a single version folder OR if it is a folderset or does not
           exist).
        '''
        # in Git all folders are MultiVersion
        return True

    def setDefaultTag(self, tagName):
        '''
        Set the value of the default tag.
        inputs:
            tagName: string; the name of the default tag.
        outputs:
            none
        '''
        self.defaultTag = tagName
        self._tree_ = None

    def getFolderStorageKeys(self, path):
        '''
        Retrieve the keys of the Record Specification for the given folder.
        inputs:
            path: string; path to the folder.
        outputs:
            list of strings; the list of storage keys.
        '''
        return ['data']

    def getPayload(self, path, when, channelID=0, tag=''):
        '''
        Retrieve the payload of the condition object valid at a given time.
        inputs:
            path:      string; path to the condition data in the database.
            when:      integer; time stamp (most likely an event time) at which the
                       value of the condition is requested.
            channelID: integer; ID of the channel in which the condition data are
                       stored.
                       -> Default = 0
            tag:       string; name of the version. If empty, defaultTag is used.
                       -> Default = ''
        outputs:
            dictionary; the contents of the attribute list
        '''
        assert self._path, 'No database connected!'
        return {
            'data':
            GitCondDB.Payload.get_payload(self._path, path.lstrip('/'), when,
                                          tag or self.defaultTag)[0]
        }

    def getXMLString(self, path, when, channelID=0, tag='', payloadKey='data'):
        '''
        Retrieve the XML string of the condition object valid at a given time.
        inputs:
            path:      string; path to the condition data in the database.
            when:      integer; time stamp (most likely an event time) at which the
                       value of the condition is requested.
            channelID: integer; ID of the channel in which the condition data are
                       stored.
                       -> Default = 0
            tag:       string; name of the version. If empty, defaultTag is used.
                       -> Default = ''
            payloadKey: string; key of the Record element we want to retrieve.
                        -> Default = 'data'
        outputs:
            string; the contents of the condition data.
        '''
        assert self.db != None, "No database connected !"
        try:
            payload = self.getPayload(path, when, channelID, tag)
            xmlString = str(payload[payloadKey])
        except Exception, details:
            raise Exception, details
        else:
            return xmlString

    def getPayloadList(self, path, fromTime, toTime, channelID=0, tag=''):
        '''
        Retrieve the payload of the condition objects valid during a given time interval.
        inputs:
            path:       string; path to the condition data in the database.
            fromTime:   integer; lower bound of the studied time interval.
            toTime:     integer; upper bound of the studied time interval. Note that an object
                        with start of validity equal to this upper bound value will be returned
                        as well.
            channelIDs: integer; IDs of the channel in which the condition data are
                        stored. If None is given instead, all channels will be browsed.
                        -> Default = 0
            tag:        string; name of the version. If empty, defaultTag is used.
                        -> Default = ''
        outputs:
            list of [dictionary, integer, integer, integer, integer]; the dictionary is the payload.
            The first two integers are the since and until values of the interval of validity. The
            third integer is the channel ID, and the last integer is the insertion time.
        '''
        payloads = GitCondDB.Payload.get_payloads(self._path, path.lstrip('/'),
                                                  fromTime, toTime, tag
                                                  or self.defaultTag)
        return [[{
            'data': payload
        }, since, until, 0, None] for payload, (since, until) in payloads]

    @property
    def _tree(self):
        if not self._tree_:
            self._tree_ = Tree(
                GitCondDB.GitAccess.listFiles(self._path, self.defaultTag))
            # prune subtrees with IOVs
            with_iovs = []
            for root, dirs, files in self._tree_.walk():
                if 'IOVs' in files:
                    with_iovs.append(root)
                    dirs[:] = []
            for item in with_iovs:
                self._tree_.remove(item)
                self._tree_.add(item)

        return self._tree_

    def getAllNodes(self):
        '''
        Convenience function: returns all the nodes of the database.
        inputs:
            none
        outputs:
            list of strings; the paths of all the nodes of the database
        '''
        assert self._path, 'No database connected!'
        res = ['/']
        for root, _, files in self._tree.walk():
            if root:
                root = '/' + root
                res.append(root)
            res.extend('/'.join([root, f]) for f in files)
        return res

    def getCOOLNode(self, node):
        """
        Get the COOL node at path 'node'.
        This functions checks if the node is a folderset or a folder
        before getting it.
        """
        node = node.lstrip('/')
        self._tree.get(node)
        return COOLNodeFacade(self, node)

    def getTagList(self, path):
        '''
        Return all the tag objects defined for the given node.
        inputs:
            path: string; path to the leaf node
        outputs:
            tagList: list of Tag; the list of Tag objects defined for this node.
                     They contains links to their parent Tag objects.
        '''
        assert self._path, "No database connected!"
        from collections import OrderedDict
        git_path = path.lstrip('/')

        def get_id(tag):
            'get blob id for a file in a tag'
            from GitCondDB.GitAccess import rev_parse
            return rev_parse(self._path, '{}:{}'.format(tag, git_path))

        local_tags = OrderedDict()
        for tag in ['HEAD'] + GitCondDB.GitAccess.listTags(self._path, True):
            blob_id = get_id(tag)
            if blob_id in local_tags:
                local_tags[get_id(tag)].append(tag)
            else:
                local_tags[get_id(tag)] = [tag]
        # prune tags not including the path
        if None in local_tags:
            del local_tags[None]

        def make_tag(tag, parents):
            'make a Tag instance with the parent tags'
            res = Tag('_auto_' + tag, path)
            res.parents = [Tag(p, '/') for p in parents]
            return res

        tags = [make_tag(tag, parents) for tag, parents in local_tags.items()]
        if not self._is_bare:
            tags.insert(0, Tag('<files>', path))
        return tags

    def resolveTag(self, path, tag, doraise=True):
        """
        Return the local tag associated with the parent tag given.
        """
        # we have only global tags in Git
        return tag

    def existsFolder(self, path):
        return path in self.getAllNodes() and self._tree.get(
            path.lstrip('/')) is None

    def existsFolderSet(self, path):
        return path in self.getAllNodes() and self._tree.get(
            path.lstrip('/')) is not None

    def getChannelsWithNames(self, path):
        return {0: ''}
