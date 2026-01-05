#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <CSFML/System.h>
#include <CSFML/Graphics.h>

const double Radian = 3.1416 / 180.0;
const int Dice = 360;
const int Step = 40;
const int count =20000;

int main(int argc, char **argv) {

	srand(time(NULL));
	
	sfVideoMode mode = {1366, 768, 32};
	sfRenderWindow *window = 
				  sfRenderWindow_create(mode, 
				  					    "Programa 3: Caminante Aleatorio", 
										sfResize | sfClose, 
										sfWindowed, NULL);
	
	int flag = 0, iteration = 0, x = (rand() % 1366) + 1, y = (rand() % 701) + 1;
	sfTime T;
	T.microseconds = 100000;
	sfVector2f v = { x, y};
	sfVertex tmp, start, end;
	
	tmp.position = v;
	tmp.color = sfGreen;
	start.color = sfRed;
	end.color = sfRed;
	
	sfVertexArray *points = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(points, sfLineStrip);
	sfVertexArray_append(points,tmp);
	
	sfVertexArray *trajectory = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(trajectory, sfLineStrip);
	
	while (sfRenderWindow_isOpen(window)) {
		sfEvent event;
		while (sfRenderWindow_pollEvent(window, &event)) {
			if (event.type == sfEvtClosed) sfRenderWindow_close(window);
		}
		
		if(iteration < count){
			int Alpha = rand() % Dice;
			x = (int)(Step * cos(Alpha * Radian)) + x;
			y = (int)(Step * sin(Alpha * Radian)) + y;
			
			
			x = x > 0 ? (x > 1365 ?
					(int)(Step * cos((180 - Alpha) * Radian)) + 1365 : x) : 
					(int)(Step * cos((180 - Alpha) * Radian));
			y = y > 0 ? (y > 701 ? 
					(int)(Step * sin(-1 * Alpha * Radian)) + 701 : y) : 
					(int)(Step * sin(-1 * Alpha * Radian));
			
			v.x = x;
			v.y = y;
			
			tmp.position = v;
			sfVertexArray_append(points,tmp);
			iteration++;	
		}
		else if (iteration == count){
			start.position = sfVertexArray_getVertex(points, 0) -> position;
			end.position = sfVertexArray_getVertex(points, count-1) -> position;
			sfVertexArray_append(trajectory,start);
			sfVertexArray_append(trajectory,end);
			printf("Distancia del punto inicial al final: %.4f unidades\n",
				   sqrt(pow((start.position.x - end.position.x),2)+
						pow((start.position.y - end.position.y),2)));
			iteration++;
		}
		
		
		
		sfRenderWindow_clear(window, sfBlack);
		sfRenderWindow_drawVertexArray(window, points, NULL);
		if(iteration >= count) 
			sfRenderWindow_drawVertexArray(window, trajectory, NULL);
		sfRenderWindow_display(window);
		sfSleep(T);
	}
	
	sfVertexArray_destroy(points);
	sfVertexArray_destroy(trajectory);
	sfRenderWindow_destroy(window);

	return 0;
}
