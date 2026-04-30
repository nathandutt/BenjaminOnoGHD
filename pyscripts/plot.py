import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# --- Load data ---
data = np.loadtxt("../output1/poles.res")
times = data[:, 0]
poles = data[:, 1:]  # shape (T, 2N)
N = poles.shape[1] // 2

x = np.linspace(-20, 100, 1000)

def density(row, x):
    xi = row[0::2]  # x1, x2, ..., xN
    yi = row[1::2]  # y1, y2, ..., yN
    return sum(1.0 / ((x - xi[i])**2 + yi[i]**2) for i in range(N))

# --- Setup figure ---
fig, ax = plt.subplots(figsize=(9, 4))
line, = ax.plot([], [], lw=1.5)
title = ax.set_title("")
ax.set_xlabel("x")
ax.set_ylabel(r"$\sum_i \frac{1}{(x-x_i)^2 + y_i^2}$")
ax.set_xlim(x[0], x[-1])

# Compute y range from a few frames for stable limits
sample = np.array([density(poles[i], x) for i in range(0, len(times), max(1, len(times)//20))])
ax.set_ylim(0, np.percentile(sample, 98) * 1.1)

def init():
    line.set_data([], [])
    return line, title

def update(frame):
    y = density(poles[frame], x)
    line.set_data(x, y)
    title.set_text(f"t = {times[frame]:.4f}")
    return line, title

ani = animation.FuncAnimation(fig, update, frames=len(times),
                               init_func=init, interval=30, blit=True)
plt.tight_layout()
plt.show()
