#include <cmath>
#include <cstdio>
#include <cstdlib>


/* Integer raster */

/*
// Bresenham Algorithm for points
void drawline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t val0, uint16_t val1) {
	short dx, dy;
	uint16_t x, y;


	dx=x1-x0;
	dy=y1-y0;
	float der=(float)dy/dx;
	if(der<0)
		der = -der;
	float er=0.0;

	x=x0;
	y=y0;

	uint16_t current_col=val0;
	uint16_t delta_col=0;
	short sign = 0;
	if(der<1) {
		sign = dy>0 ? 1 : -1;
		if(x0>x1) {
			drawline(x1,y1,x0,y0,val1,val0);
			return;
		}
		delta_col = (val1-val0)/(x1-x0);
		for(x=x0; x<=x1; x++) {
			current_col += delta_col;
			raster_buffer[x+WIN_WIDTH*y] = current_col;
			er += der;
			if(er>0.5) {
				y += sign;
				er -= 1.0;
			}
		}
	}
	else {
		sign = dx>0 ? 1 : -1;
		if(y0>y1) {
			drawline(x1,y1,x0,y0,val1,val0);
			return;
		}
		der = 1/der;
		delta_col = (val1-val0)/(y1-y0);
		for(y=y0; y<=y1; y++) {
			current_col += delta_col;
			raster_buffer[x+WIN_WIDTH*y] = current_col;
			er += der;
			if(er>0.5) {
				x += sign;
				er -= 1.0;
			}
		}
	}
}

// Scan line for single shaded polygon
void fill_polygon(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	uint16_t i_y, f_y;
	bool concave;
	for(uint16_t x=x0; x<x1; x++) {
		i_y = -1; f_y = -1;
		concave = false;
		for(uint16_t y=y0; y<y1; y++) {
			if(raster_buffer[x+WIN_WIDTH*y] != 0xffff) {
				if((i_y == (uint16_t)-1) || (i_y == y-1)) {
					i_y = y;
				} else {
					f_y = y;
					for(;y<y1;y++) {
						if(raster_buffer[x+WIN_WIDTH*y] != 0xffff)
							concave = true;
					}
				}
			}
		}
		if((i_y != (uint16_t)-1) && (f_y != (uint16_t)-1)) {
			uint16_t current_val = raster_buffer[x+WIN_WIDTH*i_y];
			uint16_t delta_val = (raster_buffer[x+WIN_WIDTH*f_y]-current_val)/(f_y-i_y);
			for(uint16_t y=i_y+1; y<f_y; y++) {
				current_val += delta_val;
				raster_buffer[x+WIN_WIDTH*y] = current_val;
			}
			if(concave)
				x--;
		}

	}
}


// Draw single shaded triangle
void draw_triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t fill_color, uint16_t wire_color) {
	uint16_t bx, sx, by, sy;
	bx = x0>x1 ? x0 : x1;
	bx = bx>x2 ? bx : x2;
	sx = x0<x1 ? x0 : x1;
	sx = sx<x2 ? sx : x2;
	by = y0>y1 ? y0 : y1;
	by = by>y2 ? by : y2;
	sy = y0<y1 ? y0 : y1;
	sy = sy<y2 ? sy : y2;
	for(uint16_t x=sx; x<bx+1; x++)
		for(uint16_t y=sy; y<by+1; y++)
			raster_buffer[x+WIN_WIDTH*y] = 0xffff;

	// Draw fill
	drawline(x0,y0,x1,y1,fill_color,fill_color);
	drawline(x1,y1,x2,y2,fill_color,fill_color);
	drawline(x2,y2,x0,y0,fill_color,fill_color);
	fill_polygon(sx,sy,bx+1,by+1);

	// Draw wireframe
	drawline(x0,y0,x1,y1,wire_color,wire_color);
	drawline(x1,y1,x2,y2,wire_color,wire_color);
	drawline(x2,y2,x0,y0,wire_color,wire_color);
}


// Draw single shaded square
void draw_square(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t fill_color, uint16_t wire_color) {
	uint16_t bx, sx, by, sy;
	bx = x0>x1 ? x0 : x1;
	bx = bx>x2 ? bx : x2;
	bx = bx>x3 ? bx : x3;
	sx = x0<x1 ? x0 : x1;
	sx = sx<x2 ? sx : x2;
	sx = sx<x3 ? sx : x3;
	by = y0>y1 ? y0 : y1;
	by = by>y2 ? by : y2;
	by = by>y3 ? by : y3;
	sy = y0<y1 ? y0 : y1;
	sy = sy<y2 ? sy : y2;
	sy = sy<y3 ? sy : y3;
	for(uint16_t x=sx; x<bx+1; x++)
		for(uint16_t y=sy; y<by+1; y++)
			hidden_buffer[x+WIN_WIDTH*y] = 0xffff;

	// Draw fill
	drawline(x0,y0,x1,y1,fill_color,fill_color);
	drawline(x1,y1,x2,y2,fill_color,fill_color);
	drawline(x2,y2,x3,y3,fill_color,fill_color);
	drawline(x3,y3,x0,y0,fill_color,fill_color);
	fill_polygon(sx,sy,bx+1,by+1);

	// Draw wireframe
	drawline(x0,y0,x1,y1,wire_color,wire_color);
	drawline(x1,y1,x2,y2,wire_color,wire_color);
	drawline(x2,y2,x3,y3,wire_color,wire_color);
	drawline(x3,y3,x0,y0,wire_color,wire_color);
}
*/

