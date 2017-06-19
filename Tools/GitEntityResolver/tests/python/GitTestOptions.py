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
        CondDB().addLayer(repo + '-overlay')

    ApplicationMgr().TopAlg.append(TestAlg(Conditions=conditions))

    # override some settings from DDDBConf
    appendPostConfigAction(lambda: XmlParserSvc(EntityResolver=ger))
