import uproot
import awkward as ak
import numpy as np
import pandas as pd

file_path = "clusters_seeds_island_79507-0.root_ntuplizer.root"
branches = ["layer", "phi", "tbin", "zbin", "phibin", "x", "y", "z", "r"]

with uproot.open(file_path) as f:
    hits = f["ntp_hit"].arrays(branches, library="pd")

# Filter out NaNs
hits = hits.dropna()

# Take a small random sample to analyze
sample = hits.sample(10, random_state=42).copy()

# Add standard trig calculations to see if there's a simple multiplier
sample['cos_phi'] = np.cos(sample['phi'])
sample['sin_phi'] = np.sin(sample['phi'])

# R is usually sqrt(x^2 + y^2), let's check
sample['r_calc'] = np.sqrt(sample['x']**2 + sample['y']**2)
sample['r_diff'] = sample['r'] - sample['r_calc']

# Let's see the ratio of X to cos(phi) -- this should give us R
sample['x_over_cos'] = sample['x'] / sample['cos_phi']
sample['y_over_sin'] = sample['y'] / sample['sin_phi']

# Look at the relationship between R and layer
sample['r_over_layer'] = sample['r'] / sample['layer']

pd.set_option('display.max_columns', None)
pd.set_option('display.width', 1000)
print(sample[['layer', 'r', 'r_calc', 'r_over_layer', 'x', 'cos_phi', 'x_over_cos', 'y', 'sin_phi', 'y_over_sin', 'tbin', 'z']])
