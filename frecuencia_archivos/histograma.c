#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CSFML/Graphics.h>


typedef struct {
    char palabra[20];
    int frecuencia;
} Dato;

int main() {
    // 1. Cargar datos del CSV
    FILE *archivo = fopen("reporte.csv", "r");
    if (!archivo) {
        perror("No se pudo abrir el CSV");
        return 1;
    }

    Dato datos[32];
    char linea[100];
    int total = 0;

    fgets(linea, 100, archivo); // Saltar encabezado (Palabra,Frecuencia)
    while (fgets(linea, 100, archivo) && total < 32) {
        sscanf(linea, "%[^,],%d", datos[total].palabra, &datos[total].frecuencia);
        total++;
    }
    fclose(archivo);
    
    sfFont* font = sfFont_createFromFile("lmromanslant10-regular.otf");
    if (!font) {
        printf("Error: No se encontro 'arial.ttf'. Descargalo o usa uno de tu sistema.\n");
        return 1;
    }
    
    sfVideoMode mode = {1366, 768, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode,
    											   "Grafica de Palabras Reservadas", 
    											   sfResize | sfClose,
    											   sfWindowed, NULL);

    // 3. Bucle principal
    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        sfRenderWindow_clear(window, sfBlack);

        // Dibujar barras
        float anchoBarra = 700.0f / total;
        for (int i = 0; i < total; i++) {
            float alturaBarra = datos[i].frecuencia * 3.0f;
            
            sfRectangleShape* barra = sfRectangleShape_create();
            sfVector2f tamaño = {anchoBarra - 5, alturaBarra};
            sfVector2f posicion = {50 + (i * anchoBarra), 550 - alturaBarra};

            sfRectangleShape_setSize(barra, tamaño);
            sfRectangleShape_setPosition(barra, posicion);
            sfRectangleShape_setFillColor(barra, sfCyan);
            sfRectangleShape_setOutlineThickness(barra, 1.0f);
            sfRectangleShape_setOutlineColor(barra, sfWhite);
            
            
            // Dibujar la etiqueta (Índice)
            sfText* etiqueta = sfText_create(font);
            sfText_setString(etiqueta, datos[i].palabra);
            sfText_setCharacterSize(etiqueta, 12); // Tamaño pequeño para que quepan
            sfText_setFillColor(etiqueta, sfWhite);
            
            // Posicionar texto debajo de la barra
            sfVector2f posTexto = {3.0f + ((i+1) * anchoBarra), 600};
            sfText_setPosition(etiqueta, posTexto);
            // Rotamos un poco el texto si son muchas palabras
            sfText_setRotation(etiqueta, 270); 

            sfRenderWindow_drawText(window, etiqueta, NULL);

            sfRenderWindow_drawRectangleShape(window, barra, NULL);
            sfRectangleShape_destroy(barra);
            sfText_destroy(etiqueta);
        }

        sfRenderWindow_display(window);
    }
    
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);
    return 0;
}
