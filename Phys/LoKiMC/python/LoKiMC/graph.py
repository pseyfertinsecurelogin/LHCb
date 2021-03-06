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
# =============================================================================
## @file LoKiMC/graph.py
#  Simple utilities to vizualize the decay tree
#
#        This file is a part of LoKi project -
#    "C++ ToolKit  for Smart and Friendly Physics Analysis"
#
#  The package has been designed with the kind help from
#  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
#  contributions and advices from G.Raven, J.van Tilburg,
#  A.Golutvin, P.Koppenburg have been used in the design.
#
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2016-01-16
# =============================================================================
"""Simple utilities to vizualize the decay tree

      This file is a part of LoKi project -
'C++ ToolKit  for Smart and Friendly Physics Analysis'

The package has been designed with the kind help from
Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
contributions and advices from G.Raven, J.van Tilburg,
A.Golutvin, P.Koppenburg have been used in the design.
"""
# =============================================================================
__author__ = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
__date__ = "2015-01-16"
__version__ = ""
__all__ = (
    'graph',  ## function to produce graph
    'view',  ## function to produce graph and immediately display it
    'MCDecayGraph'  ## helper class to produce graph
)
# =============================================================================
import pydot
from LoKiMC.MC import LHCb
#
node_attributes = {
    'shape': 'ellipse',
    'fillcolor': 'yellow',
    'color': 'red',
    'style': 'filled'
}
edge_attributes = {'color': 'red'}
#
PARTICLE = LHCb.MCParticle


#
# =============================================================================
## @class MCDecayGraph
#  Make graphical (dot) representation of the decay tree
#  @code
#  p     = ...
#  dg    = MCDecayGraph ( p )
#  graph = gd.graph()
#  graph.write('graph.png', format = 'png')
#  @endcode
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date 2016-01-16
class MCDecayGraph(object):
    """ Make graphical representation of the decay tree
    >>> p     = ...
    >>> dg    = MCDecayGraph ( p )
    >>> graph = gd.graph()
    >>> graph.write('graph.png', format = 'png')
    """

    def __init__(self, trees, decay_only=True):
        self._nodes = set()
        self._edges = set()
        self._graph = pydot.Dot('DecayTree', graph_type='digraph')
        self._decay_only = decay_only

        if isinstance(trees, PARTICLE): trees = [trees]
        for t in trees:
            self.add_tree(t)

    ## construct node-id for the given particle
    def node_id(self, p):
        """
        Construct unique node-id for the given particle
        """

        if not p: return ''

        ## construct some unique name
        nid = '%s:%s:#%d' % (p.name(), p.hex_id(), p.key())
        ## container
        cnt = p.parent()
        if cnt:
            r = cnt.registry()
            if r: nid = '%s:%s:#%d' % (p.name(), r.identifier(), p.key())
        ## massage the name:
        return pydot.quote_if_necessary(nid)

    ## add particle to graph
    def add_tree(self, p):

        nid = self.node_id(p)
        if nid in self._nodes: return nid

        # create new node
        node = pydot.Node(name=nid, label=p.name(), **node_attributes)

        self._nodes.add(nid)
        self._graph.add_node(node)

        for c in p.children(self._decay_only):
            nidc = self.add_tree(c)  ## NB: recursion here
            edge = nid, nidc
            if not edge in self._edges:
                self._graph.add_edge(pydot.Edge(
                    *edge, **edge_attributes))  ## create edge
                self._edges.add(edge)

        return nid

    ## get the graph
    def graph(self):
        return self._graph


# =============================================================================
## make a graph for given particle or container of particles
#  @code
#  p               = ...
#  graph_file_name = graph( p , format  = 'png' )
#  @endcode
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date 2016-01-16
def graph(particle, format='dot', filename='mcdecaytree', decay_only=True):
    """ Make a graph for given particle
    >>> p               = ...
    >>> graph_file_name = graph( p , format  = 'png' )
    """
    dg = MCDecayGraph(particle, decay_only)
    dt = dg.graph()
    #
    return filename if dt.write(filename, format=format) else None


# =============================================================================
## prepare the graph and vizualize it
#  @code
#  p = ...
#  p.view ( format  = 'png' )
#  @endcode
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date 2016-01-16
def view(particle, command=None, format='png', decay_only=True):
    """Prepare the graph and vizualize it
    p = ...
    p.view ( format  = 'png' )
    p.view ( format  = 'png' , commmand = 'eog' )
    """

    class _TmpName_(object):
        def __init__(self, suffix='png'):
            self.suffix = '.' + suffix

        def __enter__(self):
            import tempfile
            self.name = tempfile.mktemp(suffix=self.suffix)
            return self.name

        def __exit__(self, *_):
            import os
            try:
                if os.path.exists(self.name):
                    os.remove(self.name)
            except:
                pass

    with _TmpName_(format) as ofile:

        if not graph(particle, format, filename=ofile, decay_only=decay_only):
            print 'Error producing the graph for %s ' % particle.decay()
            return

        if not command:
            import distutils.spawn as ds
            for i in ('eog', 'display', 'gnome-photos', 'gwenview', 'gimp',
                      'gthumb', 'google-chrome'):
                command = ds.find_executable(i)
                if command: break

        if not command:
            print 'No valid command is found!'
            return

        import subprocess
        try:
            subprocess.check_call("%s %s " % (command, ofile), shell=True)
        except subprocess.CalledProcessError:
            pass


for t in (PARTICLE, PARTICLE.Container, PARTICLE.ConstVector, PARTICLE.Range):

    t.graph = graph
    t.view = view

# =============================================================================
if '__main__' == __name__:

    print 80 * '*'
    print __doc__
    print ' Author  : ', __author__
    print ' Version : ', __version__
    print ' Date    : ', __date__
    print 80 * '*'

# =============================================================================
# The END
# =============================================================================
