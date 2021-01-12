#include "font.h"
#include <SDL2/SDL.h>
#include <string>

struct GUI_Pane {
	Font *inner_font;
	
	SDL_Window *window;
	SDL_Renderer *renderer;

	unsigned long bg_color;

	// TODO: button vector??? vector of other GUI objects???
	// also, then there would be funcs like add_button and also
	// check_if_button_is_clicked, that sounds
	// like a good idea!

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

	private:
	int width, height;
	
	// Returns true if UI requires drawing change
	bool update();
	void draw();

	void fill_rect(int x, int y, int w, int h, unsigned long color);
};
