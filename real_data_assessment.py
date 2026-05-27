#!/usr/bin/env python
# coding: utf-8

# # Real Data Assessment: sPHENIX TPC
# This notebook re-evaluates the dataset `clusters_seeds_island_79507-0.root_ntuplizer.root` recognizing it as **real detector data**, not a simulated dataset. 
# 
# In real data without Monte Carlo truth labels, variables like `hitID` are typically sequential indices or hardware identifiers, **not** particle/track labels. Therefore, we cannot group by `hitID` to find 'real tracks'. Instead, we must look at the physical properties of the hits (ADC, layer, spatial coordinates) and evaluate the overall data quality and occupancy before applying unsupervised clustering or tracking algorithms.

# In[8]:


import uproot
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Adjust plot settings
plt.rcParams['figure.figsize'] = (10, 6)
plt.rcParams['font.size'] = 12


# In[9]:


file_path = "clusters_seeds_island_79507-0.root_ntuplizer.root"
print(f"Loading real data sample from {file_path}...")

# Open the file and inspect the ntp_hit tree
try:
    with uproot.open(file_path) as f:
        hit_tree = f["ntp_hit"]
        print("Available branches in 'ntp_hit':", hit_tree.keys())
        
        # Load a standard set of columns. We'll start with hitID, adc, layer.
        # If spatial columns exist (like x, y, z, or r, phi), they will be useful later.
        branches_to_load = ["hitID", "adc", "layer"]
        
        # Check if they exist to avoid crashes
        valid_branches = [b for b in branches_to_load if b in hit_tree.keys()]
        print(valid_branches)
        data = hit_tree.arrays(valid_branches, library="pd")
        print(f"\nLoaded {len(data)} hits.")
except Exception as e:
    print("Error loading file or branches:", e)

data.head()


# ### 1. Re-evaluating `hitID`
# Let's check if `hitID` is simply a unique identifier per hit, rather than a grouping variable for tracks. If every row has a unique `hitID`, it means it's an index, not a particle ID.

# In[10]:


if 'hitID' in data.columns:
    unique_ids = data['hitID'].nunique()
    total_hits = len(data)
    print(f"Total hits: {total_hits}")
    print(f"Unique hitIDs: {unique_ids}")
    
    if unique_ids == total_hits:
        print("\nConclusion: 'hitID' is a unique index for every single hit.")
        print("It cannot be used to group hits into tracks or calculate track multiplicity.")
    else:
        print("\nConclusion: 'hitID' has repeating values. Its meaning should be investigated (e.g., is it a cluster ID?).")
        print("Average hits per hitID:", total_hits / unique_ids)


# ### 2. Physical Data Distributions
# Instead of grouping by ID, we analyze the macroscopic properties of the event/data:
# - **ADC Distribution**: Helps separate minimum ionizing particle (MIP) signals from low-energy electronics noise.
# - **Hit Occupancy per Layer**: Shows detector activity, geometry coverage, and helps identify noisy layers.

# In[11]:


fig, axes = plt.subplots(1, 3, figsize=(18, 5))

if 'adc' in data.columns:
    axes[0].hist(data['adc'], bins=100, log=True, color='blue', alpha=0.7)
    axes[0].set_title("ADC Value Distribution (Log Scale)")
    axes[0].set_xlabel("ADC")
    axes[0].set_ylabel("Number of Hits")

    # Zoom in around 0 (e.g., up to 500)
    axes[1].hist(data['adc'], bins=100, range=(0, 500), log=True, color='purple', alpha=0.7)
    axes[1].set_title("Zoomed ADC Distribution (0-500)")
    axes[1].set_xlabel("ADC")
    axes[1].set_ylabel("Number of Hits")

if 'layer' in data.columns:
    axes[2].hist(data['layer'], bins=range(int(data['layer'].min()), int(data['layer'].max()) + 2), 
                 color='green', alpha=0.7, edgecolor='black', align='left')
    axes[2].set_title("Hit Multiplicity per Layer")
    axes[2].set_xlabel("Layer Index")
    axes[2].set_ylabel("Number of Hits")

plt.tight_layout()
plt.show()


