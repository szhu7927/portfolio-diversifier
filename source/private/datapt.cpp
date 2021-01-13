#include "..\public\ymath.h"
#include "..\public\date.h"
#include "..\public\dataframe.h"
#include "..\public\datapt.h"

DataPT::DataPT(double reward, double risk, std::vector<std::pair<std::string, double>> ETF_weights) {
	this->reward = reward;
	this->risk = risk;
	this->ETF_weights = ETF_weights;
}