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
def setup(tag, use_files=False, bare=False, conditions=['/dd/TestCondition'], overlay=False):
    import os
    from Gaudi.Configuration import appendPostConfigAction, VERBOSE
    from Configurables import ApplicationMgr
    from Configurables import GitEntityResolver, DDDBConf, CondDB, XmlParserSvc
    from Configurables import DetCondTest__TestConditionAlg as TestAlg

    repo = os.environ['GIT_TEST_REPOSITORY']
    if bare:
        repo += '-bare.git'
    ger = GitEntityResolver('GitDDDB',
                            PathToRepository=repo,
                            OutputLevel=VERBOSE)
    if use_files:
        ger.Commit = ''

    DDDBConf(DataType='2016', DbRoot='git:/lhcb.xml',
             EnableRunStampCheck=False)
    CondDB(Tags={'DDDB': tag},
           LatestGlobalTagByDataTypes=[])
    if overlay:
        GitEntityResolver('GitOverlay_0', OutputLevel=VERBOSE)
        CondDB().addLayer(repo + '-overlay')

    ApplicationMgr().TopAlg.append(TestAlg(Conditions=conditions))

    # override some settings from DDDBConf
    def reduce_resolver():
        resolvers = XmlParserSvc().EntityResolver.EntityResolvers
        resolvers[:] = [r for r in resolvers
                        if r.name()[8:15] in ('GitDDDB', 'GitOver')]
    appendPostConfigAction(reduce_resolver)
