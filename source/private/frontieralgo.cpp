#ifdef _WIN32
 #include "..\public\ymath.h"
 #include "..\public\date.h"
 #include "..\public\dataframe.h"
 #include "..\public\frontieralgo.h"
#else
 #include "../public/ymath.h"
 #include "../public/date.h"
 #include "../public/dataframe.h"
 #include "../public/frontieralgo.h"
#endif

std::vector<CombNormal> sarahs_algo(std::vector<CombNormal> rawlist) {
	//Starts off with one point
	std::vector<CombNormal> processed = { rawlist[0] };

	for (int index = 1; index < rawlist.size(); ++index) {

		//Gathering the point to be processed
		CombNormal new_point = rawlist[index];

		/*
		Three cases: (1) new_point has a higher risk than any other proccessed point, (2) new_point has a lower risk
		than any other processed point / new_point has a risk in between two processed points (most likely).

		Not currently accounting for equal risk values.

		The processed point list is sorted in increasing risk; therefore, processedpoints[0] is the lowest,
		processedpoints[size] is the highest.
		*/

		//Case 1: the new-point has a higher risk than any other proccessed point.
		//Simple, as no already processed points need to be deleted.
		if (new_point.stdev > processed[processed.size() - 1].stdev) {
			//Add the point to the end if the return is greater, otherwise we do not process it.
			if (new_point.mean > processed[processed.size() - 1].mean) {
				processed.push_back(new_point);
			}

			//No need to look at this point further; it is already processed or not processed.
			continue;
		}

		/*
		Case 2: new-point has a lower risk than any other proccessed point.
		Other processed points may be deleted if this one has a greater return and a smaller risk.

		First, we find out where new_point's risk value is relative to others.
		This is done by finding the first processed point that has a greater risk than it.
		*/
		int insertindex = 0;
		for (int i = 0; i < processed.size(); ++i) {
			if (new_point.stdev > processed[i].stdev) {
				insertindex = i + 1;
				break;
			}
		}

		//Second, we do not process new_point if its return is less than the found processed point (of index insertindex).
		//We do not check for the previous point if insertindex = 0, as there is none.
		if (insertindex != 0 && new_point.mean < processed[insertindex - 1].mean) {
			continue;
		}

		//Third, we delete any/all processed points that has greater risk but less return.
		//This breaks once the processed point's return is above that of new_point, as each subsequent point's return must be greater.
		for (int j = insertindex; j < processed.size(); ++j) {
			if (new_point.mean > processed[j].mean) {
				processed.erase(processed.begin() + j);
				j--;
			}
			else if (new_point.mean < processed[j].mean) {
				break;
			}
		}

		//Fourth and finally, we insert new_point into the processed list at index insertindex.
		processed.insert(processed.begin() + insertindex, new_point);
	}

	return processed;
}

