#include <iostream>
#include "guipane.h"


 
GUI_Pane::GUI_Pane(  const std::string &window_name,
				int window_width, 
				int window_height,
				unsigned int clr,
				const std::string &font_filename,
				const std::string &font_charmap,
				int font_picture_w,
				int font_picture_h,
				int font_render_w,
				int font_render_h )
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "Uh oh, something went wrong with init. GUI not loaded.\n";
		return;
	}


	window = SDL_CreateWindow( 
					window_name.c_str(), 
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					window_width, 
					window_height,
					0);
	width = window_width;
	height = window_height;

	if (window == NULL){
		std::cout << "Uh oh, something went wrong with window. GUI not loaded.\n";
		return;
	}


	Uint32 rend_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	renderer = SDL_CreateRenderer(window, -1, rend_flags);

	if (renderer == NULL){
		std::cout << "Uh oh, something went wrong with renderer. GUI not loaded.\n";
		SDL_Quit();
		return;
	}

	bg_color = clr; 

	inner_font = new Font(font_filename, font_charmap, renderer, 
						  font_picture_w, font_picture_h, font_render_w,
						  font_render_h );
}


GUI_Pane::~GUI_Pane()
{
	delete inner_font;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool GUI_Pane::update()
{
	return true;
}

void clear_screen(SDL_Renderer *rend, unsigned long rgba)
{
	int r = rgba >> 24;
	int g = (rgba >> 16) - (r << 8);
	int b = (rgba >> 8) - (r << 16) - (g << 8);
	int a = rgba - (r << 24) - (g << 16) - (b << 8);
	SDL_SetRenderDrawColor(rend, r, g, b, a);
	SDL_RenderClear(rend);
}	

void GUI_Pane::draw()
{
	clear_screen(renderer, bg_color);

	inner_font->draw_string("stonks", 100, 200, 18);
	fill_rect(200,200,100,100,0x112233FF);

	SDL_RenderPresent(renderer);
}

void GUI_Pane::loop()
{
	SDL_Event event;
	for (;;) {
		if (SDL_WaitEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					return;
			}
		}

		if (update())
			draw();
	}
}

void GUI_Pane::fill_rect(int x, int y, int w, int h, unsigned long rgba)
{
	int r = rgba >> 24;
	int g = (rgba >> 16) - (r << 8);
	int b = (rgba >> 8) - (r << 16) - (g << 8);
	int a = rgba - (r << 24) - (g << 16) - (b << 8);
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &rect);
}
