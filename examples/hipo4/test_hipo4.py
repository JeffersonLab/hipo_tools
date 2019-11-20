#!/usr/bin/env python
from __future__ import print_function
from __future__ import division

import sys

import numpy as np
import time

from hipopy4 import hipo4_reader

file_name = sys.argv[1]

event = hipo4_reader(file_name.encode())

total = 0
start_time = time.time()

print(event)

for evnt in event:
    total += 1
#    if len(evnt) == 0:
#        continue
#    if total % 100000 == 0:
#        print(str(total / (time.time() - start_time)), "hz")


print(str(time.time() - start_time), "sec")
print(str(total / (time.time() - start_time)), "hz")
