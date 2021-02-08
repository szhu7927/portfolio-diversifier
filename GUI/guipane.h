#include "guiwidget.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

// Defined in guiwidget.h
struct GUI_Widget;

struct GUI_Pane {
	Font *inner_font;
	
	SDL_Window *window;
	SDL_Renderer *renderer;

	ulong bg_color;

	std::vector<GUI_Widget*> widgets;
	// Contains the "name" of each widget
	std::vector<std::string> widget_labels;

	// Constructs Font, too
	GUI_Pane (  const std::string &window_name,
				int window_width, 
				int window_height,
				unsigned int clr,
				const std::string &font_filename,
				const std::string &font_charmap,
				int font_picture_w,
				int font_picture_h,
				int font_render_w,
				int font_render_h 
			);

	~GUI_Pane();

	void loop();

	GUI_Widget* find_widget_by_label(const std::string& label);

	// Widget-adding methods
	void add_button(const std::string& label,
					const std::string& text, int x, int y, 
					ulong rgba, void (*but)(GUI_Pane *pane) );
	// Returns created pointer so graph can be publicly modified
	GUI_Graph* add_graph(const std::string& label,
				    int x, int y, int w, int h,
					std::vector<float> x_vec, std::vector<float> y_vec,
					ulong fg_color);
	// Same with this one
	GUI_Select* add_selectable(const std::string& label, int x, int y, ulong color);

							



	// Misc drawing
	// TODO: move to its own util file???
	void switch_color(SDL_Renderer *r, ulong rgba);
	void fill_rect(int x, int y, int w, int h, ulong color);

	private:
	int width, height;
	
	// Returns true if UI requires drawing change
	bool update(SDL_Event event);
	void draw();
};
