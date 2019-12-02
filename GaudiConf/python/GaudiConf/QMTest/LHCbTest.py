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

import itertools
import os
import re
import six
from collections import OrderedDict
from itertools import takewhile

import GaudiTesting.QMTTest
from GaudiTesting.BaseTest import FilePreprocessor
from GaudiTesting.BaseTest import h_count_re as HISTO_SUMMARY_RE

from .LHCbExclusions import counter_preprocessor


def _get_new_ref_filename(reference_path):
    """Return the next available filename for a new reference."""
    count = 0
    newrefname = reference_path + '.new'
    while os.path.exists(newrefname):
        count += 1
        newrefname = '{}.~{}~.new'.format(reference_path, count)
    return newrefname


LOG_LINE_RE = re.compile(
    r'^(?P<component>[^. ]+(?:\.[^. ]+)*(?:\.\.\.)?)\s*'
    r'(?P<level>SUCCESS|VERBOSE|  DEBUG|   INFO|  ERROR|  FATAL)'
    r'\s*(?P<message>.*)$'
    # Note: the space after the message might be stripped if \n follows
)


class GroupMessages(FilePreprocessor):
    """Preprocessor that groups multi-line messages.

    Using this preprocessor only makes sense if it is called with input
    split by lines. If a string is given to `FilePreprocessor.__call__`,
    then the groups will be merged, making this a noop.

    """

    def __processFile__(self, lines):
        indices = [i for i, m in enumerate(map(LOG_LINE_RE.match, lines)) if m]
        if not indices:
            return []
        # everything until the first message is one message per line
        indices = range(indices[0]) + indices
        messages = [
            '\n'.join(lines[i1:i2])
            for i1, i2 in zip(indices, indices[1:] + [None])
        ]
        return messages


class BlockSkipper(FilePreprocessor):
    """Skip all lines between `start` and `end`.

    Skips blocks of lines identified by a starting line containing the
    substring `start` and an ending line containing the substring `end`.
    Both the starting and the ending line are excluded from the output.
    If `being` is `None`, lines starting from the beginning are skipped.
    If `end` is `None`, all lines until the end are skipped.

    """

    def __init__(self, start, end=None):
        self.start = start
        self.end = end

    def __processFile__(self, lines):
        skipping = self.start is None
        output_lines = []
        for line in lines:
            if self.start is not None and self.start in line:
                skipping = True
            if not skipping:
                output_lines.append(line)
            if self.end is not None and self.end in line:
                skipping = False
        return output_lines


def _extract_ttree_blocks(stdout):
    """Extract lines belonging to a TTree printout."""

    TTREE_START_RE = re.compile(r'^\*+$')
    TTREE_KEEP_RE = re.compile(r'^[^*].*')
    output_lines = []
    keep = False
    for line in stdout.splitlines(True):
        if TTREE_START_RE.match(line):
            keep = True
        if keep:
            output_lines.append(line)
            keep = TTREE_KEEP_RE.match(line)
    return ''.join(output_lines)


def _extract_histo_blocks(stdout):
    """Extract lines belonging to histo summary printout."""

    TABLE_START_RE = re.compile(r'.*SUCCESS\s+(1D|2D|3D|1D profile|2D profile)'
                                r' histograms in directory\s+"(\w*)".*')
    TABLE_KEEP_RE = re.compile(r'^({})|( \| .*)$')
    output_lines = []
    keep = False
    for line in stdout.splitlines(True):
        if HISTO_SUMMARY_RE.match(line):
            output_lines.append(line)
        if TABLE_START_RE.match(line):
            keep = True
        if keep:
            output_lines.append(line)
            keep = TABLE_KEEP_RE.match(line)
    return ''.join(output_lines)


_COMPONENT_RE = r'^(?P<component>[^. ]+(?:\.[^. ]+)*(?:\.\.\.)?)\s*'

_COUNTER_START_RE = {
    'Counters':
    (_COMPONENT_RE +
     r'(?:SUCCESS|   INFO) Number of counters : (?P<nCounters>\d+)$'),
    '1DHistograms':
    (_COMPONENT_RE +
     r'SUCCESS 1D histograms in directory (?:[^. ]*) : (?P<nCounters>\d+)$'),
    '1DProfiles':
    (_COMPONENT_RE +
     r'SUCCESS 1D profile histograms in directory (?:[^. ]*) : (?P<nCounters>\d+)$'
     ),
}


def _extract_counter_blocks(s, header_pattern, preproc=None):
    """Extract all counter lines from a string containing log lines.

    In case a counter preprocessor is given, it is applied to the counter lines.

    """

    if preproc:
        s = preproc(s)
    lines = s.splitlines() if isinstance(s, six.string_types) else s

    blocks = OrderedDict()

    header_re = re.compile(header_pattern)

    # find counter block headers
    headers = filter(lambda x: bool(x[1]),
                     enumerate(map(header_re.match, lines)))
    for i, m in headers:
        component = m.group('component')

        # take care of the case where several algos have same name
        # (problem being that names are cut when too long)
        if component in blocks:
            candidates = (component + str(i) for i in itertools.count(1))
            component = next(c for c in candidates if c not in blocks)

        blocks[component] = lines[i:(i + int(m.group('nCounters')) + 2)]
        # We might get less than nCounters as some lines might be filtered
        # out by the counter preprocessor. Only keep what is left:
        blocks[component] = blocks[component][:1] + list(
            takewhile(re.compile(r'^ \|[* ].*$').match, blocks[component][1:]))

    return blocks


