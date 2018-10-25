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

def main(argv):
    if len(argv) != 3 or '-h' in argv or '--help' in argv:
        from os.path import basename 
        print "Usage: %s patch_db dest_db\n" % basename(argv[0])
        print "where:"
        print "\t%s\t%s"%("patch_db ", "the connection string for the source DB")
        print "\t%s\t%s"%("dest_db  ", "the connection string for the DB to modify")
        return 1
     
    # Main body
    from CondDBUI import merge, LOG_FORMAT
    
    import logging
    hndlr = logging.StreamHandler()
    hndlr.setFormatter(logging.Formatter(LOG_FORMAT))
    logging.getLogger().addHandler(hndlr)
    
    merge(source,target)
    
    return 0

if __name__ == '__main__':
    from sys import exit, argv
    exit(main(argv))