std::vector<CombNormal> raw_point_generator(DataFrame df, std::vector<std::string> input_etf_list, double inc) {
	//Precondition: inc must evenly divide 1.
	//ONLY WORKS WITH 2 ETFs RIGHT NOW

	//Makes sure that increment is fits the conditions: 0 < inc <= 1.
	if (0 >= inc || inc > 1) throw std::invalid_argument("The increment must be greater than zero, and less than or equal to 1.");

	//Finds out how many ETFs are being worked with.
	int etfsize = input_etf_list.size();

	//Makes the vector of input ETFs into a vector of Normal objects
	std::vector<Normal> normallist;
	for (std::string etf : input_etf_list) {
		Normal singlenormal(etf, percgrowth(df.etfdata(etf)));
		normallist.push_back(singlenormal);
	}

	//Instantiates where the CombNormal points will be stored.
	std::vector<CombNormal> rawpointlist;

	//Generator of all points (CombNormals) for the given increment.
	//Weightings for 2 ETFs.
	for (double i = 0; i <= 1; i += inc) {

		//Creates pairs of ETFs and their weights.
		std::pair<Normal, double> etfweight1 = std::make_pair(normallist[0], i);
		std::pair<Normal, double> etfweight2 = std::make_pair(normallist[1], 1 - i);

		//Makes both of these pairs into a CombNormal, and adds it to the list.
		std::vector<std::pair<Normal, double>> pairvector = { etfweight1, etfweight2 };
		rawpointlist.push_back(CombNormal(pairvector));
	}
	
	/*
	//Builds off of the previous generator for 3 or more ETFs.
	std::vector<CombNormal> templist = rawpointlist;
	std::vector<CombNormal> accuraterawpointlist;

	if (etfsize > 2) {
		//For each to-be-considered ETF in the list.
		for (int index = 2; index < etfsize; ++index) {

			//Index of rawpointlist to be cloned 1/inc + 1 times with different weights.
			for (double wc = 0; wc < rawpointlist.size(); ++wc) {

				//Finds the already-espablished vector of weights of the raw point.
				std::vector<std::pair<Normal, double>> extendedweight;
				for (int j = 0; j < rawpointlist[wc].ETF_weights.size(); ++j) {
					std::string newetf = rawpointlist[wc].ETF_weights[j].first;

					//Finds which normal corresponds with the string, and then adds the pair to the list.
					int normalindex = 0;
					for (int k = 0; k < normallist.size(); ++k) {
						if (normallist[k].etf == newetf) {
							normalindex = k;
							break;
						}
					}

					//Makes a pair and adds it to the established pair list.
					Normal estnormal = normallist[normalindex];
					double estweight = rawpointlist[wc].ETF_weights[j].second;
					std::pair<Normal, double> newpair = std::make_pair(estnormal, estweight);
					extendedweight.push_back(newpair);
				}

				//Generates different weights in increments of inc.
				for (double i = 0; i <= 1; i += inc) {

					//Generates new weight paired with its ETF.
					std::pair<Normal, double> newweight = std::make_pair(normallist[index], i);

					//Merges the two vectors.
					std::vector<std::pair<Normal, double>> weightclone = extendedweight;
					weightclone.push_back(newweight);

					//Regenerate this point, and add it to templist.
					CombNormal addition(weightclone);
					templist.push_back(addition);
				}
			}

			//After each ETF is processed, update the rawpointlist.
			rawpointlist = templist;
		}

		//Trashes the points where the weights do not add up to 1.
		for (int r = 0; r < rawpointlist.size(); ++r) {
			double add_to_one = 0.0;
			for (int s = 0; s < rawpointlist[r].ETF_weights.size(); ++s) {
				add_to_one += rawpointlist[r].ETF_weights[s].second;
			}
			if (add_to_one > .996 && add_to_one < 1.004) {
				accuraterawpointlist.push_back(rawpointlist[r]);
			}
		}
	}

	for (int y = 0; y < rawpointlist.size(); ++y) {
		accuraterawpointlist[y].print();
	}
	
	return accuraterawpointlist;
	*/
	return rawpointlist;
}

std::vector<CombNormal> find_optimal_points(DataFrame df, std::vector<std::string> input_etf_list, double inc) {
	//Precondition: inc must evenly divide 1.
	
	//Generates and stores a list of points.
	std::vector<CombNormal> rawpointlist = raw_point_generator(df, input_etf_list, inc);

	//Algorithm select.
	//1 = sarah's algorithm. 2 = no processing at all
	//We may add algorithms and switch between them depending on the amount of points there are. 3 is for debugging.
	int algoselect = 1;
	std::vector<CombNormal> processedpoints;
	switch (algoselect) {
	case 1:
		processedpoints = sarahs_algo(rawpointlist);
		break;
	case 2:
		processedpoints = rawpointlist;
		break;
	}

	//DEBUG: print all rewards and risks
	for (CombNormal datapoint : processedpoints) {
		datapoint.print();
	}

	//DEBUG
	std::vector<CombNormal> test;
	return test;
}

//Run this to test the algorithm
void fa_debug()
{
	//constructs testcase
	std::vector<CombNormal> testcase1 = { CombNormal(2, 2), CombNormal(1, 1), CombNormal(3,3), CombNormal(4,4), CombNormal(5,5), CombNormal(.5, 5.5) };

	//prints the pre-processed points
	std::cout << "\nRAW LIST: " << std::endl;
	for (CombNormal datapoint : testcase1) {
		datapoint.xyprint();
	}
	std::cout << std::endl;

	//runs algo
	std::vector<CombNormal> testcase1_result = sarahs_algo(testcase1);

	//prints the processed points
	std::cout << "\nPROCESSED LIST: " << std::endl;
	for (CombNormal datapoint : testcase1_result) {
		datapoint.xyprint();
	}
	std::cout << std::endl;
}
