import sys
import tempfile
from ConfigurablesTest import *

OK = '\033[92mOK\033[0m'
FAILED = '\033[91mFAILED\033[0m'

# input data
refinput = '''DDDB.TimeOutChe...   INFO Disconnect from database after being idle for 120s (will reconnect if needed)
ApplicationMgr       INFO Application Manager Stopped successfully
createUTClusters     INFO Successfully processed 100 %
createUTClusters  SUCCESS Number of counters : 3
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# banks found"                                 |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "# valid banks"                                 |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "skipped Banks"                                 |         0 |          0 |     0.0000 |     0.0000 | 1.7977e+308 |-1.7977e+308 |
createUTLiteClu...   INFO Successfully processed 100 %
createUTLiteClu...SUCCESS Number of counters : 3
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# valid banks"                                 |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "# valid source ID"                             |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "skipped Banks"                                 |         0 |          0 |     0.0000 |     0.0000 | 1.7977e+308 |-1.7977e+308 |
PrPixelTracking...SUCCESS Booked 26 Histogram(s) : 1D=14 2D=10 3D=2 
PrVeloUTFast      SUCCESS Number of counters : 2
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "#seeds"                                        |       100 |      24227 |     242.27 |     129.32 |      37.000 |      631.00 |
 | "#tracks"                                       |       100 |       7189 |     71.890 |     45.104 |      6.0000 |      234.00 |
ForwardFitterAl...   INFO   Fitting performance   :   100.00 %
ForwardFitterAl...SUCCESS Number of counters : 7
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "Long.badChisq"                                 |      1534 |       1055 |    0.68774 |    0.46341 |      0.0000 |      1.0000 |
 | "Long.chisqprobSum"                             |      1534 |   91.86575 |   0.059886 |    0.15307 |      0.0000 |     0.98345 |
 | "Long.flipCharge"                               |      1534 |          1 | 0.00065189 |   0.025524 |      0.0000 |      1.0000 |
 | "Long.numOutliers"                              |      1534 |       2142 |     1.3963 |    0.77654 |      0.0000 |      2.0000 |
 | "nBadInput"                                     |       100 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "nFitted"                                       |       100 |       1534 |     15.340 |     10.650 |      0.0000 |      52.000 |
 | "nTracks"                                       |       100 |       1534 |     15.340 |     10.650 |      0.0000 |      52.000 |
TrackBestTrackC...   INFO   Fitting performance   :   100.00 %
TrackBestTrackC...SUCCESS Number of counters : 15
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "BadInput"                                      |     25791 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "FitFailed"                                     |     25791 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "FittedBefore"                                  |      1528 |       1528 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "Upstream.badChisq"                             |      6098 |       1638 |    0.26861 |    0.44324 |      0.0000 |      1.0000 |
 | "Upstream.chisqprobSum"                         |      6098 |   2069.539 |    0.33938 |    0.33160 |      0.0000 |      1.0000 |
 | "Upstream.flipCharge"                           |      6098 |        448 |   0.073467 |    0.26090 |      0.0000 |      1.0000 |
 | "Upstream.numOutliers"                          |      6098 |       3557 |    0.58331 |    0.80505 |      0.0000 |      2.0000 |
 | "Velo.badChisq"                                 |      9452 |       1772 |    0.18747 |    0.39029 |      0.0000 |      1.0000 |
 | "Velo.chisqprobSum"                             |      9452 |   2968.502 |    0.31406 |    0.31430 |      0.0000 |      1.0000 |
 | "Velo.flipCharge"                               |      9452 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "Velo.numOutliers"                              |      9452 |       4205 |    0.44488 |    0.74388 |      0.0000 |      2.0000 |
 | "VeloBackward.badChisq"                         |      8713 |        230 |   0.026397 |    0.16031 |      0.0000 |      1.0000 |
 | "VeloBackward.chisqprobSum"                     |      8713 |   4136.634 |    0.47477 |    0.31212 |      0.0000 |      1.0000 |
 | "VeloBackward.flipCharge"                       |      8713 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "VeloBackward.numOutliers"                      |      8713 |       1123 |    0.12889 |    0.40994 |      0.0000 |      2.0000 |
RichFutureDecod...SUCCESS Booked 352 Histogram(s) : 2D=1 1DProf=351 
RichFutureDetec...SUCCESS Booked 6 Histogram(s) : 1D=6 
RichFuturePixel...SUCCESS Booked 2 Histogram(s) : 1D=2 
RichFutureRecoS...   INFO ===========================================================================
RichFutureRecoS...   INFO                  RichFutureRecoStatsDown Summary : 100 events
RichFutureRecoS...   INFO ---------------------------------------------------------------------------
RichFutureRecoS...   INFO     Selected     0.00 +- 0.00 tracks/event : RICH eff   0.00 +- 0.00 % 
'''