# ### 3. Noise vs. Signal Separation Strategies in Real Data
# Since we don't have simulated truth labels, we must rely on physical properties to identify signal from background and prepare for ML.
# 
# 1. **ADC Thresholding**: Low ADC values often correspond to electronics noise. We can apply a threshold based on the distribution above to clean the data.
# 2. **Spatial Correlation (Clustering)**: Real tracks leave contiguous hits in adjacent layers and pads. We can use spatial clustering algorithms (like DBSCAN on physical coordinates $x, y, z$) to find contiguous hit structures.
# 3. **Track Finding**: Using classical tracking algorithms (e.g., Hough Transform) or Graph Neural Networks (GNNs) on the physical hit coordinates to identify track trajectories.

# In[12]:


print("\n--- ML PREPROCESSING INSIGHTS FOR REAL DATA ---")
if 'adc' in data.columns:
    # Example threshold calculation (e.g., 90th percentile to isolate high-energy hits)
    adc_threshold = data['adc'].quantile(0.90)
    print(f"1. Suggested initial ADC threshold for noise reduction (e.g. 90th percentile): > {adc_threshold:.1f}")
    
print("2. Need to load spatial features (e.g., x, y, z or r, phi) to perform spatial clustering.")
print("3. Once spatial clusters/tracklets are formed, we can compute features like 'multiplicity' and 'layer_span' on THOSE structures, rather than arbitrary hitIDs.")


# ### 4. Interactive Data Labeling using Plotly FigureWidget
# To create training labels manually, we can plot a subset of our data (e.g., high ADC hits) in 3D and interactively click on them to label them as 'signal' or 'noise'.

# In[13]:


import plotly.graph_objects as go
import pandas as pd
import numpy as np

# 1. Create a dummy dataframe for demonstration (replace with actual spatial data if loaded)
df_sample = pd.DataFrame({
    'x': np.random.rand(100) * 100,
    'y': np.random.rand(100) * 100,
    'z': np.random.rand(100) * 100,
    'label': ['unknown'] * 100  # Initial state
})

# 2. MUST use FigureWidget for Python callbacks to work in Jupyter
fig = go.FigureWidget()

# 3. Add the 3D scatter trace
fig.add_trace(go.Scatter3d(
    x=df_sample['x'], 
    y=df_sample['y'], 
    z=df_sample['z'],
    mode='markers',
    marker=dict(size=4, color=['gray'] * len(df_sample)), # Explicit color array needed for indexed updates
    # Store the DataFrame index in customdata so we know EXACTLY which row we clicked
    customdata=df_sample.index, 
    hoverinfo='text',
    text=[f"Index: {i}<br>Label: unknown" for i in df_sample.index]
))

scatter_trace = fig.data[0]

# 4. Define the Python function that runs when a point is clicked
def on_point_click(trace, points, selector):
    if not points.point_inds:
        return
    
    point_idx = points.point_inds[0]
    df_row_idx = trace.customdata[point_idx]
    
    current_label = df_sample.loc[df_row_idx, 'label']
    
    if current_label == 'unknown':
        new_label = 'signal'
        new_color = 'red'
    elif current_label == 'signal':
        new_label = 'noise'
        new_color = 'blue'
    else:
        new_label = 'unknown'
        new_color = 'gray'
        
    df_sample.loc[df_row_idx, 'label'] = new_label
    
    colors = list(trace.marker.color)
    colors[point_idx] = new_color
    
    hover_texts = list(trace.text)
    hover_texts[point_idx] = f"Index: {df_row_idx}<br>Label: {new_label}"
    
    with fig.batch_update():
        trace.marker.color = colors
        trace.text = hover_texts

# 5. Attach the callback to the trace
scatter_trace.on_click(on_point_click)

# 6. Display the widget
fig


# ### 5. Advanced Interactive Labeling (2D Selection Linked to 3D)
# Plotly 3D scatter plots do not support lasso or box selection. To select many points at once, we can plot a 2D projection (e.g., X vs Y) next to the 3D plot. 
# You can use the **Lasso Select** or **Box Select** tools on the 2D plot, and it will update the 3D plot and your underlying DataFrame automatically.

