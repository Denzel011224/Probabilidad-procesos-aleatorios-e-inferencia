#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <CSFML/Graphics.h>

const double Radian = 3.1416 / 180.0;

int main(int argc, char **argv) {

	srand(time(NULL));
	
	sfVideoMode mode = {1366, 768, 32};
	sfRenderWindow *window = sfRenderWindow_create(mode, 
												   "Programa 2: Triangulo de Sierpinski", 
												   sfResize | sfClose, 
												   sfWindowed, NULL);
	sfVector2f v, u;
	sfVertex tmp;
	tmp.color = sfGreen;
	int x, y, step = 0, Alpha = 90, diferencia = 360 / 3;
	sfVertexArray *triangle = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(triangle, sfLineStrip);
	for(int i = 0; i < 4; i++){
		x = (int)(348 * cos(Alpha * Radian)) + 683;
		y = (int)(-348 * sin(Alpha * Radian)) + 352;
		v.x = x;
		v.y = y;
		tmp.position = v;
		sfVertexArray_append(triangle,tmp);
		Alpha += diferencia;
	}
	 
	
	x = 683;
	y = 352;
	
	sfVertexArray *points = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(points, sfPoints);
	while (sfRenderWindow_isOpen(window)) {
		sfEvent event;
		while (sfRenderWindow_pollEvent(window, &event)) {
			if (event.type == sfEvtClosed) sfRenderWindow_close(window);
		}
		v.x = x;
		v.y = y;
		tmp.position = v;
		sfVertexArray_append(points,tmp);
		
		if(step < 1000000){
			int r = rand() % 3;
			x = (int) ((x + 
					   sfVertexArray_getVertex(triangle, r) -> position.x) / 2);
			y = (int) ((y + 
					   sfVertexArray_getVertex(triangle, r) -> position.y) / 2);
		}
		
		step++;
		sfRenderWindow_clear(window, sfBlack);
		sfRenderWindow_drawVertexArray(window, triangle, NULL);
		sfRenderWindow_drawVertexArray(window, points, NULL);
		sfRenderWindow_display(window);
		}
	
	sfVertexArray_destroy(triangle);
	sfVertexArray_destroy(points);
	sfRenderWindow_destroy(window);

	return 0;
}
