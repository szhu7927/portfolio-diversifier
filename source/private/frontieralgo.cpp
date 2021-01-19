#include "..\public\ymath.h"
#include "..\public\date.h"
#include "..\public\dataframe.h"
#include "..\public\frontieralgo.h"

std::vector<CombNormal> sarahs_algo(std::vector<CombNormal> rawlist) {
	//Starts off with one point
	std::cout << "\n\n" << "PROCESSING POINT: ";
	rawlist[0].xyprint();
	std::cout << std::endl;
	std::cout << "AUTO-INSERTED" << "\n";
	std::vector<CombNormal> processed = { rawlist[0] };

	for (int index = 1; index < rawlist.size(); ++index) {

		//Gathering the point to be processed
		CombNormal new_point = rawlist[index];

		std::cout << "CURRENT LIST: ";
		for (int d = 0; d < processed.size(); ++d) {
			processed[d].xyprint();
		}

		std::cout << "\n\n";
		std::cout << "PROCESSING POINT: ";
		rawlist[index].xyprint();
		std::cout << std::endl;

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
			std::cout << "CASE 1: HIGHEST RISK AND RETURN" << std::endl;
			//Add the point to the end if the return is greater, otherwise we do not process it.
			if (new_point.mean > processed[processed.size() - 1].mean) {
				std::cout << "POINT ADDED" << std::endl;
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
		std::cout << "INSERTINDEX = " << insertindex << std::endl;

		//Second, we do not process new_point if its return is less than the found processed point (of index insertindex).
		//We do not check for the previous point if insertindex = 0, as there is none.
		if (insertindex != 0 && new_point.mean < processed[insertindex - 1].mean) {
			std::cout << "RETURN IS LESS THAN START: TRASHED" << std::endl;
			continue;
		}

		//Third, we delete any/all processed points that has greater risk but less return.
		//This breaks once the processed point's return is above that of new_point, as each subsequent point's return must be greater.
		for (int j = insertindex; j < processed.size(); ++j) {
			std::cout << "POINT SCAN: ";
			std::cout << "index = " << j << ", ";
			processed[j].xyprint();
			std::cout << std::endl;
			if (new_point.mean > processed[j].mean) {
				std::cout << "ERASING POINT: ";
				processed[j].xyprint();
				std::cout << std::endl;
				processed.erase(processed.begin() + j);
				j--;
			}
			else if (new_point.mean < processed[j].mean) {
				std::cout << "NO MORE DELETIONS" << std::endl;
				break;
			}
		}

		//Fourth and finally, we insert new_point into the processed list at index insertindex.
		std::cout << "POINT ADDED" << std::endl;
		processed.insert(processed.begin() + insertindex, new_point);
	}

	return processed;
}

std::vector<CombNormal> find_optimal_points(DataFrame df, std::vector<std::string> input_etf_list, double inc) {
	//Precondition: inc must evenly divide 1.
	//ONLY WORKS WITH 2 ETFs RIGHT NOW

	//Makes sure that increment is fits the conditions: 0 < inc <= 1.
	if (0 >= inc || inc > 1) throw std::invalid_argument("The increment must be greater than zero, and less than or equal to 1.");

	//Makes the vector of input ETFs into a vector of Normal objects
	std::vector<Normal> normallist;
	for (std::string etf : input_etf_list) {
		Normal singlenormal(etf, percgrowth(df.etfdata(etf)));
		normallist.push_back(singlenormal);
	}

	//Generator of all points (CombNormals) for the given increment. (2 ETF limit)
	std::vector<CombNormal> pointlist;
	for (double i = 0; i <= 1; i += inc) {

		//Creates pairs of ETFs and their weights.
		std::pair<Normal, double> etfweight1 = std::make_pair(normallist[0], i);
		std::pair<Normal, double> etfweight2 = std::make_pair(normallist[1], 1 - i);

		//Makes both of these pairs into a CombNormal, and adds it to the list.
		std::vector<std::pair<Normal, double>> pairvector = { etfweight1, etfweight2 };
		pointlist.push_back(CombNormal(pairvector));
	}

	//Algorithm select.
	//1 = sarah's algorithm. 2 = matt's algorithm. 3 = no processing at all
	//Ideally we will select from 1 or 2 depending on the amount of points there are. 3 for debugging.
	int algoselect = 1;
	std::vector<CombNormal> processedpoints;
	switch (algoselect) {
	case 1:
		//WIP
		processedpoints = sarahs_algo(pointlist);
		break;
	case 2:
		//TODO
		break;
	case 3:
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
	for (CombNormal datapoint : testcase1) {
		datapoint.xyprint();
	}

	//runs algo
	std::vector<CombNormal> testcase1_result = sarahs_algo(testcase1);

	//prints the processed points
	std::cout << "\nFINAL LIST: " << std::endl;
	for (CombNormal datapoint : testcase1_result) {
		datapoint.xyprint();
		std::cout << std::endl;
	}
}