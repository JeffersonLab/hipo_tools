#!/usr/bin/env python
from __future__ import print_function
from __future__ import division

import sys

import numpy as np
import time
try:
    from hipopy4 import hipo4_reader
except ImportError:
    from src.hipopy.hipopy4 import hipo4_reader

file_name = sys.argv[1]

event = hipo4_reader(file_name.encode())

total = 0
start_time = time.time()

# print(event)

for event_num, evnt in enumerate(event):
    print("##### DATA #####")
    print(event_num, len(event))
    for part in range(len(event)):
        print(event.pid(part), event.px(part))
    print("##### MC #####")
    print(event.mc_npart)
    # for part in range(event.mc_npart):
    #    print(event.mc_pid(part), event.mc_px(part), event.mc_weight)

    if event_num > 10:
        break

print(dir(event))

print(str(time.time() - start_time), "sec")
print(str(total / (time.time() - start_time)), "hz")
