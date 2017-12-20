import os
import re
import GaudiTesting.QMTTest

def _floatDiffer(ref, val, sensibility):
    '''Compares 2 floats according to sensibility'''
    val = float(val)
    ref = float(ref)
    if ref == 0.0:
        return abs(val) > sensibility
    else:
        return abs(ref-val)/max(abs(ref),abs(val)) > sensibility

class LHCbTest(GaudiTesting.QMTTest.QMTTest):
    '''Extension of the original QMTTest of Gaudi adding checking of the counters'''

    def validateWithReference(self, stdout=None, stderr=None, result=None,
                              causes=None, preproc=None, counter_preproc=None):
        '''Overwrite of the base class method by adding extra checks for counters'''
        # call upper class method
        super(LHCbTest, self).validateWithReference(stdout, stderr, result, causes, preproc)
        # check the counters
        self._compareCounters(stdout, causes, result, counter_preproc)

    def _extractCounters(self, s, counter_preproc=None):
        """
        Extract all counter lines from a string containing log lines.
        In case a counter preprocessor is given, it is applied to the counter lines.
        Then the lines are parsed and a dictionnary is returned where key is the algorithm name and value a list of counters.
        Each counter itself is a list with first element being the counter name and the others counter's values, all as string
        """
        counters = {}
        counterStartRE = re.compile('^([^. ]*)[. ]+SUCCESS Number of counters : (\d+)$')
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
            algoName = m.group(1)
            # take care of the case where several algos have same name (problem being that names are cut when too long)
            if (algoName not in counters):
                counters[algoName] = {}
            # loop through counters (note +1 as we skip header lines of the block)
            for i in range(int(m.group(2))):
                # note the horrible hack to handle the fact that /Event/ is sometimes omited at the beginning of paths
                # on top master and future branch behave differently on that, so in order to keep a common reference,
                # we have to remove /Event/
                items = [v.strip() for v in lines[n+i+1].strip().replace('/Event/','').split('|') if v.strip() != ""]
                counters[algoName][items[0]] = items[1:]
            n += i+1
        return counters

    def _compareCounterLine(self, ref, value):
        '''Compares 2 lines of a counter and check whether numbers are "close enough"'''
        # rough check
        if (ref == value):
            return True
        if len(ref) != len(value):
            return False
        sensibility = 0.0001
        if len(ref) != 6:
            # special case where no mean/max are given and only 6 columns are present
            # these lines look like :
            #  | 10 | 10 |(  100.000 +- 10.0000  )%| ------- | ------- |
            if len(ref) == 5 and ref[-1] == '-------':
                # check only number and sum
                if ref[0] != value[0]: return False
                if _floatDiffer(ref[1], value[1], sensibility): return False
                return True
            else:
                return False
        # more in depth check, parsing numbers and comparing with given sensibility
        refNb, refSum, refMean, refRMS, refMin, refMax = ref
        valNb, valSum, valMean, valRMS, valMin, valMax = value
        if refNb != valNb: return False
        if _floatDiffer(refSum, valSum, sensibility): return False
        if _floatDiffer(refMean, valMean, sensibility): return False
        if _floatDiffer(refRMS, valRMS, sensibility): return False
        if _floatDiffer(refMin, valMin, sensibility): return False
        if _floatDiffer(refMax, valMax, sensibility): return False
        return True

    def _compareCutSets(self, refCounters, stdoutCounters):
        """
        Compares 2 set of counter names that may be incomplete and thus cut befoer the end !
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
                if stdoutName.startswith(refName) or refName.startswith(stdoutName):
                    counterPairs.append((refName, stdoutName))
                    donestdout.add(stdoutName)
                    found = True
                    break
            if found:
                continue
            else:
                onlyref.add(refName)
        return onlyref, stdoutCounters-donestdout, counterPairs

    def _compareCounters(self, stdout, causes, result, counter_preproc):
        """
        Compares values of counters to the reference file
        stdout: the test output
        causes: the usual QMTest causes
        result: the usual QMTest result
        """
        # get reference
        lreference = self._expandReferenceFileName(self.reference)
        if not (lreference and os.path.isfile(lreference)):
            return
        # extract counters from reference and stdout
        refCounters = self._extractCounters(open(lreference).read(), counter_preproc)
        newCounters = self._extractCounters(stdout, counter_preproc)
        # diff counters
        refAlgoNames = set(refCounters)
        newAlgoNames = set(newCounters)
        if refAlgoNames != newAlgoNames:
            msg = ''
            if refAlgoNames.difference(newAlgoNames):
                msg += '    Missing : ' + ', '.join(refAlgoNames.difference(newAlgoNames))
            if newAlgoNames.difference(refAlgoNames):
                msg += '    Extra : ' + ', '.join(newAlgoNames.difference(refAlgoNames))
            causes.append("Different set of algorithms in counters")
            if type(result) == dict:
                result["CountersMismatch"]=msg
            else:
                result["CountersMismatch"]=result.Quote(msg)
            return
        msg = ''
        for algoName in refAlgoNames:
            onlyref, onlystdout, counterPairs = self._compareCutSets(set(refCounters[algoName]), set(newCounters[algoName]))
            if onlyref or onlystdout:
                msg += '    Different set of counters for algo %s\n' % algoName
                msg += '      Ref has %d counters, found %d of them in stdout\n' % (len(refCounters[algoName]), len(newCounters[algoName]))
                if onlyref:
                    msg += '      Counters in ref and not in stdout : %s\n' % str(sorted(list(onlyref)))
                if onlystdout:
                    msg += '      Counters in stdout and not in ref : %s\n' % str(sorted(list(onlystdout)))
                continue
            for counterNameRef, counterNameStdout in counterPairs:
                if not self._compareCounterLine(refCounters[algoName][counterNameRef], newCounters[algoName][counterNameStdout]):
                    msg += '(%s ref) %s\n(%s new) %s\n' % (algoName, ' | '.join([counterNameRef]+refCounters[algoName][counterNameRef]), algoName, ' | '.join([counterNameStdout]+newCounters[algoName][counterNameStdout]))
        if msg:
            causes.append("Wrong Counters")
            if type(result) == dict:
                result["CountersMismatch"]=msg
            else:
                result["CountersMismatch"]=result.Quote(msg)
