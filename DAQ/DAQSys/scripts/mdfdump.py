#!/usr/bin/env python
###############################################################################
# (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           #
#                                                                             #
# This software is distributed under the terms of the GNU General Public      #
# Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   #
#                                                                             #
# In applying this licence, CERN does not waive the privileges and immunities #
# granted to it by virtue of its status as an Intergovernmental Organization  #
# or submit itself to any jurisdiction.                                       #
###############################################################################
from __future__ import print_function
import argparse
import GaudiPython
from pprint import pformat

parser = argparse.ArgumentParser(description='Dump MDF file')
parser.add_argument('file')
args = parser.parse_args()

from GaudiConf import IOHelper
IOHelper('MDF').inputFiles(['mdf:' + args.file])

from Configurables import LHCbApp
LHCbApp()

def events(appmgr):
    TES = appmgr.evtsvc()
    appmgr.run(1)
    while TES['/Event']:
        yield TES
        appmgr.run(1)

LHCb = GaudiPython.gbl.LHCb
RAW_BANK_TYPES = [(i, LHCb.RawBank.typeName(i))
                  for i in range(LHCb.RawBank.LastType)]

def chunk(s, n=16):
    return [s[i:i+n] for i in range(0, len(s), n)]


def rawbank_dict(rb, fulldata=False):
    data = rb.data()[:rb.size()]
    padding = rb.data()[rb.size():(rb.totalSize() - rb.hdrSize())]
    return {
        'magic': rb.magic(),
        'size': rb.size(),
        'totalSize': rb.totalSize(),
        'type': rb.type(),
        'version': rb.version(),
        'sourceID': rb.sourceID(),
        'data': hash(data) if not fulldata else list(enumerate(chunk(data))),
        'padding': padding,
        }


def unzip(data):
    import ROOT
    if not hasattr(ROOT, 'unzip'):
        code = '''
#include "RZip.h"

std::vector<char> unzip(std::string data) {
  std::vector<char> buffer;
  buffer.resize(data.size() * 4); // TODO get rid of unneeded initialization
  while (true) {
    int output_size = 0;
    int input_size = data.size();
    int buffer_size = buffer.size();
    R__unzip(&input_size, (unsigned char*)&data[0],
             &buffer_size, (unsigned char*)&buffer[0], &output_size);
    if (output_size > 0) {
      buffer.resize(output_size);
      break;
    } else if (buffer.size() < 64 * data.size()) {
      // Maybe the buffer is not big enough?
      buffer.resize(2 * buffer.size());
    } else {
      // Failed to decompress data, quitting
      buffer.resize(0);
      break;
    }
  }
  return buffer;
}
'''
        ROOT.gInterpreter.Declare(code)
    return ''.join(ROOT.unzip(data))

appMgr = GaudiPython.AppMgr()
for i_event, TES in enumerate(events(appMgr)):
    raw = TES['DAQ/RawEvent']
    for bank_type, name in RAW_BANK_TYPES:
        banks = raw.banks(bank_type)
        if not banks.size():
            continue
        print("{}:{}".format(i_event, name))
        # fulldata = (name == 'DstData')
        fulldata = False
        print(''.join(["    " + x for x in pformat([rawbank_dict(rb, fulldata)
                      for rb in banks]).splitlines(True)]))
        if bank_type == LHCb.RawBank.DstData:
            data = ''.join(rb.data()[:rb.size()] for rb in banks)
            print("    Uncompressed DstData:\n{}".format(pformat(list(enumerate(chunk(unzip(data)))))))
