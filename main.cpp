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
#include "modelFunk.h"
#include "rasterFunk.h"

/*float cube_pos[3*8] = {
-0.5f,-0.5f, 0.0f,
 0.0f,-1.0f,-1.0f,
-0.5f, 0.5f, 0.0f,
 0.0f, 0.0f,-1.0f,
 0.0f, 0.0f, 1.0f,
 0.5f,-0.5f, 0.0f,
 0.0f, 1.0f, 1.0f,
 0.5f, 0.5f, 0.0f};*/

float cube_pos[3*8] = {
-1.0f,-1.0f, 0.0f,
 0.0f,-1.0f,-1.0f,
-1.0f, 1.0f, 0.0f,
 0.0f, 1.0f,-1.0f,
 0.0f,-1.0f, 1.0f,
 1.0f,-1.0f, 0.0f,
 0.0f, 1.0f, 1.0f,
 1.0f, 1.0f, 0.0f};

float cube_tex[2*4] = {
0.0f, 0.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 1.0f};

uint16_t face_order[4*6] = {
0,1,3,2,
2,3,7,6,
0,1,5,4,
0,4,6,2,
1,5,7,3,
4,5,7,6};

uint16_t tex_order[4*6] = {
2,3,1,0,
2,3,1,0,
2,3,1,0,
2,3,1,0,
2,3,1,0,
2,3,1,0};

float model_matrix[4*4] = {
0.8f, 0.0f, 0.0f, 0.0f,
0.0f, 0.8f, 0.0f, 0.0f,
0.0f, 0.0f, 0.8f, 0.0f,
0.0f, 0.0f, 0.0f, 0.8f};

