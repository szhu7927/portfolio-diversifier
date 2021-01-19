#include "guipane.h"
#include <iostream>
#include <string>
#include <vector>

#define FONTCHARS " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`-=[]\\;',./~!@#$%^&*()_+{}|:\"<>?"
#define FONTSIZE_H 32
#define FONTSIZE_W 22
#define FONT_HEIGHT 17
#define FONT_WIDTH 11

#define WIN_HEIGHT 500
#define WIN_WIDTH 500

void rando_print(GUI_Pane *pane){
	std::cout << "test!\n";
}

int main(int argc, char **argv)
{
	GUI_Pane *gui = new GUI_Pane("test!", WIN_WIDTH, WIN_HEIGHT, 0xFFFFFFFF,
								 "tom_vii_font.png", FONTCHARS, 
								 FONT_WIDTH, FONT_HEIGHT, FONTSIZE_W, FONTSIZE_H);
	
	gui->add_button("Print in console",100, 100,0xBEEF12FF, rando_print);
	
	std::vector<int> testx;
	testx.push_back(1);
	testx.push_back(2);
	testx.push_back(3);
	testx.push_back(4);
	std::vector<int> testy;
	testy.push_back(5);
	testy.push_back(2);
	testy.push_back(9);
	testy.push_back(2);
	
	//GUI_Graph *g = gui->add_graph(20, 20, 450, 450, testx, testy, 0x1234EEFF);
	//GUI_Graph *g2 = gui->add_graph(20, 230, 200, 200, testx, testy, 0x1234EEFF);
	//GUI_Graph *g3 = gui->add_graph(230, 20, 200, 200, testx, testy, 0x1234EEFF);
	//GUI_Graph *g4 = gui->add_graph(230, 230, 200, 200, testx, testy, 0x1234EEFF);
	//g->tick_x_min = 0;
	//g->tick_y_min = 0;

	gui->loop();

	delete gui;
	return EXIT_SUCCESS;
}
