# Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
# Author: Paul Seyfert <pseyfert@cern.ch>
#
# This software is distributed under the terms of the GNU General Public
# Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
#
# In applying this licence, CERN does not waive the privileges and immunities
# granted to it by virtue of its status as an Intergovernmental Organization
# or submit itself to any jurisdiction.

from Configurables import ZipBarrierExampleSelector, ZipBarrierExampleProducer, PrintInts
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
sel1.OutputLocation = "/Event/Select_1"

sel2 = ZipBarrierExampleSelector("selecteven")
sel2.Divisor = 2
sel2.OutputLocation = "/Event/Select_2"

sel3 = ZipBarrierExampleSelector("select_every_third")
sel3.Divisor = 3
sel3.OutputLocation = "/Event/Select_3"

sel4 = ZipBarrierExampleSelector("select_every_fourth")
sel4.Divisor = 4
sel4.OutputLocation = "/Event/Select_4"

sels = [sel1, sel2, sel3, sel4]
for sel in sels:
    sel.InputLocation = prod.OutputLocation
    sel.InputSelection = prod.OutputSelection


gather1 = ZipBarrierGatherer("gather_all")
gather1.InputSelections = [ sel.OutputSelection.Path for sel in sels ]
gather1.OutputSelection = "/Event/AllGathers"
merger1 = ZipBarrierMerger("merge_all")
merger1.InputSelection = gather1.OutputSelection
merger1.OutputSelection = "/Event/AllMerges"

gather2 = ZipBarrierGatherer("gather_rares")
gather2.InputSelections = [ sel.OutputSelection.Path for sel in [sel3, sel4] ]
gather2.OutputSelection = "/Event/RareGathers"
merger2 = ZipBarrierMerger("merge_rares")
merger2.InputSelection = gather2.OutputSelection
merger2.OutputSelection = "/Event/RareMerges"

gather3 = ZipBarrierGatherer("gather_rares_")
gather3.InputSelections = [ sel.OutputSelection.Path for sel in [sel3, sel4] ] + [ "/Event/Select_42" ]
gather3.OutputSelection = "/Event/RareGathers_"
merger3 = ZipBarrierMerger("merge_rares_")
merger3.InputSelection = gather3.OutputSelection
merger3.OutputSelection = "/Event/RareMerges_"

gather4 = ZipBarrierGatherer("gather_rares__")
gather4.InputSelections =  [ "/Event/Select_42" ] + [ sel.OutputSelection.Path for sel in [sel3, sel4] ]
gather4.OutputSelection = "/Event/RareGathers__"
merger4 = ZipBarrierMerger("merge_rares__")
merger4.InputSelection = gather4.OutputSelection
merger4.OutputSelection = "/Event/RareMerges__"

gather5 = ZipBarrierGatherer("gather_rares___")
gather5.InputSelections =  [ "/Event/Select_42" ]
gather5.OutputSelection = "/Event/RareGathers___"
merger5 = ZipBarrierMerger("merge_rares___")
merger5.InputSelection = gather5.OutputSelection
merger5.OutputSelection = "/Event/RareMerges___"

topalgs = [
        prod, sel1, sel2, sel3, sel4,
        gather1, merger1, gather2, merger2,
        gather3, merger3, gather4, merger4,
        # gather5, merger5, # throws
        ]

somenumber = 1
printers = []
for a in topalgs:
    if a in [prod, gather1, gather2, gather3, gather4, gather5]: continue
    somenumber+=1
    printer = PrintInts( "Printer_" + str(somenumber) )
    printer.InputLocation = prod.OutputLocation.Path
    printer.InputSelection = a.OutputSelection.Path
    printers.append(printer)

topalgs += printers

app.TopAlg = topalgs

# - Events
app.EvtMax = 2
app.EvtSel = "NONE"
app.HistogramPersistency = "NONE"
