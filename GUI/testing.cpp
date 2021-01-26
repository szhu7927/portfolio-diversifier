#include "guipane.h"
#include "../source/diversify.h"
#include <iostream>
#include <string>
#include <vector>

#define FONTCHARS " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`-=[]\\;',./~!@#$%^&*()_+{}|:\"<>?"
#define FONTSIZE_H 32
#define FONTSIZE_W 22
#define FONT_HEIGHT 17
#define FONT_WIDTH 11

#define WIN_HEIGHT 500
#define WIN_WIDTH 950

void rando_print(GUI_Pane *pane){
	std::cout << "test!\n";
}

int main(int argc, char **argv)
{
	GUI_Pane *gui = new GUI_Pane("test!", WIN_WIDTH, WIN_HEIGHT, 0xFFFFFFFF,
								 "tom_vii_font.png", FONTCHARS, 
								 FONT_WIDTH, FONT_HEIGHT, FONTSIZE_W, FONTSIZE_H);
	
//	gui->add_button("picbuttn", "Print in console",100, 100,0xBEEF12FF, rando_print);

	//Old test points
	/*
	std::vector<float> testx;
	testx.push_back(1);
	testx.push_back(2);
	testx.push_back(3);
	testx.push_back(4);
	std::vector<float> testy;
	testy.push_back(5);
	testy.push_back(2);
	testy.push_back(9);
	testy.push_back(2);
	*/
	
	std::vector<std::string> input_etf_list = { "SPY", "VTV" };
	double increment = .1;

	std::vector<CombNormal> rpoints = rawpoints(input_etf_list, increment);
	std::vector<float> rawx;
	std::vector<float> rawy;

	for (CombNormal normal : rpoints) {
		rawx.push_back((float)normal.stdev);
		rawy.push_back((float)normal.mean);
	}

	std::vector<CombNormal> bpoints = bestpoints(input_etf_list, increment);
	std::vector<float> bestx;
	std::vector<float> besty;

	for (CombNormal normal : bpoints) {
		bestx.push_back((float)normal.stdev);
		besty.push_back((float)normal.mean);
	}

	std::cout << "besty ";
	for (int i = 0; i < besty.size(); i++)
		std::cout << bestx[i] << ", " << besty[i] << "; ";
	std::cout << "\n";
	std::cout << "rawy ";
	for (int i = 0; i < rawy.size(); i++)
		std::cout << rawx[i] << ", " << rawy[i] << "; ";
	std::cout << "\n";

	GUI_Graph *g = gui->add_graph("graph", 20, 20, 450, 450, rawx, rawy, 0x1234EEFF);
	GUI_Graph *g2 = gui->add_graph("graph", 480, 20, 450, 450, bestx, besty, 0x1234EEFF);

	gui->loop();

	delete gui;
	return EXIT_SUCCESS;
}
