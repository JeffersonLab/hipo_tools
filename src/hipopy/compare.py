from __future__ import print_function
import uproot
from hipopy import hipo_reader

import math
import sys
import matplotlib.pyplot as plt

tree = uproot.open(sys.argv[1] + ".root")["clas12"]
pid, px, py, pz = tree.arrays(
    ["REC_Particle_pid", "REC_Particle_px", "REC_Particle_py", "REC_Particle_pz"], outputtype=tuple)
momentum_root = []
for pid, pxi, pyi, pzi in zip(pid, px, py, pz):
    if(len(pid) > 0 and pid[0] == 11):
        for pxj, pyj, pzj in zip(pxi, pyi, pzi):
            momentum_root.append(math.sqrt(pxj**2 + pyj**2 + pzj**2))


plt.hist(momentum_root, bins=500, range=(0, 5))

file_name = sys.argv[1]
reader = hipo_reader(unicode(file_name, "utf-8"))
rec_part_px = reader.getFloatNode(u"REC::Particle", u"px")
rec_part_py = reader.getFloatNode(u"REC::Particle", u"py")
rec_part_pz = reader.getFloatNode(u"REC::Particle", u"pz")
rec_part_pid = reader.getIntNode(u"REC::Particle", u"pid")


momentum_hipo = []
while(reader.next()):
    if(rec_part_pid.getLength() > 0 and rec_part_pid[0] == 11):
        for i in range(0, rec_part_pid.getLength()):
            momentum_hipo.append(
                math.sqrt(rec_part_px[i]**2 + rec_part_py[i]**2 + rec_part_pz[i]**2))

plt.hist(momentum_hipo, bins=500, range=(0, 5))
plt.show()


plt.plot(momentum_root, momentum_hipo)
plt.show()