float projection_matrix[4*4] = {
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f};

		float view_matrix[4*4] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

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

	static float translate_vector[3] = 	{0.0f,0.0f,-3.0f};
	static float scale_vector[3] = 		{0.5f,0.5f,0.5f};
	static float rotate_vector[3] = 	{0.0f,1.0f,0.0f};
	static float rotate_angle = 0.5f;
	//translate_vector[2] *= 1.01f;
	rotate_angle += 0.01f;
	// Draw model
	for(uint16_t i=0, j=0; i<model.size; i++) {
	//		{
	//	uint16_t i=4;
	/*	float pos0[4] = {cube_pos[3*face_order[i]], cube_pos[3*face_order[i]+1], cube_pos[3*face_order[i]+2],1.0f};
		float uv0[2] = {cube_tex[2*tex_order[i]], cube_tex[2*tex_order[i]+1]};
		i++;
		float pos1[4] = {cube_pos[3*face_order[i]], cube_pos[3*face_order[i]+1], cube_pos[3*face_order[i]+2],1.0f};
		float uv1[2] = {cube_tex[2*tex_order[i]], cube_tex[2*tex_order[i]+1]};
		i++;
		float pos2[4] = {cube_pos[3*face_order[i]], cube_pos[3*face_order[i]+1], cube_pos[3*face_order[i]+2],1.0f};
		float uv2[2] = {cube_tex[2*tex_order[i]], cube_tex[2*tex_order[i]+1]};
		i++;
		float pos3[4] = {cube_pos[3*face_order[i]], cube_pos[3*face_order[i]+1], cube_pos[3*face_order[i]+2],1.0f};
		float uv3[2] = {cube_tex[2*tex_order[i]], cube_tex[2*tex_order[i]+1]};
		i++;*/
		
		float identity_matrix[4*4] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

	//	math::mat_mult<float>(view_matrix, identity_matrix, identity_matrix, 4, 4, 4);

		// Model matrix
		model3d::scale<float>(identity_matrix, scale_vector, identity_matrix);
		model3d::rotate<float>(identity_matrix, rotate_vector, rotate_angle, identity_matrix);
		model3d::translate<float>(identity_matrix, translate_vector, identity_matrix);
	//	math::mat_mult<float>(model_matrix, identity_matrix, identity_matrix, 4, 4, 4);

		// Projection matrix
		math::mat_mult<float>(projection_matrix, identity_matrix, identity_matrix, 4, 4, 4);

		// Vertex
	//	math::mat_mult<float>(pos0, identity_matrix, pos0, 4, 4, 1);
	//	math::mat_mult<float>(pos1, identity_matrix, pos1, 4, 4, 1);
	//	math::mat_mult<float>(pos2, identity_matrix, pos2, 4, 4, 1);
	//	math::mat_mult<float>(pos3, identity_matrix, pos3, 4, 4, 1);
	//	for(uint8_t j=0; j<4; j++)
	//		printf("%f,",pos0[j]);
	//	printf("A\n");
		float pos0[4] = {model.pos[3*model.pos_face[j]], model.pos[3*model.pos_face[j]+1], model.pos[3*model.pos_face[j]+2],1.0f};
		float uv0[2] = {model.tex[2*model.tex_face[j]], model.tex[2*model.tex_face[j]+1]};
		math::mat_mult<float>(identity_matrix, pos0, pos0, 4, 4, 1);
		model3d::divideW<float>(pos0, pos0);
		j++;
		float pos1[4] = {model.pos[3*model.pos_face[j]], model.pos[3*model.pos_face[j]+1], model.pos[3*model.pos_face[j]+2],1.0f};
		float uv1[2] = {model.tex[2*model.tex_face[j]], model.tex[2*model.tex_face[j]+1]};
		math::mat_mult<float>(identity_matrix, pos1, pos1, 4, 4, 1);
		model3d::divideW<float>(pos1, pos1);
		j++;
		float pos2[4] = {model.pos[3*model.pos_face[j]], model.pos[3*model.pos_face[j]+1], model.pos[3*model.pos_face[j]+2],1.0f};
		float uv2[2] = {model.tex[2*model.tex_face[j]], model.tex[2*model.tex_face[j]+1]};
		math::mat_mult<float>(identity_matrix, pos2, pos2, 4, 4, 1);
		model3d::divideW<float>(pos2, pos2);
		j++;

		if(model.poly_size[i] == 4) {
		//if(model.poly_size[i] == 0) {
			float pos3[4] = {model.pos[3*model.pos_face[j]], model.pos[3*model.pos_face[j]+1], model.pos[3*model.pos_face[j]+2],1.0f};
			float uv3[2] = {model.tex[2*model.tex_face[j]], model.tex[2*model.tex_face[j]+1]};
			math::mat_mult<float>(identity_matrix, pos3, pos3, 4, 4, 1);
			model3d::divideW<float>(pos3, pos3);
			j++;
			draw_square_image(pos0, pos1, pos2, pos3, uv0, uv1, uv2, uv3);
		}
//		else if(model.poly_size[i] == 3)

		/*printf("X\n");
		for(uint8_t l=0; l<model.size; l++)
			printf("%i,",model.pos_face[l]);
		printf("\n");

		printf("A\n");
		for(uint8_t l=0; l<4; l++) {
			for(uint8_t k=0; k<4; k++)
				printf("%f,",identity_matrix[4*l+k]);
			printf("\n");
		}
		printf("B\n");
		for(uint8_t l=0; l<4; l++)
			printf("%f,",pos0[l]);
		printf("\n");
		
		for(uint8_t l=0; l<4; l++)
			printf("%f,",pos1[l]);
		printf("\n");
		for(uint8_t l=0; l<4; l++)
			printf("%f,",pos2[l]);
		printf("\n");
	//	for(uint8_t j=0; j<4; j++)
	//		printf("%f,",pos3[j]);
	//	printf("\n");
		getchar();*/

	}

	drawRaster();

	// Swap and call display function
	glutSwapBuffers();
	glutPostRedisplay();
}

void timer(int t)
{
	//glutPostRedisplay();
	//glutTimerFunc(1000, timer, 0);
}

void mouse(int button, int state, int x, int y)
{
}

void init()
{

	model3d::projection<float>(projection_matrix, WIN_WIDTH, WIN_HEIGHT, 90.0f, 1.0f, -10.0f);

	//printf("%f\n",projection_matrix[4*3+3]);
	// Initializations
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
	load_obj_model("resources/cube.obj");
}

int main(int argc, char ** argv)
{
	glutInit(&argc , argv);
	init();
	glutMainLoop();

	return 0;
}
