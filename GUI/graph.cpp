#include "guipane.h"
#include <math.h>
#include <iostream>
#include <cassert>
#include <limits>
#include <climits>

// TODO: remains as tom7 font, should change?
#define FONTPIC "tom_vii_font.png"
#define FONTCHARS " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`-=[]\\;',./~!@#$%^&*()_+{}|:\"<>?"
#define FONTSIZE_H 12
#define FONTSIZE_W 10
#define FONT_HEIGHT 17
#define FONT_WIDTH 11
#define LETR_SPC 7

// Num pixels difference from edge of graph's pane to
// edge of graph's x or y axis
#define G_EDGE_SPACE 50
#define PIXELS_PER_TICKMARK 50
#define BORDER_THICKNESS 3
#define DOT_THICKNESS 5
#define TICK_LENGTH 10

#define DEFAULT_BGCOLOR 0xFFFFFFFF // White
#define BORDER_COLOR 0x000000FF // Black

GUI_Graph::GUI_Graph(int loc_x, int loc_y, int dim_w, int dim_h, 
			     	 std::vector<float> x_vec, std::vector<float> y_vec, 
					 ulong fg_color, GUI_Pane *pane)
{
	x = loc_x;
	y = loc_y;
	width = dim_w;
	height = dim_h;
	graph_x.assign(x_vec.begin(), x_vec.end());
	graph_y.assign(y_vec.begin(), y_vec.end());
	background_color = DEFAULT_BGCOLOR;
	foreground_color = fg_color;

	assert (graph_x.size() == graph_y.size());

	gr_y_min = gr_x_min = INT_MAX;
	gr_y_max = gr_x_max = INT_MIN;
	for (int i = 0; i < graph_x.size(); i++){
		if (graph_x[i] > gr_x_max)
			gr_x_max = graph_x[i];
		if (graph_x[i] < gr_x_min)
			gr_x_min = graph_x[i];

		if (graph_y[i] > gr_y_max)
			gr_y_max = graph_y[i];
		if (graph_y[i] < gr_y_min)
			gr_y_min = graph_y[i];
	}
	gr_y_min -= (gr_y_max / graph_y.size());
	gr_y_max += (gr_y_max / graph_y.size());
	gr_x_min -= (gr_x_max / graph_x.size());
	gr_x_max += (gr_x_max / graph_y.size());
	
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

/* Helper func for drawing tick marks. 
 * Rounds [f] to [decimal_round] places, then casts to string. */
std::string float_to_string_round(float f, int decimal_round)
{
	return std::to_string(f).substr(0, 2 + decimal_round);
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
	const int num_x_ticks = graph_w / PIXELS_PER_TICKMARK;
	const int num_y_ticks = graph_h / PIXELS_PER_TICKMARK;
	const double point_diff_x = gr_x_max - gr_x_min;
	const double point_diff_y = gr_y_max - gr_y_min;

	// To warn in case of divide by zero
	if (point_diff_x == 0 || point_diff_y == 0)
		std::cout << "Your X or Y point differential seems to be zero!\n";
	const double pixels_per_point_x = graph_w / point_diff_x;
	const double pixels_per_point_y = graph_h / point_diff_y;

	const int graph_top_x    = x + G_EDGE_SPACE;
	const int graph_top_y    = y + G_EDGE_SPACE;
	const int graph_bottom_y = y + height - G_EDGE_SPACE;

	// Draw borders
	outer_pane->fill_rect(graph_top_x, graph_top_y,
						  BORDER_THICKNESS, graph_h, BORDER_COLOR);
	outer_pane->fill_rect(graph_top_x, graph_bottom_y,
						  graph_w, BORDER_THICKNESS, BORDER_COLOR);

	// Draw ticks
	int tick_drawx, tick_drawy;
	float tick_number;
	for (int X = 1; X < num_x_ticks; X++) {
		tick_drawx = graph_top_x + (X * PIXELS_PER_TICKMARK); 
		tick_drawy = graph_bottom_y;
		tick_number = gr_x_min + (((gr_x_max - gr_x_min) / num_x_ticks) * X);


		outer_pane->fill_rect(tick_drawx, tick_drawy,
							  BORDER_THICKNESS, TICK_LENGTH, BORDER_COLOR);
		inner_font->draw_string(float_to_string_round(tick_number, 2),
							   tick_drawx - LETR_SPC, 
							   tick_drawy + (G_EDGE_SPACE * 0.33), 
							   LETR_SPC);	
	}
	for (int Y = 1; Y < num_y_ticks; Y++) {
		tick_drawx = graph_top_x - TICK_LENGTH;
		tick_drawy = graph_bottom_y - (Y * PIXELS_PER_TICKMARK);
		tick_number = gr_y_min + (((gr_y_max - gr_y_min) / num_y_ticks) * Y);

		outer_pane->fill_rect(tick_drawx, tick_drawy,
							  TICK_LENGTH, BORDER_THICKNESS, BORDER_COLOR);


		inner_font->draw_string(float_to_string_round(tick_number, 2),
							   tick_drawx - (G_EDGE_SPACE * 0.66), 
							   tick_drawy - LETR_SPC, 
							   LETR_SPC);	
	}
	
	// Draw dots
	for (int i = 0; i < graph_x.size(); i++)
	{
		int pixel_x = graph_top_x  + 
								((graph_x[i] - gr_x_min) * pixels_per_point_x);
		int pixel_y = graph_bottom_y - 
							    ((graph_y[i] - gr_y_min) * pixels_per_point_y);
		outer_pane->fill_rect(pixel_x - DOT_THICKNESS/2, 
							  pixel_y - DOT_THICKNESS/2,
							  DOT_THICKNESS, DOT_THICKNESS, foreground_color);
	}
}
