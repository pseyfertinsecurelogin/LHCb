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
import genSrcUtils, tools, gparser, genClasses
import logging


#================================================================================
class genNamespaces(genSrcUtils.genSrcUtils):
    #--------------------------------------------------------------------------------
    def __init__(self, godRoot):
        genSrcUtils.genSrcUtils.__init__(self)
        self.godRoot = godRoot
        self.tools = tools.tools()
        self.packInclude = []
        self.packExcludes = []
        self.forwardDeclLHCb = []
        self.forwardDeclGlob = {}
        self.forwardIncl = []
        self.log = logging.getLogger('GODGenNamespaces')
#--------------------------------------------------------------------------------

    def reset(self, package, godNamespace):
        genSrcUtils.genSrcUtils.reset(self, godNamespace)
        self.include = list(package.include)
        self.forwardDeclLHCb = list(package.forwardDeclLHCb)
        self.forwardDeclGlob = package.forwardDeclGlob
        self.forwardIncl = list(package.forwardIncl)
        self.excludes = list(package.excludes)
#--------------------------------------------------------------------------------

    def parseNamespaceImport(self, dict):
        forwardIncl = []
        self.parseImport(dict, self.include, self.stdIncludes,
                         self.forwardDeclLHCb, self.forwardDeclGlob,
                         forwardIncl)
#--------------------------------------------------------------------------------

    def genAuthor(self, godNamespace):
        return godNamespace.get('author', 'unknown')
#--------------------------------------------------------------------------------

    def genClasses(self, package, godPackage, outputDir, lname, allocatorType,
                   nsname):
        s = ''
        incs = ''
        fwds = ''
        if 'class' in godPackage:
            self.log.debug('generating classes inside namespace')
            gClassesNS = genClasses.genClasses(self.godRoot)
            (s2, inc, fwd) = gClassesNS.doit(
                package,
                godPackage['class'],
                outputDir,
                lname,
                allocatorType,
                nsname=nsname)
            s += s2
            incs += inc
            fwds += fwd
        return (s, incs, fwds)
#--------------------------------------------------------------------------------

    def doit(self,
             package,
             godNamespaces,
             outputDir,
             lname,
             allocatorType,
             format=None):

        default_scope = package.dict['packagenamespace']

        for godNamespace in godNamespaces:

            self.reset(package, godNamespace)
            self.parseNamespaceImport(godNamespace)

            namespaceDict = package.dict
            namespacename = godNamespace['attrs']['name']
            scope = godNamespace['attrs'].get('scope', default_scope)
            scoped_namespacename = scope + '::' + namespacename

            fileName = '%s.h' % namespacename
            if 'fileName' in godNamespace['attrs']:
                fileName = '%s.h' % godNamespace['attrs']['fileName']

            mess = 'File ' + fileName.ljust(lname)

            namespaceDict['namespacename'] = namespacename
            namespaceDict['desc'] = self.genDesc(godNamespace)
            namespaceDict['author'] = self.genAuthor(godNamespace)
            namespaceDict['namespaceScope'] = scope
            namespaceDict['inline']           = 'inline ' if\
                                                godNamespace['attrs']['inline']\
                                                == 'TRUE' else ''
            namespaceDict['typedefs'] = self.genTypedefs('all', godNamespace)
            namespaceDict['enums'] = self.genEnums('all', godNamespace)
            namespaceDict[
                'enumOstreamOverloads'] = self.genEnumOstreamOverloads(
                    godNamespace)
            namespaceDict['attributes'] = self.genAttributes(
                'all', godNamespace, 1)
            namespaceDict['methods'] = self.genMethods('all', godNamespace)
            namespaceDict['methodDefs'] = self.genMethods(
                'all', godNamespace, scoped_namespacename)
            (namespaceDict['classes'],
             clincludes, clfwDeclGlob) = self.genClasses(
                 package, godNamespace, outputDir, 1, allocatorType,
                 godNamespace['attrs']['name'])
            namespaceDict['includes'] = self.genIncludes() + clincludes
            namespaceDict['forwardDeclsLHCb'] = self.genForwardDeclsLHCb(
            ) + clfwDeclGlob
            namespaceDict['forwardDeclsGlob'] = self.genForwardDeclsGlob()
            namespaceDict['forwardIncludes'] = self.genForwardIncludes(
                namespacename)

            g = gparser.gparser()

            g.parse(self.godRoot + 'templates/namespace.tpl', namespaceDict)

            file = open(outputDir + os.sep + fileName, 'w')
            file.write(g.stream)
            file.close()
            if format:
                format(outputDir + os.sep + fileName)

            mess += ' - Done'
            self.log.info(mess)


#================================================================================
#import xparser
#if __name__ == '__main__':
#  x = xparser.xparser('xml_files/MuonEvent.xml','xml_files/GODsClassDB.xml')
#  x.parse()
#  g = genNamespaces(x.gdd)
#  g.doit()