# Test parsing
expCounters = {'ForwardFitterAl': [['"Long.badChisq"', '1534', '1055', '0.68774', '0.46341', '0.0000', '1.0000'], ['"Long.chisqprobSum"', '1534', '91.86575', '0.059886', '0.15307', '0.0000', '0.98345'], ['"Long.flipCharge"', '1534', '1', '0.00065189', '0.025524', '0.0000', '1.0000'], ['"Long.numOutliers"', '1534', '2142', '1.3963', '0.77654', '0.0000', '2.0000'], ['"nBadInput"', '100', '0', '0.0000', '0.0000', '0.0000', '0.0000'], ['"nFitted"', '100', '1534', '15.340', '10.650', '0.0000', '52.000'], ['"nTracks"', '100', '1534', '15.340', '10.650', '0.0000', '52.000']], 'PrVeloUTFast': [['"#seeds"', '100', '24227', '242.27', '129.32', '37.000', '631.00'], ['"#tracks"', '100', '7189', '71.890', '45.104', '6.0000', '234.00']], 'createUTLiteClu': [['"# valid banks"', '18000', '18000', '1.0000', '0.0000', '1.0000', '1.0000'], ['"# valid source ID"', '18000', '18000', '1.0000', '0.0000', '1.0000', '1.0000'], ['"skipped Banks"', '0', '0', '0.0000', '0.0000', '1.7977e+308', '-1.7977e+308']], 'createUTClusters': [['"# banks found"', '18000', '18000', '1.0000', '0.0000', '1.0000', '1.0000'], ['"# valid banks"', '18000', '18000', '1.0000', '0.0000', '1.0000', '1.0000'], ['"skipped Banks"', '0', '0', '0.0000', '0.0000', '1.7977e+308', '-1.7977e+308']], 'TrackBestTrackC': [['"BadInput"', '25791', '0', '0.0000', '0.0000', '0.0000', '0.0000'], ['"FitFailed"', '25791', '0', '0.0000', '0.0000', '0.0000', '0.0000'], ['"FittedBefore"', '1528', '1528', '1.0000', '0.0000', '1.0000', '1.0000'], ['"Upstream.badChisq"', '6098', '1638', '0.26861', '0.44324', '0.0000', '1.0000'], ['"Upstream.chisqprobSum"', '6098', '2069.539', '0.33938', '0.33160', '0.0000', '1.0000'], ['"Upstream.flipCharge"', '6098', '448', '0.073467', '0.26090', '0.0000', '1.0000'], ['"Upstream.numOutliers"', '6098', '3557', '0.58331', '0.80505', '0.0000', '2.0000'], ['"Velo.badChisq"', '9452', '1772', '0.18747', '0.39029', '0.0000', '1.0000'], ['"Velo.chisqprobSum"', '9452', '2968.502', '0.31406', '0.31430', '0.0000', '1.0000'], ['"Velo.flipCharge"', '9452', '0', '0.0000', '0.0000', '0.0000', '0.0000'], ['"Velo.numOutliers"', '9452', '4205', '0.44488', '0.74388', '0.0000', '2.0000'], ['"VeloBackward.badChisq"', '8713', '230', '0.026397', '0.16031', '0.0000', '1.0000'], ['"VeloBackward.chisqprobSum"', '8713', '4136.634', '0.47477', '0.31212', '0.0000', '1.0000'], ['"VeloBackward.flipCharge"', '8713', '0', '0.0000', '0.0000', '0.0000', '0.0000'], ['"VeloBackward.numOutliers"', '8713', '1123', '0.12889', '0.40994', '0.0000', '2.0000']]}
counters = extractCounters(refinput)
if counters == expCounters:
    print 'Testing counter extraction : ' + OK
