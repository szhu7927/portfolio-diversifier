#include "font.h"
#include <string>
#include <SDL2/SDL_image.h>

Font::Font(const std::string &filename,
		 const std::string &letters_in_order,
		 SDL_Renderer *renderer,
		 int picture_w,
		 int picture_h,
		 int render_w,
		 int render_h )
{
	rend = renderer;
	
	SDL_Surface *img_surf = IMG_Load(filename.c_str());
	tex = SDL_CreateTextureFromSurface(rend, img_surf);
	SDL_FreeSurface(img_surf);

	pic_width = picture_w;
	pic_height = picture_h;
	width = render_w;
	height = render_h;

	short i;
	const int len = letters_in_order.length();
	for (i = 0; i < len; i++)
		char_map[(int)letters_in_order[i]] = i;

	pic_letter_cols = len;
}


void Font::draw_letter(char letter, int x, int y)
{
	const short frame_num = char_map[(int)letter];

	SDL_Rect draw_location;
	draw_location.x = x;
	draw_location.y = y;
	draw_location.w = width;
	draw_location.h = height;

	SDL_Rect ani_rec;
	ani_rec.x = pic_width * (frame_num % pic_letter_cols);
	ani_rec.y = pic_height * (frame_num / pic_letter_cols);
	ani_rec.w = pic_width;
	ani_rec.h = pic_height;
	
	SDL_RenderCopy(rend, tex, &ani_rec, &draw_location);
}


void Font::draw_string(const std::string &str, int x, int y, int letter_space)
{
	const int NEWLINE_PADDING = 10;

	int draw_y = y;
	int draw_x = x;
	int letter_width; 
	if (letter_space < 0)
		letter_width = width;
	else
		letter_width = letter_space;

	for (int i = 0; i < str.length(); i++){
		if (str[i] == '\n'){
			draw_y += height + NEWLINE_PADDING;
			draw_x = x;
		}
		else
			draw_letter(str[i], draw_x, draw_y);

		draw_x = draw_x + letter_width;
	}
}

Font::~Font() {};