# In[14]:


import plotly.graph_objects as go
import pandas as pd
import numpy as np
import ipywidgets as widgets

# 1. Dummy data (replace with your actual data)
df_linked = pd.DataFrame({
    'x': np.random.rand(200) * 100,
    'y': np.random.rand(200) * 100,
    'z': np.random.rand(200) * 100,
    'label': ['unknown'] * 200
})

# 2. Create the 3D FigureWidget
fig3d = go.FigureWidget()
fig3d.add_trace(go.Scatter3d(
    x=df_linked['x'], y=df_linked['y'], z=df_linked['z'],
    mode='markers',
    marker=dict(size=4, color=['gray'] * len(df_linked)),
    customdata=df_linked.index,
    hoverinfo='text',
    text=[f"Index: {i}<br>Label: unknown" for i in df_linked.index]
))
fig3d.update_layout(title="3D View", margin=dict(l=0, r=0, b=0, t=40))

# 3. Create the 2D FigureWidget (e.g., X vs Y projection)
fig2d = go.FigureWidget()
fig2d.add_trace(go.Scatter(
    x=df_linked['x'], y=df_linked['y'],
    mode='markers',
    marker=dict(size=6, color=['gray'] * len(df_linked)),
    customdata=df_linked.index,
    hoverinfo='text',
    text=[f"Index: {i}<br>Label: unknown" for i in df_linked.index]
))
# Enable lasso by default
fig2d.update_layout(title="2D Projection (Drag to Select)", dragmode='lasso', margin=dict(l=0, r=0, b=0, t=40))

trace3d = fig3d.data[0]
trace2d = fig2d.data[0]

# 4. Define the callback for LASSO / BOX selection
def on_selection(trace, points, selector):
    if not points.point_inds:
        return
    
    # Example: we mark lasso-selected points as 'noise' (blue) for bulk removal
    # You can change this to 'signal' (red) based on your needs.
    new_label = 'noise'
    new_color = 'blue'
    
    colors3d = list(trace3d.marker.color)
    colors2d = list(trace2d.marker.color)
    texts = list(trace3d.text)
    
    for idx in points.point_inds:
        df_row_idx = trace.customdata[idx]
        df_linked.loc[df_row_idx, 'label'] = new_label
        
        colors3d[idx] = new_color
        colors2d[idx] = new_color
        texts[idx] = f"Index: {df_row_idx}<br>Label: {new_label}"
        
    with fig3d.batch_update():
        trace3d.marker.color = colors3d
        trace3d.text = texts
        
    with fig2d.batch_update():
        trace2d.marker.color = colors2d
        trace2d.text = texts

# Attach the selection callback to the 2D trace
trace2d.on_selection(on_selection)

# 5. Define the callback for INDIVIDUAL CLICKS (just like before)
def on_click(trace, points, selector):
    if not points.point_inds:
        return
    idx = points.point_inds[0]
    df_row_idx = trace.customdata[idx]
    
    current_label = df_linked.loc[df_row_idx, 'label']
    if current_label == 'unknown':
        new_label, new_color = 'signal', 'red'
    elif current_label == 'signal':
        new_label, new_color = 'noise', 'blue'
    else:
        new_label, new_color = 'unknown', 'gray'
        
    df_linked.loc[df_row_idx, 'label'] = new_label
    
    colors3d = list(trace3d.marker.color)
    colors2d = list(trace2d.marker.color)
    texts = list(trace3d.text)
    
    colors3d[idx] = new_color
    colors2d[idx] = new_color
    texts[idx] = f"Index: {df_row_idx}<br>Label: {new_label}"
    
    with fig3d.batch_update():
        trace3d.marker.color = colors3d
        trace3d.text = texts
    with fig2d.batch_update():
        trace2d.marker.color = colors2d
        trace2d.text = texts

# Attach the click callback to BOTH traces
trace2d.on_click(on_click)
trace3d.on_click(on_click)

# 6. Display them side-by-side using an ipywidgets HBox
widgets.HBox([fig2d, fig3d])


# In[ ]:




