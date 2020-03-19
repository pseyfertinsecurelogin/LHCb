# Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
# Author: Paul Seyfert <pseyfert@cern.ch>
#
# This software is distributed under the terms of the GNU General Public
# Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
#
# In applying this licence, CERN does not waive the privileges and immunities
# granted to it by virtue of its status as an Intergovernmental Organization
# or submit itself to any jurisdiction.

# A barrier consists of 4 steps:
#
# 1+2: A gatherer and a merger. The gatherer reading data from a list of
# location and stores un-merged in an intermediate location. The merger merges
# the data properly. With ZipSelections, the gatherer stores a vector of
# ExportedSelection*s* in the intermediate location, the merger stores *one*
# ExportedSelection.
#
# 3: A transformer, that does actual work. I.e. for all selected tracks,
# compute "the isolation", compute RichPID, ... , in general, perform any
# operation that is computationally so expensive that we do not want to perform
# it on all input objects, but that will also be a common task among multiple
# trigger/prucing lines.
#
# 4: A scatterer, that distributes the output from the *one* output of the
# transformer back to all lines, such that each line receives only the
# transformed output of what they requested (the RichPIDs for the tracks it
# selected, not of all other tracks some other line selected). In the case of
# the ExportedSelection, such a scatterer is not needed.
#
# See also:
# https://indico.cern.ch/event/797774/contributions/3316748/attachments/1803283/2941932/LHCC_19_final.pdf

#  * In this example, a bunch of integers is Produced by
#    ZipBarrierExampleProducer.
#  * A bunch of ZipBarrierExampleSelector*s* select the integers they are
#   interested in (in this simple example, numbers that are divisible by some
#   Divisor are selected).
# * The "expensive" task is squaring the numbers, as done by
#   ZipBarrierExampleWorker
# * PrintInts, PrintSquaredInts, PrintIntsAndSquaredInts print the data of various
#   selections.
#
# What a reader should ideally understand:
# * How to configure a ZipBarrierGatherer and ZipBarrierMerger
# * How to use the output of a transformer together with the pre-existing
#   ExportedSelection to process only selected output from the transformer
#   (i.e. use a single ExportedSelection throughout a sprucing/trigger line
#   with changing data columns)

from Configurables import Examples__ZipBarrierExampleSelector as ZipBarrierExampleSelector
from Configurables import Examples__PrintInts as PrintInts
from Configurables import Examples__PrintSquaredInts as PrintSquaredInts
from Configurables import Examples__PrintIntsAndSquaredInts as PrintIntsAndSquaredInts
from Configurables import Examples__ZipBarrierExampleProducer as ZipBarrierExampleProducer
from Configurables import Examples__ZipBarrierExampleWorker as ZipBarrierExampleWorker
from Configurables import ZipBarrierGatherer, ZipBarrierMerger
from Gaudi.Configuration import *

# Application setup
app = ApplicationMgr()
# - Algorithms
prod = ZipBarrierExampleProducer()
prod.OutputLocation = "/Event/OriginalVector"
prod.OutputSelection = "/Event/AllSelection"

sel1 = ZipBarrierExampleSelector("selectall")
sel1.Divisor = 1
sel1.OutputSelection = "/Event/Select_1"

sel2 = ZipBarrierExampleSelector("selecteven")
sel2.Divisor = 2
sel2.OutputSelection = "/Event/Select_2"

sel3 = ZipBarrierExampleSelector("select_every_third")
sel3.Divisor = 3
sel3.OutputSelection = "/Event/Select_3"

sel4 = ZipBarrierExampleSelector("select_every_fourth")
sel4.Divisor = 4
sel4.OutputSelection = "/Event/Select_4"

sels = [sel1, sel2, sel3, sel4]
for sel in sels:
    sel.InputLocation = prod.OutputLocation
    sel.InputSelection = prod.OutputSelection

gather1 = ZipBarrierGatherer("gather_all")
gather1.InputSelections = [sel.OutputSelection.Path for sel in sels]
gather1.OutputSelection = "/Event/AllGathers"
merger1 = ZipBarrierMerger("merge_all")
merger1.InputSelection = gather1.OutputSelection
merger1.OutputSelection = "/Event/AllMerges"

gather2 = ZipBarrierGatherer("gather_rares")
gather2.InputSelections = [sel.OutputSelection.Path for sel in [sel3, sel4]]
gather2.OutputSelection = "/Event/RareGathers"
merger2 = ZipBarrierMerger("merge_rares")
merger2.InputSelection = gather2.OutputSelection
merger2.OutputSelection = "/Event/RareMerges"
worker = ZipBarrierExampleWorker("worker_rares")
worker.InputSelection = merger2.OutputSelection
worker.InputLocation = prod.OutputLocation
worker.OutputLocation = "/Event/ProcessedVector"

# Select_42 is never created. gather3 has it last, gather4 has it first in the inputs to test corner cases of the merging step.
gather3 = ZipBarrierGatherer("gather_rares_")
gather3.InputSelections = [sel.OutputSelection.Path
                           for sel in [sel3, sel4]] + ["/Event/Select_42"]
gather3.OutputSelection = "/Event/RareGathers_"
merger3 = ZipBarrierMerger("merge_rares_")
merger3.InputSelection = gather3.OutputSelection
merger3.OutputSelection = "/Event/RareMerges_"

gather4 = ZipBarrierGatherer("gather_rares__")
gather4.InputSelections = ["/Event/Select_42"] + [
    sel.OutputSelection.Path for sel in [sel3, sel4]
]
gather4.OutputSelection = "/Event/RareGathers__"
merger4 = ZipBarrierMerger("merge_rares__")
merger4.InputSelection = gather4.OutputSelection
merger4.OutputSelection = "/Event/RareMerges__"

gather5 = ZipBarrierGatherer("gather_rares___")
gather5.InputSelections = ["/Event/Select_42"]
gather5.OutputSelection = "/Event/RareGathers___"
merger5 = ZipBarrierMerger("merge_rares___")
merger5.InputSelection = gather5.OutputSelection
merger5.OutputSelection = "/Event/RareMerges___"

topalgs = [
    prod,
    sel1,
    sel2,
    sel3,
    sel4,
    gather1,
    merger1,
    gather2,
    merger2,
    gather3,
    merger3,
    gather4,
    merger4,
    # gather5, merger5, # throws
]

somenumber = 1
printers = []
for a in topalgs:
    if a in [prod, gather1, gather2, gather3, gather4, gather5]: continue
    somenumber += 1
    printer = PrintInts("Printer_" + str(somenumber))
    printer.InputLocation = prod.OutputLocation.Path
    printer.InputSelection = a.OutputSelection.Path
    printers.append(printer)

topalgs += [worker]
topalgs += printers

printsquares = PrintSquaredInts("PrintRareSquares")
printsquares.InputLocation = worker.OutputLocation.Path
printsquares.InputSelection = worker.InputSelection.Path
printsquares_v = PrintIntsAndSquaredInts("PrintRareSquaresVerbose")
printsquares_v.InputSelection = worker.InputSelection.Path
printsquares_v.InputIntLocation = prod.OutputLocation.Path
printsquares_v.InputSquaredIntLocation = worker.OutputLocation.Path

topalgs += [printsquares, printsquares_v]

app.TopAlg = topalgs

# - Events
app.EvtMax = 2
app.EvtSel = "NONE"
app.HistogramPersistency = "NONE"
