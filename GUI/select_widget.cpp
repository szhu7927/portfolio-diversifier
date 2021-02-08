#include "guipane.h" // NOT guiwidget! because it uses pane functions
#define SELECTABLE_LETTERSPACE 18
#define WIDTH_TEXTPAD 10
#define HEIGHT_TEXTPAD 10
#define BORDER_COLOR 0x000000FF // Black

#define OPTION_HEIGHT outer_pane->inner_font->height + (2 * HEIGHT_TEXTPAD)
#define OPTION_WIDTH (longest_name_length * outer_pane->inner_font->width) + (2 * WIDTH_TEXTPAD)
#define SELECTED_COLOR_OFF 0x222222FF

ulong convert_selected(ulong color)
{
	// color is "lower down" unless black, else white
	if (color == 0x000000FF)
		return 0xFFFFFFFF;
	else if (color < SELECTED_COLOR_OFF)
		return 0xFFFFFFFF;
	else
		return color - SELECTED_COLOR_OFF;
}

GUI_Select::GUI_Select(GUI_Pane *outer, int x_cor, int y_cor, ulong color)
{
	length = 0;
	unselected_color = color;
	selected_color = convert_selected(color);
	longest_name_length = 0;
	outer_pane = outer;
	x = x_cor;
	y = y_cor;
}

GUI_Select::~GUI_Select() {}

void GUI_Select::add_option(const std::string &option)
{
	names.push_back(option);
	toggles.push_back(false);
	if (option.length() > longest_name_length)
		longest_name_length = option.length();
	length++;
}

void GUI_Select::add_options(std::vector<std::string> str_vec)
{
	for (int i = 0; i < str_vec.size(); i++)
		add_option(str_vec[i]);
}


bool GUI_Select::update(SDL_Event event)
{
	if (event.type != SDL_MOUSEBUTTONDOWN)
		return false;

	SDL_MouseButtonEvent *mouse_ev = (SDL_MouseButtonEvent*)&event;
	int click_off_x = mouse_ev->x - x;
	int click_off_y = mouse_ev->y - y;

	/* Toggle index if it's clicked */
	if (click_off_x >= 0 && click_off_x <= OPTION_WIDTH	&&  
			click_off_y >= 0 && click_off_y <= OPTION_HEIGHT * length)
	{
		for (int i = 0; i < length; i++)
			if (click_off_y < OPTION_HEIGHT * (i + 1)) {
				toggles[i] = !toggles[i];
				return true;
			}
	}

	return false;
}


void GUI_Select::draw(SDL_Renderer *rend)
{
	/* Current plan: Just draw a bunch of rectangles lmao */
	SDL_Rect draw_rect;
	draw_rect.x = x;
	draw_rect.y = y;
	draw_rect.w = OPTION_WIDTH;
	draw_rect.h = OPTION_HEIGHT;

	for (int i = 0; i < length; i++)
	{
		if (toggles[i])
			outer_pane->switch_color(rend, unselected_color);
		else 
			outer_pane->switch_color(rend, selected_color);

		SDL_RenderFillRect(rend, &draw_rect);

		outer_pane->inner_font->draw_string(names[i], draw_rect.x + WIDTH_TEXTPAD, 
											draw_rect.y + HEIGHT_TEXTPAD, SELECTABLE_LETTERSPACE);

		draw_rect.y += OPTION_HEIGHT;
	}
}
