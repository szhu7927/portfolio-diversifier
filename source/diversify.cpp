#include ".\public\storedata.h"
#include ".\public\frontieralgo.h"
#include ".\public\risktopointalgo.h"

void areatest() {
    std::vector<std::pair<double, double>> vec;
    vec.push_back(std::make_pair(0, 0));
    vec.push_back(std::make_pair(0, .5));
    vec.push_back(std::make_pair(1, 1));
    vec.push_back(std::make_pair(1, 0));

    //Should be 0.75
    std::cout << area(vec) << "\n";

    std::vector<std::pair<double, double>> vec1;
    vec1.push_back(std::make_pair(0, 0));
    vec1.push_back(std::make_pair(1, 3));
    vec1.push_back(std::make_pair(7, 7));
    vec1.push_back(std::make_pair(2, 4));
    vec1.push_back(std::make_pair(6, 5));

    //Should be 6.5
    std::cout << totalarea(vec1) << "\n";
}

int main()
{
    DataFrame AlphaFrame = read_csv();
    //AlphaFrame.print();
    std::vector<std::string> input_etf_list = { "SPY", "VTV" }; //User input to determine which/how many ETFs would be analyzed. Variable length
    std::vector<CombNormal> bestpointset = find_optimal_points(AlphaFrame, input_etf_list); //ONLY WORKS WITH 2 ETFs RIGHT NOW 

    //areatest();

    system("pause");
}