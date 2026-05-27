import uproot
import awkward as ak
import numpy as np
import pandas as pd
from scipy import stats

file_path = "clusters_seeds_island_79507-0.root_ntuplizer.root"
branches = ["layer", "phi", "tbin", "zbin", "phibin", "x", "y", "z", "r"]

with uproot.open(file_path) as f:
    hits = f["ntp_hit"].arrays(branches, library="pd")

hits = hits.dropna()

# We clearly saw that:
# x = r * cos(phi)
# y = r * sin(phi)
# But how does 'r' relate to 'layer'? Let's look at all unique layers and their mean/std 'r'
print("--- R vs Layer ---")
grouped = hits.groupby('layer')['r'].agg(['mean', 'std', 'min', 'max'])
print(grouped.head(15))

# How does 'z' relate to 'tbin'?
print("\n--- Z vs Tbin ---")
# tbin is discrete. Let's see how z varies for a given tbin
grouped_tbin = hits.groupby('tbin')['z'].agg(['mean', 'std', 'min', 'max'])
print(grouped_tbin.head(15))

# Is Z just a linear transformation of Tbin? z = m * tbin + c ?
# Actually Z also depends on which side of the TPC we are on (usually z > 0 or z < 0)
print("\n--- Z distribution for Tbin == 0 ---")
print(hits[hits['tbin'] == 0]['z'].describe())