/* Floating raster */

struct vertex{
	float x;
	float y;
	float z;
	float uvx;
	float uvy;
};

float fbx_grid[WIN_WIDTH*(WIN_HEIGHT+1)];
float fby_grid[WIN_WIDTH*(WIN_HEIGHT+1)];
float depth_grid[WIN_WIDTH*(WIN_HEIGHT+1)];

#define LTO_F 0.999999940395355224609375f
//#define LTO_F 1.0f

// Bresenham Algorithm for points
void drawlinef(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, float val0, float val1, float* sel_grid) {
	short dx=x1-x0, dy=y1-y0;
	uint16_t x=x0, y=y0;

	float current_val=val0;
	float delta_val=0;

	// Special cases
	if(dx==0) {
		if(y0>y1) {
			drawlinef(x1,y1,x0,y0,val1,val0,sel_grid);
			return;
		}
		delta_val = (val1-val0)/dy;
		for(y=y0; y<=y1; y++) {
			current_val += delta_val;
			sel_grid[x+WIN_WIDTH*y] = current_val;
		}
		return;
	}
	if(dy==0) {
		if(x0>x1) {
			drawlinef(x1,y1,x0,y0,val1,val0,sel_grid);
			return;
		}
		delta_val = (val1-val0)/dx;
		for(x=x0; x<=x1; x++) {
			current_val += delta_val;
			sel_grid[x+WIN_WIDTH*y] = current_val;
		}
		return;
	}

	short sign;
	float er=0.0;
	float der=(float)dy/dx;
	if(der<0)
		der = -der;

	if(der<1.0f) {
		if(x0>x1) {
			drawlinef(x1,y1,x0,y0,val1,val0,sel_grid);
			return;
		}
		sign = dy>0 ? 1 : -1;
		delta_val = (val1-val0)/(dx);
		for(x=x0; x<=x1; x++) {
			current_val += delta_val;
			sel_grid[x+WIN_WIDTH*y] = current_val;
			er += der;
			if(er>0.5) {
				y += sign;
				er -= 1.0;
			}
		}
	}
	else {
		if(y0>y1) {
			drawlinef(x1,y1,x0,y0,val1,val0,sel_grid);
			return;
		}
		sign = dx>0 ? 1 : -1;
		der = 1/der;
		delta_val = (val1-val0)/(dy);
		for(y=y0; y<=y1; y++) {
			current_val += delta_val;
			sel_grid[x+WIN_WIDTH*y] = current_val;
			er += der;
			if(er>0.5) {
				x += sign;
				er -= 1.0;
			}
		}
	}
}

