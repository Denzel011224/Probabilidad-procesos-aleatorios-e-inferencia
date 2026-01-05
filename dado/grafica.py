import pandas as pd
import matplotlib.pyplot as plt
import sys

def generar_grafica(archivo_csv):
    try:
        # Leer el CSV
        df = pd.read_csv(archivo_csv)
        
        # Contar frecuencias
        frecuencias = df['Resultado']
        
        # Crear la gráfica
        plt.figure(figsize=(10, 6))
        frecuencias.plot(kind='bar', color='skyblue', edgecolor='black')
        
        plt.title('Frecuencia de Resultados del Dado')
        plt.xlabel('Cara del Dado')
        plt.ylabel('Cantidad de veces')
        plt.grid(axis='y', linestyle='--', alpha=0.7)
        
        # Mostrar
        print("Gráfica guardada como 'grafica_resultados.png'")
        plt.show()
        
    except Exception as e:
        print(f"Error al graficar: {e}")

if __name__ == "__main__":
    generar_grafica('resultados_dados.csv')
