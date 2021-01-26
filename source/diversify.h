#pragma once

#ifdef _WIN32
	#include ".\public\storedata.h"
	#include ".\public\frontieralgo.h"
	#include ".\public\risktopointalgo.h"
#else
	#include "./public/storedata.h"
	#include "./public/frontieralgo.h"
	#include "./public/risktopointalgo.h"
#endif

void areatest();
int fakemain();

std::vector<CombNormal> rawpoints(std::vector<std::string> input_etf_list, double increment);
std::vector<CombNormal> bestpoints(std::vector<std::string> input_etf_list, double increment);
CombNormal allocation(std::vector<CombNormal> frontier, double risk);