// Scan line for single shaded polygon
void fill_polygonf(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, float* sel_grid) {
	uint16_t i_y, f_y;
	bool concave;
	for(uint16_t x=x0; x<x1; x++) {
		i_y = -1; f_y = -1;
		concave = false;
		for(uint16_t y=y0; y<y1; y++) {
			if(!std::isnan(sel_grid[x+WIN_WIDTH*y])) {
				if((i_y == (uint16_t)-1) || (i_y == y-1)) {
					i_y = y;
				} else {
					f_y = y;
					for(;y<y1;y++) {
						if(!std::isnan(sel_grid[x+WIN_WIDTH*y]))
							concave = true;
					}
				}
			}
		}
		if((i_y != (uint16_t)-1) && (f_y != (uint16_t)-1)) {
			float current_val = sel_grid[x+WIN_WIDTH*i_y];
			float delta_val = (sel_grid[x+WIN_WIDTH*f_y]-current_val)/(f_y-i_y);
			//sel_grid[x+WIN_WIDTH*i_y] = nanf("");
			//if(current_val <= 0.0f) current_val = 0.01f;
			for(uint16_t y=i_y+1; y<f_y; y++) {
				current_val += delta_val;
				sel_grid[x+WIN_WIDTH*y] = current_val;
			}
			if(concave)
				x--;
		}
	}
}

