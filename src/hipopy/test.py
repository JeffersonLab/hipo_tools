from hipopy import hipo_reader
import numpy as np

reader = hipo_reader(b'/Users/tylern/Data/hipo/out_clas_004203.evio.101.hipo')

rec_part_pid = reader.getIntNode(331, 1)  # REC::Particle pid
rec_part_px = reader.getFloatNode(331, 3)  # REC::Particle px


px = []
num = 0
while(reader.next() and num < 5):
    num += 1
    print("length (" + str(rec_part_pid.getLength()) + "):", end=' ')
    for i in range(0, rec_part_pid.getLength()):
        print(rec_part_pid.getValue(i), end=' ')

    print()
