#!/usr/bin/env python
'''
Small script to simplify/reduce/fix IOVs files in a Git repository.
'''
import logging
from GitCondDB.IOVs import clean_iovs


def main():
    from optparse import OptionParser
    parser = OptionParser(usage='%prog [options] repository')
    parser.add_option('--partition', action='store_true')
    parser.add_option('--no-partition', action='store_false', dest='partition')
    parser.set_defaults(partition=True)

    opts, args = parser.parse_args()

    logging.basicConfig(level=logging.DEBUG)

    if len(args) != 1:
        parser.error('can work only on one repository at a time')

    clean_iovs(args[0], opts.partition)


if __name__ == '__main__':
    main()
