#!/usr/bin/env python
from __future__ import print_function
from __future__ import division

import numpy as np
import time
import sys
from tqdm import tqdm
from clas12 import clas12Event
from physics import LorentzVector

import matplotlib.pyplot as plt

SECTORS = 7
BEAM_E = 10.9
MASS_ELEC = 0.000511
part_mass = {
    0: 0,
    45: 0,
    11: 0.000511,
    -11: 0.000511,
    211: 0.13957,
    -211: 0.13957,
    2212: 0.93827,
    -2212: 0.93827,
    2112: 0.939565,
    321: 0.493667,
    -321: 0.493667,
    22: 0
}


file_name = sys.argv[1]

event = clas12Event(file_name.encode("utf8"))

total = 0

e_mu = LorentzVector(0.0, 0.0, BEAM_E, energy=BEAM_E)
sf_p = np.ones((2, event.numEvents()), dtype=np.float) * np.nan

i = -1
#t = tqdm(total=event.numEvents())
start_time = time.time()
for evnt in event:
    i += 1
    #if i % 5000 == 0:
    #    t.update(5000)
    if evnt.charge(0) != -1:
        continue
    e_mu_prime = LorentzVector(evnt.px(0),
                               evnt.py(0),
                               evnt.pz(0),
                               mass=MASS_ELEC)
    sf = evnt.ec_tot_energy(0)/e_mu_prime.P

    sf_p[0][i] = sf
    sf_p[1][i] = e_mu_prime.P


print("\n")
print(str(time.time() - start_time), "sec")
print(str(i / (time.time() - start_time)), "hz")

sf = sf_p[0][~np.isnan(sf_p[0])]
p = sf_p[1][~np.isnan(sf_p[0])]

plt.hist2d(p,sf, bins=100)
plt.show()
