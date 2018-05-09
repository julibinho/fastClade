#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Apr  6 11:50:03 2018

@author: Nathan
"""

import matplotlib.pyplot as plt
import numpy as np
import sys


FILENAME = sys.argv[1] 
OUTNAME = sys.argv[2]
NBBINS = sys.argv[3]

flux = open(FILENAME,'r')


intraFa = []
extraFa = []
for ligne in flux:
    sp = ligne.split("\t")
    intraFa.append(float(sp[2]))
    
    extraFa.append(float(sp[3][:-1]))
    
flux.close()


fig, ax = plt.subplots(1,1)
ax.set_title("Intra and Extra Family minHash similarity distribution")
ax.set_xlabel("minHash similarity")
ax.set_ylabel("Occurences")
bins = np.linspace(0,1,NBBINS)
ax.hist(intraFa, bins, alpha=0.5, label='IntraFamily similarities')
ax.hist(extraFa, bins, alpha=0.5, label='ExtraFamily similiraties')

ax.legend(loc='upper right')
fig.savefig(OUTNAME)