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
	for (int i = 0; i < widgets.size(); i++)
		delete widgets[i];
	delete inner_font;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool GUI_Pane::update(SDL_Event event)
{
	bool ui_dirty = false;

	for (int i = 0; i < widgets.size(); i++)
		widgets[i]->update(event);

	return ui_dirty;
}

void clear_screen(SDL_Renderer *rend, ulong rgba)
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

	for (int i = 0; i < widgets.size(); i++)
		widgets[i]->draw(renderer);

	SDL_RenderPresent(renderer);
}

void GUI_Pane::loop()
{
	bool ui_dirty = true;

	for (;;) {
		SDL_Event event;
		if (SDL_WaitEvent(&event))
			if (event.type == SDL_QUIT)
				return;

		if (ui_dirty)
			draw();
		ui_dirty = update(event);
	}
}


void GUI_Pane::switch_color(SDL_Renderer *ren, ulong rgba)
{
	int r = rgba >> 24;
	int g = (rgba >> 16) - (r << 8);
	int b = (rgba >> 8) - (r << 16) - (g << 8);
	int a = rgba - (r << 24) - (g << 16) - (b << 8);
	SDL_SetRenderDrawColor(ren, r, g, b, a);
}

void GUI_Pane::fill_rect(int x, int y, int w, int h, ulong rgba)
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

void GUI_Pane::add_button(const std::string& text, int x, int y,
						  ulong rgba, void (*but)(GUI_Pane *pane) )
{
	GUI_Button *b = new GUI_Button(text, x, y, rgba, but, this);
	widgets.push_back(b);
}

GUI_Graph* GUI_Pane::add_graph(int x, int y, int w, int h, 
						std::vector<int> x_vec, 
						std::vector<int> y_vec, 
						ulong fg_color )
{
	GUI_Graph *g = new GUI_Graph(x, y, w, h, x_vec, y_vec, fg_color, this);
	widgets.push_back(g);
	return g;
}
