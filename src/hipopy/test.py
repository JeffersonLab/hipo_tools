from hipopy import hipo_reader
import numpy as np

reader = hipo_reader("/Users/tylern/Data/hipo/out_clas_004203.evio.101.hipo")

rec_part_pid = reader.getIntNode(331, 1)  # REC::Particle pid
rec_part_px = reader.getFloatNode(331, 3)  # REC::Particle px

px = []
num = 0
while(reader.next()):
    if(rec_part_pid.getValue(0) == 11):
        px.append(rec_part_px.getValue(0))


px = np.array(px)
print(px)
