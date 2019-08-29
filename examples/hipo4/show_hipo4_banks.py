#!/usr/bin/env python
from __future__ import print_function
from __future__ import division

import sys

import numpy as np
import time

from hipopy4 import hipo4_reader
import json

file_name = sys.argv[1]

hipo4_file = hipo4_reader(file_name.encode())

print(hipo4_file)

