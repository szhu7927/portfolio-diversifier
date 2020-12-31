#include <cmath>
#include <vector>

std::vector<float> percgrowth(std::vector<float> vec)
{
	std::vector<float> growthvec;
	for (int i = 0; i < vec.size() - 1; i++) {
		growthvec.push_back((vec[i + 1] - vec[i]) / vec[i]);
	}
	return growthvec;
}

float mean_func(std::vector<float> vec)
{
	float sum = 0;
	for (auto x : vec) sum += x;
	return sum / vec.size();
}

float var(std::vector<float> vec)
{
	float sum_num = 0;
	float ev = mean_func(vec);

	for (auto x : vec) sum_num += pow(x - ev, 2);

	return sum_num / (vec.size() - 1);
}

float stdev_func(std::vector<float> vec)
{
	return pow(var(vec), 0.5);
}

float covar(std::vector<float> vec1, std::vector<float> vec2)
{
	if (vec1.size() != vec2.size()) throw "Vector sizes not equal";

	float sum_num = 0;
	float ev1 = mean_func(vec1);
	float ev2 = mean_func(vec2);

	for (int i = 0; i < vec1.size(); i++) {
		sum_num += (vec1[i] - ev1) * (vec2[i] - ev2);
	}

	return sum_num / (vec1.size() - 1);
}

struct normal {
	std::vector<float> vec;
	float mean;
	float stdev;

	normal(std::vector<float> vec) {
		this->vec = vec;
		mean = mean_func(vec);
		stdev = stdev_func(vec);
	}
};

struct comb_normal {
	float mean;
	float stdev;

	comb_normal(normal normal1, normal normal2, float ratio) {
		if (ratio < 0 || ratio > 1) throw "Ratio not between 0 and 1 (inclusive)";

		mean = normal1.mean * ratio + normal2.mean * (1 - ratio);
		stdev = pow(pow(normal1.stdev * ratio, 2) + pow(normal2.stdev * (1 - ratio), 2) + 2 * ratio * (1 - ratio) * covar(normal1.vec, normal2.vec), 0.5);
	}
};