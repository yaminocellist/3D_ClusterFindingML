import uproot
import numpy as np
import pandas as pd
import sys

def get_tpc_hits(file_path, tree_name="ntp_hit"):
    try:
        with uproot.open(file_path) as f:
            if tree_name not in f.keys() and f"{tree_name};1" not in f.keys():
                print(f"Tree '{tree_name}' not found in {file_path}")
                return None
            tree = f[tree_name]
            branches = ["layer"]
            # Try loading ADC as well
            if "adc" in tree.keys():
                branches.append("adc")
            if "x" in tree.keys():
                branches.append("x")
            
            data = tree.arrays(branches, library="pd")
            
            # sPHENIX TPC layers are typically 7 to 54
            tpc_data = data[(data['layer'] >= 7) & (data['layer'] <= 54)]
            return tpc_data
    except Exception as e:
        print(f"Error loading {file_path}: {e}")
        return None

def main():
    if len(sys.argv) < 3:
        print("Usage: python assess_tpc.py <true_data.root> <sim_data.root>")
        sys.exit(1)
        
    true_path = sys.argv[1]
    sim_path = sys.argv[2]
    
    print(f"Loading true data from {true_path}...")
    true_tpc = get_tpc_hits(true_path)
    
    print(f"\nLoading sim data from {sim_path}...")
    sim_tpc = get_tpc_hits(sim_path)
    
    if true_tpc is None or sim_tpc is None:
        print("Could not load TPC data for comparison.")
        sys.exit(1)
        
    print("\n=== Assessment of TPC Data ===")
    print(f"Total TPC Hits (True Data): {len(true_tpc)}")
    print(f"Total TPC Hits (Sim Data) : {len(sim_tpc)}")
    
    print("\nLayer Distribution:")
    print("True Data layers:", true_tpc['layer'].min(), "to", true_tpc['layer'].max())
    print("Sim Data layers :", sim_tpc['layer'].min(), "to", sim_tpc['layer'].max())
    
    if "adc" in true_tpc.columns and "adc" in sim_tpc.columns:
        print("\nADC Statistics:")
        print(f"True Data ADC - Mean: {true_tpc['adc'].mean():.2f}, Std: {true_tpc['adc'].std():.2f}")
        print(f"Sim Data ADC  - Mean: {sim_tpc['adc'].mean():.2f}, Std: {sim_tpc['adc'].std():.2f}")

    if "x" in true_tpc.columns and "x" in sim_tpc.columns:
        print("\nSpatial Statistics (x):")
        print(f"True Data X - Mean: {true_tpc['x'].mean():.2f}, Std: {true_tpc['x'].std():.2f}")
        print(f"Sim Data X  - Mean: {sim_tpc['x'].mean():.2f}, Std: {sim_tpc['x'].std():.2f}")
        
if __name__ == "__main__":
    main()
