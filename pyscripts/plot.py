import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

ktest = 1.1
xtest = -131.947
# --- Load data ---
data = np.loadtxt("/home/ndutoit/Solitons/BenjaminOno/out/output0/poles.res")

times = data[:, 0]
poles = data[:, 1:]  # shape (T, 2N)
N = poles.shape[1] // 2

x = np.linspace(-10000, 10000, 20000)


def density(row, x):
    xi = row[0::2]  # x1, x2, ..., xN
    yi = row[1::2]  # y1, y2, ..., yN
    return sum(1.0 / ((x - xi[i])**2 + yi[i]**2) for i in range(N))

def soliton_unique(t, x):
    pole = +1j/ktest + ktest*t + xtest
    polestar = -1j/ktest + ktest*t + xtest
    return -1j/(x-pole) + 1j/(x-polestar)

# --- Setup figure ---
fig, ax = plt.subplots(figsize=(9, 4))
line, = ax.plot([], [], lw=1.5)
line2, = ax.plot([], [], lw=1.5, color='red')
title = ax.set_title("")
ax.set_xlabel("x")
ax.set_ylabel(r"$\sum_i \frac{1}{(x-x_i)^2 + y_i^2}$")
ax.set_xlim(x[0], x[-1])

# Compute y range from a few frames for stable limits
sample = np.array([density(poles[i], x) for i in range(0, len(times), max(1, len(times)//20))])
ax.set_ylim(0, 3)

def init():
    line.set_data([], [])
    line2.set_data([], [])
    return line, title

def update(frame):
    t = times[frame]
    ax.set_xlim(ktest*t+xtest - 500, ktest*t+xtest+500)
    y = density(poles[frame], x)
    yu = soliton_unique(times[frame], x)
    line.set_data(x, y)
    line2.set_data(x, yu)
    title.set_text(f"t = {times[frame]:.4f}")
    return line, line2, title

ani = animation.FuncAnimation(fig, update, frames=len(times),
                               init_func=init, interval=30, blit=True)
plt.tight_layout()
plt.show()