else:
    print 'Testing counter extraction : ' + FAILED
    print '  Expected :'
    print '    %s' % str(expCounters)
    print '  Got :'
    print '    %s' % str(counters)
    sys.exit(-1)

# Test selection
expSelectedCounters = {'ForwardFitterAl': [['"nFitted"', '100', '1534', '15.340', '10.650', '0.0000', '52.000'], ['"nTracks"', '100', '1534', '15.340', '10.650', '0.0000', '52.000']], 'createUTLiteClu': [['"# valid banks"', '18000', '18000', '1.0000', '0.0000', '1.0000', '1.0000'], ['"# valid source ID"', '18000', '18000', '1.0000', '0.0000', '1.0000', '1.0000'], ['"skipped Banks"', '0', '0', '0.0000', '0.0000', '1.7977e+308', '-1.7977e+308']]}
selection = ("createUTLiteClusters", ("ForwardFitterAlgorithm", ("nFitted", "nTracks")), "NonExistant")
selectedCounters = selectCounters(counters, selection)
if selectedCounters == expSelectedCounters:
    print 'Testing counter selection : ' + OK
else:
    print 'Testing counter selection : ' + FAILED
    print '  Expected :'
    print '    %s' % str(expSelectedCounters)
    print '  Got :'
    print '    %s' % str(selectedCounters)
    sys.exit(-1)

# Test checks, given as name, input, causes expected, results expected
goodInput = 'Good input', '''
createUTLiteClu...SUCCESS Number of counters : 3
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# valid banks"                                 |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "# valid source ID"                             |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "skipped Banks"                                 |         0 |          0 |     0.0000 |     0.0000 | 1.7977e+308 |-1.7977e+308 |
PrPixelTracking...SUCCESS Booked 26 Histogram(s) : 1D=14 2D=10 3D=2 
ForwardFitterAl...SUCCESS Number of counters : 4
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "Long.numOutliers"                              |      1534 |       2142 |     1.3963 |    0.77654 |      0.0000 |      2.0000 |
 | "nBadInput"                                     |       100 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "nFitted"                                       |       100 |       1534 |     15.340 |     10.650 |      0.0000 |      52.000 |
 | "nTracks"                                       |       100 |       1534 |     15.340 |     10.650 |      0.0000 |      52.000 |
''', [], {}

missingAlgoInput = 'One algo missing', '''
createUTLiteClu...SUCCESS Number of counters : 3
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# valid banks"                                 |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "# valid source ID"                             |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "skipped Banks"                                 |         0 |          0 |     0.0000 |     0.0000 | 1.7977e+308 |-1.7977e+308 |
''', ['Different set of algorithms in counters'], {'CountersMismatch': '    Missing : ForwardFitterAl'}

tooMuchAlgoInput = 'One algo too much', '''
createUTLiteClu...SUCCESS Number of counters : 3
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# valid banks"                                 |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "# valid source ID"                             |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "skipped Banks"                                 |         0 |          0 |     0.0000 |     0.0000 | 1.7977e+308 |-1.7977e+308 |
PrPixelTracking...SUCCESS Booked 26 Histogram(s) : 1D=14 2D=10 3D=2 
ForwardFitterAl...SUCCESS Number of counters : 4
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "Long.numOutliers"                              |      1534 |       2142 |     1.3963 |    0.77654 |      0.0000 |      2.0000 |
 | "nBadInput"                                     |       100 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "nFitted"                                       |       100 |       1534 |     15.340 |     10.650 |      0.0000 |      52.000 |
 | "nTracks"                                       |       100 |       1534 |     15.340 |     10.650 |      0.0000 |      52.000 |
NonExistant       SUCCESS Number of counters : 1
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# valid banks"                                 |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
''', ['Different set of algorithms in counters'], {'CountersMismatch': '    Extra : NonExistant'}

missingCounterInput = 'One counter missing', '''
createUTLiteClu...SUCCESS Number of counters : 3
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# valid banks"                                 |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "# valid source ID"                             |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "skipped Banks"                                 |         0 |          0 |     0.0000 |     0.0000 | 1.7977e+308 |-1.7977e+308 |
PrPixelTracking...SUCCESS Booked 26 Histogram(s) : 1D=14 2D=10 3D=2 
ForwardFitterAl...SUCCESS Number of counters : 3
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "Long.numOutliers"                              |      1534 |       2142 |     1.3963 |    0.77654 |      0.0000 |      2.0000 |
 | "nBadInput"                                     |       100 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "nFitted"                                       |       100 |       1534 |     15.340 |     10.650 |      0.0000 |      52.000 |
''', ['Wrong Counters'], {'CountersMismatch': '    Different number of counters for algo ForwardFitterAl\n      Expected to check 2, found 1 of them'}

