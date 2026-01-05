#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALABRAS 32
#define MAX_LEN_PALABRA 100 // Aumentado para evitar desbordamiento

const char *reservadas[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

int main() {
    FILE *archivoEntrada, *archivoSalida;
    char palabra[MAX_LEN_PALABRA];
    int conteo[MAX_PALABRAS] = {0};
    char nombreArchivo[100];

    printf("Introduce el nombre del archivo .c: ");
    if (scanf("%99s", nombreArchivo) != 1) return 1;

    archivoEntrada = fopen(nombreArchivo, "r");
    if (archivoEntrada == NULL) {
        printf("Error: No se pudo abrir el archivo '%s'. Verifica que el nombre sea correcto.\n", nombreArchivo);
        return 1; // Salida segura si el archivo no existe
    }

    // Leemos palabra por palabra limitando el tamaño para evitar SegFault
    while (fscanf(archivoEntrada, "%99s", palabra) != EOF) {
        char palabraLimpia[MAX_LEN_PALABRA] = "";
        int j = 0;

        for (int i = 0; palabra[i] != '\0'; i++) {
            if (isalpha((unsigned char)palabra[i])) {
                palabraLimpia[j++] = palabra[i];
            }
        }
        palabraLimpia[j] = '\0';

        if (strlen(palabraLimpia) > 0) {
            for (int i = 0; i < MAX_PALABRAS; i++) {
                if (strcmp(palabraLimpia, reservadas[i]) == 0) {
                    conteo[i]++;
                    break;
                }
            }
        }
    }

    fclose(archivoEntrada);

    archivoSalida = fopen("reporte.csv", "w");
    if (archivoSalida == NULL) {
        printf("Error al crear el archivo CSV.\n");
        return 1;
    }

    fprintf(archivoSalida, "Palabra,Frecuencia\n");
    for (int i = 0; i < MAX_PALABRAS; i++) {
        if (conteo[i] > 0) {
            fprintf(archivoSalida, "%s,%d\n", reservadas[i], conteo[i]);
        }
    }

    fclose(archivoSalida);
    printf("¡Éxito! Resultados guardados en 'reporte.csv'.\n");

    return 0;
}
