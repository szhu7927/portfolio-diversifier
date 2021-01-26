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


void print_vec(std::vector<float> v){
	std::cout << "{ ";
	for (int i = 0; i < v.size(); i++)
		std::cout << v[i] << ", ";
	std::cout << "}\n";
}

GUI_Graph::GUI_Graph(int loc_x, int loc_y, int dim_w, int dim_h, 
			     	 std::vector<float> x_vec, std::vector<float> y_vec, 
					 ulong fg_color, GUI_Pane *pane)
{
	x = loc_x;
	y = loc_y;
	width = dim_w;
	height = dim_h;
	funcs_to_graph.push_back(new GraphFunc(x_vec, y_vec, fg_color));
	background_color = DEFAULT_BGCOLOR;

	std::vector<float> graph_x, graph_y;
	graph_x.assign(x_vec.begin(), x_vec.end());
	graph_y.assign(y_vec.begin(), y_vec.end());

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
#if 0
	std::cout << "GR_YMIN " << gr_y_min << "\n";
	std::cout << "GR_Ymax " << gr_y_max << "\n";
	std::cout << "GR_xmin " << gr_x_min << "\n";
	std::cout << "grxmax " << gr_x_max << "\n";
	std::cout << "ysize " << graph_y.size() << "\n";
	std::cout << "xsize " << graph_x.size() << "\n";
	std::cout << "\n\n";
	std::cout << "ymax divided by ysize " << gr_y_max / graph_y.size() << "\n";
	std::cout << "xmax div by xsize " << gr_x_max / graph_x.size() << "\n";
#endif
	gr_y_min -= (gr_y_max / graph_y.size());
	gr_y_max += (gr_y_max / graph_y.size());
	gr_x_min -= (gr_x_max / graph_x.size());
	gr_x_max += (gr_x_max / graph_x.size());
#if 0
	std::cout << "\n\n";
	std::cout << "GR_YMIN " << gr_y_min << "\n";
	std::cout << "GR_Ymax " << gr_y_max << "\n";
	std::cout << "GR_xmin " << gr_x_min << "\n";
	std::cout << "grxmax " << gr_x_max << "\n";
#endif
	
	inner_font = new Font(FONTPIC, FONTCHARS, pane->renderer, 
						  FONT_WIDTH, FONT_HEIGHT, FONTSIZE_W, FONTSIZE_H);

	outer_pane = pane;
}

GUI_Graph::~GUI_Graph()
{
	delete inner_font;
	for (int i = 0; i < funcs_to_graph.size(); i++)
	{
		delete funcs_to_graph[i];
	}
}


bool GUI_Graph::update(SDL_Event event)
{
	return false;
}

/* Helper func for drawing tick marks. 
 * Rounds [f] to [decimal_round] places, then casts to string. */
std::string float_to_string_round(float f, int decimal_round)
{
	std::string f_str = std::to_string(f);
	if (f_str.size() < 5)
		return f_str;
	return f_str.substr(0, 2 + decimal_round);
}

void draw_dots(GUI_Pane *outer_pane, std::vector<float> graph_x,
			   std::vector<float> graph_y, ulong color, 
			   float gr_x_min, float gr_y_min, 
			   int ppp_x, int ppp_y,
			   int graph_top_x, int graph_bottom_y)
{
#if 0
	std::cout << "GR_X_MIN " << gr_x_min << "\n";
	std::cout << "GR_Y_MIN " << gr_y_min << "\n";
	std::cout << "GR_TOPX " << graph_top_x << "\n";
	std::cout << "GR_BOTY " << graph_bottom_y << "\n";
	std::cout << "PPP_X " << ppp_x << "\n";
	std::cout << "PPP_Y " << ppp_y << "\n";
	std::cout << "GRAPHX: ";
	print_vec(graph_x);
	std::cout << "GRAPHY: ";
	print_vec(graph_y);
	std::cout << "PIXELXY ";
#endif
	for (int i = 0; i < graph_x.size(); i++)
	{
		int pixel_x = graph_top_x  + 
								((graph_x[i] - gr_x_min) * ppp_x);
		int pixel_y = graph_bottom_y - 
							    ((graph_y[i] - gr_y_min) * ppp_y);
#if 0
		std::cout << " " << pixel_x << " " << pixel_y;
#endif

		outer_pane->fill_rect(pixel_x - DOT_THICKNESS/2, 
							  pixel_y - DOT_THICKNESS/2,
							  DOT_THICKNESS, DOT_THICKNESS, color);
	}
	std::cout << "\n";
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
	for (int i = 0; i < funcs_to_graph.size(); i++)
	{
		draw_dots(outer_pane, funcs_to_graph[i]->x_points,
				 funcs_to_graph[i]->y_points, funcs_to_graph[i]->color,
				 gr_x_min, gr_y_min, pixels_per_point_x, 
				 pixels_per_point_y, graph_top_x, graph_bottom_y );
	}
}


void GUI_Graph::copy_axes(GUI_Graph *src)
{
	width = src->width;
	height = src->height;
	gr_x_min = src->gr_x_min;
	gr_x_max = src->gr_x_max;
	gr_y_min = src->gr_y_min;
	gr_y_max = src->gr_y_max;
}

void GUI_Graph::add_graph_func(std::vector<float> xvec, std::vector<float> yvec,
								ulong clr)
{
	funcs_to_graph.push_back(new GraphFunc(xvec, yvec, clr));
}
