#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dado.h"

int main() {
    int n, i;
    FILE *archivo;

    // 1. Solicitar parámetros al usuario
    printf("Introduce el número de caras del dado (n): ");
    scanf("%d", &n);
    printf("Introduce el número de veces a tirar el dado (i): ");
    scanf("%d", &i);
    
    int resultados[n];
    
    for (int j = 0; j < n; j++)
        resultados[j] = 0;

    if (n <= 0 || i <= 0) {
        printf("Error: Los parámetros deben ser mayores a 0.\n");
        return 1;
    }

    // 2. Abrir (o crear) el archivo CSV para escritura
    archivo = fopen("resultados_dados.csv", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    // Escribir el encabezado del CSV
    fprintf(archivo, "Tiro,Resultado\n");

    // 3. Inicializar la semilla para números aleatorios
    srand(time(NULL));

    // 4. Simular los tiros y guardar en el archivo
    printf("\nSimulando %d tiros de un dado de %d caras...\n", i, n);
    
    for (int j = 1; j <= i; j++) {
        // Generar número entre 1 y n
        int resultado = lanzamiento(n);
        resultados[resultado-1]++;
    }
    
    for (int j = 0; j < n; j++)
        fprintf(archivo, "%d,%d\n", j+1, resultados[j]);

    // 5. Cerrar el archivo y finalizar
    fclose(archivo);
    printf("\nCSV generado. Iniciando script de Python para graficar...\n");

    // Ejecución de comando bash desde C
    // Asegúrate de usar 'python' o 'python3' según tu sistema
    system("python3 grafica.py");

    return 0;
}
