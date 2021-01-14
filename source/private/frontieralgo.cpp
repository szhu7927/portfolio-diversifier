#include "..\public\ymath.h"
#include "..\public\date.h"
#include "..\public\dataframe.h"
#include "..\public\frontieralgo.h"

std::vector<CombNormal> find_optimal_points(DataFrame df, std::vector<std::string> input_etf_list) {
	//ONLY WORKS WITH 2 ETFs RIGHT NOW
	
	//Makes the vector of input ETFs into a vector of Normal objects
	std::vector<Normal> normallist;
	for (std::string etf : input_etf_list) {
		Normal singlenormal(etf, df.etfdata(etf));
		normallist.push_back(singlenormal);
	}

	//Generator of all points (CombNormals) in increments of 10%. (2 ETF limit)
	std::vector<CombNormal> pointlist;
	for (double i = 0; i <= 1; i += .1) {

		//Creates pairs of ETFs and their weights.
		std::pair<Normal, double> etfweight1 = std::make_pair(normallist[0], i);
		std::pair<Normal, double> etfweight2 = std::make_pair(normallist[1], 1 - i);

		//Makes both of these pairs into a CombNormal, and adds it to the list.
		std::vector<std::pair<Normal, double>> pairvector = { etfweight1, etfweight2 };
		pointlist.push_back(CombNormal(pairvector));
	}

	//DEBUG: print all rewards and risks
	for (CombNormal datapoint : pointlist) {
		datapoint.print();
	}

	//DEBUG
	std::vector<CombNormal> test;
	return test;
}