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

	// Widget-adding methods
	void add_button(const std::string& text, int x, int y, 
					ulong rgba, void (*but)(GUI_Pane *pane) );
	// Returns created pointer so graph can be publicly modified
	GUI_Graph* add_graph(int x, int y, int w, int h, 
				    std::vector<int> x_vec, std::vector<int> y_vec,
					ulong fg_color);

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
