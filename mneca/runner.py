#!/usr/bin/env python3

##
## Main Routine
##

import argparse

from kanji import KanjiCard


#------------------------------------------------------------------------------#

def parse_args():
    # Process command arguments
    parser = argparse.ArgumentParser(prog='mneca')
    parser.add_argument('-o', '--output', help = 'save to a given file')
    parser.add_argument('source', help = 'a source file to make cards')

    args = parser.parse_args()

    return args

def set_params(card, args = None):
    if args.output:
        card.output = args.output
    if args.source:
        card.source = args.source

def main():
    try:
        args = parse_args()
    
        c = KanjiCard()

        set_params(c, args)

        c.make()
    except Exception as e:
        print(e)
        

if __name__ == '__main__':
    main()