// Render triangle with texture
void draw_triangle_image(float* pos0, float* pos1, float* pos2, float* uv0, float* uv1, float* uv2, float light_intensity){
	uint16_t x0, x1, x2;
	uint16_t y0, y1, y2;
	
	/*x0 = (uint16_t)math::clamp((short)((pos0[0]+1.0f)*WIN_WIDTH)>>1,WIN_WIDTH-1, 0);
	x1 = (uint16_t)math::clamp((short)((pos1[0]+1.0f)*WIN_WIDTH)>>1,WIN_WIDTH-1, 0);
	x2 = (uint16_t)math::clamp((short)((pos2[0]+1.0f)*WIN_WIDTH)>>1,WIN_WIDTH-1, 0);
	x3 = (uint16_t)math::clamp((short)((pos3[0]+1.0f)*WIN_WIDTH)>>1,WIN_WIDTH-1, 0);
	y0 = (uint16_t)math::clamp((short)((pos0[1]+1.0f)*WIN_HEIGHT)>>1,WIN_HEIGHT-1, 0);
	y1 = (uint16_t)math::clamp((short)((pos1[1]+1.0f)*WIN_HEIGHT)>>1,WIN_HEIGHT-1, 0);
	y2 = (uint16_t)math::clamp((short)((pos2[1]+1.0f)*WIN_HEIGHT)>>1,WIN_HEIGHT-1, 0);
	y3 = (uint16_t)math::clamp((short)((pos3[1]+1.0f)*WIN_HEIGHT)>>1,WIN_HEIGHT-1, 0);*/
	x0 = ((uint16_t)math::clamp<short>((short)((pos0[0]+1.0f)*WIN_WIDTH),2*WIN_WIDTH-1, 0))>>1;
	x1 = ((uint16_t)math::clamp<short>((short)((pos1[0]+1.0f)*WIN_WIDTH),2*WIN_WIDTH-1, 0))>>1;
	x2 = ((uint16_t)math::clamp<short>((short)((pos2[0]+1.0f)*WIN_WIDTH),2*WIN_WIDTH-1, 0))>>1;
	y0 = ((uint16_t)math::clamp<short>((short)((pos0[1]+1.0f)*WIN_HEIGHT),2*WIN_HEIGHT-1, 0))>>1;
	y1 = ((uint16_t)math::clamp<short>((short)((pos1[1]+1.0f)*WIN_HEIGHT),2*WIN_HEIGHT-1, 0))>>1;
	y2 = ((uint16_t)math::clamp<short>((short)((pos2[1]+1.0f)*WIN_HEIGHT),2*WIN_HEIGHT-1, 0))>>1;
	//printf("%i,%i,%i,%i,%i,%i,%i,%i\n",x0,x1,x2,x3,y0,y1,y2,y3);

	uint16_t bx, sx, by, sy;
	bx = x0>x1 ? x0 : x1;
	bx = bx>x2 ? bx : x2;
	sx = x0<x1 ? x0 : x1;
	sx = sx<x2 ? sx : x2;
	by = y0>y1 ? y0 : y1;
	by = by>y2 ? by : y2;
	sy = y0<y1 ? y0 : y1;
	sy = sy<y2 ? sy : y2;

	for(uint16_t x=sx; x<=bx; x++)
		for(uint16_t y=sy; y<=by; y++) {
			fbx_grid[x+WIN_WIDTH*y] = nanf("");
			fby_grid[x+WIN_WIDTH*y] = nanf("");
			depth_grid[x+WIN_WIDTH*y] = nanf("");
		}

	/*if(uv0[0]==1.0f) uv0[0] = LTO_F;
	if(uv1[0]==1.0f) uv1[0] = LTO_F;
	if(uv2[0]==1.0f) uv2[0] = LTO_F;
	if(uv0[1]==1.0f) uv0[1] = LTO_F;
	if(uv1[1]==1.0f) uv1[1] = LTO_F;
	if(uv2[1]==1.0f) uv2[1] = LTO_F;*/

	drawlinef(x0,y0,x1,y1,pos0[2],pos1[2],depth_grid);
	drawlinef(x1,y1,x2,y2,pos1[2],pos2[2],depth_grid);
	drawlinef(x2,y2,x0,y0,pos2[2],pos0[2],depth_grid);
	fill_polygonf(sx,sy,bx+1,by+1,depth_grid);

	drawlinef(x0,y0,x1,y1,uv0[0],uv1[0],fbx_grid);
	drawlinef(x1,y1,x2,y2,uv1[0],uv2[0],fbx_grid);
	drawlinef(x2,y2,x0,y0,uv2[0],uv0[0],fbx_grid);
	fill_polygonf(sx,sy,bx+1,by+1,fbx_grid);

	drawlinef(x0,y0,x1,y1,uv0[1],uv1[1],fby_grid);
	drawlinef(x1,y1,x2,y2,uv1[1],uv2[1],fby_grid);
	drawlinef(x2,y2,x0,y0,uv2[1],uv0[1],fby_grid);
	fill_polygonf(sx,sy,bx+1,by+1,fby_grid);

	uint8_t light_value = math::clamp<uint8_t>(light_intensity*0x20, 0x20, 0);
	uint16_t fb_x, fb_y;
	int depth_val;
	for(uint16_t x=sx; x<=bx; x++)
		for(uint16_t y=sy; y<=by; y++) {
			if(!std::isnan(fbx_grid[x+WIN_WIDTH*y])) {
				depth_val = 0xffff*((depth_grid[x+WIN_WIDTH*y]+1.0f)/2.0f);
				if((depth_val < depth_buffer[x+WIN_WIDTH*y]) & (depth_val > 0) & (depth_val < 0xffff)) {
					fb_x = (uint16_t)(texture_ppm.width*fbx_grid[x+WIN_WIDTH*y])%texture_ppm.width;
					fb_y = (uint16_t)(texture_ppm.height*fby_grid[x+WIN_WIDTH*y])%texture_ppm.height;

					uint16_t fb_p = texture_ppm.buffer[fb_x+texture_ppm.width*fb_y];
					uint32_t red = ((fb_p&0xf800)*light_value)>>5;
					uint32_t green = ((fb_p&0x07e0)*light_value)>>5;
					uint32_t blue = ((fb_p&0x001f)*light_value)>>5;
					fb_p = 0;
					fb_p |= red & 0xf800;
					fb_p |= green & 0x07e0;
					fb_p |= blue & 0x001f;
					if(fb_p==0xffff) fb_p = 0xfffe;
					hidden_buffer[x+WIN_WIDTH*y] = fb_p;
					//hidden_buffer[x+WIN_WIDTH*y] = depth_val>>11;
					depth_buffer[x+WIN_WIDTH*y] = depth_val;
				}
			}
		}
}
/*void draw_triangle_image(vertex v0, vertex v1, vertex v2){
	uint16_t x0, x1, x2;
	uint16_t y0, y1, y2;
	
	x0 = ((uint16_t)math::clamp<short>((short)((v0.x+1.0f)*WIN_WIDTH),2*WIN_WIDTH-1, 0))>>1;
	x1 = ((uint16_t)math::clamp<short>((short)((v1.x+1.0f)*WIN_WIDTH),2*WIN_WIDTH-1, 0))>>1;
	x2 = ((uint16_t)math::clamp<short>((short)((v2.x+1.0f)*WIN_WIDTH),2*WIN_WIDTH-1, 0))>>1;
	y0 = ((uint16_t)math::clamp<short>((short)((v0.y+1.0f)*WIN_HEIGHT),2*WIN_HEIGHT-1, 0))>>1;
	y1 = ((uint16_t)math::clamp<short>((short)((v1.y+1.0f)*WIN_HEIGHT),2*WIN_HEIGHT-1, 0))>>1;
	y2 = ((uint16_t)math::clamp<short>((short)((v2.y+1.0f)*WIN_HEIGHT),2*WIN_HEIGHT-1, 0))>>1;

	uint16_t bx, sx, by, sy;
	bx = x0>x1 ? x0 : x1;
	bx = bx>x2 ? bx : x2;
	sx = x0<x1 ? x0 : x1;
	sx = sx<x2 ? sx : x2;
	by = y0>y1 ? y0 : y1;
	by = by>y2 ? by : y2;
	sy = y0<y1 ? y0 : y1;
	sy = sy<y2 ? sy : y2;

	for(uint16_t x=sx; x<=bx; x++)
		for(uint16_t y=sy; y<=by; y++) {
			fbx_grid[x+WIN_WIDTH*y] = nanf("");
			fby_grid[x+WIN_WIDTH*y] = nanf("");
			depth_grid[x+WIN_WIDTH*y] = nanf("");
		}

	if(v0.uvx==1.0f) v0.uvx = LTO_F;
	if(v1.uvx==1.0f) v1.uvx = LTO_F;
	if(v2.uvx==1.0f) v2.uvx = LTO_F;
	if(v0.uvy==1.0f) v0.uvy = LTO_F;
	if(v1.uvy==1.0f) v1.uvy = LTO_F;
	if(v2.uvy==1.0f) v2.uvy = LTO_F;

	drawlinef(x0,y0,x1,y1,v0.z,v1.z,depth_grid);
	drawlinef(x1,y1,x2,y2,v1.z,v2.z,depth_grid);
	drawlinef(x2,y2,x0,y0,v2.z,v0.z,depth_grid);
	fill_polygonf(sx,sy,bx+1,by+1,depth_grid);

	drawlinef(x0,y0,x1,y1,v0.uvx,v1.uvx,fbx_grid);
	drawlinef(x1,y1,x2,y2,v1.uvx,v2.uvx,fbx_grid);
	drawlinef(x2,y2,x0,y0,v2.uvx,v0.uvx,fbx_grid);
	fill_polygonf(sx,sy,bx+1,by+1,fbx_grid);

	drawlinef(x0,y0,x1,y1,v0.uvy,v1.uvy,fby_grid);
	drawlinef(x1,y1,x2,y2,v1.uvy,v2.uvy,fby_grid);
	drawlinef(x2,y2,x0,y0,v2.uvy,v0.uvy,fby_grid);
	fill_polygonf(sx,sy,bx+1,by+1,fby_grid);

	uint16_t fb_x, fb_y, depth_val;
	for(uint16_t x=sx; x<=bx; x++)
		for(uint16_t y=sy; y<=by; y++) {
			if(!std::isnan(fbx_grid[x+WIN_WIDTH*y])) {
				depth_val = (uint16_t)(0xffff*((depth_grid[x+WIN_WIDTH*y]+1.0f)/2.0f));
				if(depth_val > depth_buffer[x+WIN_WIDTH*y])
					continue;

				fb_x = (uint16_t)(texture_ppm.width*fbx_grid[x+WIN_WIDTH*y])%texture_ppm.width;
				fb_y = (uint16_t)(texture_ppm.height*fby_grid[x+WIN_WIDTH*y])%texture_ppm.height;

				uint16_t fb_p = texture_ppm.buffer[fb_x+texture_ppm.width*fb_y];
				if(fb_p==0xffff) fb_p = 0xfffe;
				hidden_buffer[x+WIN_WIDTH*y] = fb_p;
				//hidden_buffer[x+WIN_WIDTH*y] = depth_val>>11;
				depth_buffer[x+WIN_WIDTH*y] = depth_val;
			}
		}
}*/


