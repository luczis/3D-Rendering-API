#pragma once
#include "iostream"
#include "fstream"
#include "vector"
#include "stringFunk.h"

struct {
	uint16_t* buffer;
	uint16_t width;
	uint16_t height;
} texture_ppm;

#define PPM_B_ID	'P'		// PPM base identifier
#define PPM_6_ID	'6'		// PPM 6 identifier
#define PPM_COM		'#'		// Comment
#define PPM_NL		0x0A	// New Line
#define PPM_SPC		0x20	// Space

// Comment starts with '#' and ends with a '\n'
void ppm_comment(std::ifstream &file, char file_byte) {
	while((file_byte&0xff) != PPM_NL) {
		printf("%c", file_byte);
		file.read(&file_byte, 1);
	}
	printf("\n");
}

uint8_t read_uint8(std::ifstream &file) {
	char file_byte = 0x0;
	file.read(&file_byte, 1);
	//printf("%02x,",file_byte&0xff);

	return file_byte&0xff;
}

void load_ppm_texture(const char* file) {
	std::ifstream image(file, std::ios::in | std::ios::binary);
	if(!image)
		return;

	uint8_t file_byte = 0x0;
	uint16_t color_depth = 0;

	// Verify if valid PPM file
	// Currently only support for P6
	file_byte = read_uint8(image);
	if(file_byte != PPM_B_ID)
		goto function_end;
	file_byte = read_uint8(image);
	if(file_byte != PPM_6_ID)
		goto function_end;

	if(!image.eof()) {
		while(file_byte != PPM_NL)
			file_byte = read_uint8(image);
	
		// Comment
		file_byte = read_uint8(image);
		if(file_byte == PPM_COM) {
			ppm_comment(image, file_byte);
		}

		// Size
		char string[16];
		uint8_t i = 0;

		// Width
		file_byte = read_uint8(image);
		while(file_byte != PPM_SPC) {
			string[i++] = file_byte;
			file_byte = read_uint8(image);
		}
		string[i] = 0x0A;
		texture_ppm.width = string_to_uint16(string);
		printf("Width: %i\n",texture_ppm.width);

		// Height
		i = 0;
		file_byte = read_uint8(image);
		while(file_byte != PPM_NL) {
			string[i++] = file_byte;
			file_byte = read_uint8(image);
		}
		string[i] = 0x0A;
		texture_ppm.height = string_to_uint16(string);
		printf("Height: %i\n",texture_ppm.height);

		// Color depth
		i = 0;
		file_byte = read_uint8(image);
		while(file_byte != PPM_NL) {
			string[i++] = file_byte;
			file_byte = read_uint8(image);
		}
		string[i] = 0x0A;
		color_depth = string_to_uint16(string);

		printf("Color Depth: %i\n",color_depth);


		// Data
		uint32_t data_size = texture_ppm.width * texture_ppm.height;
		texture_ppm.buffer = (uint16_t*) calloc(1,2*data_size);
		for(uint32_t i=0; i<data_size; i++) {
			if(image.eof()) { // Error
				printf("Error:: Image EOF reached\n");
				free(texture_ppm.buffer);
				goto function_end;
			}
			uint16_t color = 0;
			file_byte = read_uint8(image);
			color |= ((file_byte&0xff)<<0x8)&0xf800; // Red
			file_byte = read_uint8(image);
			color |= ((file_byte&0xff)<<0x3)&0x07e0; // Green
			file_byte = read_uint8(image);
			color |= ((file_byte&0xff)>>0x3)&0x001f; // Blue

			texture_ppm.buffer[i] = color;
		}
		//free(texture_ppm.buffer);
	}

function_end:
	image.close();
	return;
}
