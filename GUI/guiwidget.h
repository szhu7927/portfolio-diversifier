#include "font.h"
#include <vector>
#include <string>
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


struct GUI_Graph : public GUI_Widget
{
	GUI_Graph(int loc_x, int loc_y, int dim_w, int dim_h, 
			   std::vector<float> x_vec, std::vector<float> y_vec, 
			   ulong fg_color, GUI_Pane *pane);

	~GUI_Graph();

	bool update(SDL_Event event);
	void draw(SDL_Renderer *rend);


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
	// Foreground := Like, the color of the dots and stuff
	ulong foreground_color; 

	private:
		// Should be less "thick" font than that of 
		// background GUI Pane.
		Font *inner_font;

		// Invariant: these two vectors should have the 
		// same length. The value at graph_x[i] corresponds 
		// to the value at graph_y[i], and vise versa.
		std::vector<float> graph_x;
		std::vector<float> graph_y;
		
		GUI_Pane *outer_pane;
};


