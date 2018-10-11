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
Helper to access Git-based CondDB via old interfaces for COOL CondDB
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
        self.readOnly = readOnly
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
        self.readOnly = readOnly
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
        check_output(['git', 'init', connectionString])
        self.connectionString = connectionString
        self._path = self.connectionString
        self._is_bare = False
        self._tree_ = None
        self.readOnly = False

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

    @staticmethod
    def storeObject(folder, fro, unt, payl, chan):
        raise NotImplementedError()

    @staticmethod
    def payload(o):
        raise NotImplementedError()
        import CondDBCompression
        from PyCool import cool
        _payl = o.payload()
        #        if (payl and 'data' in payl.keys()):
        if (_payl):
            payl = cool.Record(_payl.specification())
            for key in _payl:
                xmlString = _payl[key]
                if (len(xmlString) and type(xmlString) is str):
                    payl[key] = CondDBCompression.decompress(xmlString)
        return payl

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

    def getXMLStringList(self,
                         path,
                         fromTime,
                         toTime,
                         channelID=0,
                         tag='',
                         payloadKey='data'):
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
            payloadKey: string; key of the record element we want to retrieve.
                        -> Default = 'data'
        outputs:
            list of [string, integer, integer, integer, integer]; the string is the payload.
            The first two integers are the since and until values of the interval of validity.
            The third integer is the channel ID, and the last integer is the insertion time.
        '''
        raise NotImplementedError()
        assert self.db != None, "No database connected !"
        try:
            objList = self.getPayloadList(path, fromTime, toTime, channelID,
                                          tag)
            for i in range(len(objList)):
                objList[i][0] = objList[i][0][payloadKey]
        except Exception, details:
            raise Exception, details
        else:
            return objList

    def getChildNodes(self, path):
        '''
        Return a list of the children of the given node.
        inputs:
            path: string; path of the parent node. Must be a folderset.
        outputs:
            list of strings; the paths of the child nodes.
        '''
        raise NotImplementedError()
        assert self.db != None, "No database connected !"
        if self.db.existsFolderSet(path):
            folderSet = self.db.getFolderSet(path)
            nodeList = list(folderSet.listFolders()) + list(
                folderSet.listFolderSets())
            nodeList.sort()
            return nodeList
        else:
            raise Exception, "FolderSet %s not found" % path

    def getAllChildNodes(self, path):
        '''
        Return all the nodes of the tree lying under "path"
        inputs:
            path: string; path of the parent node. Must be a folderset.
        outpus:
            list of strings; the paths of all the elements of the tree under the
            given node.
        '''
        raise NotImplementedError()
        assert self.db != None, "No database connected !"
        if self.db.existsFolderSet(path):
            folderSet = self.db.getFolderSet(path)
            # Get the lists of folders and foldersets
            folderList = list(folderSet.listFolders())
            folderSetList = list(folderSet.listFolderSets())
            # Merge the lists
            nodeList = folderList + folderSetList
            # Start recursion
            for name in folderSetList:
                nodeList += self.getAllChildNodes(name)
            # Sort the list for easier reading, and return it
            nodeList.sort()
            return nodeList
        else:
            raise Exception, "FolderSet %s not found" % path

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

    def payloadToMd5(self, path='/', tag='', initialMd5Sum=None):
        '''
        Computes the md5 sum for the payload stored under the given node.
        inputs:
            path:   string; path to the top of the database subtree to check.
                    -> Default = '/'
            tag:    string; version of the data to check. If set to '', defaultTag
                    is used. If set to 'ALL', will check all the tags associated
                    to this node (NOT YET IMPLEMENTED !!)
                    -> Default = ''
            md5Sum: md5 object; starting point for the check. If none is given, a
                    new one is created.
                    -> Default = None
        outputs:
            md5 object; result from the md5 check sum.
        '''

        raise NotImplementedError()
        import hashlib
        return payloadToHash(hashlib.md5(), path, tag)

    def payloadToHash(self, initialHashSum=None, path='/', tag=''):
        '''
        Computes the hash sum with chosen algorithm for the payload(s) stored under the given node.
        inputs:
            HashSum: hash object; starting point for the check. If none is given, a
                     new one is created using md5 hashing algorithm.
                     -> Default = None
            path:    string; path to the top of the database subtree to check.
                     -> Default = '/'
            tag:     string; version of the data to check. If set to '', defaultTag
                     is used.
                     -> Default = ''
                     @todo: If set to 'ALL', will check all the tags associated
                     to this node (NOT YET IMPLEMENTED !!)

        outputs:
            hash object; result from the hash check sum.
        '''
        raise NotImplementedError()

        import logging

        _log = logging.getLogger("CondDBUI.CondDB.payloadToHash")
        _log.setLevel(logging.INFO)

        from PyCool import cool
        assert self.db != None, "No database connected !"

        if tag == 'ALL':
            raise Exception, "Hash checking over all tags is not yet implemented"
        elif tag == '':
            tag = self.defaultTag

        # retrieve the list of nodes to check
        _log.info("Building nodes list to hash ...")
        if self.db.existsFolderSet(path):
            nodes = self.getAllChildNodes(path)
            nodesToCheck = [n for n in nodes if self.resolveTag(n, tag)]
        else:
            nodesToCheck = [path]

        if initialHashSum:
            hashSumObj = initialHashSum.copy()
        else:
            import hashlib
            hashSumObj = hashlib.sha1()

        _log.info("Processing nodes ...")
        for nodeName in nodesToCheck:
            # The check is done only on payload, i.e. we use only the folders,
            # not the foldersets.
            if self.db.existsFolder(nodeName):
                _log.debug("\t" + nodeName)
                folder = self.db.getFolder(nodeName)
                payload = cool.Record(folder.payloadSpecification())
                # Separate the case of single version (no need for tag) and multi
                # version folders.
                if folder.versioningMode() == cool.FolderVersioning.MULTI_VERSION \
                and tag.upper() != 'HEAD':
                    try:
                        objIter = folder.browseObjects(cool.ValidityKeyMin,
                                                       cool.ValidityKeyMax,
                                                       cool.ChannelSelection(),
                                                       self.resolveTag(
                                                           folder, tag))
                    except Exception, details:
                        raise Exception, details
                else:
                    try:
                        objIter = folder.browseObjects(cool.ValidityKeyMin,
                                                       cool.ValidityKeyMax,
                                                       cool.ChannelSelection())
                    except Exception, details:
                        raise Exception, details
                # Fill the hash checksum
                _log.debug("\tHashing '%s' ..." % nodeName)
                for obj in objIter:
                    for k in payload.keys():
                        hashSumObj.update((self.payload(obj))[k])
        return hashSumObj

    #---------------------------------------------------------------------------------#

    #====================#
    # Tagging Operations #
    #====================#

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

    def createTagRelation(self, path, parentTag, tag):
        '''
        Create a relation between the tag of the given node and a tag
        of its parent node.
        inputs:
            path:      string; path of the node
            parentTag: string; a tag associated to the parent node.
            tag:       string; the tag which we want to relate to the parent tag.
        outputs:
            none
        '''
        raise NotImplementedError()
        from PyCool import cool
        from .Helpers import Helpers

        assert self.db != None, "No database connected !"
        assert not self.readOnly, "The database is in Read Only mode."
        if type(path) is str:
            if self.db.existsFolder(path):
                node = self.db.getFolder(path)
                if node.versioningMode(
                ) == cool.FolderVersioning.SINGLE_VERSION:
                    raise Exception, "Folder %s is Single Version" % path
            elif self.db.existsFolderSet(path):
                node = self.db.getFolderSet(path)
            else:
                raise Exception, "Node %s was not found" % path
        else:
            node = path
        if not Helpers.createTagRelation(node, parentTag, tag):
            raise Exception, "cool::ReservedHeadTag"

    def deleteTagRelation(self, path, parentTag):
        '''
        Delete a relation between the tag of the given node and a tag
        of its parent node.
        inputs:
            path:      string; path of the node
            parentTag: string; the tag we no longer want to be related to.
        outputs:
            none
        '''
        raise NotImplementedError()
        from .Helpers import Helpers

        assert self.db != None, "No database connected !"
        assert not self.readOnly, "The database is in Read Only mode."
        if type(path) is str:
            if self.db.existsFolder(path):
                node = self.db.getFolder(path)
            elif self.db.existsFolderSet(path):
                node = self.db.getFolderSet(path)
            else:
                raise Exception, "Node %s was not found" % path
        else:
            node = path
        if not Helpers.deleteTagRelation(node, parentTag):
            raise Exception, "cool::ReservedHeadTag"

    def generateUniqueTagName(self, baseName, reservedNames=[]):
        """
        Generate a random tag name based on a given one.
        inputs:
            baseName:       string; idealy, this is the "parent tag" name. If this name
                            is an automatically generated one (i.e. starting with '_auto_'
                            and finishing with '-' and 6 alphanumeric characters), the
                            function will automatically strip the name from its random
                            parts.
            reservedNames:  list of strings; list of name that can't be chosen.
                            -> Default = []
        outputs:
            string; the generated tag name. Its format is:
            '_auto_' + baseName + '-' + 6 random alphanumeric characters.
        """
        raise NotImplementedError()
        assert self.db != None, "No database connected !"
        # Create the list of ASCII codes for alpha numeric characters
        alphaNumList = range(0x30, 0x3a) + range(0x41, 0x5b) + range(
            0x61, 0x7b)
        tagName = ''
        while tagName == '' or tagName in reservedNames or self.db.existsTag(
                tagName):
            tagName = '_auto_' + baseName + '-'
            for i in range(6):
                # Append the 6 alpha-numeric characters
                tagName += '%c' % random.choice(alphaNumList)
        return tagName

    def tagLeafNode(self, path, tagName, description=''):
        '''
        Apply a new tag to the head of the given folder.
        inputs:
            path:        string; full path to the folder
            tagName:     string; name of the tag to apply. It must be unique in the database.
            description: string; details about the tagging operation.
                         -> Default = ''
        outputs:
            none
        '''
        raise NotImplementedError()
        from PyCool import cool
        assert self.db != None, "No database connected !"
        assert not self.readOnly, "The database is in Read Only mode."
        if self.db.existsFolder(path):
            folder = self.db.getFolder(path)
        else:
            raise cool.FolderNotFound

        if folder.versioningMode() == cool.FolderVersioning.SINGLE_VERSION:
            raise Exception, "Folder %s is Single Version" % path
        else:
            try:
                folder.tagCurrentHead(tagName, description)
            except Exception, details:
                raise Exception, details

    def recursiveTag(self, path, tagName, description='', reserved=None):
        '''
        Tag the given node and recursively tag the child nodes and their HEAD revisions
        with randomly generated tags.
        inputs:
            path:         string; full path to the node
            tagName:      string; name of the tag to apply. It must be unique in the database.
            description:  string; details about the tagging operation.
                          -> Default = ''
            reserved:     list of strings; list of reserved tags.
                          -> Default = None
        outputs:
            none
        '''
        raise NotImplementedError()
        from PyCool import cool
        assert self.db != None, "No database connected !"
        assert not self.readOnly, "The database is in Read Only mode."
        if reserved != None:
            reservedTags = reserved[:]
        else:
            reservedTags = []
        # store the requested tag in the reserved list.
        if tagName not in reservedTags:
            reservedTags.append(tagName)
        if self.db.existsFolderSet(path):
            childNodes = self.getChildNodes(path)
            for nodeName in childNodes:
                # generate a random tag
                if '_auto_' in tagName:
                    baseName = tagName[6:-7]
                else:
                    baseName = tagName
                auto_tag = self.generateUniqueTagName(baseName, reservedTags)
                if self.db.existsFolder(nodeName):
                    # Apply the tag to the HEAD revision of the folder and link it
                    # to the parent tag.
                    folder = self.db.getFolder(nodeName)
                    if folder.versioningMode(
                    ) == cool.FolderVersioning.MULTI_VERSION:
                        folder.tagCurrentHead(auto_tag, description)
                        try:
                            self.createTagRelation(folder, tagName, auto_tag)
                        except Exception, details:
                            raise Exception, details
                else:
                    # Start recursion for the foldersets and create a tag relation with
                    # the parent.
                    self.recursiveTag(nodeName, auto_tag, description,
                                      reservedTags)
                    folderSet = self.db.getFolderSet(nodeName)
                    try:
                        self.createTagRelation(folderSet, tagName, auto_tag)
                    except Exception, details:
                        raise Exception, details
        elif self.db.existsFolder(path):
            node = self.db.getFolder(path)
            if node.versioningMode() == cool.FolderVersioning.MULTI_VERSION:
                node.tagCurrentHead(tagName, description)
        else:
            raise Exception, "Node %s was not found" % path
        # To exit the recursion correctly, we need to return from this function...
        return

    def tagWithAncestorTag(self, path, ancestorTag, description=''):
        '''
        Recursively tag (with automatically generated tag names) the given node and
        associate the tags with the ancestor tag given.
        inputs:
            path:        string; path of the node to tag
            ancestorTag: string; tag to associate the node with. It must be an existing
                         tag and a relation must exist with the parent of the given node.
                         Otherwise, an exception is raised.
            description: string; description to associate with the tagged elements. If empty,
                         the description of the closest ancestor tag will be used.
                         -> Default = ''
        outputs:
            none
        '''
        raise NotImplementedError()
        from PyCool import cool
        assert self.db != None, "No database connected !"
        assert not self.readOnly, "The database is in Read Only mode."
        # Check if the ancestor tag really exists.
        if not self.db.existsTag(ancestorTag):
            raise Exception, "Tag %s was not found" % ancestorTag
        # Retrieve the node
        node = self.getCOOLNode(path)
        if node is None:
            raise Exception, "Node %s was not found" % path
        # Check if the node is already related to the ancestor tag
        try:
            relatedTag = self.resolveTag(node, ancestorTag)
        except Exception:
            # The node is not related to the ancestor tag. We are checking if
            # its parent is (it *has* to be).
            try:
                sep = os.path.sep
                os.path.sep = '/'
                parentPath = os.path.dirname(path)
            finally:
                os.path.sep = sep
            parentNode = self.db.getFolderSet(parentPath)

            try:
                parentTag = self.resolveTag(parentNode, ancestorTag)
            except cool.NodeRelationNotFound:
                # The parent folderset doesn't know about the ancestor tag: this
                # means we don't know how to deal with its other child nodes.
                raise Exception, "No relation found between node %s and tag %s" % (
                    parentPath, ancestorTag)
            else:
                # The parent contains a relation to the ancestor tag.
                # Create (recursively if needed) a new tag.
                auto_tag = self.generateUniqueTagName(ancestorTag)
                self.recursiveTag(path, auto_tag, description)
                # Associate with parent tag
                self.createTagRelation(node, parentTag, auto_tag)
        else:
            # The node is already related to the ancestor tag. If the node is a folder,
            # it means that we have to delete the old relation and create a new one.
            # If the node is a folderset, it means that at least one of its child node
            # is related to the ancestor tag as well.
            if self.db.existsFolder(path):
                # get the parent tag related to the ancestor tag:
                try:
                    sep = os.path.sep
                    os.path.sep = '/'
                    parentPath = os.path.dirname(path)
                finally:
                    os.path.sep = sep
                parentNode = self.db.getFolderSet(parentPath)
                parentTag = self.resolveTag(parentNode, ancestorTag)
                # delete the relation between the related tag and the ancestor tag by
                # deleting the relation to the parent tag.
                self.deleteTagRelation(node, parentTag)
                # Create a new tag and connect it to the parent tag
                auto_tag = self.generateUniqueTagName(ancestorTag)
                node.tagCurrentHead(auto_tag, description)
                self.createTagRelation(node, parentTag, auto_tag)
            else:
                # We get all the child nodes, and to avoid the problem of child tags
                # already related to the ancestor tag, we call tagWithAncestorTag
                # recursively. Watch for bugs around here...
                childNodes = self.getChildNodes(path)
                for child in childNodes:
                    self.tagWithAncestorTag(child, ancestorTag, description)
        # To exit the recursion correctly, we need to return from this function...
        return

    def deleteTag(self, path, tagName, delete_relations=True):
        '''
        Delete a tag from the database, and its relations if asked for.
        inputs:
            path:             string; path to the node
            tagName:          string; name of the tag to delete
            delete_relations: boolean; this has a meaning only for folders. If True,
                              delete also the relations with the parent tag.
                              -> Default = True
        outputs:
            none
        '''
        raise NotImplementedError()
        assert self.db != None, "No database connected !"
        assert not self.readOnly, "The database is in Read Only mode."
        # Retrieve the node
        if self.db.existsFolder(path):
            node = self.db.getFolder(path)
        elif self.db.existsFolderSet(path):
            node = self.db.getFolderSet(path)
        else:
            raise Exception, "Node %s was not found" % path

        if tagName not in list(node.listTags()):
            raise Exception, "Tag %s was not found" % tagName

        # Get the tag object to be able to access parent tags
        tagList = self.getTagList(path)
        tag = tagList.pop()
        while tagList and tag.name != tagName:
            tag = tagList.pop()

        # If the node is a folderset, deleting the tag consists in deleting
        # its relations with parents and children. If it is a folder, deleting relations
        # with the parents is optional, but advisable (and the default behaviour of the
        # function).
        if self.db.existsFolderSet(path):
            for childPath in self.getChildNodes(path):
                if not self.isSingleVersionFolder(childPath):
                    self.deleteTagRelation(childPath, tagName)
            for parentTag in tag.parents:
                self.deleteTagRelation(path, parentTag.name)
        else:
            if delete_relations:
                for parentTag in tag.parents:
                    self.deleteTagRelation(path, parentTag.name)
            node.deleteTag(tagName)

    def isTagReady(self, tagName, path="/"):
        """
        Check if the given tag name is usable at the level of the specified path.
        inputs:
            tagName:          string; name of the tag to delete
            path:             string; path to the node
                              -> Default = '/'
        outputs:
            none
        """
        raise NotImplementedError()
        if tagName.upper() in ['', 'HEAD']:
            return True  # HEAD tags are always valid
        if self.db.existsFolderSet(path):
            # the path points to a folderset, I check the tag in all its subnodes
            nodes = self.getChildNodes(path)
            try:
                for n in nodes:
                    f = self.getCOOLNode(n)
                    self.resolveTag(f, tagName)
            except:
                return False
        else:
            try:
                f = self.db.getFolder(path)
                self.resolveTag(f, tagName)
            except:
                return False
        return True

    def cloneTag(self, path, src_tag, dest_tag, exclude=[]):
        """
        Create a copy of the tag src_tag with the name dest_tag on folderset
        path.
        The sub folder[set] in exclude (specified as fullpath) are not taken
        into account for the new tag.
        """
        raise NotImplementedError()
        if self.readOnly:
            raise "Cannot write on a database opened in read-only mode"
        if self.db.existsFolderSet(path):
            for n in [
                    self.getCOOLNode(p) for p in self.getChildNodes(path)
                    if p not in exclude
            ]:
                try:
                    local_tag = self.resolveTag(n, src_tag)
                    #print "%s: %s"%(n.fullPath(),local_tag)
                    self.createTagRelation(n, dest_tag, local_tag)
                except RuntimeError, x:
                    print "Warning: %s" % x
        else:
            if self.db.existsFolder(path):
                print "Warning: cloneTag not supported for folders"
            else:
                raise "Node '%s' does not exist" % path

    def _moveTagOnNodes_recursion(self, nodes_tags, path, basetag, nodestree):
        """
        Internal method to move a tag on a list of nodes.
        nodes_tags: dictionary with the tags to be associated to the folder[set]s.
        path: starting directory
        basetag: tag to use on path
        nodestree: dictionary of dictionaries representing the tree of nodes that
                   need to be changed.
        """
        raise NotImplementedError()
        # Get local tag
        tag = None
        currnode = self.getCOOLNode(path)
        if path in nodes_tags:
            try:
                tag = self.resolveTag(currnode, nodes_tags[path])
            except:
                tag = nodes_tags[path]

        if nodestree:  # if it has children
            for n in nodestree:
                nodes_tags[n] = self._moveTagOnNodes_recursion(
                    nodes_tags, n, basetag, nodestree[n])

            # the base node is the only node allowed to have
            # the tag in the nodes_tags dictionary
            if tag is None:
                tag = self.generateUniqueTagName(basetag)
                self.cloneTag(path, basetag, tag, nodestree.keys())

            for n in nodestree:
                x = self.getCOOLNode(n)

                try:
                    # try to delete an already present relation
                    self.deleteTagRelation(x, tag)
                except:
                    # if there is not such a relation, ignore the failure
                    pass
                self.createTagRelation(x, tag, nodes_tags[n])
        else:
            # Do a recursive tag if we want HEAD
            if tag is None or tag.upper() == "HEAD":
                tag = self.generateUniqueTagName(basetag)
                self.recursiveTag(path, tag)
        # return the local tag
        return tag

    def moveTagOnNodes(self, basepath, tag, nodes_tags):
        """
        Move (or create) the tag "tag" on the folderset basepath using the tags
        specified in nodes_tags (dictionary) for the sub-folder[set]s.
        If the tag for a sub-folder[set] is "HEAD", then it is automatically
        tagged with recursiveTag.
        """
        raise NotImplementedError()
        # Dictionary representing the tree of nodes to touch
        # leaf nodes are
        basepath = basepath.strip('/')
        if basepath == '':
            bp = []
        else:
            bp = basepath.split('/')
        basepath = '/' + basepath
        nodes_to_modify = {}
        for n in nodes_tags:
            ns = n.strip('/').split('/')
            if not ns[0:len(bp)] == bp:
                raise "Error: node '%s' is not under '%s'" % (n, basepath)
            d = nodes_to_modify
            base = basepath
            for dirname in ns[len(bp):]:
                if base == '/':
                    dirname = base + dirname
                else:
                    dirname = base + '/' + dirname
                if dirname not in d:
                    d[dirname] = {}
                d = d[dirname]
                base = dirname

        nodes_tags[basepath] = tag
        self._moveTagOnNodes_recursion(nodes_tags, basepath, tag,
                                       nodes_to_modify)

    def moveTagOnLocalTag(self, basepath, tag, local_tag):
        """
        Move (or create) the tag "tag" on the folderset basepath using the tag
        specified as local_tag.
        It first collects the minimal set of folder(set)s under basepath in which
        the local_tag can be resolved, then calls moveTagOnNodes.
        """
        raise NotImplementedError()
        nodes = self.findNodesWithTag(local_tag, base=basepath, leaves=False)
        if not nodes:
            # nothing to do
            print "Warning: Tag '%s' not found" % local_tag
            return
        nodes_tags = {}
        for n in nodes:
            nodes_tags[n] = local_tag
        self.moveTagOnNodes(basepath, tag, nodes_tags)

    def findTagRelation(self, node, tag):
        """
        Return the local tag associated with the parent tag given.
        """
        raise NotImplementedError()
        from .Helpers import Helpers
        localtag = Helpers.findTagRelation(node, tag)
        if localtag == "":
            raise RuntimeError('No child tag can be found in node')
            return ""
        return localtag

    def resolveTag(self, path, tag, doraise=True):
        """
        Return the local tag associated with the parent tag given.
        """
        # we have only global tags in Git
        return tag

    def findNodesWithTag(self, tag, base="/", leaves=True):
        """
        Find all the nodes that have a local tag associated with provided parent tag.

        If leaves is True (the default), it returns a list of the folders (leaves).
        If it is False, the list is reduced to a set of foldersets in which the tag
        is completely defined and folders (in case a common folderset couldn be identified).
        """
        raise NotImplementedError()
        # this finds all the leaves in which a tag can be resolved
        l = filter(
            lambda p: self.isMultiVersionFolder(p) and self.resolveTag(p, tag),
            self.getAllChildNodes(base))

        if leaves:
            # enough
            return l

        # let's try to find a minimal set of folder(set)s below "base"
        if base == '/':
            base = ['']
        else:
            base = base.split('/')

        base_count = len(base)

        newlist = []  # where to put the extracted bases
        # compare one element with all the others until we exaust the list
        while l:
            current = l[0]
            l.remove(current)
            # do not have to compare something with the base because is guaranteed
            # to be included in each element
            curr_split = current.split('/')[base_count:]

            lcopy = l[:]
            for elem in lcopy:
                elem_split = elem.split('/')[base_count:]
                # find common part between current and elem
                i = 0
                m = min(len(elem_split), len(curr_split))
                while i < m and elem_split[i] == curr_split[i]:
                    i += 1
                if i:  # check if they have any element in common
                    candidate = '/'.join(base + curr_split[:i])
                    # here we define the other constraints:
                    #   - it can be a folder (probably overkilling as a check)
                    #   - otherwise the tag must be defined in all the children
                    if ( self.db.existsFolder(candidate) ) \
                       or ( self.isTagReady(tag,candidate) ):
                        l.remove(elem)
                        curr_split = curr_split[:i]
                        current = candidate

            newlist.append(current)
        return newlist

    #---------------------------------------------------------------------------------#

    #=============================#
    # Database Editing Operations #
    #=============================#

    def dropDatabase(cls, connectionString):
        '''
        drop the database identified by the connection string.
        inputs:
            connectionString: string; standard COOL connection string.
        outputs:
            none
        '''
        raise NotImplementedError()
        dbsvc = coolApp().databaseService()
        try:
            dbsvc.dropDatabase(connectionString)
        except Exception, details:
            raise Exception, "Impossible to drop the database: %s" % details

    dropDatabase = classmethod(dropDatabase)

    def createNode(self,
                   path,
                   description='',
                   storageType="XML",
                   versionMode="MULTI",
                   storageKeys={'data': 'String16M'}):
        '''
        Creates a new node (folder or folderset) in the database.
        inputs:
            path:        string; full path of the new node. Parents will be created if necessary.
            description: string; short description of the node.
                         -> Default = ''
            storageType: string; data type to be stored in this node, implying the type of node
                         to create. If the node is a folder, it will contain 'XML'. If it is a
                         folderset, it will contain 'NODE'.
                         -> Default = 'XML'
            versionMode: string; applies to folders only: is it multi version ('MULTI') or single
                         version ('SINGLE') ?
                         -> Default = 'MULTI'
            storageKeys: dictionary mapping strings (names) to strings (COOL storage types);
                         fields that will be stored in the folder.
                         A list of strings can be used in alternative to the dictionary, in which
                         case the type defaults to "String16M".
                         -> Default = {'data': 'String16M'}
        outputs:
            none
        '''
        raise NotImplementedError()
        from PyCool import cool
        assert self.db != None, "No database connected !"
        assert not self.readOnly, "The database is in Read Only mode."
        if storageType == 'NODE':
            try:
                self.db.createFolderSet(path, description, True)
            except Exception, details:
                raise Exception, "Impossible to create the folderset: %s" % details
        else:
            recordSpec = cool.RecordSpecification()
            if type(storageKeys) is list:
                d = {}
                for k in storageKeys:
                    d[k] = "String16M"
                storageKeys = d
            for key in storageKeys:
                recordSpec.extend(key,
                                  getattr(cool.StorageType, storageKeys[key]))

            if versionMode == 'MULTI':
                folderSpec = cool.FolderSpecification(
                    cool.FolderVersioning.MULTI_VERSION, recordSpec)
            else:
                folderSpec = cool.FolderSpecification(
                    cool.FolderVersioning.SINGLE_VERSION, recordSpec)

            # WARNING: this folderdesc stuff is VERY important for LHCb: it tells the CondDB conversion
            #          service which type of converter to call. In this case (storage_type = 7), it calls
            #          the XML converter. It has to be appended to the folder description.
            folderDesc = description + " <storage_type=7>"

            try:
                self.db.createFolder(path, folderSpec, folderDesc, True)
            except Exception, details:
                raise Exception, "Impossible to create the folder: %s" % details

    def storeXMLString(self, path, data, since, until, channelID=0):
        '''
        Adds a new condition object to the database.
        inputs:
            path:      string; path of the folder where the condition will be stored.
            data:      dictionary; a dictionary version of the attribute list to store in the database.
            since:     integer; lower bound of the interval of validity.
            until:     integer; upper bound of the interval of validity. It is excluded from
                       the interval.
            channelID: integer; ID of the channel where to store the condition.
                       -> Default: 0

        outputs:
            none
        '''
        raise NotImplementedError()
        if type(data) is str:
            data = {'data': data}
        objDict = {
            'payload': data,
            'since': since,
            'until': until,
            'channel': channelID
        }
        self.storeXMLStringList(path, [objDict])

    def storeXMLStringList(self, path, XMLList, writeDuplicate=True):
        '''
        Allows to store a list of XML string into a given folder.
        inputs:
            path:    string; the path to the folder
            XMLList: list of dictionary of 'payload': dictionary: a dictionary version of the attribute
                                                                  list to store in the database.
                                           'since': integer:      the lower bound of the IOV.
                                           'until': integer:      the upper bound of the IOV.
                                           'channel': integer:    the channel ID.
        outputs:
            none
        '''
        raise NotImplementedError()
        from PyCool import cool
        assert self.db != None, "No database connected !"
        assert not self.readOnly, "The database is in Read Only mode."
        if self.db.existsFolder(path):
            folder = self.db.getFolder(path)
            # Create a payload object with the correct specifications
            payload = cool.Record(folder.payloadSpecification())
            for key in payload.keys():
                payload[key] = 'nothing'
            # Start filling the buffer...
            folder.setupStorageBuffer()
            for obj in XMLList:
                # Verify that the object can be stored in the folder
                payloadKeys = payload.keys()
                payloadKeys.sort()
                objPayloadKeys = obj['payload'].keys()
                objPayloadKeys.sort()
                assert payloadKeys == objPayloadKeys, "An object's payload specification doesn't fit the folder's one"

                for key in payloadKeys:
                    payload[key] = obj['payload'][key]
                since = cool.ValidityKey(obj['since'])
                until = cool.ValidityKey(obj['until'])
                channelID = obj['channel']
                doWrite = True
                if not writeDuplicate:
                    # Do not write the same xml content twice
                    dbxmlstr = ""
                    try:
                        dbobj = folder.findObject(since, channelID)
                        dbpayload = dict(self.payload(dbobj))
                        for key in payloadKeys:  #NB: Only works with the case of only one key
                            if payload[key] == str(dbpayload[key]):
                                print "identical content for path %s, no overwriting" % path
                                doWrite = False
                    except:
                        pass
                if doWrite:
                    self.storeObject(folder, since, until, payload,
                                     cool.ChannelId(channelID))

            # Write the data to the DB
            folder.flushStorageBuffer()
        else:
            raise Exception, "Folder %s was not found" % path

    def deleteNode(self, path, delete_subnodes=False):
        """
        Delete a node from the database permanently.
        inputs:
            path:            string; node's path in the database
            delete_subnodes: boolean; only useful for fodlersets. If True, all the subnodes
                             will be destroyed as well. If False, a node can be deleted only
                             if it has no children.
                             -> Default = False

        outputs:
            none
        """
        raise NotImplementedError()
        assert self.db != None, "No database connected !"
        assert not self.readOnly, "The database is in Read Only mode."
        # Deal first with the full tree deletion
        if delete_subnodes and self.db.existsFolderSet(path):
            subnodeList = self.getAllChildNodes(path)
            # Reverse the tree order to start from the subelements
            subnodeList.reverse()
            for node in subnodeList:
                try:
                    self.db.dropNode(node)
                except Exception, details:
                    raise Exception, "Impossible to delete node %s: %s" % (
                        node, details)
        # In all cases, try to delete the given node
        try:
            self.db.dropNode(path)
        except Exception, details:
            raise Exception, "Impossible to delete node %s: %s" % (path,
                                                                   details)

    def existsFolder(self, path):
        return path in self.getAllNodes() and self._tree.get(
            path.lstrip('/')) is None

    def existsFolderSet(self, path):
        return path in self.getAllNodes() and self._tree.get(
            path.lstrip('/')) is not None

    def getChannelsWithNames(self, path):
        return {0: ''}
