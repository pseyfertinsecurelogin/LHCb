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

import os, re
import GaudiTesting.QMTTest


class LHCbTest(GaudiTesting.QMTTest.QMTTest):
    '''Extension of the original QMTTest of Gaudi adding checking of the counters and histograms'''

    # default values found in the ref for old counters
    oldCountersDefaults = ["", "", "1.0000", "0.0000", "1.0000", "1.0000"]

    def _floatDiffer(self, ref, val, sensitivity):
        '''Compares 2 floats according to sensitivity'''
        val = float(val)
        ref = float(ref)
        if ref == 0.0:
            return abs(val) > sensitivity
        else:
            return abs(ref - val) / max(abs(ref), abs(val)) > sensitivity

    def validateWithReference(self,
                              stdout=None,
                              stderr=None,
                              result=None,
                              causes=None,
                              preproc=None,
                              counter_preproc=None,
                              sensitivities={}):
        '''Overwrite of the base class method by adding extra checks for counters.
           sensitivities allows to overwrite the default sensitivity of the counter checking (0.0001).
           It is containing a dictionnary with Algorithm name as key and a dictionnary as value
           having counter name a key and sensitivity for that counter as value'''
        # call upper class method
        super(LHCbTest, self).validateWithReference(stdout, stderr, result,
                                                    causes, preproc)
        # check the counters
        self._compare(stdout, causes, result, counter_preproc, 'Counters',
                      sensitivities)
        # check the 1D histograms
        self._compare(stdout, causes, result, counter_preproc, '1DHistograms',
                      sensitivities)
        # check the 1D profile histograms
        self._compare(stdout, causes, result, counter_preproc, '1DProfiles',
                      sensitivities)

    def _extract(self, s, counter_preproc=None, comp_type='Counters'):
        """
        Extract all counter lines from a string containing log lines.
        In case a counter preprocessor is given, it is applied to the counter lines.
        Then the lines are parsed and a dictionary is returned where key is the algorithm name and value a list of counters.
        Each counter itself is a list with first element being the counter name and the others counter's values, all as string
        """
        counters = {}
        if comp_type == 'Counters':
            counterStartRE = re.compile(
                '^(?P<algorithm>[^. ]*)[. ]+(?:SUCCESS|INFO) Number of counters : (?P<nCounters>\d+)$'
            )
            firstValueIndex = 1
        elif comp_type == '1DHistograms':
            counterStartRE = re.compile(
                '^(?P<algorithm>[^. ]*)[. ]+SUCCESS 1D histograms in directory (?:[^. ]*) : (?P<nCounters>\d+)$'
            )
            firstValueIndex = 2
        elif comp_type == '1DProfiles':
            counterStartRE = re.compile(
                '^(?P<algorithm>[^. ]*)[. ]+SUCCESS 1D profile histograms in directory (?:[^. ]*) : (?P<nCounters>\d+)$'
            )
            firstValueIndex = 2
        if counter_preproc:
            lines = counter_preproc(s)
        else:
            lines = s.split(os.linesep)
        n = 0
        while n < len(lines):
            # find counter block's header
            m = counterStartRE.match(lines[n].strip())
            n += 1
            if None == m: continue
            algoName = m.group('algorithm')
            # take care of the case where several algos have same name (problem being that names are cut when too long)
            if (algoName not in counters):
                counters[algoName] = {}
            # loop through counters (note +1 as we skip header lines of the block)
            for i in range(int(m.group('nCounters'))):
                # note the horrible hack to handle the fact that /Event/ is sometimes omited at the beginning of paths
                # on top master and future branch behave differently on that, so in order to keep a common reference,
                # we have to remove /Event/
                items = [
                    v.strip()
                    for v in lines[n + i +
                                   1].strip().replace('/Event/', '').split('|')
                    if v.strip() != ""
                ]
                counters[algoName][items[0].strip(
                    '"*')] = items[firstValueIndex:]
            n += i + 1
        return counters

    def _compareCounterLine(self, ref, value, comp_type, sensitivity):
        '''Compares 2 lines of a counter/histogram and check whether numbers are "close enough"'''

        # rough check
        if (ref == value):
            return True

        # Handle counters
        if comp_type == "Counters":

            # special case for efficiency counters
            # these lines contain ')%' and look like look like :
            #  | 10 | 10 |(  100.000 +- 10.0000  )%|...
            if len(ref) >= 3 and ')%' in ref[2]:
                # check only number and sum, the other numbers are only a combination of these
                if ref[0] != value[0]: return False
                if ref[1] != value[1]: return False
                return True
            else:
                # first check count. No sensitivity there and it's always present
                if ref[0] != value[0]: return False
                # now check number of values present
                if len(ref) != len(value):
                    # in case we are missing some compared to the ref, it may be due
                    # to the transition to new counters and no update of the ref.
                    # In that case, the missing parts must all have predefined values
                    # in the ref, except the second one (sum) that should be equal to
                    # first one (count)
                    if len(ref) > len(value) and len(ref) == 6:
                        if len(value) == 1 and ref[1] != ref[0]:
                            return False
                        for n in range(max(2, len(value)), 6):
                            if ref[n] != self.oldCountersDefaults[n]:
                                return False
                    else:
                        return False
                # and finally the rest of the values, with sensitivity
                for n in range(1, len(value)):
                    if self._floatDiffer(ref[n], value[n], sensitivity):
                        return False

        # Handle 1D Histos and Profiles
        if comp_type == '1DHistograms' or comp_type == '1DProfiles':

            # First value is count so check without precision
            if ref[0] != value[0]: return False
            # Check rest with given precision
            for n in range(1, len(value)):
                if self._floatDiffer(ref[n], value[n], sensitivity):
                    return False

        # If get here return OK
        return True

    def _compareCutSets(self, refCounters, stdoutCounters):
        """
        Compares two sets of counter names that may be incomplete and thus cut before the end !
        outputs a tuple containing:
            - the set of counters in ref and not in stdout, may be empty
            - the set of counters in stdout and not in ref, may be empty
            - a list of matching counters with pairs containing the name as seen in ref and the name as seen in stdout
        """
        onlyref = set([])
        donestdout = set([])
        counterPairs = []
        # go through names in ref and try to find them in stdout
        for refName in refCounters:
            # try full name first
            if refName in stdoutCounters:
                counterPairs.append((refName, refName))
                donestdout.add(refName)
                continue
            # suppose one of the 2 names was cut
            found = False
            for stdoutName in stdoutCounters:
                if stdoutName.startswith(refName) or refName.startswith(
                        stdoutName):
                    counterPairs.append((refName, stdoutName))
                    donestdout.add(stdoutName)
                    found = True
                    break
            if found:
                continue
            else:
                onlyref.add(refName)
        return onlyref, stdoutCounters - donestdout, counterPairs

    def _compare(self, stdout, causes, result, counter_preproc, comp_type,
                 sensitivities):
        """
        Compares values of counters/histograms to the reference file
        stdout: the test output
        causes: the usual QMTest causes
        result: the usual QMTest result
        """
        # get reference
        lreference = self._expandReferenceFileName(self.reference)
        if not (lreference and os.path.isfile(lreference)):
            return
        # extract counters from reference and stdout
        refCounters = self._extract(
            open(lreference).read(), counter_preproc, comp_type)
        newCounters = self._extract(stdout, counter_preproc, comp_type)
        # diff counters
        refAlgoNames = set(refCounters)
        newAlgoNames = set(newCounters)
        msg = ''
        if refAlgoNames != newAlgoNames:
            msg += 'Different set of algorithms in ' + comp_type + '\n'
            if refAlgoNames.difference(newAlgoNames):
                msg += '    Missing : ' + ', '.join(
                    refAlgoNames.difference(newAlgoNames)) + '\n'
            if newAlgoNames.difference(refAlgoNames):
                msg += '    Extra : ' + ', '.join(
                    newAlgoNames.difference(refAlgoNames)) + '\n'
            causes.append("Different set of algorithms in " + comp_type)
            # make sure we create newref file when there are only counters differences
            if causes:
                self._createNewRef(stdout)
        for algoName in refAlgoNames.intersection(newAlgoNames):
            onlyref, onlystdout, counterPairs = self._compareCutSets(
                set(refCounters[algoName]), set(newCounters[algoName]))
            if onlyref or onlystdout:
                msg += 'Different set of ' + comp_type + ' for algo %s\n' % algoName
                msg += ('    Ref has %d ' + comp_type +
                        ', found %d of them in stdout\n') % (len(
                            refCounters[algoName]), len(newCounters[algoName]))
                if onlyref:
                    msg += '    ' + comp_type + ' in ref and not in stdout : %s\n' % str(
                        sorted(list(onlyref)))
                if onlystdout:
                    msg += '    ' + comp_type + ' in stdout and not in ref : %s\n' % str(
                        sorted(list(onlystdout)))
            headerPrinted = False
            for counterNameRef, counterNameStdout in counterPairs:
                # floating point comparison precision
                sensitivity = 0.0001
                if algoName in sensitivities and counterNameRef in sensitivities[
                        algoName]:
                    sensitivity = sensitivities[algoName][counterNameRef]
                if not self._compareCounterLine(
                        refCounters[algoName][counterNameRef],
                        newCounters[algoName][counterNameStdout], comp_type,
                        sensitivity):
                    if not headerPrinted:
                        msg += 'Different content of ' + comp_type + ' for algo %s\n' % algoName
                        headerPrinted = True
                    msg += '    (%s ref) %s\n    (%s new) %s\n' % (
                        algoName,
                        ' | '.join([counterNameRef] +
                                   refCounters[algoName][counterNameRef]),
                        algoName,
                        ' | '.join([counterNameStdout] +
                                   newCounters[algoName][counterNameStdout]))
        if msg:
            causes.append("Wrong " + comp_type)
            if type(result) == dict:
                result[comp_type + "Mismatch"] = msg
            else:
                result[comp_type + "Mismatch"] = result.Quote(msg)
            # make sure we create newref file when there are only counters differences
            if causes:
                self._createNewRef(stdout)

    def _createNewRef(self, stdout):
        """
        Creates a new reference file
        stdout:     the test output
        """
        # get reference
        lreference = self._expandReferenceFileName(self.reference)
        if not (lreference and os.path.isfile(lreference)):
            return

        # this is copied from GaudiTest.py. Should be factorized out ideally
        try:
            newref = open(lreference + ".new", "w")
            # sanitize newlines
            for l in stdout.splitlines():
                newref.write(l.rstrip() + '\n')
            del newref  # flush and close
        except IOError:
            # Ignore IO errors when trying to update reference files
            # because we may be in a read-only filesystem
            pass
