#!/usr/bin/env python

import argparse
from Configurables import ApplicationMgr
from Configurables import ConfigCDBAccessSvc, PropertyConfigSvc
import cppyy
import weakref
from pprint import pprint
import shelve

parser = argparse.ArgumentParser(usage='usage: %(prog)s tck')

parser.add_argument("tck", help="what TCK")

args = parser.parse_args()

tck = int(args.tck, 16)

ApplicationMgr().AppName = ""
ApplicationMgr().OutputLevel = 3

cas_conf = ConfigCDBAccessSvc()
pcs_conf = PropertyConfigSvc(ConfigAccessSvc=cas_conf.getFullName())

import GaudiPython

# Decorate the MD5 object
MD5 = cppyy.gbl.Gaudi.Math.MD5
MD5.__str__ = MD5.str
MD5.__hash__ = lambda x: x.str().__hash__()
MD5.__eq__ = lambda self, other: self.str() == str(other)
MD5.__ne__ = lambda self, other: self.str() != str(other)
digest = MD5.createFromStringRep

ConfigTreeNodeAlias = cppyy.gbl.ConfigTreeNodeAlias
ConfigTreeNodeAlias.alias_type.__str__ = ConfigTreeNodeAlias.alias_type.str
alias = ConfigTreeNodeAlias.alias_type
topLevelAlias = ConfigTreeNodeAlias.createTopLevel
TCK = ConfigTreeNodeAlias.createTCK


def _tck(x):
    if isinstance(x, basestring) and x.upper().startswith('0X'):
        return int(x, 16)
    return int(x)


def _digest(x):
    if type(x) == str:
        x = digest(x)
    return x


def resolveTCK(tck, cas):
    """Return the id (digest) for a TCK."""
    tck = _tck(tck)
    for i in cas.configTreeNodeAliases(alias('TCK/')):
        if _tck(i.alias().str().split('/')[-1]) == tck:
            return digest(i.ref().str())
    raise KeyError('Not a valid TCK: {:#x}'.format(tck))


def _2id(id, cas):
    if type(id) is int:
        id = '0x%08x' % id
    if type(id) is str and len(id) == 32:
        id = _digest(id)
    #  if we're not a valid id at this point, maybe we're a TCK...
    if type(id) is not MD5:
        id = resolveTCK(id, cas)
    return id


def resolveConfigTreeNode(id, cas, pcs):
    if type(id) is not MD5:
        id = _2id(id, cas)
    return pcs.resolveConfigTreeNode(id) if (id and id.valid()) else None


def loadAliases(prefix, cas, pcs):
    """Get the existing entries from the access services"""
    r = {}
    for i in cas.configTreeNodeAliases(alias(prefix + '/')):
        tck_id = i.ref().str()
        s = i.alias().str()
        x = resolveConfigTreeNode(i.ref(), cas, pcs)
        r[tck_id] = (s, x)
    return r


# create the AppMgr
gaudi = GaudiPython.AppMgr()

# Create the services we need
gaudi.createSvc(cas_conf.getFullName())
cas = gaudi.service(cas_conf.getFullName(), 'IConfigAccessSvc')
gaudi.createSvc(pcs_conf.getFullName())
pcs = gaudi.service(pcs_conf.getFullName(), 'IPropertyConfigSvc')


class TreeNode(object):
    " A class representing a ConfigTreeNode "
    #  use flyweight pattern, and use digest to identify objects...
    import weakref
    _pool = weakref.WeakValueDictionary()
    _nreq = 0
    _noid = 0
    _cm = 0
    #  TODO: add functionality to flush _pool

    def __new__(cls, id=None):
        TreeNode._nreq = TreeNode._nreq + 1
        if not id:
            TreeNode._noid = TreeNode._noid + 1
            return object.__new__(cls)
        if type(id) != MD5 and type(id) != int:
            print id, type(id)
            id = digest(id)
        obj = TreeNode._pool.get(id)
        if not obj:
            TreeNode._cm = TreeNode._cm + 1
            obj = resolveConfigTreeNode(id, cas, pcs)
            TreeNode._pool[id] = obj
        if not obj:
            print 'failed to resolve ConfigTreeNode %s' % id
        return obj


