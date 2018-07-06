from hipopy import hipo_reader
import sys
import numpy as np
import json

file_name = sys.argv[1]
reader = hipo_reader(file_name)

rec_part_pid = reader.getIntNode("REC::Particle", "pid")  # REC::Particle pid
rec_part_px = reader.getFloatNode("REC::Particle", "px")

px = []
num = 0
while(reader.next() and num < 5):
    num += 1
    print("length (" + str(rec_part_pid.getLength()) + "):", end=' ')
    for i in range(0, rec_part_pid.getLength()):
        print(rec_part_pid[i], end=' ')
        print(rec_part_px[i], end=' ')

    print()


bankdef = reader.getjson()
