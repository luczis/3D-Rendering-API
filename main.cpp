//
//  Raster Project
//
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

/* Global */
static const int
WIN_WIDTH = 1900,
WIN_HEIGHT = 1040;
bool current_buffer = 0;
uint16_t raster_buffer0[WIN_WIDTH*WIN_HEIGHT];
uint16_t raster_buffer1[WIN_WIDTH*WIN_HEIGHT];
uint16_t* drawing_buffer = raster_buffer0;
uint16_t* hidden_buffer = raster_buffer1;
uint16_t depth_buffer[WIN_WIDTH*WIN_HEIGHT];

#include "mathFunk.h"
#include "imagePpmFunk.h"
#include "rasterFunk.h"

// Draw raster
void drawRaster()
{
	uint16_t x,y;
    
	/* Clear Window */
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
    
	/* Draw Raster */
	uint16_t pixel_color;
	for (y = 0; y < WIN_HEIGHT; y++){
		for (x = 0; x < WIN_WIDTH; x++) {
			// RGB565 to 3 color 1 byte each
			pixel_color = drawing_buffer[x+WIN_WIDTH*y];
			if(pixel_color!=0xffff) {
    			glColor3ub(	(uint8_t)((pixel_color&0xf800)>>0x8),
							(uint8_t)((pixel_color&0x07e0)>>0x3),
							(uint8_t)((pixel_color&0x001f)<<0x3));
				glVertex2i(x, y);
			}
		}
	}

	glEnd();
}

void display()
{
	// Swap buffers
	drawing_buffer = current_buffer ? raster_buffer0: raster_buffer1;
	hidden_buffer = current_buffer ? raster_buffer1: raster_buffer0;
	current_buffer ^= 1;

	// Clear buffers for drawing
	uint16_t x,y;
	for (y = 0; y < WIN_HEIGHT; y++){
		for (x = 0; x < WIN_WIDTH; x++) {
			hidden_buffer[x+WIN_WIDTH*y] = (uint16_t)(0xffff);
			depth_buffer[x+WIN_WIDTH*y] = (uint16_t)(0xffff);
		}
	}


	/*uint16_t x0,x1,x2,x3,y0,y1,y2,y3;
	x0=(uint16_t)(rand()%WIN_WIDTH),
	y0=(uint16_t)(rand()%WIN_HEIGHT),
	x1=(uint16_t)(rand()%WIN_WIDTH),
	y1=(uint16_t)(rand()%WIN_HEIGHT),
	x2=(uint16_t)(rand()%WIN_WIDTH),
	y2=(uint16_t)(rand()%WIN_HEIGHT),
	x3=(uint16_t)(rand()%WIN_WIDTH),
	y3=(uint16_t)(rand()%WIN_HEIGHT),
	//draw_triangle(x0,y0,x1,y1,x2,y2,0x4ef2,0x1f);
	draw_square(x0,y0,x1,y1,x2,y2,x3,y3,0x4ef2,0x1f);
	std::cout	<<x0<<","
				<<y0<<","
				<<x1<<","
				<<y1<<","
				<<x2<<","
				<<y2<<","
				<<x3<<","
				<<y3<<"\n";*/
	//draw_triangle(536,705,351,782,589,444,0x4ff2,0x1f);

	//draw_square(1621,75,1157,957,1649,786,919,680,0x4ef2,0x1f);
	//drawline(589,444,351,782,0x00,0x7e00);

	cplx<float> a;

	//std::cout<< "abc";
	vertex v0,v1,v2,v3;
	v0.x = 0.5f;
	v0.y = 0.9f;
	v0.z = -1.0f;
	v0.uvx = 0.0f;
	v0.uvy = 0.0f;
	v1.x = 0.0f;
	v1.y = 0.9f;
	v1.z = 0.9f;
	v1.uvx = 1.0f;
	v1.uvy = 0.0f;
	v2.x = -0.2f;
	v2.y = 0.0f;
	v2.z = 0.0f;
	v2.uvx = 1.0f;
	v2.uvy = 1.0f;
	v3.x = 0.9f;
	v3.y = 0.0f;
	v3.z = 0.0f;
	v3.uvx = 0.0f;
	v3.uvy = 1.0f;
	draw_triangle_image(v0,v1,v2);
	draw_triangle_image(v0,v2,v3);
	v0.y -= 0.5f;
	v1.y -= 0.5f;
	v2.y -= 1.0f;
	v3.y -= 1.0f;
	draw_square_image(v0,v1,v2,v3);

	drawRaster();

	// Swap and call display function
	glutSwapBuffers();
	//glutPostRedisplay();
}

void timer(int t)
{
	glutPostRedisplay();
	glutTimerFunc(1000, timer, 0);
}

void mouse(int button, int state, int x, int y)
{
}

void init()
{
	/* Initializations */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Raster");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WIN_WIDTH, 0.0, WIN_HEIGHT);
    
	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutMouseFunc(mouse);

	srand(time(NULL));

	// Init frame buffer
	uint16_t x,y;
	for (y = 0; y < WIN_HEIGHT; y++){
		for (x = 0; x < WIN_WIDTH; x++) {
			drawing_buffer[x+WIN_WIDTH*y] = (uint16_t)(0xffff);
			hidden_buffer[x+WIN_WIDTH*y] = (uint16_t)(0xffff);
			depth_buffer[x+WIN_WIDTH*y] = (uint16_t)(0xffff);
		}
	}

	// Load object
	load_ppm_texture("resources/Landscape.ppm");

}


int main(int argc, char ** argv)
{
	glutInit(&argc , argv);
	init();
	glutMainLoop();

	return 0;
}
