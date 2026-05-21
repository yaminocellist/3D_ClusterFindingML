#!/usr/bin/env python3

import csv
from collections import Counter
from pathlib import Path

import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from matplotlib import colormaps
from matplotlib.colors import LinearSegmentedColormap, PowerNorm
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401


SCRIPT_DIR = Path(__file__).parent
CSV_DIR = SCRIPT_DIR.parent / "csv"

HIT_CSV_PATH = CSV_DIR / "event74_hits.csv"
CLUSTER_CSV_PATH = CSV_DIR / "event74_ntp_cluster.csv"
CLUS_TRK_CSV_PATH = CSV_DIR / "event74_ntp_clus_trk.csv"

HIT_PDF_PATH = SCRIPT_DIR / "event74_py.pdf"
CLUSTER_PDF_PATH = SCRIPT_DIR / "event74_ntp_cluster_py.pdf"
CLUS_TRK_PDF_PATH = SCRIPT_DIR / "event74_ntp_clus_trk_py.pdf"

BASE_CMAP = colormaps.get_cmap("viridis")
BLUE_TO_YELLOW = LinearSegmentedColormap.from_list(
    "viridis_blue_yellow",
    BASE_CMAP(np.linspace(0.25, 1.0, 256)),
)
ADC_NORM = PowerNorm(gamma=2.4, vmin=10, vmax=150, clip=True)


def cluster_norm(adcs: list[float]) -> PowerNorm:
    adc_mode = Counter(int(value) for value in adcs).most_common(1)[0][0]
    vmax = max(float(np.percentile(adcs, 99.5)), adc_mode + 1.0)
    return PowerNorm(gamma=2.4, vmin=adc_mode, vmax=vmax, clip=True)


def render(
    xs,
    ys,
    zs,
    adcs,
    output_path: Path,
    labels: tuple[str, str, str],
    norm: PowerNorm,
    invert_z: bool = False,
) -> None:
    fig = plt.figure(figsize=(11, 8))
    ax = fig.add_subplot(111, projection="3d")

    ax.scatter(
        xs,
        ys,
        zs,
        c=adcs,
        cmap=BLUE_TO_YELLOW,
        norm=norm,
        s=1.2,
        linewidths=0,
        rasterized=True,
    )

    ax.set_xlabel(labels[0])
    ax.set_ylabel(labels[1])
    ax.set_zlabel(labels[2])
    ax.view_init(elev=24, azim=62)
    if invert_z:
        ax.invert_zaxis()
    ax.grid(False)
    ax.set_facecolor("white")
    for axis in (ax.xaxis, ax.yaxis, ax.zaxis):
        axis.pane.fill = False
        axis.pane.set_edgecolor((1, 1, 1, 0))
    fig.tight_layout()
    fig.savefig(output_path, dpi=450, bbox_inches="tight")


def load_rows(csv_path: Path, coordinates: tuple[str, str, str]) -> tuple[list[float], list[float], list[float], list[float]]:
    if not csv_path.exists():
        raise FileNotFoundError(f"Missing input CSV: {csv_path}")

    xs = []
    ys = []
    zs = []
    adcs = []

    with csv_path.open(newline="") as handle:
        reader = csv.DictReader(handle)
        for row in reader:
            zelem = float(row["zelem"])
            if zelem != 1:
                continue
            layer = float(row["layer"])
            if not (7 < layer < 55):
                continue

            xs.append(float(row[coordinates[0]]))
            ys.append(float(row[coordinates[1]]))
            zs.append(float(row[coordinates[2]]))
            adcs.append(float(row["adc"]))

    return xs, ys, zs, adcs


def main() -> None:
    hit_data = load_rows(HIT_CSV_PATH, ("plot_x", "plot_y", "plot_z"))
    cluster_data = load_rows(CLUSTER_CSV_PATH, ("x", "y", "z"))
    clus_trk_data = load_rows(CLUS_TRK_CSV_PATH, ("x", "y", "z"))
    cluster_plot_data = (cluster_data[2], cluster_data[0], cluster_data[1], cluster_data[3])
    clus_trk_plot_data = (clus_trk_data[2], clus_trk_data[0], clus_trk_data[1], clus_trk_data[3])

    render(*hit_data, HIT_PDF_PATH, ("time bin", "x (cm)", "y (cm)"), ADC_NORM)
    render(
        *cluster_plot_data,
        CLUSTER_PDF_PATH,
        ("z (cm)", "x (cm)", "y (cm)"),
        cluster_norm(cluster_data[3]),
    )
    render(
        *clus_trk_plot_data,
        CLUS_TRK_PDF_PATH,
        ("z (cm)", "x (cm)", "y (cm)"),
        cluster_norm(clus_trk_data[3]),
    )
    print("Displaying plots... Close the windows to exit.")
    plt.show()


if __name__ == "__main__":
    main()
