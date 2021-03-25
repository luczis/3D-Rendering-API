#pragma once
#include "iostream"
#include "fstream"
#include "vector"
#include "stringFunk.h"

struct {
	float* pos;
	float* tex;
	float* norm;
	uint16_t* pos_face;
	uint16_t* tex_face;
	uint16_t* norm_face;
	uint8_t* poly_size;
	uint16_t size = 0;
} model;

#define OBJ_COM		'#'
#define OBJ_POS		"v "
#define OBJ_TEX		"vt"
#define OBJ_NORM	"vn"
#define OBJ_FACE	'f'
#define OBJ_F_S		'/'
#define OBJ_NL		'\n'
#define OBJ_SPC		0x20

uint8_t get_polygon(const char* line) {
	uint8_t result = 0;
	for(uint8_t i=0; (i!=255)&&(line[i]!='\0'); i++)
		if(line[i] == OBJ_F_S)
			result++;
	return result/2;
}

uint16_t line_to_float(uint8_t size, uint16_t pos, float* buffer, const char* line) {
	char subline[256];
	for(uint8_t i=0; (i!=255)&&(line[i]!='\0');) {
		if(((line[i]<0x40)&&(line[i]>=0x30))||(line[i]==0x2D)) {
			uint8_t j=0;
			while((line[i]!='\0')&&(line[i]!=OBJ_SPC))
				subline[j++] = line[i++];
			subline[j] = '\0';
			buffer[pos++] = string_to_float(subline);
		}
		else
			i++;
	}
	return pos;
}

uint16_t line_to_face(uint16_t* v_pos, uint16_t* poly_pos, uint16_t* v_buffer, uint16_t* vt_buffer, uint16_t* vn_buffer, uint8_t* poly_count, const char* line) {
	char subline[256];
	uint16_t temp_pos = *v_pos;
	//printf(line);
	//printf("\n");
	//printf(":%i\n",temp_pos);
	//getchar();
	for(uint8_t i=0; (i!=255)&&(line[i]!='\0')&&(line[i]!='\n');) {
		//printf("<%02x>\n",line[i]);
		if(((line[i]<0x40)&&(line[i]>=0x30))||(line[i]==0x2D)||(line[i]==OBJ_F_S)) {
			// Position
			if(line[i]==OBJ_F_S) {
				v_buffer[temp_pos] = 0;
			} else {
				uint8_t j=0;
				while(line[i]!=OBJ_F_S)
					subline[j++] = line[i++];
				subline[j] = '\0';
				//printf(subline);
				//printf("|");
				v_buffer[temp_pos] = string_to_uint16(subline)-1;
			}
			i++;
	//		printf("I%iI\n",i);
			// Texture
			if(line[i]==OBJ_F_S) {
				vt_buffer[temp_pos] = 0;
			} else {
				uint8_t j=0;
				while(line[i]!=OBJ_F_S)
					subline[j++] = line[i++];
				subline[j] = '\0';
				//printf(subline);
				//printf("|");
				vt_buffer[temp_pos] = string_to_uint16(subline)-1;
			}
			i++;
	//		printf("I%iI\n",i);
			// Normal
			if((line[i]==0x20)||(line[i]=='\0')) {
				vn_buffer[temp_pos] = 0;
			} else {
				uint8_t j=0;
				while((line[i]!=OBJ_SPC)&&(line[i]!='\0'))
					subline[j++] = line[i++];
				subline[j] = '\0';
				//printf(subline);
				//printf("|");
				vn_buffer[temp_pos] = string_to_uint16(subline)-1;
			}
	//		i++;
	//		printf("I%iI\n",i);
			temp_pos++;
	//		printf(":%i",i);
		//	getchar();
		}
		else
			i++;
	}
	
	poly_count[*poly_pos] = temp_pos-(*v_pos);
	*poly_pos = (*poly_pos) + 1;

	return temp_pos;
}

void load_obj_model(const char* file) {
	std::ifstream obj(file, std::ios::in);
	if(!obj)
		return;

	char line[256];
	uint16_t v_count=0, vt_count=0, vn_count=0, f_count=0;
	// Count
	while(!obj.eof()) {
		obj.getline(line, 256);
		switch(line[0]) {
			case OBJ_POS[0]:
				switch(line[1]) {
					case OBJ_POS[1]:	// Position
						v_count++;
						break;
					case OBJ_TEX[1]:	// Texture
						vt_count++;
						break;
					case OBJ_NORM[1]:	// Normal
						vn_count++;
						break;
					default:
						continue;
						break;
				}
				break;
			case OBJ_FACE:	// Face
				f_count++;
				break;
			case OBJ_COM:	// Comment
				printf(line);
				printf("\n");
				break;
			default:
				continue;
				break;
		}
	}
	//printf("%i,%i,%i,%i,%i\n",v_count,vt_count,vn_count,f_count,model.polygon);

	// Allocate memory
	model.pos = (float*)malloc(v_count*3*sizeof(float));
	model.tex = (float*)malloc(vt_count*3*sizeof(float));
	model.norm = (float*)malloc(vn_count*3*sizeof(float));
	model.pos_face = (uint16_t*)malloc(f_count*4*sizeof(uint16_t));
	model.tex_face = (uint16_t*)malloc(f_count*4*sizeof(uint16_t));
	model.norm_face = (uint16_t*)malloc(f_count*4*sizeof(uint16_t));
	model.poly_size = (uint8_t*)malloc(f_count*sizeof(uint8_t));

	printf("Polygons: %i\n", f_count);
	model.size = f_count;

	obj.clear();
	obj.seekg(0, std::ios::beg);
	uint16_t v_pos=0, vt_pos=0, vn_pos=0, f_pos=0, poly_pos=0;
	// Get data
	while(!obj.eof()) {
		obj.getline(line, 256);
		switch(line[0]) {
			case OBJ_POS[0]:
				switch(line[1]) {
					case OBJ_POS[1]:	// Position
						v_pos = line_to_float(3, v_pos, model.pos, line);
						break;
					case OBJ_TEX[1]:	// Texture
						vt_pos = line_to_float(2, vt_pos, model.tex, line);
						break;
					case OBJ_NORM[1]:	// Normal
						vn_pos = line_to_float(3, vn_pos, model.norm, line);
						break;
					default:
						continue;
						break;
				}
				break;
			case OBJ_FACE:	// Face
				f_pos = line_to_face(&f_pos, &poly_pos, model.pos_face, model.tex_face, model.norm_face, model.poly_size, line);
				break;
			default:
				continue;
				break;
		}
	}
	//printf("Poly_pos:%i\n",model.poly_size[48]);
}