// Render square with texture
void draw_square_image(float* pos0, float* pos1, float* pos2, float* pos3, float* uv0, float* uv1, float* uv2, float* uv3, float light_intensity){
	uint16_t x0, x1, x2, x3;
	uint16_t y0, y1, y2, y3;
	
	/*x0 = (uint16_t)math::clamp((short)((pos0[0]+1.0f)*WIN_WIDTH)>>1,WIN_WIDTH-1, 0);
	x1 = (uint16_t)math::clamp((short)((pos1[0]+1.0f)*WIN_WIDTH)>>1,WIN_WIDTH-1, 0);
	x2 = (uint16_t)math::clamp((short)((pos2[0]+1.0f)*WIN_WIDTH)>>1,WIN_WIDTH-1, 0);
	x3 = (uint16_t)math::clamp((short)((pos3[0]+1.0f)*WIN_WIDTH)>>1,WIN_WIDTH-1, 0);
	y0 = (uint16_t)math::clamp((short)((pos0[1]+1.0f)*WIN_HEIGHT)>>1,WIN_HEIGHT-1, 0);
	y1 = (uint16_t)math::clamp((short)((pos1[1]+1.0f)*WIN_HEIGHT)>>1,WIN_HEIGHT-1, 0);
	y2 = (uint16_t)math::clamp((short)((pos2[1]+1.0f)*WIN_HEIGHT)>>1,WIN_HEIGHT-1, 0);
	y3 = (uint16_t)math::clamp((short)((pos3[1]+1.0f)*WIN_HEIGHT)>>1,WIN_HEIGHT-1, 0);*/
	x0 = ((uint16_t)math::clamp<short>((short)((pos0[0]+1.0f)*WIN_WIDTH),2*WIN_WIDTH-1, 0))>>1;
	x1 = ((uint16_t)math::clamp<short>((short)((pos1[0]+1.0f)*WIN_WIDTH),2*WIN_WIDTH-1, 0))>>1;
	x2 = ((uint16_t)math::clamp<short>((short)((pos2[0]+1.0f)*WIN_WIDTH),2*WIN_WIDTH-1, 0))>>1;
	x3 = ((uint16_t)math::clamp<short>((short)((pos3[0]+1.0f)*WIN_WIDTH),2*WIN_WIDTH-1, 0))>>1;
	y0 = ((uint16_t)math::clamp<short>((short)((pos0[1]+1.0f)*WIN_HEIGHT),2*WIN_HEIGHT-1, 0))>>1;
	y1 = ((uint16_t)math::clamp<short>((short)((pos1[1]+1.0f)*WIN_HEIGHT),2*WIN_HEIGHT-1, 0))>>1;
	y2 = ((uint16_t)math::clamp<short>((short)((pos2[1]+1.0f)*WIN_HEIGHT),2*WIN_HEIGHT-1, 0))>>1;
	y3 = ((uint16_t)math::clamp<short>((short)((pos3[1]+1.0f)*WIN_HEIGHT),2*WIN_HEIGHT-1, 0))>>1;
	//printf("%i,%i,%i,%i,%i,%i,%i,%i\n",x0,x1,x2,x3,y0,y1,y2,y3);

	uint16_t bx, sx, by, sy;
	bx = x0>x1 ? x0 : x1;
	bx = bx>x2 ? bx : x2;
	bx = bx>x3 ? bx : x3;
	sx = x0<x1 ? x0 : x1;
	sx = sx<x2 ? sx : x2;
	sx = sx<x3 ? sx : x3;
	by = y0>y1 ? y0 : y1;
	by = by>y2 ? by : y2;
	by = by>y3 ? by : y3;
	sy = y0<y1 ? y0 : y1;
	sy = sy<y2 ? sy : y2;
	sy = sy<y3 ? sy : y3;

	for(uint16_t x=sx; x<=bx; x++)
		for(uint16_t y=sy; y<=by; y++) {
			fbx_grid[x+WIN_WIDTH*y] = nanf("");
			fby_grid[x+WIN_WIDTH*y] = nanf("");
			depth_grid[x+WIN_WIDTH*y] = nanf("");
		}

	/*if(uv0[0]==1.0f) uv0[0] = LTO_F;
	if(uv1[0]==1.0f) uv1[0] = LTO_F;
	if(uv2[0]==1.0f) uv2[0] = LTO_F;
	if(uv0[1]==1.0f) uv0[1] = LTO_F;
	if(uv1[1]==1.0f) uv1[1] = LTO_F;
	if(uv2[1]==1.0f) uv2[1] = LTO_F;*/

	drawlinef(x0,y0,x1,y1,pos0[2],pos1[2],depth_grid);
	drawlinef(x1,y1,x2,y2,pos1[2],pos2[2],depth_grid);
	drawlinef(x2,y2,x3,y3,pos2[2],pos3[2],depth_grid);
	drawlinef(x3,y3,x0,y0,pos3[2],pos0[2],depth_grid);
	fill_polygonf(sx,sy,bx+1,by+1,depth_grid);

	drawlinef(x0,y0,x1,y1,uv0[0],uv1[0],fbx_grid);
	drawlinef(x1,y1,x2,y2,uv1[0],uv2[0],fbx_grid);
	drawlinef(x2,y2,x3,y3,uv2[0],uv3[0],fbx_grid);
	drawlinef(x3,y3,x0,y0,uv3[0],uv0[0],fbx_grid);
	fill_polygonf(sx,sy,bx+1,by+1,fbx_grid);

	drawlinef(x0,y0,x1,y1,uv0[1],uv1[1],fby_grid);
	drawlinef(x1,y1,x2,y2,uv1[1],uv2[1],fby_grid);
	drawlinef(x2,y2,x3,y3,uv2[1],uv3[1],fby_grid);
	drawlinef(x3,y3,x0,y0,uv3[1],uv0[1],fby_grid);
	fill_polygonf(sx,sy,bx+1,by+1,fby_grid);

	uint8_t light_value = math::clamp<uint8_t>(light_intensity*0x40, 0x40, 0);
	uint16_t fb_x, fb_y;
	int depth_val;
	for(uint16_t x=sx; x<=bx; x++)
		for(uint16_t y=sy; y<=by; y++) {
			if(!std::isnan(fbx_grid[x+WIN_WIDTH*y])) {
				depth_val = 0xffff*((depth_grid[x+WIN_WIDTH*y]+1.0f)/2.0f);
				if((depth_val < depth_buffer[x+WIN_WIDTH*y]) & (depth_val > 0) & (depth_val < 0xffff)) {
					fb_x = (uint16_t)(texture_ppm.width*fbx_grid[x+WIN_WIDTH*y])%texture_ppm.width;
					fb_y = (uint16_t)(texture_ppm.height*fby_grid[x+WIN_WIDTH*y])%texture_ppm.height;

					uint16_t fb_p = texture_ppm.buffer[fb_x+texture_ppm.width*fb_y];
					uint32_t red = ((fb_p&0xf800)*light_value)>>6;
					uint32_t green = ((fb_p&0x07e0)*light_value)>>6;
					uint32_t blue = ((fb_p&0x001f)*light_value)>>6;
					fb_p = 0;
					fb_p |= red & 0xf800;
					fb_p |= green & 0x07e0;
					fb_p |= blue & 0x001f;
					if(fb_p==0xffff) fb_p = 0xfffe;
					hidden_buffer[x+WIN_WIDTH*y] = fb_p;
					//hidden_buffer[x+WIN_WIDTH*y] = depth_val>>11;
					depth_buffer[x+WIN_WIDTH*y] = depth_val;
				}
			}
		}
}

