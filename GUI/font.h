#include <string>
#include <SDL2/SDL.h>
#define _MAX_LETTER_BYTE_SIZE_ 128

struct Font {

	// Sizes for appearing on the screen
	int width, height;
	
	// Sizes within the font file
	int pic_width, pic_height;

	// Maps character to "frame" in font pic
	short char_map[_MAX_LETTER_BYTE_SIZE_];

	// Number of letters in the row in picture
	int pic_letter_cols;

	SDL_Texture *tex;
	SDL_Renderer *rend;
	
	Font(const std::string &filename,
		 const std::string &letters_in_order,
		 SDL_Renderer *renderer,
		 int picture_w,
		 int picture_h,
		 int render_w,
		 int render_h );

	~Font();

	void draw_letter(char letter, int x, int y);

	// If [letter_space] < 0, just uses default font size for 
	// space btwn letters
	void draw_string(const std::string &str, int x, int y, int letter_space);

};