class PropCfg(object):
    " A class representing a PropertyConfig "
    #  use flyweight pattern, and use digest to identify objects...
    __slots__ = ('__weakref__', 'name', 'type', 'kind',
                 'fullyQualifiedName', 'digest', 'props')

    import weakref
    _pool = weakref.WeakValueDictionary()
    _nreq = 0
    _noid = 0
    _cm = 0
    #  TODO: make a singleton svc which we use to resolve IDs if not
    #  existent yet...
    # TODO: add functionality to flush _pool

    def __new__(cls, id=None):
        PropCfg._nreq = PropCfg._nreq + 1
        if not id:
            PropCfg._noid = PropCfg._noid + 1
            # added to make it possible to recv in parent process...
            return object.__new__(cls)
        if type(id) != MD5:
            print id, type(id)
            id = digest(id)
        obj = PropCfg._pool.get(id)
        if not obj:
            PropCfg._cm = PropCfg._cm + 1
            x = pcs.resolvePropertyConfig(id)
            if not x:
                print 'failed to resolve PropertyConfig ' + id
            obj = object.__new__(cls)
            obj.name = x.name()
            obj.type = x.type()
            obj.kind = x.kind()
            obj.fullyQualifiedName = x.fullyQualifiedName()
            obj.digest = id
            obj.props = dict((i.first, i.second) for i in x.properties())
            PropCfg._pool[id] = obj
        if not obj:
            print 'invalid PropCfg %s' % id
        return obj

    def properties(self):
        return self.props

    def fqn(self):
        return self.fullyQualifiedName + ' (' + self.kind + ')'

    def fmt(self):
        return ["'%s':%s\n" % (k, v) for k, v in self.props.iteritems()]

    def evaled_props(self):
        def try_eval(x):
            try:
                return eval(x, {}, {})
            except (SyntaxError, NameError):
                return x
            except:
                print x
                raise
        return {k: try_eval(v) for k, v in self.props.iteritems()}


class Tree(object):
    __slots__ = ('__weakref__', 'digest', 'label', 'leaf', 'nodes')
    _pool = weakref.WeakValueDictionary()

    def __new__(cls, id=None):
        if not id:
            return object.__new__(cls)
        if not isinstance(id, (MD5, int)):
            print id, type(id)
            id = digest(id)
        obj = cls._pool.get(id)
        if not obj:
            obj = object.__new__(cls)
            cls._pool[id] = obj
            node = TreeNode(id)
            if not node:
                print 'invalid TreeNode for ' + id
            obj.digest = node.digest().str()
            obj.label = node.label()
            leaf = node.leaf()
            obj.leaf = PropCfg(leaf) if leaf.valid() else None
            obj.nodes = [cls(nid) for nid in node.nodes()]
        return obj

    def prnt(self, depth=0, file=None):
        s = ' --> ' + str(self.leaf.digest) if self.leaf else ''
        indent = depth * '   '
        print >>file, indent + str(self.digest) + (30 - len(indent)) * ' ' + s
        for tree in self.nodes:
            tree.prnt(depth + 1, file)

    def _inorder(self, visited=None):
        if visited is not None:
            if self in visited:
                return
            visited.add(self)
        yield self
        for i in self.nodes:
            for x in i._inorder(visited):
                yield x

    def iter(self, duplicates=False):
        return self._inorder(None if duplicates else set())

    def __iter__(self):
        return self._inorder(set())

    def iterleafs(self, duplicates=False):
        """Return a generator yielding leafs.

        Args:
            duplicates: whether to traverse the full tree (slow!)

        Yields:
            leaf objects of type PropCfg

        """
        return (i.leaf for i in self.iter(duplicates) if i.leaf)

    def leafs(self):
        return {i.name: i for i in self.iterleafs()}


def getConfigTree(id, cas=cas):
    if not hasattr(getConfigTree, 'forest'):
        getConfigTree.forest = dict()
    if id not in getConfigTree.forest:
        tree = Tree(id)
        getConfigTree.forest[id] = tree
        if tree.digest != id:
            # in case we got a TCK, the remote side resolves this to an ID
            # and we mark this ID in our cache. Unfortunately, it doesn't work
            # the other way around (i.e. we first get an ID, then a TCK )
            # and we must rely on the remote to cache as much as possible...
            getConfigTree.forest[tree.digest] = tree
    return getConfigTree.forest[id]


def getProperties(id, algname='', property='', cas=cas):
    retlist = dict()
    tree = getConfigTree(id, cas)
    import re
    if algname:
        reqNode = re.compile(algname)

        def matchNode(leaf):
            return reqNode.match(leaf.fullyQualifiedName)
    else:

        def matchNode(leaf):
            return True
    if property:
        reqProp = re.compile(property)

        def matchProp(x):
            return reqProp.match(x)
    else:

        def matchProp(x):
            return True

    for leaf in tree.iterleafs():
        if not leaf or not matchNode(leaf):
            continue
        pdict = {k: v for k, v in leaf.properties().iteritems()
                 if matchProp(k)}
        if pdict:
            retlist[leaf.fullyQualifiedName] = pdict
    return retlist


gaudi.initialize()

db = shelve.open('0x%08x.props' % tck)
db['factories'] = getProperties(tck, '.*LoKi.*/.*Factory.*', 'Modules')
db['writers'] = getProperties(tck, 'HltRoutingBitsWriter/.*RoutingBitsWriter',
                              'RoutingBits')

db.close()

gaudi.finalize()
gaudi.exit()
