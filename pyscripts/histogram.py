import matplotlib.pyplot as plt

filename = "velocity.res"

values = []

with open(filename, "r") as f:
    for line in f:
        parts = line.strip().split()
        
        # Skip empty or malformed lines
        if len(parts) < 1:
            continue
        
        try:
            val = float(parts[0])
            values.append(val)
        except ValueError:
            continue  # skip lines that don't parse

# Plot histogram
plt.hist(values, bins=30, edgecolor='black')
plt.xlabel("First value")
plt.ylabel("Frequency")
plt.title("Histogram of first column")
plt.show()
