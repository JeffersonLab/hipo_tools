from __future__ import print_function
from hipopy import hipo_reader
import sys
import numpy as np


file_name = sys.argv[1]
reader = hipo_reader(unicode(file_name, "utf-8"))

rec_part_pid = reader.getIntNode(u"REC::Particle", u"pid")
rec_part_px = reader.getFloatNode(u"REC::Particle", u"px")
rec_part_py = reader.getFloatNode(u"REC::Particle", u"py")
rec_part_pz = reader.getFloatNode(u"REC::Particle", u"pz")
rec_part_vx = reader.getFloatNode(u"REC::Particle", u"vx")
rec_part_vy = reader.getFloatNode(u"REC::Particle", u"vy")
rec_part_vz = reader.getFloatNode(u"REC::Particle", u"vz")
rec_part_charge = reader.getByteNode(u"REC::Particle", u"charge")
rec_part_beta = reader.getFloatNode(u"REC::Particle", u"beta")

px_hipo = []
num = 0
while(reader.next() and num < 20):
    num += 1
    print("Event:")
    for i in range(0, rec_part_pid.getLength()):
        px_hipo.append(rec_part_px[i])
        print("pid\t" + str(rec_part_pid[i]))
        print("px\t" + str(rec_part_px[i]))
        print("py\t" + str(rec_part_py[i]))
        print("pz\t" + str(rec_part_pz[i]))
        print("vx\t" + str(rec_part_vx[i]))
        print("vy\t" + str(rec_part_vy[i]))
        print("vz\t" + str(rec_part_vz[i]))
        print("q\t" + str(rec_part_charge[i]))
        print("b\t" + str(rec_part_beta[i]))
    print()
