from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

# 1. Cargar datos
CSV_PATH = Path(__file__).resolve().parents[2] / "data" / "simulation_data3.csv"
df = pd.read_csv(CSV_PATH)

df.columns=df.columns.str.strip()
print("Nombres encontrados: ", df.columns)

# 2. Configurar la figura profesional
plt.figure(figsize=(10, 6)) 
plt.title("PID Controller Analysis (kp=150.0, ki=20.0, kd=5.0, dt=0.1)", fontsize=14)

# 3. Graficar (SetPoint como linea punteada, Nivel Real como linea sólida)
plt.plot(df['Time(s)'], df['SetPoint(m)'], label='SetPoint', linestyle='--', color='red', alpha=0.7)
plt.plot(df['Time(s)'], df['Real_Level(m)'], label='Level', linewidth=2, color='blue')

# 4. Configurar Ejes (Tus requerimientos)
plt.xlim(0, 300)
plt.ylim(0, 2)
plt.xlabel("Time (s)")
plt.ylabel("Level (m)")
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend()

# 5. Guardar o mostrar
plt.show()