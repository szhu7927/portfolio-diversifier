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

std::vector<CombNormal> standard_algo(std::vector<CombNormal> rawlist) {
	//Starts off with one point
	std::vector<CombNormal> processed = { rawlist[0] };

	int count = 0;

	for (int index = 1; index < rawlist.size(); ++index) {

		//Gathering the point to be processed
		CombNormal new_point = rawlist[index];
		std::cout << "\nPOINT: ";
		new_point.xyprint();
		std::cout << std::endl;

		/*
		Two cases: (1) new_point has a higher risk than any other proccessed point, (2) new_point has a lower risk
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

			count++;
			std::cout << "COUNT: " << count << ", INSERTINDEX: " << processed.size() - 1 << std::endl;

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
		for (int i = 0; i < processed.size() - 1; ++i) {
			if (new_point.stdev < processed[i].stdev) {
				insertindex = i;
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
		while (processed.size() > insertindex && new_point.mean > processed[insertindex].mean) {
			std::cout << "ERASING: ";
			processed[insertindex].xyprint();
			std::cout << std::endl;
			processed.erase(processed.begin() + insertindex);
		}

		//Fourth and finally, we insert new_point into the processed list at index insertindex.
		processed.insert(processed.begin() + insertindex, new_point);

		count++;
		std::cout << "COUNT: " << count << ", INSERTINDEX: " << insertindex << std::endl;
	}

	return processed;
}

std::vector<std::vector<double>> weight_generator(double inc, int length) {
	//Brute force, it doesn't look good but it's what works best right now
	std::vector<std::vector<double>> builder;
	switch (length) {
	case 2:
		for (double i = 0; i <= 1; i += inc) {
			builder.push_back({ i, 1 - i });
		}
		break;
	case 3:
		for (double i = 0; i <= 1; i += inc) {
			for (double j = 0; j <= 1 - i; j += inc) {
				builder.push_back({ i, j, 1 - i - j });
			}
		}
		break;
	case 4:
		for (double i = 0; i <= 1; i += inc) {
			for (double j = 0; j <= 1 - i; j += inc) {
				for (double k = 0; k <= 1 - i - j; k += inc) {
					builder.push_back({ i, j, k, 1-i-j-k });
				}
			}
		}
		break;
	case 5:
		for (double i = 0; i <= 1; i += inc) {
			for (double j = 0; j <= 1 - i; j += inc) {
				for (double k = 0; k <= 1 - i - j; k += inc) {
					for (double l = 0; l <= 1 - i - j - k; k += inc) {
						builder.push_back({ i, j, k, l, 1-i-j-k-l});
					}
				}
			}
		}
		break;
	case 6:
		for (double i = 0; i <= 1; i += inc) {
			for (double j = 0; j <= 1 - i; j += inc) {
				for (double k = 0; k <= 1 - i - j; k += inc) {
					for (double l = 0; l <= 1 - i - j - k; k += inc) {
						for (double m = 0; m <= 1 - i - j - k - l; m += inc) {
							builder.push_back({ i, j, k, l, m, 1 - i - j - k - l - m});
						}
					}
				}
			}
		}
		break;
	case 7:
		for (double i = 0; i <= 1; i += inc) {
			for (double j = 0; j <= 1 - i; j += inc) {
				for (double k = 0; k <= 1 - i - j; k += inc) {
					for (double l = 0; l <= 1 - i - j - k; k += inc) {
						for (double m = 0; m <= 1 - i - j - k - l; m += inc) {
							for (double n = 0; n <= 1 - i - j - k - l - m; n += inc) {
								builder.push_back({ i, j, k, l, m, n, 1 - i - j - k - l - m - n });
							}
						}
					}
				}
			}
		}
		break;
	case 8:
		for (double i = 0; i <= 1; i += inc) {
			for (double j = 0; j <= 1 - i; j += inc) {
				for (double k = 0; k <= 1 - i - j; k += inc) {
					for (double l = 0; l <= 1 - i - j - k; k += inc) {
						for (double m = 0; m <= 1 - i - j - k - l; m += inc) {
							for (double n = 0; n <= 1 - i - j - k - l - m; n += inc) {
								for (double o = 0; o <= 1 - i - j - k - l - m - n; o += inc) {
									builder.push_back({ i, j, k, l, m, n, o, 1 - i - j - k - l - m - n - o });
								}
							}
						}
					}
				}
			}
		}
		break;
	case 9:
		for (double i = 0; i <= 1; i += inc) {
			for (double j = 0; j <= 1 - i; j += inc) {
				for (double k = 0; k <= 1 - i - j; k += inc) {
					for (double l = 0; l <= 1 - i - j - k; k += inc) {
						for (double m = 0; m <= 1 - i - j - k - l; m += inc) {
							for (double n = 0; n <= 1 - i - j - k - l - m; n += inc) {
								for (double o = 0; o <= 1 - i - j - k - l - m - n; o += inc) {
									for (double p = 0; p <= 1 - i - j - k - l - m - n - o; p += inc) {
										builder.push_back({ i, j, k, l, m, n, o, p, 1 - i - j - k - l - m - n - o - p});
									}
								}
							}
						}
					}
				}
			}
		}
		break;
	case 10:
		for (double i = 0; i <= 1; i += inc) {
			for (double j = 0; j <= 1 - i; j += inc) {
				for (double k = 0; k <= 1 - i - j; k += inc) {
					for (double l = 0; l <= 1 - i - j - k; k += inc) {
						for (double m = 0; m <= 1 - i - j - k - l; m += inc) {
							for (double n = 0; n <= 1 - i - j - k - l - m; n += inc) {
								for (double o = 0; o <= 1 - i - j - k - l - m - n; o += inc) {
									for (double p = 0; p <= 1 - i - j - k - l - m - n - o; p += inc) {
										for (double q = 0; p <= 1 - i - j - k - l - m - n - o - p; q += inc) {
											builder.push_back({ i, j, k, l, m, n, o, p, q, 1 - i - j - k - l - m - n - o - p - q });
										}
									}
								}
							}
						}
					}
				}
			}
		}
		break;
	}
	//See how many points are generated.
	//std::cout << builder.size() << std::endl;
	return builder;
}

std::vector<CombNormal> raw_point_generator(DataFrame df, std::vector<std::string> input_etf_list, double inc) {
	//Precondition: inc must evenly divide 1.
	//ONLY WORKS WITH 2 ETFs RIGHT NOW

	//Makes sure that increment is fits the conditions: 0 < inc <= 1.
	if (0 >= inc || inc > 1) throw std::invalid_argument("The increment must be greater than zero, and less than or equal to 1.");

	//Instantiates variables used
	std::vector<std::pair<Normal, double>> pairvector;
	std::vector<CombNormal> rawpointlist;

	//Generates the vector of Normals
	std::vector<Normal> normallist;
	for (std::string etf : input_etf_list) {
		Normal singlenormal(etf, percgrowth(df.etfdata(etf)));
		normallist.push_back(singlenormal);
	}

	//Generates the weightings (x amount of doubles that add up to 1)
	std::vector<std::vector<double>> weightings = weight_generator(inc, input_etf_list.size());

	//Combines normallist and weight_generator to add each possible CombNormal to the list.
	for (std::vector<double> single_weight_list : weightings) {
		for (int i = 0; i < single_weight_list.size(); ++i) {
			std::pair<Normal, double> weight_pair = std::make_pair(normallist[i], single_weight_list[i]);
			pairvector.push_back(weight_pair);
		}
		rawpointlist.push_back(CombNormal(pairvector));
		pairvector = {};
	}
	return rawpointlist;
}

std::vector<CombNormal> find_optimal_points(DataFrame df, std::vector<std::string> input_etf_list, double inc) {
	//Precondition: inc must evenly divide 1.
	
	//Generates and stores a list of points.
	std::vector<CombNormal> rawpointlist = raw_point_generator(df, input_etf_list, inc);

	//Algorithm select.
	//1 = standard algorithm. 2 = no processing at all
	//We may add algorithms and switch between them depending on the amount of points there are. 3 is for debugging.
	int algoselect = 1;
	std::vector<CombNormal> processedpoints;
	switch (algoselect) {
	case 1:
		processedpoints = standard_algo(rawpointlist);
		break;
	case 2:
		processedpoints = rawpointlist;
		break;
	}

	//DEBUG: print all rewards and risks
	/*
	for (CombNormal datapoint : processedpoints) {
		datapoint.print();
	}
	*/

	return processedpoints;
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
	std::vector<CombNormal> testcase1_result = standard_algo(testcase1);

	//prints the processed points
	std::cout << "\nPROCESSED LIST: " << std::endl;
	for (CombNormal datapoint : testcase1_result) {
		datapoint.xyprint();
	}
	std::cout << std::endl;
}
