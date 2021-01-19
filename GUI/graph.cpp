#include "guipane.h"
#include <iostream>
#include <cassert>
#include <limits>

// TODO: remains as tom7 font, should change?
#define FONTPIC "tom_vii_font.png"
#define FONTCHARS " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`-=[]\\;',./~!@#$%^&*()_+{}|:\"<>?"
#define FONTSIZE_H 32
#define FONTSIZE_W 22
#define FONT_HEIGHT 17
#define FONT_WIDTH 11

// Num pixels difference from edge of graph's pane to
// edge of graph's x or y axis
#define G_EDGE_SPACE 50
#define PIXELS_PER_TICKMARK 50
#define BORDER_THICKNESS 3
#define DOT_THICKNESS 5

#define DEFAULT_BGCOLOR 0xFFFFFFFF // White
#define BORDER_COLOR 0x000000FF // Black

GUI_Graph::GUI_Graph(int loc_x, int loc_y, int dim_w, int dim_h, 
			     	 std::vector<int> x_vec, std::vector<int> y_vec, 
					 ulong fg_color, GUI_Pane *pane)
{
	x = loc_x;
	y = loc_y;
	width = dim_w;
	height = dim_h;
	ticks_x.assign(x_vec.begin(), x_vec.end());
	ticks_y.assign(y_vec.begin(), y_vec.end());
	background_color = DEFAULT_BGCOLOR;
	foreground_color = fg_color;

	assert (ticks_x.size() == ticks_y.size());

	tick_y_min = tick_x_min = INT_MAX;
	tick_y_max = tick_x_max = INT_MIN;
	for (int i = 0; i < ticks_x.size(); i++){
		if (ticks_x[i] > tick_x_max)
			tick_x_max = ticks_x[i];
		if (ticks_x[i] < tick_x_min)
			tick_x_min = ticks_x[i];

		if (ticks_y[i] > tick_y_max)
			tick_y_max = ticks_y[i];
		if (ticks_y[i] < tick_y_min)
			tick_y_min = ticks_y[i];
	}
	tick_y_min -= (tick_y_max / ticks_y.size());
	tick_y_max += (tick_y_max / ticks_y.size());
	tick_x_min -= (tick_x_max / ticks_x.size());
	tick_x_max += (tick_x_max / ticks_y.size());
	
	inner_font = new Font(FONTPIC, FONTCHARS, pane->renderer, 
						  FONT_WIDTH, FONT_HEIGHT, FONTSIZE_W, FONTSIZE_H);

	outer_pane = pane;
}

GUI_Graph::~GUI_Graph()
{
	delete inner_font;
}


bool GUI_Graph::update(SDL_Event event)
{
	return false;
}

void GUI_Graph::draw(SDL_Renderer *rend)
{
	// Draw outer box
	SDL_Rect draw_rect;
	draw_rect.x = x;
	draw_rect.y = y;
	draw_rect.w = width;
	draw_rect.h = height; 

	outer_pane->switch_color(rend, background_color);
	SDL_RenderFillRect(rend, &draw_rect);
	outer_pane->switch_color(rend, BORDER_COLOR);
	SDL_RenderDrawRect(rend, &draw_rect);

	const int graph_w =  width - (2 * G_EDGE_SPACE);
	const int graph_h = height - (2 * G_EDGE_SPACE);
	//const int num_x_ticks = graph_w / PIXELS_PER_TICKMARK;
	//const int num_y_ticks = graph_h / PIXELS_PER_TICKMARK;
	const int num_points_x = tick_x_max - tick_x_min;
	const int num_points_y = tick_y_max - tick_y_min;
	const double pixels_per_point_x = (double)graph_w / (double)num_points_x;
	// TODO: should we do this??? Feels off somehow, but idk what else I'd do
	const double pixels_per_point_y = graph_h / num_points_y;

	const int graph_top_x    = x + G_EDGE_SPACE;
	const int graph_top_y    = y + G_EDGE_SPACE;
	const int graph_bottom_y = y + height - G_EDGE_SPACE;

	// Draw borders
	outer_pane->fill_rect(graph_top_x, graph_top_y,
						  BORDER_THICKNESS, graph_h, BORDER_COLOR);
	outer_pane->fill_rect(graph_top_x, graph_bottom_y,
						  graph_w, BORDER_THICKNESS, BORDER_COLOR);
	

	// Draw dots
	for (int i = 0; i < ticks_x.size(); i++)
	{
		int pixel_x = graph_top_x  + 
								((ticks_x[i] - tick_x_min) * pixels_per_point_x);
		int pixel_y = graph_bottom_y - 
							    ((ticks_y[i] - tick_y_min) * pixels_per_point_y);
		outer_pane->fill_rect(pixel_x - DOT_THICKNESS/2, pixel_y - DOT_THICKNESS/2,
							  DOT_THICKNESS, DOT_THICKNESS, foreground_color);
	}
}
