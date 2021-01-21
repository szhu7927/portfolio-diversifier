#include "guipane.h" // NOT guiwidget! because it uses pane functions
#define BUTTON_LETTERSPACE 18
#define WIDTH_TEXTPAD 10
#define HEIGHT_TEXTPAD 10
#define BORDER_COLOR 0x000000FF // Black

GUI_Button::GUI_Button (
		const std::string &t, 
		int x_location,
		int y_location,
	    ulong rgba, 
	    void (*but)(GUI_Pane *pane),
		GUI_Pane *p
) : text(t), x(x_location), y(y_location),
	color(rgba), button_func(but), outer_pane(p)
{
	width = (text.length() * outer_pane->inner_font->width);
	width += 2 * WIDTH_TEXTPAD;
	height = outer_pane->inner_font->height + (2 * HEIGHT_TEXTPAD);

	pressed_this_frame = false;
}

GUI_Button::~GUI_Button () {}

bool GUI_Button::update(SDL_Event event)
{
	if (event.type != SDL_MOUSEBUTTONDOWN)
		return false;

	SDL_MouseButtonEvent *mouse_ev = (SDL_MouseButtonEvent*)&event;
	
	if (mouse_ev->x >= x && mouse_ev->x < x + width &&
	      mouse_ev->y >= y && mouse_ev->y < y + height) {
		press_action();
		pressed_this_frame = true;
	}

	return true;
}

ulong dark_color(ulong color)
{
	ulong SUBTRACT = 0x222222FF;
	// Darker color is "lower down" unless black, else white
	if (color == 0x000000FF)
		return 0xFFFFFFFF;
	else if (color < SUBTRACT)
		return 0xFFFFFFFF;
	else
		return color - SUBTRACT;
}

void GUI_Button::draw(SDL_Renderer *rend)
{
	SDL_Rect draw_rect;
	draw_rect.x = x;
	draw_rect.y = y;
	draw_rect.w = width;
	draw_rect.h = height; 

	if (!pressed_this_frame)
		outer_pane->switch_color(rend, color);
	else 
		outer_pane->switch_color(rend, dark_color(color));

	SDL_RenderFillRect(rend, &draw_rect);
	outer_pane->switch_color(rend, BORDER_COLOR);
	SDL_RenderDrawRect(rend, &draw_rect);

	pressed_this_frame = false;

	outer_pane->inner_font->draw_string(text, x + WIDTH_TEXTPAD, 
										y + HEIGHT_TEXTPAD, BUTTON_LETTERSPACE);
}

void GUI_Button::press_action()
{
	button_func(outer_pane);
}
