import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def generar_grafica(archivo_csv):
    try:
        df = pd.read_csv(archivo_csv)
        datos = df['x']

        media = datos.mean()
        lmbda = 1 / media

        plt.figure(figsize=(10, 6))

        n, bins, patches = plt.hist(datos, bins=50, density=True, 
                                     color='#3498db', alpha=0.7, 
                                     edgecolor='white', label='Frecuencia de datos')


        x_teorica = np.linspace(datos.min(), datos.max(), 100)
        y_teorica = lmbda * np.exp(-lmbda * x_teorica)

        plt.plot(x_teorica, y_teorica, color='red', lw=3, 
                 label=f'Curva Teórica ($\lambda$={lmbda:.2f})')

        plt.title('Distribución Exponencial del Dataset', fontsize=14)
        plt.xlabel('Valor de X', fontsize=12)
        plt.ylabel('Densidad de Probabilidad', fontsize=12)
        plt.grid(alpha=0.3)
        plt.legend()

        plt.show()

    except Exception as e:
        print(f"Error al procesar el archivo: {e}")

if __name__ == "__main__":
    generar_grafica('Dataset.csv')
