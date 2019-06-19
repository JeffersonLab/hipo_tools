#!/usr/bin/env python
from __future__ import print_function
from __future__ import division

import numpy as np
import time
import sys
from tqdm import tqdm
from clas12 import clas12Event
from physics import LorentzVector

from ROOT import TH2D, TH1D, TFile

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


def Q2_calc(_e_mu, _e_mu_prime):
    """Retruns Q^2 value: -q^mu^2 = -(e^mu - e^mu')^2 = Q^2."""
    _q_mu = _e_mu - _e_mu_prime
    return -_q_mu.Mag2


def W_calc(_e_mu, _e_mu_prime):
    """Returns W: Gotten from s channel [(gamma - P)^2 == s == w^2], Sqrt[M_p^2 - Q^2 + 2 M_p gamma]."""
    _q_mu = _e_mu - _e_mu_prime
    _p_target = LorentzVector(0.0, 0.0, 0.0, mass=0.93827)
    return (_p_target + _q_mu).Mag


file_name = sys.argv[1]

event = clas12Event(file_name.encode("utf8"))

total = 0
start_time = time.time()

sampling_fraction_hist = TH2D("sampling_fraction_hist",
                              "sampling_fraction_hist", 500, 0, 10, 500, 0, 1)

w_vs_q2_hist = TH2D("w_vs_q2", "w_vs_q2", 500, 0, 5, 500, 0, 5)
w_hist = TH1D("w", "w", 500, 0, 5)
p_hist = TH1D("p", "p", 500, 0, 5)
pz_hist = TH1D("pz", "p_{z}", 500, 0, 10)
e_mu = LorentzVector(0.0, 0.0, BEAM_E, energy=BEAM_E)

total = 0
t = tqdm(total=event.numEvents())
for evnt in event:
    total += 1
    if total % 5000 == 0:
        t.update(5000)
    pz_hist.Fill(evnt.pz(0))
    if evnt.charge(0) != -1:
        continue
    e_mu_prime = LorentzVector(evnt.px(0),
                               evnt.py(0),
                               evnt.pz(0),
                               mass=MASS_ELEC)
    mom = e_mu_prime.P
    sf = evnt.ec_tot_energy(0) / mom
    sampling_fraction_hist.Fill(mom, sf)

    w = W_calc(e_mu, e_mu_prime)
    q2 = Q2_calc(e_mu, e_mu_prime)
    w_hist.Fill(w)
    w_vs_q2_hist.Fill(w, q2)
    for i in range(1, len(evnt)):
        part_prime = LorentzVector(evnt.px(i),
                                   evnt.py(i),
                                   evnt.pz(i),
                                   mass=part_mass[evnt.pid(i)])
        p_hist.Fill(part_prime.P)

print("\n")
print(str(time.time() - start_time), "sec")
print(str(total / (time.time() - start_time)), "hz")

hfile = TFile("pz.root", "RECREATE", "Demo ROOT file with pz hist")
sampling_fraction_hist.Write()
pz_hist.Write()
w_vs_q2_hist.Write()
w_hist.Write()
p_hist.Write()
hfile.Write()
