#ifdef _WIN32
 #include "..\public\ymath.h"
#else
 #include "../public/ymath.h"
#endif

std::vector<double> percgrowth(std::vector<double> vec)
{
	std::vector<double> growthvec;
	for (unsigned int i = 0; i < vec.size() - 1; i++) {
		growthvec.push_back((vec[i + 1] - vec[i]) / vec[i] * 100);
	}
	return growthvec;
}

double mean_func(std::vector<double> vec)
{
	double sum = 0;
	for (auto x : vec) sum += x;
	return sum / vec.size();
}

double var(std::vector<double> vec)
{
	double sum_num = 0;
	double ev = mean_func(vec);

	for (auto x : vec) sum_num += pow(x - ev, 2);

	return sum_num / (vec.size() - 1);
}

double stdev_func(std::vector<double> vec)
{
	return pow(var(vec), 0.5);
}

double covar(std::vector<double> vec1, std::vector<double> vec2)
{
	if (vec1.size() != vec2.size()) throw "Vector sizes not equal";

	double sum_num = 0;
	double ev1 = mean_func(vec1);
	double ev2 = mean_func(vec2);

	for (unsigned int i = 0; i < vec1.size(); i++) {
		sum_num += (vec1[i] - ev1) * (vec2[i] - ev2);
	}

	return sum_num / (vec1.size() - 1);
}

Normal::Normal(std::string etf, std::vector<double> vec) {
	this->etf = etf;
	this->vec = vec;
	mean = mean_func(vec);
	stdev = stdev_func(vec);
};

CombNormal::CombNormal(std::vector<std::pair<Normal, double>> vec) {
	//AKA a DataPoint
	//Scale is sum of weights
	double scale = 0;
	std::vector<std::pair<std::string, double> > weights;
	for (std::pair<Normal, double> entry : vec) {
		scale += entry.second;
		weights.push_back(std::make_pair(entry.first.etf, entry.second));
	}

	mean = 0;
	double var = 0;
	for (int i = 0; i < vec.size(); ++i) {
		mean += vec[i].first.mean * vec[i].second / scale;
		var += pow(vec[i].first.stdev * vec[i].second / scale, 2);

		for (int j = i + 1; j < vec.size(); ++j) {
			var += 2 * (vec[i].second / scale) * (vec[j].second / scale) * covar(vec[i].first.vec, vec[j].first.vec);
		}
	}

	stdev = pow(var, 0.5);
	ETF_weights = weights;
};

CombNormal::CombNormal(double stdev, double mean) {
	this->stdev = stdev;
	this->mean = mean;
	std::vector<std::pair<std::string, double> > weights;
	weights.push_back(std::make_pair("DEBUG", 0.0));
	weights.push_back(std::make_pair("DEBUG", 0.0));
	ETF_weights = weights;
	//Constructor used for debugging
};

void CombNormal::print() {
	for (int i = 0; i < ETF_weights.size(); ++i) {
		std::cout << ETF_weights[i].first << " weight = " << ETF_weights[i].second << std::endl;
	}
	std::cout << "Return: " << mean << std::endl;
	std::cout << "Risk: " << stdev << "\n\n";
};

void CombNormal::xyprint() {
	//To print a combnormal like a coordinate
	std::cout << "(" << stdev << ", " << mean << ")";
}
