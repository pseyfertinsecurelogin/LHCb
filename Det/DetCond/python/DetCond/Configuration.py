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
"""
High level configuration tools for Conditions Database.
"""
__author__ = "Marco Clemencic <Marco.Clemencic@cern.ch>"

from Gaudi.Configuration import (allConfigurables, ConfigurableUser,
                                 getConfigurable, log)
from Configurables import ApplicationMgr

import os
import re

__all__ = ('CondDB', )


class NoTagError(RuntimeError):
    def __init__(self, partition, datatype):
        RuntimeError.__init__(
            self, "Cannot find tags for partition '%s' and"
            " data type '%s'" % (partition, datatype))
        self.partition = partition
        self.datatype = datatype


class CondDB(ConfigurableUser):
    """
    Configurable user to allow high-level configuration of the access to the
    conditions database.
    """
    __slots__ = {
        "Tags": {},
        "Simulation": False,
        "Upgrade": False,
        "Overlays": [],
        "Online": False,
        "IgnoreHeartBeat": False,
        "EnableRunStampCheck": False,
        "LatestGlobalTagByDataType": "",
        "LatestGlobalTagByDataTypes": [],
        "QueryGranularity": 0,
        'EnableRunChangeHandler': False,
        'RunChangeHandlerConditions': {
            'online_%d.xml': [
                "Conditions/Online/LHCb/Magnet/Set",
                "Conditions/Online/Velo/MotionSystem",
                "Conditions/Online/LHCb/Lumi/LumiSettings",
                "Conditions/Online/LHCb/LHCFillingScheme",
                "Conditions/Online/LHCb/RunParameters",
                "Conditions/Online/Rich1/R1HltGasParameters",
                "Conditions/Online/Rich2/R2HltGasParameters"
            ]
        },
        'AllLocalTagsByDataType': [],
        'LocalTags': {}
    }
    _propertyDocDct = {
        'Tags':
        """ Dictionary of tags (partition:tag) to use for the COOL databases. """
        """The special tag value '<latest:DataType>' can be used to select the latest global tag for the specified DataType """,
        'Simulation':
        """ Boolean flag to select the simulation or real-data configuration """,
        'Upgrade':
        """ Boolean flag to select the Upgrade simulation configuration """,
        'Overlays':
        """ Internal property used to add layers or alternatives """,
        'Online':
        """ Flag to activate configuration options specific for the Online environment """,
        'IgnoreHeartBeat':
        """ Do not set the HeartBeatCondition for the Online partition """,
        'EnableRunStampCheck':
        """ Enable the check for the special RunStamp condition """,
        'LatestGlobalTagByDataType':
        """ Use latest CondDB global tag marked with the data type""",
        'LatestGlobalTagByDataTypes':
        """ Use latest CondDB global tag marked with the data type, will override LatestGlobalTagByDataType if set""",
        'QueryGranularity':
        """Granularity of the query in the database (in time units)""",
        'AllLocalTagsByDataType':
        '[[deprecated]] Local tags not supported by Git CondDB',
        'LocalTags':
        '[[deprecated]] Local tags not supported by Git CondDB',
    }

    def addOverlay(self, path):
        """
        Add the given path as a overlay of the existing configuration.

        Example:
        CondDB().addOverlay('path/to/my/files')
        """
        if not os.path.isdir(path):
            raise ValueError('invalid argument: %r must be a directory' % path)
        self.Overlays.append(path)

    addLayer = addOverlay
    addAlternative = addOverlay

    def _addOverlay(self, path):
        if not os.path.isdir(path):
            raise ValueError(
                'invalid overlay argument: %r must be a directory' % path)
        from Configurables import GitEntityResolver, XmlParserSvc
        name = "GitOverlay_0"
        i = 0
        while name in allConfigurables:
            i += 1
            name = "GitOverlay_{}".format(i)
        # look for entries in path
        entries = []
        path = os.path.abspath(path)
        for root, dirs, files in os.walk(path):
            if '.git' in dirs:
                dirs.remove('.git')
            if 'IOVs' in files:
                entries.append(os.path.relpath(root, path))
                dirs[:] = []
            else:
                entries.extend(
                    os.path.relpath(os.path.join(root, f), path)
                    for f in files)
        resolver = GitEntityResolver(
            name,
            Ignore='^(?!({})(\\/.*)?$).*'.format('|'.join(
                re.escape(entry) for entry in entries)),
            PathToRepository=path,
            Commit='')
        XmlParserSvc().EntityResolver.EntityResolvers.insert(0, resolver)

    def _getLatestTag(self, partition, datatype):
        ger = allConfigurables.get('ToolSvc.Git{}'.format(partition))
        if ger:
            from GitCondDB.Tags import getTagsInfos
            dt = int(datatype) if datatype.isdigit() else datatype
            if not ger.PathToRepository:
                raise ValueError(
                    'CondDB._getLatestTag: {}: missing repository path'.format(
                        ger.name()))
            for tag, data in getTagsInfos(ger.PathToRepository).iteritems():
                if dt in data.info.get('datatypes', []):
                    return tag
            else:
                raise NoTagError(partition, datatype)
        else:
            raise ValueError(
                'cannot find Git CondDB configuration for %s' % partition)

    def _useLatestTags(self, DataTypes):
        """
        Configure the conditions database to use the latest global tag for a given data type.
        """
        # Check arguments
        if type(DataTypes) is not list:
            DataTypes = [DataTypes]

        # Check if the latest tags should be set for simulation or not
        if not self.getProp("Simulation"):
            partitions = ["DDDB", "LHCBCOND", "DQFLAGS"]
        else:
            partitions = ["DDDB", "SIMCOND"]

        # Set the latest tags
        for partition in partitions:
            tag = None
            for dt in DataTypes:
                try:
                    tag = self._getLatestTag(partition, dt)
                    break
                except NoTagError:
                    # DQFLAGS and CALIBOFF (before 2015) may not have valid tags
                    if partition != 'DQFLAGS':
                        raise

            if tag:  # Only for partitions with available global tags
                self.Tags[partition] = tag

    def __apply_configuration__(self):
        """
        Converts the high-level information passed as properties into low-level configuration.
        """
        # Set the usage of the latest global/local tags
        latest_GTags_prop = self.getProp("LatestGlobalTagByDataTypes")
        if not latest_GTags_prop:  # if property not set
            latest_GTags_prop = self.getProp("LatestGlobalTagByDataType")

        if latest_GTags_prop:
            datatype = latest_GTags_prop
            if self.getProp("Tags"):
                self.Tags = {}
            self._useLatestTags(datatype)
            log.warning(
                "Default global tags will be overridden with the latest ones"
                " available for '%s' data type: %s" % (datatype,
                                                       self.getProp("Tags")))

        requested_tags = self.getProp("Tags")
        for partition in requested_tags:
            m = re.match(r'<latest:(.*)>', requested_tags[partition])
            if m:  # the latest tag was explicitly requested for the partition
                requested_tags[partition] = self._getLatestTag(
                    partition, m.group(1))

        # In the Online environment, IgnoreHeartBeat should be defaulted to True
        self._properties["IgnoreHeartBeat"].setDefault(self.getProp("Online"))

        # GitONLINE is configured in DDDBConf, but it's better to check here
        # for backward compatibility
        if 'ToolSvc.GitONLINE' in allConfigurables:
            allConfigurables[
                'ToolSvc.GitONLINE'].LimitToLastCommitTime = not self.getProp(
                    "IgnoreHeartBeat")

        if not (self.getProp("Online") or self.getProp("Simulation")):
            self._properties["EnableRunStampCheck"].setDefault(True)
        if self.getProp("EnableRunStampCheck"):
            from Configurables import RunStampCheck
            rsc = RunStampCheck()
            if not rsc.isPropertySet('ValidRunsList'):
                rsc.ValidRunsList = 'git:///Conditions/Online/valid_runs.txt'
            ApplicationMgr().ExtSvc.append(rsc)

        for overlay in self.getProp("Overlays"):
            self._addOverlay(overlay)

        # Set up Virtual File System service, can be used by ParticlePropertySvc
        from Gaudi.Configuration import VFSSvc
        for partition in ['DDDB', 'LHCBCOND', 'SIMCOND', 'ONLINE', 'DQFLAGS']:
            ger = allConfigurables.get('ToolSvc.Git{0}'.format(partition))
            if ger:
                if not ger.isPropertySet('Commit'):
                    ger.Commit = self.getProp("Tags").get(partition, 'HEAD')
                    if (self.getProp('Upgrade') and
                            not (ger.Commit.startswith('upgrade/')
                                 or ger.Commit in ('', 'HEAD', 'upgrade') or
                                 re.match(r'^[0-9a-f]{7,40}$', ger.Commit))):
                        ger.Commit = 'upgrade/' + ger.Commit
                VFSSvc().FileAccessTools.append(ger)
