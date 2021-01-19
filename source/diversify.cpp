#include ".\public\storedata.h"
#include ".\public\frontieralgo.h"
#include ".\public\risktopointalgo.h"

void areatest() {
    std::vector<std::pair<double, double>> vec;
    vec.push_back(std::make_pair(0, 0));
    vec.push_back(std::make_pair(0, .5));
    vec.push_back(std::make_pair(1, 1));
    vec.push_back(std::make_pair(1, 0));

    std::cout << "Area 1: " << area(vec) << " (Should be 0.75) \n";

    std::vector<std::pair<double, double>> vec1;
    vec1.push_back(std::make_pair(0, 0));
    vec1.push_back(std::make_pair(1, 3));
    vec1.push_back(std::make_pair(7, 7));
    vec1.push_back(std::make_pair(2, 4));
    vec1.push_back(std::make_pair(6, 5));

    std::cout << "Area 2: " << totalarea(vec1) << " (Should be 6.5) \n \n";

    std::vector<std::pair<std::pair<double, double>, double>> vec2 = cumareavector(vec1);
    printareas(vec2);

    std::cout << "\n";

    double risk = 0.7;
    std::pair<double, double> point = bestpoint(vec2, risk);

    std::cout << "For risk " << risk << ", the best point is (" << point.first << ", " << point.second << ") \n";
}

//The real main
int main()
{
    //uncomment the line below to see a test of the algorithm
    //fa_debug();
    DataFrame AlphaFrame = read_csv();
    //AlphaFrame.print();
    std::vector<std::string> input_etf_list = { "SPY", "VTV" }; //User input to determine which/how many ETFs would be analyzed. Variable length
    double increment = .1;
    //uncommenting the below line would theoretically compile but don't
    //std::vector<CombNormal> bestpointset = find_optimal_points(AlphaFrame, input_etf_list, increment); //ONLY WORKS WITH 2 ETFs RIGHT NOW 

    //areatest();
    system("pause");
    return 0;
}