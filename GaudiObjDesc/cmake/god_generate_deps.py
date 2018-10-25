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

import os

from collections import defaultdict
from xml.dom import minidom
from os.path import join, basename, splitext, exists
from optparse import OptionParser

def main():
    parser = OptionParser(usage='%prog <cmake file> <xml files...>')
    parser.add_option('-s', '--src-output', action='store', metavar="DIR",
                      help='define possible output destination of source code. '
                      'Use the special value "env" to use the content of the environment '
                      'variable GODDOTHOUT. [default: %default]')

    parser.set_defaults(src_output=os.curdir)

    opts, args = parser.parse_args()
    if len(args) < 2:
        parser.error('wrong number of arguments')

    cmake_info = args.pop(0)
    xmlfiles = args

    products = defaultdict(set)

    for xmlfile in xmlfiles:
        key = splitext(basename(xmlfile))[0]
        dom = minidom.parse(xmlfile)
        elements_filename_set =  [elem.getAttribute('fileName') + '.h' for elem in dom.getElementsByTagName('class') + dom.getElementsByTagName('namespace')  if elem.hasAttribute('fileName')]
        if len(elements_filename_set)>0:
            elements = elements_filename_set
        else:
            elements = [elem.getAttribute('name') + '.h' for elem in dom.getElementsByTagName('class') + dom.getElementsByTagName('namespace')]
        products[key].update(join(opts.src_output,elem)
                             for elem in elements)

    old_data = open(cmake_info).read() if exists(cmake_info) else ''
    data = ''
    for key in sorted(products):
        files = sorted(products[key])
        data += ('set({0}_generated_files\n'
                 '    {1})\n').format(key, '\n    '.join(files))
    if data != old_data:
        open(cmake_info, 'w').write(data)

if __name__ == '__main__':
    main()
