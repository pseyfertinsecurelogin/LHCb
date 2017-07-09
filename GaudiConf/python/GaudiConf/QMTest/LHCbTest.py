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
                counters[algoName] = []
            # loop through counters (note +1 as we skip header lines of the block)
            for i in range(int(m.group(2))):
                # note the horrible hack to handle the fact that /Event/ is sometimes omited at the beginning of paths
                # on top master and future branch behave differently on that, so in order to keep a common reference,
                # we have to remove /Event/
                counters[algoName].append([v.strip() for v in lines[n+i+1].strip().replace('/Event/','').split('|') if v.strip() != ""])
            n += i+1
        return counters

    def _compareCounterLine(self, ref, value):
        '''Compares 2 lines of a counter and check whether numbers are "close enough"'''
        # rough check
        if (ref == value):
            return True
        sensibility = 0.0001
        if len(ref) != 7:
            # special case where no mean/max are given and only 6 columns are present
            # these lines look like :
            #  |*"Accepted events"  | 10 | 10 |(  100.000 +- 10.0000  )%| ------- | ------- |
            if len(ref) == 6 and ref[-1] == '-------':
                # check only number and sum
                if ref[1] != value[1]: return False
                if _floatDiffer(ref[2], value[2], sensibility): return False
                return True
            else:
                return False
        # more in depth check, parsing numbers and comparing with given sensibility
        refName, refNb, refSum, refMean, refRMS, refMin, refMax = ref
        valName, valNb, valSum, valMean, valRMS, valMin, valMax = value
        if refNb != valNb: return False
        if _floatDiffer(refSum, valSum, sensibility): return False
        if _floatDiffer(refMean, valMean, sensibility): return False
        if _floatDiffer(refRMS, valRMS, sensibility): return False
        if _floatDiffer(refMin, valMin, sensibility): return False
        if _floatDiffer(refMax, valMax, sensibility): return False
        return True

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
            if len(refCounters[algoName]) != len(newCounters[algoName]):
                msg += '    Different number of counters for algo %s\n' % algoName
                msg += '      Expected to check %d, found %d of them\n' % (len(refCounters[algoName]), len(newCounters[algoName]))
                msg += '      Reference list : %s\n' % str([a[0] for a in refCounters[algoName]])
                msg += '      New list : %s\n' % str([a[0] for a in newCounters[algoName]])
                continue
            for n in range(len(refCounters[algoName])):
                if not self._compareCounterLine(refCounters[algoName][n], newCounters[algoName][n]):
                    msg += '(%s ref) %s\n(%s new) %s\n' % (algoName, ' | '.join(refCounters[algoName][n]), algoName, ' | '.join(newCounters[algoName][n]))
        if msg:
            causes.append("Wrong Counters")
            if type(result) == dict:
                result["CountersMismatch"]=msg
            else:
                result["CountersMismatch"]=result.Quote(msg)

