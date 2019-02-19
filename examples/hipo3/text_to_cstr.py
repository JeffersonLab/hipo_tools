#!/usr/bin/python

import sys

filename = sys.argv[1]

file = open(filename)
for line in file:
    sline = line[:-1]
    cline = sline.replace('"','\\"')
    print 'std__string.append("' + cline + '\\n");'

