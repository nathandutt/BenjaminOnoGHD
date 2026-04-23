"""
plot_res.py — Plot value distributions per bin_id from a .res file.

File format: each line is "t bin_id value"
  - t      : 0 or 10
  - bin_id : 0 to 11
  - value  : float

Produces two figures (one per time value), each showing a KDE curve
per bin_id. Run as:
    python plot_res.py path/to/file.res
"""

import sys
import collections
import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import gaussian_kde

# ── 1. Parse arguments ──────────────────────────────────────────────────────

if len(sys.argv) < 2:
    print("Usage: python plot_res.py <file.res>")
    sys.exit(1)

filepath = sys.argv[1]

# ── 2. Read the file ─────────────────────────────────────────────────────────

# data[t][bin_id] = list of values
data = collections.defaultdict(lambda: collections.defaultdict(list))

with open(filepath) as f:
    for lineno, line in enumerate(f, 1):
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split()
        if len(parts) != 3:
            print(f"Warning: skipping malformed line {lineno}: {line!r}")
            continue
        t, bin_id, value = int(parts[0]), int(parts[1]), float(parts[2])
        data[t][bin_id].append(value)

times = sorted(data.keys())
print(f"Found time values: {times}")
for t in times:
    bins = sorted(data[t].keys())
    counts = [len(data[t][b]) for b in bins]
    print(f"  t={t}: {len(bins)} bins, {sum(counts)} total values "
          f"(min/max per bin: {min(counts)}/{max(counts)})")

# ── 3. Color palette ─────────────────────────────────────────────────────────

N_BINS = 12
cmap = plt.get_cmap("tab20", N_BINS)
colors = [cmap(i) for i in range(N_BINS)]

# ── 4. Plot ──────────────────────────────────────────────────────────────────

def plot_distributions(ax, bin_data, title):
    """Draw one histogram per bin_id on *ax*."""
    all_values = [v for vals in bin_data.values() for v in vals]
    x_min, x_max = min(all_values), max(all_values)

    # shared bin edges so all histograms are comparable
    bin_edges = np.linspace(x_min, x_max, 60)  # adjust 60 to taste

    for bin_id in sorted(bin_data.keys()):
        vals = np.array(bin_data[bin_id])
        ax.hist(vals,
                bins=bin_edges,
                density=True,          # normalise to density so bins are comparable
                alpha=0.4,             # transparency so overlapping histos are visible
                color=colors[bin_id % N_BINS],
                label=f"bin {bin_id}")

    ax.set_title(title, fontsize=13, fontweight="bold")
    ax.set_xlabel("Value", fontsize=11)
    ax.set_ylabel("Density", fontsize=11)
    ax.legend(loc="upper right", ncol=2, fontsize=8, framealpha=0.7)
    ax.grid(True, alpha=0.3, linestyle="--")
    ax.spines[["top", "right"]].set_visible(False)

fig, axes = plt.subplots(
    1, len(times),
    figsize=(7 * len(times), 5),
    sharey=False,
)

# Make axes always iterable even if there's only one time value
if len(times) == 1:
    axes = [axes]

for ax, t in zip(axes, times):
    plot_distributions(ax, data[t], title=f"t = {t}")

fig.suptitle("Value distributions per bin_id", fontsize=15, y=1.02)
plt.tight_layout()

out = filepath.rsplit(".", 1)[0] + "_distributions.png"
plt.savefig(out, dpi=150, bbox_inches="tight")
print(f"\nSaved → {out}")
plt.show()
