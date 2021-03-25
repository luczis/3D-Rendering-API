#pragma once
#include "iostream"

uint16_t string_to_uint16(char* string) {
	uint16_t result = 0;
	uint16_t tmp_result = 0;
	char current_char = 0x0;
	uint8_t total_size = 0;

	for(total_size=0; (total_size<16) && (string[total_size]!='\0') && (string[total_size]!='\n'); total_size++) {}

	//printf("T%i:",total_size);
	//printf(string);
	//printf("\n");
	for(uint8_t count=0; count<total_size; count++) {
		current_char = string[total_size-1-count];
		if((current_char=='\0')||(current_char=='\n'))
			break;
		tmp_result = current_char&0x0f;
		//printf("%02x|",current_char);
		for(uint8_t i=0; i<count; i++)
			tmp_result *= 10;
		result += tmp_result;
	}

	return result;
}

float string_to_float(char* string) {
	float result = 0.0f;
	float tmp_result = 0;
	char current_char = 0x0;
	uint8_t total_size = 0;
	uint8_t dot_pos = 0;
	bool is_negative = (string[0]=='-') ? true : false;
	string+= is_negative;


	for(total_size=0; (total_size<16) && (string[total_size]!='\0') && (string[total_size]!='\n'); total_size++) {
		if(string[total_size] == '.')
			dot_pos = total_size;
			}

	// To the dot left
	uint8_t count = 0;
	for(; count<dot_pos; count++) {
		current_char = string[count];
		if((current_char=='\0')||(current_char=='\n'))
			break;
		tmp_result = (float)(current_char&0x0f);
		for(uint8_t i=0; i<dot_pos-1-count; i++)
			tmp_result *= 10.0f;
		result += tmp_result;
	}
	
	// To the dot right
	count++;
	for(; count<total_size; count++) {
		current_char = string[count];
		if((current_char=='\0')||(current_char=='\n'))
			break;
		tmp_result = (float)(current_char&0x0f);
		//printf("%02x|",current_char);
		for(uint8_t i=dot_pos; i<count; i++)
			tmp_result /= 10.0f;
		result += tmp_result;
	}

	if(is_negative)	return -result;
	return result;
}