def _extract_counters(blocks, comp_type='Counters'):
    """
    Then the lines are parsed and a dictionary is returned where key is the algorithm name and value a list of counters.
    Each counter itself is a list with first element being the counter name and the others counter's values, all as string
    """
    counters = {}
    firstValueIndex = 1 if comp_type == 'Counters' else 2
    for component, lines in blocks.items():
        assert re.match(_COUNTER_START_RE[comp_type], lines[0])
        counters[component] = {}
        # exclude the first two lines which contain message and table header
        for line in lines[2:]:
            # note the horrible hack to handle the fact that /Event/
            # is sometimes omited at the beginning of paths on top
            # master and future branch behave differently on that,
            # so in order to keep a common reference, we have to
            # remove /Event/
            items = [
                v.strip()
                for v in line.strip().replace('/Event/', '').split('|')
                if v.strip() != ""
            ]
            counters[component][items[0].strip('"*')] = items[firstValueIndex:]
    return counters


def extract_counters(s, counter_preproc=None, comp_type='Counters'):
    """Parse counter table in a string containing log lines."""
    blocks = _extract_counter_blocks(
        s, _COUNTER_START_RE[comp_type], preproc=counter_preproc)
    return _extract_counters(blocks, comp_type=comp_type)


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

    def ValidateOutput(self, stdout, stderr, result):
        try:
            return super(LHCbTest, self).ValidateOutput(stdout, stderr, result)
        except:
            import traceback
            self.causes.append("Exception in validator")
            result["validator_exception"] = result.Quote(
                traceback.format_exc().rstrip())
            return result, self.causes

    def validateWithReference(self,
                              stdout=None,
                              stderr=None,
                              result=None,
                              causes=None,
                              preproc=None,
                              counter_preproc=counter_preprocessor,
                              sensitivities={}):
        '''Overwrite of the base class method by adding extra checks for counters.
           sensitivities allows to overwrite the default sensitivity of the counter checking (0.0001).
           It is containing a dictionnary with Algorithm name as key and a dictionnary as value
           having counter name a key and sensitivity for that counter as value'''

        if stdout is None:
            stdout = self.out
        if stderr is None:
            stderr = self.err
        if result is None:
            result = self.result
        if causes is None:
            causes = self.causes

        # call upper class method
        super(LHCbTest, self).validateWithReference(stdout, stderr, result,
                                                    causes, preproc)

        # get reference
        lreference = self._expandReferenceFileName(self.reference)
        if not lreference or not os.path.isfile(lreference):
            return

        # extract counters from reference and stdout
        with open(lreference) as f:
            ref = f.read()
        ref_blocks = {
            comp_type: _extract_counter_blocks(
                ref, pattern, preproc=counter_preproc)
            for comp_type, pattern in _COUNTER_START_RE.items()
        }

        blocks = {
            comp_type: _extract_counter_blocks(
                stdout, pattern, preproc=counter_preproc)
            for comp_type, pattern in _COUNTER_START_RE.items()
        }

        for comp_type in ['Counters', '1DHistograms', '1DProfiles']:
            self._compare(blocks[comp_type], ref_blocks[comp_type], causes,
                          result, comp_type, sensitivities)

        if causes:
            try:
                newrefname = os.path.join(self.basedir,
                                          result['New Output Reference File'])
            except KeyError:
                newrefname = _get_new_ref_filename(lreference)

            with open(newrefname, "w") as newref:
                # sanitize newlines
                new = stdout.splitlines()
                # write the preprocessed stdout (excludes counter tables)
                if preproc:
                    new = preproc(new)
                for l in new:
                    newref.write(l.rstrip() + '\n')
                # write counter tables
                for ct_blocks in blocks.values():
                    for lines in ct_blocks.values():
                        for line in lines:
                            newref.write(line + '\n')
                # write TTrees
                newref.write(_extract_ttree_blocks(stdout))
                # write Histos
                newref.write(_extract_histo_blocks(stdout))

            result['New Output Reference File'] = os.path.relpath(
                newrefname, self.basedir)

    def _compareCounterLine(self, ref, value, comp_type, sensitivity):
        '''Compares 2 lines of a counter/histogram and check whether numbers are "close enough"'''

        # rough check
        if (ref == value):
            return True

        # Handle counters
        if comp_type == "Counters":

            # special case for efficiency counters
            # these lines contain ')%' and look like :
            #  | 10 | 10 |(  100.000 +- 10.0000  )%|...
            if len(ref) >= 3 and ')%' in ref[2]:
                # check only number and sum, the other numbers are only a combination of these
                if ref[0] != value[0]: return False
                if ref[1] != value[1]: return False
                return True
            elif len(value) >= 3 and ')%' in value[2]:
                # we got efficiency counters when the ref has regular ones...
                return False
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

    def _compare(self, blocks, ref_blocks, causes, result, comp_type,
                 sensitivities):
        """
        Compares values of counters/histograms to the reference file
        blocks: a dict of {component: list of lines}
        causes: the usual QMTest causes
        result: the usual QMTest result
        """
        refCounters = _extract_counters(ref_blocks, comp_type=comp_type)
        newCounters = _extract_counters(blocks, comp_type=comp_type)
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
