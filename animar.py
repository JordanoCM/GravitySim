import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
from matplotlib.widgets import Button

# Lê o histórico gerado pelo C
frames = []
current_frame = []

try:
    with open("historico.txt", "r") as f:
        for line in f:
            if "FRAME" in line:
                if current_frame:
                    frames.append(current_frame)
                current_frame = []
            else:
                parts = line.split()
                if parts:
                    current_frame.append((float(parts[0]), float(parts[1])))
        if current_frame:
            frames.append(current_frame)
except FileNotFoundError:
    print("Execute o programa C primeiro para gerar o arquivo 'historico.txt'.")
    exit(1)

if not frames:
    print("Nenhum frame encontrado no histórico.")
    exit(1)

fig, ax = plt.subplots(figsize=(6, 6.5))
plt.subplots_adjust(bottom=0.15)
ax.grid(True)
ax.set_title("Simulação N-Body (Gravidade 2D)")

scat = ax.scatter([], [], s=120, c='blue', edgecolors='black')

def init():
    scat.set_offsets(np.empty((0, 2)))
    return scat,

def update(frame_idx):
    data = frames[frame_idx]
    scat.set_offsets(data)
    
    xs = [p[0] for p in data]
    ys = [p[1] for p in data]
    max_val = max(max(abs(x) for x in xs), max(abs(y) for y in ys), 0.2)
    ax.set_xlim(-max_val * 1.5, max_val * 1.5)
    ax.set_ylim(-max_val * 1.5, max_val * 1.5)
    return scat,

# Intervalo fixo em milissegundos para controlar a velocidade
INTERVAL_MS = 100

ani = animation.FuncAnimation(fig, update, frames=len(frames), init_func=init, interval=INTERVAL_MS, repeat=False, blit=False)

ax_button = plt.axes([0.4, 0.03, 0.2, 0.07])
btn_restart = Button(ax_button, 'Reiniciar')

def restart(event):
    ani.frame_seq = ani.new_frame_seq()
    if ani.event_source is None:
        ani.event_source = fig.canvas.new_timer(interval=INTERVAL_MS)
        ani.event_source.add_callback(ani._step)
    ani.event_source.start()

btn_restart.on_clicked(restart)

plt.show()