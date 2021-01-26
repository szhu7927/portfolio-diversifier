#include "diversify.h"

void areatest() {
    std::vector<std::pair<double, double> > vec;
    vec.push_back(std::make_pair(0, 0));
    vec.push_back(std::make_pair(0, .5));
    vec.push_back(std::make_pair(1, 1));
    vec.push_back(std::make_pair(1, 0));

    std::cout << "Area 1: " << area(vec) << " (Should be 0.75) \n";

    std::vector<std::pair<double, double> > vec1;
    vec1.push_back(std::make_pair(0, 0));
    vec1.push_back(std::make_pair(1, 3));
    vec1.push_back(std::make_pair(7, 7));
    vec1.push_back(std::make_pair(2, 4));
    vec1.push_back(std::make_pair(6, 5));

    std::cout << "Area 2: " << totalarea(vec1) << " (Should be 6.5) \n \n";

    std::vector<std::pair<std::pair<double, double>, double> > vec2 = cumareavector(vec1);
    printareas(vec2);

    std::cout << "\n";

    double risk = 0.7;
    std::pair<double, double> point = bestpoint(vec2, risk);

    std::cout << "For risk " << risk << ", the best point is (" << point.first << ", " << point.second << ") \n";
}

//Change back to main to test
int fakemain()
{
    //testing the algorithm
    //fa_debug();
    DataFrame AlphaFrame = read_csv();
    //AlphaFrame.print();
    //Options: SPY, IWF, VTV, SCHD, USMV, VEA, VWO, AGG, LQD, GLD
    std::vector<std::string> input_etf_list = { "SPY", "VTV" }; //User input to determine which/how many ETFs would be analyzed. Variable length
    double increment = .1;

    //Raw points, to be visible in graph 1.
    std::vector<CombNormal> rawpointset = raw_point_generator(AlphaFrame, input_etf_list, increment);

    //Processed points, to be visible in graph 2.
    std::vector<CombNormal> bestpointset = find_optimal_points(AlphaFrame, input_etf_list, increment); //ONLY WORKS WITH 2 ETFs RIGHT NOW 

    //areatest();
    system("pause");
    return 0;
}

std::vector<CombNormal> rawpoints(std::vector<std::string> input_etf_list, double increment) {
    DataFrame AlphaFrame = read_csv();
    
    return raw_point_generator(AlphaFrame, input_etf_list, increment);
}

std::vector<CombNormal> bestpoints(std::vector<std::string> input_etf_list, double increment) {
    DataFrame AlphaFrame = read_csv();

    return find_optimal_points(AlphaFrame, input_etf_list, increment);
}

CombNormal allocation(std::vector<CombNormal> frontier, double risk) {
    //Convert CombNormal to a format that risktopointalgo accepts
    std::vector<std::pair<double, double>> frontierpoints;

    for (CombNormal normal : frontier) {
        frontierpoints.push_back(std::make_pair(normal.stdev, normal.mean));
    }

    std::vector<std::pair<std::pair<double, double>, double> > areadata = cumareavector(frontierpoints);
    std::pair<double, double> coordinates = bestpoint(areadata, risk);

    //Find corresponding CombNormal
    for (CombNormal normal : frontier) {
        if (normal.stdev == coordinates.first && normal.mean == coordinates.second) return normal;
    }

    //Error if the function reaches this point
    return CombNormal(0, 0);
}