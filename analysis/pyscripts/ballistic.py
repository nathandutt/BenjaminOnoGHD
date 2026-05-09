import sys
import collections
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

def load_res(filepath):
    """Load a .res file with lines: time bin value — counts occurrences per bin."""
    counter = collections.defaultdict(lambda: collections.defaultdict(int))
    values = collections.defaultdict(lambda: collections.defaultdict(int))
    data = collections.defaultdict(lambda: collections.defaultdict(int))
    with open(filepath, "r") as f:
        for lineno, line in enumerate(f, 1):
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split()
            if len(parts) != 3:
                print(f"Warning: skipping line {lineno} (expected 3 values, got {len(parts)})")
                continue
            try:
                time = float(parts[0])
                bin_ = float(parts[1])
                k = float(parts[2])
                val = k
                new_bin = bin_ + int(k*10/4)*4
            except ValueError:
                print(f"Warning: skipping line {lineno} (non-numeric data)")
                continue
            if(time < 1):
                counter[time][new_bin] +=1
                values[time][new_bin] += val #Change this to calculate different moments, here it is average of k
                #data[time][bin_] += 1

            else:
                counter[time][bin_] +=1
                values[time][bin_]+= val
    times = sorted(counter.keys())
    for time in times:
        bins_d = counter[time]
        bins = sorted(bins_d.keys())
        for one_bin in bins:
            data[time][one_bin] = (values[time][one_bin]/counter[time][one_bin])
    return data

def plot_histograms(data, output_prefix=None):
    times = sorted(data.keys())
    print(f"Found {len(times)} unique time value(s): {times}")

    for time in times:
        bins_dict = data[time]
        bins = sorted(bins_dict.keys())
        print(len(bins))
        values = [bins_dict[b] for b in bins]

        fig, ax = plt.subplots(figsize=(8, 5))

        # Bar width: use gap between bins if uniform, else 0.8 * min gap
        if len(bins) > 1:
            gaps = [bins[i+1] - bins[i] for i in range(len(bins)-1)]
            width = 0.8 * min(gaps)
        else:
            width = 0.8

        ax.bar(bins, values, width=width, color="steelblue", edgecolor="white", linewidth=0.5)
        if(time < 1):
            time = "Ballistic evolved"
        else:
            time = "Time evolved"
        ax.set_title(f"Histogram — time = {time}", fontsize=14, fontweight="bold")
        ax.set_xlabel("Bin", fontsize=12)
        ax.set_ylabel("Count", fontsize=12)
        ax.set_ylim(0, 75)
        ax.set_xlim(-1500, 1500)
        ax.grid(axis="y", linestyle="--", alpha=0.5)
        fig.tight_layout()

        if output_prefix:
            out = f"{output_prefix}_t{time}.png"
            fig.savefig(out, dpi=150)
            print(f"Saved: {out}")
        else:
            plt.show()

        plt.close(fig)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python plot_res.py <file.res> [output_prefix]")
        print("  output_prefix  optional — if given, saves PNGs instead of showing interactively")
        sys.exit(1)

    filepath = sys.argv[1]
    output_prefix = sys.argv[2] if len(sys.argv) > 2 else None

    data = load_res(filepath)
    if not data:
        print("No data loaded. Check the file format.")
        sys.exit(1)

    plot_histograms(data, output_prefix=output_prefix)
