# 3D_ClusterFindingML

A Machine Learning project focused on identifying and reconstructing 3D clusters within the **sPHENIX TPC** detector using real data. This project aims to bridge the gap between traditional reconstruction and modern deep learning by creating a manually labeled Ground Truth dataset.

## 🔗 Project Resources
*   **Shared Project Documentation:** [Google Docs Link - Placeholder]

## 🚀 ML Training Pipeline
The project follows a three-stage strategy to ensure robust cluster identification:

1.  **Numerical Classification:** Utilize traditional ML models (Random Forest, XGBoost, or MLP) on numerical hit data to perform initial categorization and feature importance analysis.
2.  **Geometric Deep Learning:** Implement Graph Neural Networks (GNNs) to exploit the spatial and topological relationships of hits, predicting clusters and track segments directly from the hit-graph.
3.  **Ensemble & Validation:** Merge and cross-verify results from both the numerical and GNN models to improve purity and efficiency, using the manually labeled Ground Truth for final performance metrics.

---

## 💻 Environment Setup

### Mac (Apple Silicon M-series)
1.  **Xcode Command Line Tools:** `xcode-select --install`
2.  **Homebrew:** Install [Homebrew](https://brew.sh/) if not already present.
3.  **ROOT CERN:** `brew install root`
4.  **Python Environment:**
    ```bash
    conda create -n cluster_ml python=3.10
    conda activate cluster_ml
    pip install numpy matplotlib pandas scikit-learn torch torch-geometric
    ```

### WSL (Windows Subsystem for Linux - Ubuntu)
1.  **System Dependencies:**
    ```bash
    sudo apt-get update
    sudo apt-get install dpkg-dev cmake g++ gcc binutils libx11-dev libxpm-dev libxft-dev libxext-dev python3 libssl-dev
    ```
2.  **ROOT CERN:** It is recommended to install ROOT via `conda` for easiest dependency management in WSL:
    ```bash
    conda create -n cluster_ml -c conda-forge root python=3.10
    conda activate cluster_ml
    pip install numpy matplotlib pandas scikit-learn torch torch-geometric
    ```

---

## 📊 Data Visualization
To inspect the data and verify labeling:
1.  **Extract Data:**
    ```bash
    root -l -b -q Visualizing/src/GenerateEvent74CSVs.C
    ```
2.  **Run Plotter:**
    ```bash
    python Visualizing/src/plot_event74_hits.py
    ```

---

## 📂 Project Structure
*   `Visualizing/`: Scripts for data extraction and 3D hit inspection.
*   `Articles/`: Reference papers for TPC reconstruction and ML methodologies.
*   `*.ipynb`: Research and training notebooks.
*   `memory/`: (Private) Project development notes.