tooMuchCounterInput = 'One counter too much', '''
createUTLiteClu...SUCCESS Number of counters : 4
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# valid banks"                                 |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "# valid source ID"                             |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "skipped Banks"                                 |         0 |          0 |     0.0000 |     0.0000 | 1.7977e+308 |-1.7977e+308 |
 | "Velo.flipCharge"                               |      9452 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
PrPixelTracking...SUCCESS Booked 26 Histogram(s) : 1D=14 2D=10 3D=2 
ForwardFitterAl...SUCCESS Number of counters : 4
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "Long.numOutliers"                              |      1534 |       2142 |     1.3963 |    0.77654 |      0.0000 |      2.0000 |
 | "nBadInput"                                     |       100 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "nFitted"                                       |       100 |       1534 |     15.340 |     10.650 |      0.0000 |      52.000 |
 | "nTracks"                                       |       100 |       1534 |     15.340 |     10.650 |      0.0000 |      52.000 |
''', ['Wrong Counters'], {'CountersMismatch': '    Different number of counters for algo createUTLiteClu\n      Expected to check 3, found 4 of them'}

wrongInput = 'Wrong input', '''
createUTLiteClu...SUCCESS Number of counters : 3
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "# valid banks"                                 |     18000 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "# valid source ID"                             |     17999 |      18000 |     1.0000 |     0.0000 |      1.0000 |      1.0000 |
 | "skipped Banks"                                 |         0 |          0 |     0.0000 |     0.0000 | 1.7977e+308 |-1.7977e+308 |
PrPixelTracking...SUCCESS Booked 26 Histogram(s) : 1D=14 2D=10 3D=2 
ForwardFitterAl...SUCCESS Number of counters : 4
 |    Counter                                      |     #     |    sum     | mean/eff^* | rms/err^*  |     min     |     max     |
 | "Long.numOutliers"                              |      1534 |       2142 |     1.3963 |    0.77454 |      0.0000 |      2.0000 |
 | "nBadInput"                                     |       100 |          0 |     0.0000 |     0.0000 |      0.0000 |      0.0000 |
 | "nFitted"                                       |       100 |       1534 |     15.340 |     10.600 |      0.0000 |      52.000 |
 | "nTracks"                                       |       100 |       1534 |     15.340 |     10.650 |      0.0000 |      52.000 |
''', ['Wrong Counters'], {'CountersMismatch': '(ForwardFitterAl ref) "nFitted" | 100 | 1534 | 15.340 | 10.650 | 0.0000 | 52.000\n(ForwardFitterAl new) "nFitted" | 100 | 1534 | 15.340 | 10.600 | 0.0000 | 52.000\n(createUTLiteClu ref) "# valid source ID" | 18000 | 18000 | 1.0000 | 0.0000 | 1.0000 | 1.0000\n(createUTLiteClu new) "# valid source ID" | 17999 | 18000 | 1.0000 | 0.0000 | 1.0000 | 1.0000\n'}

for inputName, inputString, expCauses, expResults in (goodInput, missingAlgoInput, tooMuchAlgoInput, missingCounterInput, tooMuchCounterInput, wrongInput):
    with tempfile.NamedTemporaryFile() as file:
        file.write(refinput)
        file.flush()
        causes = []
        results = {}
        compareCounters(inputString, file.name, causes, results, selection)
        if expCauses == causes and expResults == results:
            print 'Testing %s: %s' % (inputName, OK)
        else:
            print 'Testing %s: %s' % (inputName, FAILED)
            print '  Expected cause :'
            for cause in expCauses:
                print '    %s' % cause
            print '  Expected results :'
            for name in expResults:
                print '%s : %s' % (name, expResults[name])
            print '  Cause :'
            for cause in causes:
                print '    %s' % cause
            print '  Results :'
            for name in results:
                print '%s : %s' % (name, results[name])
