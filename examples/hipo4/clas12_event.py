#!/usr/bin/env python
from __future__ import print_function
from __future__ import division

import numpy as np
import time
import sys
from tqdm import tqdm
from clas12 import clas12Event, LorentzVector

from ROOT import TH2D, TH1D, TFile

SECTORS = 7
BEAM_E = 10.9
MASS_ELEC = 0.000511


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

sampling_fraction_hist = TH2D(
    "sampling_fraction_hist", "sampling_fraction_hist", 500, 0, 10, 500, 0, 1
)
w_vs_q2_hist_FD = TH2D("w_vs_q2_FD", "w_vs_q2", 500, 0, 5, 500, 0, 5)
w_hist_FD = TH1D("w_FD", "w", 500, 0, 5)

w_vs_q2_hist_FT = TH2D("w_vs_q2_FT", "w_vs_q2", 500, 0, 5, 500, 0, 5)
w_hist_FT = TH1D("w_FT", "w", 500, 0, 5)

w_vs_q2_hist_CD = TH2D("w_vs_q2_CD", "w_vs_q2", 500, 0, 5, 500, 0, 5)
w_hist_CD = TH1D("w_CD", "w", 500, 0, 5)

e_mu = LorentzVector(0.0, 0.0, BEAM_E, energy=BEAM_E)

total = 0
t = tqdm(total=event.numEvents())
for evnt in event:
    total += 1
    if total % 5000 == 0:
        t.update(5000)
    if evnt.charge(0) != -1:
        continue
    mom = np.sqrt(np.square(evnt.px(0)) + np.square(evnt.py(0)) + np.square(evnt.pz(0)))
    sf = evnt.ec_tot_energy(0) / mom
    sampling_fraction_hist.Fill(mom, sf)
    e_mu_prime = LorentzVector(evnt.px(0), evnt.py(0), evnt.pz(0), mass=MASS_ELEC)
    w = W_calc(e_mu, e_mu_prime)
    q2 = Q2_calc(e_mu, e_mu_prime)
    if evnt.detector_system(0) == "FT":
        w_hist_FT.Fill(w)
        w_vs_q2_hist_FT.Fill(w, q2)
    elif evnt.detector_system(0) == "FD":
        w_hist_FD.Fill(w)
        w_vs_q2_hist_FD.Fill(w, q2)
    elif evnt.detector_system(0) == "CD":
        w_hist_CD.Fill(w)
        w_vs_q2_hist_CD.Fill(w, q2)

print("\n")
print(str(time.time() - start_time), "sec")
print(str(total / (time.time() - start_time)), "hz")

hfile = TFile("sf.root", "RECREATE", "Demo ROOT file with sampling fraction histogram")
sampling_fraction_hist.Write()
w_vs_q2_hist_FT.Write()
w_hist_FT.Write()
w_vs_q2_hist_FD.Write()
w_hist_FD.Write()
w_vs_q2_hist_CD.Write()
w_hist_CD.Write()
hfile.Write()
