#include "font.h"
#include <vector>
#include <string>
#include <cassert>
// In guipane.h
struct GUI_Pane;

typedef unsigned long ulong;

// Interface for items that are placed on GUI pane
struct GUI_Widget
{
	bool hidden;

	GUI_Widget() { hidden = false; }
	virtual ~GUI_Widget(){}
	virtual bool update(SDL_Event event) = 0;
	virtual void draw(SDL_Renderer *rend) = 0;
};

struct GUI_Button : public GUI_Widget
{
	GUI_Button(
		const std::string &t, 
		int x_location,
		int y_location,
	    ulong rgba, 
	    void (*but)(GUI_Pane *pane),
		GUI_Pane *p
	);

	~GUI_Button();

	bool update(SDL_Event event);
	void draw(SDL_Renderer *rend);

	// Call button_func 
	void press_action();

	int x, y;
	private:
		const std::string &text;
		int width;
		int height;
		ulong color;
		void (*button_func)(GUI_Pane *pane);
		GUI_Pane *outer_pane;

		// For drawing button as darker when pressed
		bool pressed_this_frame;
};

/* Instance represents one "function" in a GUI_Graph that can be graphed
 * with others on the same set of axes on the same graph pane */
struct GraphFunc
{
	GraphFunc(std::vector<float> x_vec, std::vector<float> y_vec, ulong clr)
	{
		assert(x_vec.size() == y_vec.size());
		x_points.assign(x_vec.begin(), x_vec.end());
		y_points.assign(y_vec.begin(), y_vec.end());
		color = clr;
	}
	
	~GraphFunc() {};

	// Invariant: these two vectors should have the 
	// same length. The value at x_points[i] corresponds 
	// to the value at y_points[i], and vise versa.
	std::vector<float> x_points;
	std::vector<float> y_points;

	// Color of this function's dots
	ulong color; 
};


struct GUI_Graph : public GUI_Widget
{
	/* Constructs an instance of a GUI_Graph containing one 
	 * function for graphing with the given parameters. Other
	 * functions to graph can be added afterwards. */
	GUI_Graph(int loc_x, int loc_y, int dim_w, int dim_h, 
			   std::vector<float> x_vec, std::vector<float> y_vec, 
			   ulong fg_color, GUI_Pane *pane);

	~GUI_Graph();

	bool update(SDL_Event event);
	void draw(SDL_Renderer *rend);

	// Makes X, Y axes of this graph "the same" as graph [src]
	void copy_axes(GUI_Graph *src);

	void add_graph_func(std::vector<float> xvec, std::vector<float> yvec, 
						ulong clr);

	// All public fields are so by design, and should be 
	// manually set if necessary.

	// Referring to display location, widget dimensions
	int x, y;
	int width, height; 

	// Here gr_x and gr_y refer to location on graph, 
	// not locations on the GUI Pane.
	float gr_x_min, gr_y_min;
	float gr_x_max, gr_y_max;

	ulong background_color;

	private:
		// Should be less "thick" font than that of 
		// background GUI Pane.
		Font *inner_font;


		std::vector<GraphFunc*> funcs_to_graph;
		
		GUI_Pane *outer_pane;
};


struct GUI_Select : public GUI_Widget
{
	/* Constructs a GUI_Select widget with empty
	 * lists. Things are added to selectable list (vector)
	 * using methods only */
	GUI_Select(GUI_Pane *outer, int x_cor, int y_cor, ulong color);
	
	~GUI_Select();

	bool update(SDL_Event event);
	void draw(SDL_Renderer *rend);

	void add_option(const std::string &option);
	void add_options(std::vector<std::string> str_vec);

	ulong unselected_color;
	ulong selected_color;
	int x, y;

	private:
		/* Invariant: a given index represents
		 * the same name, toggle, etc. for each of 
		 * these vectors. They should also all have
		 * length [length]. */
		std::vector<std::string> names;
		std::vector<bool> toggles;
		int length;

		/* Length of longest string in [names], used
		 * to determine length for drawing selectable */
		int longest_name_length;

		GUI_Pane* outer_pane;
};
